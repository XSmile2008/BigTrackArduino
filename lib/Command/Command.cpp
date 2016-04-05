#include "Command.h"
#include "ArrayList.cpp"

const byte Command::COMMAND_START[] = {':', ':'};
const uint8_t Command::COMMAND_START_LENGTH = 2;

const byte Command::COMMAND_END[] = {'\r', '\n'};
const uint8_t Command::COMMAND_END_LENGTH = 2;

const int Command::EMPTY_COMMAND_LENGTH = COMMAND_START_LENGTH + 2 + COMMAND_END_LENGTH;

Command::Command(byte key) {
	Command::key = key;
	arguments = new ArrayList<Argument*>();
}

Command::Command(byte key, List<Argument*>* arguments) {
	Command::key = key;
	Command::arguments = arguments;
}

Command::~Command() {
	// Serial.println(F("Command.destructor()"));
	for (uint16_t i = 0; i < arguments->size(); i++) delete arguments->get(i);
	delete arguments;
}

void Command::serialize() {//TODO: check on multy argument commands
	uint8_t size = EMPTY_COMMAND_LENGTH;
	for (uint8_t i = 0; i < arguments->size(); i++) size += Argument::OFFSET + arguments->get(i)->getSize();
	byte bytes[size];
	memcpy(bytes, COMMAND_START, COMMAND_START_LENGTH);//TODO: test
	uint8_t pos = COMMAND_START_LENGTH;
	bytes[pos++] = key;
	bytes[pos++] = arguments->size();
	for (uint8_t i = 0; i < arguments->size(); i++) {
		bytes[pos++] = arguments->get(i)->getKey();
		bytes[pos++] = arguments->get(i)->getSize();
		memcpy(&bytes[pos], arguments->get(i)->getValue(), arguments->get(i)->getSize());
		pos += arguments->get(i)->getSize();
	}
	memcpy(&bytes[pos], COMMAND_END, COMMAND_END_LENGTH);//TODO: check

	// Serial.println(F("serialized :"));
	// for (uint8_t i  = 0; i < size; i++) { Serial.print(bytes[i]); Serial.print(F(", ")); }
	// Serial.println();
	Serial.write(bytes, size);
}

Command* Command::deserialize(byte bytes[], uint16_t bytesLength) {
	if (bytesLength < EMPTY_COMMAND_LENGTH) return NULL;
	uint16_t pos = COMMAND_START_LENGTH;
	uint8_t key = bytes[pos++];
	uint8_t argsCount = bytes[pos++];

	// Serial.println(bytesLength);	Serial.print(F("key = ")); Serial.print(key);
	// Serial.print(F(" | argsCount = ")); Serial.println(argsCount);

	List<Argument*>* arguments = new ArrayList<Argument*>(argsCount);
	for (uint8_t i = 0; i < argsCount; i++) {
		uint8_t argSize = bytes[pos + Argument::SIZE];
		if (pos + Argument::OFFSET + argSize + COMMAND_END_LENGTH > bytesLength || argSize < 0) break;

		arguments->add(new Argument(bytes[pos + Argument::KEY], bytes[pos + Argument::SIZE], &bytes[pos + Argument::OFFSET]));
		pos += Argument::OFFSET + arguments->get(i)->getSize();

		// arguments->get(i)->print();
	}
	if ((arguments->size() == argsCount) && (pos + COMMAND_END_LENGTH == bytesLength)) {
		return new Command(key, arguments);
	} else {
		for (uint8_t i = 0; i < arguments->size(); i++) delete arguments->get(i);
		delete arguments;
		return NULL;
	}
}

byte Command::getKey() {
	return key;
}

void Command::setKey(byte key) {
	Command::key = key;
}

List<Argument*>* Command::getArguments() {
	return arguments;
}

void Command::setArguments(List<Argument*> arguments) {
	delete Command::arguments;//TODO: check it
	*Command::arguments = arguments;
}

Argument* Command::getArgument(byte key) {
	for (uint8_t i = 0; i < arguments->size(); i++) {
		if (arguments->get(i)->getKey() == key) return arguments->get(i);
	}
	return NULL;
}
