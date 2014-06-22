#ifndef PIN_H
#define PIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Port.h"
#include "NullPort.h"

class Pin
{
protected:
    Port* _port;
    uint8_t _pin;
    
public:
    Pin() :
    _port(&NullPort::nullport),
    _pin(0x7F)
    { }
    
    Pin* from_string(std::string inValue);
    
    bool connected()
    { return (_port != &NullPort::nullport) && this->pin() != 0x7F; }
    
    inline Pin* as_output()
    { this->_port->configure_type(this->pin(), true); return this; }
    
    inline Pin* as_input()
    { this->_port->configure_type(this->pin(), false); return this; }
    
    inline Pin* as_open_drain()
    { this->_port->configure_opendrain(this->pin(), true); return this; }
    
    inline Pin* as_repeater()
    { this->_port->configure_mode(this->pin(), PIN_REPEATER); return this; }
    
    inline Pin* pull_up()
    { this->_port->configure_mode(this->pin(), PIN_PULLUP); return this; }
    
    inline Pin* pull_down()
    { this->_port->configure_mode(this->pin(), PIN_PULLDOWN); return this; }
    
    inline Pin* pull_none()
    { this->_port->configure_mode(this->pin(), PIN_PULLNONE); return this; }
    
    inline uint8_t pin()
    { return _pin & 0x7F; }
    
    inline void pin(uint8_t inValue)
    { _pin = (_pin & 0x80) | (inValue & 0x7F); }
    
    inline bool inverting()
    { return _pin >= 0x80; }
    
    inline void inverting(bool inValue)
    { _pin = (_pin & 0x7F) | (inValue << 7); }
    
    inline bool get()
    {
        if (this->connected())
            return this->_port->get_pin(this->pin()) ^ this->inverting();
        else
            return false;
    }
    
    inline void set(bool inValue)
    {
        if (this->connected())
        {
            if (inValue ^ this->inverting())
                this->_port->set_pin(this->pin());
            else
                this->_port->clear_pin(this->pin());
        }
    }
    
    inline uint16_t port_identifier()
    { return this->_port->identifier(); }
    
    inline PinName mbed_pinname()
    { return this->_port->mbed_pinname(this->pin()); }
    
    inline void dump(std::string inName)
    { printf("%s ", inName.c_str()); this->_port->dump_pin(this->pin()); }
};

#endif /* defined(PIN_H) */
