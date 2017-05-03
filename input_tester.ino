//LCD setup
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, A5, A4, A3, A2);

//Pins
const int R_button = 13;
const int M_button = 1;
const int L_button = 2;
const int LED1 = 3;
const int LED2 = 4;
const int LED3 = 5;
const int LED4 = 6;
const int LED5 = 7;
const int laser = 11;

//abbreviations
const int hello = 0;
const int familiar = 1;
const int testing = 2;
const int verify = 3;
const int complete = 4;

//variables
bool button_pressed = false;
bool ammo = true;
bool verified = false;
int stage = 0; //0 - hello. 1 - familiar. 2 - testing. 3 - verify. 4 - complete
int lag = 0;
int lagHigh = 250;
int lagLow = 1;
int lever = 0;
int crossings = 0;
int steplevel = 0;
int stepindex = 0;
int delays[] = {30, 20, 10, 5, 1, 1, 1, 1};
int threshold = -1;

void setup() {
  //Initizlize Pins
  pinMode(R_button, INPUT);
  pinMode(L_button, INPUT);
  pinMode(M_button, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);

  digitalWrite(L_button, LOW);
  digitalWrite(R_button, LOW);
  digitalWrite(M_button, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  
  // set up the LCD's number of columns and rows, print message:
  lcd.begin(16, 2);
  lcd.print("Welcome! :)");
  lcd.setCursor(0, 1);
  lcd.print("Please press R");

}

void loop() {

  //pevent multiple inputs
  if(digitalRead(L_button) == HIGH || digitalRead(M_button) == HIGH || digitalRead(R_button) == HIGH){
    button_pressed == true;
  }
  if(digitalRead(L_button) == LOW && digitalRead(M_button) == LOW && digitalRead(R_button) == LOW){
    button_pressed == false;
    ammo = true;
  }

  //debug
  if(digitalRead(L_button) == HIGH && digitalRead(M_button) == HIGH && digitalRead(R_button) == HIGH){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Debug Override");
    lcd.setCursor(0, 1);
    lcd.print((lagHigh + lagLow)/2);
    lcd.print("ms");
    if(steplevel == 190){
      lcd.print("*");
    }
    lcd.print("   C: ");
    lcd.print(crossings);
  }

  //hello -> familiar
  if(digitalRead(R_button) == HIGH && stage == hello){
    ammo = false;
    stage = familiar;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Train Phase");
    lcd.setCursor(0, 1);
    lcd.print("Press R to Begin");
   
  }
  
  //Familiarize, no lag 
  if(ammo == true){
      if(digitalRead(L_button) == HIGH && digitalRead(M_button) == HIGH){
        delay(400);
        fire();
        ammo = false;
      }
      else if(digitalRead(M_button) == HIGH){
        fire();
        ammo = false;
      }
      
  }
  if(digitalRead(M_button) == LOW){
    delay(lag);
    digitalWrite(LED5, LOW);
    delay(150);
  }

  //familiar -> testing
  if(digitalRead(R_button) == HIGH && stage == familiar && ammo == true){
    stage = testing;
    ammo = false;
    lever = random(1, 100);
    if(lever < 33){
      lag = lagHigh;
    }
    else if(lever > 66){
      lag = lagLow;
    }
    else{
      lag = 0;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Is there delay?");
  } 

  //testing -> verify
  //Yes
  if(digitalRead(R_button) == HIGH && ammo == true && stage == testing){
    stage = verify;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Response Saved:");
    lcd.setCursor(0,1);
    lcd.print("Yes     Press R");
    ammo = false;
    verified = true;

    if(lag == lagHigh){
      if((lagHigh - delays[stepindex]) < lagLow){
        int temp = lagLow;
        if (lagHigh - delays[stepindex] < 0){
          lagLow = 1;
        }
        else{
          lagLow = lagHigh - delays[stepindex];
        }
        steplevel++;
        if(steplevel == 8){
          stepindex++;
        }
        else if(steplevel == 16){
          stepindex++;
        }
        else if(steplevel == 25){
          stepindex++;
        }
        lagHigh = temp;
        crossings++;
      }
      else{
        if (lagHigh - delays[stepindex] < 0){
          lagHigh = 1;
        }
        else{
          lagHigh = lagHigh - delays[stepindex];
        }
        steplevel++;
        if(steplevel == 8){
          stepindex++;
        }
        else if(steplevel == 16){
          stepindex++;
        }
        else if(steplevel == 25){
          stepindex++;
        }
      }
    }
    else if(lag == lagLow){
      if(lagLow < delays[steplevel]){
        lagLow = 1;
        steplevel++;
        if(steplevel == 8){
          stepindex++;
        }
        else if(steplevel == 16){
          stepindex++;
        }
        else if(steplevel == 25){
          stepindex++;
        }
      }
      else{
        lagLow = lagLow - delays[stepindex];
        steplevel++;
        if(steplevel == 8){
          stepindex++;
        }
        else if(steplevel == 16){
          stepindex++;
        }
        else if(steplevel == 25){
          stepindex++;
        }
      }
    }

    if(lag == 0){
      lagLow = lagLow + delays[stepindex];
      lagHigh = lagHigh + delays[stepindex];
    }
    
  }

  //No
  if(digitalRead(L_button) == HIGH && ammo == true && stage == testing){
    stage = verify;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Response Saved:");
    lcd.setCursor(0,1);
    lcd.print("No      Press R");
    ammo = false;
    verified = true;
    
    if(lag == lagHigh){
        lagHigh = lagHigh + delays[stepindex];
        steplevel++;
        if(steplevel == 8){
          stepindex++;
        }
        else if(steplevel == 16){
          stepindex++;
        }
        else if(steplevel == 25){
          stepindex++;
        }
    }
    else if(lag == lagLow){
      if((lagLow + delays[stepindex]) > lagHigh){
        int temp = lagHigh;
        lagHigh = lagLow + delays[stepindex];
        lagLow = temp;
        crossings++;
        steplevel++;
        if(steplevel == 8){
          stepindex++;
        }
        else if(steplevel == 16){
          stepindex++;
        }
        else if(steplevel == 25){
          stepindex++;
        }
      }
      else{
        lagLow = lagLow + delays[stepindex];
        steplevel++;
        if(steplevel == 8){
          stepindex++;
        }
        else if(steplevel == 16){
          stepindex++;
        }
        else if(steplevel == 25){
          stepindex++;
        }
      }
    }

  }

  //verify -> testing
  if(digitalRead(R_button) == HIGH && stage == verify && ammo == true && verified == true && crossings <= 4){
    stage = testing;
    ammo = false;
    verified = false;
    lever = random(1, 100);
    if(lever < 33){
      lag = 0;
    }
    else if(lever > 66){
      lag = lagHigh;
    }
    else{
      lag = lagLow;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Is there delay?");
  } 

  //verify -> complete
  if((digitalRead(R_button) == HIGH && stage == verify && ammo == true && verified == true && crossings >= 5) || steplevel == 190){
    stage = complete;
    threshold = lag;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You're done!");
    lcd.setCursor(0,1);
    lcd.print("Score: ");
    lcd.print((lagHigh + lagLow)/2);
    lcd.print("ms");
    if(steplevel == 190){
      lcd.print("*");
    }
    //inf loop to make it not able to move on from here
    while(true){
      ammo = false;
    }
  }

  

}


void fire() {
  delay(lag);
  digitalWrite(LED1, HIGH);
  tone(laser, 5000, 1000);
  delay(32);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  tone(laser, 4000, 1000);
  delay(32);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  tone(laser, 3000, 1000);
  delay(32);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, HIGH);
  tone(laser, 2000, 1000);
  delay(32);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, HIGH);
  tone(laser, 1000, 1000);
  noTone(laser);
}
