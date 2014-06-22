//
//  Port.cpp
//
//	An abstract class which represents a group of GPIO pins.
//
//  Created by Ron Aldrich on 6/15/14.
//

#include "Port.h"

#include <algorithm>    // std::find

#include "IPort.h"

#include "XPort_SPI.h"

#include "Module.h"
#include "ConfigValue.h"
#include "Config.h"
#include "libs/Kernel.h"
#include "checksumm.h"
#include "utils.h"

std::vector<Port*> Port::__xports;

const uint16_t Port::xport_checksum = CHECKSUM("xport");
const uint16_t Port::type_checksum = CHECKSUM("type");

/******************************************************************************/

uint16_t Port::identifier_checksum(uint16_t inIdentifier)
{
    char theIdentifier[6];
    snprintf(theIdentifier, sizeof(theIdentifier)-1, "%d", inIdentifier);
    return get_checksum(theIdentifier);
}

/******************************************************************************/

Port* Port::with_identifier(uint16_t inIdentifier)
{
    if (inIdentifier < 5)
        return &IPort::__iports[inIdentifier];
    
    std::vector<Port*>::iterator theIterator;
    
    theIterator = std::find_if(__xports.begin(), __xports.end(), [inIdentifier](Port* const& n){
        return n->_identifier == inIdentifier;
    });
    
    if (theIterator != __xports.end())
    {
        return *theIterator;
    }
    
    // Couldn't find a port with the given identifier.  If possible, create it.
    
    Port* thePort = &NullPort::nullport;
    
    if (inIdentifier < 5)
    {
        thePort = new IPort(inIdentifier);
	}
    else
    {
        // Must be an XPort - which kind?
        
        std::string theType = THEKERNEL->config->value(xport_checksum,
                                                       Port::identifier_checksum(inIdentifier),
                                                       type_checksum)->as_string();
       
        if (theType == "spi")
            thePort = new XPort_SPI(inIdentifier);
        /*
         else if (theType == "i2c")
         	thePort = new XPort_i2c(inIdentifier);
         */
    }
        
    if (thePort != &NullPort::nullport)
        __xports.push_back(thePort);
    
    return thePort;
}

/******************************************************************************/

void Port::configure_pins(uint32_t inPins, bool inOutput, bool inOpendrain, uint8_t inMode)
{
    for (uint8_t thePin = 0; thePin < 32; thePin++)
    {
        if (inPins & 1 << thePin)
        {
            this->configure_use(thePin, true);
            
            this->configure_type(thePin, inOutput);
            
            this->configure_opendrain(thePin, inOpendrain);
            
            this->configure_mode(thePin, inMode);
        }
    }
}

/******************************************************************************/

void Port::configure_use(uint8_t inPin, bool inAsGPIO)
{
}

/******************************************************************************/

void Port::configure_type(uint8_t inPin, bool inOutput)
{
}

/******************************************************************************/

void Port::configure_opendrain(uint8_t inPin, bool inOpendrain)
{
}

/******************************************************************************/

void Port::configure_mode(uint8_t inPin, uint8_t inMode)
{
}

/******************************************************************************/

PinName Port::mbed_pinname(uint8_t inPin)
{
    return NC;
}
