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
    BAUD_RATE = 115200

    # Initialize serial communication
    with serial.Serial(COM_PORT, BAUD_RATE, timeout=1) as ser:
        time.sleep(2)  # Wait for the connection to establish
        while True:
            # Получаем ввод от пользователя
            inst = input("Введите три значения (1 - 8, 0 или 1, любое целое число): ").split(' ')
            # Проверяем, что введено ровно три значения
            if len(inst) == 3:
                try:
                    num1 = int(inst[0])
                    num2 = int(inst[1])
                    num3 = int(inst[2])

                    # Проверяем условия для первого и второго чисел
                    if num1 in (1,2,3,4,5,6,7,8) and num2 in (0, 1):
                        # Если условия выполнены, отправляем инструкцию
                        send_instruction(ser, Instruction(num1, bool(num2), num3))
                        print(f'send num = {num1}, dir = {bool(num2)}, count = {num3}')
                        time.sleep(1)  # Ждем немного перед отправкой следующей инструкции
                    else:
                        print("Ошибка: первое 1 до 8, второе число должны быть 0 или 1.")
                except ValueError:
                    print("Ошибка: Пожалуйста, введите целые числа.")
            else:
                print("Ошибка: Необходимо ввести ровно три значения.")

if __name__ == "__main__":
    main()
    print("Hi")