#include <Wire.h>
 
// Temperature monitor: reads diode sensors through a mux, reports over I2C, flags >60C
 
const uint8_t NUM_CHANNELS = 8;                 // 8 for new mux, 16 for old
const uint8_t SELECT_PINS[4] = {10, 11, 14, 13};
const uint8_t SIG_PIN   = A0;
const uint8_t FAULT_LED = 6;
const uint8_t I2C_ADDR  = 0x08;                
 
// temp conversion numbers (measure these for real)
const float VREF     = 1.1;                     // the voltage the Arduino compares against when reading
const float V_AT_CAL = 0.600;                   // diode voltage at room temp (approximation)
const float T_CAL    = 25.0;
const float TEMPCO   = -0.002;                  // diode voltage drops around 2 mV per degree
const float FAULT_C  = 60.0;
 
uint8_t txBuffer[1 + 2 * NUM_CHANNELS];            // txBuf layout: [0] = fault flag, then 2 bytes per sensor (high byte first)
 
void selectChannel(uint8_t ch)
{
  // TODO: write ch in binary onto the 4 select pins
  for (uint8_t counter = 0; counter < 4; counter++)
  {
      uint8_t level = (ch >> counter) & 0x01;  
      digitalWrite(SELECT_PINS[counter], level ? HIGH : LOW); // Drive that select pin HIGH for a 1 bit or LOW for a 0 bit.
  }
}
 
float readTemp(uint8_t ch)
{
  // TODO: select channel, wait a bit, throw away first read
  // TODO: average 8 reads, convert to volts, then to degrees C
  selectChannel(ch);
 
  delay(2);
 
  analogRead(SIG_PIN); // throw away call.
 
  uint16_t total = 0;
 
  for(uint8_t counter = 0; counter < 8; counter++)
  {
    total = analogRead(SIG_PIN) + total;
  }
 
  float average = total/8.0;
 
  float volts = average * VREF / 1024.0;  //googled please check formulae. please confirm with data sheet.
 
  float temperature = T_CAL + (volts - V_AT_CAL) / TEMPCO; //check with data sheet.
 
  return temperature;
}
 
void onRequestHandler()
{
  Wire.write(txBuffer, sizeof(txBuffer));
}
 
void setup()
{
  // TODO: ADC reference to INTERNAL
  // TODO: pin modes
   // TODO: join I2C as a slave using I2C_ADDR
  // TODO: tell I2C to run onRequestHandler when the master asks for data
  // TODO: start Serial at 9600 so we can see stuff on the computer
 
  analogReference(INTERNAL);
 
  analogRead(SIG_PIN); // flush out any stray singnals.
 
  delay(10);
 
  digitalWrite(FAULT_LED, LOW)
 
  memset(txBuffer, sizeof(txBuffer));
 
  Wire.begin(I2C_ADDR);
  Wire.onRequest(onRequestHandler);
}
 
void loop()
{
  // TODO: read every sensor, set fault if over FAULT_C
  // TODO: pack temps into a temp buffer (tenths of a degree, 2 bytes each)
  // TODO: set LED, then copy buffer into txBuf with interrupts off
 
  uint8_t tempBuffer[sizeof(txBuffer)];
 
  bool fault = false;
 
  for(uint8_t channel = 0; channel < NUM_CHANNELS; channel++)
  {
      float temperature = readTemp(channel);
 
      if(temperature > FAULT_C)
      {
          fault = true;
      }
 
      long tenths = lround(temperature * 10.0);
      tenths = constrain(tenths, -32768L, 32767L);
      uint16_t raw = (uint16_t)(int16_t)tenths;
     
      tempBuffer[1 + 2 * channel] = (uint8_t)(raw >> 8); // extract and pack high byte first.
      tempBuffer[2 + 2 * channel] = (uint8_t)(raw & 0xFF); // extract and pack low byte next.
  }
 
  digitalWrite(FAULT_LED, fault ? HIGH : LOW)
 
  memcpy(txBuffer, tempBuffer, sizeof(txBuffer));
 
  delay(250);

}
