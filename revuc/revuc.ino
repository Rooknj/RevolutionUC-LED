#include "FastLED.h"

//*******************************************************************************
// Config
//*******************************************************************************
#define NUM_LEDS 60                  // Number of LEDS in your strip
#define NUM_LEDS_HALF (NUM_LEDS / 2) // Half the number of LEDS in your strip
#define DATA_PIN 5         // The data pin for the LED strip (green wire)
#define buttonPin 6        // The pin hooked up to the push button
#define ANALOG_READ A5     // The pin used to read the output signal from the microphone
#define DC_OFFSET 0        // DC offset in mic signal - if unusure, leave 0
#define NOISE 10           // Expected level of Noise/hum/interference in mic signal
#define TOP (NUM_LEDS + 2) // The top of the led strip (used in height calculations) (is slightly off of the actual led strip)
#define SAMPLES 60         // Length of buffer for dynamic level adjustment
#define PEAK_FALL 40       // Rate of peak falling dot
#define SPEED .20          // Amount to increment RGB color by each cycle

// Globals
byte
    peak = 0,   // Used for falling dot
    height = 0; // Used for the body of the animation (current sound level)
float
    greenOffset = 30, // Starting green value for gradient changing animations
    blueOffset = 150; // Starting blue value for gradient changing animations

CRGB leds[NUM_LEDS]; // LED array
int mode = 1; // Current animation mode

//*******************************************************************************
// Setup
//*******************************************************************************
void setup()
{
  //Serial.begin(115200);
  // Add leds and set brightness
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  setRGB(0, 0, 0);   // Clear the strip
  pinMode(buttonPin, INPUT); // Set up the button pin to receive input
  analogReference(EXTERNAL); // Remove microphone signal interferece from power supply
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
    dance1();
    break;
  case 2:
    dance2();
    break;
  case 3:
    dance3();
    break;
  case 4:
    dance4();
    break;
  case 5:
    dance5();
    break;
  case 6:
    dance6();
    break;
  }
  handleButton();
}

//*******************************************************************************
// Helper Functions
//*******************************************************************************
void dance1()
{
  calculatePeakAndHeight(TOP);
  // Color pixels based on rainbow gradient
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i >= height)
      leds[i] = CRGB(0, 0, 0);
    else
      leds[i] = getColorFromGradient(map(i, 0, NUM_LEDS - 1, 30, 150));
  }
  // Draw peak dot
  if (peak > 0 && peak <= NUM_LEDS - 1)
    leds[peak] = getColorFromGradient(map(peak, 0, NUM_LEDS - 1, 30, 150));
  FastLED.show(); // Update strip
}

void dance2()
{
  calculatePeakAndHeight(TOP);

  greenOffset += SPEED;
  blueOffset += SPEED;
  if (greenOffset >= 255)
    greenOffset = 0;
  if (blueOffset >= 255)
    blueOffset = 0;

  // Color pixels based on rainbow gradient
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i >= height)
    {
      leds[i] = CRGB(0, 0, 0);
    }
    else
    {
      leds[i] = getColorFromGradient(
          map(i, 0, NUM_LEDS - 1, (int)greenOffset, (int)blueOffset));
    }
  }
  // Draw peak dot
  if (peak > 0 && peak <= NUM_LEDS - 1)
    leds[peak] = getColorFromGradient(map(peak, 0, NUM_LEDS - 1, 30, 150));

  FastLED.show(); // Update strip
}

void dance3()
{
  calculatePeakAndHeight(TOP / 2);
  for (int i = 0; i < NUM_LEDS_HALF; i++)
  {
    if (i >= height)
    {
      leds[NUM_LEDS_HALF - i - 1] = CRGB(0, 0, 0);
      leds[NUM_LEDS_HALF + i] = CRGB(0, 0, 0);
    }
    else
    {
      CRGB color = getColorFromGradient(map(i, 0, NUM_LEDS_HALF - 1, 30, 150));
      leds[NUM_LEDS_HALF - i - 1] = color;
      leds[NUM_LEDS_HALF + i] = color;
    }
  }

  // Draw peak dot
  if (peak > 0 && peak <= NUM_LEDS_HALF - 1)
  {
    CRGB color = getColorFromGradient(map(peak, 0, NUM_LEDS_HALF - 1, 30, 150));
    leds[NUM_LEDS_HALF - peak - 1] = color;
    leds[NUM_LEDS_HALF + peak] = color;
  }

  FastLED.show(); // Update strip
}

void dance4()
{
  calculatePeakAndHeight(TOP / 2);

  greenOffset += SPEED;
  blueOffset += SPEED;
  if (greenOffset >= 255)
    greenOffset = 0;
  if (blueOffset >= 255)
    blueOffset = 0;

  for (int i = 0; i < NUM_LEDS_HALF; i++)
  {
    if (i >= height)
    {
      leds[NUM_LEDS_HALF - i - 1] = CRGB(0, 0, 0);
      leds[NUM_LEDS_HALF + i] = CRGB(0, 0, 0);
    }
    else
    {
      CRGB color = getColorFromGradient(map(i, 0, NUM_LEDS_HALF - 1, (int)greenOffset, (int)blueOffset));
      leds[NUM_LEDS_HALF - i - 1] = color;
      leds[NUM_LEDS_HALF + i] = color;
    }
  }

  // Draw peak dot
  if (peak > 0 && peak <= NUM_LEDS_HALF - 1)
  {
    CRGB color = getColorFromGradient(map(peak, 0, NUM_LEDS_HALF - 1, (int)greenOffset, (int)blueOffset));
    leds[NUM_LEDS_HALF - peak - 1] = color;
    leds[NUM_LEDS_HALF + peak] = color;
  }

  FastLED.show(); // Update strip
}

void dance5()
{
  calculatePeakAndHeight(TOP / 2);
  for (int i = 0; i < NUM_LEDS_HALF; i++)
  {
    if (i >= height)
    {
      leds[NUM_LEDS - i - 1] = CRGB(0, 0, 0);
      leds[i] = CRGB(0, 0, 0);
    }
    else
    {
      CRGB color = getColorFromGradient(map(i, 0, NUM_LEDS_HALF - 1, 30, 150));
      leds[NUM_LEDS - i - 1] = color;
      leds[i] = color;
    }
  }

  // Draw peak dot
  if (peak > 0 && peak <= NUM_LEDS_HALF - 1)
  {
    CRGB color = getColorFromGradient(map(peak, 0, NUM_LEDS_HALF - 1, 30, 150));
    leds[NUM_LEDS - peak - 1] = color;
    leds[peak] = color;
  }

  FastLED.show(); // Update strip
}

void dance6()
{
  calculatePeakAndHeight(TOP / 2);

  greenOffset += SPEED;
  blueOffset += SPEED;
  if (greenOffset >= 255)
    greenOffset = 0;
  if (blueOffset >= 255)
    blueOffset = 0;

  for (int i = 0; i < NUM_LEDS_HALF; i++)
  {
    if (i >= height)
    {
      leds[NUM_LEDS - i - 1] = CRGB(0, 0, 0);
      leds[i] = CRGB(0, 0, 0);
    }
    else
    {
      CRGB color = getColorFromGradient(map(i, 0, NUM_LEDS_HALF - 1, (int)greenOffset, (int)blueOffset));
      leds[NUM_LEDS - i - 1] = color;
      leds[i] = color;
    }
  }

  // Draw peak dot
  if (peak > 0 && peak <= NUM_LEDS_HALF - 1)
  {
    CRGB color = getColorFromGradient(map(peak, 0, NUM_LEDS_HALF - 1, (int)greenOffset, (int)blueOffset));
    leds[NUM_LEDS - peak - 1] = color;
    leds[peak] = color;
  }

  FastLED.show(); // Update strip
}

void setRGB(uint8_t p_red, uint8_t p_green, uint8_t p_blue)
{
  CRGB newColor = CRGB(p_red, p_green, p_blue);
  fill_solid(leds, NUM_LEDS, newColor);
  FastLED.show();
}

// Returns a gradient from blends between red-green-blue (input: 0-255)
CRGB getColorFromGradient(byte gradientPosition)
{

  if (gradientPosition < 85)
  {
    return CRGB((gradientPosition * 3), (255 - gradientPosition * 3), 0);
  }
  else if (gradientPosition < 170)
  {
    gradientPosition -= 85;
    return CRGB((255 - gradientPosition * 3), 0, (gradientPosition * 3));
  }
  else
  {
    gradientPosition -= 170;
    return CRGB(0, (gradientPosition * 3), (255 - gradientPosition * 3));
  }
}

// Detects when the button is pressed and switches between modes
int reading;            // the current reading from the input pin
int previous = LOW;     // the previous reading from the input pin
long debounceStart = 0; // the last time the output pin was toggled
long debounce = 200;    // the debounce time, increase if the output flickers
void handleButton()
{
  reading = digitalRead(buttonPin);
  // Check if the input went from low to high, then debounce to prevent the button from accidently being clicked twice from bounce
  if (reading == HIGH && previous == LOW && millis() - debounceStart > debounce)
  {
    mode++;
    // If you go past the last mode, reset to 0 (the off state) and clear the strip
    if (mode > 6)
    {
      mode = 0;
      setRGB(0, 0, 0);
    }

    debounceStart = millis();
  }
  previous = reading;
}

// **************************************************************************************
// DO NOT EDIT BELOW THIS LINE: COMPLEX AUDIO PROCESSING getColorFromGradient
//  inspired by https://learn.adafruit.com/led-ampli-tie/overview
// **************************************************************************************
byte dotCount = 0; // Frame counter for delaying dot-falling speed
byte volCount = 0; // Frame counter for storing past volume data
int vol[SAMPLES];  // Collection of prior volume samples
int lvl = 10;      // Current "dampened" audio level
int minLvlAvg = 0; // For dynamic adjustment of graph low & high
int maxLvlAvg = 512;
void calculatePeakAndHeight(int top)
{
  uint16_t minLvl, maxLvl;
  int n;

  n = analogRead(ANALOG_READ);        // Raw reading from mic
  n = abs(n - 512 - DC_OFFSET);       // Center on zero
  n = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;         // "Dampened" reading (else looks twitchy)

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = top * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if (height < 0L)
    height = 0; // Height cant be less than 0 (0L just means (long)0)
  else if (height > top)
    height = top; // Height cant be greater than TOP
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
  for (int i = 1; i < SAMPLES; i++)
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
  if ((maxLvl - minLvl) < top)
    maxLvl = minLvl + top;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
}
