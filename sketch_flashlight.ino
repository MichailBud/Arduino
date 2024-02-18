// используемые пины
const int encPinA = 5;  // A
const int encPinB = 6;  // B
const int pin_led = 9;  // пин для подачи питания на светодиод
const int pin_but = 3;  // пин для подачи питания на светодиод

int mode = 0;

bool encA; // действующее значение на выводе энкодера
bool encB;

bool preA=0; // предыдущее значение энкодера
bool preB=0;

int value = 1;
int  enc_val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pin_led, OUTPUT);
  pinMode(encPinA, INPUT_PULLUP);
  pinMode(encPinB, INPUT_PULLUP);
  pinMode(pin_but, INPUT_PULLUP);

  attachInterrupt(1, mode_flag, FALLING);

  Serial.println(value);
}

void loop() {
  enc_val = 0;
  encoder();
  switch (mode){
      case 0: // Режим регулировки яркости
        value += enc_val;
        analogWrite(pin_led, value);
        if (value > 200) {
        value = 0;
        }
        else if (value < 0){
        value = 200;
        }
        break;
      case 1:
        break;
      case 2:
        break;
    }
}


void mode_flag(){
  mode++;
  if (mode > 2) {
    mode = 0;
  }
}

void pr(){
  //Serial.print(encA);
  //Serial.println(encB);
  //Serial.print(preA);
  //Serial.println(preB);
  Serial.println(value);
}

void encoder(){
  encA = digitalRead(encPinA);
  encB = digitalRead(encPinB);
  if ((encA)&&(encB)){
    if ((preA)&&(!preB)){
      enc_val = 1;
    }
    else if ((!preA)&&(!preB)){
      enc_val = -1;
    }
    pr();
    }

  else if ((encA)&&(!encB)){
    if ((!preA)&&(!preB)){
      enc_val = 1;
    }
    else if ((preA)&&(preB)){
      enc_val = -1;
    }
    pr();
    }

  else if ((!encA)&&(!encB)){
    if ((preA)&&(preB)){
      enc_val = 1;
    }
    else if ((preA)&&(!preB)){
      enc_val = -1;
    }
    pr();
    }
  preA=encA;
  preB=encB;
  pr();  
}



