#ifndef AD5206_H
#define AD5206_H

#include "DigipotBase.h"
#include "spi.h"
#include "Pin.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))

class AD5206 : public DigipotBase
{
public:
    AD5206();
    
    void set_current( int axis, float current );
    
    
    //taken from 4pi firmware
    unsigned char current_to_wiper( float current );
    
    float get_current(int channel);
    
private:
    Pin cs;
    SPI* spi;
    float _tr;  // Digitpot's Terminal resistance.
    float _ar;  // A(n) resistance.
    float _sr;  // Current sense resistors.
    float _vss; // Board vss.

    uint8_t channels[6];
    float currents[6];
};


#endif
