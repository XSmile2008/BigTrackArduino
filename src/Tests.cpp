#include "Tests.h"

void Tests::lifeTest() {
  int lifetime = 0;
  while (true) {
    Serial.println(lifetime++);
    for (int i = 0; i < 100; i++) printf_P(PSTR("nyan"));
    delay(1000);
  }
}

void Tests::listTest() {
  printf_P(PSTR("Free memory before test: %d\n"), freeMemory());
  printf_P(PSTR("create list: "));
  ArrayList<int> *list = new ArrayList<int>();
  list->print();

  printf_P(PSTR("add 20 items: "));
  for (int i = 0; i < 20; i++) list->add(i);
  list->print();

  printf_P(PSTR("remove [5]: "));
  list->remove(5);
  list->print();

  printf_P(PSTR("trim: "));
  list->trimToSize();
  list->print();

  printf_P(PSTR("remove [0]: "));
  list->remove(0);
  list->print();

  printf_P(PSTR("add 5 @[4]: "));
  list->add(4, 5);
  list->print();

  printf_P(PSTR("remove [size - 1]: "));
  list->remove(list->size() - 1);
  list->print();

  printf_P(PSTR("add 11 items: "));
  for (int i = 0; i < 11; i++) list->add(i);
  list->print();

  printf_P(PSTR("clear: "));
  list->clear();
  list->print();

  Serial.println(list->get(11));

  printf_P(PSTR("add 11 items: "));
  for (int i = 0; i < 11; i++) list->add(i);
  list->print();

  delete list;
  printf_P(PSTR("after delete: %d\n"), freeMemory());
}

void Tests::circularBufferTest() {
  printf_P(PSTR("Free memory before test: %d\n"), freeMemory());
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
  printf_P(PSTR("Free memory before after test: %d\n"), freeMemory());
}

void Tests::parserTest() {
  printf_P(PSTR("\nstart of test: %d\n"), freeMemory());

  Parser* parser = new Parser();
  printf_P(PSTR("new Parser(): %d\n"), freeMemory());

  const uint8_t dataLength = 24;
  byte* data = new byte[dataLength] {0,0,0,0, 58,58,100,2, 101,2,11,7, 102,2,11,12, 13,10,    /*100,100,100,100,*/    58,58,97,0,13,10};
  //byte* data = new byte[dataLength] {58, 100,2, 101,1,11, 102,2,11,12, 13,10};
  //byte* data = new byte[dataLength] {58, 100, 0, 13,10};

  printf_P(PSTR("after init test data: %d\n"), freeMemory());
  List<Command*>* commands = parser->parse(data, dataLength);
  for (uint8_t i = 0; i < commands->size(); i++) {
    byte* bytes;
    uint16_t slength;
    commands->get(i)->serialize(bytes, slength);
    Serial.write(bytes, slength);
    delete[] bytes;
    printf_P(PSTR("after send command: %d\n"), freeMemory());
    delete commands->get(i);
  }

  delete[] data;
  delete commands;
  delete parser;
  printf_P(PSTR("end of test: %d\n"), freeMemory());
}

void Tests::commandTest() {
  Serial.println();
  printf_P(PSTR("start of test: %d\n"), freeMemory());

  uint8_t test = 2;
  switch (test) {
    case 0: {
      const uint8_t length = 4;
      byte* data = new byte[length] {101,2,11,7};
      Argument* a = new Argument(data, length);
      // Argument* a = new Argument(data[0], data[1], &data[2]);
      a->print();
      delete a;
      delete[] data;
      printf_P(PSTR("end of test: %d\n"), freeMemory());
      return;
    }
    case 1: {
      const uint8_t length = 14;
      byte* data = new byte[length] {58,58,100,2, 101,2,11,7, 102,2,11,12, 13,10};
      printf_P(PSTR("after init test data: %d\n"), freeMemory());

      Command* command = Command::deserialize(data, length);
      printf_P(PSTR("after deserialize: %d\n"), freeMemory());

      delay(3000);
      int i = 100;
      while (i-- > 0) {
        byte* bytes;
        uint16_t slength;
        command->serialize(bytes, slength);
        Serial.write(bytes, slength);
        delete[] bytes;
        printf_P(PSTR("after send command: %d\n"), freeMemory());
      }

      delete command;
      printf_P(PSTR("after delete command: %d\n"), freeMemory());

      delete[] data;
    }
    case 2: {
      Command* command = new Command('n');
      int64_t nyan = 117;
      command->getArguments()->add(new Argument('n', 8, &nyan));
      command->getArguments()->add(new Argument('y', 8, &nyan));
      command->getArguments()->add(new Argument('a', 8, &nyan));
      command->getArguments()->add(new Argument('n', 8, &nyan));
      command->getArguments()->add(new Argument('n', 8, &nyan));
      command->getArguments()->add(new Argument('y', 8, &nyan));
      command->getArguments()->add(new Argument('a', 8, &nyan));
      command->getArguments()->add(new Argument('n', 8, &nyan));
      command->getArguments()->add(new Argument('n', 8, &nyan));
      command->getArguments()->add(new Argument('y', 8, &nyan));
      command->getArguments()->add(new Argument('a', 8, &nyan));
      command->getArguments()->add(new Argument('n', 8, &nyan));
      command->getArguments()->add(new Argument('n', 8, &nyan));
      command->getArguments()->add(new Argument('y', 8, &nyan));
      command->getArguments()->add(new Argument('a', 8, &nyan));
      command->getArguments()->add(new Argument('n', 8, &nyan));
      delay(1000);
      printf_P(PSTR("toSerial():start - %lu\n"), micros());
      command->toSerial();
      printf(PSTR("toSerial():end - %lu\n"), micros());
      delay(3000);
      printf(PSTR("serialize():start - %lu\n"), micros());
      byte* bytes;
      uint16_t slength;
      command->serialize(bytes, slength);
      Serial.write(bytes, slength);
      delete[] bytes;
      printf(PSTR("serialize():end - %lu\n"), micros());

      delete command;
    }
  }

  printf_P(PSTR("end of test: %d\n"), freeMemory());
}

void Tests::printBytes(byte bytes[], int size) {
  printf_P(PSTR("bytes[%d]: --------------------\n"), size);
	for (int i = 0; i < size; i++) Serial.print((char) bytes[i]);
	Serial.println();
	for (int i = 0; i < size; i++) {Serial.print(bytes[i]); Serial.print(F(", "));}
	Serial.println(F("\r\n--------------------"));
}
