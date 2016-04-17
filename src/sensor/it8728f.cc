/**
 * reference https://github.com/groeck/it87
 *
 * Operating Conditions
 * 3VSB/SYS_3VSB/AVCC3 ....................3.3V ± 0.15V 
 * VBAT....................................2.3V to 3.0V 
 * Operation Temperature (Topt)............0°C to +70°C
 */


#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include "sensor.h"

#ifdef __FreeBSD__
#include <machine/cpufunc.h>

#define	BIT(__n) (((__n) == 32) ? 0 : ((uint32_t)1 << (__n)))

#define	REG_2E	0x2e	/* The register to read/write */
#define	REG_4E	0x4e	/* Secondary register to read/write */


#define	DEV	0x07	/* Register: Logical device select */
#define PME	0x04	/* The device with the fan registers in it */


#define	DEVID	0x20	/* Register: Device ID */
#define	DEVREV	0x22	/* Register: Device Revision */

/* Length of ISA address segment */
#define IT87_EXTENT 8

/* Length of ISA address segment for Environmental Controller */
#define IT87_EC_EXTENT 2

/* Offset of EC registers from ISA base address */
#define IT87_EC_OFFSET 5

/* Where are the ISA address/data registers relative to the EC base address */
#define IT87_ADDR_REG_OFFSET 0
#define IT87_DATA_REG_OFFSET 1

/* Logical device 4 registers */
#define IT8712F_DEVID 0x8712
#define IT8705F_DEVID 0x8705
#define IT8716F_DEVID 0x8716
#define IT8718F_DEVID 0x8718
#define IT8720F_DEVID 0x8720
#define IT8721F_DEVID 0x8721
#define IT8726F_DEVID 0x8726
#define IT8728F_DEVID 0x8728
#define IT8732F_DEVID 0x8732
#define IT8771E_DEVID 0x8771
#define IT8772E_DEVID 0x8772
#define IT8781F_DEVID 0x8781
#define IT8782F_DEVID 0x8782
#define IT8783E_DEVID 0x8783
#define IT8786E_DEVID 0x8786
#define IT8790E_DEVID 0x8790
#define IT8603E_DEVID 0x8603
#define IT8620E_DEVID 0x8620
#define IT8623E_DEVID 0x8623
#define IT8628E_DEVID 0x8628
#define IT87_ACT_REG  0x30
#define IT87_BASE_REG 0x60


#define FAN_FROM_REG(val) ((val) == 0 ? -1 : (val) == 255 ? 0 : \
				1350000 / ((val) * 2))

#define VAL_FROM_REG(val) ((val) * 2 * 12  )


static const uint8_t IT87_REG_FAN[]  = { 0x0d, 0x0e, 0x0f };
static const uint8_t IT87_REG_TEMP[] = { 0x29, 0x2a, 0x2b };
static const uint8_t IT87_REG_VIN[]	= { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28 };


int fd;
int ioreg = REG_2E;

static inline int superio_inb(int reg)
{
	outb(ioreg,reg);
	return inb(ioreg + 1);
}

static inline int superio_inw(int reg)
{
	int val;
	outb(ioreg + IT87_ADDR_REG_OFFSET,reg++);
	val = inb(ioreg + IT87_DATA_REG_OFFSET) << 8;	
	outb(ioreg + IT87_ADDR_REG_OFFSET, reg);
	val |= inb(ioreg + IT87_DATA_REG_OFFSET);	
	return val;
}

static inline void superio_select(int ldn)
{
	outb(ioreg, DEV );
	outb(ioreg + 1, ldn);
}


static int superio_enter()
{
	fd = open("/dev/io", O_RDWR);
	if (fd < 0) {printf("open /dev/io for r/w mode error");return fd;}
	outb(ioreg, 0x87);
	outb(ioreg, 0x01);
	outb(ioreg, 0x55);
	outb(ioreg, ioreg == REG_4E ? 0xaa : 0x55 );
	return 0;
}

static int superio_exit()
{
	outb(ioreg, 0x02);
	outb(ioreg + 1, 0x02);	
	close(fd);
	return 0;
}

static int it87_read_value(u_int addr, u_int reg)
{
	outb(addr + IT87_ADDR_REG_OFFSET,reg);	
	return inb(addr + IT87_DATA_REG_OFFSET);
	
}

static void it87_write_value(u_int addr, u_int reg, u_int value)
{
	outb( addr + IT87_ADDR_REG_OFFSET ,reg);	
	outb( addr + IT87_DATA_REG_OFFSET , value);
}

void test()
{
	//test to get fan speeds
	std::vector<int> fans = sensor::get_fan_speeds();
	//debug output
	for (std::vector<int>::iterator it = fans.begin() ; it != fans.end(); ++it){
		std::cout << "FAN Speed " 
					<< std::distance(fans.begin(),it) <<" : "  
					<< *it << std::endl;	
	}
    


    //test to get system temperatures
	std::vector<int> temperatures = sensor::get_system_tempertures();	
	//debug output
	for (std::vector<int>::iterator it = temperatures.begin() ; it != temperatures.end(); ++it){
		std::cout << "System Temp "
					<< std::distance(temperatures.begin(),it) <<" : " 
		 			<< *it << std::endl;		
	}



	//test to get system voltages
	std::vector<int> volts = sensor::get_voltages();
	//debug output
	for (std::vector<int>::iterator it = volts.begin() ; it != volts.end(); ++it){
		std::cout << "System Voltage "
					<< std::distance(volts.begin(),it) <<" : " 
		 			<< *it << std::endl;		
	}

}

int main(int argc, char **argv)
{
	if(!check_chip()) return -1;	
	// test();	
	return 0;
}

#endif

namespace shadowgrid { 

	std::vector<int> sensor::get_fan_speeds()
	{
		std::vector<int> result; 	
	#ifdef __FreeBSD__		
		superio_enter();
		superio_select(PME);
		superio_inb(IT87_ACT_REG);
		int address = superio_inw(IT87_BASE_REG);
		int val;	

		for(int i = 0; i < (int)sizeof(IT87_REG_FAN) ; i++)
		{		
			val = it87_read_value(address + IT87_EC_OFFSET, IT87_REG_FAN[i]) ;
			result.push_back( FAN_FROM_REG(val) );
		}

		superio_exit();	
	#endif		
		return result;
	}

	std::vector<int> sensor::get_system_tempertures()
	{
		std::vector<int> result; 	
	#ifdef __FreeBSD__	
		superio_enter();
		superio_select(PME);
		superio_inb(IT87_ACT_REG);
		int address = superio_inw(IT87_BASE_REG);
		int val;	
		
		for(int i = 0; i < (int)sizeof(IT87_REG_TEMP) ; i++)
		{
			val = it87_read_value(address + IT87_EC_OFFSET, IT87_REG_TEMP[i]) ;		
			result.push_back( val );
		}

		superio_exit();
	#endif		
		return result;
	}

	std::vector<int> sensor::get_voltages()
	{
		std::vector<int> result; 
	#ifdef __FreeBSD__
		superio_enter();
		superio_select(PME);
		superio_inb(IT87_ACT_REG);
		int address = superio_inw(IT87_BASE_REG);
		int val;
		
		
		for(int i = 0; i < (int)sizeof(IT87_REG_VIN) ; i++)
		{
			val = it87_read_value(address + IT87_EC_OFFSET, IT87_REG_VIN[i]) ;		
			result.push_back( VAL_FROM_REG(val) );
		}

		superio_exit();
	#endif	
		return result;
	}




	int sensor::check_chip()
	{
	#ifndef __FreeBSD__
		return 0;
	#endif
	#ifdef __FreeBSD__		
		superio_enter();
		superio_select(PME);

		int chip_type = superio_inw(DEVID);	
		if(chip_type != IT8728F_DEVID) return 0; //may not support
		superio_exit();

		return 1;
	#endif		
	}

}
