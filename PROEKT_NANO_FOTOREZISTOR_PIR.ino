/*Изменения:
Вывод статуса в монитор порта:
Добавлен вывод статуса фоторезистора, PIR-датчика и реле в монитор порта.
Теперь в консоли всегда отображается текущее состояние каждого компонента.
Оптимизация условий:
Условия для управления реле и активности PIR-датчика теперь находятся внутри проверки на темноту,
чтобы избежать ненужных проверок, когда светло.
*/
#define LED_PIN 13 // Светодиод на 13 пине
#define PHOTORESISTOR_PIN A0 // Фоторезистор на аналоговом входе A0
#define PIR_PIN 2 // PIR датчик на цифровом входе 2
#define RELAY_PIN 7 // Реле освещения на цифровом входе 7

#define BLINK_INTERVAL 1000 // Интервал мигания светодиода (мс)
#define DARKNESS_THRESHOLD 500 // Пороговое значение для темноты
#define MOTION_TIMEOUT 10000 // Таймаут выключает реле после движения (10 секунд)

unsigned long lastBlinkTime = 0;
bool ledState = false;

bool relayOn = false;
unsigned long lastMotionTime = 0; // Время последнего обнаружения движения
bool pirActive = false; // Флаг активности PIR-датчика

// Переменные для замедленного вывода
unsigned long lastPrintTime = 0;
const long printInterval = 1000; // Интервал вывода в монитор порта (1 секунда)

void setup() {
 pinMode(LED_PIN, OUTPUT);
 pinMode(PIR_PIN, INPUT);
 pinMode(RELAY_PIN, OUTPUT);
 Serial.begin(9600);
}

void loop() {
 unsigned long currentTime = millis();

 // Чтение значения с фоторезистора
 int lightValue = analogRead(PHOTORESISTOR_PIN);

 // Контроль датчика PIR
 bool pirDetected = (digitalRead(PIR_PIN) == HIGH);

 // Управление реле
 if (lightValue < DARKNESS_THRESHOLD) { // Если темно
 if (pirDetected && !relayOn) { // Если PIR сработал и реле выключено
 digitalWrite(RELAY_PIN, HIGH); // Включаем реле
 relayOn = true; // Устанавливаем флаг включенного реле
 pirActive = true; // Устанавливаем флаг активности PIR-датчика
 lastMotionTime = currentTime; // Запоминаем время обнаружения движения
 } else if (pirDetected && relayOn) { // Если PIR сработал и реле уже включено
 lastMotionTime = currentTime; // Обновляем время обнаружения движения
 pirActive = true;
 }

 if (relayOn) { // Если реле включено
 if (!pirDetected && (currentTime - lastMotionTime >= MOTION_TIMEOUT)) { // Если нет движения и прошло время таймаута
 digitalWrite(RELAY_PIN, LOW); // Выключаем реле
 relayOn = false; // Сбрасываем флаг включенного реле
 pirActive = false; // Сбрасываем флаг активности PIR-датчика
 }
 }
 } else { // Если светло
 if (relayOn) { // если реле было включено, то выключаем
 digitalWrite(RELAY_PIN, LOW);
 relayOn = false;
 pirActive = false;
 }
 }

 // Мигание светодиода только в темное время
 if (lightValue < DARKNESS_THRESHOLD && currentTime - lastBlinkTime >= BLINK_INTERVAL) {
 ledState = !ledState;
 digitalWrite(LED_PIN, ledState);
 lastBlinkTime = currentTime;
 }

 // Вывод в монитор порта статуса светодиода, реле, PIR-датчика и фоторезистора
 if (currentTime - lastPrintTime >= printInterval) {
 Serial.print("Light: ");
 Serial.print(lightValue);
 Serial.print(", PIR: ");
 Serial.print(pirDetected);
 Serial.print(", Relay: ");
 Serial.println(relayOn ? "ON" : "OFF");
 lastPrintTime = currentTime;
 }

 delay(50);
}