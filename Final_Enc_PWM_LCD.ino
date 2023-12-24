
// Пины энкодера
#define encPinA 6  // CLK
#define encPinB 5  // DT
// Кнопка энкодера
#define encBut 2
// Пин сервопривода
#define servo_pin 13

bool encA;
bool encB;

byte prevState;


volatile int value = 0;

void encoder(float value_add){
  if ((encA) && (encB)){
    if (prevState==B00000001){
      value-=value_add;
    }
    else if (prevState==B00000010){
      value+=value_add;
    }
    prevState=B00000011;
  }
  else if ((!encA) && (encB)){
    if (prevState==B00000000){
      value-=value_add;
    }
    else if (prevState==B00000011){
      value+=value_add;
    }
    prevState=B00000001;
  }
  else if ((encA) && (!encB)){
    if (prevState==B00000011){
      value-=value_add;
    }
    else if (prevState==B00000000){
      value+=value_add;
    }
    prevState=B00000010;
  }
}
void reset(){
  value = 0;
}




void setup() {
  Serial.begin(9600);
  pinMode(encPinA, INPUT_PULLUP);// Подтяжка к плюсу
  pinMode(encPinB, INPUT_PULLUP);
  Serial.println(value);
  attachInterrupt(0, reset, FALLING); // Подключение прерывания

void loop() {
  encA = digitalRead(encPinA);
  encB = digitalRead(encPinB);
  encoder(1);
  Serial.println(value);

}
