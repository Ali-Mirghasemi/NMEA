/**
 * @file NMEAConfig.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief This is configuration file for NMEA library
 * @version 0.1
 * @date 2025-11-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _NMEA_CONFIG_H_
#define _NMEA_CONFIG_H_

#include <stdint.h>

/* Check User Config file exists to include it or not */
#ifndef __has_include
    #define __has_include(X)    0
#endif

#if defined(NMEA_USER_CONFIG) || __has_include("NMEAUserConfig.h")
    #include "NMEAUserConfig.h"
#endif

#ifndef NMEA_LIB_STREAM
    #define NMEA_LIB_STREAM     0
#endif

/******************************************************************************/
/*                                Configuration                               */
/******************************************************************************/
/**
 * @brief Enable to use StreamIn for parse input gps data
*/
#ifndef NMEA_SUPPORT_ISTREAM
    #define NMEA_SUPPORT_ISTREAM                    (1 || NMEA_LIB_STREAM)
#endif
/**
 * @brief Enable Args in NMEA structure
 */
#ifndef NMEA_SUPPORT_ARGS
    #define NMEA_SUPPORT_ARGS                       1
 #endif
/**
 * @brief Enable Callback per message
 */
#ifndef NMEA_SUPPORT_MULTI_CALLBACK
    #define NMEA_SUPPORT_MULTI_CALLBACK             1
 #endif
/**
 * @brief Enable Feature to Access callbacks and advance features
 * Disable it if you want use basic parse
 */
#ifndef NMEA_SUPPORT_MODULAR
    #define NMEA_SUPPORT_MODULAR                    1
 #endif
/**
 * @brief Enable deinit message before parse
 */
#ifndef NMEA_SUPPORT_DEINIT_MESSAGE
    #define NMEA_SUPPORT_DEINIT_MESSAGE             1
 #endif

/* --------------------------- Messages Support ----------------------------- */
#ifndef NMEA_MESSAGE_GGA
    #define NMEA_MESSAGE_GGA                        1
#endif
#if NMEA_MESSAGE_GGA
    #ifndef NMEA_MESSAGE_GGA_LATITUDE    
        #define NMEA_MESSAGE_GGA_LATITUDE           1
    #endif
    #ifndef NMEA_MESSAGE_GGA_LONGITUDE
        #define NMEA_MESSAGE_GGA_LONGITUDE          1
    #endif
    #ifndef NMEA_MESSAGE_GGA_HDOP
        #define NMEA_MESSAGE_GGA_HDOP               1
    #endif
    #ifndef NMEA_MESSAGE_GGA_ALTITUDE
        #define NMEA_MESSAGE_GGA_ALTITUDE           1
    #endif
    #ifndef NMEA_MESSAGE_GGA_GEOID
        #define NMEA_MESSAGE_GGA_GEOID              1
    #endif
    #ifndef NMEA_MESSAGE_GGA_DIFFREFSTATIONID
        #define NMEA_MESSAGE_GGA_DIFFREFSTATIONID   1
    #endif
    #ifndef NMEA_MESSAGE_GGA_TIME
        #define NMEA_MESSAGE_GGA_TIME               1
    #endif
    #ifndef NMEA_MESSAGE_GGA_POSITIONFIX
        #define NMEA_MESSAGE_GGA_POSITIONFIX        1
    #endif
    #ifndef NMEA_MESSAGE_GGA_SATELLITESUSED
        #define NMEA_MESSAGE_GGA_SATELLITESUSED     1
    #endif
    #ifndef NMEA_MESSAGE_GGA_ALTITUDE_UNIT
        #define NMEA_MESSAGE_GGA_ALTITUDE_UNIT      1
    #endif
    #ifndef NMEA_MESSAGE_GGA_GEOIDUNITS
        #define NMEA_MESSAGE_GGA_GEOIDUNITS         1
    #endif
    #ifndef NMEA_MESSAGE_GGA_AGEOFDIFF
        #define NMEA_MESSAGE_GGA_AGEOFDIFF          1
    #endif
#endif

#ifndef NMEA_MESSAGE_GLL
    #define NMEA_MESSAGE_GLL                        1
#endif
#if NMEA_MESSAGE_GLL
    #ifndef NMEA_MESSAGE_GLL_LATITUDE    
        #define NMEA_MESSAGE_GLL_LATITUDE           1
    #endif
    #ifndef NMEA_MESSAGE_GLL_LONGITUDE
        #define NMEA_MESSAGE_GLL_LONGITUDE          1
    #endif
    #ifndef NMEA_MESSAGE_GLL_TIME
        #define NMEA_MESSAGE_GLL_TIME               1
    #endif
    #ifndef NMEA_MESSAGE_GLL_STATUS
        #define NMEA_MESSAGE_GLL_STATUS             1
    #endif
    #ifndef NMEA_MESSAGE_GLL_MODE
        #define NMEA_MESSAGE_GLL_MODE               1
    #endif
#endif

#ifndef NMEA_MESSAGE_GSA
    #define NMEA_MESSAGE_GSA                        1
#endif
#if NMEA_MESSAGE_GSA
    #ifndef NMEA_MESSAGE_GSA_PDOP    
        #define NMEA_MESSAGE_GSA_PDOP               1
    #endif
    #ifndef NMEA_MESSAGE_GSA_HDOP
        #define NMEA_MESSAGE_GSA_HDOP               1
    #endif
    #ifndef NMEA_MESSAGE_GSA_VDOP
        #define NMEA_MESSAGE_GSA_VDOP               1
    #endif
    #ifndef NMEA_MESSAGE_GSA_MODE
        #define NMEA_MESSAGE_GSA_MODE               1
    #endif
    #ifndef NMEA_MESSAGE_GSA_FIX_STATUS
        #define NMEA_MESSAGE_GSA_FIX_STATUS         1
    #endif
    #ifndef NMEA_MESSAGE_GSA_SATELLITESUSED
        #define NMEA_MESSAGE_GSA_SATELLITESUSED     1
    #endif
#endif

#ifndef NMEA_MESSAGE_GSV
    #define NMEA_MESSAGE_GSV                        1
#endif
#if NMEA_MESSAGE_GSV
    #ifndef NMEA_MESSAGE_GSV_SATELLITES    
        #define NMEA_MESSAGE_GSV_SATELLITES         1
    #endif
    #ifndef NMEA_MESSAGE_GSV_TOTALMESSAGES
        #define NMEA_MESSAGE_GSV_TOTALMESSAGES      1
    #endif
    #ifndef NMEA_MESSAGE_GSV_MESSAGENUMBER
        #define NMEA_MESSAGE_GSV_MESSAGENUMBER      1
    #endif
    #ifndef NMEA_MESSAGE_GSV_SATELLITESINVIEW
        #define NMEA_MESSAGE_GSV_SATELLITESINVIEW   1
    #endif
#endif

#ifndef NMEA_MESSAGE_MSS
    #define NMEA_MESSAGE_MSS                        1
#endif
#if NMEA_MESSAGE_MSS
    #ifndef NMEA_MESSAGE_MSS_BEACONFREQUENCY    
        #define NMEA_MESSAGE_MSS_BEACONFREQUENCY    1
    #endif
    #ifndef NMEA_MESSAGE_MSS_BEACONBITRATE
        #define NMEA_MESSAGE_MSS_BEACONBITRATE      1
    #endif
    #ifndef NMEA_MESSAGE_MSS_SIGNALSTRENGTH
        #define NMEA_MESSAGE_MSS_SIGNALSTRENGTH     1
    #endif
    #ifndef NMEA_MESSAGE_MSS_SNR
        #define NMEA_MESSAGE_MSS_SNR                1
    #endif
    #ifndef NMEA_MESSAGE_MSS_CHANNELNUMBER
        #define NMEA_MESSAGE_MSS_CHANNELNUMBER      1
    #endif
#endif

#ifndef NMEA_MESSAGE_RMC
    #define NMEA_MESSAGE_RMC                        1
#endif
#if NMEA_MESSAGE_RMC
    #ifndef NMEA_MESSAGE_RMC_LATITUDE    
        #define NMEA_MESSAGE_RMC_LATITUDE           1
    #endif
    #ifndef NMEA_MESSAGE_RMC_LONGITUDE
        #define NMEA_MESSAGE_RMC_LONGITUDE          1
    #endif
    #ifndef NMEA_MESSAGE_RMC_MAGNETICVARIATION
        #define NMEA_MESSAGE_RMC_MAGNETICVARIATION  1
    #endif
    #ifndef NMEA_MESSAGE_RMC_SPEEDOVERGROUND
        #define NMEA_MESSAGE_RMC_SPEEDOVERGROUND    1
    #endif
    #ifndef NMEA_MESSAGE_RMC_COURSEOVERGROUND
        #define NMEA_MESSAGE_RMC_COURSEOVERGROUND   1
    #endif
    #ifndef NMEA_MESSAGE_RMC_TIME
        #define NMEA_MESSAGE_RMC_TIME               1
    #endif
    #ifndef NMEA_MESSAGE_RMC_DATE
        #define NMEA_MESSAGE_RMC_DATE               1
    #endif
    #ifndef NMEA_MESSAGE_RMC_STATUS
        #define NMEA_MESSAGE_RMC_STATUS             1
    #endif
    #ifndef NMEA_MESSAGE_RMC_MODE
        #define NMEA_MESSAGE_RMC_MODE               1
    #endif
#endif

#ifndef NMEA_MESSAGE_VTG
    #define NMEA_MESSAGE_VTG                        1
#endif
#if NMEA_MESSAGE_VTG
    #ifndef NMEA_MESSAGE_VTG_COURSE1    
        #define NMEA_MESSAGE_VTG_COURSE1            1
    #endif
    #ifndef NMEA_MESSAGE_VTG_COURSE2
        #define NMEA_MESSAGE_VTG_COURSE2            1
    #endif
    #ifndef NMEA_MESSAGE_VTG_SPEED1
        #define NMEA_MESSAGE_VTG_SPEED1             1
    #endif
    #ifndef NMEA_MESSAGE_VTG_SPEED2
        #define NMEA_MESSAGE_VTG_SPEED2             1
    #endif
    #ifndef NMEA_MESSAGE_VTG_REFERENCE1
        #define NMEA_MESSAGE_VTG_REFERENCE1         1
    #endif
    #ifndef NMEA_MESSAGE_VTG_REFERENCE2
        #define NMEA_MESSAGE_VTG_REFERENCE2         1
    #endif
    #ifndef NMEA_MESSAGE_VTG_SPEEDUNITS1
        #define NMEA_MESSAGE_VTG_SPEEDUNITS1        1
    #endif
    #ifndef NMEA_MESSAGE_VTG_SPEEDUNITS2
        #define NMEA_MESSAGE_VTG_SPEEDUNITS2        1
    #endif
    #ifndef NMEA_MESSAGE_VTG_MODE
        #define NMEA_MESSAGE_VTG_MODE               1
    #endif
#endif

#ifndef NMEA_MESSAGE_ZDA
    #define NMEA_MESSAGE_ZDA                        1
#endif
#if NMEA_MESSAGE_ZDA
    #ifndef NMEA_MESSAGE_ZDA_TIME    
        #define NMEA_MESSAGE_ZDA_TIME               1
    #endif
    #ifndef NMEA_MESSAGE_ZDA_YEAR
        #define NMEA_MESSAGE_ZDA_YEAR               1
    #endif
    #ifndef NMEA_MESSAGE_ZDA_MONTH
        #define NMEA_MESSAGE_ZDA_MONTH              1
    #endif
    #ifndef NMEA_MESSAGE_ZDA_DAY
        #define NMEA_MESSAGE_ZDA_DAY                1
    #endif
    #ifndef NMEA_MESSAGE_ZDA_LOCALZONEHOURS
        #define NMEA_MESSAGE_ZDA_LOCALZONEHOURS     1
    #endif
    #ifndef NMEA_MESSAGE_ZDA_LOCALZONEMINUTES
        #define NMEA_MESSAGE_ZDA_LOCALZONEMINUTES   1
    #endif
#endif

#ifndef NMEA_MESSAGE_CHECKSUM
    #define NMEA_MESSAGE_CHECKSUM                   1
#endif
#if NMEA_MESSAGE_CHECKSUM
    #ifndef NMEA_MESSAGE_VALIDATE_CHECKSUM    
        #define NMEA_MESSAGE_VALIDATE_CHECKSUM      1
    #endif
#endif

#if NMEA_SUPPORT_ISTREAM
    #ifndef NMEA_MAX_BUFF_SIZE    
        #define NMEA_MAX_BUFF_SIZE                  128
    #endif
#endif

#ifndef NMEA_SUPPORT_BEARING
    #define NMEA_SUPPORT_BEARING                    1
#endif

/******************************************************************************/

#endif // _NMEA_CONFIG_H_
