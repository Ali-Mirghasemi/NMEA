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

uint32_t Assert_Num(int num1, int num2, uint32_t line);
uint32_t Assert_Float(float num1, float num2, uint32_t line);

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
                .Minutes = 41.68513f,
                .Indicator = NMEA_Indicator_North,
            },
            .Longitude = {
                .Degrees = 51,
                .Minutes = 23.50382f,
                .Indicator = NMEA_Indicator_East,
            },
            .PositionFix = NMEA_PositionFix_DifferentialGPS,
            .SatellitesUsed = 11,
            .HDOP = 0.85f,
            .Altitude = 1236.4f,
            .AltitudeUnit = 'M',
            .Geoid = -17.6f,
            .GeoidUnits = 'M',
            .AgeOfDiff = 0,
            .DiffRefStationId = 0,
        },
    };
    Str_copy(temp, "$GPGGA,071457.00,3541.68513,N,05123.50382,E,2,11,0.85,1236.4,M,-17.6,M,,0000*4E\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG1);

    const NMEA_Message MSG2 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_GGA,
        },
        .Checksum = 0x4E,
        .GGA = {
            .Time = {
                .Hours = 17,
                .Minutes = 19,
                .Seconds = 56,
                .Milliseconds = 500,
            },
            .Latitude = {
                .Degrees = 38,
                .Minutes = 46.68513f,
                .Indicator = NMEA_Indicator_South,
            },
            .Longitude = {
                .Degrees = 14,
                .Minutes = 23.50382f,
                .Indicator = NMEA_Indicator_West,
            },
            .PositionFix = NMEA_PositionFix_DifferentialGPS,
            .SatellitesUsed = 11,
            .HDOP = 0.85f,
            .Altitude = 100.4f,
            .AltitudeUnit = 'M',
            .Geoid = -19.7f,
            .GeoidUnits = 'M',
            .AgeOfDiff = 0,
            .DiffRefStationId = 0,
        },
    };
    Str_copy(temp, "$GPGGA,171956.500,3846.68513,S,01423.50382,W,2,11,0.85,100.4,M,-19.7,M,,0000*4E\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG2);

    return 0;
}
#endif
#if NMEA_MESSAGE_GLL
uint32_t Test_GLL(void) {
    NMEA_Message msg;
    char temp[128];

    const NMEA_Message MSG1 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_GLL,
        },
        .Checksum = 0x6F,
        .GLL = {
            .Latitude = {
                .Degrees = 35,
                .Minutes = 41.68513f,
                .Indicator = NMEA_Indicator_North,
            },
            .Longitude = {
                .Degrees = 51,
                .Minutes = 23.50382f,
                .Indicator = NMEA_Indicator_East,
            },
            .Time = {
                .Hours = 7,
                .Minutes = 14,
                .Seconds = 57,
                .Milliseconds = 0,
            },
            .Status = NMEA_MessageStatus_Valid,
            .Mode = NMEA_Mode_DGPS,
        },
    };
    Str_copy(temp, "$GPGLL,3541.68513,N,05123.50382,E,071457.00,A,D*6F\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG1);

    const NMEA_Message MSG2 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_GLL,
        },
        .Checksum = 0x69,
        .GLL = {
            .Latitude = {
                .Degrees = 35,
                .Minutes = 41.68521f,
                .Indicator = NMEA_Indicator_North,
            },
            .Longitude = {
                .Degrees = 51,
                .Minutes = 23.50386f,
                .Indicator = NMEA_Indicator_East,
            },
            .Time = {
                .Hours = 7,
                .Minutes = 15,
                .Seconds = 0,
                .Milliseconds = 0,
            },
            .Status = NMEA_MessageStatus_Valid,
            .Mode = NMEA_Mode_DGPS,
        },
    };
    Str_copy(temp, "$GPGLL,3541.68521,N,05123.50386,E,071500.00,A,D*69\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG2);

    return 0;
}
#endif
#if NMEA_MESSAGE_GSA
uint32_t Test_GSA(void) {
    NMEA_Message msg;
    char temp[128];

    const NMEA_Message MSG1 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_GSA,
        },
        .Checksum = 0x03,
        .GSA = {
            .Mode = NMEA_GSA_Mode_Auto,
            .FixStatus = NMEA_FixStatus_3D,
            .SatellitesUsed = {24, 02, 15, 17, 30, 06, 13, 28, 12, 19, 40},
            .PDOP = 1.72f,
            .HDOP = 0.85f,
            .VDOP = 1.49f,
        },
    };
    Str_copy(temp, "$GPGSA,A,3,24,02,15,17,30,06,13,28,12,19,40,,1.72,0.85,1.49*03\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG1);

    const NMEA_Message MSG2 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_GSA,
        },
        .Checksum = 0x33,
        .GSA = {
            .Mode = NMEA_GSA_Mode_Auto,
            .FixStatus = NMEA_FixStatus_3D,
            .SatellitesUsed = {7, 2, 26, 27, 9, 4, 15},
            .PDOP = 1.8f,
            .HDOP = 1.0f,
            .VDOP = 1.5f,
        },
    };
    Str_copy(temp, "$GPGSA,A,3,07,02,26,27,09,04,15,,,,,,1.8,1.0,1.5*33\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG2);

    return 0;
}
#endif
#if NMEA_MESSAGE_GSV
uint32_t Test_GSV(void) {
    NMEA_Message msg;
    char temp[128];

    const NMEA_Message MSG1 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_GSV,
        },
        .Checksum = 0x71,
        .GSV = {
            .TotalMessages = 2,
            .MessageNumber = 1,
            .SatellitesInView = 7,
            .Satellites = {
                {
                    .Id = 7,
                    .Elevation = 79,
                    .Azimuth = 48,
                    .SNR = 42,
                },
                {
                    .Id = 2,
                    .Elevation = 51,
                    .Azimuth = 62,
                    .SNR = 43,
                },
                {
                    .Id = 26,
                    .Elevation = 36,
                    .Azimuth = 256,
                    .SNR = 42,
                },
                {
                    .Id = 27,
                    .Elevation = 27,
                    .Azimuth = 138,
                    .SNR = 42,
                },
            },
        },
    };
    Str_copy(temp, "$GPGSV,2,1,07,07,79,048,42,02,51,062,43,26,36,256,42,27,27,138,42*71\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG1);

    const NMEA_Message MSG2 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_GSV,
        },
        .Checksum = 0x41,
        .GSV = {
            .TotalMessages = 2,
            .MessageNumber = 2,
            .SatellitesInView = 7,
            .Satellites = {
                {
                    .Id = 9,
                    .Elevation = 23,
                    .Azimuth = 313,
                    .SNR = 42,
                },
                {
                    .Id = 4,
                    .Elevation = 19,
                    .Azimuth = 159,
                    .SNR = 41,
                },
                {
                    .Id = 15,
                    .Elevation = 12,
                    .Azimuth = 41,
                    .SNR = 42,
                },
            },
        },
    };
    Str_copy(temp, "$GPGSV,2,2,07,09,23,313,42,04,19,159,41,15,12,041,42*41\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG2);

    return 0;
}
#endif
#if NMEA_MESSAGE_RMC
uint32_t Test_RMC(void) {
    NMEA_Message msg;
    char temp[128];

    const NMEA_Message MSG1 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_RMC,
        },
        .Checksum = 0x10,
        .RMC = {
            .Time = {
                .Hours = 16,
                .Minutes = 12,
                .Seconds = 29,
                .Milliseconds = 487,
            },
            .Status = NMEA_MessageStatus_Valid,
            .Latitude = {
                .Degrees = 37,
                .Minutes = 23.2475f,
                .Indicator = NMEA_Indicator_North,
            },
            .Longitude = {
                .Degrees = 121,
                .Minutes = 58.3416f,
                .Indicator = NMEA_Indicator_West,
            },
            .SpeedOverGround = 0.13f,
            .CourseOverGround = 309.62f,
            .Date = {
                .Day = 12,
                .Month = 5,
                .Year = 98,
            },
        },
    };
    Str_copy(temp, "$GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598,,*10\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG1);

    const NMEA_Message MSG2 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_RMC,
        },
        .Checksum = 0x7E,
        .RMC = {
            .Time = {
                .Hours = 7,
                .Minutes = 14,
                .Seconds = 56,
                .Milliseconds = 0,
            },
            .Status = NMEA_MessageStatus_Valid,
            .Latitude = {
                .Degrees = 35,
                .Minutes = 41.68511f,
                .Indicator = NMEA_Indicator_North,
            },
            .Longitude = {
                .Degrees = 51,
                .Minutes = 23.50384f,
                .Indicator = NMEA_Indicator_East,
            },
            .SpeedOverGround = 0.012f,
            .CourseOverGround = 0.0f,
            .Date = {
                .Day = 18,
                .Month = 5,
                .Year = 20,
            },
        },
    };
    Str_copy(temp, "$GPRMC,071456.00,A,3541.68511,N,05123.50384,E,0.012,,180520,,,D*7E\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG2);

    return 0;
}
#endif
#if NMEA_MESSAGE_VTG
uint32_t Test_VTG(void) {
    NMEA_Message msg;
    char temp[128];

    const NMEA_Message MSG1 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_VTG,
        },
        .Checksum = 0x23,
        .VTG = {
            .Course1 = 309.62f,
            .Reference1 = 'T',
            .Course2 = 0.0f,
            .Reference2 = 'M',
            .Speed1 = 0.13f,
            .SpeedUnits1 = 'N',
            .Speed2 = 0.2f,
            .SpeedUnits2 = 'K',
            .Mode = NMEA_Mode_Autonomous,
        },
    };
    Str_copy(temp, "$GPVTG,309.62,T,,M,0.13,N,0.2,K,A*23\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG1);

    return 0;
}
#endif
#if NMEA_MESSAGE_ZDA
uint32_t Test_ZDA(void) {
    NMEA_Message msg;
    char temp[128];

    const NMEA_Message MSG1 = {
        .Header = {
            .Id = NMEA_Id_GPS,
            .Type = NMEA_MessageType_ZDA,
        },
        .Checksum = 0x4F,
        .ZDA = {
            .Time = {
                .Hours = 18,
                .Minutes = 18,
                .Seconds = 13,
                .Milliseconds = 0,
            },
            .Day = 14,
            .Month = 10,
            .Year = 2003,
            .LocalZoneHours = 0,
            .LocalZoneMinutes = 0,
        },
    };
    Str_copy(temp, "$GPZDA,181813,14,10,2003,00,00*4F\r\n");
    Mem_set(&msg, 0, sizeof(msg));
    assert(Result, NMEA_parseRaw(temp, &msg));
    assert(Message, &msg, &MSG1);

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
            assert(Message_GLL, &msg1->GLL, &msg2->GLL);
            break;
    #endif
    #if NMEA_MESSAGE_GSA
        case NMEA_MessageType_GSA:
            assert(Message_GSA, &msg1->GSA, &msg2->GSA);
            break;
    #endif
    #if NMEA_MESSAGE_GSV
        case NMEA_MessageType_GSV:
            assert(Message_GSV, &msg1->GSV, &msg2->GSV);
            break;
    #endif
    #if NMEA_MESSAGE_RMC
        case NMEA_MessageType_RMC:
            assert(Message_RMC, &msg1->RMC, &msg2->RMC);
            break;
    #endif
    #if NMEA_MESSAGE_VTG
        case NMEA_MessageType_VTG:
            assert(Message_VTG, &msg1->VTG, &msg2->VTG);
            break;
    #endif
    #if NMEA_MESSAGE_ZDA
        case NMEA_MessageType_ZDA:
            assert(Message_ZDA, &msg1->ZDA, &msg2->ZDA);
            break;
    #endif
    }

    return 0;
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
    assert(Num, msg1->Latitude.Indicator, msg2->Latitude.Indicator);
#endif
#if NMEA_MESSAGE_GGA_LONGITUDE
    assert(Float, msg1->Longitude.Degrees, msg2->Longitude.Degrees);
    assert(Float, msg1->Longitude.Minutes, msg2->Longitude.Minutes);
    assert(Num, msg1->Longitude.Indicator, msg2->Longitude.Indicator);
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
#if NMEA_MESSAGE_GGA_ALTITUDE_UNIT
    assert(Num, msg1->AltitudeUnit, msg2->AltitudeUnit);
#endif
#if NMEA_MESSAGE_GGA_GEOID
    assert(Float, msg1->Geoid, msg2->Geoid);
#endif
#if NMEA_MESSAGE_GGA_GEOIDUNITS
    assert(Num, msg1->GeoidUnits, msg2->GeoidUnits);
#endif
#if NMEA_MESSAGE_GGA_DIFFREFSTATIONID
    assert(Num, msg1->DiffRefStationId, msg2->DiffRefStationId);
#endif
#if NMEA_MESSAGE_GGA_AGEOFDIFF
    assert(Num, msg1->AgeOfDiff, msg2->AgeOfDiff);
#endif
    return 0;

}
uint32_t Assert_Message_GLL(NMEA_Message_GLL* msg1, NMEA_Message_GLL* msg2, uint32_t line) {
#if NMEA_MESSAGE_GLL_LATITUDE
    assert(Num, msg1->Latitude.Degrees, msg2->Latitude.Degrees);
    assert(Float, msg1->Latitude.Minutes, msg2->Latitude.Minutes);
    assert(Num, msg1->Latitude.Indicator, msg2->Latitude.Indicator);
#endif
#if NMEA_MESSAGE_GLL_LONGITUDE
    assert(Num, msg1->Longitude.Degrees, msg2->Longitude.Degrees);
    assert(Float, msg1->Longitude.Minutes, msg2->Longitude.Minutes);
    assert(Num, msg1->Longitude.Indicator, msg2->Longitude.Indicator);
#endif
#if NMEA_MESSAGE_GLL_TIME
    assert(Num, msg1->Time.Hours, msg2->Time.Hours);
    assert(Num, msg1->Time.Minutes, msg2->Time.Minutes);
    assert(Num, msg1->Time.Seconds, msg2->Time.Seconds);
    assert(Num, msg1->Time.Milliseconds, msg2->Time.Milliseconds);
#endif
#if NMEA_MESSAGE_GLL_STATUS
    assert(Num, msg1->Status, msg2->Status);
#endif
#if NMEA_MESSAGE_GLL_MODE
    assert(Num, msg1->Mode, msg2->Mode);
#endif
    return 0;
}
uint32_t Assert_Message_GSA(NMEA_Message_GSA* msg1, NMEA_Message_GSA* msg2, uint32_t line) {
#if NMEA_MESSAGE_GSA_MODE
    assert(Num, msg1->Mode, msg2->Mode);
#endif
#if NMEA_MESSAGE_GSA_FIX_STATUS
    assert(Num, msg1->FixStatus, msg2->FixStatus);
#endif
#if NMEA_MESSAGE_GSA_SATELLITESUSED
    for (int i = 0; i < NMEA_GSA_MAX_SATELLITES; i++) {
        assert(Num, msg1->SatellitesUsed[i], msg2->SatellitesUsed[i]);
    }
#endif
#if NMEA_MESSAGE_GSA_PDOP
    assert(Float, msg1->PDOP, msg2->PDOP);
#endif
#if NMEA_MESSAGE_GSA_HDOP
    assert(Float, msg1->HDOP, msg2->HDOP);
#endif
#if NMEA_MESSAGE_GSA_VDOP
    assert(Float, msg1->VDOP, msg2->VDOP);
#endif
    return 0;
}
uint32_t Assert_Message_GSV(NMEA_Message_GSV* msg1, NMEA_Message_GSV* msg2, uint32_t line) {
#if NMEA_MESSAGE_GSV_TOTALMESSAGES
    assert(Num, msg1->TotalMessages, msg2->TotalMessages);
#endif
#if NMEA_MESSAGE_GSV_MESSAGENUMBER
    assert(Num, msg1->MessageNumber, msg2->MessageNumber);
#endif
#if NMEA_MESSAGE_GSV_SATELLITESINVIEW
    assert(Num, msg1->SatellitesInView, msg2->SatellitesInView);
#endif
#if NMEA_MESSAGE_GSV_SATELLITES
    for (int i = 0; i < NMEA_GSV_MAX_SATELLITES; i++) {
        assert(Num, msg1->Satellites[i].Id,         msg2->Satellites[i].Id);
        assert(Num, msg1->Satellites[i].Elevation,  msg2->Satellites[i].Elevation);
        assert(Num, msg1->Satellites[i].Azimuth,    msg2->Satellites[i].Azimuth);
        assert(Num, msg1->Satellites[i].SNR,        msg2->Satellites[i].SNR);
    }
#endif
    return 0;
}
uint32_t Assert_Message_RMC(NMEA_Message_RMC* msg1, NMEA_Message_RMC* msg2, uint32_t line) {
#if NMEA_MESSAGE_RMC_TIME
    assert(Num, msg1->Time.Hours, msg2->Time.Hours);
    assert(Num, msg1->Time.Minutes, msg2->Time.Minutes);
    assert(Num, msg1->Time.Seconds, msg2->Time.Seconds);
    assert(Num, msg1->Time.Milliseconds, msg2->Time.Milliseconds);
#endif
#if NMEA_MESSAGE_RMC_STATUS
    assert(Num, msg1->Status, msg2->Status);
#endif
#if NMEA_MESSAGE_RMC_LATITUDE
    assert(Num, msg1->Latitude.Degrees, msg2->Latitude.Degrees);
    assert(Float, msg1->Latitude.Minutes, msg2->Latitude.Minutes);
    assert(Num, msg1->Latitude.Indicator, msg2->Latitude.Indicator);
#endif
#if NMEA_MESSAGE_RMC_LONGITUDE
    assert(Num, msg1->Longitude.Degrees, msg2->Longitude.Degrees);
    assert(Float, msg1->Longitude.Minutes, msg2->Longitude.Minutes);
    assert(Num, msg1->Longitude.Indicator, msg2->Longitude.Indicator);
#endif
#if NMEA_MESSAGE_RMC_SPEEDOVERGROUND
    assert(Float, msg1->SpeedOverGround, msg2->SpeedOverGround);
#endif
#if NMEA_MESSAGE_RMC_COURSEOVERGROUND
    assert(Float, msg1->CourseOverGround, msg2->CourseOverGround);
#endif
#if NMEA_MESSAGE_RMC_DATE
    assert(Num, msg1->Date.Day, msg2->Date.Day);
    assert(Num, msg1->Date.Month, msg2->Date.Month);
    assert(Num, msg1->Date.Year, msg2->Date.Year);
#endif
    return 0;
}
uint32_t Assert_Message_VTG(NMEA_Message_VTG* msg1, NMEA_Message_VTG* msg2, uint32_t line) {
#if NMEA_MESSAGE_VTG_COURSE1
    assert(Float, msg1->Course1, msg2->Course1);
#endif
#if NMEA_MESSAGE_VTG_REFERENCE1
    assert(Num, msg1->Reference1, msg2->Reference1);
#endif
#if NMEA_MESSAGE_VTG_COURSE2
    assert(Float, msg1->Course2, msg2->Course2);
#endif
#if NMEA_MESSAGE_VTG_REFERENCE2
    assert(Num, msg1->Reference2, msg2->Reference2);
#endif
#if NMEA_MESSAGE_VTG_SPEED1
    assert(Float, msg1->Speed1, msg2->Speed1);
#endif
#if NMEA_MESSAGE_VTG_SPEEDUNITS1
    assert(Num, msg1->SpeedUnits1, msg2->SpeedUnits1);
#endif
#if NMEA_MESSAGE_VTG_SPEED2
    assert(Float, msg1->Speed2, msg2->Speed2);
#endif
#if NMEA_MESSAGE_VTG_SPEEDUNITS2
    assert(Num, msg1->SpeedUnits2, msg2->SpeedUnits2);
#endif
#if NMEA_MESSAGE_VTG_MODE
    assert(Num, msg1->Mode, msg2->Mode);
#endif
    return 0;
}
uint32_t Assert_Message_ZDA(NMEA_Message_ZDA* msg1, NMEA_Message_ZDA* msg2, uint32_t line) {
#if NMEA_MESSAGE_ZDA_TIME
    assert(Num, msg1->Time.Hours, msg2->Time.Hours);
    assert(Num, msg1->Time.Minutes, msg2->Time.Minutes);
    assert(Num, msg1->Time.Seconds, msg2->Time.Seconds);
    assert(Num, msg1->Time.Milliseconds, msg2->Time.Milliseconds);
#endif
#if NMEA_MESSAGE_ZDA_DAY
    assert(Num, msg1->Day, msg2->Day);
#endif
#if NMEA_MESSAGE_ZDA_MONTH
    assert(Num, msg1->Month, msg2->Month);
#endif
#if NMEA_MESSAGE_ZDA_YEAR
    assert(Num, msg1->Year, msg2->Year);
#endif
#if NMEA_MESSAGE_ZDA_LOCALZONEHOURS
    assert(Num, msg1->LocalZoneHours, msg2->LocalZoneHours);
#endif
#if NMEA_MESSAGE_ZDA_LOCALZONEMINUTES
    assert(Num, msg1->LocalZoneMinutes, msg2->LocalZoneMinutes);
#endif
    return 0;
}

uint32_t Assert_Num(int num1, int num2, uint32_t line) {
	if (num1 != num2) {
		PRINTF("Expected: %d, Found: %d\r\n", num2, num1);
		return Str_Error | line << 1;
	}
	return (uint32_t) Str_Ok;
}
uint32_t Assert_Float(float num1, float num2, uint32_t line) {
	if (num1 != num2) {
		PRINTF("Expected: %f, Found: %f\r\n", num2, num1);
		return Str_Error | line << 1;
	}
	return (uint32_t) Str_Ok;
}

