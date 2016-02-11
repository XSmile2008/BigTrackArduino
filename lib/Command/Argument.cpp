#include "Argument.h"

Argument::Argument(byte key, byte size, void* value) {
  Serial.println("Argument.constructor()");
  Argument::key = key;
  Argument::size = size;
  Argument::value = memcpy(malloc(size), value, size);
}

Argument::~Argument() {
  Serial.println("Argument.destructor()");
  free(value);
}

byte Argument::getKey() {
  return key;
}

byte Argument::getSize() {
  return size;
}

void* Argument::getValue() {
  return value;
}

void Argument::toString() {
  Serial.print("arg.key = "); Serial.println(key);
  Serial.print("arg.size = "); Serial.println(size);
  Serial.print("arg.value@"); Serial.print((long) value); Serial.print(" = "); Serial.println(*(int*) value);
  for(int i = 0; i < size; i++) {
    Serial.print("byte"); Serial.print(i); Serial.print("@"); Serial.print((long) value + i); Serial.print(" = ");
    Serial.print(*(byte*) (value + i)); Serial.print(", ");
  } Serial.println();
}
