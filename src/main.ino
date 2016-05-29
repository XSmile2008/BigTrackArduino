#include "MemoryFree.h"
#include "Chassis.h"
#include "Command.h"
#include "Commands.h"
#include "Parser.h"
#include "Tests.h"
#include "Sonar.h"

//TODO: try to disable printf, try send sonar data directly without circular buffer, try handle serialEvent yourself;
//TODO: hard test CircularBuffer

static FILE uartout = { 0 };   // FILE struct
static int uart_putchar (char c, FILE *stream){
   if (c == '\n') Serial.write('\r');
   Serial.write(c);
   return 0;
}
static void printf_begin() {
  fdev_setup_stream(&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;
}

Chassis* chassis;
Sonar* sonar;

Parser parser = Parser();
byte* data = new byte[64];//TODO: test it!!!
void serialEvent() {
  while (Serial.available()) {
    // printf_P(PSTR("serialEvent(%d) - %d\n"), Serial.available(), freeMemory());//TODO: debug
    // Serial.println(Serial.available());

    int dataLength = Serial.available();
    Serial.readBytes(data, dataLength);

    List<Command*>* commands = parser.parse(data, dataLength);
    for (uint8_t i = 0; i < commands->size(); i++) {
        runCommand(commands->get(i));
        delete commands->get(i);
    }
    delete commands;

    // printf_P(PSTR("~serialEvent() - %d\n"), freeMemory());
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
  printf_begin();
  Serial.begin(115200);
  printf_P(PSTR("Boot complete, free memory: %d\n"), freeMemory());
  chassis = new Chassis(4, 7, 5, 6);
  sonar = new Sonar(10, 8, 9, 11, 12);
  printf_P(PSTR("Intialising chassis, free memomory: %d\n"), freeMemory());
  // chassis->test();
  // Tests::listTest();
  // Tests::commandTest();
  // Tests::parserTest();
  // Tests::circularBufferTest();
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
      command->toSerial();
      // printf_P(PSTR("%lu, angle = %d, distance = %d, mem = %d\n"), data->captureTime, data->angle, data->distance, freeMemory());
      delete command;
      delete data;
    }
  }
}
