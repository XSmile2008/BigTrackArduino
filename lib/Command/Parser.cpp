#include "Parser.h"

Parser::Parser() {
  bufferLength = 0;
  buffer = new byte[bufferLength];
}

Parser::~Parser() {
  delete[] buffer;
}

List<Command*>* Parser::parse(byte* data, uint8_t dataLength) {
  if (bufferLength > MAX_BUFFER_LEGHT) trim(bufferLength / 2);
  if (bufferLength == 0) {
    int start = searchStart(data, dataLength, 0);
    if (start != NOT_FIND) {
      bufferLength = dataLength - start + 1;
      buffer = (byte*) realloc(buffer, bufferLength);
      memcpy(buffer, data + start, bufferLength);
    }
  } else {
    buffer = (byte*) realloc(buffer, bufferLength + dataLength);
    memcpy(buffer + bufferLength, data, dataLength);
    bufferLength += dataLength;
  }

  List<Command*>* commands = new ArrayList<Command*>();
  while (true) {
    Serial.println("Buffer length = " + String(bufferLength));//TODO: remove
    Command* command = searchCommand();
    if (command != NULL) commands->add(command);
    else break;
  }
  return commands;
}

/**
     * used for trim buffer
     * to cut off unused data from start of @buffer,
     * and delete already parsed commands
     *
     * @param from search from this position to find new command start, if not find delete all buffer
     */
void Parser::trim(uint16_t from) {
  int16_t start = searchStart(buffer, bufferLength, from);
  if (start == NOT_FIND) {//TODO:
    free(buffer);
    bufferLength = 0;
  } else if (start != 0) {
    bufferLength = bufferLength - start;//TODO: test this!
    memcpy(buffer, buffer + start, bufferLength);
    buffer = (byte*) realloc(buffer, bufferLength);
  }
}

uint16_t Parser::searchStart(byte* buffer, uint16_t lenght, uint16_t from) {
  for (uint16_t i = from; i < lenght; i++) {
    if (buffer[i] == Command::COMMAND_START[0]) {
      uint8_t errors = 0;
      int16_t overflow = (i + Command::COMMAND_START_LENGTH) - lenght;
      for (uint8_t j = 1; j < Command::COMMAND_START_LENGTH - (overflow > 0 ? overflow : 0); j++) {
        if (buffer[i + j] != Command::COMMAND_START[j]) errors++;
      }
      if (errors == 0) return i;
    }
  }
  return NOT_FIND;
}

uint16_t Parser::searchEnd(byte* buffer, uint16_t lenght, uint16_t from) {
  // from = from - Command::COMMAND_END_LENGTH >= 0 ? from : Command::COMMAND_END_LENGTH;
  for (uint16_t i = from; i < lenght; i++) {
    if (buffer[i] == Command::COMMAND_END[Command::COMMAND_END_LENGTH - 1]) {
      uint8_t errors = 0;
      for (uint8_t j = 0; j < Command::COMMAND_END_LENGTH; j++) {
        if (buffer[i - Command::COMMAND_END_LENGTH + j + 1] != Command::COMMAND_END[j]) errors++;
      }
      if (errors == 0) return i;
    }
  }
  return NOT_FIND;
}

Command* Parser::searchCommand() {
  for (int16_t start = searchStart(buffer, bufferLength, 0); start != NOT_FIND; start = searchStart(buffer, bufferLength, start + 1)) {
    for (int16_t end = searchEnd(buffer, bufferLength, start + Command::EMPTY_COMMAND_LENGTH - 1); end != NOT_FIND; end = searchEnd(buffer, bufferLength, end + 1)) {
      Command* command = Command::deserialize(buffer + start, end - start + 1);
      if (command != NULL) {
        trim(end + 1);
        return command;
      }
    }
  }
  return NULL;
}
