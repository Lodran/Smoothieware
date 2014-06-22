//
//  Port.h
//
//	An abstract class which represents a group of GPIO pins.
//
//  Created by Ron Aldrich on 6/15/14.
//

#ifndef _Port_HPP
#define _Port_HPP

#include <vector>
#include <stdint.h>

// Pin configuration constants.

// Pin type (input or output).

#define PIN_TYPE_INPUT false
#define PIN_TYPE_OUTPUT true

// Open drain?

#define PIN_NORMAL false
#define PIN_OPENDRAIN true

// Pin attributes (pullup, pulldown, repeater)

#define PIN_PULLUP		0b00
#define PIN_REPEATER	0b01
#define PIN_PULLNONE	0b10
#define PIN_PULLDOWN	0b11

#include "libs/ADC/adc.h"

class Port
{
protected:
    static std::vector<Port*> __xports;
    
    static const uint16_t xport_checksum;
    static const uint16_t type_checksum;
    
    uint16_t _identifier;
    
    inline Port(uint16_t inIdentifier) :
    _identifier (inIdentifier)
    { }

    static uint16_t identifier_checksum(uint16_t inIdentifier);
    
public:
        
    // Return the port with the given identifier.
    //	If the identifier is that of an external port, its description
    //	must be available in config.txt at the time that this is called.
    
    static Port* with_identifier(uint16_t inIdentifier);
    
    inline uint16_t identifier()
    { return _identifier; }
    
    
    // Configure the set of pins defined by inPins as GPIO, with the given configuration.
    
    void configure_pins(uint32_t inPins, bool inOutput, bool inOpendrain, uint8_t inMode);
    
    virtual void configure_use(uint8_t inPin, bool inAsGPIO);

    virtual void configure_type(uint8_t inPin, bool inOutput);
    
    virtual void configure_opendrain(uint8_t inPin, bool inOpendrain);
    
    virtual void configure_mode(uint8_t inPin, uint8_t inMode);
    
    // Get the values of the group of pins.
    
    virtual uint32_t get_value() = 0;
    
    virtual void set_value(uint32_t inPins) = 0;
    
    virtual void clear_pins(uint32_t inPins) = 0;
    
    virtual void set_pins(uint32_t inPins = 0);
    
    virtual PinName mbed_pinname(uint8_t inPin);
    
    virtual void dump_pin(uint8_t inPin) = 0;
    
    inline bool get_pin(uint8_t inPin)
    { return (get_value() >> inPin) & 1; }
    
    inline void clear_pin(uint8_t inPin)
    { clear_pins(1 << inPin); }
    
    inline void set_pin(uint8_t inPin)
    { set_pins(1 << inPin); }
};

#endif
