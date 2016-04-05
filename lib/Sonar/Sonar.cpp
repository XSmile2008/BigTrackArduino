#include "Sonar.h"

Sonar::Sonar(uint8_t trig0, uint8_t echo0, uint8_t trig1, uint8_t echo1) {
  sonar0 = new NewPing(trig0, echo0, 250);
  sonar0 = new NewPing(trig1, echo1, 250);
}
