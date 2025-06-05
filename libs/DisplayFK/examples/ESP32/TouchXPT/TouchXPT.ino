#include <XPT2046.h>

#define TC_MISO 19
#define TC_MOSI 23
#define TC_SCLK 18
#define TC_CS   21

XPT2046 *xpt = new XPT2046(TC_SCLK, TC_MISO, TC_MOSI, TC_CS);

void setup(){
    Serial.begin(115200);
    delay(1000);
    SPIClass *meuspi = new SPIClass();
    meuspi->begin(TC_SCLK, TC_MISO, TC_MOSI);
    xpt->begin(meuspi);
}

void loop(){
    if(xpt->getInputBodmer()){
        Serial.printf("%i\t%i\t%i\n", xpt->x, xpt->y, xpt->z);
    }else{
        //Serial.println("No touch");
    }
    delay(100);
}