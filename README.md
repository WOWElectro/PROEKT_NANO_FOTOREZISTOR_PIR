# PROEKT_NANO_FOTOREZISTOR_PIR
Этот скетч представляет собой усовершенствованную версию системы автоматического управления освещением на базе Arduino. Он использует фоторезистор для измерения уровня освещенности и PIR-датчик для обнаружения движения. В зависимости от этих данных система управляет реле, которое включает или выключает освещение. Ниже приведено подробное описание скетча.

Все подробности здесь: 
https://dzen.ru/a/Z6bt9JkjsCl0pe5d

---

### **1. Назначение и функциональность**
Скетч реализует систему автоматического управления освещением, которая:
- Включает свет, если в помещении темно и обнаружено движение.
- Выключает свет, если движение не обнаружено в течение заданного времени (10 секунд).
- Мигает светодиодом в темное время суток для визуальной индикации работы системы.
- Выводит текущее состояние системы (освещенность, состояние PIR-датчика и реле) в монитор порта.

---

### **2. Основные компоненты и их подключение**
- **Светодиод (LED_PIN = 13)**: Подключен к цифровому пину 13. Используется для визуальной индикации работы системы.
- **Фоторезистор (PHOTORESISTOR_PIN = A0)**: Подключен к аналоговому пину A0. Измеряет уровень освещенности.
- **PIR-датчик (PIR_PIN = 2)**: Подключен к цифровому пину 2. Обнаруживает движение.
- **Реле (RELAY_PIN = 7)**: Подключено к цифровому пину 7. Управляет включением и выключением освещения.

---

### **3. Константы и переменные**
#### **Константы**
- **`LED_PIN`**, **`PHOTORESISTOR_PIN`**, **`PIR_PIN`**, **`RELAY_PIN`**: Пины, к которым подключены компоненты.
- **`BLINK_INTERVAL = 1000`**: Интервал мигания светодиода (1 секунда).
- **`DARKNESS_THRESHOLD = 500`**: Пороговое значение освещенности. Если значение с фоторезистора ниже этого порога, считается, что в помещении темно.
- **`MOTION_TIMEOUT = 10000`**: Время (10 секунд), в течение которого освещение остается включенным после последнего обнаружения движения.
- **`printInterval = 1000`**: Интервал вывода данных в монитор порта (1 секунда).

#### **Переменные**
- **`lastBlinkTime`**: Время последнего изменения состояния светодиода.
- **`ledState`**: Текущее состояние светодиода (включен/выключен).
- **`relayOn`**: Флаг, указывающий, включено ли реле.
- **`lastMotionTime`**: Время последнего обнаружения движения.
- **`pirActive`**: Флаг активности PIR-датчика.
- **`lastPrintTime`**: Время последнего вывода данных в монитор порта.

---

### **4. Функция `setup()`**
В функции `setup()` выполняется начальная настройка:
- Настраиваются пины светодиода, PIR-датчика и реле на выход или вход.
- Инициализируется Serial-порт для вывода данных на компьютер.

---

### **5. Функция `loop()`**
Основная логика программы реализована в функции `loop()`. Она выполняется циклически.

#### **5.1. Чтение данных с датчиков**
- **Фоторезистор**: С помощью функции `analogRead(PHOTORESISTOR_PIN)` считывается значение уровня освещенности.
- **PIR-датчик**: С помощью функции `digitalRead(PIR_PIN)` проверяется, обнаружено ли движение.

#### **5.2. Управление реле**
- Если уровень освещенности ниже порогового значения (`lightValue < DARKNESS_THRESHOLD`):
  - Если движение обнаружено (`pirDetected == true`) и реле выключено (`relayOn == false`), реле включается, устанавливаются флаги `relayOn` и `pirActive`, а также обновляется время последнего обнаружения движения.
  - Если движение обнаружено, но реле уже включено, просто обновляется время последнего обнаружения движения.
  - Если реле включено, но движение не обнаружено в течение времени `MOTION_TIMEOUT` (10 секунд), реле выключается, а флаги `relayOn` и `pirActive` сбрасываются.
- Если уровень освещенности выше порогового значения, реле выключается (если оно было включено), и флаги сбрасываются.

#### **5.3. Мигание светодиода**
- Светодиод мигает только в темное время суток (`lightValue < DARKNESS_THRESHOLD`).
- Если прошло больше или равно 1000 мс с момента последнего изменения состояния светодиода, его состояние меняется на противоположное.

#### **5.4. Вывод данных в монитор порта**
- Каждую секунду (`printInterval = 1000`) в монитор порта выводятся текущие значения:
  - Уровень освещенности (`lightValue`).
  - Состояние PIR-датчика (`pirDetected`).
  - Состояние реле (`relayOn`).

#### **5.5. Задержка**
- В конце каждой итерации цикла `loop()` добавляется задержка на 50 мс (`delay(50)`), чтобы снизить нагрузку на процессор.

---

### **6. Логика работы системы**
1. **В темноте**:
   - Если PIR-датчик обнаруживает движение, реле включается, и освещение загорается.
   - Освещение остается включенным в течение 10 секунд после последнего обнаружения движения.
   - Если движение не обнаружено в течение 10 секунд, реле выключается.
   - Светодиод мигает каждую секунду, указывая на работу системы.
2. **На свету**:
   - Реле всегда выключено, независимо от состояния PIR-датчика.
   - Светодиод не мигает.
3. **Мониторинг**:
   - В монитор порта выводятся данные об освещенности, состоянии PIR-датчика и реле.

---

### **7. Оптимизации и улучшения**
- **Оптимизация условий**: Условия для управления реле и активности PIR-датчика теперь находятся внутри проверки на темноту, что позволяет избежать ненужных проверок, когда светло.
- **Вывод статуса**: Добавлен вывод статуса фоторезистора, PIR-датчика и реле в монитор порта. Это упрощает отладку и мониторинг работы системы.

---

### **8. Пример использования**
Эта система может быть использована для автоматического управления освещением в помещении. Например:
- В темное время суток, когда кто-то входит в комнату, освещение автоматически включается.
- Если в течение 10 секунд движение не обнаруживается, освещение выключается.
- На свету система не реагирует на движение, чтобы избежать ненужного включения освещения.

---

### **9. Возможные улучшения**
- Добавление регулировки порога освещенности через потенциометр.
- Использование более сложной логики для управления освещением (например, учет времени суток).
- Добавление возможности ручного управления реле через кнопку.

Этот скетч является простым, но эффективным примером автоматизации на основе Arduino.
