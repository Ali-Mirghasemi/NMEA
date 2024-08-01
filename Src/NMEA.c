#include "NMEA.h"

/* Private Typedef */
typedef NMEA_Result (*NMEA_ParseFn)(char* line, NMEA_Message* msg);
typedef NMEA_Result (*NMEA_ParseFieldFn)(char* line, void* val);

typedef struct {
    NMEA_ParseFieldFn   parse;
    uint8_t             Index;
    uint8_t             Offset;
} NMEA_Field;

typedef struct {
    const NMEA_Field*   Fields;
    uint8_t             Len;
} NMEA_FieldList;
/* Private Defines */
#define NMEA_TALKER_ID_LEN          2
#define NMEA_MESSAGE_ID_LEN         3
#define NMEA_MESSAGE_IDS_LEN        NMEA_MessageType_Reserved
/* Private Functions */
// Basic Field Parser
static NMEA_Result NMEA_Field_Char_parse(char* line, char* val);
static NMEA_Result NMEA_Field_UInt8_parse(char* line, uint8_t* val);
static NMEA_Result NMEA_Field_UInt16_parse(char* line, uint16_t* val);
static NMEA_Result NMEA_Field_UInt32_parse(char* line, uint32_t* val);
static NMEA_Result NMEA_Field_Float_parse(char* line, float* val);
static NMEA_Result NMEA_Field_Time_parse(char* line, NMEA_Time* time);
static NMEA_Result NMEA_Field_Date_parse(char* line, NMEA_Date* date);
// Advance Field Parser
static NMEA_Result NMEA_Field_PositionFix_parse(char* line, uint8_t* pos);
static NMEA_Result NMEA_Field_MessageStatus_parse(char* line, uint8_t* status);
static NMEA_Result NMEA_Field_Mode_parse(char* line, uint8_t* mode);
static NMEA_Result NMEA_Field_GSA_Mode_parse(char* line, uint8_t* mode);
static NMEA_Result NMEA_Field_FixStatus_parse(char* line, uint8_t* mode);
static NMEA_Result NMEA_Field_Latitude_parse(char* line, NMEA_Coordinate* cor);
static NMEA_Result NMEA_Field_Longitude_parse(char* line, NMEA_Coordinate* cor);
static NMEA_Result NMEA_Field_LatitudeIndicator_parse(char* line, NMEA_Coordinate* cor);
static NMEA_Result NMEA_Field_LongitudeIndicator_parse(char* line, NMEA_Coordinate* cor);
static NMEA_Result NMEA_Field_MagneticVariationIndicator_parse(char* line, NMEA_MagneticVariation* mag);
// Full Fields List Parser
static NMEA_Result NMEA_FieldList_parse(const NMEA_FieldList* f, char* line, NMEA_Message* msg);
/* Private Macros */
#define ARR_LEN(ARR)                            (sizeof(ARR)/sizeof(ARR[0]))
#define __offset(MSG, FIELD)                    ((uint8_t) (uint32_t) ((uint8_t*) &((const MSG*) 0)->FIELD - (uint8_t*) 0))
#define __field(MSG, IDX, TYPE, FIELD)          { .parse = (NMEA_ParseFieldFn) NMEA_Field_ ##TYPE ##_parse , .Index = IDX, .Offset = __offset(MSG, FIELD) }
#define __fieldList(MSG)                        { .Fields = MSG ##_PARSE_FIELD, .Len = NMEA_MESSAGE_ ##MSG ##_FIELDS_LENGTH }

#if NMEA_SUPPORT_MULTI_CALLBACK
    #define __onMessage(N, MSG)                 if ((N)->Callbacks.fn[(MSG).Header.Type]) { (N)->Callbacks.fn[(MSG).Header.Type]((N), &msg); }
#else
    #define __onMessage(N, MSG)                 if ((N)->Callbacks.fn[0]) { (N)->Callbacks.fn[0]((N), &msg); }
#endif
/* Private Static Const */
// GGA fields
#if NMEA_MESSAGE_GGA
static const NMEA_Field GGA_PARSE_FIELD[NMEA_MESSAGE_GGA_FIELDS_LENGTH] = {
#if NMEA_MESSAGE_GGA_TIME
    __field(NMEA_Message_GGA, 0, Time, Time),
#endif
#if NMEA_MESSAGE_GGA_LATITUDE
    __field(NMEA_Message_GGA, 1, Latitude, Latitude),
    __field(NMEA_Message_GGA, 2, LatitudeIndicator, Latitude),
#endif
#if NMEA_MESSAGE_GGA_LONGITUDE
    __field(NMEA_Message_GGA, 3, Longitude, Longitude),
    __field(NMEA_Message_GGA, 4, LongitudeIndicator, Longitude),
#endif
#if NMEA_MESSAGE_GGA_POSITIONFIX
    __field(NMEA_Message_GGA, 5, PositionFix, PositionFix),
#endif
#if NMEA_MESSAGE_GGA_SATELLITESUSED
    __field(NMEA_Message_GGA, 6, UInt8, SatellitesUsed),
#endif
#if NMEA_MESSAGE_GGA_HDOP
    __field(NMEA_Message_GGA, 7, Float, HDOP),
#endif
#if NMEA_MESSAGE_GGA_ALTITUDE
    __field(NMEA_Message_GGA, 8, Float, Altitude),
#endif
#if NMEA_MESSAGE_GGA_ALTITUDE_UNIT
    __field(NMEA_Message_GGA, 9, Char, AltitudeUnit),
#endif
#if NMEA_MESSAGE_GGA_GEOID
    __field(NMEA_Message_GGA, 10, Float, Geoid),
#endif
#if NMEA_MESSAGE_GGA_GEOIDUNITS
    __field(NMEA_Message_GGA, 11, Char, GeoidUnits),
#endif
#if NMEA_MESSAGE_GGA_AGEOFDIFF
    __field(NMEA_Message_GGA, 12, UInt8, AgeOfDiff),
#endif
#if NMEA_MESSAGE_GGA_DIFFREFSTATIONID
    __field(NMEA_Message_GGA, 13, UInt32, DiffRefStationId),
#endif
};
#endif
// GLL Fields
#if NMEA_MESSAGE_GLL
static const NMEA_Field GLL_PARSE_FIELD[NMEA_MESSAGE_GLL_FIELDS_LENGTH] = {
#if NMEA_MESSAGE_GLL_LATITUDE
    __field(NMEA_Message_GLL, 0, Latitude, Latitude),
    __field(NMEA_Message_GLL, 1, LatitudeIndicator, Latitude),
#endif
#if NMEA_MESSAGE_GLL_LONGITUDE
    __field(NMEA_Message_GLL, 2, Longitude, Longitude),
    __field(NMEA_Message_GLL, 3, LongitudeIndicator, Longitude),
#endif
#if NMEA_MESSAGE_GLL_TIME
    __field(NMEA_Message_GLL, 4, Time, Time),
#endif
#if NMEA_MESSAGE_GLL_STATUS
    __field(NMEA_Message_GLL, 5, Char, Status),
#endif
#if NMEA_MESSAGE_GLL_MODE
    __field(NMEA_Message_GLL, 6, Char, Mode),
#endif
};
#endif
// GSA Fields
#if NMEA_MESSAGE_GSA
static const NMEA_Field GSA_PARSE_FIELD[NMEA_MESSAGE_GSA_FIELDS_LENGTH] = {
#if NMEA_MESSAGE_GSA_MODE
    __field(NMEA_Message_GSA, 0, GSA_Mode, Mode),
#endif
#if NMEA_MESSAGE_GSA_FIX_STATUS
    __field(NMEA_Message_GSA, 1, FixStatus, FixStatus),
#endif
#if NMEA_MESSAGE_GSA_SATELLITESUSED
    __field(NMEA_Message_GSA, 2, UInt8, SatellitesUsed[0]),
    __field(NMEA_Message_GSA, 3, UInt8, SatellitesUsed[1]),
    __field(NMEA_Message_GSA, 4, UInt8, SatellitesUsed[2]),
    __field(NMEA_Message_GSA, 5, UInt8, SatellitesUsed[3]),
    __field(NMEA_Message_GSA, 6, UInt8, SatellitesUsed[4]),
    __field(NMEA_Message_GSA, 7, UInt8, SatellitesUsed[5]),
    __field(NMEA_Message_GSA, 8, UInt8, SatellitesUsed[6]),
    __field(NMEA_Message_GSA, 9, UInt8, SatellitesUsed[7]),
    __field(NMEA_Message_GSA, 10, UInt8, SatellitesUsed[8]),
    __field(NMEA_Message_GSA, 11, UInt8, SatellitesUsed[9]),
    __field(NMEA_Message_GSA, 12, UInt8, SatellitesUsed[10]),
    __field(NMEA_Message_GSA, 13, UInt8, SatellitesUsed[11]),
#endif
#if NMEA_MESSAGE_GSA_PDOP
    __field(NMEA_Message_GSA, 14, Float, PDOP),
#endif
#if NMEA_MESSAGE_GSA_HDOP
    __field(NMEA_Message_GSA, 15, Float, HDOP),
#endif
#if NMEA_MESSAGE_GSA_VDOP
    __field(NMEA_Message_GSA, 16, Float, VDOP),
#endif
};
#endif
// GSV Fields
#if NMEA_MESSAGE_GSV
static const NMEA_Field GSV_PARSE_FIELD[NMEA_MESSAGE_GSV_FIELDS_LENGTH] = {
#if NMEA_MESSAGE_GSV_TOTALMESSAGES
    __field(NMEA_Message_GSV, 0, UInt8, TotalMessages),
#endif
#if NMEA_MESSAGE_GSV_MESSAGENUMBER
    __field(NMEA_Message_GSV, 1, UInt8, MessageNumber),
#endif
#if NMEA_MESSAGE_GSV_SATELLITESINVIEW
    __field(NMEA_Message_GSV, 2, UInt8, SatellitesInView),
#endif
#if NMEA_MESSAGE_GSV_SATELLITES
    __field(NMEA_Message_GSV, 3,  UInt8,  Satellites[0].Id),
    __field(NMEA_Message_GSV, 4,  UInt8,  Satellites[0].Elevation),
    __field(NMEA_Message_GSV, 5,  UInt16, Satellites[0].Azimuth),
    __field(NMEA_Message_GSV, 6,  UInt8,  Satellites[0].SNR),
    __field(NMEA_Message_GSV, 7,  UInt8,  Satellites[1].Id),
    __field(NMEA_Message_GSV, 8,  UInt8,  Satellites[1].Elevation),
    __field(NMEA_Message_GSV, 9,  UInt16, Satellites[1].Azimuth),
    __field(NMEA_Message_GSV, 10, UInt8,  Satellites[1].SNR),
    __field(NMEA_Message_GSV, 11, UInt8,  Satellites[2].Id),
    __field(NMEA_Message_GSV, 12, UInt8,  Satellites[2].Elevation),
    __field(NMEA_Message_GSV, 13, UInt16, Satellites[2].Azimuth),
    __field(NMEA_Message_GSV, 14, UInt8,  Satellites[2].SNR),
    __field(NMEA_Message_GSV, 15, UInt8,  Satellites[3].Id),
    __field(NMEA_Message_GSV, 16, UInt8,  Satellites[3].Elevation),
    __field(NMEA_Message_GSV, 17, UInt16, Satellites[3].Azimuth),
    __field(NMEA_Message_GSV, 18, UInt8,  Satellites[3].SNR),
#endif
};
#endif
// MSS Fields
#if NMEA_MESSAGE_MSS
static const NMEA_Field MSS_PARSE_FIELD[NMEA_MESSAGE_MSS_FIELDS_LENGTH] = {
#if NMEA_MESSAGE_MSS_SIGNALSTRENGTH
    __field(NMEA_Message_MSS, 0, UInt8, SignalStrength),
#endif
#if NMEA_MESSAGE_MSS_SNR
    __field(NMEA_Message_MSS, 1, UInt8, SNR),
#endif
#if NMEA_MESSAGE_MSS_BEACONFREQUENCY
    __field(NMEA_Message_MSS, 2, Float, BeaconFrequency),
#endif
#if NMEA_MESSAGE_MSS_BEACONBITRATE
    __field(NMEA_Message_MSS, 3, UInt16, BeaconBitRate),
#endif
#if NMEA_MESSAGE_MSS_CHANNELNUMBER
    __field(NMEA_Message_MSS, 4, UInt8, ChannelNumber),
#endif
};
#endif
// RMC Fields
#if NMEA_MESSAGE_RMC
static const NMEA_Field RMC_PARSE_FIELD[NMEA_MESSAGE_RMC_FIELDS_LENGTH] = {
#if NMEA_MESSAGE_RMC_TIME
    __field(NMEA_Message_RMC, 0, Time, Time),
#endif
#if NMEA_MESSAGE_RMC_STATUS
    __field(NMEA_Message_RMC, 1, MessageStatus, Status),
#endif
#if NMEA_MESSAGE_RMC_LATITUDE
    __field(NMEA_Message_RMC, 2, Latitude, Latitude),
    __field(NMEA_Message_RMC, 3, LatitudeIndicator, Latitude),
#endif
#if NMEA_MESSAGE_RMC_LONGITUDE
    __field(NMEA_Message_RMC, 4, Longitude, Longitude),
    __field(NMEA_Message_RMC, 5, LongitudeIndicator, Longitude),
#endif
#if NMEA_MESSAGE_RMC_SPEEDOVERGROUND
    __field(NMEA_Message_RMC, 6, Float, SpeedOverGround),
#endif
#if NMEA_MESSAGE_RMC_COURSEOVERGROUND
    __field(NMEA_Message_RMC, 7, Float, CourseOverGround),
#endif
#if NMEA_MESSAGE_RMC_DATE
    __field(NMEA_Message_RMC, 8, Date, Date),
#endif
#if NMEA_MESSAGE_RMC_MAGNETICVARIATION
    __field(NMEA_Message_RMC, 9, Float, MagneticVariation.Degrees),
    __field(NMEA_Message_RMC, 10, MagneticVariationIndicator, MagneticVariation),
#endif
#if NMEA_MESSAGE_RMC_MODE
    __field(NMEA_Message_RMC, 11, Mode, Mode),
#endif
};
#endif
// VTG Fields
#if NMEA_MESSAGE_VTG
static const NMEA_Field VTG_PARSE_FIELD[NMEA_MESSAGE_VTG_FIELDS_LENGTH] = {
#if NMEA_MESSAGE_VTG_COURSE1
    __field(NMEA_Message_VTG, 0, Float, Course1),
#endif
#if NMEA_MESSAGE_VTG_REFERENCE1
    __field(NMEA_Message_VTG, 1, Char, Reference1),
#endif
#if NMEA_MESSAGE_VTG_COURSE2
    __field(NMEA_Message_VTG, 2, Float, Course2),
#endif
#if NMEA_MESSAGE_VTG_REFERENCE2
    __field(NMEA_Message_VTG, 3, Char, Reference2),
#endif
#if NMEA_MESSAGE_VTG_SPEED1
    __field(NMEA_Message_VTG, 4, Float, Speed1),
#endif
#if NMEA_MESSAGE_VTG_SPEEDUNITS1
    __field(NMEA_Message_VTG, 5, Char, SpeedUnits1),
#endif
#if NMEA_MESSAGE_VTG_SPEED2
    __field(NMEA_Message_VTG, 6, Float, Speed2),
#endif
#if NMEA_MESSAGE_VTG_SPEEDUNITS2
    __field(NMEA_Message_VTG, 7, Char, SpeedUnits2),
#endif
#if NMEA_MESSAGE_VTG_MODE
    __field(NMEA_Message_VTG, 8, Mode, Mode),
#endif
};
#endif
// ZDA Fields
#if NMEA_MESSAGE_ZDA
static const NMEA_Field ZDA_PARSE_FIELD[NMEA_MESSAGE_ZDA_FIELDS_LENGTH] = {
#if NMEA_MESSAGE_ZDA_TIME
    __field(NMEA_Message_ZDA, 0, Time, Time),
#endif
#if NMEA_MESSAGE_ZDA_DAY
    __field(NMEA_Message_ZDA, 1, UInt8, Day),
#endif
#if NMEA_MESSAGE_ZDA_MONTH
    __field(NMEA_Message_ZDA, 2, UInt8, Month),
#endif
#if NMEA_MESSAGE_ZDA_YEAR
    __field(NMEA_Message_ZDA, 3, UInt16, Year),
#endif
#if NMEA_MESSAGE_ZDA_LOCALZONEHOURS
    __field(NMEA_Message_ZDA, 4, UInt8, LocalZoneHours),
#endif
#if NMEA_MESSAGE_ZDA_LOCALZONEMINUTES
    __field(NMEA_Message_ZDA, 5, UInt8, LocalZoneMinutes),
#endif
};
#endif
// List of supported messages parser
static const NMEA_FieldList NMEA_FIELDS[] = {
#if NMEA_MESSAGE_GGA
    __fieldList(GGA),
#endif
#if NMEA_MESSAGE_GLL
    __fieldList(GLL),
#endif
#if NMEA_MESSAGE_GSA
    __fieldList(GSA),
#endif
#if NMEA_MESSAGE_GSV
    __fieldList(GSV),
#endif
#if NMEA_MESSAGE_MSS
    __fieldList(MSS),
#endif
#if NMEA_MESSAGE_RMC
    __fieldList(RMC),
#endif
#if NMEA_MESSAGE_VTG
    __fieldList(VTG),
#endif
#if NMEA_MESSAGE_ZDA
    __fieldList(ZDA),
#endif
};
// List of supported messages
static const char NMEA_MESSAGE_IDS[NMEA_MESSAGE_IDS_LEN][NMEA_MESSAGE_ID_LEN] = {
#if NMEA_MESSAGE_GGA
    "GGA",
#endif
#if NMEA_MESSAGE_GLL
    "GLL",
#endif
#if NMEA_MESSAGE_GSA
    "GSA",
#endif
#if NMEA_MESSAGE_GSV
    "GSV",
#endif
#if NMEA_MESSAGE_MSS
    "MSS",
#endif
#if NMEA_MESSAGE_RMC
    "RMC",
#endif
#if NMEA_MESSAGE_VTG
    "VTG",
#endif
#if NMEA_MESSAGE_ZDA
    "ZDA",
#endif
};
// List of Supported Talker ids
static const char NMEA_TALKER_IDS[][NMEA_TALKER_ID_LEN] = {
    "BD",
    "GA",
    "GB",
    "GL",
    "GN",
    "GP",
};
static const uint8_t NMEA_TALKER_IDS_LEN = sizeof(NMEA_TALKER_IDS) / sizeof(NMEA_TALKER_IDS[0]);

static const char NMEA_START[1] = "$";
static const char NMEA_END[2]   = "\r\n";
/**
 * @brief Raw parse NMEA messages
 *
 * @param line
 * @param msg
 */
NMEA_Result NMEA_parseRaw(char* line, NMEA_Message* msg) {
    char* end;
    Str_UNum num;
    // Find '$' for message start
    line = Str_indexOf(line, NMEA_START[0]);
    if (line == NULL) {
        return NMEA_Result_MessageStartNotFound;
    }
    line++;
    // Find <CR><LF>
    end = Str_indexOf(line, NMEA_END[0]);
    if (end == NULL || end[1] != NMEA_END[1]) {
        return NMEA_Result_MessageEndNotFound;
    }
    *end = '\0';
#if NMEA_SUPPORT_DEINIT_MESSAGE
    // DeInit Message
    Mem_set(msg, 0, sizeof(NMEA_Message));
#endif
    // Find Talker ID
    msg->Header.Id = (uint8_t) Mem_binarySearch(NMEA_TALKER_IDS, NMEA_TALKER_IDS_LEN, NMEA_TALKER_ID_LEN, line, (Mem_CompareFn) Mem_compare);
    line += NMEA_TALKER_ID_LEN;
    // Find Message Type
    msg->Header.Type = (uint8_t) Mem_binarySearch(NMEA_MESSAGE_IDS, NMEA_MESSAGE_IDS_LEN, NMEA_MESSAGE_ID_LEN, line, (Mem_CompareFn) Mem_compare);
    line += NMEA_MESSAGE_ID_LEN;
    // Find Checksum
    end = Str_indexOf(line, '*');
    if (end == NULL) {
        return NMEA_Result_ChecksumNotFound;
    }
    *end = '\0';
#if NMEA_MESSAGE_CHECKSUM
    if (Str_convertUNumHexFix(end + 1,  &num, 2) != Str_Ok) {
        return NMEA_Result_ChecksumField;
    }
    msg->Checksum = (uint8_t) num;
#endif
    // Remove ','
    line++;
    if (msg->Header.Type != NMEA_MessageType_Unknown) {
        // Run Message Parser
        return NMEA_FieldList_parse(&NMEA_FIELDS[msg->Header.Type], line, msg);
    }
    else {
        return NMEA_Result_MessageTypeNotSupported;
    }
}
/**
 * @brief Convert Latitude to float
 * 
 * @param coord 
 * @return float 
 */
float NMEA_convertLatitude(NMEA_Coordinate* coord) {
    return ((float) coord->Degrees + (coord->Minutes / 60.0f)) * (coord->Indicator == NMEA_Indicator_South? -1 : 1);
}
/**
 * @brief Convert Longitude to float
 * 
 * @param coord 
 * @return float 
 */
float NMEA_convertLongitude(NMEA_Coordinate* coord) {
    return ((float) coord->Degrees + (coord->Minutes / 60.0f)) * (coord->Indicator == NMEA_Indicator_West ? -1 : 1);
}
/**
 * @brief Convert NMEA Coordinate to Full Coordinate
 * 
 * @param lat 
 * @param lon 
 * @param coord 
 */
void NMEA_convert(NMEA_Coordinate* lat, NMEA_Coordinate* lon, float altitude, NMEA_CoordinateFull* coord) {
    coord->Latitude = NMEA_convertLatitude(lat);
    coord->Longitude = NMEA_convertLongitude(lon);
    coord->Altitude = altitude;
}
#if NMEA_SUPPORT_MODULAR
/**
 * @brief Initialize NMEA structure with default values
 *
 * @param nmea
 */
void NMEA_init(NMEA* nmea) {
    Mem_set(nmea, 0, sizeof(NMEA));
}
/**
 * @brief NMEA parse line and call callbacks
 *
 * @param nmea
 * @param line
 */
void NMEA_parse(NMEA* nmea, char* line) {
    NMEA_Message msg;
    NMEA_Result result;

    if ((result = NMEA_parseRaw(line, &msg)) != NMEA_Result_Ok) {
        if (nmea->onError) {
            nmea->onError(nmea, &msg, result);
        }
    }
    else {
        __onMessage(nmea, msg);
    }
}
/**
 * @brief Set callback for message
 *
 * @param nmea
 * @param type
 * @param fn
 */
void NMEA_onMessage(NMEA* nmea, NMEA_MessageType type, NMEA_OnMessageFn fn) {
#if NMEA_SUPPORT_MULTI_CALLBACK
    nmea->Callbacks.fn[type] = fn;
#else
    nmea->Callbacks.fn[0] = fn;
#endif
}
/**
 * @brief Set callback for error
 * 
 * @param nmea 
 * @param fn 
 */
void NMEA_onError(NMEA* nmea, NMEA_OnErrorFn fn) {
    nmea->onError = fn;
}

#if NMEA_SUPPORT_ISTREAM
/**
 * @brief Handle NMEA messages from input stream
 *
 * @param nmea
 * @param input
 */
void NMEA_handle(NMEA* nmea, IStream* input) {
    if (IStream_available(input) > 0) {
        char line[NMEA_MAX_BUFF_SIZE];
        Stream_LenType start;
        Stream_LenType len;
        // Find Start if message
        while ((start = IStream_findByte(input, (uint8_t) NMEA_START[0])) >= 0) {
            // Trim start
            if (start > 0) {
                IStream_ignore(input, start);
            }
            // Read until end
            if ((len = IStream_readBytesUntilPattern(input, (uint8_t*) NMEA_END, sizeof(NMEA_END), (uint8_t*) line, sizeof(line))) == 0) {
                // No end found
                break;
            }
            // Parse line
            line[len] = '\0';
            NMEA_parse(nmea, line);
        }
    }
}
#endif

#if NMEA_SUPPORT_ARGS
/**
 * @brief Set args to NMEA structure
 *
 * @param nmea
 * @param args
 */
void  NMEA_setArgs(NMEA* nmea, void* args) {
    nmea->Args = args;
}
/**
 * @brief Get args from NMEA structure
 *
 * @param nmea
 * @return void*
 */
void* NMEA_getArgs(NMEA* nmea) {
    return nmea->Args;
}
#endif // NMEA_SUPPORT_ARGS

#endif // NMEA_SUPPORT_MODULAR

#if NMEA_SUPPORT_BEARING
#include <math.h>
/**
 * @brief Calculate bearing between two coordinates
 *
 * @param a
 * @param b
 * @param result
 * @return float return the distance in meters
 */
float NMEA_calculateBearing(const NMEA_CoordinateFull* a, const NMEA_CoordinateFull* b, NMEA_Bearing* result) {
    #define M_PI   3.14159265358979323846f
    // Calculate the difference in latitude and longitude
    const float earthRadius = 6371000; // Radius of the Earth in meters
    float latA = a->Latitude * M_PI / 180;
    float latB = b->Latitude * M_PI / 180;
    float lonA = a->Longitude * M_PI / 180;
    float lonB = b->Longitude * M_PI / 180;
    float deltaLat = b->Latitude - a->Latitude;
    float deltaLon = b->Longitude - a->Longitude;
    float aLatRad = a->Latitude * M_PI / 180;
    float bLatRad = b->Latitude * M_PI / 180;
    float deltaLatRad = deltaLat * M_PI / 180;
    float deltaLonRad = deltaLon * M_PI / 180;

    // Calculate the bearing using the Haversine formula
    float y = sinf(deltaLonRad) * cosf(bLatRad);
    float x = cosf(aLatRad) * sinf(bLatRad) - sinf(aLatRad) * cosf(bLatRad) * cosf(deltaLonRad);
    float bearing = atan2(y, x);

    // Convert the bearing to degrees
    bearing = bearing * 180 / M_PI;
    if (bearing < 0.0f) {
        bearing = bearing + 360.0f;
    }
    if (bearing > 360.0f) {
        bearing = bearing - 360.0f;
    }

    // Calculate the distance using the Haversine formula
    float haversineA = sinf(deltaLatRad / 2) * sinf(deltaLatRad / 2) + cosf(aLatRad) * cosf(bLatRad) * sinf(deltaLonRad / 2) * sinf(deltaLonRad / 2);
    float haversineC = 2 * atan2(sqrt(haversineA), sqrt(1 - haversineA));
    float distance = earthRadius * haversineC;

    // Calculate the elevation using the altitude difference
    float altitudeDiff = b->Altitude - a->Altitude;
    float elevationRad = atan2(altitudeDiff, distance);
    float elevationDeg = elevationRad * (180 / M_PI);

    // Set the result values
    if (result) {
        result->Azimuth = bearing;
        result->Elevation = elevationDeg;
        result->Distance = distance;
    }

    // Return the distance in meters
    return distance;
}
#endif

// ---------------------------------- Private APIs ------------------------------
// Basic Field Parser
static NMEA_Result NMEA_Field_Char_parse(char* line, char* val) {
    *val = *line;
    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_UInt8_parse(char* line, uint8_t* val) {
    Str_UNum num = 0;
    Str_Result result;
    if ((result = Str_convertUNumDecimal(line, &num)) != Str_Ok) {
        result |= NMEA_Result_Custom;
    }
    *val = (uint8_t) num;

    return (NMEA_Result) result;
}
static NMEA_Result NMEA_Field_UInt16_parse(char* line, uint16_t* val) {
    Str_UNum num = 0;
    Str_Result result;
    if ((result = Str_convertUNumDecimal(line, &num)) != Str_Ok) {
        result |= NMEA_Result_Custom;
    }
    *val = (uint16_t) num;

    return (NMEA_Result) result;
}
static NMEA_Result NMEA_Field_UInt32_parse(char* line, uint32_t* val) {
    Str_UNum num = 0;
    Str_Result result;
    if ((result = Str_convertUNumDecimal(line, &num)) != Str_Ok) {
        result |= NMEA_Result_Custom;
    }
    *val = (uint32_t) num;

    return (NMEA_Result) result;
}
static NMEA_Result NMEA_Field_Float_parse(char* line, float* val) {
    Str_Result result;
    if ((result = Str_convertFloat(line, val)) != Str_Ok) {
        result |= NMEA_Result_Custom;
    }

    return (NMEA_Result) result;
}
static NMEA_Result NMEA_Field_Time_parse(char* line, NMEA_Time* time) {
    Str_UNum num = 0;
    Str_Result result;
    // Hour
    if ((result = Str_convertUNumDecimalFix(&line[0], &num, 2)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }
    time->Hours = (uint8_t) num;
    // Minutes
    if ((result = Str_convertUNumDecimalFix(&line[2], &num, 2)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }
    time->Minutes = (uint8_t) num;
    // Seconds
    if ((result = Str_convertUNumDecimalFix(&line[4], &num, 2)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }
    time->Seconds = (uint8_t) num;
    // Check there is millis
    if (line[6] == '.') {
        // Milliseconds
        if ((result = Str_convertUNumDecimal(&line[7], &num)) != Str_Ok) {
            return (NMEA_Result) (result | NMEA_Result_Custom);
        }
        time->Milliseconds = (uint16_t) num;
    }
    else {
        time->Milliseconds = 0;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_Date_parse(char* line, NMEA_Date* date) {
    Str_UNum num = 0;
    Str_Result result;
    // Day
    if ((result = Str_convertUNumDecimalFix(&line[0], &num, 2)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }
    date->Day = (uint8_t) num;
    // Month
    if ((result = Str_convertUNumDecimalFix(&line[2], &num, 2)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }
    date->Month = (uint8_t) num;
    // Year
    if ((result = Str_convertUNumDecimalFix(&line[4], &num, 2)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }
    date->Year = (uint16_t) num;

    return NMEA_Result_Ok;
}
// Advance Field Parser
static NMEA_Result NMEA_Field_PositionFix_parse(char* line, uint8_t* pos) {
    NMEA_Result result;
    if ((result = NMEA_Field_UInt8_parse(line, pos)) != NMEA_Result_Ok) {
        return result;
    }
    // Verify Value
    if (*pos != NMEA_PositionFix_NotValid &&
        *pos != NMEA_PositionFix_GPS &&
        *pos != NMEA_PositionFix_DifferentialGPS &&
        *pos != NMEA_PositionFix_DeadReckoning
    ) {
        return NMEA_Result_PositionFixInvalid;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_MessageStatus_parse(char* line, uint8_t* status) {
    *status = (uint8_t) *line;
    // Verify Value
    if (*status != '\0' &&
        *status != NMEA_MessageStatus_NotValid &&
        *status != NMEA_MessageStatus_Valid
    ) {
        return NMEA_Result_MessageStatusInvalid;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_Mode_parse(char* line, uint8_t* mode) {
    *mode = (uint8_t) *line;
    // Verify Value
    if (*mode != '\0' &&
        *mode != NMEA_Mode_Autonomous &&
        *mode != NMEA_Mode_DGPS &&
        *mode != NMEA_Mode_DeadReckoning
    ) {
        return NMEA_Result_ModeInvalid;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_GSA_Mode_parse(char* line, uint8_t* mode) {
    *mode = (uint8_t) *line;
    // Verify Value
    if (*mode != '\0' &&
        *mode != NMEA_GSA_Mode_Auto &&
        *mode != NMEA_GSA_Mode_Manual
    ) {
        return NMEA_Result_GSAModeInvalid;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_FixStatus_parse(char* line, uint8_t* mode) {
    NMEA_Result result;
    if ((result = NMEA_Field_UInt8_parse(line, mode)) != NMEA_Result_Ok) {
        return result;
    }
    // Verify Value
    if (*mode != NMEA_FixStatus_NotFix &&
        *mode != NMEA_FixStatus_2D &&
        *mode != NMEA_FixStatus_3D
    ) {
        return NMEA_Result_FixStatusInvalid;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_Latitude_parse(char* line, NMEA_Coordinate* cor) {
    Str_UNum num;
    Str_Result result;

    // Degrees
    if ((result = Str_convertUNumDecimalFix(line, &num, 2)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }
    cor->Degrees = (uint8_t) num;
    // Minutes
    if ((result = Str_convertFloat(&line[2], &cor->Minutes)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_Longitude_parse(char* line, NMEA_Coordinate* cor) {
    Str_UNum num;
    Str_Result result;

    // Degrees
    if ((result = Str_convertUNumDecimalFix(line, &num, 3)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }
    cor->Degrees = (uint8_t) num;
    // Minutes
    if ((result = Str_convertFloat(&line[3], &cor->Minutes)) != Str_Ok) {
        return (NMEA_Result) (result | NMEA_Result_Custom);
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_LatitudeIndicator_parse(char* line, NMEA_Coordinate* cor) {
    cor->Indicator = (NMEA_Indicator) *line;
    // Verify Indicator
    if (cor->Indicator != '\0' &&
        cor->Indicator != NMEA_Indicator_North &&
        cor->Indicator != NMEA_Indicator_South
    ) {
        return NMEA_Result_LatitudeIndicatorInvalid;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_LongitudeIndicator_parse(char* line, NMEA_Coordinate* cor) {
    cor->Indicator = (NMEA_Indicator) *line;
    // Verify Indicator
    if (cor->Indicator != '\0' &&
        cor->Indicator != NMEA_Indicator_East &&
        cor->Indicator != NMEA_Indicator_West
    ) {
        return NMEA_Result_LongitudeIndicatorInvalid;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_Field_MagneticVariationIndicator_parse(char* line, NMEA_MagneticVariation* mag) {
    mag->Indicator = (NMEA_Indicator) *line;
    // Verify Indicator
    if (mag->Indicator != '\0' &&
        mag->Indicator != NMEA_Indicator_East &&
        mag->Indicator != NMEA_Indicator_West
    ) {
        return NMEA_Result_MagneticVariationIndicatorInvalid;
    }

    return NMEA_Result_Ok;
}
static NMEA_Result NMEA_FieldList_parse(const NMEA_FieldList* f, char* line, NMEA_Message* msg) {
    NMEA_Result result;
    const NMEA_Field* field = f->Fields;
    uint8_t len = f->Len;
    uint8_t idx = 0;
    char* end;
    // Parse fields
    while ((end = Str_indexOf(line, ',')) != NULL && len > 0) {
        *end = '\0';
        if (line != '\0' && field->Index == idx) {
            if ((result = field->parse(line, (void*) ((uint8_t*) msg + field->Offset))) != NMEA_Result_Ok) {
                return result;
            }
            field++;
            len--;
        }
        line = end + 1;
        idx++;
    }
    // Parse last
    if (line != '\0' && field->Index == idx) {
        if ((result = field->parse(line, (void*) ((uint8_t*) msg + field->Offset))) != NMEA_Result_Ok) {
            return result;
        }
    }

    return result;
}

