#include <avr/io.h>
#include <avr/interrupt.h>

const int stepPins[] = {28, 26, 24, 22, 23, 25, 27, 29};
const int dirPins[] = {36, 34, 32, 30, 31, 33, 35, 37};
#define enable 40
#define START_MARKER '<'
#define END_MARKER '>'

void microDelay(int k);
struct __attribute__((packed)) Instruction {
    uint8_t number;
    bool direction;
    uint32_t count;
};

uint8_t calculateChecksum(Instruction inst);
void sendResponse(const char* message);
void step(int motorIndex, bool direction, int count);

void microDelay(int k) {
    long int startMicros = micros();
    while (micros() - startMicros < k) {
        // Ждем заданное количество микросекунд
    }
}

void step(int motorIndex, bool direction, int count) {
    if (motorIndex < 1 || motorIndex > 8) return; // Проверка на допустимый индекс
    int pin_motor = stepPins[motorIndex - 1];
    int pin_dir = dirPins[motorIndex - 1];
    digitalWrite(pin_dir, direction ? HIGH : LOW);
    for (int i = 0; i < count; i++) {
        digitalWrite(pin_motor, LOW);
        microDelay(165); // Оптимальная задержка
        digitalWrite(pin_motor, HIGH);
        microDelay(165); // Чем выше -> тем медленнее
    }
}

uint8_t calculateChecksum(Instruction inst) {
    uint8_t checksum = 0;
    uint8_t* data = (uint8_t*)&inst;
    for (size_t i = 0; i < sizeof(Instruction); ++i) {
        checksum += data[i];
        Serial.print("Byte: ");
        Serial.print(data[i], HEX); // Отладочное сообщение
        Serial.print(", Checksum: ");
        Serial.println(checksum); // Отладочное сообщение
    }
    return checksum;
}

void sendResponse(const char* message) {
    Serial.print(START_MARKER);
    Serial.print(message);
    Serial.println(END_MARKER);
}

bool getInst(Instruction* inst) {
    static boolean recvInProgress = false;
    static byte dataIndex = 0;
    char receivedChar;
    static char data[sizeof(Instruction) + 1]; // +1 для контрольной суммы

    while (Serial.available() > 0) {
        receivedChar = Serial.read();

        Serial.print("Received char: ");
        Serial.println(receivedChar); // Отладочное сообщение

        if (!recvInProgress) {
            if (receivedChar == START_MARKER) {
                recvInProgress = true;
                dataIndex = 0;
                Serial.println("Start marker detected."); // Отладочное сообщение
            }
        } else if (receivedChar == END_MARKER) {
            recvInProgress = false;
            Serial.println("End marker detected."); // Отладочное сообщение
            if (dataIndex == sizeof(Instruction) + 1) {
                Instruction receivedInst;
                memcpy(&receivedInst, data, sizeof(Instruction));
                uint8_t receivedChecksum = data[sizeof(Instruction)];
                uint8_t calculatedChecksum = calculateChecksum(receivedInst);
                Serial.print("Received checksum: ");
                Serial.println(receivedChecksum); // Отладочное сообщение
                Serial.print("Calculated checksum: ");
                Serial.println(calculatedChecksum); // Отладочное сообщение

                if (calculatedChecksum == receivedChecksum) {
                    memcpy(inst, &receivedInst, sizeof(Instruction));
                    sendResponse("OK");
                    return true;
                } else {
                    sendResponse("Error: Checksum Mismatch");
                    return false;
                }
            } else {
                sendResponse("Error: Incomplete Data");
                return false;
            }
        } else {
            if (dataIndex < sizeof(data)) {
                data[dataIndex++] = receivedChar; // Сохранение байта в массив
                Serial.print("Data byte stored: ");
                Serial.println(receivedChar); // Отладочное сообщение
            } else {
                recvInProgress = false; // Слишком много данных
                sendResponse("Error: Too Much Data");
                return false;
            }
        }
    }
    return false; // Нет полного полученного сообщения
}

void setup() {
    for (int i = 0; i < 8; i++) {
        pinMode(stepPins[i], OUTPUT);
        pinMode(dirPins[i], OUTPUT);
    }
    pinMode(enable, OUTPUT);
    digitalWrite(enable, LOW);
    Serial.begin(115200); // Инициализация серийного порта на 115200 бод
    Serial.print("Size of Instruction: ");
    Serial.println(sizeof(Instruction)); // Должно быть 6
}

void loop() {
    Instruction inst;
    if (getInst(&inst)) {
        Serial.print("Я получил: ");
        Serial.print(inst.number);
        Serial.print(" ");
        Serial.print(inst.direction);
        Serial.print(" ");
        Serial.print(inst.count);
        Serial.println();
        step(inst.number, inst.direction, inst.count);
        delay(100); // Задержка 100 мс для обработки
    }
}