/*****************************************************************************
  BM1383GLV.h

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
#ifndef _BM1383GLV_H_
#define _BM1383GLV_H_

#define BM1383GLV_DEVICE_ADDRESS           (0x5D)    // 7bit Addrss
#define BM1383GLV_ID_VAL                   (0x31)

#define BM1383GLV_ID                       (0x10)
#define BM1383GLV_POWER_DOWN               (0x12)
#define BM1383GLV_RESET                    (0x13)
#define BM1383GLV_MODE_CONTROL             (0x14)
#define BM1383GLV_PRESSURE_MSB             (0x1C)

#define BM1383GLV_POWER_DOWN_PWR_DOWN      (1 << 0)
#define BM1383GLV_RESET_RSTB               (1 << 0)
#define BM1383GLV_MODE_CONTROL_AVE_NUM64   (6 << 5)
#define BM1383GLV_MODE_CONTROL_T_AVE       (1 << 3)
#define BM1383GLV_MODE_CONTORL_MODE_200MS  (4 << 0)


#define BM1383GLV_POWER_DOWN_VAL      (BM1383GLV_POWER_DOWN_PWR_DOWN)
#define BM1383GLV_RESET_VAL           (BM1383GLV_RESET_RSTB)
#define BM1383GLV_MODE_CONTROL_VAL    (BM1383GLV_MODE_CONTROL_AVE_NUM64 | BM1383GLV_MODE_CONTROL_T_AVE |BM1383GLV_MODE_CONTORL_MODE_200MS)

class BM1383GLV
{
  public:
      BM1383GLV(void);
    byte init(void) ;
    byte get_rawval(unsigned char *data);
    byte get_val(float *press);
    byte write(unsigned char memory_address, unsigned char *data, unsigned char size);
    byte read(unsigned char memory_address, unsigned char *data, int size);
};

#endif // _BM1383GLV_H_
