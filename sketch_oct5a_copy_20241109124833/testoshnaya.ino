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
    // Конструктор: инициализируем все двигатели значением 0
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


void setup() {
  Serial.begin(115200); 
}

void loop() {

}
