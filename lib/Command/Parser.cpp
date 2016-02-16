#include "Parser.h"

Parser::Parser() {
  bufferLength = 0;
  buffer = new byte[bufferLength];
}

Parser::~Parser() {
  delete[] buffer;
}

Command* Parser::parse(byte* data, uint8_t dataLength) {
  if (bufferLength == 0) {
    for (int i = 0; i < dataLength; i++) {
      if (data[i] == Command::COMMAND_START[0]) {
        uint8_t newBufferLenght = dataLength - i + 1;
        buffer = (byte*) realloc(buffer, newBufferLenght);
        memcpy(buffer, data + i, newBufferLenght);//TODO:check
        bufferLength = dataLength;
        break;
      }
    }
  } else {
    buffer = (byte*) realloc(buffer, bufferLength + dataLength);
    memcpy(buffer + bufferLength, data, dataLength);
    bufferLength += dataLength;
  }

  Serial.println("Buffer:");
  for (int i = 0; i < bufferLength; i++) {Serial.print(buffer[i]); Serial.print(F(", "));}//TODO: remove


  for (int start = searchStart(0); start != NOT_FIND; start = searchStart(start + 1)) {
    for (int end = searchEnd(start + Command::EMPTY_COMMAND_LENGTH - 1); end != NOT_FIND; end = searchEnd(end + 1)) {
      Serial.print("start = "); Serial.print(start); Serial.print(" | end = "); Serial.println(end);//TODO: remove
      Command* command = Command::deserialize(buffer + start, end - start + 1);
      if (command != NULL) {
        Serial.println(bufferLength);
        uint8_t from = end + (end < bufferLength - 1 ? 1 : 0);
        uint8_t to = bufferLength - 1;
        bufferLength = to - from + 1;
        buffer = (byte*) realloc(buffer, bufferLength);
        memcpy(buffer, buffer + from, bufferLength);
        return command;
      }
    }
  }
  return NULL;
}

uint16_t Parser::searchStart(uint16_t from) {
  if (from + Command::COMMAND_START_LENGTH > bufferLength) return NOT_FIND;
    for (int i  = from; i < bufferLength; i++) {
      int bytesMatch = 0;
      for (int j = 0; j < Command::COMMAND_START_LENGTH; j++) {
        if (buffer[i + j] == Command::COMMAND_START[j]) bytesMatch++;
      }
      if (bytesMatch == Command::COMMAND_START_LENGTH) return i;
    }
  return NOT_FIND;
}

uint16_t Parser::searchEnd(uint16_t from) {
  from = from - Command::COMMAND_END_LENGTH >= 0 ? from : Command::COMMAND_END_LENGTH;
  for (int i  = from; i < bufferLength; i++) {
    int bytesMatch = 0;
    for (int j = 0; j < Command::COMMAND_END_LENGTH; j++) {
      if (buffer[i - Command::COMMAND_END_LENGTH + j + 1] == Command::COMMAND_END[j]) bytesMatch++;
    }
    if (bytesMatch == Command::COMMAND_END_LENGTH) return i;
  }
  return NOT_FIND;
}
