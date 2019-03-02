#define FASTLED_INTERNAL
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include "FastLED.h"

//*******************************************************************************
// Config
//*******************************************************************************
#define NUM_LEDS 2 // Number of LEDS in your strip
#define DATA_PIN 5 // Pin 5 on ESP8266
#define buttonPin 12     // Pin 6 on ESP8266
#define DELAY 1 //Arduino loop delay

// Globals
CRGB leds[NUM_LEDS]; // LED array
int buttonState = 0;         // variable for reading the pushbutton status
bool ledState = false; // are leds On or Off

//*******************************************************************************
// Setup
//*******************************************************************************
void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  setRGB(255,0,0);
  //Serial.begin(115200);
  //Serial.println("LOW");
  pinMode(buttonPin, INPUT);
}

//*******************************************************************************
// Loop
//*******************************************************************************
void loop() { 
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // turn LED on:
    leds[1] = CRGB::Green;
    FastLED.show();
  } else {
    // turn LED off:
    leds[1] = CRGB::Black;
    FastLED.show();
  }

  if(shouldUpdate()) {
    if(ledState){
      leds[0] = CRGB::Black;
      FastLED.show();
      ledState = false;
    } else {
      leds[0] = CRGB::Red;
      FastLED.show();
      ledState = true;
    }
  }
  delay(DELAY);       // delay in between reads for stability
}

//*******************************************************************************
// Helper Functions
//*******************************************************************************
void setRGB(uint8_t p_red, uint8_t p_green, uint8_t p_blue)
{
  CRGB newColor = CRGB(p_red, p_green, p_blue);
  fill_solid(leds, NUM_LEDS, newColor);
  FastLED.show();
}

long lastUpdate = 0;
bool shouldUpdate()
{
  int updateThreshold = 500;
  long now = millis();

  if (now - lastUpdate > updateThreshold)
  {
    lastUpdate = now;
    return true;
  }
  return false;
}
