#include "Pin.h"

#include "utils.h"

#include "NullPort.h"

// Make a new pin object from a string
Pin* Pin::from_string(std::string value)
{
    // printf("%s %s\n", __PRETTY_FUNCTION__, value.c_str());
    
    // cs is the current position in the string
    const char* cs = value.c_str();
    // cn is the position of the next char after the number we just read
    char* cn = NULL;
    
    // grab first integer as port identifier. pointer to first non-digit goes in cn
    uint16_t theIdentifier = strtol(cs, &cn, 10);
    // if cn > cs then strtol read at least one digit
    if (cn > cs)
    {
        // translate port identifier into something useful
        this->_port = Port::with_identifier(theIdentifier);
                
        // if the char after the first integer is a . then we should expect a pin index next
        if (*cn == '.')
        {
            // move pointer to first digit (hopefully) of pin index
            cs = ++cn;
            
            // grab pin index.
            this->pin(strtol(cs, &cn, 10));
            
            // if strtol read some numbers, cn will point to the first non-digit
            if ((cn > cs) && (this->pin() < 32))
            {
                this->_port->configure_use(this->pin(), true);
                                
                // now check for modifiers:-
                // ! = invert pin
                // o = set pin to open drain
                // ^ = set pin to pull up
                // v = set pin to pull down
                // - = set pin to no pull up or down
                // @ = set pin to repeater mode
                for (;*cn;cn++) {
                    switch(*cn) {
                        case '!':
                            this->inverting(true);
                            break;
                        case 'o':
                            this->as_open_drain();
                            break;
                        case '^':
                            this->pull_up();
                            break;
                        case 'v':
                            this->pull_down();
                            break;
                        case '-':
                            this->pull_none();
                            break;
                        case '@':
                            this->as_repeater();
                            break;
                        default:
                            // skip any whitespace following the pin index
                            if (!is_whitespace(*cn))
                                return this;
                    }
                }
                return this;
            }
        }
    }
    
    // from_string failed. TODO: some sort of error
    _port = &NullPort::nullport;
    _pin = 0x7F;
    return this;
}