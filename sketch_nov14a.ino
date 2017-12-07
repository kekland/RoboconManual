void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  //Serial2.begin(9600);
}

int stoif(String s) {
  int n = 0;

  int i = 0;
  bool neg = false;
  
  if(s[0] == '-') {
    neg = true;
    i = 1;
  }
  
  for(; i < s.length(); i++) {
    n *= 10;
    n += (s[i] - '0');
  }

  n *= (neg)? -1 : 1;
  
  return n;
}

struct stick {
  int x;
  int y;
};
struct driver {
  stick leftStick;
  stick rightStick;

  bool buttonY;
  bool buttonB;
  bool buttonA;
  bool buttonX;

  bool buttonLB;
  bool buttonRB;
  bool buttonLT;
  bool buttonRT;

  bool dpadUp;
  bool dpadRight;
  bool dpadDown;
  bool dpadLeft;

  bool precisionEnabled;
  void parseCommand(String c) {
    //Serial.println(c);
    String cmd = "";
    int num = 0;
    for(int i = 0; i < c.length(); i++) {
      if(c[i] == ';') {
        switch(num) {
          case 0: leftStick.x = stoif(cmd); break;
          case 1: leftStick.y = stoif(cmd);  break;
          case 2: rightStick.x = stoif(cmd); break;
          case 3: rightStick.y = stoif(cmd); break;
          case 4: buttonY = (cmd == "1"); break;
          case 5: buttonB = (cmd == "1"); break;
          case 6: buttonA = (cmd == "1"); break;
          case 7: buttonX = (cmd == "1"); break;
          case 8: buttonLB = (cmd == "1"); break;
          case 9: buttonLT = (cmd == "1"); break;
          case 10: buttonRB = (cmd == "1"); break;
          case 11: buttonRT = (cmd == "1"); break;
          case 12: dpadUp = (cmd == "1"); break;
          case 13: dpadRight = (cmd == "1"); break;
          case 14: dpadDown = (cmd == "1"); break;
          case 15: dpadLeft = (cmd == "1"); break;
          case 16: precisionEnabled = (cmd == "1"); break;
       }
       cmd = "";
       num++;
      }
      else {
        cmd += c[i];
      }
    }  
  }
};

int convertBtToAnalog(int in) {
  in = abs(in);

  if(in >= 80) {
    in = 100;
  }
  //in -> 0 - 100
  //out -> 0 - 255
  return (in / 100.0) * 255.0;
}

//PINS


driver d1;
String cmd_d1;

void configureMotor(int pin1, int pin2, int pinS, int stick, int desiredSpeed = -1) {
  if(desiredSpeed == -1) {
    analogWrite(pinS, convertBtToAnalog(abs(stick)));
  }
  else {
    analogWrite(pinS, desiredSpeed);
  }
  //Serial.println(stick);
  if(stick > 0) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  }
  else if(stick < 0) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }
  else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
}

void configureMotorBoolean(int pin1, int pin2, bool btn1, bool btn2) {
  if(btn1) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  }
  else if(btn2) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }
  else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
}

//Pins
int rightWheels_first_1 = 24;
int rightWheels_first_2 = 25;
int rightWheels_first_s = 3;
int rightWheels_second_1 = 22;
int rightWheels_second_2 = 23;
int rightWheels_second_s = 2;

int leftWheels_first_1 = 34;
int leftWheels_first_2 = 35;
int leftWheels_first_s = 4;

int leftWheels_second_1 = 36;
int leftWheels_second_2 = 37;
int leftWheels_second_s = 5;

int liftFirst_1 = 26;
int liftFirst_2 = 27;

int clawFirst_1 = 28;
int clawFirst_2 = 29;

int liftSecond_1 = 30;
int liftSecond_2 = 31;

int clawSecond_1 = 32;
int clawSecond_2 = 33;

void debugSerial() {
  Serial.print(d1.leftStick.x);
  Serial.print(" ");
  Serial.print(d1.leftStick.y);
  Serial.print(" ");
  Serial.print(d1.rightStick.x);
  Serial.print(" ");
  Serial.print(d1.rightStick.y);
  Serial.print(" ");
  Serial.print(d1.buttonY);
  Serial.print(" ");
  Serial.print(d1.buttonB);
  Serial.print(" ");
  Serial.print(d1.buttonA);
  Serial.print(" ");
  Serial.print(d1.buttonX);
  Serial.print(" ");
  Serial.print(d1.buttonLB);
  Serial.print(" ");
  Serial.print(d1.buttonLT);
  Serial.print(" ");
  Serial.print(d1.buttonRB);
  Serial.print(" ");
  Serial.print(d1.buttonRT);
  Serial.print("\n");
}
void loop() {
  //digitalWrite(2, HIGH);
  //digitalWrite(3, LOW);
  while(Serial1.available()) {
    char x = Serial1.read();
    //Serial.print(x);
    if(x != '#') {
      cmd_d1 += x;
    }
    else {  
      //Serial.println(cmd_d1);
      //qqSerial.println(cmd_d1);
      d1.parseCommand(cmd_d1);
      cmd_d1 = "";
    }
  }
  //debugSerial();
  //Serial.println(d1.leftStick.y);
  double multiplier = 1.0;
  if(d1.precisionEnabled) {
    multiplier = 0.6;
  }
  configureMotor(leftWheels_first_2, leftWheels_first_1, leftWheels_first_s, d1.leftStick.y * multiplier);
  configureMotor(leftWheels_second_2, leftWheels_second_1, leftWheels_second_s, d1.leftStick.y * multiplier);
  
  configureMotor(rightWheels_first_1, rightWheels_first_2, rightWheels_first_s, d1.rightStick.y * multiplier);
  configureMotor(rightWheels_second_1, rightWheels_second_2, rightWheels_second_s, d1.rightStick.y * multiplier);

  //Serial.println(d1.selectedClaw);
  configureMotorBoolean(liftFirst_1, liftFirst_2, d1.buttonLB, d1.buttonLT);
  configureMotorBoolean(liftSecond_1, liftSecond_2, d1.buttonRT, d1.buttonRB);

  configureMotorBoolean(clawFirst_1, clawFirst_2, d1.buttonY, d1.buttonX);
  configureMotorBoolean(clawSecond_1, clawSecond_2, d1.buttonA, d1.buttonB);

  // TODO : ADD THIRD CLAW
  delay(10);
}

