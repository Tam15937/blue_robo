// Абстрактный родительский класс
const int stepPins[] = {28, 26, 24, 22, 23, 25, 27};
const int dirPins[] = {36, 34, 32, 30, 31, 33, 35};
const int stopoPins[] = {42, 44, 46, 48, 49, 47, 43};

/*
TODO 
1. одновременное движение двигателей
2. обработка поворота через датчики или просто навсидку
3. ускорение для движения
4. регулировка максимальной скорости
*/

class Axes {
protected:
    int v,a,w;  // Скорость, ускорение, угол поворота


public:
    virtual void steps(int count) = 0;  // Чисто виртуальный метод
    virtual void check() = 0;  // Чисто виртуальный метод
    virtual ~Axes() = default; // Виртуальный деструктор
	void microDelay(int k) {
		k = k * 4;
		long int microseconds = micros();
		while (micros() - k < microseconds) {
			microseconds += 1;
		}
	}
	
};

// Классы-наследники для осей 1-6
class Axis1 : public Axes {
public:
	Axis1 (int v, int a, int w){
		this->v = v;
		this->a = a;
		this->w = w;
	}
    void steps(int count) override {
		int pin_motor = stepPins[0];
        int pin_dir = dirPins[0];
        digitalWrite(pin_dir, (count > 0) ? HIGH : LOW);

        for (int i = 0; i < abs(count); i++) {
            microDelay(165);
            digitalWrite(pin_motor, HIGH);
            microDelay(v); // Higher 165 -> slower
            digitalWrite(pin_motor, LOW);
        }
	}
    void check() override {
		
	}
};

class Axis2 : public Axes {
public:
	Axis2 (int v, int a, int w){
		this->v = v;
		this->a = a;
		this->w = w;
	}
    void steps(int count) override {
		int pin_motor1 = stepPins[1];
        int pin_dir1 = dirPins[1];
		int pin_motor2 = stepPins[2];
        int pin_dir2 = dirPins[2];
		auto sig_dir =(count > 0) ? HIGH : LOW;
        digitalWrite(pin_dir1, sig_dir);
		digitalWrite(pin_dir2, sig_dir);
        for (int i = 0; i < abs(count); i++) {
            microDelay(165);
            digitalWrite(pin_motor1, HIGH);
			digitalWrite(pin_motor2, HIGH);
            microDelay(v); // Higher 165 -> slower
            digitalWrite(pin_motor1, LOW);
			digitalWrite(pin_motor2, LOW);
        }
	}
    void check() override {}
};

class Axis3 : public Axes {
public:
	Axis3 (int v, int a, int w){
		this->v = v;
		this->a = a;
		this->w = w;
	}
    void steps(int count) override {
		int pin_motor = stepPins[3];
        int pin_dir = dirPins[3];
        digitalWrite(pin_dir, (count > 0) ? HIGH : LOW);

        for (int i = 0; i < abs(count); i++) {
            microDelay(165);
            digitalWrite(pin_motor, HIGH);
            microDelay(v); // Higher 165 -> slower
            digitalWrite(pin_motor, LOW);
        }
	}
    void check() override {}
};

class Axis4 : public Axes {
public:
	Axis4 (int v, int a, int w){
		this->v = v;
		this->a = a;
		this->w = w;
	}
    void steps(int count) override {
		int pin_motor = stepPins[4];
        int pin_dir = dirPins[4];
        digitalWrite(pin_dir, (count > 0) ? HIGH : LOW);

        for (int i = 0; i < abs(count); i++) {
            microDelay(165);
            digitalWrite(pin_motor, HIGH);
            microDelay(v); // Higher 165 -> slower
            digitalWrite(pin_motor, LOW);
        }
	}
    void check() override {}
};

class Axis5 : public Axes {
public:
	Axis5 (int v, int a, int w){
		this->v = v;
		this->a = a;
		this->w = w;
	}
    void steps(int count) override {
		int pin_motor1 = stepPins[5];
        int pin_dir1 = dirPins[5];
		int pin_motor2 = stepPins[6];
        int pin_dir2 = dirPins[6];
		auto sig_dir =(count > 0) ? HIGH : LOW;
        digitalWrite(pin_dir1, sig_dir);
		digitalWrite(pin_dir2, !sig_dir);

        for (int i = 0; i < abs(count); i++) {
            microDelay(165);
            digitalWrite(pin_motor1, HIGH);
			digitalWrite(pin_motor2, HIGH);
            microDelay(v); // Higher 165 -> slower
            digitalWrite(pin_motor1, LOW);
			digitalWrite(pin_motor2, LOW);
        }
	}
    void check() override {}
};

class Axis6 : public Axes {
public:
	Axis6 (int v, int a, int w){
		this->v = v;
		this->a = a;
		this->w = w;
	}
    void steps(int count) override {
		int pin_motor1 = stepPins[5];
        int pin_dir1 = dirPins[5];
		int pin_motor2 = stepPins[6];
        int pin_dir2 = dirPins[6];
		auto sig_dir =(count > 0) ? HIGH : LOW;
        digitalWrite(pin_dir1, sig_dir);
		digitalWrite(pin_dir2, sig_dir);

        for (int i = 0; i < abs(count); i++) {
            microDelay(165);
            digitalWrite(pin_motor1, HIGH);
			digitalWrite(pin_motor2, HIGH);
            microDelay(v); // Higher 165 -> slower
            digitalWrite(pin_motor1, LOW);
			digitalWrite(pin_motor2, LOW);
        }
	}
    void check() override {}
};
