#include <FastLED.h>
#define NUM_LEDS 122
#define DATA_PIN 6
#define CLOCK_PIN 11
CRGB leds[NUM_LEDS];
int index = 0;

// 43,51 is fix
// 103 can be set to any id to identfiy this arduino
byte identifier[3] =  {43, 51, 103}; 

byte version[3] = {1,0,0};
byte data[10];

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  //FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, GBR>(leds, NUM_LEDS);
  //FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
   readStream(Serial);  
}

void readStream(Stream& stream) {  
  while(stream.available()) {
    data[index++] = stream.read();
    if (data[0] == 'c' && index>6) {
      lightsOn();
      index = 0;
      data[0] = 0;
    }
    
    if (data[0] == 'i') {
      serialPrintInfo(identifier, version, (byte)NUM_LEDS);
      index = 0;
      data[0] = 0;
    }
  }
}

void lightsOn() {
  for (byte i=data[1]; i<data[1]+data[2]; i++) {
    leds[i].r = data[3];
    leds[i].g = data[4];
    leds[i].b = data[5];
  }
  
  FastLED.setBrightness(data[6]);
  FastLED.show();

  Serial.flush();
}

void serialPrintInfo(byte identifier[3], byte version[3], byte ledCount) {
  byte buffer[7];
  buffer[0] = identifier[0];
  buffer[1] = identifier[1];
  buffer[2] = identifier[2];
  buffer[3] = version[0];
  buffer[4] = version[1];
  buffer[5] = version[2];
  buffer[6] = ledCount;
  Serial.write(buffer, 7);
  Serial.flush();
}
