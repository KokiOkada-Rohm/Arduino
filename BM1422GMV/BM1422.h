/*****************************************************************************
  BM1422.h

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
#ifndef _BM1422_H_
#define _BM1422_H_

#define BM1422_DEVICE_ADDRESS_0E   (0x0E)    // 7bit Addrss
#define BM1422_DEVICE_ADDRESS_0F   (0x0F)    // 7bit Address
#define BM1422_WIA_VAL             (0x41)

#define BM1422_WIA                 (0x0F)
#define BM1422_DATAX               (0x10)
#define BM1422_STA1                (0x18)
#define BM1422_CNTL1               (0x1B)
#define BM1422_CNTL2               (0x1C)
#define BM1422_CNTL3               (0x1D)
#define BM1422_AVE_A               (0x40)
#define BM1422_CNTL4               (0x5C)

#define BM1422_STA1_RD_DRDY        (1 << 6)

#define BM1422_CNTL1_FS1           (1 << 1)
#define BM1422_CNTL1_ODR_10Hz      (0 << 3)
#define BM1422_CNTL1_RST_LV        (1 << 5)
#define BM1422_CNTL1_OUT_BIT       (1 << 6)
#define BM1422_CNTL1_PC1           (1 << 7)

#define BM1422_CNTL2_DRP           (1 << 2)
#define BM1422_CNTL2_DREN          (1 << 3)

#define BM1422_CNTL3_FORCE         (1 << 6)

#define BM1422_CNTL1_VAL           (BM1422_CNTL1_FS1 | BM1422_CNTL1_OUT_BIT | BM1422_CNTL1_PC1)
#define BM1422_CNTL2_VAL           (BM1422_CNTL2_DREN)
#define BM1422_CNTL3_VAL           (BM1422_CNTL3_FORCE)
#define BM1422_CNTL4_VAL           (0x0000)

#define BM1422_14BIT_SENS          (24)
#define BM1422_12BIT_SENS          (6)

class BM1422
{
  public:
      BM1422(int slave_address);
    byte init(void) ;
    byte get_rawval(unsigned char *data);
    byte get_val(float *data);
    void convert_uT(signed short *rawdata, float *data);
    byte write(unsigned char memory_address, unsigned char *data, unsigned char size);
    byte read(unsigned char memory_address, unsigned char *data, int size);
  private:
    int _device_address;
    unsigned char _sens;
};

#endif // _BM1422_H_
