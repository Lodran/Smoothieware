//
//  NullPort.cpp
//  Smoothie
//
//  Created by Ron Aldrich on 6/16/14.
//  Copyright (c) 2014 Ron Aldrich. All rights reserved.
//

#include "NullPort.h"

#include <stdio.h>

NullPort NullPort::nullport;

// Get the values of the group of pins.

uint32_t NullPort::get_value()
{
    return 0;
}

void NullPort::set_value(uint32_t inPins)
{
}

void NullPort::clear_pins(uint32_t inPins)
{
}

void NullPort::set_pins(uint32_t inPins)
{
}

void NullPort::dump_pin(uint8_t inPin)
{
    printf("%d.%d N/C\n", _identifier, inPin);
}
