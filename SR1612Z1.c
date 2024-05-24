#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "MinUnit.h"
#include "SR1612Z1.h"

#define SR1612Z1_BAUD_RATE_MSG_ID "PCAS01"
#define SR1612Z1_UPDATE_RATE_MSG_ID "PCAS02"
#define SR1612Z1_OUTPUT_MSG_ID "PCAS03"
#define SR1612Z1_MODE_MSG_ID "PCAS04"
#define SR1612Z1_RESTART_MSG_ID "PCAS10"

#define SR1612Z1_TXT_MSG_ID "GPTXT"

#define SR1612Z1_TXT_MAX_LEN 32

static uint8_t calcChecksum(const char *msg, int msgLen) {
    uint8_t checksum = 0;

    for (int i = 1; i < msgLen - 1; i++) {
        checksum ^= msg[i];
    }

    return checksum;
}

static void writeChecksum(char *msg, int msgLen) {
    // Calculate checksum
    uint8_t checksum = calcChecksum(msg, msgLen);

    // Write checksum as two hexadecimal characters
    sprintf(msg + msgLen, "%02X", checksum);
}

static bool checkChecksum(const char *msg, int msgLen, uint8_t checksum) {
    // Check start character
    if (msg[0] != '$') {
        return false;
    }

    // Find end character for checksum calculation
    char *coreMsgEnd = memchr(msg, '*', msgLen);
    if (coreMsgEnd == NULL) {
        return false;
    }

    // Calculate checksum
    uint8_t coreChecksum = calcChecksum(msg, coreMsgEnd - msg);

    // Check checksum
    return coreChecksum == checksum;
}

static bool parseTxtMsg(const char *msg, struct SR1612Z1_TxtMessage *txtMessage) {
    // Example message: $GPTXT,01,01,01,ANTENNA OPEN*25

    // Read message fields
    char msgId[6];
    char data[SR1612Z1_TXT_MAX_LEN];
    int checksum;
    int numRead = sscanf(msg, "$%[^,],%*[^,],%*[^,],%*[^,],%31[^*]*%d", msgId, data, &checksum);
    if (numRead != 6) {
        return false;
    }

    // Check type
    if (strcmp(msgId, SR1612Z1_TXT_MSG_ID) != 0) {
        return false;
    }

    // Check checksum
    if (!checkChecksum(msg, strlen(msg), checksum)) {
        return false;
    }

    // Parse message data
    if (!strcmp(data, "ANTENNA OPEN")) {
        txtMessage->antennaStatus = SR1612Z1_ANTENNA_OPEN;
    } else if (!strcmp(data, "ANTENNA OK")) {
        txtMessage->antennaStatus = SR1612Z1_ANTENNA_OK;
    } else if (!strcmp(data, "ANTENNA SHORT")) {
        txtMessage->antennaStatus = SR1612Z1_ANTENNA_SHORT;
    } else {
        txtMessage->antennaStatus = SR1612Z1_ANTENNA_UNKNOWN;
    }

    return true;
}

void SR1612Z1_MakeBaudRateMsg(enum SR1612Z1_BaudRate baudRate, char *msg) {
    // Example message: $PCAS01,1*1D

    // Write core message
    int len = sprintf(msg, "$%s,%d*", SR1612Z1_BAUD_RATE_MSG_ID, (int)baudRate);

    // Calculate and write checksum
    writeChecksum(msg, len);
}

void SR1612Z1_MakeUpdateRateMsg(enum SR1612Z1_PosUpdateRate posUpdateRate, char *msg) {
    // Example message: $PCAS02,1000*2E

    // Write core message
    int len = sprintf(msg, "$%s,%d*", SR1612Z1_UPDATE_RATE_MSG_ID, (int)posUpdateRate);

    // Calculate and write checksum
    writeChecksum(msg, len);
}

void SR1612Z1_MakeOutputMsg(const struct SR1612Z1_OutputRates *outputRates, char *msg) {
    // Example message: $PCAS03,1,1,1,1,1,1,1,1,0,1,0,0,1,0*02

    // Write core message
    int len = sprintf(msg, "$%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d*", SR1612Z1_OUTPUT_MSG_ID,
        outputRates->nGGA, outputRates->nGLL, outputRates->nGSA, outputRates->nGSV,
        outputRates->nRMC, outputRates->nVTG, outputRates->nZDA, outputRates->nANT,
        outputRates->nDHV, outputRates->nLPS, outputRates->nUTC, outputRates->nGST);
    
    // Calculate and write checksum
    writeChecksum(msg, len);
}

void SR1612Z1_MakeModeMsg(enum SR1612Z1_Mode mode, char *msg) {
    // Example message: $PCAS04,3*1A

    // Write core message
    int len = sprintf(msg, "$%s,%d*", SR1612Z1_MODE_MSG_ID, (int)mode);
    
    // Calculate and write checksum
    writeChecksum(msg, len);
}

void SR1612Z1_MakeRestartMsg(enum SR1612Z1_RestartType restartType, char *msg) {
    // Example message: $PCAS10,0*1C

    // Write core message
    int len = sprintf(msg, "$%s,%d*", SR1612Z1_RESTART_MSG_ID, (int)restartType);
    
    // Calculate and write checksum
    writeChecksum(msg, len);
}

enum SR1612Z1_CustomMessageType SR1612Z1_GetCustomMessageType(const char *msg) {
    // Check prefix
    if (msg[0] != '$') {
        // Common prefix
        return false;
    }

    // Check type
    if (strncmp(msg, SR1612Z1_TXT_MSG_ID, strlen(SR1612Z1_TXT_MSG_ID)) == 0) {
        return SR1612Z1_CUSTOM_MSG_TYPE_TXT;
    } else {
        return SR1612Z1_CUSTOM_MSG_TYPE_NONE;
    }
}

bool SR1612Z1_ParseCustomMessage(const char *msg, struct SR1612Z1_CustomMessage *customMessage) {
    // Get custom message type
    enum SR1612Z1_CustomMessageType type = SR1612Z1_GetCustomMessageType(msg);
    if (type == SR1612Z1_CUSTOM_MSG_TYPE_NONE) {
        return false;
    }

    // Parse custom message
    if (type == SR1612Z1_CUSTOM_MSG_TYPE_TXT) {
        customMessage->type = SR1612Z1_CUSTOM_MSG_TYPE_TXT;
        return SR1612Z1_ParseTxtMessage(msg, &customMessage->txt);
    } else {
        return false;
    }
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

    // Test custom message type for non-custom message
    enum SR1612Z1_CustomMessageType type;
    const char *nonCustomMsg = "$GNVTG,0.00,T,,M,0.00,N,0.00,K,A*23";
    type = SR1612Z1_GetCustomMessageType(nonCustomMsg);
    mu_assert("Wrong custom message type", type == SR1612Z1_CUSTOM_MSG_TYPE_NONE);

    // Test custom message type for custom message
    const char *customMsg = "$GPTXT,01,01,01,ANTENNA OPEN*25";
    type = SR1612Z1_GetCustomMessageType(customMsg);
    mu_assert("Wrong custom message type", type == SR1612Z1_CUSTOM_MSG_TYPE_TXT);

    // Test custom message parsing
    struct SR1612Z1_CustomMessage customMessage;
    bool result = SR1612Z1_ParseCustomMessage(customMsg, &customMessage);
    mu_assert("Wrong custom message parsing", result);
    mu_assert("Wrong custom message type", customMessage.type == SR1612Z1_CUSTOM_MSG_TYPE_TXT);
    mu_assert("Wrong antenna status", customMessage.txt.antennaStatus == SR1612Z1_ANTENNA_OPEN);

    return 0;
}
