#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "NMEA.h"

#define PRINTLN						puts
#define PRINTF						printf

uint32_t Assert_Result(NMEA_Result res, uint32_t line);
uint32_t Assert_Message(NMEA_Message* msg1, NMEA_Message* msg2, uint32_t line);
uint32_t Assert_Message_GGA(NMEA_Message_GGA* msg1, NMEA_Message_GGA* msg2, uint32_t line);
uint32_t Assert_Message_GLL(NMEA_Message_GLL* msg1, NMEA_Message_GLL* msg2, uint32_t line);
uint32_t Assert_Message_GSA(NMEA_Message_GSA* msg1, NMEA_Message_GSA* msg2, uint32_t line);
uint32_t Assert_Message_GSV(NMEA_Message_GSV* msg1, NMEA_Message_GSV* msg2, uint32_t line);
uint32_t Assert_Message_RMC(NMEA_Message_RMC* msg1, NMEA_Message_RMC* msg2, uint32_t line);
uint32_t Assert_Message_VTG(NMEA_Message_VTG* msg1, NMEA_Message_VTG* msg2, uint32_t line);
uint32_t Assert_Message_ZDA(NMEA_Message_ZDA* msg1, NMEA_Message_ZDA* msg2, uint32_t line);

uint32_t Assert_Str(const char* str1, const char* str2, uint32_t line);
uint32_t Assert_Word(const char* str, const char* word, uint32_t line);
uint32_t Assert_Num(int num1, int num2, uint32_t line);
uint32_t Assert_Float(float num1, float num2, uint32_t line);
uint32_t Assert_Strs(const char** strs1, const char** strs2, int len, uint32_t line);
uint32_t Assert_Ptr(const void* ptr1, const void* ptr2, uint32_t line);

uint32_t assertResult = 0;
#define assert(TYPE, ...)			if((assertResult = Assert_ ##TYPE (__VA_ARGS__, __LINE__)) != Str_Ok) return assertResult

#define ARRAY_LEN(ARR)			(sizeof(ARR) / sizeof(ARR[0]))

typedef uint32_t (*TestFunc)(void);

uint32_t Test_GGA(void);
uint32_t Test_GLL(void);
uint32_t Test_GSA(void);
uint32_t Test_GSV(void);
uint32_t Test_RMC(void);
uint32_t Test_VTG(void);
uint32_t Test_ZDA(void);

static const TestFunc TESTS[] = {
#if NMEA_MESSAGE_GGA
    Test_GGA,
#endif
#if NMEA_MESSAGE_GLL
    Test_GLL,
#endif
#if NMEA_MESSAGE_GSA
    Test_GSA,
#endif
#if NMEA_MESSAGE_GSV
    Test_GSV,
#endif
#if NMEA_MESSAGE_RMC
    Test_RMC,
#endif
#if NMEA_MESSAGE_VTG
    Test_VTG,
#endif
#if NMEA_MESSAGE_ZDA
    Test_ZDA
#endif
};
static const uint16_t TESTS_LEN = ARRAY_LEN(TESTS);

int main()
{
    int32_t testIndex;
	int32_t countTestError = 0;
	NMEA_Result res;

	for (testIndex = 0; testIndex < TESTS_LEN; testIndex++) {
		PRINTF("---------------- Begining Test[%d]------------ \r\n", testIndex);
		res = TESTS[testIndex]();
		PRINTF("Test Result: %s\r\n", res & 0x01 ? "Error" : "Ok");
		if (res & 1) {
			PRINTF("Error in Line: %d\r\n", res >> 1);
		}
		countTestError += (int) res & 1;
		PRINTF("---------------------------------------------- \r\n");
	}
	PRINTLN("Test Done\r\n");
	PRINTF("Tests Errors: %d\r\n", countTestError);
}
// -------------------------------------------------------------------------------
#if NMEA_MESSAGE_GGA
uint32_t Test_GGA(void) {
    NMEA_Message msg;
    char temp[128];

    const NMEA_Message MSG1 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_GGA,
        },
        .Checksum = 0x4E,
        .GGA = {
            .Time = {
                .Hours = 7,
                .Minutes = 14,
                .Seconds = 57,
                .Milliseconds = 0,
            },
            .Latitude = {
                .Degrees = 35,
                .Minutes = 41.68513,
                .Indicator = NMEA_Indicator_North,
            },
            .Longitude = {
                .Degrees = 51,
                .Minutes = 23.50382,
                .Indicator = NMEA_Indicator_East,
            },
            .PositionFix = NMEA_PositionFix_GPS,
            .SatellitesUsed = 11,
            .HDOP = 0.85,
            .Altitude = 1236.4,
        },
    };
    Str_copy(temp, "$GPGGA,071457.00,3541.68513,N,05123.50382,E,2,11,0.85,1236.4,M,-17.6,M,,0000*4E\r\n");
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG1);

    return 0;
}
#endif
#if NMEA_MESSAGE_GLL
uint32_t Test_GLL(void) {

    return 0;
}
#endif
#if NMEA_MESSAGE_GSA
uint32_t Test_GSA(void) {

    return 0;
}
#endif
#if NMEA_MESSAGE_GSV
uint32_t Test_GSV(void) {

    return 0;
}
#endif
#if NMEA_MESSAGE_RMC
uint32_t Test_RMC(void) {

    return 0;
}
#endif
#if NMEA_MESSAGE_VTG
uint32_t Test_VTG(void) {

    return 0;
}
#endif
#if NMEA_MESSAGE_ZDA
uint32_t Test_ZDA(void) {

    return 0;
}
#endif
// -------------------------------------------------------------------------------
uint32_t Assert_Result(NMEA_Result res, uint32_t line) {
    static const char* RESULTS[] = {
        "Ok",
        "MessageStartNotFound",
        "MessageEndNotFound",
        "ChecksumNotFound",
        "ChecksumField",
        "MessageTypeNotSupported",
        "PositionFixInvalid",
        "MessageStatusInvalid",
        "ModeInvalid",
        "GSAModeInvalid",
        "FixStatusInvalid",
        "LatitudeFormatInvalid",
        "LatitudeIndicatorInvalid",
        "LongitudeFormatInvalid",
        "LongitudeIndicatorInvalid",
        "MagneticVariationFormatInvalid",
        "MagneticVariationIndicatorInvalid",
    };
    
    if (res != NMEA_Result_Ok) {
        if (res & NMEA_Result_Custom) {
            PRINTF("Custom(%d)\r\n", res & 0x7F);
        }
        else {
            PRINTF("%s(%d)\r\n", RESULTS[res & 0x7F], res & 0x7F);
        }
        return Str_Error | line << 1;
    }
    return (uint32_t) Str_Ok;

}
uint32_t Assert_Message(NMEA_Message* msg1, NMEA_Message* msg2, uint32_t line) {
    if (Mem_compare(&msg1->Header, &msg2->Header, sizeof(msg2->Header)) != 0) {
        PRINTF("Header Error\r\n");
        return Str_Error | line << 1;
    }

    if (msg1->Checksum != msg2->Checksum) {
        PRINTF("Checksum Error\r\n");
        return Str_Error | line << 1;
    }

    switch (msg1->Header.Type) {
    #if NMEA_MESSAGE_GGA 
        case NMEA_MessageType_GGA:
            assert(Message_GGA, &msg1->GGA, &msg2->GGA);
            break;
    #endif
    #if NMEA_MESSAGE_GLL
        case NMEA_MessageType_GLL:
            if (Mem_compare(&msg1->GLL, &msg2->GLL, sizeof(msg2->GLL)) != 0) {
                PRINTF("GLL Error\r\n");
                return Str_Error | line << 1;
            }
            break;
    #endif
    #if NMEA_MESSAGE_GSA
        case NMEA_MessageType_GSA:
            if (Mem_compare(&msg1->GSA, &msg2->GSA, sizeof(msg2->GSA)) != 0) {
                PRINTF("GSA Error\r\n");
                return Str_Error | line << 1;
            }
            break;
    #endif
    #if NMEA_MESSAGE_GSV
        case NMEA_MessageType_GSV:
            if (Mem_compare(&msg1->GSV, &msg2->GSV, sizeof(msg2->GSV)) != 0) {
                PRINTF("GSV Error\r\n");
                return Str_Error | line << 1;
            }
            break;
    #endif
    #if NMEA_MESSAGE_RMC
        case NMEA_MessageType_RMC:
            if (Mem_compare(&msg1->RMC, &msg2->RMC, sizeof(msg2->RMC)) != 0) {
                PRINTF("RMC Error\r\n");
                return Str_Error | line << 1;
            }
            break;  
    #endif
    #if NMEA_MESSAGE_VTG
        case NMEA_MessageType_VTG:
            if (Mem_compare(&msg1->VTG, &msg2->VTG, sizeof(msg2->VTG)) != 0) {
                PRINTF("VTG Error\r\n");
                return Str_Error | line << 1;
            }
            break;
    #endif
    #if NMEA_MESSAGE_ZDA
        case NMEA_MessageType_ZDA:
            if (Mem_compare(&msg1->ZDA, &msg2->ZDA, sizeof(msg2->ZDA)) != 0) {
                PRINTF("ZDA Error\r\n");
                return Str_Error | line << 1;
            }
            break;
    #endif
    }

    return (uint32_t) Str_Ok;
}
uint32_t Assert_Message_GGA(NMEA_Message_GGA* msg1, NMEA_Message_GGA* msg2, uint32_t line) {
#if NMEA_MESSAGE_GGA_TIME
    assert(Num, msg1->Time.Hours, msg2->Time.Hours);
    assert(Num, msg1->Time.Minutes, msg2->Time.Minutes);
    assert(Num, msg1->Time.Seconds, msg2->Time.Seconds);
    assert(Num, msg1->Time.Milliseconds, msg2->Time.Milliseconds);
#endif
#if NMEA_MESSAGE_GGA_LATITUDE
    assert(Num, msg1->Latitude.Degrees, msg2->Latitude.Degrees);
    assert(Float, msg1->Latitude.Minutes, msg2->Latitude.Minutes);
    assert(Word, msg1->Latitude.Indicator, msg2->Latitude.Indicator);
#endif
#if NMEA_MESSAGE_GGA_LONGITUDE
    assert(Float, msg1->Longitude.Degrees, msg2->Longitude.Degrees);
    assert(Float, msg1->Longitude.Minutes, msg2->Longitude.Minutes);
    assert(Word, msg1->Longitude.Indicator, msg2->Longitude.Indicator);
#endif
#if NMEA_MESSAGE_GGA_POSITIONFIX
    assert(Num, msg1->PositionFix, msg2->PositionFix);
#endif
#if NMEA_MESSAGE_GGA_SATELLITESUSED
    assert(Num, msg1->SatellitesUsed, msg2->SatellitesUsed);
#endif
#if NMEA_MESSAGE_GGA_HDOP
    assert(Float, msg1->HDOP, msg2->HDOP);
#endif
#if NMEA_MESSAGE_GGA_ALTITUDE
    assert(Float, msg1->Altitude, msg2->Altitude);
#endif
#if NMEA_MESSAGE_GGA_ALTITUDEUNITS
    assert(Word, msg1->AltitudeUnit, msg2->AltitudeUnit);
#endif
#if NMEA_MESSAGE_GGA_GEOID
    assert(Float, msg1->Geoid, msg2->Geoid);
#endif
#if NMEA_MESSAGE_GGA_GEOIDUNITS
    assert(Word, msg1->GeoidUnits, msg2->GeoidUnits);
#endif
#if NMEA_MESSAGE_GGA_DIFFREFSTATIONID
    assert(Num, msg1->DiffRefStationId, msg2->DiffRefStationId);
#endif
#if NMEA_MESSAGE_GGA_AGEOFDIFF
    assert(Num, msg1->AgeOfDiff, msg2->AgeOfDiff);
#endif
    return 0;

}
uint32_t Assert_Message_GLL(NMEA_Message_GLL* msg1, NMEA_Message_GLL* msg2, uint32_t line);
uint32_t Assert_Message_GSA(NMEA_Message_GSA* msg1, NMEA_Message_GSA* msg2, uint32_t line);
uint32_t Assert_Message_GSV(NMEA_Message_GSV* msg1, NMEA_Message_GSV* msg2, uint32_t line);
uint32_t Assert_Message_RMC(NMEA_Message_RMC* msg1, NMEA_Message_RMC* msg2, uint32_t line);
uint32_t Assert_Message_VTG(NMEA_Message_VTG* msg1, NMEA_Message_VTG* msg2, uint32_t line);
uint32_t Assert_Message_ZDA(NMEA_Message_ZDA* msg1, NMEA_Message_ZDA* msg2, uint32_t line);
uint32_t Assert_Str(const char* str1, const char* str2, uint32_t line) {
	if (Str_compare(str1, str2)) {
		PRINTF("\"%s\"\n", str1);
		return Str_Error | line << 1;
	}
	return (uint32_t) Str_Ok;
}
uint32_t Assert_Word(const char* str, const char* word, uint32_t line) {
	if (Str_compareWord(str, word)) {
		PRINTF("\"%s\"\n", word);
		return Str_Error | line << 1;
	}
	return (uint32_t) Str_Ok;
}
uint32_t Assert_Num(int num1, int num2, uint32_t line) {
	if (num1 != num2) {
		PRINTF("%d\r\n", (num1));
		return Str_Error | line << 1;
	}
	return (uint32_t) Str_Ok;
}
uint32_t Assert_Float(float num1, float num2, uint32_t line) {
	if (num1 != num2) {
		PRINTF("%f\r\n", (num1));
		return Str_Error | line << 1;
	}
	return (uint32_t) Str_Ok;
}

uint32_t Assert_Strs(const char** strs1, const char** strs2, int len, uint32_t line) {
	int tempLen = len;
	const char** tempStrs = strs1;
	while (len--) {
		if (Str_compare(*strs1++, *strs2++)) {
			printStrs(tempStrs, tempLen);
			return Str_Error | line << 1;
		}
	}
	return (uint32_t) Str_Ok;
}

uint32_t Assert_Ptr(const void* ptr1, const void* ptr2, uint32_t line) {
    if (ptr1 != ptr2) {
        PRINTF("%p\r\n", (ptr1));
		return Str_Error | line << 1;
    }
    return (uint32_t) Str_Ok;
}

