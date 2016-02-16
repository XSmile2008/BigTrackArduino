#include "Argument.h"

Argument::Argument(byte key, byte size, void* value) {
  Serial.println(F("Argument.constructor()"));
  Argument::key = key;
  Argument::size = size;
  Argument::value = memcpy(malloc(size), value, size);
}

Argument::~Argument() {
  Serial.println(F("Argument.destructor()"));
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

void Argument::print() {
  Serial.print(F("arg.key = ")); Serial.print(key);
  Serial.print(F(" | arg.size = ")); Serial.println(size);
  for(int i = 0; i < size; i++) {
    Serial.print(F("byte")); Serial.print(i); Serial.print(F("@")); Serial.print((long) value + i); Serial.print(F(" = "));
    Serial.print(*(byte*) ((int) value + i)); Serial.print(F(", "));
  } Serial.println();
}
