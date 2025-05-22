#import "C:\axes.cpp"
#import "C:\io_serial.cpp"
#define enable 40

void setup() {

  for (int i = 22; i < 50; i++) {
    if (i < 42) {
        pinMode(i, OUTPUT);
    } else {
        pinMode(i, INPUT);
    }
  }
  digitalWrite(enable, HIGH);

  Axis1 axis1= Axis1(165,5,180);
  Axis2 axis2= Axis2(165,5,180);
  Axis3 axis3= Axis3(165,5,180);
  Axis4 axis4= Axis4(165,5,180);
  Axis5 axis5= Axis5(165,5,180);
  Axis6 axis6= Axis6(165,5,180);

  Io_serial io_serial = Io_serial();
}

void loop() {
  io_serial.read_serial();
  if(io_serial.is_empty()){
    io_serial.get_command();
  }
}