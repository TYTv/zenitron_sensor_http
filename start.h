#pragma once

#include "xively.h"
#include <math.h>

#include "wiced.h"
#include "resources.h"
#include "sntp.h"
#include "command_console.h"
#include "wiced_management.h"
#include "command_console_ping.h"

#include "HTS221.h"
#include "LIS2DH12.h"
#include "KX122.h"
#include "ICM20602.h"

#include "WEB.h"
#include "JSON_CMD.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @cond */
/******************************************************
 *                     Macros
 ******************************************************/
#define NUM_I2C_MESSAGE_RETRIES   (3)

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

/** @endcond */
#ifdef __cplusplus
} /*extern "C" */
#endif

