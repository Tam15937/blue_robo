const int stepPins[] = {28, 26, 24, 22, 23, 25, 27, 29};
const int dirPins[] = {36, 34, 32, 30, 31, 33, 35, 37};
const int stopoPins[] = {42, 44, 46, 48, 49, 47, 45, 43};

#define enable 40
#define stool 7


class States {
private:
    int steps[7]; // Массив для хранения состояний двигателей
    int sensor[5]; // Массив для хранения состояний датчиков

public:
    // Конструктор: инициализируем значением 0
    MotorStates() {
        for (int i = 0; i < 7; i++) {
            steps[i] = 0;
        }
        for (int i = 0; i < 5; i++) {
            sensor[i] = 0;
        }
    }

    // Получить текущее состоянее для двигателя по его номеру (от 0 до 6)
    int getSteps(int motorIndex) const {
        if (motorIndex >= 0 && motorIndex < 7) {
            return steps[motorIndex];
        } else {
            return 0; // Неверный индекс
        }
    }

    // Установить новое значение сдвига
    void setSteps(int motorIndex, int newSteps) {
        if (motorIndex >= 0 && motorIndex < 7) {
            steps[motorIndex] = newSteps;
        }
    }

    // Добавить сдвиг 
    void addSteps(int motorIndex, int deltaSteps) {
        if (motorIndex >= 0 && motorIndex < 7) {
            steps[motorIndex] += deltaSteps;
        }
    }

    // Сброс состояний
    void resetAll() {
        for (int i = 0; i < 7; i++) {
            steps[i] = 0;
        }
        for (int i = 0; i < 5; i++) {
            sensor[i] = 0;
        }
    }
};

// Экземпляр класса
States motorStates;

// Функция движения двигателя на заданное количество шагов
void moveMotor(int motorIndex, int stepsToMove) {
    if (motorIndex < 0 || motorIndex >= 7) return;

    pinMode(dirPins[motorIndex], OUTPUT);
    pinMode(stepPins[motorIndex], OUTPUT);

    digitalWrite(enable, LOW); // Включаем двигун

    if (stepsToMove > 0) {
        digitalWrite(dirPins[motorIndex], HIGH); // Направление вправо
    } else {
        digitalWrite(dirPins[motorIndex], LOW); // Направление влево
    }

    stepsToMove = abs(stepsToMove);

    for (int i = 0; i < stepsToMove; i++) {
        digitalWrite(stepPins[motorIndex], HIGH);
        delayMicroseconds(500); // Подстрой под нужную скорость
        digitalWrite(stepPins[motorIndex], LOW);
        delayMicroseconds(500);
    }

    digitalWrite(enable, HIGH); // Отключаем двигун после завершения
}

// Парсинг строки и вызов функций
void processCommand(const String& command) {
    int count = command.toInt();
    command = command.substring(command.indexOf(' ') + 1); // Убираем первое число

    for (int i = 0; i < count * 2; i += 2) {
        int motorIndex = command.substring(i*2, i*2 + 1).toInt(); // Индекс двигателя
        int steps = command.substring(i*2 + 1, i*2 + 1 + 4).toInt(); // Шаги

        moveMotor(motorIndex, steps); // Двигаем
        motorStates.addSteps(motorIndex, steps); // Обновляем состояние
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(enable, OUTPUT);
    digitalWrite(enable, HIGH); // По умолчанию выключен

    for (int i = 0; i < 5; i++) {
        pinMode(stopoPins[i], INPUT_PULLUP); // Инициализация стоп-датчиков
    }
}

// Пример отправки 3 1 300 3 -400 6 100 (3(кол-во двигателей) 1(индекс двигателя) 300(количество шагов и направление) 3(№) -400(шаги) 6(№) 100(шаги))
void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        int firstSpace = input.indexOf(' ');
        if (firstSpace != -1) {
            int commandCount = input.substring(0, firstSpace).toInt();
            String rest = input.substring(firstSpace + 1);

            processCommand(rest);
        }
    }
}



