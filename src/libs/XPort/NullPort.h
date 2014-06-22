//
//  NullPort.h
//  Smoothie
//
//  Created by Ron Aldrich on 6/16/14.
//  Copyright (c) 2014 Ron Aldrich. All rights reserved.
//

#ifndef __Smoothie__NullPort__
#define __Smoothie__NullPort__

#include "Port.h"

class NullPort : public Port
{
    friend class Port;
    
private:
    typedef Port super;
    
    inline NullPort() :
    super(0xFFFF)
    { }

public:
    static NullPort nullport;
        
    // Get the values of the group of pins.
    
    virtual uint32_t get_value();
    
    virtual void set_value(uint32_t inPins);
    
    virtual void clear_pins(uint32_t inPins);
    
    virtual void set_pins(uint32_t inPins);

    virtual void dump_pin(uint8_t inPin);
};

#endif /* defined(__Smoothie__NullPort__) */
