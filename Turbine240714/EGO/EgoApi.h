#ifndef EGO_API
#define EGO_API

#include <string>
#include <Windows.h>

class EgoApi
{
public:
  virtual int loadApi(std::string _typeApi) = 0;
  virtual int freeApi() = 0;

protected:
  HMODULE hDll = 0;
  std::string typeApi = "";
};

#endif