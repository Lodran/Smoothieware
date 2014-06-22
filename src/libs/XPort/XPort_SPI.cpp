//
//  XPort_SPI.cpp
//
//  Created by Ron Aldrich on 6/16/14.
//

#include "XPort_SPI.h"

#include "Module.h"
#include "ConfigValue.h"
#include "Config.h"
#include "libs/Kernel.h"
#include "checksumm.h"
#include "utils.h"

#include <string>

#include <mri.h>


const uint16_t XPort_SPI::mosi_checksum = CHECKSUM("mosi");
const uint16_t XPort_SPI::miso_checksum = CHECKSUM("miso");
const uint16_t XPort_SPI::sclk_checksum = CHECKSUM("sclk");
const uint16_t XPort_SPI::cs_checksum = CHECKSUM("cs");
const uint16_t XPort_SPI::latch_checksum = CHECKSUM("latch");
const uint16_t XPort_SPI::pincount_checksum = CHECKSUM("pincount");

XPort_SPI::XPort_SPI(uint16_t inIdentifier) :
    super(inIdentifier),
    _spi(0),
    _latch(),
    _pinCount(32)
{
    uint16_t identifier_checksum = Port::identifier_checksum(inIdentifier);
    
    std::string theType = THEKERNEL->config->value(xport_checksum,
                                                   identifier_checksum,
                                                   type_checksum)->as_string();
    
    // We need a manager to own the SPI objects.
    
    Pin theMOSI;
    Pin theMISO;
    Pin theSCLK;
    
    std::string theValue;
    
    theValue = THEKERNEL->config->value(xport_checksum,
                                        identifier_checksum,
                                        mosi_checksum)->by_default("nc")->as_string();
    theMOSI.from_string(theValue);

    theValue = THEKERNEL->config->value(xport_checksum,
                                        identifier_checksum,
                                        miso_checksum)->by_default("nc")->as_string();
    theMISO.from_string(theValue);

    theValue = THEKERNEL->config->value(xport_checksum,
                                        identifier_checksum,
                                        sclk_checksum)->by_default("nc")->as_string();
    theSCLK.from_string(theValue);
    
    _spi = new SPI(theMOSI.mbed_pinname(),
                   theMISO.mbed_pinname(),
                   theSCLK.mbed_pinname());
    
    _spi->frequency(2500000);

    // Turns out that chip select isn't a useful concept for SPI shift registers.
    //  Luckily, we can just latch data at the end of any SPI traffic that's intended
    //  for the shift register.
    //
    //  If there's a CS pin defined, just ground it.

    Pin theCS;

    theValue = THEKERNEL->config->value(xport_checksum,
                                        identifier_checksum,
                                        cs_checksum)->by_default("nc")->as_string();
    theCS.from_string(theValue)->as_output();
    
    theCS.set(0);
    
    theValue = THEKERNEL->config->value(xport_checksum,
                                        identifier_checksum,
                                        latch_checksum)->by_default("nc")->as_string();
    _latch.from_string(theValue)->as_output();
    
    _pinCount = THEKERNEL->config->value(xport_checksum,
                                         identifier_checksum,
                                         pincount_checksum)->by_default(32)->as_number();
}

void XPort_SPI::write()
{
    // Chip select isn't a useful concept for 74HC595 chips.  If you connect it to
    //  _enable, as you might think, it enables the output pins only while you're
    //  programming them.
    //
    // Instead, we let the device watch all SPI traffic, but only latch the data
    //  when we've written correct pin values.
    
    if (_pinCount > 24)
        _spi->write((uint8_t)(_value >> 24));
    
    if (_pinCount > 16)
        _spi->write((uint8_t)(_value >> 16));
    
    if (_pinCount > 8)
        _spi->write((uint8_t)(_value >> 8));
    
    _spi->write((uint8_t) _value);
    
    _latch.set(1);
    __NOP();
    __NOP();
    _latch.set(0);
}
