

#include <Servo.h>
int SPD;
uint32_t myTimer1;

#define spd_L_LOW 1548
#define spd_R_LOW 1456
#define spd_L_LOW_REV 1460
#define spd_R_LOW_REV 1532

#define spd_L_HIGH 2000
#define spd_R_HIGH 1000
#define spd_L_HIGH_REV 1000
#define spd_R_HIGH_REV 2000

#define spd_L_STOP 1500
#define spd_R_STOP 1500

#include <NewPing.h>      // !! sonar.ping_cm() !!
#define TRIGGER_PIN  10  // trigger pin 
#define ECHO_PIN     A1  // echo pin
#define MAX_DISTANCE 200            
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

Servo servo_L;  // 93*
Servo servo_R;  // 92* /93
Servo servo_S;

void setup() {
Serial.begin(9600);

  
  pinMode(6, INPUT);        // ИК датчик
  pinMode(A4, INPUT_PULLUP); // фототранзистор
  pinMode(8, OUTPUT);        // ИК светодиод
  pinMode(A2, INPUT_PULLUP);
  digitalWrite(8, HIGH);
  servo_L.attach(5);     // левая серва
  servo_R.attach(A3);     // правая серва
  servo_S.attach(3);                        //!!!!!!!!!!!!!!!!!!!!!!!поворот дальномера

  
  
 if (!digitalRead(A2) == HIGH) {
  SPD = 2;
 }
 else SPD = 1;
 uint32_t now;
 
 Serial.print("1-  ");
 Serial.println(analogRead(A4));
 
  while ((checkSens_L() == LOW) and (checkSens_R() == LOW)) {
    uint32_t now = millis();
    while (millis () - now < 250) {
    digitalWrite(13, HIGH);
    }
    uint32_t now1 = millis();
    while (millis () - now1 < 250) {
    digitalWrite(13, LOW);
    }
}

}

void loop() {
  servo_S.write(75);
 if ((checkSens_L() == HIGH) and (checkSens_R() == HIGH) and ((sonar.ping_cm() > 7) or (sonar.ping_cm() == 0)))  {
  mov_FOV();
 }
 else {
  if ((checkSens_L() == LOW) and (checkSens_R() == HIGH)){
  
  if (random(1, 16) == 7) {
      turn_LEFT_ONE();
      uint32_t now = millis();
      while (millis () - now < 3000) {
      mov_BACK(500);
      STOP(100);
      while ((checkSens_L() == HIGH) and (checkSens_R() == HIGH) and ((sonar.ping_cm() > 7) or (sonar.ping_cm() == 0))) {
        mov_FOV();
      }
      STOP(100);
  }
    }
    mov_BACK(random(500, 700));
   
    if (random(1, 4) == 1) turn_LEFT(random(600, 1100));
    else turn_RIGHT(random(600, 1100));
  }
  
  if ((checkSens_R() == LOW) and (checkSens_L() == HIGH)){

if (random(1, 16) == 8) {
      turn_RIGHT_ONE();
      uint32_t now1 = millis();
      while (millis () - now1 < 3000) {
      mov_BACK(500);
      STOP(100);
      while ((checkSens_L() == HIGH) and (checkSens_R() == HIGH) and ((sonar.ping_cm() > 7) or (sonar.ping_cm() == 0))) {
        mov_FOV();
      }
      STOP(100);
  }
    }
    
    mov_BACK(random(450, 650));
    if (random(1, 4) == 1) turn_RIGHT(random(700, 1300));
    else turn_LEFT(random(600, 1100));
  }

  if (((sonar.ping_cm() > 7) or (sonar.ping_cm() == 0)) == false) {   // шо делать ежели увидел впереди препятствие
    int i = 0;
    mov_BACK(random(500, 700));
    servo_L.writeMicroseconds(spd_L_STOP);
    servo_R.writeMicroseconds(spd_R_STOP);
    uint32_t now = millis();
    while (millis () - now < 600) {
    servo_S.write(0);
    }
    int rightR = sonar.ping_cm();
    uint32_t now1 = millis();
   while (millis () - now1 < 600) {
    servo_S.write(180);
    }
    int leftR = sonar.ping_cm();
    uint32_t now2 = millis();
    while (millis () - now2 < 600) {
    servo_S.write(75);
    }
    if (leftR != 0 and rightR == 0) {
      Serial.println("препятствие СЛЕВА");
      turn_RIGHT(random(800, 900));
    }
    if (rightR != 0 and leftR == 0) {
      Serial.println("препятствие СПРАВА");
      turn_LEFT(random(800, 900));
    }
         if (rightR != 0 and leftR != 0 and rightR < 10 and leftR < 10) {
     i = 1;     
     Serial.println("тупик");
    if(random(1, 3) == 1) turn_LEFT(random(1500, 1550));
    else turn_RIGHT(random(1500, 1550));
    }
    
    if (rightR != 0 and leftR != 0 and rightR > leftR and i != 1) {
      Serial.println("препятствие СЛЕВА");
      turn_RIGHT(random(800, 900));
    }
    
    if (rightR != 0 and leftR != 0 and rightR < leftR and i != 1) {
       Serial.println("препятствие СПРАВА");
      turn_LEFT(random(800, 900));
    }

    if (rightR == 0 and leftR == 0) {
      Serial.println("препятствий НЕ ОБНАРУЖЕНО");
      if(random(1, 3) == 1) turn_LEFT(random(800, 900));
      else turn_RIGHT(random(800, 900));
    }
  }
 }
 
if ((checkSens_R() == LOW) and (checkSens_L() == LOW)) {
  STOP(200);
}


}


boolean checkSens_R() {                         // функция опроса правого ИК датчика
  if (analogRead(A4) < 600)  return(HIGH); // измеряем напругу
  else  return(LOW);  
  
}

boolean checkSens_L() {                     // функция опроса левого ИК датчика
  if (!digitalRead(6) == HIGH) return(HIGH);
   else  return(LOW); 
}

void mov_FOV() {                  // функция движения вперёд
       //int spd_L = map(spd, 1, 10, 1500, 1702);
       //int spd_R = map(spd, 1, 10, 1503, 1300);
    int spd_L;
    int spd_R;    
       if (SPD == 2) {
    spd_L = spd_L_HIGH;
    spd_R = spd_R_HIGH;
  }
  else {
    spd_L = spd_L_LOW;
    spd_R = spd_R_LOW; 
  }
  servo_L.writeMicroseconds(spd_L);                   
  servo_R.writeMicroseconds(spd_R);
}

void mov_BACK(int TIME) {    // функция движения назад
  int spd_L ;
   int spd_R;
  if (SPD == 2) {
    TIME = TIME/3.5;
    spd_L = spd_L_HIGH_REV;
    spd_R = spd_R_HIGH_REV;
  }
  else {
    spd_L = spd_L_LOW_REV;
    spd_R = spd_R_LOW_REV; 
  }
       // byte spd_L = map(spd, 1, 10, 93, 78);
       // byte spd_R = map(spd, 1, 10, 92, 108);
  uint32_t now = millis();
  while (millis () - now < TIME) {
  servo_L.writeMicroseconds(spd_L);
  servo_R.writeMicroseconds(spd_R);
 
  }
}

void turn_RIGHT (int TIME) {               // функция движения вправо
  //byte spd_L = map(spd, 1, 10, 93, 108);
  //byte spd_R = map(spd, 1, 10, 92, 108);
   int spd_L ;
   int spd_R;
  if (SPD == 2) {
    TIME = TIME/3.5;
    spd_L = spd_L_HIGH;
    spd_R = spd_R_HIGH_REV;
  }
  else {
    spd_L = spd_L_LOW;
    spd_R = spd_R_LOW_REV; 
  }
  uint32_t now = millis();
while (millis () - now < TIME) {
  servo_L.writeMicroseconds(spd_L);
  servo_R.writeMicroseconds(spd_R);
}
}

void turn_RIGHT_ONE() {               // вправо ОДНИМ колесом для доворота к краю стола
   int spd_L ;
  if (SPD == 2) {
    spd_L = spd_L_HIGH;
  }
  else {
   spd_L = spd_L_LOW;
  }
             // byte spd_L = map(spd, 1, 10, 93, 78);
  while (checkSens_L() == HIGH) {
  servo_R.writeMicroseconds(spd_L);
}
}

void turn_LEFT_ONE() {               // влево ОДНИМ колесом для доворота к краю стола
           // byte spd_R = map(spd, 1, 10, 92, 108);
    int spd_R ;
  if (SPD == 2) {
    spd_R = spd_R_HIGH;
  }
  else {
   spd_R = spd_R_LOW;
  }     
  while (checkSens_R() == HIGH) {
  servo_L.writeMicroseconds(spd_R);
}
}

void turn_LEFT (int TIME) {               // функция движения влево
 
            //byte spd_L = map(spd, 1, 10, 93, 78);
            //byte spd_R = map(spd, 1, 10, 92, 78);
   int spd_L ;
   int spd_R;
  if (SPD == 2) {
    TIME = TIME/3.5;
    spd_L = spd_L_HIGH_REV;
    spd_R = spd_R_HIGH;
  }
  else {
    spd_L = spd_L_LOW_REV;
    spd_R = spd_R_LOW;
  }        
  uint32_t now = millis();
  while (millis () - now < TIME) {
  servo_L.writeMicroseconds(spd_L);
  servo_R.writeMicroseconds(spd_R);
}
}

void STOP(int TIME) {                // остановка
   
  if (SPD == 2) TIME = TIME/3.5;
  uint32_t now = millis();
  while (millis () - now < TIME) {
  servo_L.writeMicroseconds(spd_L_STOP);
  servo_R.writeMicroseconds(spd_R_STOP);
}
}


