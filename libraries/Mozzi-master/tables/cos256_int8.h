#ifndef COS256_H_
#define COS256_H_

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "mozzi_pgmspace.h"

#define COS256_NUM_CELLS 256
#define COS256_SAMPLERATE 256

CONSTTABLE_STORAGE(int8_t) COS256_DATA []  =
        {
                -128, -128, -128, -128, -128, -128, -127,
                -127, -126, -125, -125, -124, -123, -122, -121, -120, -119, -118, -116, -115,
                -113, -112, -110, -109, -107, -105, -103, -101, -99, -97, -95, -93, -91, -89,
                -86, -84, -82, -79, -77, -74, -72, -69, -66, -64, -61, -58, -55, -52, -49, -47,
                -44, -41, -38, -35, -32, -29, -25, -22, -19, -16, -13, -10, -7, -4, -1, 3, 6, 9,
                12, 15, 18, 21, 24, 28, 31, 34, 37, 40, 43, 46, 48, 51, 54, 57, 60, 63, 65, 68,
                71, 73, 76, 78, 81, 83, 85, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108,
                109, 111, 112, 114, 115, 117, 118, 119, 120, 121, 122, 123, 124, 124, 125, 126,
                126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 126, 126, 125, 124,
                124, 123, 122, 121, 120, 119, 118, 117, 115, 114, 112, 111, 109, 108, 106, 104,
                102, 100, 98, 96, 94, 92, 90, 88, 85, 83, 81, 78, 76, 73, 71, 68, 65, 63, 60,
                57, 54, 51, 48, 46, 43, 40, 37, 34, 31, 28, 24, 21, 18, 15, 12, 9, 6, 3, -1, -4,
                -7, -10, -13, -16, -19, -22, -25, -29, -32, -35, -38, -41, -44, -47, -49, -52,
                -55, -58, -61, -64, -66, -69, -72, -74, -77, -79, -82, -84, -86, -89, -91, -93,
                -95, -97, -99, -101, -103, -105, -107, -109, -110, -112, -113, -115, -116, -118,
                -119, -120, -121, -122, -123, -124, -125, -125, -126, -127, -127, -128, -128,
                -128, -128, -128,
        };

#endif /* COS256_H_ */
