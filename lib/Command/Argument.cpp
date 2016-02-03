#include "Argument.h"

Argument::Argument(byte bytes[], int pos) {
  Serial.println("Argument.constructor()");
  key = bytes[pos++];
  size = 3;//bytes[pos++];
  value = malloc(size);
  memcpy(value, &bytes[pos], size);
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
  Serial.print("arg.value = "); Serial.println(*(int*) value);
  Serial.print("&arg.value = "); Serial.println((int) value);
  for(int i = 0; i < size; i++) {
    Serial.print("&byte"); Serial.print(i); Serial.print("@"); Serial.print((int) value + i); Serial.print(" = ");
    Serial.print(*(byte*) (value + i)); Serial.print(", ");
  } Serial.println();
}
