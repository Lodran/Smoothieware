//
//  XPort.cpp
//  Smoothie
//
//  Created by Ron Aldrich on 6/16/14.
//  Copyright (c) 2014 Ron Aldrich. All rights reserved.
//

#include "XPort.h"

#include <stdio.h>

uint32_t XPort::get_value()
{
    return _value;
}

void XPort::set_value(uint32_t inPins)
{
    this->_value = inPins;
    
    this->write();
}

void XPort::clear_pins(uint32_t inPins)
{
    this->_value &= ~inPins;
    
    this->write();
}

void XPort::set_pins(uint32_t inPins)
{
    this->_value |= inPins;
    
    this->write();
}

void XPort::dump_pin(uint8_t inPin)
{
    printf("%d.%d -> %d\n", _identifier, inPin, this->get_pin(inPin));
}
