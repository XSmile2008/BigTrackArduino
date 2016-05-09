#include <Arduino.h>
#include "HMC5883L.h"
#include "Wire.h"

HMC5883L::HMC5883L() {
  HMC5883L(0.88, Measurement_Continuous);
}

HMC5883L::HMC5883L(float gauss, uint8_t mode) {
  Wire.begin();
	setScale(gauss);
	setMeasurementMode(mode);
}

MagnetometerRaw HMC5883L::readRawAxis() {
  uint8_t* buffer = read(DataRegisterBegin, 6);
  MagnetometerRaw raw = MagnetometerRaw();
  raw.XAxis = (buffer[0] << 8) | buffer[1];
  raw.ZAxis = (buffer[2] << 8) | buffer[3];
  raw.YAxis = (buffer[4] << 8) | buffer[5];
  return raw;
}

//TODO: try use enum
int HMC5883L::setScale(float gauss) {
	uint8_t regValue = 0x00;
	if(gauss == 0.88) {
		regValue = 0x00;
		mScale = 0.73;
	} else if(gauss == 1.3) {
		regValue = 0x01;
		mScale = 0.92;
	} else if(gauss == 1.9) {
		regValue = 0x02;
		mScale = 1.22;
	} else if(gauss == 2.5) {
		regValue = 0x03;
		mScale = 1.52;
	} else if(gauss == 4.0) {
		regValue = 0x04;
		mScale = 2.27;
	} else if(gauss == 4.7) {
		regValue = 0x05;
		mScale = 2.56;
	} else if(gauss == 5.6) {
		regValue = 0x06;
		mScale = 3.03;
	} else if(gauss == 8.1) {
		regValue = 0x07;
		mScale = 4.35;
	} else return ErrorCode_1_Num;

	// Setting is in the top 3 bits of the register.
	regValue = regValue << 5;
	write(ConfigurationRegisterB, regValue);
}

void HMC5883L::setMeasurementMode(uint8_t mode) {
	write(ModeRegister, mode);
}

void HMC5883L::write(int address, int data) {
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t* HMC5883L::read(int address, int length) {
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(address);
  Wire.endTransmission();

  Wire.beginTransmission(HMC5883L_Address);
  Wire.requestFrom(HMC5883L_Address, length);

  uint8_t buffer[length];
  if(Wire.available() == length) {
	  for(uint8_t i = 0; i < length; i++) {
		  buffer[i] = Wire.read();
	  }
  }
  Wire.endTransmission();

  return buffer;
}

String HMC5883L::getErrorText(int errorCode) {
	if(ErrorCode_1_Num == 1) return ErrorCode_1;
	else return "Error not defined.";
}

float HMC5883L::getAzimuth() {
	MagnetometerRaw raw = readRawAxis();

	calibrate(&raw);
  float heading = atan2(raw.YAxis, raw.XAxis);

  if(heading < 0)
    heading += 2*PI;
  else if(heading > 2*PI)
    heading -= 2*PI;
  float azimuth = heading * 180/M_PI + 180;//TODO: hardcode, make calibration

  if (azimuth > 360) azimuth -= 360;
  else if (azimuth < 0) azimuth += 360;

  return azimuth;
}

void HMC5883L::scale(MagnetometerRaw* raw) {
  raw->XAxis *= mScale;
  raw->YAxis *= mScale;
  raw->ZAxis *= mScale;
}

void HMC5883L::calibrate(MagnetometerRaw* raw) {
  float uncalibrated_values[3] = {(float)raw->XAxis, (float)raw->YAxis, (float)raw->ZAxis};
  double calibration_matrix[3][3] = {
    { 1.011,  0.043, -0.033},
    {-0.017,  0.975,  0.163},
    { 0.018, -0.033,  1.205}
  };

  double bias[3] = {
      -5.701,
    -245.225,
     -61.258
  };

  for (int i=0; i<3; ++i) uncalibrated_values[i] -= bias[i];
  float result[3] = {0, 0, 0};
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
  raw->XAxis = result[0]; raw->YAxis = result[1]; raw->ZAxis = result[2];
}
