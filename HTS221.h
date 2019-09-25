
#include "start.h"

#define HTS221_SLAVE_ADDR        (0x5F)
#define HTS221_WOAMI_REG         (0x0F | 0x80)
#define HTS221_CTRL_REG1         (0x20 | 0x80)
#define HTS221_TEMP_OUT_L        (0x2A | 0x80)
#define HTS221_TEMP_OUT_H        (0x2B | 0x80)
#define HTS221_T0_DEGC_X8        (0x32 | 0x80)
#define HTS221_T1_DEGC_X8        (0x33 | 0x80)
#define HTS221_T1_T0_MSB         (0x35 | 0x80)
#define HTS221_T0_OUT_L          (0x3C | 0x80)
#define HTS221_T0_OUT_H          (0x3D | 0x80)
#define HTS221_T1_OUT_L          (0x3E | 0x80)
#define HTS221_T1_OUT_H          (0x3F | 0x80)

#define HTS221_CTRL1_PD          (0x80)
#define HTS221_CTRL1_BDU         (0x02)

wiced_result_t hts221_init( void );
int hts221_get(int argc, char *argv[]);
