// Первый регулятор управляет временем, которое будет литься вода (от 4 до 15 секунд)
#define MAX_FLOWTIME 15 // seconds
#define MIN_FLOWTIME 4 // seconds

// Второй регулятор управляет частотой полива от раза в день до раза в неделю
#define MAX_PERIOD 7 // days
#define MIN_PERIOD 1 // days

#define MAX 1023
#define MIN 0

int volumePin = A0; // Пин, к которому подключен регулятор, отвечающий за объём поливаемой воды
int periodPin = A1; // Пин, к которому подключен регулятор, отвечающий за период между поливами
int sensorPin = A2; // Пин, к которому подключен датчик
int pumpPin = 5; // Пин, к которому подключено управление насосом
int lightPin = 12; // Пин, к которому подключен светодиод

int volume;
int period;
int wasser;

// Процедура, включающая насос на время, заданное в volume
void water() {
  digitalWrite(pumpPin, HIGH); // включаем насос
  delay(volume);
  digitalWrite(pumpPin, LOW); // выключаем насос
  delay(period);  
}


void setup() {
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW); 
  pinMode(lightPin, OUTPUT); // пин 12 со светодиодом будет выходом
  pinMode(sensorPin, INPUT);  // к входу A2 подключим датчик
  Serial.begin(9600);  // подключаем монитор порта
}

void loop() {
  // Считываем значения регуляторов (переменных резисторов) и приводим их к заданным пределам
  volume = map(analogRead(volumePin), MIN, MAX, MIN_FLOWTIME, MAX_FLOWTIME) * 1000; 
  period = map(analogRead(periodPin), MIN, MAX, MIN_PERIOD, MAX_PERIOD) * 1000 * 60 * 60 * 24; 
  water();
  wasser = analogRead(sensorPin); // переменная "wasser" находится в интервале от 0 до 1023

    if (wasser > 100) { digitalWrite(lightPin, LOW); } // выключаем светодиод
    if (wasser < 100) { digitalWrite(lightPin, HIGH); }  // включаем светодиод

  delay(1000);           // задержка в одну секунду
}
