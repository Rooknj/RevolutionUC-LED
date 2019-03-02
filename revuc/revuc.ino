//#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include "FastLED.h"

//*******************************************************************************
// Config
//*******************************************************************************
#define NUM_LEDS 20 // Number of LEDS in your strip
#define DATA_PIN 5 // Pin 5 on ESP8266
#define buttonPin 12     // Pin 6 on ESP8266
#define ANALOG_READ 17 //The pin that we read sensor values form
#define DELAY 1 //Arduino loop delay

#define MIC_LOW 500
#define MIC_HIGH 1024

// Globals
CRGB leds[NUM_LEDS]; // LED array
bool powerOn = false; // are leds On or Off

//*******************************************************************************
// Setup
//*******************************************************************************
void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  setRGB(0,0,0);
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
}

//*******************************************************************************
// Loop
//*******************************************************************************
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(17);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

   Serial.println(peakToPeak);
   handleToggle();
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

long lastShow = 0;
bool shouldShow()
{
  int updateThreshold = 1000/60;
  long now = millis();

  if (now - lastShow > updateThreshold)
  {
    lastShow = now;
    return true;
  }
  return false;
}

int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
long debounceStart = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
void handleToggle() {
  reading = digitalRead(buttonPin);
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - debounceStart > debounce) {
    if(powerOn){
      setRGB(0,0,0);
      powerOn = false;
    } else {
      setRGB(255,0,0);
      powerOn = true;
    }

    debounceStart = millis();    
  }
  previous = reading;
}

