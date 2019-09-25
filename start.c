/** @file
 *
 * I2C Application
 *
 * This application tests I2C capable board features.
 *
 *
 */

#include "start.h"

/******************************************************
 *                      Macros
 ******************************************************/


/******************************************************
 *                    Constants
 ******************************************************/
#define MAX_LINE_LENGTH  (128)
#define MAX_HISTORY_LENGTH (20)
#define MAX_NUM_COMMAND_TABLE  (8)

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/
static char line_buffer[MAX_LINE_LENGTH];
static char history_buffer_storage[MAX_LINE_LENGTH * MAX_HISTORY_LENGTH];


#define DIAGNOSTICS_COMMANDS \
{ "hts221",  hts221_get,  0, NULL, NULL,"",  "get HTS221 temperature" }, \
{ "lis2dh12",  lis2dh12_get,  0, NULL, NULL,"",  "get LIS2DH12 accelerometer" }, \
{ "kx122",  kx122_get,  0, NULL, NULL,"",  "get KX122 accelerometer" }, \
{ "icm20602",  icm20602_get,  0, NULL, NULL,"",  "get ICM20602 accelerometer" }, \


/******************************************************
 *               Function Definitions
 ******************************************************/
static const command_t init_commands[] = {
        DIAGNOSTICS_COMMANDS
        CMD_TABLE_END
};

/******************************Application start/main function.**********************************/
void application_start( void )
{
    //wiced_result_t result;

    wiced_init( );

    /* Initialize the device */
    wiced_core_init( );

    /* probe for temperature device */
    hts221_init();

    /* probe for accelerometer device */
    lis2dh12_init();
    kx122_init();
    icm20602_init();

    /* enable command line interface */
    WPRINT_APP_INFO( ( "\r\nType help to know more about commands ...\r\n" ) );
    command_console_init( STDIO_UART, MAX_LINE_LENGTH, line_buffer, MAX_HISTORY_LENGTH, history_buffer_storage, " " );
    console_add_cmd_table( init_commands );


}
