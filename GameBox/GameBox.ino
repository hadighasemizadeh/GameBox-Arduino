#include "pitches.h"
#include "SimpleList.h"

// Lose melody data
int loseMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int loseNoteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

// Win melody data
int winMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int winNoteDurations[] = {8, 8, 8, 8, 8, 8, 8, 8};
// Random melody data

int randomMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int randomNoteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4};

// Master win
int finalMelody[] = 
{ NOTE_E7, NOTE_E7, 0, NOTE_E7, 0, NOTE_C7, NOTE_E7, 0, NOTE_G7, 0, 0,  0, NOTE_G6, 0, 0, 0,
  NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0,
  NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7,0, NOTE_F7, NOTE_G7,0,NOTE_E7,0, NOTE_C7,NOTE_D7, NOTE_B6, 0, 0,
  NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6,0, NOTE_AS6, NOTE_A6, 0,
  NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0, NOTE_C7,NOTE_D7, NOTE_B6, 0, 0};
                 
int finalDurations[] = 
{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  9, 9, 9, 12, 12, 12, 12,12, 12, 12, 12,12, 12, 12, 12,};
//Selection melody
int selectionMelodyS[]= 
 {NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,NOTE_AS3, NOTE_AS4, 0,
  0,NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,NOTE_AS3, NOTE_AS4, 0,
  0, NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4, NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4, NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_DS4, NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4, NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_GS3, 0, 0, 0
  };

int selectionempoDurations[]= 
{ 12, 12, 12, 12, 12, 12, 6, 3, 12, 12, 12, 12, 12, 12, 6, 3, 12, 12, 12, 12,
  12, 12, 6, 3, 12, 12, 12, 12, 12, 12, 6, 6, 18, 18, 18, 6, 6, 6, 6, 6, 6,18,
  18, 18, 18, 18, 18, 10, 10, 10, 10, 10, 10, 3, 3, 3};
  
// players
//int Btn_p1       = 0;
//int led_green_p1 = 2;
//int led_red_p1   = 3;

//int Btn_p2       = 1;
//int led_green_p2 = 4;
//int led_red_p2   = 5;

//int Btn_p3       = 2;
//int led_green_p3 = 6;
//int led_red_p3   = 7;

//int Btn_p4       = 3;
//int led_green_p4 = 8;
//int led_red_p4   = 9;

//int players_ins[]      = {0,0,0,0};
SimpleList<int> playersInGameList;
 
int players_R_pins[]   = {3,5,7,9};
int players_G_pins[]   = {2,4,6,8};
int players_Btn_pins[] = {0,1,2,3};

int  delayBlink    = 300;
int  currentBoss   = -1;
int  playerSize    = 0;

bool selectingMode = true;
bool NOPMode       = true;
bool newTurnMode   = false;
bool readyGo       = false;

// GameBox items
int led_yellow_GB    = 10;
int speaker_Op       = 11;
int Potentiameter_Ip = 5;
int counter    = 0;

void setup() {
 // Add to counter
 RandomCounter();
 
 playerSize = sizeof(players_G_pins) / sizeof(int);
 playersInGameList.reserve(playerSize);
 
 // Set green and red LEDs' pin on
 for(int pin=0; pin<playerSize; pin++){
   pinMode(players_G_pins[pin], OUTPUT);
   pinMode(players_R_pins[pin], OUTPUT);
 }
 
// GameBox pin out
 pinMode(led_yellow_GB,   OUTPUT);
 Serial.begin(9600);
}

void loop() {
  // State of input number of players
  if(NOPMode == true){
    // Get number of player base and change random number
    int numberPlayer = analogRead(Potentiameter_Ip)/(1023/playerSize);
    RandomCounter();
     
    // Turn on player green LEDs base on potentiometer
    for(int nop = 0; nop < playerSize; nop++){
      if(nop <= (numberPlayer-1)){
         digitalWrite(players_G_pins[nop], HIGH);
      }else{
         digitalWrite(players_G_pins[nop], LOW);
      }
    }  
    // Player push button to say I am ready no blink it
     for(int nop = 0; nop < numberPlayer; nop++){
       if(IsButtonPushed(players_Btn_pins[nop])){
         CheckAndAddPlayerToList(players_Btn_pins[nop]);
         Serial.println(playersInGameList.size());
       }
     }  
     // Change random number
     RandomCounter();  
       
    // Reduce NOP when player is ready but we reduce NOP
     CheckAndDeletePlayerFromList(numberPlayer);
     
     // Blink in players
     BlinkAllNotReady(numberPlayer);  
         
    // All players in then lets go and play the game
    if(numberPlayer == playersInGameList.size()&& playersInGameList.size() > 1){ 
      NOPMode       = false;
      selectingMode = true;
    }
    return;
  }
  // Select a boss
  if(selectingMode == true){
    if(playersInGameList.size() > 1){
      SelectionMelodyPlay();
      // Turn new boss red LED's on
      digitalWrite(players_R_pins[currentBoss], HIGH);
    }else{
      // Play GameBox is winner on
      GameBoxIsBossMelodyPlay();
      
      currentBoss = -1;
    }
      // Blink and then turn GameBox yellow LED on
      BlinkGameBox();
      selectingMode = false;
      newTurnMode   = true ;
  }
  // Main game logic start here
  if(newTurnMode == true){
    // if player push before boss
    for (SimpleList<int>::iterator itr = playersInGameList.begin(); itr != playersInGameList.end();++itr){
      
    }
    
    // if player push after boss but late

    // time for finding new bus
    if(currentBoss>0){
      
    
    }else{
    
    }
  }
}
// Blink GameBox yellow LED
void BlinkGameBox(){
  for(int i=0; i<4; i++){
    digitalWrite(led_yellow_GB, LOW);
    delay(delayBlink);
    
    digitalWrite(led_yellow_GB, HIGH);
    delay(delayBlink);
  }
}

// is selected player in the GameBox player list if it is not add it
void CheckAndAddPlayerToList(int buttonPin){
   for (SimpleList<int>::iterator itr = playersInGameList.begin(); itr != playersInGameList.end(); ++itr){
     if((*itr) == buttonPin){
        return;
      }
   }
  playersInGameList.push_back(buttonPin);
}

// is selected player in the GameBox player list or not
bool IsButtonPinInList(int buttonPin){
   for (SimpleList<int>::iterator itr = playersInGameList.begin(); itr != playersInGameList.end(); ++itr){
      if((*itr) == buttonPin){
        return true;
      }
   }
   return false;
}

// Delete ready player from list if player is ready but NOP reduced
void CheckAndDeletePlayerFromList(int numberPlayerBl){
  for (SimpleList<int>::iterator itr = playersInGameList.begin(); itr != playersInGameList.end();){
    if((*itr) > numberPlayerBl-1){
      itr = playersInGameList.erase(itr);
      continue;
    }
    ++itr;
  }
}

// Check the button is on or not
bool IsButtonPushed(int analogPin){
  if(analogRead(analogPin)>900)
    return true;
    
  return false;
}

//Blink all players' green LED's who are not ready
void BlinkAllNotReady(int _playersIn){
  delay(delayBlink); 
  // All off
   for(int i=0; i<_playersIn; i++){
      if(IsButtonPinInList(players_Btn_pins[i])== false){
        digitalWrite(players_G_pins[i], LOW); 
      }
    }
    delay(delayBlink);
    // All on
    for(int i=0; i<_playersIn; i++){
      if(IsButtonPinInList(players_Btn_pins[i])== false){
        digitalWrite(players_G_pins[i], HIGH); 
      }
    }
  }

//Change blink state all players' green LED's
void BlinkStateAll(bool state){
  int _size = playersInGameList.size();
   for(int i=0; i<_size; i++){
     if(state == true){
        digitalWrite(players_G_pins[i], HIGH); 
     }else{
        digitalWrite(players_G_pins[i], LOW); 
     }
    }
  }
  
//Blink all players' green LED
void BlinkAll(){
  delay(delayBlink); 
  AllLEDOffOn(false);
  delay(delayBlink); 
  AllLEDOffOn(true);
  }
  
//Blink all players green LED one bye one
void BlinkAllOBO(){
  int pSize = sizeof(players_G_pins) / sizeof(int);
  for(int i=0; i<pSize; i++){
    AllLEDOffOn(false);
    digitalWrite(players_G_pins[i], HIGH);   
    delay(delayBlink);
   }
 }
  
// All LEDs off/On
void AllLEDOffOn(bool state){
  int pSize = sizeof(players_G_pins) / sizeof(int);
  for(int i=0; i<pSize; i++){
    if(state == false){
      digitalWrite(players_G_pins[i], LOW); 
     }else{
      digitalWrite(players_G_pins[i], HIGH); 
     }
   }
 }
 
// when payer lose it should play
void LoseMelodyPlay(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    int loseNoteDuration = 1000 / loseNoteDurations[thisNote];
    tone(speaker_Op, loseMelody[thisNote], loseNoteDuration);

    int pauseBetweenloseNotes = loseNoteDuration * 1.30;
    delay(pauseBetweenloseNotes);
    noTone(speaker_Op);
  }
}
// when payer win
void WinMelodyPlay(){
  for (int thisWinNote = 0; thisWinNote < 8; thisWinNote++) {

    int winNoteDuration = 1000 / winNoteDurations[thisWinNote];
    tone(speaker_Op, winMelody[thisWinNote], winNoteDuration);

    int pauseBetweenWinNotes = winNoteDuration * 1.30;
    delay(pauseBetweenWinNotes);
    noTone(speaker_Op);
  }
}

// when final winer selected
void FinalWinMelodyPlay(){
  int FinalWinnerNoteSize = sizeof(finalMelody) / sizeof(int);
  for (int thisFinalWinNote = 0; thisFinalWinNote < FinalWinnerNoteSize; thisFinalWinNote++) {

    int finalDuration = 1000 / finalDurations[thisFinalWinNote];
    tone(speaker_Op, finalMelody[thisFinalWinNote], finalDuration);

    int pauseBetweenFinalNotes = finalDuration * 1.30;
    delay(pauseBetweenFinalNotes);
    noTone(speaker_Op);
  }
}

// When GameBox is boss melody
void GameBoxIsBossMelodyPlay(){
  for (int thisRNDNote = 0; thisRNDNote < 8; thisRNDNote++) {

    int randomNoteDuration = 1000 / randomNoteDurations[thisRNDNote];
    tone(speaker_Op, randomMelody[thisRNDNote], randomNoteDuration);

    int pauseBetweenrandomNotes = randomNoteDuration * 1.30;
    delay(pauseBetweenrandomNotes);
    noTone(speaker_Op);
  }
}

// Always adding number to use this for making random number
void RandomCounter(){
 if(counter<10000){
    counter++;
 }else{
    counter = 0;
 }
}

// When selecting new boss
void SelectionMelodyPlay(){
  int selectionSize = sizeof(selectionMelodyS) / sizeof(int);
  int _sizeP = playersInGameList.size();
  for (int thisNDNote = 0; thisNDNote < selectionSize; thisNDNote++) {
    // blink
    BlinkStateAll(false);
    int pSelected = thisNDNote%_sizeP;
     digitalWrite(players_G_pins[pSelected], HIGH);

    int selectionempoDuration = 1000 / selectionempoDurations[thisNDNote];
    tone(speaker_Op, selectionMelodyS[thisNDNote], selectionempoDuration);
    
    int pauseBetweenelectNotes = selectionempoDuration * 1.30;
    delay(pauseBetweenelectNotes);
    noTone(speaker_Op);
  }
  //Turn all player green LED on
  BlinkStateAll(true);
  currentBoss = counter%_sizeP;
}
