

// используемые пины
const int encPinA = 6;  // CLK
const int encPinB = 5;  // DT

bool encA;
bool encB;

bool prev11;
bool clockwise1=true;
bool clockwise2=false;

int value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(encPinA, INPUT);
  digitalWrite(encPinA, HIGH);  // Подтяжка к плюсу
  pinMode(encPinB, INPUT);
  digitalWrite(encPinB, HIGH);
  Serial.println(value);
}

void loop() {
  encA = digitalRead(encPinA);
  encB = digitalRead(encPinB);
  if ((encA)&&(encB)&&(!prev11)){
    if ((clockwise1)&&(clockwise2)){
      value++;
    }
    if ((!clockwise1)&&(!clockwise2)){
      value--;
    }
    prev11 = true;
    Serial.println(value);
  }
  else if ((!encA)&&(encB)&&(prev11)){
    clockwise1 = true;
    prev11 = false;
  }
  else if ((encA)&&(!encB)&&(prev11)){
    clockwise1 = false;
    prev11 = false;
  }
  else if ((!encA)&&(encB)&&(!prev11)){
    clockwise2 = false;
  }
  else if ((encA)&&(!encB)&&(!prev11)){
    clockwise2 = true;
  }
}
