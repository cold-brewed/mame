// license:BSD-3-Clause
// copyright-holders:Nigel Barnes
/*********************************************************************

    BBC Micro ROM slot emulation

*********************************************************************/

#ifndef MAME_BUS_BBC_ROM_SLOT_H
#define MAME_BUS_BBC_ROM_SLOT_H

#pragma once

#include "softlist_dev.h"


#define BBC_ROM_REGION_TAG ":cart:rom"

/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

// ======================> bbc_romslot_device

class device_bbc_rom_interface;

class bbc_romslot_device : public device_t,
							public device_image_interface,
							public device_slot_interface
{
public:
		// image-level overrides
	virtual image_init_result call_load() override;
	virtual void call_unload() override;
	virtual const software_list_loader &get_software_list_loader() const override { return rom_software_list_loader::instance(); }

	virtual iodevice_t image_type() const override { return IO_ROM; }
	virtual bool is_readable()  const override { return 1; }
	virtual bool is_writeable() const override { return 0; }
	virtual bool is_creatable() const override { return 0; }
	virtual bool must_be_loaded() const override { return 0; }
	virtual bool is_reset_on_load() const override { return 1; }
	virtual const char *image_interface() const override { return "bbc_rom"; }
	virtual const char *file_extensions() const override { return "rom,bin"; }

	// slot interface overrides
	virtual std::string get_default_card_software(get_default_card_software_hook &hook) const override;

	// reading and writing
	virtual DECLARE_READ8_MEMBER(read);
	virtual DECLARE_WRITE8_MEMBER(write);

	uint32_t get_rom_size();
	uint32_t get_slot_size() const { return m_slot_size; }

protected:
	// construction/destruction
	bbc_romslot_device(const machine_config &mconfig, device_type type, char const *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void device_start() override;

	uint32_t m_slot_size;

private:
	device_bbc_rom_interface*   m_cart;
};

// ======================> bbc_romslot16_device

class bbc_romslot16_device : public bbc_romslot_device
{
public:
	// construction/destruction
	template <typename T>
	bbc_romslot16_device(machine_config const &mconfig, char const *tag, device_t *owner, T &&slot_options, char const *default_option)
		: bbc_romslot16_device(mconfig, tag, owner)
	{
		option_reset();
		slot_options(*this);
		set_default_option(default_option);
		set_fixed(false);
		m_slot_size = 0x4000;
	}

	bbc_romslot16_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);
};

// ======================> bbc_romslot32_device

class bbc_romslot32_device : public bbc_romslot_device
{
public:
	// construction/destruction
	template <typename T>
	bbc_romslot32_device(machine_config const &mconfig, char const *tag, device_t *owner, T &&slot_options, char const *default_option)
		: bbc_romslot32_device(mconfig, tag, owner)
	{
		option_reset();
		slot_options(*this);
		set_default_option(default_option);
		set_fixed(false);
		m_slot_size = 0x8000;
	}

	bbc_romslot32_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);
};


// ======================> device_bbc_rom_interface

class device_bbc_rom_interface : public device_slot_card_interface
{
public:
	// construction/destruction
	virtual ~device_bbc_rom_interface();

	// reading and writing
	virtual DECLARE_READ8_MEMBER(read) { return 0xff; }
	virtual DECLARE_WRITE8_MEMBER(write) { m_device.logerror("unhandled ROM write to %04X = %02X\n", offset | 0x8000, data); }

	void rom_alloc(uint32_t size, const char *tag);
	void ram_alloc(uint32_t size);
	void nvram_alloc(uint32_t size);

	uint8_t* get_rom_base() { return m_rom; }
	virtual uint32_t get_rom_size() { return m_rom_size; }

	uint8_t* get_ram_base() { return &m_ram[0]; }
	uint32_t get_ram_size() { return m_ram.size(); }

	uint8_t* get_nvram_base() { return &m_nvram[0]; }
	uint32_t get_nvram_size() { return m_nvram.size(); }

protected:
	device_bbc_rom_interface(const machine_config &mconfig, device_t &device);

	bbc_romslot_device *m_slot;

private:
	// internal state
	uint8_t *m_rom;
	uint32_t m_rom_size;
	std::vector<uint8_t> m_ram;
	std::vector<uint8_t> m_nvram;
};


// device type definition
DECLARE_DEVICE_TYPE(BBC_ROMSLOT16, bbc_romslot16_device)
DECLARE_DEVICE_TYPE(BBC_ROMSLOT32, bbc_romslot32_device)

void bbc_rom_devices(device_slot_interface &device);

#endif // MAME_BUS_BBC_ROM_SLOT_H
