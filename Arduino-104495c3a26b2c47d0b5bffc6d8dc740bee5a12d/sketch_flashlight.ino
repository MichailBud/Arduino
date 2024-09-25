// используемые пины
const int encPinA = 5;  // A
const int encPinB = 6;  // B
const int pin_led = 9;  // пин для подачи питания на светодиод
const int pin_but = 3;  // пин кнопки

int mode = 0;
int pre_mode = mode;

bool encA; // действующее значение на выводе энкодера
bool encB;

bool preA=0; // предыдущее значение энкодера
bool preB=0;

int value = 10;
int max_value = 200;
int  enc_val = 0;

int light = 100; //задержка, свет включен, микросекунд
float dark = 1; //свет выключен, микросекунды
float min_dark = 1; //миминмальная задержка темноты 1000 Герц
float max_dark = 30; //максимальная задержка темноты 33 Герца
byte potent_pin = 5;  //аналоговый пин потенциометра

bool activate = 1;

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
  if (pre_mode != mode ){ // Борьба с дребезгом контактов
    delay(200);
    pre_mode++;
    mode = pre_mode;
    if (mode > 2) {
      mode = 0;
      pre_mode = 0;
    }

  }
  enc_val = 0;
  encoder();
  switch (mode){
      case 0: // Режим регулировки яркости
        value += enc_val;
        analogWrite(pin_led, value);
        if (value > max_value) {
        value = 0;
        }
        else if (value < 0){
        value = max_value;
        }
        break;
      case 1: // Режим стробоскопа
        dark += enc_val*0.1;
        digitalWrite(pin_led, 1); // Включаем свет
        delayMicroseconds(light); // ждем
        digitalWrite(pin_led, 0); // выключаем
        delay(dark);
        Serial.println(dark);
        break;
      case 2: // Режим моргалки
        encA = digitalRead(encPinA);
        analogWrite(pin_led, value*encA);
        break;
    }
}


void mode_flag(){
  mode++;
}

void pr(){
  //Serial.print(encA);
  //Serial.println(encB);
  //Serial.print(preA);
  //Serial.println(preB);
  //Serial.println(value);
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
    }

  else if ((encA)&&(!encB)){
    if ((!preA)&&(!preB)){
      enc_val = 1;
    }
    else if ((preA)&&(preB)){
      enc_val = -1;
    }
    }

  else if ((!encA)&&(!encB)){
    if ((preA)&&(preB)){
      enc_val = 1;
    }
    else if ((preA)&&(!preB)){
      enc_val = -1;
    }
    }
  preA=encA;
  preB=encB;
  pr();  
}



