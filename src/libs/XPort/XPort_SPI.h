//
//  XPort_SPI.h
//
//  Concrete implementation of an (output only) expansion port based on the 74HC595 shift register.
//
//  Created by Ron Aldrich on 6/16/14.
//

#ifndef _XPort_SPI_HPP
#define _XPort_SPI_HPP

#include "XPort.h"
#include "Pin.h"

#include "spi.h"

class XPort_SPI : public XPort
{
    friend class Port;
    
private:
    typedef XPort super;
    
protected:
    static const uint16_t mosi_checksum;
    static const uint16_t miso_checksum;
    static const uint16_t sclk_checksum;
    static const uint16_t cs_checksum;
    static const uint16_t latch_checksum;
    static const uint16_t pincount_checksum;
    
    SPI* _spi;
    Pin _latch;
    uint8_t _pinCount;
    
    XPort_SPI(uint16_t inIdentifier);
    
    virtual void write();
};

#endif /* defined(_XPort_SPI_HPP) */
