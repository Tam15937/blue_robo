#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long microseconds = 0; // Глобальный счетчик микросекунд

// Обработчик прерывания таймера (например, Timer1)
ISR(TIMER1_COMPA_vect) {
  microseconds++;
}

#define enable 40
#define stool 7

#define stepm1 28
#define dirm1 36
#define stopo1 42

#define stepm2 26
#define dirm2 34

#define stepm3 24
#define dirm3 32

#define stepm4 22
#define dirm4 30

#define stepm5 23
#define dirm5 31

#define stepm6 25
#define dirm6 33

#define stepm7 27
#define dirm7 35

#define stepm8 29
#define dirm8 37

void setup() {
  // Настраиваем пины как выходы
  pinMode(22, OUTPUT);  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);  pinMode(29, OUTPUT);
  pinMode(30, OUTPUT);  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);  pinMode(39, OUTPUT);
  pinMode(40, OUTPUT);  pinMode(41, OUTPUT);
  pinMode(42, INPUT);  pinMode(43, OUTPUT);
  pinMode(44, OUTPUT);  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);  pinMode(47, OUTPUT);
  pinMode(48, OUTPUT);  pinMode(49, OUTPUT);
  pinMode(50, OUTPUT);  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);  pinMode(53, OUTPUT);

  digitalWrite(enable, LOW);

  // Инициализация таймера 1 (настроить частоту и режим работы по данным даташита)
  TCCR1B |= (1 << CS11); // Делитель частоты 8
  TIMSK1 |= (1 << OCIE1A); // Разрешить прерывание по сравнению
  OCR1A = 15624; // Пример значения для получения прерываний каждые 1 микросекунду при частоте 16 МГц
  sei(); // Разрешить глобальные прерывания
}


void delayMicro(unsigned long us) {
  unsigned long start = microseconds;
  while (microseconds - start < us) {
    // Ожидание прерывания таймера
  }
}

void step(int m, bool r) {
  int t_s;
  int t_d;
  switch (m) {
    case 1:
      t_s = stepm1;
      t_d = dirm1;
      break;
    case 2:
      t_s = stepm2;
      t_d = dirm2;
      break;
    case 3:
      t_s = stepm3;
      t_d = dirm3;
      break;
    case 4:
      t_s = stepm4;
      t_d = dirm4;
      break;
    case 5:
      t_s = stepm5;
      t_d = dirm5;
      break;
    case 6:
      t_s = stepm6;
      t_d = dirm6;
      break;
    case 7:
      t_s = stepm7;
      t_d = dirm7;
      break;
    case 8:
      t_s = stepm8;
      t_d = dirm8;
      break;
  }
  r == 0 ? digitalWrite(t_d, LOW) : digitalWrite(t_d, HIGH);
  digitalWrite(t_s, LOW);
  delayMicro(1); 
  digitalWrite(t_s, HIGH);
  delayMicro(1); 
}
void loop() {
  if(digitalRead(stopo1)==LOW){
    step(1,0);
  }
}