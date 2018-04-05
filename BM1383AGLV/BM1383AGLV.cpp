/*****************************************************************************
  BM1383GLV.cpp

 Copyright (c) 2016 ROHM Co.,Ltd.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
******************************************************************************/
#include <avr/pgmspace.h>
#include <Wire.h>
#include "arduino.h"
#include "BM1383GLV.h"

BM1383GLV::BM1383GLV(void)
{

}

byte BM1383GLV::init(void)
{
  byte rc;
  unsigned char reg;

  rc = read(BM1383GLV_ID, &reg, sizeof(reg));
  if (rc != 0) {
    Serial.println(F("Can't access BM1383GLV"));
    return (rc);
  }
  Serial.print(F("BM1383GL ID Register Value = 0x"));
  Serial.println(reg, HEX);

  if (reg != BM1383GLV_ID_VAL) {
    Serial.println(F("Can't find BM1383GLV"));
    return (rc);
  }

  reg = BM1383GLV_POWER_DOWN_VAL;
  rc = write(BM1383GLV_POWER_DOWN, &reg, sizeof(reg));
  if (rc != 0) {
    Serial.println(F("Can't write BM1383GLV POWER_DOWN register"));
    return (rc);
  }

  delay(1);

  reg = BM1383GLV_RESET_VAL;
  rc = write(BM1383GLV_RESET, &reg, sizeof(reg));
  if (rc != 0) {
    Serial.println(F("Can't write BM1383GLV SLEEP register"));
    return (rc);
  }

  reg = BM1383GLV_MODE_CONTROL_VAL;
  rc = write(BM1383GLV_MODE_CONTROL, &reg, sizeof(reg));
  if (rc != 0) {
    Serial.println(F("Can't write BM1383GLV MODE_CONTROL register"));
    return (rc);
  }
  
}

byte BM1383GLV::get_rawval(unsigned char *data)
{
  byte rc;

  rc = read(BM1383GLV_PRESSURE_MSB, data, 3);
  if (rc != 0) {
    Serial.println(F("Can't get BM1383GLV PRESS value"));
  }

  return (rc);
}

byte BM1383GLV::get_val( float *press)
{
  byte rc;
  unsigned char val[3];
  unsigned long rawpress;

  rc = get_rawval(val);
  if (rc != 0) {
    return (rc);
  }

  rawpress = (((unsigned long)val[0] << 16) | ((unsigned long)val[1] << 8) | val[2]&0xFC) >> 2;

  if (rawpress == 0) {
    return (-1);
  }

  *press = (float)rawpress / 2048;

  return (rc);
}

byte BM1383GLV::write(unsigned char memory_address, unsigned char *data, unsigned char size)
{
  byte rc;
  unsigned int cnt;

  Wire.beginTransmission(BM1383GLV_DEVICE_ADDRESS);
  Wire.write(memory_address);
  Wire.write(data, size);
  rc = Wire.endTransmission();
  return (rc);
}

byte BM1383GLV::read(unsigned char memory_address, unsigned char *data, int size)
{
  byte rc;
  unsigned char cnt;

  Wire.beginTransmission(BM1383GLV_DEVICE_ADDRESS);
  Wire.write(memory_address);
  rc = Wire.endTransmission(false);
  if (rc != 0) {
    return (rc);
  }

  Wire.requestFrom(BM1383GLV_DEVICE_ADDRESS, size, true);
  cnt = 0;
  while(Wire.available()) {
    data[cnt] = Wire.read();
    cnt++;
  }

  return (rc);
}
