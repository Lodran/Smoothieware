//
//  IPort.h
//
//	Concrete implementataion of an internal port.
//
//  Created by Ron Aldrich on 6/15/14.
//

#ifndef _IPort_HPP
#define _IPort_HPP

#include "Port.h"

#include "libs/LPC17xx/sLPC17xx.h" // smoothed mbed.h lib

class IPort : public Port
{
    friend class Port;
    
private:
    typedef Port super;
    
    static IPort __iports[];
    
    static LPC_GPIO_TypeDef* const __gpios[];
    //static PinName const __pinNames[][32];
    
    inline IPort(uint16_t inIdentifier) :
    super(inIdentifier)
    { }
    
    /*
    void as_open_drain(uint8_t inPin);
    
    void not_open_drain(uint8_t inPin);
    
    void as_repeater(uint8_t inPin);
    
    void pull_none(uint8_t inPin);
    
    void pull_up(uint8_t inPin);
    
    void pull_down(uint8_t inPin);
     */
    
public:
    // Configure the set of pins defined by inPins with the given configuration.
    
    virtual void configure_use(uint8_t inPin, bool inAsGPIO);
    
    virtual void configure_type(uint8_t inPin, bool inOutput);
    
    virtual void configure_opendrain(uint8_t inPin, bool inOpendrain);
    
    virtual void configure_mode(uint8_t inPin, uint8_t inMode);

    // Get the values of the group of pins.
    
    virtual uint32_t get_value();
    
    virtual void set_value(uint32_t inPins);
    
    virtual void clear_pins(uint32_t inPins);
    
    virtual void set_pins(uint32_t inPins);

    virtual PinName mbed_pinname(uint8_t inPin);

    virtual void dump_pin(uint8_t inPin);
};

#endif /* defined(_IPort_HPP) */
