#include "MemoryFree.h"
#include "Chassis.h"
#include "Command.h"
#include "Commands.h"
#include "Parser.h"
#include "Tests.h"
#include "NewPing.h"//TODO: remove

//TODO: refactoring Parser
//TODO: try use structs to return array and array size

Chassis* chassis = NULL;

NewPing sonar_1 = NewPing(10, 11);
NewPing sonar_2 = NewPing(12, 13);

Parser parser = Parser();
void serialEvent() {
  while (Serial.available()) {
    Serial.print(F("FreeMem.onCreate: ")); Serial.println(freeMemory());
    Serial.println(Serial.available());

    int dataLength = Serial.available();
    byte* data = new byte[dataLength];
    Serial.readBytes(data, dataLength);

    Command* command = parser.parse(data, dataLength);
    if (command != NULL) runCommand(command);
    else Serial.println(F("Command is not valid!"));

    delete[] data;
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
  // chassis->test();
  // Tests::commandTest();
  // Tests::circularBufferTest();
}

void loop() {
  // chassis->task();
  Serial.print("distance_1 = "); Serial.println(sonar_1.ping_cm());
  Serial.print("distance_2 = "); Serial.println(sonar_2.ping_cm());
  delay(500);
}
