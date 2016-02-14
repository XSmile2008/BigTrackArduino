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
      if ((buffer[bufferSize - 2] == Command::COMMAND_END[0]) && (buffer[bufferSize - 1] == Command::COMMAND_END[1])) {//if find end of command
        Serial.print(F("FreeMem.onCreate: ")); Serial.println(freeMemory());
        printBytes(buffer, bufferSize);
        Command* command = Command::deserialize(buffer, bufferSize);
        if (command != NULL) runCommand(command);
        else Serial.println(F("Command is not valid!"));
        delete command;
        Serial.print(F("FreeMem.onDestroy: ")); Serial.println(freeMemory());
        bufferSize = 0;
      }
    } else if (b == Command::COMMAND_START[0]) {//else start parse new command
      buffer[bufferSize++] = b;
    }
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
      lifeTest();
      break;
    }
  }
}

void printBytes(byte bytes[], int size) {
	Serial.print(F("bytes[")); Serial.print(size); Serial.println(F("]: --------------------"));
	for (int i = 0; i < size; i++) Serial.print((char) bytes[i]);
	Serial.println();
	for (int i = 0; i < size; i++) {Serial.print(bytes[i]); Serial.print(F(", "));}
	Serial.println(F("\r\n--------------------"));
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
  chassis = new Chassis(4, 7, 5, 6);
  Serial.print(F("Intialising chassis, free memomory: ")); Serial.println(freeMemory());
  //chassis->test();
  //lifeTest();
  //listTest();
}

void loop() {
  readCommands();
  chassis->task();
}
