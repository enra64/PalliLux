#include <FastLED.h>

#define NUM_LEDS 60*2+20*2
#define DATA_PIN 6

CRGBArray<NUM_LEDS> leds;

uint8_t comBuffer[64];

enum error {INCORRECT_OPEN_SEQUENCE, NO_DATA_FINISH_SYMBOL};

void show_error(error e){
    leds.fill_solid(CRGB::Black);
    switch(e){
        case INCORRECT_OPEN_SEQUENCE:
            leds[0] = CRGB::Red;
            break;
        case NO_DATA_FINISH_SYMBOL:
            leds[0] = CRGB::Blue;
            break;
    }
    while(1)
      FastLED.delay(2000);
}

void setup() {
    // add leds
    FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
    
    // begin serial communication
    Serial.begin(115200);

    // wait for start code
    while(!Serial.available());

    // check opening string
    if(!Serial.readString() == "hello"){
       show_error(INCORRECT_OPEN_SEQUENCE);
    }
    else {
        // respond
        Serial.write("SAM");
    }
}

void loop() {
  if(Serial.available()){
    Serial.readBytes((char*)leds.leds, NUM_LEDS * 3);
  }
  if(Serial.read() != 0)
    show_error(NO_DATA_FINISH_SYMBOL);
  
}
