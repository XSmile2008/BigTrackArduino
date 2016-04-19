#include "MemoryFree.h"
#include "Chassis.h"
#include "Command.h"
#include "Commands.h"
#include "Parser.h"
#include "Tests.h"
#include "Sonar.h"

//TODO: refactoring Parser
//TODO: try use structs to return array and array size

Chassis* chassis;
Sonar* sonar;

Parser parser = Parser();
byte* data = new byte[64];//TODO: test it!!!
void serialEvent() {
  while (Serial.available()) {
    Serial.print(F("FreeMem.onCreate: ")); Serial.println(freeMemory());
    Serial.println(Serial.available());

    int dataLength = Serial.available();
    Serial.readBytes(data, dataLength);

    Command* command = parser.parse(data, dataLength);
    if (command != NULL) runCommand(command);
    else Serial.println(F("Command is not valid!"));

    delete command;
    Serial.print(F("FreeMem.onDestroy: ")); Serial.println(freeMemory());
  }
}

void runCommand(Command* command) {
  switch (command->getKey()) {
    case STOP : {
      chassis->setAzimuth(-1, false);//TODO
      chassis->stop();
      break;
    }
    case MOVE : {
      Argument* diraction = command->getArgument(DIRACTION);
      if (diraction != NULL) chassis->move(*(byte*) diraction->getValue());
      break;
    }
    case ROTATE : {
      if (command->getArgument(AZIMUTH) != NULL) chassis->setAzimuth(*(int16_t*) command->getArgument(AZIMUTH)->getValue(), false);
      else if (command->getArgument(DIRACTION) != NULL) chassis->rotate(*(byte*) command->getArgument(DIRACTION)->getValue());
      break;
    }
    case LIFETEST : {
      Tests::lifeTest();
      break;
    }
  }
}

void setup() {
  // Wire.begin();//TODO: test without this and remove
  Serial.begin(115200);
  Serial.print(F("Boot complete, free memory: ")); Serial.println(freeMemory());
  chassis = new Chassis(4, 7, 5, 6);
  sonar = new Sonar(9, 10, 11, 12, 13);
  Serial.print(F("Intialising chassis, free memomory: ")); Serial.println(freeMemory());
  // chassis->test();
  // Tests::commandTest();
  // Tests::circularBufferTest();
  // sonar->task();
}

void loop() {
  // chassis->task();
  sonar->task();
}
