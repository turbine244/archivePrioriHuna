#include "egoASIO.h"
#include <iostream>
using namespace std;

//====----====----====----====----====----====----====----====----VAL

enum {
  LOAD = 0, READY, LIVE,
};

//====----====----====----====----====----====----====----====----VAR

extern AsioDrivers* asioDrivers;

ASIODriverInfo driverInfo;
ASIOBufferInfo* bufferInfo = 0;
EgoInfo egoInfo = { 0 };
ASIOCallbacks callbacks = { &acb_bs, &acb_srdc, &acb_am, &acb_bsti };

//====----====----====----====----====----====----====----====----FDEF

//====----====----====----====----PUBLIC

int driver_tell_numDriver(int* numDriver)
{
  if (asioDrivers == 0
    || asioDrivers->getCurrentDriverIndex() != -1)
  {
    return 1;
  }
  else
  {
    *numDriver = asioDrivers->asioGetNumDev();
    return 0;
  }
}

int driver_tell_listDriver(std::string* listNameDriver, int numDriver)
{
  if (numDriver < 0)
  {
    return 1;
  }
  else
  {
    char buffer[32];

    for (int i = 0; i < numDriver; i++)
    {
      asioDrivers->asioGetDriverName(i, buffer, 32);
      listNameDriver[i] = buffer;
    }

    return 0;
  }
}

int driver_tell_listChannel(std::string* listNameInput, std::string* listNameOutput, int numInput, int numOutput)
{
  string name;

  for (int i = 0; i < numInput; i++)
  {
    _tell_infoChannel(&name, 0, i, ASIOTrue);
    listNameInput[i] = name;
  }
  for (int i = 0; i < numOutput; i++)
  {
    _tell_infoChannel(&name, 0, i, ASIOFalse);
    listNameOutput[i] = name;
  }

  return 0;
}

int driver_tell_latency(double* latency)
{
  ASIOSampleRate asioSps;
  ASIOGetSampleRate(&asioSps);

  int currentSpb = _get_currentSpb();

  *latency = (double)currentSpb / asioSps * 2000;
  return 0;
}

int driver_do_init()
{
  if (asioDrivers != 0)
  {
    return 1;
  }
  else
  {
    asioDrivers = new AsioDrivers;
    return 0;
  }
}

int driver_do_clear()
{
  if (asioDrivers != 0)
  {
    delete asioDrivers;
    asioDrivers = 0;
  }

  return 0;
}

int driver_do_load(int* numInput, int* numOutput, std::string nameDriver)
{
  bool loaded = asioDrivers->loadDriver(nameDriver.data());

  if (loaded == false)
  {
    return 1;
  }

  long buffer[2];
  ASIOGetChannels(&buffer[0], &buffer[1]);
  *numInput = (int)buffer[0];
  *numOutput = (int)buffer[1];

  ASIOInit(&driverInfo);

  return 0;
}

int driver_do_launch(int* ptrSteer, int numInput, int numOutput, int** arrangement, BUFFERACTION func)
{
  // create buffers
  egoInfo.numInput = numInput;
  egoInfo.numOutput = numOutput;

  bufferInfo = new ASIOBufferInfo[egoInfo.numInput + egoInfo.numOutput];
  for (int i = 0; i < egoInfo.numInput; i++)
  {
    bufferInfo[i].isInput = ASIOTrue;
    bufferInfo[i].channelNum = (arrangement == 0 ? i : arrangement[0][i]);
  }
  for (int i = 0; i < egoInfo.numOutput; i++)
  {
    bufferInfo[egoInfo.numInput + i].isInput = ASIOFalse;
    bufferInfo[egoInfo.numInput + i].channelNum = (arrangement == 0 ? i : arrangement[1][i]);
  }

  egoInfo.spb = _get_currentSpb();
  egoInfo.bufferAction = func;  

  ASIOCreateBuffers(bufferInfo, egoInfo.numInput + egoInfo.numOutput, egoInfo.spb, &callbacks);

  // make ego sio
  egoInfo.steer = ptrSteer;

  egoInfo.ptrInput = new int** [2];
  egoInfo.ptrInput[0] = new int* [egoInfo.numInput];
  egoInfo.ptrInput[1] = new int* [egoInfo.numInput];
  for (int i = 0; i < egoInfo.numInput; i++)
  {
    egoInfo.ptrInput[0][i] = (int*)(bufferInfo[i].buffers[0]);
    egoInfo.ptrInput[1][i] = (int*)(bufferInfo[i].buffers[1]);
  }

  egoInfo.ptrOutput = new int** [2];
  egoInfo.ptrOutput[0] = new int* [egoInfo.numOutput];
  egoInfo.ptrOutput[1] = new int* [egoInfo.numOutput];
  for (int i = 0; i < egoInfo.numOutput; i++)
  {
    egoInfo.ptrOutput[0][i] = (int*)(bufferInfo[i + egoInfo.numInput].buffers[0]);
    egoInfo.ptrOutput[1][i] = (int*)(bufferInfo[i + egoInfo.numInput].buffers[1]);
  }

  // start stream
  ASIOStart();
  
  return 0;
}

int driver_do_exit(int state)
{
  switch (state)
  {
  case LIVE:
    _clear_live();
    break;
  case READY:
    break;
  case LOAD:
    _clear_load();
    break;
  default:
    break;
  }
  return 0;
}

int driver_open_control()
{
  if (asioDrivers->getCurrentDriverIndex() != -1)
  {
    ASIOControlPanel();
    return 0;
  }
  else
  {
    return 1;
  }
}

//====----====----====----====----PRIVATE

// ASIO Callbacks
void acb_bs(long index, ASIOBool processNow)
{
  egoInfo.bufferAction(egoInfo.steer, egoInfo.ptrInput[index], egoInfo.ptrOutput[index], egoInfo.spb);
}

void acb_srdc(ASIOSampleRate sRate)
{
  
}

ASIOTime* acb_bsti(ASIOTime* timeInfo, long index, ASIOBool processNow)
{
  acb_bs(index, processNow);
  return timeInfo;
}

long acb_am(long selector, long value, void* message, double* opt)
{
  return 0;
}

// misc
inline void _tell_infoChannel(std::string* name, long* type, long idxChannel, ASIOBool isInput)
{
  ASIOChannelInfo info = { idxChannel, isInput };
  ASIOGetChannelInfo(&info);
  if (name != 0) { *name = info.name; }  
  if (type != 0) { *type = info.type; }
}

inline int _get_currentSpb()
{
  long waste, data;
  ASIOGetBufferSize(&waste, &waste, &data, &waste);

  return (int)data;
}

int _cast_int_sps(void* spsInfo)
{
  int ret = 0;

  if (IEEE754_64FLOAT)
  {
    double* info = (double*)spsInfo;
    ret = (int)info;
  }
  else
  {
    char* info = (char*)spsInfo;

  }

  return ret;
}

void _clear_load()
{
  delete asioDrivers;
}

void _clear_live()
{
  delete[] egoInfo.steer;

  delete[] egoInfo.ptrInput[0];
  delete[] egoInfo.ptrInput[1];
  delete[] egoInfo.ptrInput;

  delete[] egoInfo.ptrOutput[0];
  delete[] egoInfo.ptrOutput[1];
  delete[] egoInfo.ptrOutput;

  egoInfo = { 0 };

  ASIODisposeBuffers();
  delete[] bufferInfo;
}