#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
using namespace std;

static int testsRun = 0;
static int testsFailed = 0;

#define CHECK(cond, msg) \
    do { \
        testsRun++; \
        if (cond) { \
            cout << "PASS: " << msg << endl; \
        } else { \
            testsFailed++; \
            cout << "FAIL: " << msg << " (line " << __LINE__ << ")" << endl; \
        } \
    } while (0)

// put at the end of main() in every test file
#define TEST_SUMMARY() \
    do { \
        cout << (testsRun - testsFailed) << "/" << testsRun << " passed" << endl; \
        return testsFailed == 0 ? 0 : 1; \
    } while (0)

#endif
