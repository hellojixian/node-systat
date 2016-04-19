#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <sys/ata.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>
#include <stddef.h>
#include <paths.h>
#include <sys/utsname.h>

#define __attribute_format_printf(x, y)  __attribute__((format (printf, x, y)))

namespace os_freebsd {

#define SCSI_TIMEOUT_DEFAULT    20 

// ATA Specification Command Register Values (Commands)
#define ATA_CHECK_POWER_MODE            0xe5
#define ATA_IDENTIFY_DEVICE             0xec
#define ATA_IDENTIFY_PACKET_DEVICE      0xa1
#define ATA_IDLE                        0xe3
#define ATA_SMART_CMD                   0xb0
#define ATA_SECURITY_FREEZE_LOCK        0xf5
#define ATA_SET_FEATURES                0xef
#define ATA_STANDBY_IMMEDIATE           0xe0

// ATA Specification Feature Register Values (SMART Subcommands).
// Note that some are obsolete as of ATA-7.
#define ATA_SMART_READ_VALUES           0xd0
#define ATA_SMART_READ_THRESHOLDS       0xd1
#define ATA_SMART_AUTOSAVE              0xd2
#define ATA_SMART_SAVE                  0xd3
#define ATA_SMART_IMMEDIATE_OFFLINE     0xd4
#define ATA_SMART_READ_LOG_SECTOR       0xd5
#define ATA_SMART_WRITE_LOG_SECTOR      0xd6
#define ATA_SMART_WRITE_THRESHOLDS      0xd7
#define ATA_SMART_ENABLE                0xd8
#define ATA_SMART_DISABLE               0xd9
#define ATA_SMART_STATUS                0xda
// SFF 8035i Revision 2 Specification Feature Register Value (SMART
// Subcommand)
#define ATA_SMART_AUTO_OFFLINE          0xdb

// Maximum allowed number of SMART Attributes
#define NUMBER_ATA_SMART_ATTRIBUTES     30

#define ARGUSED(x) ((void)(x))

#define SMART_CYL_LOW  0x4F
#define SMART_CYL_HI   0xC2

#define NO_RETURN 0
#define BAD_SMART 1
#define NO_DISK_3WARE 2
#define BAD_KERNEL 3
#define MAX_MSG 3

// Utility function for printing warnings
void printwarning(int msgNo, const char* extra) {
  printf("msgNo: %d, msg: %s\n", msgNo, extra);
  return;
};

struct ata_smart_attribute {
  unsigned char id;
  // meaning of flag bits: see MACROS just below
  // WARNING: MISALIGNED!
  unsigned short flags; 
  unsigned char current;
  unsigned char worst;
  unsigned char raw[6];
  unsigned char reserv;
};

struct ata_smart_values {
  unsigned short int revnumber;
  struct ata_smart_attribute vendor_attributes [NUMBER_ATA_SMART_ATTRIBUTES];
  unsigned char offline_data_collection_status;
  unsigned char self_test_exec_status;  //IBM # segments for offline collection
  unsigned short int total_time_to_complete_off_line; // IBM different
  unsigned char vendor_specific_366; // Maxtor & IBM curent segment pointer
  unsigned char offline_data_collection_capability;
  unsigned short int smart_capability;
  unsigned char errorlog_capability;
  unsigned char vendor_specific_371;  // Maxtor, IBM: self-test failure checkpoint see below!
  unsigned char short_test_completion_time;
  unsigned char extend_test_completion_time_b; // If 0xff, use 16-bit value below
  unsigned char conveyance_test_completion_time;
  unsigned short extend_test_completion_time_w; // e04130r2, added to T13/1699-D Revision 1c, April 2005
  unsigned char reserved_377_385[9];
  unsigned char vendor_specific_386_510[125]; // Maxtor bytes 508-509 Attribute/Threshold Revision #
  unsigned char chksum;
};

typedef enum {
  // returns no data, just succeeds or fails
  ENABLE,
  DISABLE,
  AUTOSAVE,
  IMMEDIATE_OFFLINE,
  AUTO_OFFLINE,
  STATUS,       // just says if SMART is working or not
  STATUS_CHECK, // says if disk's SMART status is healthy, or failing
  // return 512 bytes of data:
  READ_VALUES,
  READ_THRESHOLDS,
  READ_LOG,
  IDENTIFY,
  PIDENTIFY,
  // returns 1 byte of data
  CHECK_POWER_MODE,
  // writes 512 bytes of data:
  WRITE_LOG
} smart_command_set;

class ata_register
{
public:
  ata_register()
    : m_val(0x00), m_is_set(false) { }

  ata_register & operator=(unsigned char x)
    { m_val = x; m_is_set = true; return * this; }

  unsigned char val() const
    { return m_val; }
  operator unsigned char() const
    { return m_val; }

  bool is_set() const
    { return m_is_set; }

private:
  unsigned char m_val; ///< Register value
  bool m_is_set; ///< true if set
};

/// ATA Input registers (for 28-bit commands)
struct ata_in_regs
{
  // ATA-6/7 register names  // ATA-3/4/5        // ATA-8
  ata_register features;     // features         // features
  ata_register sector_count; // sector count     // count
  ata_register lba_low;      // sector number    // ]
  ata_register lba_mid;      // cylinder low     // ] lba
  ata_register lba_high;     // cylinder high    // ]
  ata_register device;       // device/head      // device
  ata_register command;      // command          // command

  /// Return true if any register is set
  bool is_set() const
    { return (features.is_set() || sector_count.is_set()
      || lba_low.is_set() || lba_mid.is_set() || lba_high.is_set()
      || device.is_set() || command.is_set());                    }
};

/// ATA Output registers (for 28-bit commands)
struct ata_out_regs
{
  ata_register error;
  ata_register sector_count;
  ata_register lba_low;
  ata_register lba_mid;
  ata_register lba_high;
  ata_register device;
  ata_register status;

  /// Return true if any register is set
  bool is_set() const
    { return (error.is_set() || sector_count.is_set()
      || lba_low.is_set() || lba_mid.is_set() || lba_high.is_set()
      || device.is_set() || status.is_set());                      }
};



/// 16-bit alias to a 8-bit ATA register pair.
class ata_reg_alias_16
{
public:
  ata_reg_alias_16(ata_register & lo, ata_register & hi)
    : m_lo(lo), m_hi(hi) { }

  ata_reg_alias_16 & operator=(unsigned short x)
    { m_lo = (unsigned char) x;
      m_hi = (unsigned char)(x >> 8);
      return * this;                   }

  unsigned short val() const
    { return m_lo | (m_hi << 8); }
  operator unsigned short() const
    { return m_lo | (m_hi << 8); }

private:
  ata_register & m_lo, & m_hi;

  // References must not be copied.
  ata_reg_alias_16(const ata_reg_alias_16 &);
  void operator=(const ata_reg_alias_16 &);
};


/// 48-bit alias to six 8-bit ATA registers (for LBA).
class ata_reg_alias_48
{
public:
  ata_reg_alias_48(ata_register & ll, ata_register & lm, ata_register & lh,
                   ata_register & hl, ata_register & hm, ata_register & hh)
    : m_ll(ll), m_lm(lm), m_lh(lh),
      m_hl(hl), m_hm(hm), m_hh(hh)
    { }

  ata_reg_alias_48 & operator=(uint64_t x)
    {
      m_ll = (unsigned char) x;
      m_lm = (unsigned char)(x >>  8);
      m_lh = (unsigned char)(x >> 16);
      m_hl = (unsigned char)(x >> 24);
      m_hm = (unsigned char)(x >> 32);
      m_hh = (unsigned char)(x >> 40);
      return * this;
    }

  uint64_t val() const
    {
      return (   (unsigned)m_ll
              | ((unsigned)m_lm <<  8)
              | ((unsigned)m_lh << 16)
              | ((unsigned)m_hl << 24)
              | ((uint64_t)m_hm << 32)
              | ((uint64_t)m_hh << 40));
    }

  operator uint64_t() const
    { return val(); }

private:
  ata_register & m_ll, & m_lm, & m_lh,
               & m_hl, & m_hm, & m_hh;

  // References must not be copied.
  ata_reg_alias_48(const ata_reg_alias_48 &);
  void operator=(const ata_reg_alias_48 &);
};


struct ata_in_regs_48bit
: public ata_in_regs   // "most recently written" registers
{
  ata_in_regs prev;  ///< "previous content"

  // 16-bit aliases for above pair.
  ata_reg_alias_16 features_16;
  ata_reg_alias_16 sector_count_16;
  ata_reg_alias_16 lba_low_16;
  ata_reg_alias_16 lba_mid_16;
  ata_reg_alias_16 lba_high_16;

  // 48-bit alias to all 8-bit LBA registers.
  ata_reg_alias_48 lba_48;

  /// Return true if 48-bit command
  bool is_48bit_cmd() const
    { return prev.is_set(); }

  /// Return true if 48-bit command with any nonzero high byte
  bool is_real_48bit_cmd() const
    { return (   prev.features || prev.sector_count
              || prev.lba_low || prev.lba_mid || prev.lba_high); }

  ata_in_regs_48bit();
};


/// ATA Output registers for 48-bit commands
struct ata_out_regs_48bit
: public ata_out_regs   // read with HOB=0
{
  ata_out_regs prev;  ///< read with HOB=1

  // 16-bit aliases for above pair.
  ata_reg_alias_16 sector_count_16;
  ata_reg_alias_16 lba_low_16;
  ata_reg_alias_16 lba_mid_16;
  ata_reg_alias_16 lba_high_16;

  // 48-bit alias to all 8-bit LBA registers.
  ata_reg_alias_48 lba_48;

  ata_out_regs_48bit();
};


/// Flags for each ATA output register
struct ata_out_regs_flags
{
  bool error, sector_count, lba_low, lba_mid, lba_high, device, status;

  /// Return true if any flag is set.
  bool is_set() const
    { return (   error || sector_count || lba_low
              || lba_mid || lba_high || device || status); }

  /// Default constructor clears all flags.
  ata_out_regs_flags()
    : error(false), sector_count(false), lba_low(false), lba_mid(false),
      lba_high(false), device(false), status(false) { }
};

/// ATA pass through input parameters
struct ata_cmd_in
{
  ata_in_regs_48bit in_regs;  ///< Input registers
  ata_out_regs_flags out_needed; ///< True if output register value needed
  enum { no_data = 0, data_in, data_out } direction; ///< I/O direction
  void * buffer; ///< Pointer to data buffer
  unsigned size; ///< Size of buffer

  /// Prepare for 28-bit DATA IN command
  void set_data_in(void * buf, unsigned nsectors)
    {
      buffer = buf;
      in_regs.sector_count = nsectors;
      direction = data_in;
      size = nsectors * 512;
    }

  /// Prepare for 28-bit DATA OUT command
  void set_data_out(const void * buf, unsigned nsectors)
    {
      buffer = const_cast<void *>(buf);
      in_regs.sector_count = nsectors;
      direction = data_out;
      size = nsectors * 512;
    }

  /// Prepare for 48-bit DATA IN command
  void set_data_in_48bit(void * buf, unsigned nsectors)
    {
      buffer = buf;
      // Note: This also sets 'in_regs.is_48bit_cmd()'
      in_regs.sector_count_16 = nsectors;
      direction = data_in;
      size = nsectors * 512;
    }

  ata_cmd_in();
};


/// ATA pass through output parameters
struct ata_cmd_out
{
  ata_out_regs_48bit out_regs; ///< Output registers

  ata_cmd_out();
};


ata_in_regs_48bit::ata_in_regs_48bit()
: features_16(features, prev.features),
  sector_count_16(sector_count, prev.sector_count),
  lba_low_16(lba_low, prev.lba_low),
  lba_mid_16(lba_mid, prev.lba_mid),
  lba_high_16(lba_high, prev.lba_high),
  lba_48(     lba_low,      lba_mid,      lba_high,
         prev.lba_low, prev.lba_mid, prev.lba_high)
{
}

ata_out_regs_48bit::ata_out_regs_48bit()
: sector_count_16(sector_count, prev.sector_count),
  lba_low_16(lba_low, prev.lba_low),
  lba_mid_16(lba_mid, prev.lba_mid),
  lba_high_16(lba_high, prev.lba_high),
  lba_48(     lba_low,      lba_mid,      lba_high,
         prev.lba_low, prev.lba_mid, prev.lba_high)
{
}

ata_cmd_in::ata_cmd_in()
: direction(no_data),
  buffer(0),
  size(0)
{
}

ata_cmd_out::ata_cmd_out()
{
}



// This function computes the checksum of a single disk sector (512
// bytes).  Returns zero if checksum is OK, nonzero if the checksum is
// incorrect.  The size (512) is correct for all SMART structures.
unsigned char checksum(const void * data)
{
  unsigned char sum = 0;
  for (int i = 0; i < 512; i++)
    sum += ((const unsigned char *)data)[i];
  return sum;
}

// Compile time check of byte ordering
// (inline const function allows compiler to remove dead code)
inline bool isbigendian()
{
#ifdef WORDS_BIGENDIAN
  return true;
#else
  return false;
#endif
}


// swap two bytes.  Point to low address
void swap2(char *location){
  char tmp=*location;
  *location=*(location+1);
  *(location+1)=tmp;
  return;
}

// swap four bytes.  Point to low address
void swap4(char *location){
  char tmp=*location;
  *location=*(location+3);
  *(location+3)=tmp;
  swap2(location+1);
  return;
}

// swap eight bytes.  Points to low address
void swap8(char *location){
  char tmp=*location;
  *location=*(location+7);
  *(location+7)=tmp;
  tmp=*(location+1);
  *(location+1)=*(location+6);
  *(location+6)=tmp;
  swap4(location+2);
  return;
}

// Typesafe variants using overloading
inline void swapx(unsigned short * p)
  { swap2((char*)p); }
inline void swapx(unsigned int * p)
  { swap4((char*)p); }
inline void swapx(uint64_t * p)
  { swap8((char*)p); }


/////////////////////////////////////////////////////////////////////////////
/// Implement shared open/close routines with old functions.  
class ata_device
{
public:
  explicit ata_device()
    : m_fd(-1) { }

  virtual ~ata_device() throw();

  virtual bool is_open() const;

  virtual bool open();

  virtual bool close();

  struct device_info {
    device_info()
      { }
    device_info(const char * d_name, const char * d_type, const char * r_type)
      : dev_name(d_name), info_name(d_name),
        dev_type(d_type), req_type(r_type)
      { }

    std::string dev_name;  ///< Device (path)name
    std::string info_name; ///< Informal name
    std::string dev_type;  ///< Actual device type
    std::string req_type;  ///< Device type requested by user, empty if none
  };
  
  /// Error (number,message) pair
  struct error_info {
    explicit error_info(int n = 0)
      : no(n) { }
    error_info(int n, const char * m)
      : no(n), msg(m) { }
    void clear()
      { no = 0; msg.erase(); }

    int no;          ///< Error number
    std::string msg; ///< Error message
  };

  /// Get device info struct.
  const device_info & get_info() const
    { return m_info; }

  /// Get device (path)name.
  const char * get_dev_name() const
    { return m_info.dev_name.c_str(); }
  /// Get informal name.
  const char * get_info_name() const
    { return m_info.info_name.c_str(); }
  /// Get device type.
  const char * get_dev_type() const
    { return m_info.dev_type.c_str(); }
  /// Get type requested by user, empty if none.
  const char * get_req_type() const
    { return m_info.req_type.c_str(); }

  bool ata_pass_through(const ata_cmd_in & in, ata_cmd_out & out);

protected:
  /// Flags for ata_cmd_is_supported().
  enum {
    supports_data_out = 0x01, // PIO DATA OUT
    supports_smart_status = 0x02, // read output registers for SMART STATUS only
    supports_output_regs = 0x04, // read output registers for all commands
    supports_multi_sector = 0x08, // more than one sector (1 DRQ/sector variant)
    supports_48bit_hi_null = 0x10, // 48-bit commands with null high bytes only
    supports_48bit = 0x20, // all 48-bit commands
  };

  /// Return filedesc for derived classes.
  int get_fd() const
    { return m_fd; }

  void set_fd(int fd)
    { m_fd = fd; }

  int do_cmd( struct ata_ioc_request* request, bool is_48bit_cmd);  
  bool ata_cmd_is_supported(const ata_cmd_in & in, unsigned flags, const char * type = 0);
  
  bool ata_cmd_is_ok(const ata_cmd_in & in,
    bool data_out_support = false,
    bool multi_sector_support = false,
    bool ata_48bit_support = false)
    {
      return ata_cmd_is_supported(in,
        (data_out_support ? supports_data_out : 0) |
        supports_output_regs |
        (multi_sector_support ? supports_multi_sector : 0) |
        (ata_48bit_support ? supports_48bit : 0));
    }  

  bool set_err(int no, const char * msg, ...)
    __attribute_format_printf(3, 4);

  /// Set last error info struct.
  bool set_err(const error_info & err)
    { m_err = err; return false; }

  /// Clear last error info.
  void clear_err()
    { m_err.clear(); }

  /// Set last error number and default message.
  /// Message is retrieved from interface's get_msg_for_errno(no).
  bool set_err(int no);

private:
  int m_fd; ///< filedesc, -1 if not open.
  error_info m_err;
  device_info m_info;
};


ata_device::~ata_device() throw()
{
  if (m_fd >= 0)
    ata_device::ata_device::close();
}

bool ata_device::is_open() const
{
  return (m_fd >= 0);
}


bool ata_device::open()
{
  const char *dev = get_dev_name();
  if ((m_fd = ::open(dev,O_RDONLY))<0) {
    // set_err(errno);
    return false;
  }
  return true;
}

bool ata_device::close()
{
  int failed = 0;
  // close device, if open
  if (is_open())
    failed=::close(get_fd());

  set_fd(-1);

  if(failed) return false;
    else return true;
}


int ata_device::do_cmd( struct ata_ioc_request* request, bool is_48bit_cmd)
{
  int fd = get_fd(), ret;
  ARGUSED(is_48bit_cmd); // no support for 48 bit commands in the IOCATAREQUEST
  ret = ioctl(fd, IOCATAREQUEST, request);
  if (ret) set_err(errno);
  return ret;
}



bool ata_device::ata_pass_through(const ata_cmd_in & in, ata_cmd_out & out)
{
  bool ata_48bit = false; // no ata_48bit_support via IOCATAREQUEST
  if(!strcmp("atacam",get_dev_type())) // enable for atacam interface
    ata_48bit = true;

  if (!ata_cmd_is_ok(in,
    true,  // data_out_support
    true,  // multi_sector_support
    ata_48bit) 
    ) {
      set_err(ENOSYS, "48-bit ATA commands not implemented for legacy controllers");
      return false;
    }

  struct ata_ioc_request request;
  bzero(&request,sizeof(struct ata_ioc_request));

  request.timeout=SCSI_TIMEOUT_DEFAULT;
  request.u.ata.command=in.in_regs.command;
  request.u.ata.feature=in.in_regs.features;

  request.u.ata.count = in.in_regs.sector_count_16;
  request.u.ata.lba = in.in_regs.lba_48;

  switch (in.direction) {
    case ata_cmd_in::no_data:  
      request.flags=ATA_CMD_CONTROL;
      break;
    case ata_cmd_in::data_in:  
      request.flags=ATA_CMD_READ | ATA_CMD_CONTROL;
      request.data=(char *)in.buffer;
      request.count=in.size;
      break;
    case ata_cmd_in::data_out: 
      request.flags=ATA_CMD_WRITE | ATA_CMD_CONTROL;
      request.data=(char *)in.buffer;
      request.count=in.size;
      break;
    default:
      return set_err(ENOSYS);
  }

  clear_err();
  errno = 0;
  if (do_cmd(&request, in.in_regs.is_48bit_cmd()))
      return false;
  if (request.error)
      return set_err(EIO, "request failed, error code 0x%02x", request.error);

  out.out_regs.error = request.error;
  out.out_regs.sector_count_16 = request.u.ata.count;
  out.out_regs.lba_48 = request.u.ata.lba;


  // Command specific processing
  if (in.in_regs.command == ATA_SMART_CMD
       && in.in_regs.features == ATA_SMART_STATUS
       && in.out_needed.lba_high)
  {
    unsigned const char normal_lo=0x4f, normal_hi=0xc2;
    unsigned const char failed_lo=0xf4, failed_hi=0x2c;

    // Cyl low and Cyl high unchanged means "Good SMART status"
    if (!(out.out_regs.lba_mid==normal_lo && out.out_regs.lba_high==normal_hi)
    // These values mean "Bad SMART status"
        && !(out.out_regs.lba_mid==failed_lo && out.out_regs.lba_high==failed_hi))

    {
      // We haven't gotten output that makes sense; print out some debugging info
      char buf[512];
      snprintf(buf, sizeof(buf),
        "CMD=0x%02x\nFR =0x%02x\nNS =0x%02x\nSC =0x%02x\nCL =0x%02x\nCH =0x%02x\nRETURN =0x%04x\n",
        (int)request.u.ata.command,
        (int)request.u.ata.feature,
        (int)request.u.ata.count,
        (int)((request.u.ata.lba) & 0xff),
        (int)((request.u.ata.lba>>8) & 0xff),
        (int)((request.u.ata.lba>>16) & 0xff),
        (int)request.error);
      printwarning(BAD_SMART,buf);
      out.out_regs.lba_high = failed_hi; 
      out.out_regs.lba_mid = failed_lo;
    }
  }

  return true;
}



// This function provides the pretty-print reporting for SMART
// commands: it implements the various -r "reporting" options for ATA
// ioctls.
int smartcommandhandler(ata_device * device, smart_command_set command, int select, char *data){
  // TODO: Rework old stuff below
  // This conditional is true for commands that return data
  int getsdata=(command==PIDENTIFY || 
                command==IDENTIFY || 
                command==READ_LOG || 
                command==READ_THRESHOLDS || 
                command==READ_VALUES ||
                command==CHECK_POWER_MODE);

  int sendsdata=(command==WRITE_LOG);
  
  if ((getsdata || sendsdata) && !data){
    return -1;
  }
  
  // The reporting is cleaner, and we will find coding bugs faster, if
  // the commands that failed clearly return empty (zeroed) data
  // structures
  if (getsdata) {
    if (command==CHECK_POWER_MODE)
      data[0]=0;
    else
      memset(data, '\0', 512);
  }

  // now execute the command
  int retval = -1;
  
  ata_cmd_in in;
  // Set specific values
  switch (command) {      
    case READ_VALUES:
      in.in_regs.features = ATA_SMART_READ_VALUES;
      in.set_data_in(data, 1);
      break;      
    default:
      break;
  }

  ata_cmd_out out;
  bool ok = device->ata_pass_through(in, out);
  if(ok) retval = 0;
  return retval;
  
};

int ataReadSmartValues(ata_device * device, struct ata_smart_values *data){
  
  if (smartcommandhandler(device, READ_VALUES, 0, (char *)data)){
    return -1;
  }

  // compute checksum
  if (checksum(data))
    printf("SMART Attribute Data Structure");
  
  // swap endian order if needed
  if (isbigendian()){
    int i;
    swap2((char *)&(data->revnumber));
    swap2((char *)&(data->total_time_to_complete_off_line));
    swap2((char *)&(data->smart_capability));
    swapx(&data->extend_test_completion_time_w);
    for (i=0; i<NUMBER_ATA_SMART_ATTRIBUTES; i++){
      struct ata_smart_attribute *x=data->vendor_attributes+i;
      swap2((char *)&(x->flags));
    }
  }

  return 0;
}

}; //namespace