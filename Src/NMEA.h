/**
 * @file NMEA.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief This is NMEA library parser and can use it on every hardware
 * @version 0.1
 * @date 2024-02-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _NMEA_H_
#define _NMEA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/******************************************************************************/
/*                                Configuration                               */
/******************************************************************************/
/**
 * @brief Enable to use IStream for parse input gps data
*/
#define NMEA_SUPPORT_ISTREAM                    1
/**
 * @brief Enable Args in NMEA structure
 */
#define NMEA_SUPPORT_ARGS                       1
/**
 * @brief Enable Callback per message
 */
#define NMEA_SUPPORT_MULTI_CALLBACK             1
/**
 * @brief Enable Feature to Access callbacks and advance features
 * Disable it if you want use basic parse
 */
#define NMEA_SUPPORT_MODULAR                    1
/**
 * @brief Enable deinit message before parse
 */
#define NMEA_SUPPORT_DEINIT_MESSAGE             1

/* --------------------------- Messages Support ----------------------------- */
#define NMEA_MESSAGE_GGA                        1
#if NMEA_MESSAGE_GGA
    #define NMEA_MESSAGE_GGA_LATITUDE           1
    #define NMEA_MESSAGE_GGA_LONGITUDE          1
    #define NMEA_MESSAGE_GGA_HDOP               1
    #define NMEA_MESSAGE_GGA_ALTITUDE           1
    #define NMEA_MESSAGE_GGA_GEOID              1
    #define NMEA_MESSAGE_GGA_DIFFREFSTATIONID   1
    #define NMEA_MESSAGE_GGA_TIME               1
    #define NMEA_MESSAGE_GGA_POSITIONFIX        1
    #define NMEA_MESSAGE_GGA_SATELLITESUSED     1
    #define NMEA_MESSAGE_GGA_ALTITUDE_UNIT      1
    #define NMEA_MESSAGE_GGA_GEOIDUNITS         1
    #define NMEA_MESSAGE_GGA_AGEOFDIFF          1
#endif

#define NMEA_MESSAGE_GLL                        1
#if NMEA_MESSAGE_GLL
    #define NMEA_MESSAGE_GLL_LATITUDE           1
    #define NMEA_MESSAGE_GLL_LONGITUDE          1
    #define NMEA_MESSAGE_GLL_TIME               1
    #define NMEA_MESSAGE_GLL_STATUS             1
    #define NMEA_MESSAGE_GLL_MODE               1
#endif

#define NMEA_MESSAGE_GSA                        1
#if NMEA_MESSAGE_GSA
    #define NMEA_MESSAGE_GSA_PDOP               1
    #define NMEA_MESSAGE_GSA_HDOP               1
    #define NMEA_MESSAGE_GSA_VDOP               1
    #define NMEA_MESSAGE_GSA_MODE               1
    #define NMEA_MESSAGE_GSA_FIX_STATUS         1
    #define NMEA_MESSAGE_GSA_SATELLITESUSED     1
#endif

#define NMEA_MESSAGE_GSV                        1
#if NMEA_MESSAGE_GSV
    #define NMEA_MESSAGE_GSV_SATELLITES         1
    #define NMEA_MESSAGE_GSV_TOTALMESSAGES   1
    #define NMEA_MESSAGE_GSV_MESSAGENUMBER      1
    #define NMEA_MESSAGE_GSV_SATELLITESINVIEW   1
#endif

#define NMEA_MESSAGE_MSS                        1
#if NMEA_MESSAGE_MSS
    #define NMEA_MESSAGE_MSS_BEACONFREQUENCY    1
    #define NMEA_MESSAGE_MSS_BEACONBITRATE      1
    #define NMEA_MESSAGE_MSS_SIGNALSTRENGTH     1
    #define NMEA_MESSAGE_MSS_SNR                1
    #define NMEA_MESSAGE_MSS_CHANNELNUMBER      1
#endif

#define NMEA_MESSAGE_RMC                        1
#if NMEA_MESSAGE_RMC
    #define NMEA_MESSAGE_RMC_LATITUDE           1
    #define NMEA_MESSAGE_RMC_LONGITUDE          1
    #define NMEA_MESSAGE_RMC_MAGNETICVARIATION  1
    #define NMEA_MESSAGE_RMC_SPEEDOVERGROUND    1
    #define NMEA_MESSAGE_RMC_COURSEOVERGROUND   1
    #define NMEA_MESSAGE_RMC_TIME               1
    #define NMEA_MESSAGE_RMC_DATE               1
    #define NMEA_MESSAGE_RMC_STATUS             1
    #define NMEA_MESSAGE_RMC_MODE               1
#endif

#define NMEA_MESSAGE_VTG                        1
#if NMEA_MESSAGE_VTG
    #define NMEA_MESSAGE_VTG_COURSE1            1
    #define NMEA_MESSAGE_VTG_COURSE2            1
    #define NMEA_MESSAGE_VTG_SPEED1             1
    #define NMEA_MESSAGE_VTG_SPEED2             1
    #define NMEA_MESSAGE_VTG_REFERENCE1         1
    #define NMEA_MESSAGE_VTG_REFERENCE2         1
    #define NMEA_MESSAGE_VTG_SPEEDUNITS1        1
    #define NMEA_MESSAGE_VTG_SPEEDUNITS2        1
    #define NMEA_MESSAGE_VTG_MODE               1
#endif

#define NMEA_MESSAGE_ZDA                        1
#if NMEA_MESSAGE_ZDA
    #define NMEA_MESSAGE_ZDA_TIME               1
    #define NMEA_MESSAGE_ZDA_YEAR               1
    #define NMEA_MESSAGE_ZDA_MONTH              1
    #define NMEA_MESSAGE_ZDA_DAY                1
    #define NMEA_MESSAGE_ZDA_LOCALZONEHOURS      1
    #define NMEA_MESSAGE_ZDA_LOCALZONEMINUTES   1
#endif

#define NMEA_MESSAGE_CHECKSUM                   1
#if NMEA_MESSAGE_CHECKSUM
    #define NMEA_MESSAGE_VALIDATE_CHECKSUM      1
#endif

#if NMEA_SUPPORT_ISTREAM
    #define NMEA_MAX_BUFF_SIZE                  128
#endif

/******************************************************************************/
/* Includes */
#include "Str.h"
#if NMEA_SUPPORT_ISTREAM
    #include "InputStream.h"
#endif

/* Defines */
#define NMEA_MESSAGE_GGA_FIELDS_LENGTH          ((NMEA_MESSAGE_GGA_LATITUDE * 2) + \
                                                 (NMEA_MESSAGE_GGA_LONGITUDE * 2) + \
                                                 NMEA_MESSAGE_GGA_HDOP + \
                                                 NMEA_MESSAGE_GGA_ALTITUDE + \
                                                 NMEA_MESSAGE_GGA_GEOID + \
                                                 NMEA_MESSAGE_GGA_DIFFREFSTATIONID + \
                                                 NMEA_MESSAGE_GGA_TIME + \
                                                 NMEA_MESSAGE_GGA_POSITIONFIX + \
                                                 NMEA_MESSAGE_GGA_SATELLITESUSED + \
                                                 NMEA_MESSAGE_GGA_ALTITUDE_UNIT + \
                                                 NMEA_MESSAGE_GGA_GEOIDUNITS + \
                                                 NMEA_MESSAGE_GGA_AGEOFDIFF)

#define NMEA_MESSAGE_GLL_FIELDS_LENGTH          ((NMEA_MESSAGE_GLL_LATITUDE * 2) + \
                                                 (NMEA_MESSAGE_GLL_LONGITUDE * 2) + \
                                                 NMEA_MESSAGE_GLL_TIME + \
                                                 NMEA_MESSAGE_GLL_STATUS + \
                                                 NMEA_MESSAGE_GLL_MODE)

#define NMEA_MESSAGE_GSA_FIELDS_LENGTH          (NMEA_MESSAGE_GSA_PDOP + \
                                                 NMEA_MESSAGE_GSA_HDOP + \
                                                 NMEA_MESSAGE_GSA_VDOP + \
                                                 NMEA_MESSAGE_GSA_MODE + \
                                                 NMEA_MESSAGE_GSA_FIX_STATUS + \
                                                 (NMEA_MESSAGE_GSA_SATELLITESUSED * NMEA_GSA_MAX_SATELLITES))

#define NMEA_MESSAGE_GSV_FIELDS_LENGTH          (NMEA_MESSAGE_GSV_SATELLITES + \
                                                 NMEA_MESSAGE_GSV_TOTALMESSAGES + \
                                                 NMEA_MESSAGE_GSV_MESSAGENUMBER + \
                                                 (NMEA_MESSAGE_GSV_SATELLITESINVIEW * NMEA_GSV_MAX_SATELLITES * 4))

#define NMEA_MESSAGE_MSS_FIELDS_LENGTH          (NMEA_MESSAGE_MSS_BEACONFREQUENCY + \
                                                 NMEA_MESSAGE_MSS_BEACONBITRATE + \
                                                 NMEA_MESSAGE_MSS_SIGNALSTRENGTH + \
                                                 NMEA_MESSAGE_MSS_SNR + \
                                                 NMEA_MESSAGE_MSS_CHANNELNUMBER)

#define NMEA_MESSAGE_RMC_FIELDS_LENGTH          ((NMEA_MESSAGE_RMC_LATITUDE * 2) + \
                                                 (NMEA_MESSAGE_RMC_LONGITUDE * 2) + \
                                                 (NMEA_MESSAGE_RMC_MAGNETICVARIATION * 2) + \
                                                 NMEA_MESSAGE_RMC_SPEEDOVERGROUND + \
                                                 NMEA_MESSAGE_RMC_COURSEOVERGROUND + \
                                                 NMEA_MESSAGE_RMC_TIME + \
                                                 NMEA_MESSAGE_RMC_DATE + \
                                                 NMEA_MESSAGE_RMC_STATUS + \
                                                 NMEA_MESSAGE_RMC_MODE)

#define NMEA_MESSAGE_VTG_FIELDS_LENGTH          (NMEA_MESSAGE_VTG_COURSE1 + \
                                                 NMEA_MESSAGE_VTG_COURSE2 + \
                                                 NMEA_MESSAGE_VTG_SPEED1 + \
                                                 NMEA_MESSAGE_VTG_SPEED2 + \
                                                 NMEA_MESSAGE_VTG_REFERENCE1 + \
                                                 NMEA_MESSAGE_VTG_REFERENCE2 + \
                                                 NMEA_MESSAGE_VTG_SPEEDUNITS1 + \
                                                 NMEA_MESSAGE_VTG_SPEEDUNITS2 + \
                                                 NMEA_MESSAGE_VTG_MODE)

#define NMEA_MESSAGE_ZDA_FIELDS_LENGTH          (NMEA_MESSAGE_ZDA_TIME + \
                                                 NMEA_MESSAGE_ZDA_YEAR + \
                                                 NMEA_MESSAGE_ZDA_MONTH + \
                                                 NMEA_MESSAGE_ZDA_DAY + \
                                                 NMEA_MESSAGE_ZDA_LOCALZONEHOURS + \
                                                 NMEA_MESSAGE_ZDA_LOCALZONEMINUTES)                                                                                                  

#define NMEA_GSA_MAX_SATELLITES                 12
#define NMEA_GSV_MAX_SATELLITES                 4
/**
 * @brief NMEA Message Types that support
 */
typedef enum {
#if NMEA_MESSAGE_GGA
    NMEA_MessageType_GGA,               /*< Global Positioning System Fixed Data */
#endif
#if NMEA_MESSAGE_GLL
    NMEA_MessageType_GLL,               /*< Geographic Position - Latitude/Longitude */
#endif
#if NMEA_MESSAGE_GSA
    NMEA_MessageType_GSA,               /*< GNSS DOP and Active Satellites */
#endif
#if NMEA_MESSAGE_GSV
    NMEA_MessageType_GSV,               /*< GNSS Satellites in View */
#endif
#if NMEA_MESSAGE_MSS
    NMEA_MessageType_MSS,               /*< MSK Receiver Signal */
#endif
#if NMEA_MESSAGE_RMC
    NMEA_MessageType_RMC,               /*< Recommended Minimum Specific GNSS Data */
#endif
#if NMEA_MESSAGE_VTG
    NMEA_MessageType_VTG,               /*< Course Over Ground and Ground Speed */
#endif
#if NMEA_MESSAGE_ZDA
    NMEA_MessageType_ZDA,               /*< SiRF Timing Message */
#endif
    // **Not Supported Yet**
    // NMEA_MessageType_140,
    // NMEA_MessageType_150,
    // NMEA_MessageType_151,
    // NMEA_MessageType_152,
    // NMEA_MessageType_154,
    // NMEA_MessageType_155,
    NMEA_MessageType_Reserved,
    NMEA_MessageType_Unknown        = 255,
} NMEA_MessageType;
/**
 * @brief NMEA Talker Id
 */
typedef enum {
    NMEA_Id_Beidou,                     /*< BD */
    NMEA_Id_Galileo,                    /*< GA */
    NMEA_Id_BeidouGlobal,               /*< GB */
    NMEA_Id_GLONASS,                    /*< GL */
    NMEA_Id_GPS_GLONASS,                /*< GN */
    NMEA_Id_GPS,                        /*< GP */
    NMEA_Id_Unknown             = 255,
} NMEA_Id;
/**
 * @brief NMEA Protocol Header
 */
typedef struct {
    uint8_t                     Id;     /*< NMEA_Id */
    uint8_t                     Type;   /*< NMEA_MessageType */
} NMEA_Message_Header;
/**
 * @brief NMEA UTC Time
 */
typedef struct {
    uint16_t                    Milliseconds;
    uint8_t                     Hours;
    uint8_t                     Minutes;
    uint8_t                     Seconds;
} NMEA_Time;
/**
 * @brief NMEA UTC Date
 */
typedef struct {
    uint8_t                     Year;
    uint8_t                     Month;
    uint8_t                     Day;
} NMEA_Date;
/**
 * @brief NMEA East/West indicator
 */
typedef enum {
    NMEA_Indicator_North        = 'N',
    NMEA_Indicator_South        = 'S',
    NMEA_Indicator_East         = 'E',
    NMEA_Indicator_West         = 'W',  
} NMEA_Indicator;
/**
 * @brief NMEA Coordinate
 */
typedef struct {
    float                       Minutes;
    uint8_t                     Degrees;
    uint8_t                     Indicator;
} NMEA_Coordinate;
/**
 * @brief NMEA Magnetic Variation
 */
typedef struct {
    float                       Degrees;
    uint8_t                     Indicator;
} NMEA_MagneticVariation;
/**
 * @brief NMEA Position Fix (Table 1-4)
 */
typedef enum {
    NMEA_PositionFix_NotValid         = 0,
    NMEA_PositionFix_GPS              = 1,
    NMEA_PositionFix_DifferentialGPS  = 2,
    NMEA_PositionFix_DeadReckoning    = 6,            /*< Only in NMEA 2.3 and above */
} NMEA_PositionFix;
/**
 * @brief NMEA Message data status
 */
typedef enum {
    NMEA_MessageStatus_NotValid     = 'V',
    NMEA_MessageStatus_Valid        = 'A',
} NMEA_MessageStatus;
/**
 * @brief NMEA Mode
 * Only in NMEA version 2.3 and above
 */
typedef enum {
    NMEA_Mode_Autonomous            = 'A',
    NMEA_Mode_DGPS                  = 'D',
    NMEA_Mode_DeadReckoning         = 'E', 
} NMEA_Mode;
/**
 * @brief Description of Mode1 in GSA Message
 */
typedef enum {
    NMEA_GSA_Mode_Manual            = 'M',
    NMEA_GSA_Mode_Auto              = 'A',
} NMEA_GSA_Mode;
/**
 * @brief Description of Mode2 in GSA Message
 */
typedef enum {
    NMEA_FixStatus_NotFix           = 1,
    NMEA_FixStatus_2D               = 2,
    NMEA_FixStatus_3D               = 3,
} NMEA_FixStatus;
/**
 * @brief Description of Satellite in view
 */
typedef struct {
    uint8_t                         Id;
    uint8_t                         Elevation;
    uint16_t                        Azimuth;
    uint8_t                         SNR;
} NMEA_SatelliteInView;

#if NMEA_MESSAGE_GGA
/**
 * @brief NMEA GGA Message structure
 */
typedef struct {
#if NMEA_MESSAGE_GGA_LATITUDE
    NMEA_Coordinate             Latitude;
#endif
#if NMEA_MESSAGE_GGA_LONGITUDE
    NMEA_Coordinate             Longitude;
#endif
#if NMEA_MESSAGE_GGA_HDOP
    float                       HDOP;
#endif
#if NMEA_MESSAGE_GGA_ALTITUDE
    float                       Altitude;
#endif
#if NMEA_MESSAGE_GGA_GEOID
    float                       Geoid;
#endif
#if NMEA_MESSAGE_GGA_DIFFREFSTATIONID
    uint32_t                    DiffRefStationId;
#endif
#if NMEA_MESSAGE_GGA_TIME
    NMEA_Time                   Time;
#endif
#if NMEA_MESSAGE_GGA_POSITIONFIX
    uint8_t                     PositionFix;
#endif
#if NMEA_MESSAGE_GGA_SATELLITESUSED
    uint8_t                     SatellitesUsed;
#endif
#if NMEA_MESSAGE_GGA_ALTITUDE_UNIT
    uint8_t                     AltitudeUnit;
#endif
#if NMEA_MESSAGE_GGA_GEOIDUNITS
    uint8_t                     GeoidUnits;
#endif
#if NMEA_MESSAGE_GGA_AGEOFDIFF
    uint8_t                     AgeOfDiff;
#endif
} NMEA_Message_GGA;
#endif

#if NMEA_MESSAGE_GLL
/**
 * @brief NMEA Geographic Position - Latitude/Longitude
 */
typedef struct {
#if NMEA_MESSAGE_GLL_LATITUDE
    NMEA_Coordinate             Latitude;
#endif
#if NMEA_MESSAGE_GLL_LONGITUDE
    NMEA_Coordinate             Longitude;
#endif
#if NMEA_MESSAGE_GLL_TIME
    NMEA_Time                   Time;
#endif
#if NMEA_MESSAGE_GLL_STATUS
    uint8_t                     Status;
#endif
#if NMEA_MESSAGE_GLL_MODE
    uint8_t                     Mode;
#endif
} NMEA_Message_GLL;
#endif

#if NMEA_MESSAGE_GSA
/**
 * @brief NMEA GNSS DOP and Active Satellites
 * 
 */
typedef struct {
#if NMEA_MESSAGE_GSA_PDOP
    float                       PDOP;
#endif
#if NMEA_MESSAGE_GSA_HDOP
    float                       HDOP;
#endif
#if NMEA_MESSAGE_GSA_VDOP
    float                       VDOP;
#endif
#if NMEA_MESSAGE_GSA_MODE
    uint8_t                     Mode;
#endif
#if NMEA_MESSAGE_GSA_FIX_STATUS
    uint8_t                     FixStatus;
#endif
#if NMEA_MESSAGE_GSA_SATELLITESUSED
    uint8_t                     SatellitesUsed[NMEA_GSA_MAX_SATELLITES];
#endif
} NMEA_Message_GSA;
#endif

#if NMEA_MESSAGE_GSV
/**
 * @brief NMEA GNSS Satellites in View
 */
typedef struct {
#if NMEA_MESSAGE_GSV_SATELLITES
    NMEA_SatelliteInView        Satellites[NMEA_GSV_MAX_SATELLITES];
#endif
#if NMEA_MESSAGE_GSV_TOTALMESSAGES
    uint8_t                     TotalMessages;
#endif
#if NMEA_MESSAGE_GSV_MESSAGENUMBER
    uint8_t                     MessageNumber;
#endif
#if NMEA_MESSAGE_GSV_SATELLITESINVIEW
    uint8_t                     SatellitesInView;
#endif
} NMEA_Message_GSV;
#endif

#if NMEA_MESSAGE_MSS
/**
 * @brief NMEA MSK Receiver Signal
 */
typedef struct {
#if NMEA_MESSAGE_MSS_BEACONFREQUENCY
    float                       BeaconFrequency;
#endif
#if NMEA_MESSAGE_MSS_BEACONBITRATE
    uint16_t                    BeaconBitRate;
#endif
#if NMEA_MESSAGE_MSS_SIGNALSTRENGTH
    uint8_t                     SignalStrength;
#endif
#if NMEA_MESSAGE_MSS_SNR
    uint8_t                     SNR;
#endif
#if NMEA_MESSAGE_MSS_CHANNELNUMBER
    uint8_t                     ChannelNumber;
#endif
} NMEA_Message_MSS;
#endif

#if NMEA_MESSAGE_RMC
/**
 * @brief NMEA Recommended Minimum Specific GNSS Data
 */
typedef struct {
#if NMEA_MESSAGE_RMC_LATITUDE
    NMEA_Coordinate             Latitude;
#endif
#if NMEA_MESSAGE_RMC_LONGITUDE
    NMEA_Coordinate             Longitude;
#endif
#if NMEA_MESSAGE_RMC_MAGNETICVARIATION
    NMEA_MagneticVariation      MagneticVariation;
#endif
#if NMEA_MESSAGE_RMC_SPEEDOVERGROUND
    float                       SpeedOverGround;
#endif
#if NMEA_MESSAGE_RMC_COURSEOVERGROUND
    float                       CourseOverGround;
#endif
#if NMEA_MESSAGE_RMC_TIME
    NMEA_Time                   Time;
#endif
#if NMEA_MESSAGE_RMC_DATE
    NMEA_Date                   Date;
#endif
#if NMEA_MESSAGE_RMC_STATUS
    uint8_t                     Status;
#endif
#if NMEA_MESSAGE_RMC_MODE
    uint8_t                     Mode;
#endif
} NMEA_Message_RMC;
#endif

#if NMEA_MESSAGE_VTG
/**
 * @brief NMEA Course Over Ground and Ground Speed
 */
typedef struct {
#if NMEA_MESSAGE_VTG_COURSE1
    float                       Course1;
#endif
#if NMEA_MESSAGE_VTG_COURSE2
    float                       Course2;
#endif
#if NMEA_MESSAGE_VTG_SPEED1
    float                       Speed1;
#endif
#if NMEA_MESSAGE_VTG_SPEED2
    float                       Speed2;
#endif
#if NMEA_MESSAGE_VTG_REFERENCE1
    uint8_t                     Reference1;
#endif
#if NMEA_MESSAGE_VTG_REFERENCE2
    uint8_t                     Reference2;
#endif
#if NMEA_MESSAGE_VTG_SPEEDUNITS1
    uint8_t                     SpeedUnits1;
#endif
#if NMEA_MESSAGE_VTG_SPEEDUNITS2
    uint8_t                     SpeedUnits2;
#endif
#if NMEA_MESSAGE_VTG_MODE
    uint8_t                     Mode;
#endif
} NMEA_Message_VTG;
#endif

#if NMEA_MESSAGE_ZDA
/**
 * @brief NMEA SiRF Timing Message
 */
typedef struct {
#if NMEA_MESSAGE_ZDA_TIME
    NMEA_Time                   Time;
#endif
#if NMEA_MESSAGE_ZDA_YEAR
    uint16_t                    Year;
#endif
#if NMEA_MESSAGE_ZDA_MONTH
    uint8_t                     Month;
#endif
#if NMEA_MESSAGE_ZDA_DAY
    uint8_t                     Day;
#endif
#if NMEA_MESSAGE_ZDA_LOCALZONEHOURS
    uint8_t                     LocalZoneHours;
#endif
#if NMEA_MESSAGE_ZDA_LOCALZONEMINUTES
    uint8_t                     LocalZoneMinutes;
#endif
} NMEA_Message_ZDA;
#endif

/**
 * @brief NMEA Message Holder
 */
typedef struct {
    // Supported Messages
    union {
    #if NMEA_MESSAGE_GGA    
        NMEA_Message_GGA        GGA;
    #endif
    #if NMEA_MESSAGE_GLL 
        NMEA_Message_GLL        GLL;
    #endif
    #if NMEA_MESSAGE_GSA
        NMEA_Message_GSA        GSA;
    #endif
    #if NMEA_MESSAGE_GSV 
        NMEA_Message_GSV        GSV;
    #endif
    #if NMEA_MESSAGE_MSS 
        NMEA_Message_MSS        MSS;
    #endif
    #if NMEA_MESSAGE_RMC 
        NMEA_Message_RMC        RMC;
    #endif
    #if NMEA_MESSAGE_VTG 
        NMEA_Message_VTG        VTG;
    #endif
    #if NMEA_MESSAGE_ZDA 
        NMEA_Message_ZDA        ZDA;
    #endif
    };
    // Message Information
    NMEA_Message_Header         Header;
#if NMEA_MESSAGE_CHECKSUM
    uint8_t                     Checksum;
#endif
} NMEA_Message;
/**
 * @brief NMEA Parse results
 */
typedef enum {
    NMEA_Result_Ok                                  = 0,
    NMEA_Result_MessageStartNotFound                = 1,
    NMEA_Result_MessageEndNotFound                  = 2,
    NMEA_Result_ChecksumNotFound                    = 3,
    NMEA_Result_ChecksumField                       = 4,
    NMEA_Result_MessageTypeNotSupported             = 5,
    NMEA_Result_PositionFixInvalid                  = 6,
    NMEA_Result_MessageStatusInvalid                = 7,
    NMEA_Result_ModeInvalid                         = 8,
    NMEA_Result_GSAModeInvalid                      = 9,
    NMEA_Result_FixStatusInvalid                    = 10,
    NMEA_Result_LatitudeFormatInvalid               = 11,
    NMEA_Result_LatitudeIndicatorInvalid            = 12,
    NMEA_Result_LongitudeFormatInvalid              = 13,
    NMEA_Result_LongitudeIndicatorInvalid           = 14,
    NMEA_Result_MagneticVariationFormatInvalid      = 15,
    NMEA_Result_MagneticVariationIndicatorInvalid   = 16,
    NMEA_Result_Custom                              = 0x80,
} NMEA_Result;
/* Pre-Defined Types */
struct __NMEA;
typedef struct __NMEA NMEA;
/**
 * @brief NMEA On receive message callback
 * 
 * @param nmea 
 * @param message 
 */
typedef void (*NMEA_OnMessageFn)(NMEA* nmea, NMEA_Message* message);
/**
 * @brief NMEA On Error callback
 */
typedef void (*NMEA_OnErrorFn)(NMEA* nmea, NMEA_Message* message, NMEA_Result result);
/**
 * @brief NMEA Callbacks structure
 */
typedef union {
#if NMEA_SUPPORT_MULTI_CALLBACK
    NMEA_OnMessageFn            fn[NMEA_MessageType_Reserved];
    struct {
    #if NMEA_MESSAGE_GGA
        NMEA_OnMessageFn        onGGA;
    #endif
    #if NMEA_MESSAGE_GLL
        NMEA_OnMessageFn        onGLL;
    #endif
    #if NMEA_MESSAGE_GSA
        NMEA_OnMessageFn        onGSA;
    #endif
    #if NMEA_MESSAGE_GSV
        NMEA_OnMessageFn        onGSV;
    #endif
    #if NMEA_MESSAGE_MSS
        NMEA_OnMessageFn        onMSS;
    #endif
    #if NMEA_MESSAGE_RMC
        NMEA_OnMessageFn        onRMC;
    #endif
    #if NMEA_MESSAGE_VTG
        NMEA_OnMessageFn        onVTG;
    #endif
    #if NMEA_MESSAGE_ZDA
        NMEA_OnMessageFn        onZDA;
    #endif
    };
#else
    NMEA_OnMessageFn            fn[1];
#endif
} NMEA_Callbacks;

/**
 * @brief NMEA Configuration
 */
struct __NMEA {
#if NMEA_SUPPORT_ARGS
    void*                       Args;
#endif
    NMEA_Callbacks              Callbacks;
    NMEA_OnErrorFn              onError;
};

/* APIs */
NMEA_Result NMEA_parseRaw(char* line, NMEA_Message* msg);

#if NMEA_SUPPORT_MODULAR

void NMEA_init(NMEA* nmea);
void NMEA_parse(NMEA* nmea, char* line);
void NMEA_onMessage(NMEA* nmea, NMEA_MessageType type, NMEA_OnMessageFn fn);

#if NMEA_SUPPORT_ISTREAM
    void NMEA_handle(NMEA* nmea, IStream* input);
#endif

#if NMEA_SUPPORT_ARGS
    void  NMEA_setArgs(NMEA* nmea, void* args);
    void* NMEA_getArgs(NMEA* nmea);
#endif

#ifdef __cplusplus
};
#endif

#endif // NMEA_SUPPORT_MODULAR

#endif // _NMEA_H_
