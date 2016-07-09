
const int MIDI_CHANNEL = 1;

const int NUMBER_OF_KEYS = 12;

const int TOUCH_PINS[NUMBER_OF_KEYS] = {0, 1, 15, 16,   17, 18, 19, 22,   23, 25, 32, 33};

const int NOTE_NUMBERS[NUMBER_OF_KEYS] = { 60, 62, 64, 65,   67, 69, 70, 72,   74, 75, 77, 78 };

int calibration[NUMBER_OF_KEYS]; 
bool touched[NUMBER_OF_KEYS];

void setup() {
  
  // calibrate while nothing is being touched
  for (int i=0; i < NUMBER_OF_KEYS; i++) {
    calibration[i] = touchRead(TOUCH_PINS[i]);
    touched[i] = false;
  }

}


void loop() {
  // put your main code here, to run repeatedly:

  // see if any touch pins have changed much

  for (int i=0; i < NUMBER_OF_KEYS; i++) {
    // get reading
    int t = touchRead(TOUCH_PINS[i]);
    
    // sensor has gone up a lot and wasn't previously touched
    if (t > calibration[i] * 1.75 && touched[i] == false) {
      // it's touched now! send a NOTE ON
      usbMIDI.sendNoteOn(NOTE_NUMBERS[i], 100, MIDI_CHANNEL);
      // remember that it is touched
      touched[i] = true;
    }

    // sensor has gone down a lot and was previously touched
    else if (t < calibration[i] * 1.25 && touched[i] == true) {
      // it's not touched any more - send NOTE OFF
      usbMIDI.sendNoteOff(NOTE_NUMBERS[i], 0, MIDI_CHANNEL);
      // remember it's not touched
      touched[i] = false;
    }

    // wait a little before checking again
    delay(20);
  }

}
