#include "EgoApi_priori.h"

#define DIR std::string(getenv("APPDATA")) + "\\Turbine\\api\\"

using namespace std;

int EgoApi_priori::loadApi(std::string _typeApi)
{
  string path = DIR + _typeApi + ".dll";
  hDll = ::LoadLibraryA(path.data());
  if (hDll == 0) { return 1; }

  typeApi = _typeApi;

  do_init = (PRIORI_DO_INIT)::GetProcAddress(hDll, "priori_do_init");
  do_clear = (PRIORI_DO_CLEAR)::GetProcAddress(hDll, "priori_do_clear");
  do_load = (PRIORI_DO_LOAD)::GetProcAddress(hDll, "priori_do_load");
  do_launch = (PRIORI_DO_LAUNCH)::GetProcAddress(hDll, "priori_do_launch");
  do_exit = (PRIORI_DO_EXIT)::GetProcAddress(hDll, "priori_do_exit");
  tell_initSteer = (PRIORI_TELL_INITSTEER)::GetProcAddress(hDll, "priori_tell_initSteer");
  tell_listCaption = (PRIORI_TELL_LISTCAPTION)::GetProcAddress(hDll, "priori_tell_listCaption");
  tell_latency = (PRIORI_TELL_LATENCY)::GetProcAddress(hDll, "priori_tell_latency");
  bufferAction = (PRIORI_BUFFERACTION)::GetProcAddress(hDll, "priori_bufferAction");

  if (do_init == 0
    || do_clear == 0
    || do_load == 0
    || do_launch == 0
    || do_exit == 0
    || tell_initSteer == 0
    || tell_listCaption == 0
    || tell_latency == 0
    || bufferAction == 0
    )
  {
    freeApi();
    return 2;
  }

  do_init();
  return 0;
}

int EgoApi_priori::freeApi()
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
  tell_initSteer = 0;
  tell_listCaption = 0;
  tell_latency = 0;
  bufferAction = 0;

  path = "";
  name = "";
  numSteer = 0;
  numInput = 0;
  numOutput = 0;

  return 0;
}

// Getter

std::string EgoApi_priori::get_path()
{
  return path;
}

std::string EgoApi_priori::get_name()
{
  return name;
}

int EgoApi_priori::get_numSteer()
{
  return numSteer;
}

int EgoApi_priori::get_numInput()
{
  return numInput;
}

int EgoApi_priori::get_numOutput()
{
  return numOutput;
}

// Setter

void EgoApi_priori::set_path(std::string pathPriori)
{
  path = pathPriori;
}

void EgoApi_priori::set_name(std::string namePriori)
{
  name = namePriori;
}

void EgoApi_priori::set_numSteer(int numSteerPriori)
{
  numSteer = numSteerPriori;
}

void EgoApi_priori::set_numInput(int numInputPriori)
{
  numInput = numInputPriori;
}

void EgoApi_priori::set_numOutput(int numOutputPriori)
{
  numOutput = numOutputPriori;
}