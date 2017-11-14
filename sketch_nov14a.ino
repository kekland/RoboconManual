void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

int lmSpeed = 0;
int rmSpeed = 0;
bool lb1 = false;
bool lb2 = false;
bool rb2 = false;
bool rb1 = false;
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

void setCommands(String c) {
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
String cmd = "";
void loop() {
  if(Serial1.available()) {
    char x = Serial1.read();
    //Serial.print(x);
    if(x != '#') {
      cmd += x;
    }
    else {
      setCommands(cmd);
      cmd = "";
    }
  }

 // Serial.println(lb1);

  if(lmSpeed > 0) {
    digitalWrite(22, HIGH);
    digitalWrite(23, LOW);
  }
  else if(lmSpeed < 0) {
    digitalWrite(22, LOW);
    digitalWrite(23, HIGH);
  }
  else {
    digitalWrite(22, LOW);
    digitalWrite(23, LOW);
  }

  if(rmSpeed > 0) {
    digitalWrite(24, HIGH);
    digitalWrite(25, LOW);
  }
  else if(rmSpeed < 0) {
    digitalWrite(24, LOW);
    digitalWrite(25, HIGH);
  }
  else {
    digitalWrite(24, LOW);
    digitalWrite(25, LOW);
  }

  if(lb1) {
    digitalWrite(26, HIGH);
    digitalWrite(27, LOW);
  }
  else if(lb2) {
    digitalWrite(26, LOW);
    digitalWrite(27, HIGH);
  }
  else {
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
  }

  if(rb2) {
    digitalWrite(28, HIGH);
    digitalWrite(29, LOW);
  }
  else if(rb1) {
    digitalWrite(28, LOW);
    digitalWrite(29, HIGH);
  }
  else {
    digitalWrite(28, LOW);
    digitalWrite(29, LOW);
  }
}
