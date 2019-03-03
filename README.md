# Revolution UC: Chainsmokers Project

## Inspiration
I like music and wanted to learn more about how electronic equipment can take audio signals and analyze them.

## What it does
Makes an LED strip dance to music.

## How I built it
I attached a sound sensor and LED strip to an arduino. The sound sensor listens for music and produces an analog signal. This analog signal is sampled and processed by the arduino. Then it is converted into a height value corresponding to the intensity of the music and a peak value corresponding to the highest recent spike in intensity. I designed different animations based on these two vales. I then added a push button to toggle between the different modes.

## Challenges I ran into
1. The first controller i used didnt support the led control library i wanted to use so i had to switch to a different board.
2. When the microphone and led strip were powered off of the same power supply, the extra power consumed by the LED strip would cause interference in the analog signal produced by the microphone
3. The analog signal produced by the microphone is very complicated to understand and I had to try multiple different strategies to find patterns in the data it produced
4. The microphone picks up a lot of noise in the environment other than the music. I had to find a way to smooth out that noise and focus on the music playing instead.
5. The microphone can be placed anywhere in a room relative to the speakers playing music. I had to find a way to make the code adjust to different volume levels and still produce a similar animation on the LED strip.

## Accomplishments that I'm proud of
1. I was able to make an animation that accurately corresponds to music.
2. I was able to use a push button to toggle between
3. I made something really cool that I can show my friends

## What I learned
1. How to use an analog reference signal to prevent interference from a power supply
2. How to use a push button sensor in three different modes of operation
3. How to understand and process analog signals
4. How to detect and smooth out noise from a microphone

## What's next for Chainsmokers
I want to learn how to use Fast Fourier Transforms to further analyze audio signals and break them down into frequency bands to create more interesting animations. I also want to hook this project up to the internet to be able to toggle it on/off and between modes using the Internet of Things.
