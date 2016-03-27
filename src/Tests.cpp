#include "Tests.h"

void Tests::lifeTest() {
  int lifetime = 0;
  while (true) {
    Serial.println(lifetime++);
    for (int i = 0; i < 100; i++) Serial.print("nyan");
    delay(1000);
  }
}

void Tests::listTest() {
  Serial.print(F("free memory before create list: ")); Serial.println(freeMemory());
  Serial.print(F("create list: "));
  ArrayList<int> *list = new ArrayList<int>();
  list->print();

  Serial.print(F("add 20 items: "));
  for (int i = 0; i < 20; i++) list->add(i);
  list->print();

  Serial.print(F("remove [5]: "));
  list->remove(5);
  list->print();

  Serial.print(F("trim: "));
  list->trimToSize();
  list->print();

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

void Tests::commandTest() {
  Serial.println();
  Serial.print(F("start of test: ")); Serial.println(freeMemory());

  Parser* parser = new Parser();

  const uint8_t dataLength = 21;
  byte* data = new byte[dataLength] {58, 100,2, 101,1,11, 102,2,11,12, 13,10, 100, 100, 100, 100, 58,97,0,13,10};

  // const uint8_t dataLength = 12;
  // byte* data = new byte[dataLength] {58, 100,2, 101,1,11, 102,2,11,12, 13,10};

  //byte* data = new byte[dataLength] {58, 100, 0, 13,10};
  Command* command = parser->parse(data, dataLength);
  if (command != NULL) {
    command->serialize();
  } else {
    Serial.println(F("Command not valid"));
  }

  delete[] data;
  delete command;
  delete parser;
  Serial.print(F("end of test: ")); Serial.println(freeMemory());
}

void Tests::printBytes(byte bytes[], int size) {
	Serial.print(F("bytes[")); Serial.print(size); Serial.println(F("]: --------------------"));
	for (int i = 0; i < size; i++) Serial.print((char) bytes[i]);
	Serial.println();
	for (int i = 0; i < size; i++) {Serial.print(bytes[i]); Serial.print(F(", "));}
	Serial.println(F("\r\n--------------------"));
}
