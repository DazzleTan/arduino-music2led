#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    13
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    300
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

#define MIN         540
#define MAX         740
#define COLOR_MULTI 7
#define FULL_COLOR  255

void setup() {
  delay(3000); // 3 second delay for recovery

  Serial.begin(9600);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

int incomingAudio = 520;
int adjustedAudio = 0;
int lastAdjustedAudio = 0;

void loop()
{
  int max = 1;
  incomingAudio = analogRead(A0);//read input from A0
  
  if(incomingAudio < MIN) {
    adjustedAudio = lastAdjustedAudio;
  }
  else {
    adjustedAudio = incomingAudio - MIN;
  }
  turnOnRange(adjustedAudio);

  //fadeExtras(lastAdjustedAudio, adjustedAudio);
  fadeAll();
  
  lastAdjustedAudio = adjustedAudio;
  FastLED.show();
}

void turnOnRange(int max) {
  int setIndex = 0;
  int colorCounter = 0;
  for(int i = 0; i < max; i++) {
    if(setIndex == 0) {
      leds[i] = CRGB(FULL_COLOR, colorCounter * COLOR_MULTI, 0);
      colorCounter++;
    }
    if(setIndex == 1) {
      leds[i] = CRGB(FULL_COLOR - (colorCounter * COLOR_MULTI), FULL_COLOR, 0);
      colorCounter++;
    }
    if(setIndex == 2) {
      leds[i] = CRGB(0, FULL_COLOR, colorCounter * COLOR_MULTI);
      colorCounter++;
    }
    if(setIndex == 3) {
      leds[i] = CRGB(0, FULL_COLOR - (colorCounter * COLOR_MULTI), FULL_COLOR);
      colorCounter++;
    }
    if(setIndex == 4) {
      leds[i] = CRGB(colorCounter * COLOR_MULTI, 0, FULL_COLOR);
      colorCounter++;
    }
    if(setIndex == 5) {
      leds[i] = CRGB(colorCounter * COLOR_MULTI, 0, FULL_COLOR - (colorCounter * COLOR_MULTI));
      colorCounter++;
    }
    if(colorCounter > 34) {
      colorCounter = 0;
      setIndex++;
    }
  }
}

void clearExtras(int previous, int newest) {
  if(previous - newest > 0) {
    for(int i = previous; i > newest; i--) {
      leds[i] = CRGB::Black;
    }
  }
}

void fadeExtras(int previous, int newest) {
  if(previous - newest > 0) {
    for(int i = previous; i > newest; i--) {
      leds[i].subtractFromRGB(150);
    }
  }
}

void fadeAll() {
  for(int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i].subtractFromRGB(10);
  }
}
