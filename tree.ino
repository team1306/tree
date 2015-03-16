#include <Adafruit_NeoPixel.h>
#include <math.h>

const int timeLit = 500;
const int threshold = 3;
const int increment = 5;

const int branchOnePin = 2;
const int branchOneButtonPin = 3;
bool branchOneActivated = false;
const int branchOneNum = 60;

const int bigFinishPin = 10;
bool bigFinish = false;

Adafruit_NeoPixel branchOne = Adafruit_NeoPixel(branchOneNum, branchOnePin, NEO_GRB + NEO_KHZ800);

int fadePulse(int index, int time, int maxtime, int startcolor, int endcolor, int threshold) {
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

int updatePulse(Adafruit_NeoPixel *strip, int time, int length, int timeLit, int threshold) {
  if(time < threshold*length + timeLit) {
    for(int i=0; i<length; i++) {
      // start color should be either #000000 or #603311
      strip->setPixelColor(i, fadePulse(i, time, timeLit, strip->Color(0, 0, 0), strip->Color(0, 255, 0), threshold));
    }
    strip->show();
    return 1;
  }
  return 0;
}

int fadeFill(int index, int time, int maxtime, int startcolor, int endcolor, int threshold) {
  if(time < threshold*index) {
    time = 0;
  }
  else if(time < threshold*index + maxtime) {
    time = time - threshold*index;
  }
  else {
    time = maxtime;
  }
  
  return map(time, 0, maxtime, startcolor, endcolor);
}

int updateFill(Adafruit_NeoPixel *strip, int time, int length, int timeLit, int threshold) {
  if(time < threshold*length + timeLit) {
    for(int i=0; i<length; i++) {
      // start color should be either #000000 or #603311
      strip->setPixelColor(i, fadeFill(i, time, timeLit, strip->Color(0, 0, 0), strip->Color(0, 255, 0), threshold));
    }
    strip->show();
    return 1;
  }
  return 0;
}

void setup() {
  branchOne.begin();
  branchOne.show();
}

int i = 0;

void loop() {
  
  // check buttons
  if(digitalRead(branchOneButtonPin)) {
    branchOneActivated = true;
  }
  if(digitalRead(bigFinishPin)) {
    bigFinish = true;
  }
  
  // run pulses
  if(branchOneActivated && !bigFinish) {
    if(!updatePulse(&branchOne, i, branchOneNum, timeLit, threshold)) {
      branchOneActivated = false;
    }
  }
  
  // run the big finish
  if(bigFinish) {
    // updateFill all of the branches
    updateFill(&branchOne, i, branchOneNum, timeLit, threshold);
  }
  
  delay(increment);
  i++;
}
