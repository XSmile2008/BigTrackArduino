#include "Chassis.h"
#include "Command.h"
#include "HMC5883L.h"

//TODO: write own Command class that parse String, has command, and set of argument with values
//For example ":move -fw -s 100" is move forward 100 steps
//Each command starts with ':' and end with new line

//It is not Java - check memory leaks and manualy destroy objects!!!

Chassis* chassis = NULL;

void lifeTest() {
  int lifetime = 0;
  while (true) {
    Serial.println(lifetime++);
    for (int i = 0; i < 100; i++) Serial.print("nyan");
    delay(1000);
  }
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  chassis = new Chassis(4, 7, 5, 6);
  chassis->test();
  //lifeTest();
}

void loop() {
  //read commands
  chassis->task();
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    Command* mycommand = new Command(command);
    Serial.print("\n\"");
    Serial.print(mycommand->toString());
    Serial.println("\"");

    if (mycommand->getCommand().equals(":test")) {
      chassis->test();
    } else if (mycommand->getCommand().equals(":stop")) {
      chassis->stop();
    } else if (mycommand->getCommand().equals(":move")) {
      if (mycommand->getArg("-dir")->getValue().equals("f")) {
        chassis->move(FORWARD);
      } else {
        chassis->move(BACKWARD);
      }
    } else if (mycommand->getCommand().equals(":lifetest")) {
        lifeTest();
    } else {
        Serial.println("Wrong command");
    }
  }
}
