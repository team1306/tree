#include <Adafruit_NeoPixel.h>
#include <math.h>

const int timeLit = 50;
const int threshold = 2;
const int increment = 1;

const int branchOnePin = 11;
const int branchOneButtonPin = 3;
bool branchOneActivated = false;
const int branchOneNum = 120;
int branchOneI = 0;

const int bigFinishPin = 2;
bool bigFinish = false;
int bigFinishI = 0;

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
      strip->setPixelColor(i, strip->Color(0, fadePulse(i, time, timeLit, 0, 255, threshold), 0));
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
      strip->setPixelColor(i, strip->Color(0, fadeFill(i, time, timeLit, 0, 255, threshold), 0));
    }
    strip->show();
    return 1;
  }
  return 0;
}

void blackOut(Adafruit_NeoPixel *strip, int length) {
  for(int i=0; i<length; i++) {
    strip->setPixelColor(i, strip->Color(0, 0, 0));
  }
  strip->show();
}

void setup() {
  pinMode(branchOneButtonPin, INPUT);
  pinMode(bigFinishPin, INPUT);
  pinMode(13, OUTPUT);
  
  branchOne.begin();
  branchOne.show();
}

void loop() {
  
  // check buttons
  if(digitalRead(branchOneButtonPin) && !branchOneActivated) {
    branchOneActivated = true;
    branchOneI = 0;
  }
  if(digitalRead(bigFinishPin) && !bigFinish) {
    bigFinish = true;
    bigFinishI = 0;
  }
  
  // run pulses
  if(branchOneActivated /*&& !bigFinish*/) {
    if(!updatePulse(&branchOne, branchOneI, branchOneNum, timeLit, threshold)) {
      blackOut(&branchOne, branchOneNum);
      branchOneActivated = false;
    }
  }
  
  // run the big finish
  if(bigFinish) {
    // updateFill all of the branches
    if(!updateFill(&branchOne, bigFinishI, branchOneNum, timeLit, threshold)) {
      blackOut(&branchOne, branchOneNum);
      bigFinish = false;
    }
  }
  
  if(!bigFinish) {
    delay(increment);
  }
  branchOneI++;
  bigFinishI++;
}
