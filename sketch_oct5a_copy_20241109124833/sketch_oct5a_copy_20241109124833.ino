#include <avr/io.h>
#include <avr/interrupt.h>

const int stepPins[] = {28, 26, 24, 22, 23, 25, 27, 29};
const int dirPins[] = {36, 34, 32, 30, 31, 33, 35, 37};
const int stopoPins[] = {42, 44, 46, 48, 49, 47, 45, 43};
static int dat1=0,dat2=0,dat3=0,dat4=0,dat5=0;
#define enable 40
#define stool 7

void microDelay(int k);

//struct Instruction {
//  unsigned char number;
//  bool direction;
//  int count;
//};

// Function to move a motor
void step(int motorIndex, bool direction, int count) {
  if (motorIndex < 1 || motorIndex > 8) return; // Check for valid index
  int pin_motor = stepPins[motorIndex - 1];
  int pin_dir = dirPins[motorIndex - 1];
  int datflag = 0;
  digitalWrite(pin_dir, direction ? HIGH : LOW);
  for (int i = 0; i < count; i++) {
    digitalWrite(pin_motor, LOW);
    microDelay(165); // Optimal delay
    digitalWrite(pin_motor, HIGH);
    microDelay(165); // Higher -> slower
  }
};


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

    // Print received data
    Serial.print("I get: ");
    Serial.print(number); // Print number
    Serial.print(" ");    
    Serial.print(direction); // Print direction (true/false)
    Serial.print(" ");    
    Serial.print(count);   // Print count
    Serial.println();   

    // Execute the motor step command
    step(number, direction, count);
  }
}