
#include "start.h"

// WHO_AM_I
#define KX122_WHO_AM_I 0x0F
// WHO_AM_I -value for KX122
#define KX122_WHO_AM_I_WIA_ID (0x1B << 0)
// Read/write control register that controls the main feature set.
#define KX122_CNTL1 0x18
// determines the performance mode of the KX122. The noise varies with ODR, RES and different LP_CNTL settings possibly reducing the effective resolution.
#define KX122_CNTL1_RES (0x01 << 6)

// 2g range
#define KX122_CNTL1_GSEL_2G (0x00 << 3)
// 4g range
#define KX122_CNTL1_GSEL_4G (0x01 << 3)
// 8g range
#define KX122_CNTL1_GSEL_8G (0x02 << 3)

// selects the acceleration range of the accelerometer outputs
#define KX122_CNTL1_GSEL_MASK 0x18
// This register is responsible for configuring ODR (output data rate) and filter settings
#define KX122_ODCNTL 0x1B

// 12.5Hz
#define KX122_ODCNTL_OSA_12P5 (0x00 << 0)
// 25Hz
#define KX122_ODCNTL_OSA_25 (0x01 << 0)
// 50Hz
#define KX122_ODCNTL_OSA_50 (0x02 << 0)
// 100Hz
#define KX122_ODCNTL_OSA_100 (0x03 << 0)
// 200Hz
#define KX122_ODCNTL_OSA_200 (0x04 << 0)
// 400Hz
#define KX122_ODCNTL_OSA_400 (0x05 << 0)
// 800Hz
#define KX122_ODCNTL_OSA_800 (0x06 << 0)
// 1600Hz
#define KX122_ODCNTL_OSA_1600 (0x07 << 0)
// 0.78Hz
#define KX122_ODCNTL_OSA_0P781 (0x08 << 0)
// 1.563Hz
#define KX122_ODCNTL_OSA_1P563 (0x09 << 0)
// 3.125Hz
#define KX122_ODCNTL_OSA_3P125 (0x0A << 0)
// 6.25Hz
#define KX122_ODCNTL_OSA_6P25 (0x0B << 0)
// 3200Hz
#define KX122_ODCNTL_OSA_3200 (0x0C << 0)
// 6400Hz
#define KX122_ODCNTL_OSA_6400 (0x0D << 0)
// 12800Hz
#define KX122_ODCNTL_OSA_12800 (0x0E << 0)
// 25600Hz
#define KX122_ODCNTL_OSA_25600 (0x0F << 0)

// controls the operating mode of the KX122.
#define KX122_CNTL1_PC1 (0x01 << 7)

// output register x
#define KX122_XOUT_L 0x06
#define KX122_XOUT_H 0x07
// output register y
#define KX122_YOUT_L 0x08
#define KX122_YOUT_H 0x09
// output register z
#define KX122_ZOUT_L 0x0A
#define KX122_ZOUT_H 0x0B







#define KX122_DEVICE_ADDRESS_1E         (0x1E)    // 7bit Address
#define BH1749NUC_DEVICE_ADDRESS_38     (0x38)    // 7bit Addrss
#define BM1383AGLV_DEVICE_ADDRESS       (0x5D)    // 7bit Addrss

wiced_result_t kx122_init(void);
int kx122_get(int argc, char *argv[]);

extern char kx122_data[50];
