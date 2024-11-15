#include <avr/io.h>
#include <avr/interrupt.h>

const int stepPins[] = {28, 26, 24, 22, 23, 25, 27, 29};
const int dirPins[] = {36, 34, 32, 30, 31, 33, 35, 37};
const int stopoPins[] = {42, 44, 46, 48, 49, 47, 45, 43};
#define enable 40
#define stool 7

void microDelay(int k);

struct Instruction {
    unsigned char number;
    bool direction;
    int count;
};

volatile Instruction inst; // Объявляем переменную для хранения инструкции
volatile bool instructionReady = false; // Флаг готовности инструкции

void step(int motorIndex, bool direction, int count) {
    if (motorIndex < 1 || motorIndex > 8) return; // Check for valid index
    int pin_motor = stepPins[motorIndex - 1];
    int pin_dir = dirPins[motorIndex - 1];

    digitalWrite(pin_dir, direction ? HIGH : LOW);
    for (int i = 0; i < count; i++) {
        digitalWrite(pin_motor, LOW);
        microDelay(165); // Optimal delay
        digitalWrite(pin_motor, HIGH);
        microDelay(165); // Higher -> slower
    }
}

void microDelay(int k) {
    k = k * 4;
    long int microseconds = micros();
    while (micros() - k < microseconds) {
        // Ничего не делаем
    }
}

void setup() {
    // Set pins as outputs
    for (int i = 22; i < 50; i++) {
        if (i < 42) {
            pinMode(i, OUTPUT);
        } else {
            pinMode(i, INPUT);
        }
    }
    digitalWrite(enable, LOW);

    Serial.begin(9600); // Initialize serial port at 9600 baud
    UCSR0B |= (1 << RXCIE0); // Включаем прерывание по приему данных
}

ISR(USART_RX_vect) {
    static uint8_t byteCount = 0;
    static uint8_t buffer[sizeof(Instruction)];

    buffer[byteCount] = UDR0; // Считываем принятый байт из регистра данных

    byteCount++;
    
    if (byteCount >= sizeof(Instruction)) {
        byteCount = 0; // Сбрасываем счетчик
        memcpy(&inst, buffer, sizeof(Instruction)); // Копируем данные в структуру
        instructionReady = true; // Устанавливаем флаг готовности инструкции
    }
}

void loop() {    
    // Проверяем наличие готовой инструкции
    if (instructionReady) {
        instructionReady = false; // Сбрасываем флаг готовности
        step(inst.number, inst.direction, inst.count); // Выполняем команду мотора
    }
}