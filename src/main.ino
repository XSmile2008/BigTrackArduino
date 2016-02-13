#include "MemoryFree.h"
#include "Chassis.h"
#include "Command.h"
#include "Commands.h"

#include "ArrayList.h"
#include "ArrayList.cpp"

Chassis* chassis = NULL;

//use int8_t, uint8_t

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
    if (bufferSize > 0) {//if already start parsing command
      buffer[bufferSize++] = b;
      if ((buffer[bufferSize - 2] == 13) && (buffer[bufferSize - 1] == 10)) {//if find end of command
        runCommand(new Command(buffer, bufferSize));
        bufferSize = 0;
      }
    } else if (b == ':') {//else start parse new command
      buffer[bufferSize++] = b;
    }
  }
}

void runCommand(Command* command) {
  if (command->isValid()) {
    switch (command->getKey()) {
      case STOP : {
        chassis->stop();
        break;
      }
      case MOVE : {
        Argument* diraction = command->getArg(DIRACTION);
        if (*(byte*)diraction->getValue() == FORWARD)
          chassis->move(FORWARD);
        else if (*(byte*)diraction->getValue() == BACKWARD)
          chassis->move(BACKWARD);
        break;
      }
      case ROTATE : {
        if (command->getArg(AZIMUTH) != NULL) chassis->setAzimuth(*(int16_t*) command->getArg(AZIMUTH)->getValue(), false);
        else if (command->getArg(DIRACTION) != NULL) chassis->rotate(*(byte*) command->getArg(DIRACTION)->getValue());
        break;
      }
      case LIFETEST : {
        lifeTest();
        break;
      }
    }
  } else {
    Serial.println(F("Command is not valid!"));
  }
  delete(command);
  Serial.print(F("FreeMem: ")); Serial.println(freeMemory());
}

void listTest() {
  Serial.print(F("free memory before create list: ")); Serial.println(freeMemory());
  Serial.print(F("create list: "));
  List<uint8_t> *list = new ArrayList<uint8_t>();
  list->print();

  Serial.print(F("add 20 items: "));
  for (int i = 0; i < 20; i++) list->add(i);
  list->print();

  Serial.print(F("remove [5]: "));
  list->remove(5);
  list->print();

  // Serial.print(F("trim: "));
  // list->trimToSize();
  // list->print();

  Serial.print(F("remove [0]: "));
  list->remove(0);
  list->print();

  Serial.print(F("add 5 @[4]: "));
  list->add(4, 5);
  list->print();

  Serial.print(F("remove [size - 1]: "));
  list->remove(list->size() - 1);
  list->print();

  Serial.print(F("add 11 items: "));
  for (int i = 0; i < 11; i++) list->add(i);
  list->print();

  Serial.print(F("clear: "));
  list->clear();
  list->print();

  Serial.println(list->get(11));

  Serial.print(F("add 11 items: "));
  for (int i = 0; i < 11; i++) list->add(i);
  list->print();

  delete list;
  Serial.print(F("after delete: ")); Serial.println(freeMemory());
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.print(F("Boot complete, free memory: ")); Serial.println(freeMemory());
  //chassis = new Chassis(4, 7, 5, 6);
  //Serial.print(F("Intialising chassis, free memomory: ")); Serial.println(freeMemory());
  //chassis->test();
  //lifeTest();
  //listTest();
}

void loop() {
  readCommands();
  chassis->task();
}
