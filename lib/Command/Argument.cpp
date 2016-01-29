#include "Arduino.h"
#include "Argument.h"

Argument::Argument(String arg) {//TODO: test
	arg.trim();
	int dividerPos = arg.indexOf(' ');
	key = arg.substring(0, dividerPos);
	if (dividerPos != -1) {
		value = arg.substring(dividerPos);
		value.trim();
	}
}

String Argument::getKey() {
	return key;
}

String Argument::getValue() {
	return value;
}

String Argument::toString() {
	return (value.length() > 0) ? (key + " " + value) : key;
}