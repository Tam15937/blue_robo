#include <avr/io.h>
#include <avr/interrupt.h>

const int stepPins[] = {28, 26, 24, 22, 23, 25, 27, 29};
const int dirPins[] = {36, 34, 32, 30, 31, 33, 35, 37};
const int stopoPins[] = {42, 44, 46, 48, 49, 47, 45, 43};

#define enable 40
#define stool 7



void microDelay(int k);

//struct Instruction {
//  unsigned char number;
//  bool direction;
//  int count;
//};

// Function to move a motor
void steps(int motorIndex-1, bool direction-1, int count) {
  if (motorIndex < 0 || motorIndex > 7) return; // Check for valid index
  digitalWrite(dirPins[motorIndex ], direction ? HIGH : LOW);
  bool p_stop=0;
  bool stop =0;
  for (int i = 0; i < count; i++) {
    // p_stop=stop;
    // bool stop = digitalRead(stopoPins[0]);

    // if((stop == 1)&&(p_stop==0) ){
    //  direction ? en++ : en--;
    // }

    // if ((en<=1)&&(direction==1)||(en>=-1)&&(direction==0)) onestep(pin_motor);
    // else{
    //   digitalWrite(pin_dir, !direction ? HIGH : LOW);
    //   for (int i = 0; i < 5; i++){
        onestep(stepPins[motorIndex]);
    //   }
    //   return;
    // }
  }
};

for(int i;i<=8;i++){
  stepPins[i];
  dirPins[i];
  digitalWrite(dirPins[motorIndex ], direction ? HIGH : LOW);
}

void art2move(bool direction, int count) {
  int pin_motor1 = stepPins[1]; 
  int pin_dir1 = dirPins[1];   
  int pin_motor2 = stepPins[2]; 
  int pin_dir2 = dirPins[2];    

  digitalWrite(pin_dir1, direction ? HIGH : LOW);
  digitalWrite(pin_dir2, direction ? HIGH : LOW);

  for (int i = 0; i < count; i++) {
    microDelay(165); 
    digitalWrite(pin_motor1, HIGH);
    digitalWrite(pin_motor2, HIGH);
    microDelay(165); // Higher 165 -> slower
    digitalWrite(pin_motor1, LOW);
    digitalWrite(pin_motor2, LOW);

  }
}

void onestep(int pin_motor){
    microDelay(165); // Optimal delay
    digitalWrite(pin_motor, HIGH);
    microDelay(165); // Higher 165 -> slower
    digitalWrite(pin_motor, LOW);
}

void microDelay(int k) {
  k = k * 4;
  long int microseconds = micros();
  while (micros() - k < microseconds) {
      microseconds + 1;
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

  Serial.begin(115200); // Initialize serial port at 115200 baud
}

void loop() {
  // Process the Command
  if (Serial.available() >= 6) { // Ensure enough bytes are available (1 byte + 1 byte + 4 bytes)
    unsigned char number = Serial.read(); // Read motor number
    bool direction = Serial.read();       // Read direction
    int count = 0;
    for (int i = 0; i < 4; i++) {         // Read count (4 bytes)
      count |= (Serial.read() << (i * 8));
    }

    if ((number == 2) || (number == 3))
      art2move(direction,count);
    else
      steps(number, direction, count);
    // Print received data
    Serial.print("I get: ");
    Serial.print(number); // Print number
    Serial.print(" ");    
    Serial.print(direction); // Print direction (true/false)
    Serial.print(" ");    
    Serial.print(count);   // Print count
    Serial.println();   
    Serial.println(en);
   

    
    
  }
   //Serial.println(digitalRead(stopoPins[0])); 

}


Нужно сделать:
1.Прием данных в вектор, формата 3(кол-во двигателей) 1(индекс двигателя) 300(количество шагов и направление) 3(№) -400(шаги) 6(№) 100(шаги)
2.Массив для контроля состояний
3.Начальная калибровка
4.Одновременное движение двигателями