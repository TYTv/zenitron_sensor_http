/*
 * Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
 *
 * HTTP Server-Sent Event (SSE) Example Application
 *
 * This application snippet demonstrates how to send events from a HTTP server back to its clients.
 *
 * Features demonstrated
 *  - HTTP Server-Sent Event (SSE)
 *
 * To demonstrate the app, work through the following steps.
 *  1. Modify the CLIENT_AP_SSID/CLIENT_AP_PASSPHRASE Wi-Fi credentials
 *     in the wifi_config_dct.h header file to match your Wi-Fi access point
 *  3. Plug the WICED eval board into your computer
 *  4. Open a terminal application and connect to the WICED eval board
 *  5. Build and download the application (to the WICED board)
 *
 * After the download completes, the terminal displays WICED startup
 * information and then :
 *  - Joins a Wi-Fi network
 *  - Starts a webserver on the STA interface
 *
 * Application Operation
 * This section provides a description of the application flow and usage.
 * The app runs in a thread, the entry point is application_start()
 *
 *    Startup
 *      - Initialise the device
 *      - Start the network interface to connect the device to the network
 *      - Set the local time from a time server on the internet
 *      - Setup a timer to get current time
 *      - Start a webserver to display timestamps
 *
 *    Usage *
 *        The current time is published to a webpage on the client by a local webserver using server-sent events (SSE).
 *        The webpage uses an EventSource object to receive the SSE notifications.
 *
 */

#include "wiced.h"
#include "http_server.h"
#include "resources.h"
#include "sntp.h"

#include "WEB.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define SERVER_SENT_EVENT_INTERVAL_MS  ( 1 * SECONDS )
#define MAX_SOCKETS                    ( 5 )

#define STA_MODE        // sta mode
#define SOFT_AP_MODE    // soft ap mode

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
 *               Static Function Declarations
 ******************************************************/

static wiced_result_t send_event  ( void* arg );
static int32_t        process_page( const char* url_path, const char* url_parameters, wiced_http_response_stream_t* stream, void* arg, wiced_http_message_body_t* http_message_body );

/******************************************************
 *               Variable Definitions
 ******************************************************/

static wiced_http_server_t http_server;

START_OF_HTTP_PAGE_DATABASE(web_pages)
    ROOT_HTTP_PAGE_REDIRECT("/index.html"),
    { "/index.html",                    "text/html",                WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_index_html                      },
    { "/favicon.ico",                   "image/vnd.microsoft.icon", WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_favicon_ico,                    },
    { "/cypress.jpg",                   "image/jpg",                WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_cypress_jpg,                    },
    { "/line.png",                      "image/png",                WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_line_png,                       },
    { "/events",                        "text/event-stream",        WICED_RAW_DYNAMIC_URL_CONTENT, .url_content.dynamic_data  = { process_page, 0 },                                        },
    { "/jquery-1.8.3.min.js",           "application/javascript",   WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_jquery_1_8_3_min_js,            },
    { "/jquery.flot.min.js",            "application/javascript",   WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_jquery_flot_min_js,             },
    { "/zenitron.gif",                  "image/gif",                WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_zenitron_gif,                   },
    { "/rohm.jpg",                      "image/jpg",                WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_rohm_jpg,                       },
    { "/azurewave.png",                 "image/png",                WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_azurewave_png,                  },
    { "/microchip.png",                 "image/png",                WICED_RESOURCE_URL_CONTENT,    .url_content.resource_data = &resources_apps_DIR_res_DIR_microchip_png,                  },
END_OF_HTTP_PAGE_DATABASE();

static wiced_http_response_stream_t* http_event_stream = NULL;
static wiced_timed_event_t           timed_event;

/******************************************************
 *               Function Definitions
 ******************************************************/
#ifdef SOFT_AP_MODE
static const wiced_ip_setting_t device_init_ip_settings =
{
    INITIALISER_IPV4_ADDRESS( .ip_address, MAKE_IPV4_ADDRESS(192, 168, 0,  1) ),
    INITIALISER_IPV4_ADDRESS( .netmask,    MAKE_IPV4_ADDRESS(255, 255, 255, 0) ),
    INITIALISER_IPV4_ADDRESS( .gateway,    MAKE_IPV4_ADDRESS(192, 168, 0,  1) ),
};
#endif
void http_start( void )
{
    /* Initialise the device */
    wiced_init();

#ifdef SOFT_AP_MODE     // soft ap mode
    /* Bring up the network interface */
    wiced_network_up( WICED_AP_INTERFACE, WICED_USE_INTERNAL_DHCP_SERVER, &device_init_ip_settings );
#else                   // sta mode
    /* Bring up the STA (client) interface */
    wiced_network_up( WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL );
#endif
    /* Timestamp is needed for server-sent event (SSE) data.
     * Start automatic time synchronisation and synchronise once every day.
     */
    sntp_start_auto_time_sync( 1 * DAYS );

    wiced_rtos_register_timed_event( &timed_event, WICED_NETWORKING_WORKER_THREAD, send_event, SERVER_SENT_EVENT_INTERVAL_MS, NULL );

#ifdef SOFT_AP_MODE     // soft ap mode
    wiced_http_server_start( &http_server, 80, MAX_SOCKETS, web_pages, WICED_AP_INTERFACE, DEFAULT_URL_PROCESSOR_STACK_SIZE );
#else                   // sta mode
    /* Start a web server on the STA interface */
    wiced_http_server_start( &http_server, 80, MAX_SOCKETS, web_pages, WICED_STA_INTERFACE, DEFAULT_URL_PROCESSOR_STACK_SIZE );
#endif
}

static int32_t process_page( const char* url_path, const char* url_parameters, wiced_http_response_stream_t* stream, void* arg, wiced_http_message_body_t* http_message_body )
{
    /* Grab HTTP response stream. This will be used by the application to send events back to the client */
    http_event_stream = stream;
    wiced_http_response_stream_enable_chunked_transfer( http_event_stream );
    wiced_http_response_stream_write_header( http_event_stream, HTTP_200_TYPE, CHUNKED_CONTENT_LENGTH, HTTP_CACHE_DISABLED, MIME_TYPE_TEXT_EVENT_STREAM );
    return 0;
}

static wiced_result_t send_event( void* arg )
{
    wiced_iso8601_time_t current_time;

    UNUSED_PARAMETER( arg );

    if ( http_event_stream == NULL )
    {
        return WICED_ERROR;
    }

    /* SSE is prefixed with "data: " */
//    WICED_VERIFY( wiced_http_response_stream_write( http_event_stream, (const void*)EVENT_STREAM_DATA, sizeof( EVENT_STREAM_DATA ) - 1 ) );

    /* Send current time back to the client */
//    wiced_time_get_iso8601_time( &current_time );
//    WICED_VERIFY( wiced_http_response_stream_write( http_event_stream, (const void*)&current_time, sizeof( current_time ) ) );

    // ------ rohm kx122 g sensor ------
//    kx122_get(NULL,NULL);   // update g sensor data
//    WICED_VERIFY( wiced_http_response_stream_write( http_event_stream, (const void*)&kx122_data, sizeof( kx122_data ) ) );

    for(uint32_t i=0;i<datnum;i++){
        if( strlen( json_data[i].list ) > 0 ){
            /* SSE is prefixed with "data: " */
            WICED_VERIFY( wiced_http_response_stream_write( http_event_stream, (const void*)EVENT_STREAM_DATA, sizeof( EVENT_STREAM_DATA ) - 1 ) );
            /* json command data */
            WICED_VERIFY( wiced_http_response_stream_write( http_event_stream, (const void*)&json_data[i].list, strlen( json_data[i].list ) ) );
            /* SSE is ended with two line feeds */
            WICED_VERIFY( wiced_http_response_stream_write( http_event_stream, (const void*)LFLF, sizeof( LFLF ) - 1 ) );
        }
    }

    /* SSE is prefixed with "data: " */
    WICED_VERIFY( wiced_http_response_stream_write( http_event_stream, (const void*)EVENT_STREAM_DATA, sizeof( EVENT_STREAM_DATA ) - 1 ) );
    /* SSE is ended with two line feeds */
    WICED_VERIFY( wiced_http_response_stream_write( http_event_stream, (const void*)LFLF, sizeof( LFLF ) - 1 ) );
    WICED_VERIFY( wiced_http_response_stream_flush( http_event_stream ) );
    return WICED_SUCCESS;
}

