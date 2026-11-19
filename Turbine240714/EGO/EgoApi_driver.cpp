#include "EgoApi_driver.h"

#define DIR std::string(getenv("APPDATA")) + "\\Turbine\\api\\"

using namespace std;

int EgoApi_driver::loadApi(std::string _typeApi)
{
  string path = DIR + "ego" + _typeApi + ".dll";
  hDll = ::LoadLibraryA(path.data());
  if (hDll == 0) { return 1; }
  
  do_init = (DRIVER_DO_INIT)::GetProcAddress(hDll, "driver_do_init");
  do_clear = (DRIVER_DO_CLEAR)::GetProcAddress(hDll, "driver_do_clear");
  do_load = (DRIVER_DO_LOAD)::GetProcAddress(hDll, "driver_do_load");
  do_launch = (DRIVER_DO_LAUNCH)::GetProcAddress(hDll, "driver_do_launch");
  do_exit = (DRIVER_DO_EXIT)::GetProcAddress(hDll, "driver_do_exit");
  tell_numDriver = (DRIVER_TELL_NUMDRIVER)::GetProcAddress(hDll, "driver_tell_numDriver");
  tell_listDriver = (DRIVER_TELL_LISTDRIVER)::GetProcAddress(hDll, "driver_tell_listDriver");
  tell_listChannel = (DRIVER_TELL_LISTCHANNEL)::GetProcAddress(hDll, "driver_tell_listChannel");
  tell_latency = (DRIVER_TELL_LATENCY)::GetProcAddress(hDll, "driver_tell_latency");
  open_control = (DRIVER_OPEN_CONTROL)::GetProcAddress(hDll, "driver_open_control");

  if (do_init == 0
    || do_clear == 0
    || do_load == 0
    || do_launch == 0
    || do_exit == 0
    || tell_numDriver == 0
    || tell_listDriver == 0
    || tell_listChannel == 0
    || tell_latency == 0
    || open_control == 0
    )
  {
    freeApi();
    return 2;
  }

  typeApi = _typeApi;
  do_init();
  return 0;
}

int EgoApi_driver::freeApi()
{
  if (hDll == 0) { return 1; }
  
  do_exit(State::LOAD);
  ::FreeLibrary(hDll);
  hDll = 0;

  do_init = 0;
  do_clear = 0;
  do_load = 0;
  do_launch = 0;
  do_exit = 0;
  tell_numDriver = 0;
  tell_listDriver = 0;
  tell_listChannel = 0;
  tell_latency = 0;
  open_control = 0;

  name = "";
  numInput = 0;
  numOutput = 0;

  return 0;
}

// Getter

std::string EgoApi_driver::get_name()
{
  return name;
}

int EgoApi_driver::get_numInput()
{
  return numInput;
}

int EgoApi_driver::get_numOutput()
{
  return numOutput;
}

// Setter

void EgoApi_driver::set_name(std::string nameDriver)
{
  name = nameDriver;
}

void EgoApi_driver::set_numInput(int numInputDriver)
{
  numInput = numInputDriver;
}

void EgoApi_driver::set_numOutput(int numOutputDriver)
{
  numOutput = numOutputDriver;
}