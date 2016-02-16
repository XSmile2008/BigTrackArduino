#include "MemoryFree.h"
#include "Chassis.h"
#include "Command.h"
#include "Commands.h"
#include "Parser.h"
#include "Tests.h"

Chassis* chassis = NULL;

Parser parser = Parser();
void readCommands() {
  if (Serial.available()) {
    Serial.print(F("FreeMem.onCreate: ")); Serial.println(freeMemory());
    int dataLength = Serial.available();
    uint8_t* data = (uint8_t*) Serial.readBytes(new uint8_t[dataLength], dataLength);
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
  Wire.begin();
  Serial.begin(115200);
  Serial.print(F("Boot complete, free memory: ")); Serial.println(freeMemory());
  chassis = new Chassis(4, 7, 5, 6);
  Serial.print(F("Intialising chassis, free memomory: ")); Serial.println(freeMemory());
  //chassis->test();
  Tests::commandTest();
}

void loop() {
  //readCommands();
  //chassis->task();
}
