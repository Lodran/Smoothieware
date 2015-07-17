//
//  ad5206.cpp

#include "ad5206.h"

#include <string>

#include "checksumm.h"
#include "ConfigValue.h"
#include "Config.h"

static const uint16_t ad5206_checksum = CHECKSUM("ad5206");
static const uint16_t mosi_checksum = CHECKSUM("mosi");
static const uint16_t miso_checksum = CHECKSUM("miso");
static const uint16_t sclk_checksum = CHECKSUM("sclk");
static const uint16_t cs_checksum = CHECKSUM("cs");
static const uint16_t factor_checksum = CHECKSUM("factor"); // Amps to wiper ratio.

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
    
    // 33k resistor, factor: 332.2727355957
    // 6490 resistor, factor: 127.4227294922
    // 4.7k resistor, factor: ?
    
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
        spi->frequency(250000);
        spi->write((int)channels[axis]);
        spi->write((int)current_to_wiper(current));
        cs.set(1);
        printf("%s %d (%d) %f (%d)\n", __PRETTY_FUNCTION__, axis, channels[axis], current, current_to_wiper(current));
   }
}

unsigned char AD5206::current_to_wiper( float current )
{
    int d = ceil(current * factor);
    return ((d < 0) ? 0 : (d > 255) ? 255 : d);
}

float AD5206::get_current(int channel)
{
    return currents[channel];
}
