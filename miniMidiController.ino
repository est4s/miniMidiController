#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>
#define numButtons 3
const uint8_t button1 = 2;
const uint8_t button2 = 3;
const uint8_t button3 = 4;

const uint8_t buttons[numButtons] = {button1, button2, button3};
const uint8_t notePitches[numButtons]={48, 49, 50};

uint8_t pressedButtons = 0x00;
uint8_t previousButtons = 0x00;
int intensity=60;

void setup() {
  Serial.begin(115200);
  for(int i=0; i<numButtons; i++)
    pinMode(buttons[i], INPUT_PULLUP);

}

void loop() {
readButtons();
playNotes();
}

void readButtons(){
  for(int i=0; i<numButtons; i++){
    if(digitalRead(buttons[i])){
      bitWrite(pressedButtons, i, 1);
      delay(50);
      }
     else
      bitWrite(pressedButtons, i, 0);
    }
  }

void playNotes(){
  for(int i=0; i<numButtons; i++){
    if(bitRead(pressedButtons, i) != bitRead(previousButtons, i)){
      if(bitRead(pressedButtons, i)){
        bitWrite(previousButtons, i, 1);
        noteOn(0, notePitches[i], 64);
        MidiUSB.flush();
        }
      else{
        bitWrite(previousButtons, i, 0);
        noteOff(0, notePitches[i], 64);
        MidiUSB.flush();
        }
      }
    }
  }

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
