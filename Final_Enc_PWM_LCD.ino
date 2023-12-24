#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Пины энкодера
#define encPinA 6  // CLK
#define encPinB 5  // DT
// Кнопка энкодера
#define encBut 2
// Пин сервопривода
#define servo_pin 13
// Пин кнопки настройки
#define but_settings 3


LiquidCrystal_I2C lcd(0x27,20,4); // Установка адреса дисплея 4x20 

bool encA;
bool encB;

byte prevState;
volatile bool enc_flag = false;
volatile bool set_flag = false;


int freq = 50;    // Частота ШИМ (Гц)
int duty_down = 500; // Нижняя граница ~0 градусов
int duty_up = 2400;  // Верхняя граница ~180 градусов

int duty = duty_down;
int degree = 0;
int enc_val = 0;

void encoder(){
  enc_val = 0;
  encA = digitalRead(encPinA);
  encB = digitalRead(encPinB);
  if ((encA) && (encB)){
    if (prevState==B00000001){
      enc_val = -1;
    }
    else if (prevState==B00000010){
      enc_val = 1;
    }
    prevState=B00000011;
  }
  else if ((!encA) && (encB)){
    if (prevState==B00000000){
      enc_val = -1;
    }
    else if (prevState==B00000011){
      enc_val = 1;
    }
    prevState=B00000001;
  }
  else if ((encA) && (!encB)){
    if (prevState==B00000011){
      enc_val = -1;
    }
    else if (prevState==B00000000){
      enc_val = 1;
    }
    prevState=B00000010;
  }  
}

void servo_go(){  // Функция создаёт ШИМ сигнал с частотой freq, и заполнением равным переменной duty
    lcd.setCursor(3,2);
    lcd.print("Servo moving!");
    duty = map(degree, 0, 180, duty_down, duty_up);
    for (int i = 1; i<25; i++){
      int duration = 1/freq*10000;
      digitalWrite(servo_pin, HIGH);  
      delayMicroseconds(duty);
      digitalWrite(servo_pin, LOW);
      delayMicroseconds(duration - duty);
    }
    lcd.clear();
    print_info();

}
void encoder_flag(){
  enc_flag = true;
}
void settings_flag(){
  set_flag = true;
}

void print_info(){
  lcd.setCursor(3,0);
  lcd.print("degree");
  lcd.setCursor(11,0);
  lcd.print("PWM duty");
  lcd.setCursor(7,1);
  lcd.print("'");
  lcd.setCursor(16,1);
  lcd.print("mc");
  lcd.setCursor(1,3);
  lcd.print("Freq:");
  printing_value(5, 3, freq);
  lcd.setCursor(9 ,3);
  lcd.print("hz");
}
void print_settings(){
  lcd.setCursor(5,0);
  lcd.print("Settings");
}

void printing_value(int col, int line, int value){ // Корректно печатает значения до 9999
  if ((value < 1000) && (value >= 100)){
    lcd.setCursor(col+1, line);
    lcd.print(value);
  }
  else if ((value < 100) && (value >= 10)){
    lcd.setCursor(col+2, line);
    lcd.print(value);
  }
  else if (value < 10) {
    lcd.setCursor(col+3, line);
    lcd.print(value);
    }
  else {
    lcd.setCursor(col, line);
    lcd.print(value);
  }
}

void check_print_value(int value){
  int value2 = map(degree, 0, 180, duty_down, duty_up);
  if ((value == 9) || (value == 99) || (value2 > 990 && value2 < 1000)){
    lcd.clear();
    print_info();
  }
}

void setup() {
  pinMode(encPinA, INPUT_PULLUP);// Подтяжка к плюсу
  pinMode(encPinB, INPUT_PULLUP);
  pinMode(encBut, INPUT_PULLUP);
  pinMode(but_settings, INPUT_PULLUP);
  pinMode(servo_pin, OUTPUT);

  attachInterrupt(0, encoder_flag, FALLING); // Подключение прерывания
  attachInterrupt(1, settings_flag, FALLING);

  lcd.init(); // Инициализация дисплея
  lcd.backlight();
  print_info();
}

void loop() {
  if (enc_flag){
    servo_go();
    enc_flag = false;
  }
  else if (set_flag){
    lcd.clear();
    print_settings();
    while (true){
      delay(3000);
      break;
    }
    set_flag = false;
    lcd.clear();
  }

  encoder();
  if (enc_val != 0){
    check_print_value(degree);
  }
  degree += enc_val;
  if (degree < 0) {
    degree = 180;
    }
  else if (degree > 180) {
    degree = 0;
    lcd.clear();
    print_info();
    }

  printing_value(3, 1, degree);

  duty = map(degree, 0, 180, duty_down, duty_up);

  printing_value(12, 1, duty);
}
