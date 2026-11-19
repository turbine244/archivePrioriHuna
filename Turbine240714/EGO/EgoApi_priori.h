#ifndef EGO_API__PRIORI
#define EGO_API__PRIORI

#include "EgoApi.h"
#include "enumEgo.h"
#include "typedefEgo.h"

class EgoApi_priori : public EgoApi
{
public:
  PRIORI_DO_INIT do_init = 0;
  PRIORI_DO_CLEAR do_clear = 0;
  PRIORI_DO_LOAD do_load = 0;
  PRIORI_DO_LAUNCH do_launch = 0;
  PRIORI_DO_EXIT do_exit = 0;
  PRIORI_TELL_INITSTEER tell_initSteer = 0;
  PRIORI_TELL_LISTCAPTION tell_listCaption = 0;
  PRIORI_TELL_LATENCY tell_latency = 0;
  PRIORI_BUFFERACTION bufferAction = 0;

  int loadApi(std::string _typeApi);
  int freeApi();
  
  std::string get_path();
  std::string get_name();
  int get_numSteer();
  int get_numInput();
  int get_numOutput();

  void set_path(std::string pathPriori);
  void set_name(std::string namePriori);
  void set_numSteer(int numSteerPriori);
  void set_numInput(int numInputPriori);
  void set_numOutput(int numOutputPriori);

private:
  std::string path = "";
  std::string name = "";
  int numSteer = 0;
  int numInput = 0;
  int numOutput = 0;
};

#endif