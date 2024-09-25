// используемые пины
const int encPinA = 5;  // A
const int encPinB = 6;  // B

bool encA; // действующее значение на выводе энкодера
bool encB;

bool preA=0; // предыдущее значение энкодера
bool preB=0;

int value = 1;

void pr(){
  //Serial.print(encA);
  //Serial.println(encB);
  //Serial.print(preA);
  //Serial.println(preB);
  Serial.println(value);
}

void setup() {
  Serial.begin(9600);
  pinMode(encPinA, INPUT_PULLUP);
  pinMode(encPinB, INPUT_PULLUP);
  Serial.println(value);
}

void loop() {
  encA = digitalRead(encPinA);
  encB = digitalRead(encPinB);
  if ((encA)&&(encB)){
    if ((preA)&&(!preB)){
      value++;
    }
    else if ((!preA)&&(!preB)){
      value--;
    }
    pr();
    }

  else if ((encA)&&(!encB)){
    if ((!preA)&&(!preB)){
      value++;
    }
    else if ((preA)&&(preB)){
      value--;
    }
    pr();
    }

  else if ((!encA)&&(!encB)){
    if ((preA)&&(preB)){
      value++;
    }
    else if ((preA)&&(!preB)){
      value--;
    }
    pr();
    }
  preA=encA;
  preB=encB;
  pr();  
}














