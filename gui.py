import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QDial
from PyQt5.QtCore import pyqtSignal, QObject
from motor_control import Instruction, send_instruction, initialize_serial


# Signal emitter class for communication between GUI and main logic
class Communicate(QObject):
    angle_changed = pyqtSignal(int)


class MotorControlApp(QWidget):
    def __init__(self):
        super().__init__()

        # Initialize serial communication
        self.ser = initialize_serial()

        # Initialize UI
        self.init_ui()

        # Signal emitter
        self.communicate = Communicate()
        self.communicate.angle_changed.connect(self.send_motor_command)

    def init_ui(self):
        self.setWindowTitle("Motor Control with QDial")
        self.setGeometry(100, 100, 300, 200)

        # Layout
        layout = QVBoxLayout()

        # Label to display current angle
        self.angle_label = QLabel("Angle: 0°", self)
        layout.addWidget(self.angle_label)

        # QDial for motor control
        self.dial = QDial(self)
        self.dial.setRange(-180, 180)
        self.dial.setValue(0)
        self.dial.valueChanged.connect(self.on_dial_changed)
        layout.addWidget(self.dial)

        self.setLayout(layout)

    def on_dial_changed(self, value):
        # Update label with current angle
        self.angle_label.setText(f"Angle: {value}°")

        # Emit signal with the new angle
        self.communicate.angle_changed.emit(value)

    def send_motor_command(self, angle):
        # Calculate steps based on angle (10 steps per degree)
        steps = abs(angle * 10)
        direction = angle > 0  # Clockwise if positive, counterclockwise if negative

        # Send instruction to Arduino
        send_instruction(self.ser, Instruction(1, direction, steps))

    def closeEvent(self, event):
        # Close the serial connection when the app is closed
        self.ser.close()
        event.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MotorControlApp()
    window.show()
    sys.exit(app.exec_())