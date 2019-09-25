
#include "ICM20602.h"

enum
{
    SMPLRT_DIV = 0x19,
    CONFIG = 0x1A,
    GYRO_CONFIG = 0x1B,
    ACCEL_CONFIG = 0x1C,
    ACCEL_CONFIG2 = 0x1D,
    INT_PIN_CFG = 0x37,
    INT_ENABLE = 0x38,

    INT_STATUS = 0x3A,

    ACCEL_XOUT_H = 0x3B,
    ACCEL_XOUT_L = 0x3C,
    ACCEL_YOUT_H = 0x3D,
    ACCEL_YOUT_L = 0x3E,
    ACCEL_ZOUT_H = 0x3F,
    ACCEL_ZOUT_L = 0x40,

    TEMP_OUT_H = 0x41,
    TEMP_OUT_L = 0x42,

    GYRO_XOUT_H = 0x43,
    GYRO_XOUT_L = 0x44,
    GYRO_YOUT_H = 0x45,
    GYRO_YOUT_L = 0x46,
    GYRO_ZOUT_H = 0x47,
    GYRO_ZOUT_L = 0x48,

    USER_CTRL = 0x6A,
    PWR_MGMT_1 = 0x6B,
    PWR_MGMT_2 = 0x6C,
    WHO_AM_I = 0x75,
}reg_icm20602;

enum
{
    WHO_AM_I_WIA_ID = 0xAD,

    GYRO_CONFIG_250DPS = 0x0<<3,    // gyro +-250dps, 16bit
    GYRO_CONFIG_500DPS = 0x1<<3,    // gyro +-500dps, 16bit
    GYRO_CONFIG_1000DPS = 0x2<<3,   // gyro +-1000dps, 16bit
    GYRO_CONFIG_2000DPS = 0x3<<3,   // gyro +-2000dps, 16bit
    GYRO_CONFIG_DPS_MASK = 0x3<<3,  // mask

    ACCEL_CONFIG_2G = 0x0<<3,   // accel +-2G, 16bit
    ACCEL_CONFIG_4G = 0x1<<3,   // accel +-4G, 16bit
    ACCEL_CONFIG_8G = 0x2<<3,   // accel +-8G, 16bit
    ACCEL_CONFIG_16G = 0x3<<3,  // accel +-16G, 16bit
    ACCEL_CONFIG_G_MASK = 0x3<<3,  // mask

    INT_ENABLE_DI = 0x00<<0,    // interrupt disable
    INT_ENABLE_EN = 0x01<<0,    // interrupt enable
    INT_ENABLE_MASK = 0x01<<0,  // interrupt mask

    PWR_MGMT_1_DEVICE_RESET = 0x01<<7,  // Reset the internal registers and restores the default settings.
}set_icm20602;

enum
{
    DEVICE_ADDRESS_AD0L = 0x68,     // AD0 is logic low
    DEVICE_ADDRESS_AD0H = 0x69      // AD0 is logic high
}adr_icm20602;

static wiced_i2c_device_t i2c_device =
{
    .port = WICED_I2C_2,  //I2C_1
    .address = DEVICE_ADDRESS_AD0L,
    .address_width = I2C_ADDRESS_WIDTH_7BIT,
    .speed_mode = I2C_STANDARD_SPEED_MODE,
//    .speed_mode = I2C_HIGH_SPEED_MODE,
};

wiced_result_t icm20602_init(void)
{

    uint8_t wbuf[8];
//    uint8_t rbuf[8];
    uint8_t rbuf[14];



    wbuf[0] = PWR_MGMT_1;
    wbuf[1] = PWR_MGMT_1_DEVICE_RESET;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

//    wbuf[0] = PWR_MGMT_1;
//    while(1){
//        wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
//        wiced_i2c_read( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
//        if( rbuf[0] == 0x0 ){
//            break;
//        }
//    }

    // delay 100ms
    wiced_rtos_delay_milliseconds(1000);






    /* Initialize I2C */
    if ( wiced_i2c_init( &i2c_device ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ( "I2C Initialization Failed\n" ) );
        return WICED_ERROR;
    }

    /* Probe I2C bus for accelerometer */
    if(  wiced_i2c_probe_device( &i2c_device, NUM_I2C_MESSAGE_RETRIES )  != WICED_TRUE )
    {
        WPRINT_APP_INFO( ( "Failed to connect to ICM20602 device; addr 0x%x\n", i2c_device.address ) );
        return WICED_ERROR;
    }





    wbuf[0] = WHO_AM_I;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    if( rbuf[0] != WHO_AM_I_WIA_ID )
    {
        WPRINT_APP_INFO( ( "Failed to read WHOAMI from ICM20602 device; addr 0x%x\n", i2c_device.address ) );
        return WICED_ERROR;
    }
    WPRINT_APP_INFO( ( "ICM20602 device (0x%x) at address 0x%x\n", rbuf[0], i2c_device.address ) );






    wbuf[0] = PWR_MGMT_1;
    wbuf[1] = 0x01;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = PWR_MGMT_2;
    wbuf[1] = 0x00;         // gyro on, accel on
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = USER_CTRL;
    wbuf[1] = 0x00;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = GYRO_CONFIG;
    wbuf[1] = GYRO_CONFIG_2000DPS;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = ACCEL_CONFIG;
    wbuf[1] = ACCEL_CONFIG_2G;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    switch( wbuf[1] & ACCEL_CONFIG_G_MASK ) {
        case ACCEL_CONFIG_2G:
//            _g_sens = 16384;
            WPRINT_APP_INFO(("Acc max: 2G\r\n"));
            break;
        case ACCEL_CONFIG_4G:
//            g_sens = 8192;
            WPRINT_APP_INFO(("Acc max: 4G\r\n"));
            break;
        case ACCEL_CONFIG_8G:
//            _g_sens = 4096;
            WPRINT_APP_INFO(("Acc max: 8G\r\n"));
            break;
        case ACCEL_CONFIG_16G:
//            _g_sens = 4096;
            WPRINT_APP_INFO(("Acc max: 16G\r\n"));
            break;
        default:
            break;
    }

    wbuf[0] = CONFIG;
    wbuf[1] = 0x01;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = ACCEL_CONFIG2;
    wbuf[1] = 0x01;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = SMPLRT_DIV;
    wbuf[1] = 0x04;     // 200Hz sample rate
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = INT_PIN_CFG;
    wbuf[1] = 0x00;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = INT_ENABLE;
    wbuf[1] = INT_ENABLE_EN;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    // delay 50ms
    wiced_rtos_delay_milliseconds(50);

//    wbuf[0] = PWR_MGMT_2;
//    wbuf[1] = 0x00;         // gyro on, accel on
//    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );







uint8_t r[20];

//    wbuf[0] = 0x00;
//    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
//    wiced_i2c_read( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, r, 20 );
//
//    uint32_t i;
//    for(i=0;i<20;i++){
//        WPRINT_APP_INFO(("%X\r\n", r[i]));
//    }

    wbuf[0] = ACCEL_XOUT_H;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );

    WPRINT_APP_INFO(("x: %Xg\t", rbuf[0]));

    wbuf[0] = ACCEL_YOUT_H;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    WPRINT_APP_INFO(("y: %Xg\t", rbuf[0]));

    wbuf[0] = ACCEL_ZOUT_H;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    WPRINT_APP_INFO(("z: %Xg\n", rbuf[0]));






//    WPRINT_APP_INFO(("x: %fg\t", rbuf[0]));
//    WPRINT_APP_INFO(("y: %fg\t", rbuf[1]));
//    WPRINT_APP_INFO(("z: %fg\n", rbuf[2]));
//    WPRINT_APP_INFO(("x: %fg\t", rbuf[3]));
//    WPRINT_APP_INFO(("y: %fg\t", rbuf[4]));
//    WPRINT_APP_INFO(("z: %fg\n", rbuf[5]));
//
//    WPRINT_APP_INFO(("y: %fg\t", rbuf[6]));
//    WPRINT_APP_INFO(("z: %fg\n", rbuf[7]));
//
//    WPRINT_APP_INFO(("x: %fg\t", rbuf[8]));
//    WPRINT_APP_INFO(("y: %fg\t", rbuf[9]));
//    WPRINT_APP_INFO(("z: %fg\n", rbuf[10]));
//    WPRINT_APP_INFO(("x: %fg\t", rbuf[11]));
//    WPRINT_APP_INFO(("y: %fg\t", rbuf[12]));
//    WPRINT_APP_INFO(("z: %fg\n", rbuf[13]));
}

int icm20602_get(int argc, char *argv[])
{
    uint8_t wbuf[1];
    uint8_t rbuf[6];
    int16_t acc[3];
    uint16_t _g_sens = 0;
    float data[3];

#if 0
    G = A_FSR/32768
    m/s^2 = A_FSR/32768*9.8
    degree/s = G_FSR/32768
    rad/s = G_FSR/32768*/57.3
#endif



//    wbuf[0] = KX122_CNTL1;
//    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
//    wiced_i2c_read( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
//    switch( rbuf[0] & KX122_CNTL1_GSEL_MASK ) {
//      case KX122_CNTL1_GSEL_2G:
//          _g_sens = 16384;
////          WPRINT_APP_INFO(("GSEL: 2G\r\n"));
//          break;
//      case KX122_CNTL1_GSEL_4G:
//          _g_sens = 8192;
////          WPRINT_APP_INFO(("GSEL: 4G\r\n"));
//          break;
//      case KX122_CNTL1_GSEL_8G:
//          _g_sens = 4096;
////          WPRINT_APP_INFO(("GSEL: 8G\r\n"));
//          break;
//      default:
//          break;
//    }



    wbuf[0] = INT_STATUS;
    wbuf[1] = 0x00;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = ACCEL_XOUT_H;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 6 );

//    acc[0] = ((int16_t)rbuf[0] << 8) | (rbuf[1]);
//    acc[1] = ((int16_t)rbuf[2] << 8) | (rbuf[3]);
//    acc[2] = ((int16_t)rbuf[4] << 8) | (rbuf[5]);
//
//    // Convert LSB to g
//    data[0] = (float)acc[0] / 32768;
//    data[1] = (float)acc[1] / 32768;
//    data[2] = (float)acc[2] / 32768;

    WPRINT_APP_INFO(("x: %fg\t", rbuf[0]));
    WPRINT_APP_INFO(("y: %fg\t", rbuf[1]));
    WPRINT_APP_INFO(("z: %fg\n", rbuf[2]));

    wbuf[0] = INT_STATUS;
    wbuf[1] = 0x00;
    wiced_i2c_write( &i2c_device, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );
}
