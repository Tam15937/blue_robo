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
      microseconds + 1;
  }
}

void getInst(Instruction* inst) {
    // Read the incoming bytes into the struct
    Serial.readBytes((char*)inst, sizeof(Instruction));
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
}

void loop() {    
  // Process the Command
  if (Serial.available() == sizeof(Instruction)) {
    Instruction inst; // Declare Instruction here
    getInst(&inst);   // Pass a pointer to inst

    Serial.print("I get: ");
    Serial.print(inst.number); // Print number
    Serial.print(" ");          // Print space
    Serial.print(inst.direction); // Print direction (true/false)
    Serial.print(" ");          // Print space
    Serial.print(inst.count);   // Print count
    Serial.println();   
    
    step(int(inst.number), inst.direction, inst.count); // Execute the motor step command
  }
}