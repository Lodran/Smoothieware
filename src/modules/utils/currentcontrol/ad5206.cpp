//
//  ad5206.cpp

#include "ad5206.h"

#include <string>

/*
 #include "libs/Kernel.h"
 #include "libs/utils.h"
 #include <libs/Pin.h>
 #include "spi.h"
 #include <math.h>
 */

static const uint16_t ad5206_checksum = CHECKSUM("ad5206");
static const uint16_t mosi_checksum = CHECKSUM("mosi");
static const uint16_t miso_checksum = CHECKSUM("miso");
static const uint16_t sclk_checksum = CHECKSUM("sclk");
static const uint16_t cs_checksum = CHECKSUM("cs");
static const uint16_t tr_checksum = CHECKSUM("tr");     // Digitpot's Terminal resistance.
static const uint16_t ar_checksum = CHECKSUM("ar");     // A(n) resistance.
static const uint16_t sr_checksum = CHECKSUM("sr");     // Current sense resistors.
static const uint16_t vss_checksum = CHECKSUM("vss");   // Board vss.

static const uint16_t alpha_channel_checksum = CHECKSUM("alpha_channel");
static const uint16_t beta_channel_checksum = CHECKSUM("beta_channel");
static const uint16_t gamma_channel_checksum = CHECKSUM("gamma_channel");
static const uint16_t delta_channel_checksum = CHECKSUM("delta_channel");
static const uint16_t epsilon_channel_checksum = CHECKSUM("epsilon_channel");
static const uint16_t zeta_channel_checksum = CHECKSUM("zeta_channel");

AD5206::AD5206()
{
    Pin theMOSI;
    Pin theMISO;
    Pin theSCLK;

    std::string theValue;
    
    theMOSI.from_string(THEKERNEL->config->value(ad5206_checksum,
                                                 mosi_checksum)->by_default("0.9")->as_string());
    
    theMISO.from_string(THEKERNEL->config->value(ad5206_checksum,
                                                 miso_checksum)->by_default("0.8")->as_string());
    
    theSCLK.from_string(THEKERNEL->config->value(ad5206_checksum,
                                                 sclk_checksum)->by_default("0.7")->as_string());
    
    this->spi = new SPI(theMOSI.mbed_pinname(),
                   theMISO.mbed_pinname(),
                   theSCLK.mbed_pinname());
    
    cs.from_string(THEKERNEL->config->value(ad5206_checksum,
                                            cs_checksum)->by_default("4.29")->as_string())->as_output();
    cs.set(1);
    
    this->_tr = THEKERNEL->config->value(ad5206_checksum,
                                         tr_checksum)->by_default("10000")->as_number();
    this->_ar = THEKERNEL->config->value(ad5206_checksum,
                                         ar_checksum)->by_default("6800")->as_number();
    this->_sr = THEKERNEL->config->value(ad5206_checksum,
                                         sr_checksum)->by_default(".05")->as_number();
    this->_vss = THEKERNEL->config->value(ad5206_checksum,
                                          vss_checksum)->by_default("3.3")->as_number();
    
    this->channels[0] = THEKERNEL->config->value(ad5206_checksum,
                                                 alpha_channel_checksum)->by_default(5)->as_number();
    this->channels[1] = THEKERNEL->config->value(ad5206_checksum,
                                                 beta_channel_checksum)->by_default(3)->as_number();
    this->channels[2] = THEKERNEL->config->value(ad5206_checksum,
                                                 gamma_channel_checksum)->by_default(1)->as_number();
    this->channels[3] = THEKERNEL->config->value(ad5206_checksum,
                                                 delta_channel_checksum)->by_default(0)->as_number();
    this->channels[4] = THEKERNEL->config->value(ad5206_checksum,
                                                 epsilon_channel_checksum)->by_default(2)->as_number();
    this->channels[5] = THEKERNEL->config->value(ad5206_checksum,
                                                 zeta_channel_checksum)->by_default(4)->as_number();
}

void AD5206::set_current( int axis, float current )
{
    if(axis<6){
        current = min( max( current, 0.0L ), 2.0L );
        currents[axis] = current;
        cs.set(0);
        spi->write((int)channels[axis]);
        spi->write((int)current_to_wiper(current));
        cs.set(1);
    }
}

unsigned char AD5206::current_to_wiper( float current )
{
    int d = ceil((current*(8*_sr))/(_vss/(_tr+_ar))/(_tr/255.0));
    return ((d < 0) ? 0 : (d > 255) ? 255 : d);
}

float AD5206::get_current(int channel)
{
    return currents[channel];
}
