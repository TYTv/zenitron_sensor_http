
#include "HTS221.h"

static wiced_i2c_device_t i2c_device_hts221 =
{
        .port = WICED_I2C_2,  //I2C_1
        .address = HTS221_SLAVE_ADDR,
        .address_width = I2C_ADDRESS_WIDTH_7BIT,
        .speed_mode = I2C_STANDARD_SPEED_MODE,
};

/*********************************Initializes I2C, probes for temperature device******************************************/
wiced_result_t hts221_init( void )
{
    uint8_t wbuf[8];
    uint8_t rbuf[8];

    /* Initialize I2C for the temp sensor*/
    if ( wiced_i2c_init( &i2c_device_hts221 ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ( "I2C Initialization Failed\n" ) );
        return WICED_ERROR;
    }

    /* Probe I2C bus for temperature sensor */
    if( wiced_i2c_probe_device( &i2c_device_hts221, NUM_I2C_MESSAGE_RETRIES ) != WICED_TRUE )
    {
        WPRINT_APP_INFO( ( "Failed to connect to HTS221 device; addr 0x%x\n", i2c_device_hts221.address ) );
        return WICED_ERROR;
    }

    wbuf[0] = HTS221_WOAMI_REG;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );


    if( rbuf[0] != 0xbc )
    {
        WPRINT_APP_INFO( ( "Failed to read WHOAMI from HTS221 device; addr 0x%x\n", i2c_device_hts221.address ) );
        return WICED_ERROR;
    }
    WPRINT_APP_INFO( ( "HTS221 device (0x%x) at address 0x%x\n", rbuf[0], i2c_device_hts221.address ) );

    /* Power-up the device */
    wbuf[0] = HTS221_CTRL_REG1;
    wbuf[1] = (HTS221_CTRL1_PD | HTS221_CTRL1_BDU);
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    return WICED_SUCCESS;
}


/**************************Holder function to get HTS221 temperature**************************/
int hts221_get(int argc, char *argv[])
{
    uint8_t wbuf[8];
    uint8_t rbuf[8];
    int16_t T0, T1, T2, T3, raw;
    uint8_t val[4];
    int32_t temperature;
    float tempC, tempF;

    // Temperature Calibration values
    // Read 1 byte of data from address 0x32(50)
    wbuf[0] = HTS221_T0_DEGC_X8;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    T0 = rbuf[0];

    // Read 1 byte of data from address 0x33(51)
    wbuf[0] = HTS221_T1_DEGC_X8;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    T1 = rbuf[0];

    // Read 1 byte of data from address 0x35(53)
    wbuf[0] = HTS221_T1_T0_MSB;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    raw = rbuf[0];

    // Convert the temperature Calibration values to 10-bits
    T0 = ((raw & 0x03) * 256) + T0;
    T1 = ((raw & 0x0C) * 64) + T1;

    // Read 1 byte of data from address 0x3C(60)
    wbuf[0] = HTS221_T0_OUT_L;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    val[0] = rbuf[0];

    // Read 1 byte of data from address 0x3D(61)
    wbuf[0] = HTS221_T0_OUT_H;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    val[1] = rbuf[0];

    T2 = ((val[1] & 0xFF) * 256) + (val[0] & 0xFF);

    // Read 1 byte of data from address 0x3E(62)
    wbuf[0] = HTS221_T1_OUT_L;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    val[0] = rbuf[0];

    // Read 1 byte of data from address 0x3F(63)
    wbuf[0] = HTS221_T1_OUT_H;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    val[1] = rbuf[0];

    T3 = ((val[1] & 0xFF) * 256) + (val[0] & 0xFF);

    // Read 2 bytes of data; temperature msb and lsb
    wbuf[0] = HTS221_TEMP_OUT_L;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    val[0] = rbuf[0];

    wbuf[0] = HTS221_TEMP_OUT_H;
    wiced_i2c_write( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_hts221, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    val[1] = rbuf[0];

    temperature = ((val[1] & 0xFF) * 256) + (val[0] & 0xFF);
    if(temperature > 32767)
    {
        temperature -= 65536;
    }

    tempC = ((T1 - T0) / 8.0) * (temperature - T2) / (T3 - T2) + (T0 / 8.0);
    tempF = (tempC * 1.8 ) + 32;

    WPRINT_APP_INFO( ( "HTS221 temperature %.1f°C, %.1f°F\n", tempC, tempF ) );

    return 0;
}
