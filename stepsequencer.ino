
int toggle = 24; //Quantize switch
int reverse = 26; // Reverse switvh
int buttonPins[4] = {36, 35, 34, 33};
int tempoPin = A9; // Anolog pin of the tempo potentiometer
int tempoVal = 0;
int mappedTempoVal = 0;
int leds[4] = {29, 30, 31, 32 }; // Array of the four LEDs
int tempo = 0;
int currentStep = 0;
int MidiVals[4] = {36 ,42, 38 , 42 }; // Array of the four MIDI values
unsigned long lastStepTime = 0;
boolean lastButtonStates[4] = {LOW, LOW, LOW, LOW};
boolean buttonStates[4] = {LOW, LOW, LOW, LOW};
boolean on[4] = {false, false, false, false};

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(tempoPin, INPUT);
  // pinMode(buttonPin1, INPUT);
  pinMode(toggle, INPUT);
  pinMode(reverse, INPUT);
}

void checkButtons() {
  for (int i = 0; i < 4; i++) {
    lastButtonStates[i] = buttonStates[i];
    buttonStates[i] = digitalRead(buttonPins[i]);
    if (buttonStates[i] == HIGH && lastButtonStates[i] == LOW) {
      if (on[i] == false) {
        on[i] = true;
      } else if (on[i] == true) {
        on[i] = false;
      }
    }
  }
}
void setLed() {
  for (int i = 0; i < 4; i++) {
    if (on[i] == true) {
      digitalWrite(leds[i], HIGH);
    } else if (currentStep == i) {
      digitalWrite(leds[i], HIGH);
    } else if (on[i] == false) {
      digitalWrite(leds[i], LOW);
    }
  }
}

void buttonLeds() {
  checkButtons();
  setLed();
}

void sequenceForward() {
  tempo = analogRead(A9);
  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //digitalWrite(leds[currentStep], LOW);  //turn off the current led
    usbMIDI.sendNoteOff(MidiVals[currentStep], 0, 1);
    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    }
    //digitalWrite(leds[currentStep], HIGH); //turn on the new led
    if (on[currentStep] == true) {
      usbMIDI.sendNoteOn(MidiVals[currentStep], 127, 1);
    }
    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

void sequenceBackward() {
  tempo = analogRead(A9);

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //digitalWrite(leds[currentStep], LOW);  //turn off the current led
    usbMIDI.sendNoteOff(MidiVals[currentStep], 0, 1);
    currentStep = currentStep - 1;         //increment to the next step
    if (currentStep < 0) {
      currentStep = 3;
    }
    if (on[currentStep] == true) {
      usbMIDI.sendNoteOn(MidiVals[currentStep], 127, 1);
    }
    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

void loop () {

  buttonLeds();
  if (digitalRead(reverse) == LOW) {
    if (digitalRead(toggle) == LOW) {
      sequenceForward();
    }
  }  else if (digitalRead(reverse) == HIGH) {
    if (digitalRead(toggle) ==  LOW) {
      sequenceBackward();
    }
  }
}
