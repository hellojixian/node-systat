// class ata_device
// : virtual public /*extends*/ smart_device
// {
// public:
//   /// ATA pass through.
//   /// Return false on error.
//   /// Must be implemented in derived class.
//   virtual bool ata_pass_through(const ata_cmd_in & in, ata_cmd_out & out) = 0;

//   /// ATA pass through without output registers.
//   /// Return false on error.
//   /// Calls ata_pass_through(in, dummy), cannot be reimplemented.
//   bool ata_pass_through(const ata_cmd_in & in);

//   /// Return true if OS caches ATA identify sector.
//   /// Default implementation returns false.
//   virtual bool ata_identify_is_cached() const;

// protected:
//   /// Flags for ata_cmd_is_supported().
//   enum {
//     supports_data_out = 0x01, // PIO DATA OUT
//     supports_smart_status = 0x02, // read output registers for SMART STATUS only
//     supports_output_regs = 0x04, // read output registers for all commands
//     supports_multi_sector = 0x08, // more than one sector (1 DRQ/sector variant)
//     supports_48bit_hi_null = 0x10, // 48-bit commands with null high bytes only
//     supports_48bit = 0x20, // all 48-bit commands
//   };

//   /// Check command input parameters.
//   /// Return false if required features are not implemented.
//   /// Calls set_err(...) accordingly.
//   bool ata_cmd_is_supported(const ata_cmd_in & in, unsigned flags,
//     const char * type = 0);

//   /// Check command input parameters (old version).
//   // TODO: Remove if no longer used.
//   bool ata_cmd_is_ok(const ata_cmd_in & in,
//     bool data_out_support = false,
//     bool multi_sector_support = false,
//     bool ata_48bit_support = false)
//     {
//       return ata_cmd_is_supported(in,
//         (data_out_support ? supports_data_out : 0) |
//         supports_output_regs |
//         (multi_sector_support ? supports_multi_sector : 0) |
//         (ata_48bit_support ? supports_48bit : 0));
//     }

//   /// Hide/unhide ATA interface.
//   void hide_ata(bool hide = true)
//     { m_ata_ptr = (!hide ? this : 0); }

//   /// Default constructor, registers device as ATA.
//   ata_device()
//     : smart_device(never_called)
//     { hide_ata(false); }
// };
