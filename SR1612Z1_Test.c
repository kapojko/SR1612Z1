#ifdef TESTING

#include <stdio.h>
#include "SR1612Z1.h"

int main(void) {
    const char *unitTestResult = SR1612Z1_UnitTest();
    if (unitTestResult == 0) {
        printf("UNIT TEST PASSED\n");
        return 0;
    } else {
        printf("UNIT TEST FAILED: %s\n", unitTestResult);
        return 1;
    }
}

#endif
