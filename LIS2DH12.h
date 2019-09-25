
#include "start.h"

#define LIS2DH12_SLAVE_ADDR      (0x19)
#define LIS2DH12_WOAMI_REG       (0x0F | 0x80)
#define LIS2DH12_CTRL_REG0       (0x1E | 0x80)
#define LIS2DH12_CTRL_REG1       (0x20 | 0x80)
#define LIS2DH12_CTRL_REG2       (0x21 | 0x80)
#define LIS2DH12_CTRL_REG3       (0x22 | 0x80)
#define LIS2DH12_CTRL_REG4       (0x23 | 0x80)
#define LIS2DH12_CTRL_REG5       (0x24 | 0x80)
#define LIS2DH12_CTRL_REG6       (0x25 | 0x80)
#define LIS2DH12_STATUS_REG      (0x27 | 0x80)
#define LIS2DH12_OUT_X_L         (0x28 | 0x80)
#define LIS2DH12_OUT_X_H         (0x29 | 0x80)
#define LIS2DH12_OUT_Y_L         (0x2A | 0x80)
#define LIS2DH12_OUT_Y_H         (0x2B | 0x80)
#define LIS2DH12_OUT_Z_L         (0x2C | 0x80)
#define LIS2DH12_OUT_Z_H         (0x2D | 0x80)

#define LIS2DH12_CTRL1_ODR_400    (0x07<<4) // 400 Hz
#define LIS2DH12_CTRL1_ODR_25     (0x03<<4) // 25 Hz
#define LIS2DH12_CTRL1_ODR_10     (0x02<<4) // 10 Hz
#define LIS2DH12_CTRL1_ODR_1      (0x01<<4) // 1 Hz
#define LIS2DH12_CTRL1_ZEN        (0x01<<2)
#define LIS2DH12_CTRL1_YEN        (0x01<<1)
#define LIS2DH12_CTRL1_XEN        (0x01)

#define LIS2DH12_CTRL4_BDU        (0x80)

#define LIS2DH12_STAT_ZYXDA       (0x01<<3)

wiced_result_t lis2dh12_init( void );
int lis2dh12_get(int argc, char *argv[]);
void convert_accel_data(int16_t* x, int16_t* y, int16_t* z);

