#include "Chassis.h"
#include "Command.h"
#include "HMC5883L.h"

//TODO: write own Command class that parse String, has command, and set of argument with values
//For example ":move -fw -s 100" is move forward 100 steps
//Each command starts with ':' and end with new line

//It is not Java - check memory leaks and manualy destroy objects!!!
//zero terminate strings by '\0'

Chassis* chassis = NULL;

void lifeTest() {
  int lifetime = 0;
  while (true) {
    Serial.println(lifetime++);
    for (int i = 0; i < 100; i++) Serial.print("nyan");
    delay(1000);
  }
}

byte buffer[64];
int bufferSize = 0;
void readCommands() {
  while(Serial.available()) {//TODO: check max command length
    byte b = Serial.read();//read byte
    if (b == ':') {//if find new command
      bufferSize == 0;
      buffer[bufferSize++] = b;
    } else if (bufferSize > 0) {//if already start parsing command
      buffer[bufferSize++] = b;
      if ((buffer[bufferSize - 2] == 13) && (buffer[bufferSize - 1] == 10)) {//find end of command
        Command* command = new Command(buffer, bufferSize);
        bufferSize = 0;
      }
    }
  }
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  chassis = new Chassis(4, 7, 5, 6);
  //chassis->test();
  //lifeTest();
}

void loop() {
  readCommands();
  chassis->task();
}
