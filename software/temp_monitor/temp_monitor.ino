#include <Wire.h>

// Temperature monitor: reads diode sensors through a mux, reports over I2C, flags >60C

const uint8_t NUM_CHANNELS = 8;                 // 8 for new mux, 16 for old
const uint8_t SELECT_PINS[4] = {2, 3, 4, 5};
const uint8_t SIG_PIN   = A0;
const uint8_t FAULT_LED = 6;
const uint8_t I2C_ADDR  = 0x08;                 

// temp conversion numbers (measure these for real)
const float VREF     = 1.1;                     // the voltage the Arduino compares against when reading
const float V_AT_CAL = 0.600;                   // diode voltage at room temp (approximation)
const float T_CAL    = 25.0;
const float TEMPCO   = -0.002;                  // diode voltage drops around 2 mV per degree
const float FAULT_C  = 60.0;

uint8_t txBuf[1 + 2 * NUM_CHANNELS];            // txBuf layout: [0] = fault flag, then 2 bytes per sensor (high byte first)

void selectChannel(uint8_t ch) {
  // TODO: write ch in binary onto the 4 select pins
}

float readTempC(uint8_t ch) {
  // TODO: select channel, wait a bit, throw away first read
  // TODO: average 8 reads, convert to volts, then to degrees C
}

void onRequestHandler() {
  // TODO: send txBuf
}

void setup() {
  // TODO: ADC reference to INTERNAL
  // TODO: pin modes
   // TODO: join I2C as a slave using I2C_ADDR
  // TODO: tell I2C to run onRequestHandler when the master asks for data
  // TODO: start Serial at 9600 so we can see stuff on the computer
}

void loop() {
  // TODO: read every sensor, set fault if over FAULT_C
  // TODO: pack temps into a temp buffer (tenths of a degree, 2 bytes each)
  // TODO: set LED, then copy buffer into txBuf with interrupts off
}