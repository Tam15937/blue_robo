import serial
import struct
import time

class Instruction:
    def __init__(self, number: int, direction: bool, count: int):
        self.number = number
        self.direction = direction
        self.count = count

    def to_bytes(self) -> bytes:
        # Упаковка данных: number (1 байт), direction (1 байт), count (4 байта)
        return struct.pack('B?', self.number, self.direction) + struct.pack('<I', self.count)

def calculate_checksum(instruction: Instruction) -> int:
    data = instruction.to_bytes()
    checksum = sum(data) & 0xFF
    print(f"Calculated checksum: {checksum}")  # Отладочное сообщение
    return checksum

def send_instruction(ser: serial.Serial, instruction: Instruction):
    data = instruction.to_bytes()
    checksum = calculate_checksum(instruction)
    message = b'<' + data + struct.pack('B', checksum) + b'>'
    print(f"Sending message: {message}")  # Отладочное сообщение
    ser.write(message)
    time.sleep(0.1)  # Задержка перед чтением ответа
    response = ser.readline().decode('utf-8').strip()
    if response == "":
        print("Ошибка: Не получен ответ.")
    elif response == "OK":
        print("Инструкция отправлена успешно.")
    elif response.startswith("Error"):
        print(f"Ошибка от Arduino: {response}")

def main():
    COM_PORT = 'COM3'  # Укажите правильный COM-порт
    BAUD_RATE = 115200

    with serial.Serial(COM_PORT, BAUD_RATE, timeout=2) as ser:  # Увеличен таймаут
        time.sleep(2)  # Ждем инициализацию Arduino
        while True:
            inst_input = input("Введите три значения (1 - 8, 0 или 1, любое целое число): ").split()
            if len(inst_input) == 3:
                try:
                    num1 = int(inst_input[0])
                    num2 = int(inst_input[1])
                    num3 = int(inst_input[2])
                    if 1 <= num1 <= 8 and num2 in (0, 1):
                        instruction = Instruction(num1, bool(num2), num3)
                        send_instruction(ser, instruction)
                        print(f'Отправлено: num={num1}, dir={bool(num2)}, count={num3}')
                        time.sleep(0.1)
                    else:
                        print("Ошибка: первое число должно быть от 1 до 8 и второе - либо 0 либо 1.")
                except ValueError:
                    print("Ошибка: Пожалуйста, введите целые числа.")
            else:
                print("Ошибка: Необходимо ввести ровно три значения.")

if __name__ == "__main__":
    main()