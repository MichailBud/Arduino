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
int duty_low = 500; // Нижняя граница ~0 градусов
int duty_up = 2400;  // Верхняя граница ~180 градусов
float duration = 1/(float)freq*1000000.0;

int duty = duty_low;
int angle = 0;
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
    duty = map(angle, 0, 180, duty_low, duty_up);
    for (int i = 1; i<80; i++){      
      digitalWrite(servo_pin, HIGH);  
      delayMicroseconds(duty);
      digitalWrite(servo_pin, LOW);
      delayMicroseconds(20000-duty);
    }
    lcd.clear();
    print_info();
    printing_value(3, 1, angle);
    duty = map(angle, 0, 180, duty_low, duty_up);
    printing_value(12, 1, duty);

}
void encoder_flag(){
  enc_flag = true;
}
void settings_flag(){
  set_flag = !set_flag;
}

void print_info(){  // Выводит на дисплей неизменные данные
  lcd.setCursor(3,0);
  lcd.print("Angle:");
  lcd.setCursor(11,0);
  lcd.print("PWM duty:");
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

void check_print_angle(int value){  // Очищает в нужные моменты дисплей
  int value2 = map(value, 0, 180, duty_low, duty_up);
  if ((value == 9) || (value == 99) || (value > 990 && value < 1000) || (value2 > 990 && value2 < 1000)){
    lcd.clear();
    print_info();
  }
}

void check_print_value(int value){  // Очищает в нужные моменты дисплей
  if ((value == 0) || (value == 5) || (value >= 90 && value < 100) || (value >= 990 && value < 1000)){
    lcd.clear();
    print_settings();
  }
}

int settings(int value, int change_value, int min_value, int max_value, int col, int line){
  while (!(enc_flag) && (set_flag)){ 
      encoder();
      if (enc_val != 0){
        value += enc_val * change_value;
        if (enc_val == -1){
          check_print_value(value);
        }
        if (value < min_value) {
          value = max_value;
          }
        else if (value > max_value) {
          value = min_value;
          lcd.clear();
          print_settings();
          }
        printing_value(col, line, value);
      }
    }
    delay(150);
    enc_flag = false;
    return value;
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
  printing_value(3, 1, angle);
  duty = map(angle, 0, 180, duty_low, duty_up);
  printing_value(12, 1, duty);
}

void loop() {
  if (set_flag){  // Запуск программы настройки частоты и диапазона заполнения
    lcd.clear();
    print_settings();
    delay(150);  // Задержка для устранения влияния дребезга котактов
    set_flag = true;

   
    // Настройка частоты
    lcd.setCursor(4 ,2);
    lcd.print("Freq:");
    printing_value(9, 2, freq);
    lcd.setCursor(13 ,2);
    lcd.print("hz");
    while (!((enc_flag) || (!set_flag))){ 
      encoder();
      if (enc_val != 0){
        freq += enc_val * 5;
        if (enc_val == -1){
          check_print_value(freq);
        }
        if (freq < 0) {
          freq = 1000;
          }
        else if (freq > 1000) {
          freq = 0;
          lcd.clear();
          print_settings();
          }
        printing_value(9, 2, freq);
      }
    }
    enc_flag = false;
    duration = 1/(float)freq*1000000.0;
    delay(150);
    enc_flag = false;
    


    // Настройка нижней границы диапазона
    lcd.setCursor(2 ,2);
    lcd.print("Duty cycle lower:");
    printing_value(10, 3, duty_low);
    printing_value(1, 1, duration);
    lcd.setCursor(14 ,3);
    lcd.print("mc");
    while (!((enc_flag) || (!set_flag))){ 
      encoder();
      if (enc_val != 0){
        duty_low += enc_val * 10;
        if (enc_val == -1){
          check_print_value(duty_low);
        }
        if (duty_low < 0) {
          duty_low = duration;
          }
        else if (duty_low > duration) {
          duty_low = 0;
          lcd.clear();
          print_settings();
          }
        printing_value(10, 3, duty_low);
      }
      if ((enc_flag) || (!set_flag)){ 
        
        break;
      }
    }
    delay(150);
    enc_flag = false;





    set_flag = false;
    lcd.clear();
    print_info();
  }

  else if (enc_flag && !(set_flag)){  // Запуск поворота сервопривода
    servo_go();
    enc_flag = false;
  }

  // Регулировка градуса поворота
  encoder();
  if (enc_val != 0){  // проверка на поворот энкодера, чтобы лишний раз не нагружать плату выводом данных на дисплей
    angle += enc_val;
    if (angle < 0) {
      angle = 180;
      }
    else if (angle > 180) {
      angle = 0;
      lcd.clear();
      print_info();
      }
    if (enc_val == -1){
      check_print_angle(angle);
    }
    printing_value(3, 1, angle);

    duty = map(angle, 0, 180, duty_low, duty_up);

    printing_value(12, 1, duty);
    
  }
  

}
