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

    List<Command*>* commands = parser.parse(data, dataLength);
    for (uint8_t i = 0; i < commands->size(); i++) {
        runCommand(commands->get(i));
        delete commands->get(i);
    }
    delete commands;

    Serial.print(F("FreeMem.onDestroy: ")); Serial.println(freeMemory());
  }
}

void runCommand(Command* command) {
  switch (command->getKey()) {
    case MOVE : {
      if (command->getArgument(STOP) != NULL) {
        chassis->setAzimuth(-1, false);//TODO:
        chassis->stop();
      } else {
        Argument* x = command->getArgument('x');
        Argument* y = command->getArgument('y');
        chassis->move(*(int16_t*) x->getValue(), *(int16_t*) y->getValue());
      }
      break;
    }
    case ROTATE : {
      if (command->getArgument(AZIMUTH) != NULL) chassis->setAzimuth(*(int16_t*) command->getArgument(AZIMUTH)->getValue(), false);
      // else if (command->getArgument(DIRACTION) != NULL) chassis->rotate(*(byte*) command->getArgument(DIRACTION)->getValue());
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
  sonar = new Sonar(10, 8, 9, 11, 12);
  Serial.print(F("Intialising chassis, free memomory: ")); Serial.println(freeMemory());
  // chassis->test();
  // Tests::listTest();
  // Tests::commandTest();
  // Tests::circularBufferTest();
  // sonar->task();
}

void loop() {
  chassis->task();
  // sonar->task();
  if (sonar->getData()->size() > 0) {
    for (uint8_t i = 0; i < sonar->getData()->size(); i++) {
      SonarData* data = sonar->getData()->popStart();
      Command* command = new Command(SONAR);
      command->getArguments()->add(new Argument(AZIMUTH, 2, &data->angle));
      command->getArguments()->add(new Argument(DISTANCE, 2, &data->distance));
      command->getArguments()->add(new Argument(TIME, 4, &data->captureTime));
      command->serialize();
      Serial.println(String(data->captureTime) + ", angle = " + String(data->angle) + ", distance = " + String(data->distance));
      delete data;
      delete command;
    }
  }
}
