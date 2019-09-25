NAME := App_work_zenitron_sensor_http

$(NAME)_SOURCES    := start.c

$(NAME)_COMPONENTS := daemons/HTTP_server \
                      daemons/device_configuration \
                      protocols/Xively \
                      drivers/sensors/NCP18XH103J03RB \
                      protocols/SNTP \
                      daemons/Gedday \
                      inputs/button_manager \
                      utilities/command_console \
					  utilities/command_console/ping

$(NAME)_RESOURCES  := apps/temp_control/main.html \
                      apps/temp_control/data.html \
                      images/cypresslogo.png \
                      images/cypresslogo_line.png \
                      images/favicon.ico \
                      styles/buttons.css \
                      scripts/general_ajax_script.js \
                      scripts/wpad.dat

GLOBAL_DEFINES     := USE_SELF_SIGNED_TLS_CERT

VALID_PLATFORMS    := Quicksilver*

#------ add AzureWave platform ------
VALID_PLATFORMS    += AWCU*

