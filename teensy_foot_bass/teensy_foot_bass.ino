#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


const int NUM_KEYS = 8;

// volume pot on A6, pitch pot on A7
const int VOLUME_POT = 6; // 35 (not pressed) to 325 (pressed)
const int VOLUME_POT_MIN = 35, VOLUME_POT_MAX = 325;
const int PITCH_POT = 7;
const int PITCH_POT_MIN = 0, PITCH_POT_MAX = 1023;
//
// touchsense lines
const int keys[NUM_KEYS] = {18, 15, 25, 19, 23, 22, 16, 17};
int keyCalibration[NUM_KEYS];
bool keyState[NUM_KEYS];

// bass notes C G D A  E B F# C#
const int notes[NUM_KEYS] = {0, 7, 14, 21,  4, 11, 18, 25};


void setup() { 

  pinMode(LED_BUILTIN, OUTPUT);

  for (int i=0; i<NUM_KEYS; i++) {
    keyCalibration[i] = touchRead(keys[i]);
    keyState[i] = 0;
  }
}

void loop() {
  readInputs();
  delay(20);

  while(usbMIDI.read()) {
    
  }
}


float loud = 0.0;
int lastSquish = 0;

void readInputs() {
  for (int i=0; i<NUM_KEYS; i++) {
    int t = touchRead(keys[i]);
    if (t > keyCalibration[i] * 1.75 && keyState[i] == 0) {
      usbMIDI.sendNoteOn(32+notes[i], 100, 1);
      keyState[i] = 1;
    }
    else if (t < keyCalibration[i] * 1.25 && keyState[i] == 1) {
      usbMIDI.sendNoteOff(32+notes[i], 0, 1);
      keyState[i] = 0;
    }
  }

  int squish = analogRead(VOLUME_POT);
  int dSquish = squish - lastSquish;
  lastSquish = squish;

  float squishVelocity = 0.0;
  if (dSquish > 0) {
    squishVelocity = dSquish / 11.0;
  }

  loud = 0.95 * loud + 0.05 * squishVelocity;
  if (loud > 1.0) loud = 1.0;

  usbMIDI.sendControlChange(77, loud * 127, 1);
  usbMIDI.sendControlChange(78, map(analogRead(PITCH_POT), 0, 1023, 64, 88), 1);

  bool someNoteOn = 0;
  for (int i=0; i<NUM_KEYS; i++) {
    if (keyState[i] == 1) {
      someNoteOn = 1;
      break;
    }
  }
  digitalWrite(LED_BUILTIN, someNoteOn);
}

