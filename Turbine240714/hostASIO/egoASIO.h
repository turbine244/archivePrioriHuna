#ifndef EGO_ASIO
#define EGO_ASIO

#ifdef EGO_ASIO_EXPORTS
#define EGO_ASIO_DECLSPEC __declspec(dllexport)
#else
#define EGO_ASIO_DECLSPEC __declspec(dllimport)
#endif

#include <string>
#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"
#include "typedefEgo.h"

extern "C" EGO_ASIO_DECLSPEC int driver_do_init();
extern "C" EGO_ASIO_DECLSPEC int driver_do_clear();
extern "C" EGO_ASIO_DECLSPEC int driver_do_load(int* numInput, int* numOutput, std::string nameDriver);
extern "C" EGO_ASIO_DECLSPEC int driver_do_launch(int* ptrSteer, int numInput, int numOutput, int** arrangement, BUFFERACTION func);
extern "C" EGO_ASIO_DECLSPEC int driver_do_exit(int state);
extern "C" EGO_ASIO_DECLSPEC int driver_tell_numDriver(int* numDriver);
extern "C" EGO_ASIO_DECLSPEC int driver_tell_listDriver(std::string* listNameDriver, int numDriver);
extern "C" EGO_ASIO_DECLSPEC int driver_tell_listChannel(std::string* listNameInput, std::string* listNameOutput, int numInput, int numOutput);
extern "C" EGO_ASIO_DECLSPEC int driver_tell_latency(double* latency);
extern "C" EGO_ASIO_DECLSPEC int driver_open_control();

inline void _tell_infoChannel(std::string* name, long* type, long idxChannel, ASIOBool isInput);
inline int _get_currentSpb();
int _cast_int_sps(void* spsInfo);
void _clear_load();
void _clear_live();

void acb_bs(long index, ASIOBool processNow);
void acb_srdc(ASIOSampleRate sRate);
ASIOTime* acb_bsti(ASIOTime* timeInfo, long index, ASIOBool processNow);
long acb_am(long selector, long value, void* message, double* opt);

#endif