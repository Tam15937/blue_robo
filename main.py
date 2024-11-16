from dis import Instruction

import serial
import struct
import time

# Define the Instruction structure
class Instruction:
    def __init__(self, number: int, direction: bool, count: int):
        self.number = number  # Motor index (1-8)
        self.direction = direction  # Direction (True for clockwise, False for counterclockwise)
        self.count = count  # Number of steps

    def to_bytes(self) -> bytes:
        # Convert the instruction to bytes
        return struct.pack('B?', self.number, self.direction) + struct.pack('I', self.count)

def send_instruction(ser: serial.Serial, instruction: Instruction):
    # Send the instruction over UART
    data = instruction.to_bytes()
    ser.write(data)
    for i in range(7):
        if ser.in_waiting > 0:  # Check if there is data available
            message = ser.readline().decode('utf-8').rstrip()  # Read and decode the message
            print(message)  # Print the received message

def main():
    # Configure the serial port (adjust 'COM_PORT' and 'BAUD_RATE' as needed)
    COM_PORT = 'COM3'  # Change this to your Arduino's COM port
    BAUD_RATE = 9600

    # Initialize serial communication
    with serial.Serial(COM_PORT, BAUD_RATE, timeout=1) as ser:
        time.sleep(2)  # Wait for the connection to establish
        while True:
            print(f'input >> ')
            inst = input().split(' ')
            if len(inst)== 3:
                send_instruction(ser, Instruction(int(inst[0]),bool(inst[1]),int(inst[2]))) #1037 is 360 degrees of motor with reductor
                print(f'send num = {inst[0]}, dir = {inst[1]}, count = {inst[2]}')
                time.sleep(1)  # Wait a bit before sending the next instruction

if __name__ == "__main__":
    main()
    print("Hi")