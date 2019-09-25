#include "KX122.h"

char kx122_data[50];

static wiced_i2c_device_t i2c_device_kx122 =
{
    .port = WICED_I2C_2,  //I2C_1
    .address = KX122_DEVICE_ADDRESS_1E,
    .address_width = I2C_ADDRESS_WIDTH_7BIT,
    .speed_mode = I2C_STANDARD_SPEED_MODE,
};

wiced_result_t kx122_init(void)
{

    uint8_t wbuf[8];
    uint8_t rbuf[8];

    /* Initialize I2C */
    if ( wiced_i2c_init( &i2c_device_kx122 ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ( "I2C Initialization Failed\n" ) );
        return WICED_ERROR;
    }

    /* Probe I2C bus for accelerometer */
    if(  wiced_i2c_probe_device( &i2c_device_kx122, NUM_I2C_MESSAGE_RETRIES )  != WICED_TRUE )
    {
        WPRINT_APP_INFO( ( "Failed to connect to KX122 device; addr 0x%x\n", i2c_device_kx122.address ) );
        return WICED_ERROR;
    }



    wbuf[0] = KX122_WHO_AM_I;
    wiced_i2c_write( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    if( rbuf[0] != KX122_WHO_AM_I_WIA_ID )
    {
        WPRINT_APP_INFO( ( "Failed to read WHOAMI from KX122 device; addr 0x%x\n", i2c_device_kx122.address ) );
        return WICED_ERROR;
    }
    WPRINT_APP_INFO( ( "KX122 device (0x%x) at address 0x%x\n", rbuf[0], i2c_device_kx122.address ) );

    wbuf[0] = KX122_CNTL1;
    wbuf[1] = KX122_CNTL1_RES | KX122_CNTL1_GSEL_8G;
    wiced_i2c_write( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    switch( wbuf[1] & KX122_CNTL1_GSEL_MASK ) {
      case KX122_CNTL1_GSEL_2G:
//          _g_sens = 16384;
          WPRINT_APP_INFO(("GSEL: 2G\r\n"));
          break;
      case KX122_CNTL1_GSEL_4G:
//          _g_sens = 8192;
          WPRINT_APP_INFO(("GSEL: 4G\r\n"));
          break;
      case KX122_CNTL1_GSEL_8G:
//          _g_sens = 4096;
          WPRINT_APP_INFO(("GSEL: 8G\r\n"));
          break;
      default:
          break;
    }

    wbuf[0] = KX122_ODCNTL;
    wbuf[1] = KX122_ODCNTL_OSA_50;
    wiced_i2c_write( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    wbuf[0] = KX122_CNTL1;
    wiced_i2c_write( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );

    wbuf[0] = KX122_CNTL1;
    wbuf[1] = rbuf[0] | KX122_CNTL1_PC1;
    wiced_i2c_write( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

}


int kx122_get(int argc, char *argv[])
{
    uint8_t wbuf[1];
    uint8_t rbuf[6];
    int16_t acc[3];
    uint16_t _g_sens = 0;
    float data[3];

//while(1){
    wbuf[0] = KX122_CNTL1;
    wiced_i2c_write( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    switch( rbuf[0] & KX122_CNTL1_GSEL_MASK ) {
      case KX122_CNTL1_GSEL_2G:
          _g_sens = 16384;
//          WPRINT_APP_INFO(("GSEL: 2G\r\n"));
          break;
      case KX122_CNTL1_GSEL_4G:
          _g_sens = 8192;
//          WPRINT_APP_INFO(("GSEL: 4G\r\n"));
          break;
      case KX122_CNTL1_GSEL_8G:
          _g_sens = 4096;
//          WPRINT_APP_INFO(("GSEL: 8G\r\n"));
          break;
      default:
          break;
    }

    wbuf[0] = KX122_XOUT_L;
    wiced_i2c_write( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_kx122, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 6 );

    acc[0] = ((int16_t)rbuf[1] << 8) | (rbuf[0]);
    acc[1] = ((int16_t)rbuf[3] << 8) | (rbuf[2]);
    acc[2] = ((int16_t)rbuf[5] << 8) | (rbuf[4]);

    // Convert LSB to g
    data[0] = (float)acc[0] / _g_sens;
    data[1] = (float)acc[1] / _g_sens;
    data[2] = (float)acc[2] / _g_sens;

    sprintf(kx122_data, "x: %fg\ty: %fg\tz: %fg\r\n", data[0], data[1], data[2]);

//    WPRINT_APP_INFO(("x: %fg\t", data[0]));
//    WPRINT_APP_INFO(("y: %fg\t", data[1]));
//    WPRINT_APP_INFO(("z: %fg\n", data[2]));
    WPRINT_APP_INFO(("%s",kx122_data));

//}

}
