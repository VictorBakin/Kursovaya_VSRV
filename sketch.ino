#include <TimerOne.h>

// Первый регулятор управляет временем, которое будет литься вода (от 4 до 15 секунд)
#define MAX_FLOWTIME 15 // seconds
#define MIN_FLOWTIME 4 // seconds

// Второй регулятор управляет частотой полива от раза в день до раза в неделю
#define MAX_PERIOD 7 // days
#define MIN_PERIOD 0 // days

#define MAX 1023
#define MIN 0

int volumePin = A0; // Пин, к которому подключен регулятор, отвечающий за объём поливаемой воды
int periodPin = A1; // Пин, к которому подключен регулятор, отвечающий за период между поливами
int sensorPin = A2; // Пин, к которому подключен датчик остатка воды в резервуаре
int pumpPin = 5; // Пин, к которому подключен транзистор насоса

int lightPin = 13; // Пин, к которому подключен светодиод
bool is_empty = 0; // Переменная, отвечающая за остаток воды в резервуаре
int sensor_value = 0; // Переменная, отвечающая за хранение данных об остатке в резервуаре
int volume = 0; // Переменная, отвечающая за период полива
int period = 0; // Переменная, отвечающая за время полива

// Процедура, включающая насос на время, заданное в volume, с заданным периодом
void water() {
  digitalWrite(pumpPin, HIGH); // включаем насос
  delay(volume * 1000); // Ждем заданное время
  digitalWrite(pumpPin, LOW); // выключаем насос
  delay(period * 1000 * 60 * 60 * 24); // Ждем заданное время периода
}

// Процедура, которая выводит данные в монитор порта
void print_data() {
  sensor_value = analogRead(sensorPin); // переменная "sensor_value" находится в интервале от 0 до 1023

  if (sensor_value > 100) {
    digitalWrite(lightPin, LOW); // выключаем светодиод
    is_empty = 0; // обнуляем переменную отвечающую за контроль над остатком воды
  } 
  if (sensor_value < 100) {
    digitalWrite(lightPin, HIGH); // включаем светодиод
    is_empty = 1; // резервуар пустой, поэтому присваиваем значение 1
  }  

  Serial.println("\nТекущее время полива: " + String(volume) + "\nТекущий период полива: " + String(period) + "\nОстаток воды: " + String(sensor_value));
}

void setup() {
  pinMode(volumePin, INPUT);
  pinMode(periodPin, INPUT);
  pinMode(sensorPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(lightPin, OUTPUT); // пин 13 со светодиодом будет выходом
  digitalWrite(pumpPin, LOW);
  digitalWrite(lightPin, LOW);
  Serial.begin(9600);  // подключаем монитор порта

  Timer1.initialize(10000000); // инициализируем таймер с периодом в 10 сек
  Timer1.attachInterrupt(print_data); // функция print_data будет вызываться каждые 10 сек
}

void loop() {
  if (!is_empty) {
    // Считываем значения регуляторов (переменных резисторов) и приводим их к заданным пределам
    volume = map(analogRead(volumePin), MIN, MAX, MIN_FLOWTIME, MAX_FLOWTIME);
    period = map(analogRead(periodPin), MIN, MAX, MIN_PERIOD, MAX_PERIOD);
    water();
  } else {
    delay(1000);
  }
}
