#include "FastLED.h"

//*******************************************************************************
// Config
//*******************************************************************************
#define NUM_LEDS 60 // Number of LEDS in your strip
#define DATA_PIN 5  // Pin 5 on ESP8266
#define buttonPin 6
#define ANALOG_READ A5     //The pin that we read sensor values form
#define DC_OFFSET 0        // DC offset in mic signal - if unusure, leave 0
#define NOISE 10           // Noise/hum/interference in mic signal
#define TOP (NUM_LEDS + 2) // Allow dot to go slightly off scale
#define SAMPLES 60         // Length of buffer for dynamic level adjustment
#define PEAK_FALL 40       // Rate of peak falling dot

// Globals
byte
    peak = 0,        // Used for falling dot
    height = 0;      // Used for the body of the animation (current sound level)
CRGB leds[NUM_LEDS]; // LED array
int mode = 1;

//*******************************************************************************
// Setup
//*******************************************************************************
void setup()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  setRGB(255, 0, 0);
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  analogReference(EXTERNAL);
}

//*******************************************************************************
// Loop
//*******************************************************************************
void loop()
{
  switch (mode)
  {
  case 0:
    break;
  case 1:
    vu();
    break;
  }
  handleButton();
}

//*******************************************************************************
// Helper Functions
//*******************************************************************************
void vu()
{
  calculatePeakAndHeight();
  // Color pixels based on rainbow gradient
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i >= height)
      leds[i] = CRGB(0, 0, 0);
    else
      leds[i] = CRGB(255, 0, 0);
  }
  // Draw peak dot
  if (peak > 0 && peak <= NUM_LEDS - 1)
    leds[peak] = CRGB(0, 255, 0);
  FastLED.show(); // Update strip
}

void setRGB(uint8_t p_red, uint8_t p_green, uint8_t p_blue)
{
  CRGB newColor = CRGB(p_red, p_green, p_blue);
  fill_solid(leds, NUM_LEDS, newColor);
  FastLED.show();
}

int reading;            // the current reading from the input pin
int previous = LOW;     // the previous reading from the input pin
long debounceStart = 0; // the last time the output pin was toggled
long debounce = 200;    // the debounce time, increase if the output flickers
void handleButton()
{
  reading = digitalRead(buttonPin);
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - debounceStart > debounce)
  {
    mode++;
    if (mode > 1)
    {
      mode = 0;
      setRGB(0, 0, 0);
    }

    debounceStart = millis();
  }
  previous = reading;
}

// **************************************************************************************
// DO NOT EDIT BELOW THIS LINE: COMPLEX AUDIO PROCESSING CODE
// **************************************************************************************

byte dotCount = 0; // Frame counter for delaying dot-falling speed
byte volCount = 0; // Frame counter for storing past volume data
int vol[SAMPLES];  // Collection of prior volume samples
int lvl = 10;      // Current "dampened" audio level
int minLvlAvg = 0; // For dynamic adjustment of graph low & high
int maxLvlAvg = 512;
void calculatePeakAndHeight()
{
  uint8_t i;
  uint16_t minLvl, maxLvl;
  int n;

  n = analogRead(ANALOG_READ);        // Raw reading from mic
  n = abs(n - 512 - DC_OFFSET);       // Center on zero
  n = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;         // "Dampened" reading (else looks twitchy)

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if (height < 0L)
    height = 0; // Height cant be less than 0 (0L just means (long)0)
  else if (height > TOP)
    height = TOP; // Height cant be greater than TOP
  if (height > peak)
    peak = height; // Keep 'peak' dot at top

  // Every few frames, make the peak pixel drop by 1:

  if (++dotCount >= PEAK_FALL)
  { //fall rate

    if (peak > 0)
      peak--;
    dotCount = 0;
  }

  vol[volCount] = n; // Save sample for dynamic leveling
  if (++volCount >= SAMPLES)
    volCount = 0; // Advance/rollover sample counter

  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for (i = 1; i < SAMPLES; i++)
  {
    if (vol[i] < minLvl)
      minLvl = vol[i];
    else if (vol[i] > maxLvl)
      maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if ((maxLvl - minLvl) < TOP)
    maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
}
