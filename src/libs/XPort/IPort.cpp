//
//  IPort.cpp
//
//  Created by Ron Aldrich on 6/15/14.
//

#include "IPort.h"

#include <stdio.h>

/******************************************************************************/

IPort IPort::__iports[5] {0, 1, 2, 3, 4};

LPC_GPIO_TypeDef* const IPort::__gpios[5] = { LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4 };

/******************************************************************************/

void IPort::configure_use(uint8_t inPin, bool inAsGPIO)
{
    if (inPin >= 32)
        return;
    
    LPC_GPIO_TypeDef* theGPIO = __gpios[_identifier];
    
    if (inAsGPIO)
        theGPIO->FIOMASK &= ~(1<< inPin);
    else
        theGPIO->FIOMASK |= 1 << inPin;
}

/******************************************************************************/

void IPort::configure_type(uint8_t inPin, bool inOutput)
{
    if (inPin >= 32)
        return;
    
    LPC_GPIO_TypeDef* theGPIO = __gpios[_identifier];
    
    if (inOutput)
        theGPIO->FIODIR |= 1 << inPin;
    else
        theGPIO->FIODIR &= ~(1 << inPin);
}

/******************************************************************************/

void IPort::configure_opendrain(uint8_t inPin, bool inOpendrain)
{
    if (inPin > 32)
        return;
    
    volatile uint32_t* theOpenDrain = &LPC_PINCON->PINMODE_OD0;
    
    if (inOpendrain)
        theOpenDrain[_identifier] |= 1 << inPin;
    else
        theOpenDrain[_identifier] &= ~(1 << inPin);
}

/******************************************************************************/

void IPort::configure_mode(uint8_t inPin, uint8_t inMode)
{
    if (inPin > 32)
        return;
    
    volatile uint32_t* thePinModes = &LPC_PINCON->PINMODE0;
    
    int theIndex = (_identifier * 2) + (inPin >> 4);
    int theShift = (inPin & 0x0F) * 2;
    
    thePinModes[theIndex] = (thePinModes[theIndex] & ~(3 << theShift)) | (inMode << theShift);
}

/******************************************************************************/

uint32_t IPort::get_value()
{
    return __gpios[_identifier]->FIOPIN;
}

/******************************************************************************/

void IPort::set_value(uint32_t inPins)
{
    __gpios[_identifier]->FIOCLR = ~inPins;
    __gpios[_identifier]->FIOSET = inPins;
}

/******************************************************************************/

void IPort::clear_pins(uint32_t inPins)
{
    __gpios[_identifier]->FIOCLR = inPins;
}

/******************************************************************************/

void IPort::set_pins(uint32_t inPins)
{
    __gpios[_identifier]->FIOSET = inPins;
}

/******************************************************************************/

// Convert a smoothie Pin into a mBed Pin
PinName IPort::mbed_pinname(uint8_t inPin)
{
    return ((inPin > 32) ?
            NC :
            (PinName) (inPin | (_identifier << PORT_SHIFT)));
    
    // return __pinNames[_identifier][inPin];
}

/******************************************************************************/

void IPort::dump_pin(uint8_t inPin)
{
    printf("%d.%d", _identifier, inPin);
    
    volatile uint32_t* theOpenDrain = &LPC_PINCON->PINMODE_OD0;

    if (theOpenDrain[_identifier] & (1 << inPin))
        printf("o");
    
    volatile uint32_t* thePinModes = &LPC_PINCON->PINMODE0;
    
    int theIndex = (_identifier * 2) + (inPin >> 4);
    int theShift = (inPin & 0x0F) * 2;
    
    switch ((thePinModes[theIndex] >> theShift) & 0x03)
    {
        case PIN_PULLUP:
            printf("^");
            break;
        case PIN_REPEATER:
            printf("@");
            break;
        case PIN_PULLNONE:
            printf("-");
            break;
        default:
            printf("v");
            break;
    }
    
   LPC_GPIO_TypeDef* theGPIO = __gpios[_identifier];
    
    if (theGPIO->FIODIR & (1 << inPin))
    {
        printf(" -> %d\n", this->get_pin(inPin));
    }
    else
    {
        printf(" <- %d\n", this->get_pin(inPin));
    }
}

/******************************************************************************/
