#ifndef Argument_h
#define Argument_h
#include "Arduino.h"

class Argument {
	public:
		Argument(String arg);
		String getKey();
		String getValue();
		String toString();
	private:
		String key;
		String value;
};

#endif
