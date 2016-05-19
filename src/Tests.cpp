#include "Tests.h"

void Tests::lifeTest() {
  int lifetime = 0;
  while (true) {
    Serial.println(lifetime++);
    for (int i = 0; i < 100; i++) Serial.print(F("nyan"));
    delay(1000);
  }
}

void Tests::listTest() {
  Serial.print(F("Free memory before test: ")); Serial.println(freeMemory());
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

void Tests::circularBufferTest() {
  Serial.print(F("Free memory before test: ")); Serial.println(freeMemory());
  CircularBuffer<int> *buffer =  new CircularBuffer<int>(5);
  buffer->put(5);
  buffer->put(3);
  buffer->put(10);
  buffer->put(4);
  buffer->put(5);
  buffer->put(1);
  buffer->put(9);
  buffer->put(0);

  buffer->print();

  Serial.println(F("after remove [1]"));
  buffer->remove(1);
  buffer->print();

  Serial.println(F("after put 11"));
  buffer->put(11);
  buffer->print();

  Serial.print(F("popStart = ")); Serial.println(buffer->popStart());
  buffer->print();

  Serial.print(F("popEnd = ")); Serial.println(buffer->popEnd());
  buffer->print();

  Serial.print(F("popEnd = ")); Serial.println(buffer->popEnd());
  buffer->print();

  Serial.print(F("popEnd = ")); Serial.println(buffer->popEnd());
  buffer->print();

  Serial.print(F("popEnd = ")); Serial.println(buffer->popEnd());
  buffer->print();

  Serial.println(F("after remove [2]"));
  buffer->remove(2);
  buffer->print();

  delete buffer;
  Serial.println(F("Free memory before after test: ")); Serial.println(freeMemory());
}

void Tests::commandTest() {
  Serial.println();
  Serial.print(F("start of test: ")); Serial.println(freeMemory());

  Parser parser = Parser();

  const uint8_t dataLength = 21;
  byte* data = new byte[dataLength] {58, 100,2, 101,1,11, 102,2,11,12, 13,10, 100, 100, 100, 100, 58,97,0,13,10};

  // const uint8_t dataLength = 12;
  // byte* data = new byte[dataLength] {58, 100,2, 101,1,11, 102,2,11,12, 13,10};

  //byte* data = new byte[dataLength] {58, 100, 0, 13,10};
  List<Command*>* commands = parser.parse(data, dataLength);
  for (uint8_t i = 0; i < commands->size(); i++) {
      commands->get(i)->serialize();
      delete commands->get(i);
  }

  delete[] data;
  delete commands;
  Serial.print(F("end of test: ")); Serial.println(freeMemory());
}

void Tests::printBytes(byte bytes[], int size) {
	Serial.print(F("bytes[")); Serial.print(size); Serial.println(F("]: --------------------"));
	for (int i = 0; i < size; i++) Serial.print((char) bytes[i]);
	Serial.println();
	for (int i = 0; i < size; i++) {Serial.print(bytes[i]); Serial.print(F(", "));}
	Serial.println(F("\r\n--------------------"));
}
