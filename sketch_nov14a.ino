void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
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

struct driver {
  int lmSpeed = 0;
  int rmSpeed = 0;
  bool lb1 = false;
  bool lb2 = false;
  bool rb2 = false;
  bool rb1 = false;

  void parseCommand(String c) {
    String cmd = "";
    int num = 0;
    for(int i = 0; i < c.length(); i++) {
      if(c[i] == ';') {
        switch(num) {
          case 0: lmSpeed = stoif(cmd); break;
          case 1: rmSpeed = stoif(cmd); break;
          case 2: lb1 = (cmd == "1"); Serial.print(cmd + ";"); Serial.print(lb1); Serial.print("\n"); break;
          case 3: lb2 = (cmd == "1"); break;
          case 4: rb2 = (cmd == "1"); break;
          case 5: rb1 = (cmd == "1"); break;
       }
       cmd = "";
       num++;
      }
      else {
        cmd += c[i];
      }
    }  
  }
}

int convertBtToAnalog(int in) {
  in = abs(in);
  //in -> 0 - 100
  //out -> 0 - 255
  return (in / 100.0) * 255.0;
}

//PINS
int wheelL_1 = 22;
int wheelL_2 = 23;
int wheelL_S = 4;

int wheelR_1 = 24;
int wheelR_2 = 25;
int wheelR_S = 5;

int lift_1 = 26;
int lift_2 = 27;

int claw_1 = 28;
int claw_2 = 29;

int projectileClaw_X_1 = 30;
int projectileClaw_X_2 = 31;
int projectileClaw_X_S = 6;

int projectileClaw_Y_1 = 32;
int projectileClaw_Y_2 = 33;
int projectileClaw_Y_S = 7;

int catapult_1 = 32;

driver d1, d2;
String cmd_d1, cmd_d2;

void configureMotor(int pin1, int pin2, int pinS, int stick) {
  analogWrite(pinS, convertBtToAnalog(abs(stick));

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
    digitalWrite(pin2, HIGH);
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

void loop() {
  if(Serial1.available()) {
    char x = Serial1.read();
    //Serial.print(x);
    if(x != '#') {
      cmd_d1 += x;
    }
    else {
      d1.parseCommand(cmd_d1);
      cmd_d1 = "";
    }
  }

  if(Serial2.available()) {
    char x = Serial2.read();
    if(x != '#') {
      cmd_d2 += x;
    }
    else {
      d2.parseCommand(cmd_d2);
      cmd_d2 = "";
    }
  }

  //Serial.println(lb1);
  configureMotor(wheelL_1, wheelL_2, wheelL_S, d1.lmSpeed);
  configureMotor(wheelR_1, wheelR_2, wheelR_S, d1.rmSpeed);
  configureMotorBoolean(lift_1, lift_2, d1.lb1, d1.lb2);
  configureMotorBoolean(claw_1, claw_2, d1.rb2, d1.rb1);

  configureMotor(projectileClaw_X_1, projectileClaw_X_2, projectileClaw_X_S, d2.lmSpeed);
  configureMotor(projectileClaw_Y_1, projectileClaw_Y_2, projectileClaw_Y_S, d2.rmSpeed);
}
