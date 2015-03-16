#include <Adafruit_NeoPixel.h>
#include <math.h>

const int timeLit = 500;
const int threshold = 3;
const int increment = 5;

const int branchOnePin = 2;
const int branchOneNum = 60;
int branchOneTimes [branchOneNum];

uint32_t fade(int index, int time, int maxtime, int startcolor, int endcolor, int threshold) {
  if(time < threshold*index) {
    time = 0;
  }
  else if(time < threshold*index + maxtime) {
    time = time - threshold*index;
  }
  else {
    time = maxtime;
  }
  
  if(time < maxtime/2) {
    return map(time, 0, maxtime, startcolor, endcolor);
  }
  else {
    return map(time, floor(maxtime/2), maxtime, endcolor, startcolor);
  }
}

void updateLights(Adafruit_NeoPixel *strip, int *times, int length, int timeLit, int threshold) {
  for(int i=0; i<length; i++) {
    // start color should be either #000000 or #603311
    strip->setPixelColor(i, fade(i, times[i], timeLit, strip->Color(0, 0, 0), strip->Color(0, 255, 0), threshold));
    strip->show();
  }
}

Adafruit_NeoPixel branchOne = Adafruit_NeoPixel(branchOneNum, branchOnePin, NEO_GRB + NEO_KHZ800);

void setup() {
  branchOne.begin();
  branchOne.show();
}

void loop() {
  updateLights(&branchOne, branchOneTimes, branchOneNum, timeLit, threshold);
  delay(increment);
}
