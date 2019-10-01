
#include "JSON_CMD.h"
#include<stdio.h>
#include<string.h>

JDAT json_data[toknum] = {NULL};

uint32_t asc2dec( char *asc, uint32_t *dec ){

    uint32_t tmp = 0;

    for( uint32_t i=0 ;i<strlen(asc) ;i++ ){
        if( (asc[i] < '0') || (asc[i] > '9') ){
            return i;
        }
        tmp = (tmp*10) + (asc[i]-'0');
    }
    *dec = tmp;

    return 0;

}

int json_parser(int argc, char *argv[])
{
    //    json_parser {"BM1383GLV(hPa)":1017,"BD7411_Detect":false,"ML8511A_UV(mW/cm2)":0,"BD1020_Temp(degC)":22}
    //    json_parser {"a":123,"b":456,"c":789,"d":111}
    //    json_parser {"a": 123}
//    json_parser 0 {"BM1383GLV(hPa)":1016}
//    json_parser 1 {"BM1422AGMV_X(uT)":4,"BM1422AGMV_Y(uT)":-36,"BM1422AGMV_Z(uT)":-66}
//    json_parser 2 {"PRP0521_Proximity(count)":1,"PRP0521_Ambient(lx)":574}
//    json_parser 3 {"KX224_X(mG)":26,"KX224_Y(mG)":157,"KX224_Z(mG)":1081}
//    json_parser 4 {"ML8511A_UV(mW/cm2)":0}
//    json_parser 5 {"BD1020_Temp(C)":23}
//    json_parser 6 {"BD7411_detect":0}
//    json_parser 7 {"BH1749_RED":444,"BH1749_GREEN":1050,"BH1749_BLUE":459,"BH1749_IR":28,"BH1749_GREEN2":1050}




//    struct json_token key, val;

    // ------ get raw data from command line ------
    uint8_t raw[rawsiz] = "";
    uint32_t sn;
    if( asc2dec( (char*)argv[1], &sn ) != 0 ){
        printf( "Second parameter must be between 0 and %d.", toknum-1 );
        return sn;
    }
    if( sn >= toknum ){
        printf( "Second parameter must be between 0 and %d.", toknum-1 );
        return sn;
    }

    for(int i=2; i<argc; i++){
        strcat(raw, argv[i]);
    }

    // ------ get key and val ------
    memset( json_data[sn].list, 0x00, sizeof(json_data[sn].list) );
    void *h = NULL;
    for( int i=0; ; i++ ){
        h = json_next_key(raw, strlen(raw), h, ".", &json_data[sn].key[i], &json_data[sn].val[i]);
        if( h == NULL ){
            break;
        }
        char tmp[100] = "";
        sprintf( tmp, "[%.*s] %.*s\t", json_data[sn].key[i].len, json_data[sn].key[i].ptr, json_data[sn].val[i].len, json_data[sn].val[i].ptr );
        strcat( json_data[sn].list, tmp );
    }

//    printf("%s",json_data[sn].list);


//    json_data[sn] = tmp;
//    printf("SN=%d",sn);

//    for( int i=0 ; i<toknum ; i++ ){
//        for( int j=0 ; j<datnum ; j++ ){
//            if( json_data[i].key[j].len != 0){
//                printf( "[%s]%d,", json_data[i].key[j].ptr, json_data[i].val[j].ptr);
//            }
//        }
//        printf("\r\n");
//    }

}


