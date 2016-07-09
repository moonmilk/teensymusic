
const int MIDI_CHANNEL = 1;

int NUM_THINGIES = 4;

const int TOUCH_PINS[] = {0, 1, 15, 16};
const int CONTROLLERS[] = {20, 21, 22, 23};

float touch[] = {0,0,0,0};

float slow = 0.1 ;

void setup() {
  
}

void loop() {
  for (int i=0; i < NUM_THINGIES; i++) {
    int a = touchRead(TOUCH_PINS[i]);
    float c = a/4.0;

    touch[i] = touch[i]*(1.0-slow) + c*slow;

    usbMIDI.sendControlChange(CONTROLLERS[i], (int)touch[i], MIDI_CHANNEL);
    delay(20);

  }
}
