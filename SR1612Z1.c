#include <string.h>
#include <stdio.h>
#include "MinUnit.h"
#include "SR1612Z1.h"

#define SR1612Z1_BAUD_RATE_MSG_ID "PCAS01"
#define SR1612Z1_UPDATE_RATE_MSG_ID "PCAS02"
#define SR1612Z1_OUTPUT_MSG_ID "PCAS03"
#define SR1612Z1_MODE_MSG_ID "PCAS04"
#define SR1612Z1_RESTART_MSG_ID "PCAS10"

static void calcWriteChecksum(char *msg, int msgLen) {
    uint8_t checksum = 0;

    // Calculate checksum
    for (int i = 1; i < msgLen - 1; i++) {
        checksum ^= msg[i];
    }

    // Write checksum as two hexadecimal characters
    sprintf(msg + msgLen, "%02X", checksum);
}

void SR1612Z1_MakeBaudRateMsg(enum SR1612Z1_BaudRate baudRate, char *msg) {
    // Write core message
    int len = sprintf(msg, "$%s,%d*", SR1612Z1_BAUD_RATE_MSG_ID, (int)baudRate);

    // Calculate and write checksum
    calcWriteChecksum(msg, len);
}

void SR1612Z1_MakeUpdateRateMsg(enum SR1612Z1_PosUpdateRate posUpdateRate, char *msg) {
    // Write core message
    int len = sprintf(msg, "$%s,%d*", SR1612Z1_UPDATE_RATE_MSG_ID, (int)posUpdateRate);

    // Calculate and write checksum
    calcWriteChecksum(msg, len);
}

void SR1612Z1_MakeOutputMsg(const struct SR1612Z1_OutputRates *outputRates, char *msg) {
    // Write core message
    int len = sprintf(msg, "$%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d*", SR1612Z1_OUTPUT_MSG_ID,
        outputRates->nGGA, outputRates->nGLL, outputRates->nGSA, outputRates->nGSV,
        outputRates->nRMC, outputRates->nVTG, outputRates->nZDA, outputRates->nANT,
        outputRates->nDHV, outputRates->nLPS, outputRates->nUTC, outputRates->nGST);
    
    // Calculate and write checksum
    calcWriteChecksum(msg, len);
}

void SR1612Z1_MakeModeMsg(enum SR1612Z1_Mode mode, char *msg) {
    // Write core message
    int len = sprintf(msg, "$%s,%d*", SR1612Z1_MODE_MSG_ID, (int)mode);
    
    // Calculate and write checksum
    calcWriteChecksum(msg, len);
}

void SR1612Z1_MakeRestartMsg(enum SR1612Z1_RestartType restartType, char *msg) {
    // Write core message
    int len = sprintf(msg, "$%s,%d*", SR1612Z1_RESTART_MSG_ID, (int)restartType);
    
    // Calculate and write checksum
    calcWriteChecksum(msg, len);
}

const char *SR1612Z1_UnitTest(void) {
    char msg[SR1612Z1_MAX_NMEA0183_MESSAGE_LENGTH];

    // Test baud rate message
    SR1612Z1_MakeBaudRateMsg(SR1612Z1_BAUD_9600, msg);
    const char *baudRateExpected = "$PCAS01,1*1D";
    mu_assert("Wrong baud rate message", strcmp(msg, baudRateExpected) == 0);

    // Test update rate message
    SR1612Z1_MakeUpdateRateMsg(SR1612Z1_RATE_1HZ, msg);
    const char *updateRateExpected = "$PCAS02,1000*2E";
    mu_assert("Wrong update rate message", strcmp(msg, updateRateExpected) == 0);

    // Test output message
    struct SR1612Z1_OutputRates outputRates = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0};
    SR1612Z1_MakeOutputMsg(&outputRates, msg);
    const char *outputExpected = "$PCAS03,1,1,1,1,1,1,1,1,0,1,0,0,1,0*02";
    mu_assert("Wrong output message", strcmp(msg, outputExpected) == 0);

    // Test mode message
    SR1612Z1_MakeModeMsg(SR1612Z1_MODE_GPS_BDS, msg);
    const char *modeExpected = "$PCAS04,3*1A";
    mu_assert("Wrong mode message", strcmp(msg, modeExpected) == 0);

    // Test restart message
    SR1612Z1_MakeRestartMsg(SR1612Z1_RESTART_HOT, msg);
    const char *restartExpected = "$PCAS10,0*1C";
    mu_assert("Wrong restart message", strcmp(msg, restartExpected) == 0);

    return 0;
}
