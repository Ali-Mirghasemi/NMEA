/**
 * @file NMEAUserConfig.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief This is configuration template for NMEA library
 * @version 0.1
 * @date 2025-11-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _NMEA_USER_CONFIG_H_
#define _NMEA_USER_CONFIG_H_

#include <stdint.h>

/**
 * @brief Enable to use StreamIn for parse input gps data
*/
//#define NMEA_SUPPORT_ISTREAM                    1
/**
 * @brief Enable Args in NMEA structure
 */
//#define NMEA_SUPPORT_ARGS                       1
/**
 * @brief Enable Callback per message
 */
//#define NMEA_SUPPORT_MULTI_CALLBACK             1
/**
 * @brief Enable Feature to Access callbacks and advance features
 * Disable it if you want use basic parse
 */
//#define NMEA_SUPPORT_MODULAR                    1
/**
 * @brief Enable deinit message before parse
 */
//#define NMEA_SUPPORT_DEINIT_MESSAGE             1

/* --------------------------- Messages Support ----------------------------- */
//#define NMEA_MESSAGE_GGA                          1
//#define NMEA_MESSAGE_GGA_LATITUDE                 1
//#define NMEA_MESSAGE_GGA_LONGITUDE                1
//#define NMEA_MESSAGE_GGA_HDOP                     1
//#define NMEA_MESSAGE_GGA_ALTITUDE                 1
//#define NMEA_MESSAGE_GGA_GEOID                    1
//#define NMEA_MESSAGE_GGA_DIFFREFSTATIONID         1
//#define NMEA_MESSAGE_GGA_TIME                     1
//#define NMEA_MESSAGE_GGA_POSITIONFIX              1
//#define NMEA_MESSAGE_GGA_SATELLITESUSED           1
//#define NMEA_MESSAGE_GGA_ALTITUDE_UNIT            1
//#define NMEA_MESSAGE_GGA_GEOIDUNITS               1
//#define NMEA_MESSAGE_GGA_AGEOFDIFF                1

//#define NMEA_MESSAGE_GLL                          1
//#define NMEA_MESSAGE_GLL_LATITUDE                 1
//#define NMEA_MESSAGE_GLL_LONGITUDE                1
//#define NMEA_MESSAGE_GLL_TIME                     1
//#define NMEA_MESSAGE_GLL_STATUS                   1
//#define NMEA_MESSAGE_GLL_MODE                     1

//#define NMEA_MESSAGE_GSA                          1
//#define NMEA_MESSAGE_GSA_PDOP                     1
//#define NMEA_MESSAGE_GSA_HDOP                     1
//#define NMEA_MESSAGE_GSA_VDOP                     1
//#define NMEA_MESSAGE_GSA_MODE                     1
//#define NMEA_MESSAGE_GSA_FIX_STATUS               1
//#define NMEA_MESSAGE_GSA_SATELLITESUSED           1

//#define NMEA_MESSAGE_GSV                          1
//#define NMEA_MESSAGE_GSV_SATELLITES               1
//#define NMEA_MESSAGE_GSV_TOTALMESSAGES            1
//#define NMEA_MESSAGE_GSV_MESSAGENUMBER            1
//#define NMEA_MESSAGE_GSV_SATELLITESINVIEW         1

//#define NMEA_MESSAGE_MSS                          1
//#define NMEA_MESSAGE_MSS_BEACONFREQUENCY          1
//#define NMEA_MESSAGE_MSS_BEACONBITRATE            1
//#define NMEA_MESSAGE_MSS_SIGNALSTRENGTH           1
//#define NMEA_MESSAGE_MSS_SNR                      1
//#define NMEA_MESSAGE_MSS_CHANNELNUMBER            1

//#define NMEA_MESSAGE_RMC                          1
//#define NMEA_MESSAGE_RMC_LATITUDE                 1
//#define NMEA_MESSAGE_RMC_LONGITUDE                1
//#define NMEA_MESSAGE_RMC_MAGNETICVARIATION        1
//#define NMEA_MESSAGE_RMC_SPEEDOVERGROUND          1
//#define NMEA_MESSAGE_RMC_COURSEOVERGROUND         1
//#define NMEA_MESSAGE_RMC_TIME                     1
//#define NMEA_MESSAGE_RMC_DATE                     1
//#define NMEA_MESSAGE_RMC_STATUS                   1
//#define NMEA_MESSAGE_RMC_MODE                     1

//#define NMEA_MESSAGE_VTG                          1
//#define NMEA_MESSAGE_VTG_COURSE1                  1
//#define NMEA_MESSAGE_VTG_COURSE2                  1
//#define NMEA_MESSAGE_VTG_SPEED1                   1
//#define NMEA_MESSAGE_VTG_SPEED2                   1
//#define NMEA_MESSAGE_VTG_REFERENCE1               1
//#define NMEA_MESSAGE_VTG_REFERENCE2               1
//#define NMEA_MESSAGE_VTG_SPEEDUNITS1              1
//#define NMEA_MESSAGE_VTG_SPEEDUNITS2              1
//#define NMEA_MESSAGE_VTG_MODE                     1

//#define NMEA_MESSAGE_ZDA                          1
//#define NMEA_MESSAGE_ZDA_TIME                     1
//#define NMEA_MESSAGE_ZDA_YEAR                     1
//#define NMEA_MESSAGE_ZDA_MONTH                    1
//#define NMEA_MESSAGE_ZDA_DAY                      1
//#define NMEA_MESSAGE_ZDA_LOCALZONEHOURS           1
//#define NMEA_MESSAGE_ZDA_LOCALZONEMINUTES         1

//#define NMEA_MESSAGE_CHECKSUM                     1
//#define NMEA_MESSAGE_VALIDATE_CHECKSUM            1

//#define NMEA_MAX_BUFF_SIZE                        128

//#define NMEA_SUPPORT_BEARING                      1

#endif // _NMEA_USER_CONFIG_H_
