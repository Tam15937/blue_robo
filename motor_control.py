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
        # Convert the instruction to bytes in sequence
        return struct.pack('B', self.number) + \
               struct.pack('?', self.direction) + \
               struct.pack('I', self.count)

# Function to send instruction over UART
def send_instruction(ser: serial.Serial, instruction: Instruction):
    data = instruction.to_bytes()
    ser.write(data)

# Initialize serial communication
def initialize_serial(port='COM4', baud_rate=115200, timeout=1):
    ser = serial.Serial(port, baud_rate, timeout=timeout)
    time.sleep(2)  # Wait for the connection to establish
    return ser