#ifndef HUNA
#define HUNA

#ifdef HUNA_EXPORTS
#define HUNA_DECLSPEC __declspec(dllexport)
#else
#define HUNA_DECLSPEC __declspec(dllimport)
#endif

#include <string>
#include <fstream>
#include <chrono>
#include <wtypes.h>
#include "typedefHuna.h"

extern "C" HUNA_DECLSPEC int priori_do_init();
extern "C" HUNA_DECLSPEC int priori_do_clear();
extern "C" HUNA_DECLSPEC int priori_do_load(std::string path, int numSteer, int numInput, int numOutput);
extern "C" HUNA_DECLSPEC int priori_do_launch();
extern "C" HUNA_DECLSPEC int priori_do_exit(int state);
extern "C" HUNA_DECLSPEC int priori_tell_initSteer(int* initSteer);
extern "C" HUNA_DECLSPEC int priori_tell_listCaption(std::string * listCaptionSteer, std::string * listCaptionInput, std::string * listCaptionOutput, int numSteer, int numInput, int numOutput);
extern "C" HUNA_DECLSPEC int priori_tell_latency(double* latency, int spb);
extern "C" HUNA_DECLSPEC void priori_bufferAction(int* steer, int** input, int** output, int spb);

LARGE_INTEGER _cookFort(RecipeFort recipe, int page);
void _playAffair(int idxAffair);
void _applyTouch(int idxAffair, int idxTouch, int _valAngle);
void _readChunk(int*& dest, int length, int* _mmap_bin, ptrdiff_t& _cur);
void _readChunkChunk(int**& dest, int lengthRow, int* lengthCol, int* _mmap_bin, ptrdiff_t& _cur);
void _aimFox(int idxFox, int _lenBody);

int _check_hdr_binary(std::string path, int numSteer, int numInput, int numOutput);
void _clear();

#endif