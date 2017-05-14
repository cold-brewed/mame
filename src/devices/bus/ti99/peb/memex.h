// license:LGPL-2.1+
// copyright-holders:Michael Zapf
/****************************************************************************

    Geneve "Memex" memory expansion
    See memex.c for documentation

    Michael Zapf, February 2011
    February 2012: Rewritten as class

*****************************************************************************/

#ifndef MAME_BUS_TI99_PEB_MEMEX_H
#define MAME_BUS_TI99_PEB_MEMEX_H

#pragma once

#include "peribox.h"
#include "machine/ram.h"

DECLARE_DEVICE_TYPE(TI99_MEMEX, geneve_memex_device)

class geneve_memex_device : public ti_expansion_card_device
{
public:
	geneve_memex_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	DECLARE_READ8Z_MEMBER(readz) override;
	DECLARE_WRITE8_MEMBER(write) override;

	DECLARE_READ8Z_MEMBER(crureadz) override { }
	DECLARE_WRITE8_MEMBER(cruwrite) override { }

protected:
	void device_start() override;
	void device_reset() override;
	machine_config_constructor device_mconfig_additions() const override;
	virtual ioport_constructor device_input_ports() const override;

private:
	bool    access_enabled(offs_t offset);
	required_device<ram_device> m_ram;
	uint8_t   m_switches;
};

#endif // MAME_BUS_TI99_PEB_MEMEX_H