#include <avr/io.h>
#include <avr/interrupt.h>

typedef struct  {
    uint8_t stepPin;
    uint8_t dirPins;
    uint8_t stopPins;
}Motor_t;

typedef struct  {
    unsigned char number;
    bool direction;
    int count;
}Instruction_t;



static void UART_Init(void);
static void microDelay(int k);




#define enable 40
#define stool 7

static const Motor_t Motors = {
    {28, 36, 42},
    {26, 34, 44},
    {24, 32, 46},
    {22, 30, 48},
    {23, 31, 49},
    {25, 33, 47},
    {27, 35, 45},
    {29, 37, 43},
};


// Буфер для хранения принятых данных в идеале здесь сделать FIFO но это на вас
#define BUFFER_SIZE sizeof(Instruction_t)*10
uint8_t buffer[BUFFER_SIZE];
int bufferIndex = 0;


volatile Instruction_t inst; // Объявляем переменную для хранения инструкции
volatile bool instructionReady = false; // Флаг готовности инструкции

void step(const Instruction_t * const command) {
    digitalWrite(Motors[command->number].dirPins, command->direction ? HIGH : LOW);
    for (int i = 0; i < command->count; i++) {
        digitalWrite(Motors[command->number].stepPin, LOW);
        microDelay(165); // Optimal delay
        digitalWrite(Motors[command->number].stepPin, HIGH);
        microDelay(165); // Higher -> slower
    }
}

void setup() {
    // Set pins as outputs
    for (uint8_t i = 0; i < sizeof(Motors)/sizeof(Motor_t); i++) {
        pinMode(Motors[i].stepPin, OUTPUT);
        pinMode(Motors[i].dirPins, OUTPUT);
        pinMode(Motors[i].stopPins, OUTPUT);
    }

    digitalWrite(enable, LOW);
    //отправку данных нужно будет реализовать ручками 
    UART_Init();
}


void loop() {    

    if(bufferIndex >= sizeof(Instruction_t)) {
        Instruction_t command =  ((Instruction_t*)buffer)[0] //так как нужен кольцевой буфер то пока обрабатываем 1 команду
        bufferIndex = 0;
        

    }
    // Проверяем наличие готовой инструкции
    if (instructionReady) {
        instructionReady = false; // Сбрасываем флаг готовности
        step(inst.number, inst.direction, inst.count); // Выполняем команду мотора
    }
}




void microDelay(int k) {
    k = k * 4;
    long int microseconds = micros();
    while (micros() - k < microseconds) {
        // Ничего не делаем
    }
}

void UART_Init(void){
    // Настройка UART на скорость 9600 бод
    UBRR0H = 0;
    UBRR0L = 103; // Для скорости 9600 бод при частоте 16 МГц

    // Разрешить прерывание по приему данных
    UCSR0B |= (1 << RXCIE0);

    // Включить приемник и передатчик
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    // Включить глобальные прерывания
    sei();
}




// Функция обработки прерывания по приему данных
ISR(USART0_RX_vect) {
    // Прочитать принятый байт
    char received = UDR0; //в if иф не переносить чтение данных из регистра сбрасывает флаг прерывания

    // Записать байт в буфер
    if (bufferIndex < BUFFER_SIZE) {
    buffer[bufferIndex++] = received;
    }
}


