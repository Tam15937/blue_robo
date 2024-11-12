#include <avr/io.h>
#include <avr/interrupt.h>

const int stepPins[] = {28, 26, 24, 22, 23, 25, 27, 29};
const int dirPins[] = {36, 34, 32, 30, 31, 33, 35, 37};
const int stopoPins[] = {42, 44, 46, 48, 49, 47, 45, 43};
#define enable 40
#define stool 7



void setup() {
  // Настраиваем пины как выходы
  for(int i=22;i<50;i++){
    i<42? pinMode(i, OUTPUT) : pinMode(i, INPUT);
  }
  digitalWrite(enable, LOW);
}

void microDelay(int k){
  k=k*4;
  long int microseconds = micros();// счетчик микросекунд, что бы запомнить состояние micros() и сделать над ним операцию +1 
  while(micros()-k<microseconds){
      microseconds+1;
  }
}

void step(int motorIndex, bool direction) {
    if (motorIndex < 1 || motorIndex > 8) return; // Проверка на допустимый индекс
    int t_s = stepPins[motorIndex - 1];
    int t_d = dirPins[motorIndex - 1];

    digitalWrite(t_d, direction ? HIGH : LOW);
    digitalWrite(t_s, LOW);
    microDelay(23);//23 optimal 21 fast
    digitalWrite(t_s, HIGH);
    microDelay(23); 
}
void loop() {
  if(digitalRead(stopoPins[0])==LOW){
    for (int i = 1; i <= 6; i++) {
        step(i, false);
    }
  }
}