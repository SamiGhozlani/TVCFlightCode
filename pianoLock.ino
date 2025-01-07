//Sami's Piano Lock
//Servo 180 = unlocked
//Servo 0 = locked

#include <Vector.h>
#include <Servo.h>

Servo servo;

#define CTone 262
#define CsharpTone 277
#define DTone 294
#define DsharpTone 311
#define ETone 330
#define FTone 349
#define FsharpTone 370
#define GTone 392
#define GsharpTone 415
#define ATone 440
#define AsharpTone 466
#define BTone 494

const int answerLength = 9;
int storage_array[answerLength];
Vector<int> userInput(storage_array);
//Für Elise
int answer[answerLength] = {12,11,12,11,12,7,10,8,6};

int buttonValue;
int count = 0;
bool correct = true;

void setup() {
  // C
  pinMode(0, INPUT_PULLUP);
  // C#
  pinMode(1, INPUT_PULLUP);
  // D
  pinMode(2, INPUT_PULLUP);
  // D#
  pinMode(3, INPUT_PULLUP);
  // E
  pinMode(4, INPUT_PULLUP);
  // F
  pinMode(5, INPUT_PULLUP);
  // F#
  pinMode(6, INPUT_PULLUP);
  // G
  pinMode(7, INPUT_PULLUP);
  // G#
  pinMode(8, INPUT_PULLUP);
  // A
  pinMode(9, INPUT_PULLUP);
  //Servo
  servo.attach(10);
  //Pizeoelectric buzzer
  pinMode(11, OUTPUT);
  // A#
  pinMode(12, INPUT_PULLUP);
  //B
  pinMode(13, INPUT_PULLUP);
}

void loop() {
  //Detecting the first nine key presses
  while (count < 9){
    buttonValue = digitalRead(0);
    if (buttonValue == LOW){;l
      count++;
      tone(11,CTone,200);
      userInput.push_back(1);
      while (buttonValue == LOW){
        tone(11,CTone,200);
        buttonValue = digitalRead(0);
      }
    }
    buttonValue = digitalRead(1);
    if (buttonValue == LOW){
      count++;
      tone(11,CsharpTone,200);
      userInput.push_back(2);
      while (buttonValue == LOW){
        tone(11,CsharpTone,200);
        buttonValue = digitalRead(1);
      }
    }
    buttonValue = digitalRead(2);
    if (buttonValue == LOW){
      count++;
      tone(11,DTone,200);
      userInput.push_back(3);
      while (buttonValue == LOW){
        tone(11,DTone,200);
        buttonValue = digitalRead(2);
      }
    }
    buttonValue = digitalRead(3);
    if (buttonValue == LOW){
      count++;
      tone(11,DsharpTone,200);
      userInput.push_back(4);
      while (buttonValue == LOW){
        tone(11,DsharpTone,200);
        buttonValue = digitalRead(3);
      }
    }
    buttonValue = digitalRead(4);
    if (buttonValue == LOW){
      count++;
      tone(11,ETone,200);
      userInput.push_back(5);
      while (buttonValue == LOW){
        tone(11,ETone,200);
        buttonValue = digitalRead(4);
      }
    }
    buttonValue = digitalRead(5);
    if (buttonValue == LOW){
      count++;
      tone(11,FTone,200);
      userInput.push_back(6);
      while (buttonValue == LOW){
        tone(11,FTone,200);
        buttonValue = digitalRead(5);
      }
    }
    buttonValue = digitalRead(6);
    if (buttonValue == LOW){
      count++;
      tone(11,FsharpTone,200);
      userInput.push_back(7);
      while (buttonValue == LOW){
        tone(11,FsharpTone,200);
        buttonValue = digitalRead(6);
      }
    }
    buttonValue = digitalRead(7);
    if (buttonValue == LOW){
      count++;
      tone(11,GTone,200);
      userInput.push_back(8);
      while (buttonValue == LOW){
        tone(11,GTone,200);
        buttonValue = digitalRead(7);
      }
    }
    buttonValue = digitalRead(8);
    if (buttonValue == LOW){
      count++;
      tone(11,GsharpTone,200);
      userInput.push_back(9);
      while (buttonValue == LOW){
        tone(11,GsharpTone,200);
        buttonValue = digitalRead(8);
      }
    }
    buttonValue = digitalRead(9);
    if (buttonValue == LOW){
      count++;
      tone(11,ATone,200);
      userInput.push_back(10);
      while (buttonValue == LOW){
        tone(11,ATone,200);
        buttonValue = digitalRead(9);
      }
    }
    buttonValue = digitalRead(12);
    if (buttonValue == LOW){
      count++;
      tone(11,AsharpTone,200);
      userInput.push_back(11);
      while (buttonValue == LOW){
        tone(11,AsharpTone,200);
        buttonValue = digitalRead(12);
      }
    }
    buttonValue = digitalRead(13);
    if (buttonValue == LOW){
      count++;
      tone(11,BTone,200);
      userInput.push_back(12);
      while (buttonValue == LOW){
        buttonValue = digitalRead(13);
        tone(11,BTone,200);
      }
    }
  }

  // Checking combination
  for (int i = 0; i<9; i++){
    if (answer[8-i] != userInput.pop_back()){
      correct = false;
    }
  }
  //Unlocking for 10 seconds
  if (correct == true){
    servo.write(180);
    delay(10000);
    servo.write(0);
  }
  correct = true;
  count = 0;
}
