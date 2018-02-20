/* PongGame
    Suppachai
    IOT_Class by Ajahn Piti,
*/
#include "PongGame.h"
#include "LEDMatrix.h"
#include "HC595.h"

using namespace std;
using namespace hw4;

LEDMatrix ledMat(2,3,4,5);
//HC595 shreg(2, 3, 4, 5);

#define leftUpBut 8
#define leftDownBut 9
#define rightUpBut 11
#define rightDownBut 12
#define resetBut 13
// shift 14 connect to D2
// shift 13 connect to GND
// shift 12 connect to D4
// shift 11 connect to D3
// shift 10 connect to D5


const int nrow = 8;
const int ncol = 8;

PongGame pongGame(nrow, ncol);
int* buffer = new int[nrow*ncol];

void setup(){
    Serial.begin(9600);
    pongGame.start(millis());
    pinMode(leftUpBut, INPUT_PULLUP);
    pinMode(leftDownBut, INPUT_PULLUP);
    pinMode(rightUpBut, INPUT_PULLUP);
    pinMode(rightDownBut, INPUT_PULLUP);
    pinMode(resetBut, INPUT_PULLUP);
}

void processInput(unsigned long tick){ //You will need to change this
    
    if ((digitalRead(leftUpBut)==LOW)){
      pongGame.movePad(Player::PLAYER_TWO, PadDirection::UP);
      delay(50);
      
    }
    else if ((digitalRead(leftDownBut)==LOW)){
      pongGame.movePad(Player::PLAYER_TWO, PadDirection::DOWN);
      delay(50);
    }
    else if ((digitalRead(resetBut)==LOW)){
      pongGame.reset();
      pongGame.start(tick);
      // Serial.println(digitalRead(resetBut));
    }
    else if ((digitalRead(rightUpBut)==LOW)){
      pongGame.movePad(Player::PLAYER_ONE, PadDirection::UP);
      delay(50);
    }
    else if ((digitalRead(rightDownBut)==LOW)){
      pongGame.movePad(Player::PLAYER_ONE, PadDirection::DOWN);
      delay(50);
    }
  // }
}

byte BoolArrayToByte(bool boolArray[8]){
  byte result = 0; 

  for(int i = 0; i < 8; i++)
  {
    if(boolArray[i])
    {
      result = result | (1 << i);
    }
  }
  return result;
}

void drawScreen(int* buffer){
    bool rowArray[8];
    for(int irow=0; irow<nrow; irow++){
        for(int icol=0; icol<ncol; icol++){
            rowArray[icol] = buffer[irow*ncol + icol];
        }
        byte rowByte = BoolArrayToByte(rowArray);
        ledMat.writeRow(irow, rowByte);
        // ledMat.update();
    }
}

void loop(){
    const unsigned long tick = millis();
    processInput(tick);
    pongGame.update(tick);
    

    if(pongGame.isDirty()){
      pongGame.paint(buffer);
      drawScreen(buffer);
    }
    else{
      drawScreen(buffer);
    }
 }
