void setup()
{
  pinMode(13, OUTPUT); // Подключаем управляющий провод сервопривода к 13 цифровому пину
}

void servo_go(duty, freq, pin){  // Функция создаёт импульс ШИМ сигнала с частотой freq, и заполнением равным переменной duty
    int duration = 1/freq*10000; // Период в микросекундах
    digitalWrite(pin, HIGH);  
    delayMicroseconds(duty);
    digitalWrite(pin, LOW);
    delayMicroseconds(duration - duty);
}

void loop()
{ 
    servo_pin = 13;   // Пин, к которому подключён сервопривод
    int freq = 50;    // Частота ШИМ (Гц)
    int duty_down = 800; // Нижняя граница ~0 градусов
    int duty_up = 2400;  // Верхняя граница ~180 градусов
    int duty = duty_down;  // Приравниваем основное значение заполнения ШИМ, которое будет управлять сервоприводом, равным нижней границе, чтобы сервориовд начинал крутиться с 0 градусов
  while (duty <= duty_up){  // Цикл поворачивает сервопривод от 0 до 180 градусов
    duty += 1;              // Увеличиваем заполнение, следовательно увеличиваем угол поворота сервопривода
    // Воссоздаём импульс ШИМ сигнала с частотой 50 гц, т.е. периодом = 20 мс и заполнением равным переменной duty
    servo_go(duty, freq, pin);
    delay(0.5); // Задержка, регулирующая скорость поворота сервопривода. Чем меньше значение, тем выше скорость поворота
  }
  while (duty >= duty_down){  // Цикл аналогичный первому, только поворачивает сервопривод в другую сторону от 180 до 0 градусов
    duty -= 1;
    servo_go(duty, freq, pin);
    delay(0.5);
  }

  
}