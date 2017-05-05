#include "pitches.h"
#include "SimpleList.h"

// Lose melody data
int loseMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int loseNoteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

// Win melody data
int readyMelody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int readyNoteDurations[] = {8, 8, 8, 8, 8, 8, 8, 8};
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

SimpleList<int> playersInGameList;
SimpleList<int> playersPushedBTNList;

int players_Btn_pins[] = {0,1,2,3};
int players_G_pins[]   = {2,4,6,8};
int players_R_pins[]   = {3,5,7,9};

int  delayBlink    = 300;
int  currentBoss   = -1;
int  playerSize    = 0;

bool NOPMode       = true;
bool selectingMode = false;
bool newTurnMode   = false;
bool readyGo       = false;
bool thereIsWinner = false;

// GameBox items
int led_yellow_GB    = 10;
int speaker_Op       = 11;
int Potentiameter_Ip = 5;
int counter    = 0;
int maxCounter = 10000;

void setup() {
 // Add to counter
 RandomCounter();
 
 playerSize = sizeof(players_G_pins) / sizeof(int);
 playersInGameList.reserve(playerSize);
 playersPushedBTNList.reserve(playerSize);
 
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
  /* =========================== Select number of player =========================== */
  if(NOPMode == true){
    // Get number of player base and change random number
    int numberPlayer = playerSize - analogRead(Potentiameter_Ip)/(1023/playerSize);
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
  
  TurnNext:
  /* =========================== Select a boss =========================== */
  if(selectingMode == true){
    Serial.println("Step random selection!!");
    if(playersInGameList.size() > 2){
      // Play random selection melody
      SelectionMelodyPlay();
      //Turn all player green LED on
      BlinkStateAll(true,players_G_pins); 
      //Choose a boss
      ChooseBoss();
      // Turn new boss red LED's on
      digitalWrite(players_R_pins[currentBoss], HIGH);
    }else if(playersInGameList.size() == 2){
      // Play GameBox is winner on
      GameBoxIsBossMelodyPlay();
      currentBoss = -1;
      // Turn GameBox boss yellow LED's on
      digitalWrite(led_yellow_GB, HIGH);
    } else{
      Serial.println("Error: It is impossible situation!");
    }
      // Blink and then turn GameBox yellow LED on
      BlinkGameBox();
      NOPMode       = true;
      selectingMode = false;
      readyGo       = false;
      newTurnMode   = true ;
      // Counter change to zero in case that GameBox is boss
      counter       = 0;
  }
   /* =========================== New turn start here =========================== */
  // Main game logic start here
  if(newTurnMode == true){
    RandomCounter();
      if(readyGo == false){
        Serial.println("Ready to go is false!");
        // GameBox yellow LED is on
        digitalWrite(led_yellow_GB, HIGH);
         // Boss is player
         if(currentBoss >= 0){
          Serial.println("There is  boss!");
           for (SimpleList<int>::iterator itr = playersInGameList.begin(); itr != playersInGameList.end();){
             // A player pushed
             if(IsButtonPushed((*itr)) == true){
              Serial.println(currentBoss);
              Serial.println((*itr));
              Serial.println("A player pushed button!");
                // That Player is not boss then he lose
                if((*itr)!= currentBoss ){                 
                   // Send this player out of game
                   ThisPlayerOut(itr);                  
                   // All player lose and game is over ??
                   GameOver();                  
                   //If there is still opponent
                   continue; 
                   // Boss commanded
                 } else if((*itr)== currentBoss ) {
                 // That Player is boss
                   readyGo = false;
                   selectingMode = false;
                 
//                 Serial.println("boss pushed button!");
//                 //get last in list to set for order  
//                 playersPushedBTNList =  playersInGameList;
//                 Serial.print("Player size is -> ");
//                 Serial.println(playersInGameList.size());
//                 Serial.println("Boss says push now!!");
                 // Boss never lose
//                 itr = playersPushedBTNList.erase(itr);
//                 Serial.print("Out list is -> ");
//                 Serial.println(playersPushedBTNList.size());
                 return;
                }
            }
            ++itr;
            
            if(itr == playersInGameList.end()){
               itr = playersInGameList.begin();
            }
        }
      // Boss is Game box
      }else{
        Serial.println("Boss is GameBox!");
        // Get random number
        RandomCounter();
        for (SimpleList<int>::iterator _itr1 = playersInGameList.begin(); _itr1 != playersInGameList.end();){
          // Push before GameBox order
          if(IsButtonPushed((*_itr1)) == true){              
            // Send this player out of game
            ThisPlayerOut(_itr1);          
            // All player lose and game is over ??
            GameOver();
            return;
           }
                  
           // GameBox boss commanded
           if(counter >(maxCounter-100)){
             // get last in list to set for order  
             playersPushedBTNList =  playersInGameList;
             readyGo = true;
             return;
            }
            
            ++_itr1; 
            
            if(_itr1 == playersInGameList.end()){
               _itr1 = playersInGameList.begin();
            } 
          }
        }
      // If ready to go true
      }else{
        Serial.println("Ready to go is true!");
        // GameBox yellow LED is off then push nooooooow
        digitalWrite(led_yellow_GB, false);
        // Get random number
        RandomCounter(); 
        // if player push after boss but late
        if(currentBoss >= 0){
          // There is a boss who commanded
          for (SimpleList<int>::iterator _itr2 = playersPushedBTNList.begin(); _itr2 != playersPushedBTNList.end();){            
            // First push first out
            if(IsButtonPushed((*_itr2)) == true && (*_itr2)!= currentBoss){
              
              _itr2 = playersPushedBTNList.erase(_itr2);
              
              Serial.println(currentBoss);
              Serial.println(*_itr2);
              
              Serial.println(players_G_pins[*_itr2]);
              
              Serial.println(playersInGameList.size());
              Serial.println(playersPushedBTNList.size());
                 
              if(playersPushedBTNList.size() == 1){                
                playersInGameList.erase(_itr2);
                
                Serial.print("New playersInGameList size is: ");
                Serial.println(playersInGameList.size());
                
                playersPushedBTNList = playersInGameList;
                delay(6000);
                
                // All Red green off
                 ChangeStateArray(false, players_R_pins, sizeof(players_R_pins) / sizeof(int));
                 ChangeStateArray(false, players_G_pins, sizeof(players_G_pins) / sizeof(int));
                  
                selectingMode = true ;
                newTurnMode   = false;
                thereIsWinner = false;
                return;
              }
              continue;   
            }
            ++_itr2;
            if(_itr2 == playersInGameList.end()){
               _itr2 = playersInGameList.begin();
            }
         }
      // Boss is game boss and it commanded
      }else{
      for (SimpleList<int>::iterator _itr3 = playersInGameList.begin(); _itr3 != playersInGameList.end();++_itr3){
        if(IsButtonPushed((*_itr3)) == true){
          ++_itr3;
          Serial.print("go to the next address and delete it. Then deleted is");
          Serial.println(*_itr3);
          _itr3 = playersInGameList.erase(_itr3);
          selectingMode = false ;
          newTurnMode   = false;
          thereIsWinner = true;
          return;
        }
      } 
    }
  }
}
 
  /* =========================== Game finished =========================== */
  if(thereIsWinner == true){
    Serial.println("There is a winner!");
    // Turn off green and red pins
    BlinkStateAll(false, players_G_pins);
    BlinkStateAll(false, players_R_pins);
    
    // GameBox is winner
    if(playersInGameList.size()==0){
      // Melody and blinking for GameBox
      FinalWinMelodyGBWPlay();

    // Player is winner
    } else if(playersInGameList.size()==1){
      // Melody and blinking for Player
      FinalWinMelodyPWPlay();  
    }else{
      Serial.println(playersPushedBTNList.size());
      Serial.println("Why I am Here!!??");
    }
    Serial.println("Finished!!");
    // Wait alittle
    delay(1000);    
    // Reset GameBox
    Reset();
  }
}

//Delete remained player in checker list from min list
void DeleteLoserFromList(){
  SimpleList<int>::iterator _itrLoser = playersPushedBTNList.begin();
  for (SimpleList<int>::iterator _itr4 = playersInGameList.begin(); _itr4 != playersInGameList.end();++_itr4){
    if((*_itr4)==(*_itrLoser)){
    
    }
  }
}
  
// Blink GameBox yellow LED
void BlinkGameBox(){
  for(int i=0; i<14; i++){
    digitalWrite(led_yellow_GB, i%2);
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
void ReadyMelodyPlay(){
  for (int thisReadyNote = 0; thisReadyNote < 8; thisReadyNote++) {

    int readyNoteDuration = 1000 / readyNoteDurations[thisReadyNote];
    tone(speaker_Op, readyMelody[thisReadyNote], readyNoteDuration);

    int pauseBetweenReadyNotes = readyNoteDuration * 1.30;
    delay(pauseBetweenReadyNotes);
    noTone(speaker_Op);
  }
}

// When final winer selected and is player
void FinalWinMelodyPWPlay(){
  int FinalWinnerNoteSize = sizeof(finalMelody) / sizeof(int);
  int _size = sizeof(players_G_pins) / sizeof(int);

  bool _state = false;
  SimpleList<int>::iterator _it = playersInGameList.begin();
  
  // Winner Red LED off
   digitalWrite( players_R_pins[*_it],false);
   
  for (int thisFinalWinNote = 0; thisFinalWinNote < FinalWinnerNoteSize; thisFinalWinNote++) {
    // Blink winner green LED
    digitalWrite( players_G_pins[*_it],_state);
    
    // Blink others Red LED
    for(int i=0;i<_size; i++){
      auto it = playersInGameList.begin();
      if(i != *_it){
        digitalWrite(players_R_pins[i],_state);
      }
    } 
    int finalDuration = 1000 / finalDurations[thisFinalWinNote];
    tone(speaker_Op, finalMelody[thisFinalWinNote], finalDuration);

    int pauseBetweenFinalNotes = finalDuration * 1.30;
    delay(pauseBetweenFinalNotes);
    _state = !_state;
    noTone(speaker_Op);
  }
}

// When final winer is GameBox
void FinalWinMelodyGBWPlay(){
  int FinalWinnerNoteSize = sizeof(finalMelody) / sizeof(int);
  bool _state = false;
  for (int thisFinalWinNote = 0; thisFinalWinNote < FinalWinnerNoteSize; thisFinalWinNote++) {
    //Start blinking
    BlinkStateAll(_state, players_R_pins);
    digitalWrite(led_yellow_GB,_state);
    
    int finalDuration = 1000 / finalDurations[thisFinalWinNote];
    tone(speaker_Op, finalMelody[thisFinalWinNote], finalDuration);

    int pauseBetweenFinalNotes = finalDuration * 1.30;
    delay(pauseBetweenFinalNotes);
    
    // Change state of blinking
    _state = !_state;
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
   if(counter <= maxCounter){
      counter++;
   }else{
      counter = 0;
   }
}

// When selecting new boss
void SelectionMelodyPlay(){

  int selectionSize = sizeof(selectionMelodyS) / sizeof(int);
   SimpleList<int>::iterator itr = playersInGameList.begin();
   
  for (int thisNDNote = 0; thisNDNote < selectionSize; thisNDNote++) {
    // Turn off all LEDs andturn on requred one
    BlinkStateAll(false,players_G_pins);
    digitalWrite(players_G_pins[(*itr)],true);
    
    int selectionempoDuration = 1000 / selectionempoDurations[thisNDNote];
    tone(speaker_Op, selectionMelodyS[thisNDNote], selectionempoDuration);
    
    int pauseBetweenelectNotes = selectionempoDuration * 1.30;
    delay(pauseBetweenelectNotes);
    noTone(speaker_Op);
    
    ++itr;
    // Reset iterator
    if(itr ==  playersInGameList.end()){
      itr = playersInGameList.begin();
    } 
  }
}

// Which player is boss
void ChooseBoss(){
  int _sizeP = playersInGameList.size()-1;
  int index = 0;
  
  for (SimpleList<int>::iterator _itr = playersInGameList.begin(); _itr != playersInGameList.end(); ++_itr, ++index){
    if(index == counter%_sizeP){
      currentBoss = (*_itr);
      break;
    }
  }   
}

//Change blink state all players' green or red LED's
void BlinkStateAll(bool state, int arrayLED[]){
  for (SimpleList<int>::iterator _it7 = playersInGameList.begin(); _it7 != playersInGameList.end();++_it7){
      digitalWrite(arrayLED[(*_it7)], state); 
  }
}

// Change state of LED
void ChangeStateArray(bool _state, int _array[], int _sizeOfArray){
  for(int i=0; i<_sizeOfArray; i++){
    digitalWrite(_array[i], _state);
  }
}

// This player out
void ThisPlayerOut(SimpleList<int>::iterator _ledPin){
   digitalWrite(players_G_pins[*_ledPin],LOW);
   digitalWrite(players_R_pins[*_ledPin],HIGH);
   _ledPin = playersInGameList.erase(_ledPin);
}

// Game is over
void GameOver(){
  if(playersInGameList.size() <= 1){
     newTurnMode   = false;
     thereIsWinner = true;
     return;
  }
}

// Reset GameBox
void Reset(){
  playersInGameList.clear();
  playersPushedBTNList.clear();

  // All red and green LEDs off
  ChangeStateArray(false, players_R_pins, sizeof(players_R_pins) / sizeof(int));
  ChangeStateArray(false, players_G_pins, sizeof(players_G_pins) / sizeof(int));
  
  // gameBox yellow LED off
  digitalWrite(led_yellow_GB,LOW);
  
  NOPMode       = true;
  selectingMode = false;
  newTurnMode   = false;
  readyGo       = false;
  thereIsWinner = false;
  noTone(speaker_Op);
}
