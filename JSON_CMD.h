#include "wiced.h"
#include "frozen.h"

#define rawsiz  512     // json raw data buffer sizes
#define toknum  20      // number of token
#define datnum  10      // number of data

typedef struct{
    struct json_token key[datnum];
    struct json_token val[datnum];
    uint8_t list[100];
}JDAT;

extern JDAT json_data[toknum];

int json_parser(int argc, char *argv[]);
