//
//  XPort.h
//  Smoothie
//
//  Created by Ron Aldrich on 6/16/14.
//  Copyright (c) 2014 Ron Aldrich. All rights reserved.
//

#ifndef __Smoothie__XPort__
#define __Smoothie__XPort__

#include "Port.h"

class XPort : public Port
{
private:
    typedef Port super;
    
protected:
    uint32_t _value;

    inline XPort(uint16_t inIdentifier) :
    	super(inIdentifier),
    	_value(0)
    { }
    
    virtual void write() = 0;
    
public:
    virtual uint32_t get_value();
    
    virtual void set_value(uint32_t inPins);
    
    virtual void clear_pins(uint32_t inPins);
    
    virtual void set_pins(uint32_t inPins);
    
    virtual void dump_pin(uint8_t inPin);
};

#endif /* defined(__Smoothie__XPort__) */
