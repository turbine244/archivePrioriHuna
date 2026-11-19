#ifndef EGO_API__DRIVER
#define EGO_API__DRIVER

#include "EgoApi.h"
#include "enumEgo.h"
#include "typedefEgo.h"

class EgoApi_driver : public EgoApi
{
public:
  DRIVER_DO_INIT do_init = 0;
  DRIVER_DO_CLEAR do_clear = 0;
  DRIVER_DO_LOAD do_load = 0;
  DRIVER_DO_LAUNCH do_launch = 0;
  DRIVER_DO_EXIT do_exit = 0;
  DRIVER_TELL_NUMDRIVER tell_numDriver = 0;
  DRIVER_TELL_LISTDRIVER tell_listDriver = 0;
  DRIVER_TELL_LISTCHANNEL tell_listChannel = 0;
  DRIVER_TELL_LATENCY tell_latency = 0;
  DRIVER_OPEN_CONTROL open_control = 0;

  int loadApi(std::string _typeApi);
  int freeApi();

  std::string get_name();
  int get_numInput();
  int get_numOutput();

  void set_name(std::string nameDriver);
  void set_numInput(int numInput);
  void set_numOutput(int numOutput);

private:
  std::string name = "";
  int numInput = 0;
  int numOutput = 0;
};

#endif