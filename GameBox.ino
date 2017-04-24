// Constant header file
#include "Piches.h"

// ================================> Lose <============================================\\
int loseMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int loseNoteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4};

// ================================> Random selection <================================\\
int randomMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int randomNoteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4};

// ================================> Game Box is winner <==============================\\
int gbiwMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int gbiwNoteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4};

// ================================> player is winner <================================\\
int piwMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int piwNoteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4};
// ====================================================================================\\

int P1_OP;
int P1_Red_LED;
int P1_Green_LED;

int P2_OP;
int P2_Red_LED;
int P2_Green_LED;

int P3_OP;
int P3_Red_LED;
int P3_Green_LED;

int P4_OP;
int P4_Red_LED;
int P4_Green_LED;

int GB_Yellow_LED;
int Speaker_Op;
int GB_Number_PL_In;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

// Play lose melody
void LoseMelodyPlay() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}
