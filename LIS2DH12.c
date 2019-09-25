
#include "LIS2DH12.h"

static wiced_i2c_device_t i2c_device_lis2dh12 =
{
        .port = WICED_I2C_2,  //I2C_1
        .address = LIS2DH12_SLAVE_ADDR,
        .address_width = I2C_ADDRESS_WIDTH_7BIT,
        .speed_mode = I2C_STANDARD_SPEED_MODE,
};


/********************************Initializes I2C, probes for accelerometer device***************************************/
wiced_result_t lis2dh12_init( void )
{

    uint8_t wbuf[8];
    uint8_t rbuf[8];

    /* Initialize I2C */
    if ( wiced_i2c_init( &i2c_device_lis2dh12 ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ( "I2C Initialization Failed\n" ) );
        return WICED_ERROR;
    }

    /* Probe I2C bus for accelerometer */
    if(  wiced_i2c_probe_device( &i2c_device_lis2dh12, NUM_I2C_MESSAGE_RETRIES )  != WICED_TRUE )
    {
        WPRINT_APP_INFO( ( "Failed to connect to LIS2DH12 device; addr 0x%x\n", i2c_device_lis2dh12.address ) );
        return WICED_ERROR;
    }

    wbuf[0] = LIS2DH12_WOAMI_REG;
    wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );

    if( rbuf[0] != 0x33 )
    {
        WPRINT_APP_INFO( ( "Failed to read WHOAMI from LIS2DH12 device; addr 0x%x\n", i2c_device_lis2dh12.address ) );
        return WICED_ERROR;
    }
    WPRINT_APP_INFO( ( "LIS2DH12 device (0x%x) at address 0x%x\n", rbuf[0], i2c_device_lis2dh12.address ) );

    /* Power-up the device */
    wbuf[0] = LIS2DH12_CTRL_REG1;
    wbuf[1] = 0;
    wbuf[1] = (LIS2DH12_CTRL1_ODR_400 | LIS2DH12_CTRL1_ZEN | LIS2DH12_CTRL1_YEN | LIS2DH12_CTRL1_XEN);
    wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    /* Set normal mode */
    wbuf[0] = LIS2DH12_CTRL_REG4;
    wbuf[1] = LIS2DH12_CTRL4_BDU;
    wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2 );

    return WICED_SUCCESS;

}

/************************Holder function to get LIS2DH12 accelerometer*****************************/
int lis2dh12_get(int argc, char *argv[])
{

    uint8_t wbuf[8];
    uint8_t rbuf[8];
    uint8_t val[8];
    int16_t xdata, ydata, zdata;
    float xdataf, ydataf, zdataf;
    // Read status register from address 0x27
    wbuf[0] = LIS2DH12_STATUS_REG;
    wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    val[0] = rbuf[0];

    if( val[0] & LIS2DH12_STAT_ZYXDA ) {

        // Read 1 byte of data from address 0x29
        wbuf[0] = LIS2DH12_OUT_X_H;
        wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
        wiced_i2c_read( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
        val[1] = rbuf[0];

        // Read 1 byte of data from address 0x28
        wbuf[0] = LIS2DH12_OUT_X_L;
        wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
        wiced_i2c_read( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
        val[0] = rbuf[0];

        // Read 1 byte of data from address 0x2B
        wbuf[0] = LIS2DH12_OUT_Y_H;
        wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
        wiced_i2c_read( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
        val[3] = rbuf[0];

        // Read 1 byte of data from address 0x2A
        wbuf[0] = LIS2DH12_OUT_Y_L;
        wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
        wiced_i2c_read( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
        val[2] = rbuf[0];

        // Read 1 byte of data from address 0x2D
        wbuf[0] = LIS2DH12_OUT_Z_H;
        wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
        wiced_i2c_read( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
        val[5] = rbuf[0];

        // Read 1 byte of data from address 0x2C
        wbuf[0] = LIS2DH12_OUT_Z_L;
        wiced_i2c_write( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
        wiced_i2c_read( &i2c_device_lis2dh12, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
        val[4] = rbuf[0];

        xdata =(val[1]<<8 | val[0]);
        ydata =(val[3]<<8 | val[2]);
        zdata =(val[5]<<8 | val[4]);

        // Convert to mg
        convert_accel_data(&xdata, &ydata, &zdata);

        // Check for negative
        if(xdata&0x800) {
            xdata = (((~xdata&0x7ff)+1) * -1);
        }
        if(ydata&0x800) {
            ydata = (((~ydata&0x7ff)+1) * -1);
        }
        if(zdata&0x800) {
            zdata = (((~zdata&0x7ff)+1) * -1);
        }

        // Convert to g and round
        xdataf = roundf(xdata * 0.001);
        ydataf = roundf(ydata * 0.001);
        zdataf = roundf(zdata * 0.001);

        WPRINT_APP_INFO(("x: %.fg\t", xdataf));
        WPRINT_APP_INFO(("y: %.fg\t", ydataf));
        WPRINT_APP_INFO(("z: %.fg\n", zdataf));

    } else {
        WPRINT_APP_INFO(("No new XYZ data\n"));
        return 0;
    }

    return 0;
}

/********************Converts raw accelerometer data to mg.***************************/
void convert_accel_data(int16_t* x, int16_t* y, int16_t* z)
{
    uint16_t lx, ly, lz;

    lx = *x;
    ly= *y;
    lz= *z;

    *x = (int32_t)lx*1000/(1024*16); // transform data to millig, for 2g scale axis*1000/(1024*16),
    *y = (int32_t)ly*1000/(1024*16); // for 4g scale axis*1000/(1024*8),
    *z = (int32_t)lz*1000/(1024*16); // for 8g scale axis*1000/(1024*4)

    return;
}
