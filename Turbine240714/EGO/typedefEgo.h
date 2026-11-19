#ifndef EGO_TYPEDEF
#define EGO_TYPEDEF

#include <string>

// API : PRIORI
typedef int (*PRIORI_DO_INIT) ();
typedef int (*PRIORI_DO_CLEAR) ();
typedef int (*PRIORI_DO_LOAD) (std::string path, int numSteer, int numInput, int numOutput);
typedef int (*PRIORI_DO_LAUNCH) ();
typedef int (*PRIORI_DO_EXIT) (int state);
typedef int (*PRIORI_TELL_INITSTEER) (int* initSteer);
typedef int (*PRIORI_TELL_LISTCAPTION) (std::string* listCaptionSteer, std::string* listCaptionInput, std::string* listCaptionOutput, int numSteer, int numInput, int numOutput);
typedef int (*PRIORI_TELL_LATENCY) (double* latency, int spb);
typedef void (*PRIORI_BUFFERACTION) (int* steer, int** input, int** output, int spb);

// API : DRIVER
typedef int (*DRIVER_DO_INIT) ();
typedef int (*DRIVER_DO_CLEAR) ();
typedef int (*DRIVER_DO_LOAD) (int* numInput, int* numOutput, std::string nameDriver);
typedef int (*DRIVER_DO_LAUNCH) (int* ptrSteer, int numInput, int numOutput, int** arrangement, PRIORI_BUFFERACTION func);
typedef int (*DRIVER_DO_EXIT) (int state);
typedef int (*DRIVER_TELL_NUMDRIVER) (int* numDriver);
typedef int (*DRIVER_TELL_LISTDRIVER) (std::string* listNameDriver, int numDriver);
typedef int (*DRIVER_TELL_LISTCHANNEL) (std::string* listNameInput, std::string* listNameOutput, int numInput, int numOutput);
typedef int (*DRIVER_TELL_LATENCY) (double* latency);
typedef int (*DRIVER_OPEN_CONTROL) ();

#endif