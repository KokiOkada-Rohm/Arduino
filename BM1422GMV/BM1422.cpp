/*****************************************************************************
  BM1422.cpp

 Copyright (c) 2018 ROHM Co.,Ltd.

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
//#include <avr/pgmspace.h>
#include <Wire.h>
#include "arduino.h"
#include "BM1422.h"

BM1422::BM1422(int slave_address)
{
  _device_address = slave_address ;
}

byte BM1422::init(void)
{
  byte rc;
  unsigned char reg;
  unsigned char buf[2];

  rc = read(BM1422_WIA, &reg, sizeof(reg));
  if (rc != 0) {
    Serial.println("Can't access BM1422");
    return (rc);
  }
  Serial.print("BM1422_WHO_AMI Register Value = 0x");
  Serial.println(reg, HEX);

  if (reg != BM1422_WIA_VAL) {
    Serial.println("Can't find BM1422");
    return (rc);
  }

  // Step1
  reg = BM1422_CNTL1_VAL;
  rc = write(BM1422_CNTL1, &reg, sizeof(reg));
  if (rc != 0) {
    Serial.println("Can't write BM1422_CNTL1 Register");
    return (rc);
  }

  // Check 12bit or 14bit
  buf[0] = (BM1422_CNTL1_VAL & BM1422_CNTL1_OUT_BIT);
  if (buf[0] == BM1422_CNTL1_OUT_BIT) {
    _sens = BM1422_14BIT_SENS;
  } else {
    _sens = BM1422_12BIT_SENS;
  }

  buf[0] = (BM1422_CNTL4_VAL >> 8) & 0xFF;
  buf[1] = (BM1422_CNTL4_VAL & 0xFF);
  rc = write(BM1422_CNTL4, buf, sizeof(buf));
  if (rc != 0) {
    Serial.println("Can't write BM1422_CNTL4 Register");
    return (rc);
  }

  // Step2
  reg = BM1422_CNTL2_VAL;
  rc = write(BM1422_CNTL2, &reg, sizeof(reg));
  if (rc != 0) {
    Serial.println("Can't write BM1422_CNTL2 Register");
    return (rc);
  }

  // Step3

  return (rc);
}

byte BM1422::get_rawval(unsigned char *data)
{
  byte rc;
  unsigned char reg;

  // Step4
  reg = BM1422_CNTL3_VAL;
  rc = write(BM1422_CNTL3, &reg, sizeof(reg));
  if (rc != 0) {
    Serial.println("Can't write BM1422_CNTL3 Register");
    return (rc);
  }

  do {
    rc = read(BM1422_STA1, &reg, 1);
    if (rc != 0) {
      Serial.println("Can't read BM1422_STA1 Register");
      return (rc);
    }
  } while((reg & BM1422_STA1_RD_DRDY) != BM1422_STA1_RD_DRDY);

  rc = read(BM1422_DATAX, data, 6);
  if (rc != 0) {
    Serial.println("Can't get BM1422 magnet values");
  }

  return (rc);
}

byte BM1422::get_val(float *data)
{
  byte rc;
  unsigned char val[6];
  signed short mag[3];

  rc = get_rawval(val);
  if (rc != 0) {
    return (rc);
  }

  mag[0] = ((signed short)val[1] << 8) | (val[0]);
  mag[1] = ((signed short)val[3] << 8) | (val[2]);
  mag[2] = ((signed short)val[5] << 8) | (val[4]);

  convert_uT(mag, data);

  return (rc);  
}

void BM1422::convert_uT(signed short *rawdata, float *data)
{
  // LSB to uT
  data[0] = (float)rawdata[0] / _sens;
  data[1] = (float)rawdata[1] / _sens;
  data[2] = (float)rawdata[2] / _sens;
}

byte BM1422::write(unsigned char memory_address, unsigned char *data, unsigned char size)
{
  byte rc;
  unsigned int cnt;

  Wire.beginTransmission(_device_address);
  Wire.write(memory_address);
  Wire.write(data, size);
  rc = Wire.endTransmission();
  return (rc);
}

byte BM1422::read(unsigned char memory_address, unsigned char *data, int size)
{
  byte rc;
  unsigned char cnt;

  Wire.beginTransmission(_device_address);
  Wire.write(memory_address);
  rc = Wire.endTransmission(false);
  if (rc != 0) {
    return (rc);
  }

  Wire.requestFrom(_device_address, size, true);
  cnt = 0;
  while(Wire.available()) {
    data[cnt] = Wire.read();
    cnt++;
  }

  return (rc);
}
