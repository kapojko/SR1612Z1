#ifndef SR1612Z1_H
#define SR1612Z1_H

#include <stdbool.h>

#define SR1612Z1_DEF_BAUD_RATE 38400

#define SR1612Z1_MAX_NMEA0183_MESSAGE_LENGTH 82

enum SR1612Z1_BaudRate {
    SR1612Z1_BAUD_4800 = 0,
    SR1612Z1_BAUD_9600 = 1,
    SR1612Z1_BAUD_19200 = 2,
    SR1612Z1_BAUD_38400 = 3,
    SR1612Z1_BAUD_57600 = 4,
    SR1612Z1_BAUD_115200 = 5
};

enum SR1612Z1_PosUpdateRate {
    SR1612Z1_RATE_1HZ = 1000,
    SR1612Z1_RATE_2HZ = 500,
    SR1612Z1_RATE_4HZ = 250,
    SR1612Z1_RATE_5HZ = 200,
    SR1612Z1_RATE_10HZ = 100
};

struct SR1612Z1_OutputRates {
    int nGGA; // 0 - no output, 1-9 - output every n positioning times
    int nGLL;
    int nGSA;
    int nGSV;
    int nRMC;
    int nVTG;
    int nZDA;
    int nANT;
    int nDHV;
    int nLPS;
    int nUTC;
    int nGST;
};

enum SR1612Z1_Mode {
    SR1612Z1_MODE_GPS = 1,
    SR1612Z1_MODE_BDS = 2,
    SR1612Z1_MODE_GPS_BDS = 3,
    SR1612Z1_MODE_GLONASS = 4,
    SR1612Z1_MODE_GPS_GLONASS = 5,
    SR1612Z1_MODE_BDS_GLONASS = 6,
    SR1612Z1_MODE_GPS_BDS_GLONASS = 7
};

enum SR1612Z1_RestartType {
    SR1612Z1_RESTART_HOT = 0,
    SR1612Z1_RESTART_WARM = 1,
    SR1612Z1_RESTART_COLD = 2,
    SR1612Z1_RESTART_FACTORY_BOOT = 3
};

enum SR1612Z1_AntennaStatus {
    SR1612Z1_ANTENNA_UNKNOWN = 0,
    SR1612Z1_ANTENNA_OPEN = 1,
    SR1612Z1_ANTENNA_OK = 2,
    SR1612Z1_ANTENNA_SHORT = 3
};

struct SR1612Z1_TxtMessage {
    enum SR1612Z1_AntennaStatus antennaStatus;
};

enum SR1612Z1_CustomMessageType {
    SR1612Z1_CUSTOM_MSG_TYPE_NONE = 0,
    SR1612Z1_CUSTOM_MSG_TYPE_TXT = 1
};

struct SR1612Z1_CustomMessage {
    enum SR1612Z1_CustomMessageType type;
    union {
        struct SR1612Z1_TxtMessage txt;
    };
};

void SR1612Z1_MakeBaudRateMsg(enum SR1612Z1_BaudRate baudRate, char *msg);
void SR1612Z1_MakeUpdateRateMsg(enum SR1612Z1_PosUpdateRate posUpdateRate, char *msg);
void SR1612Z1_MakeOutputMsg(const struct SR1612Z1_OutputRates *outputRates, char *msg);
void SR1612Z1_MakeModeMsg(enum SR1612Z1_Mode mode, char *msg);
void SR1612Z1_MakeRestartMsg(enum SR1612Z1_RestartType restartType, char *msg);

enum SR1612Z1_CustomMessageType SR1612Z1_GetCustomMsgType(const char *msg);
bool SR1612Z1_ParseCustomMsg(const char *msg, struct SR1612Z1_CustomMessage *customMessage);

const char *SR1612Z1_UnitTest(void);

#endif // SR1612Z1_H
