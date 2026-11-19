#ifndef EGO
#define EGO

#include <string>
#include <fstream>
#include <filesystem>
#include "enumEgo.h"
#include "EgoApi_driver.h"
#include "EgoApi_priori.h"

class Ego {
public:
  Ego();
  ~Ego();

  int load_priori(std::string pathPriori);
  int load_driver(std::string typeDriver, std::string nameDriver);
  int prepare(std::string** tableString);
  int arrange(int* arrInput, int* arrOutput);
  int launch();
  int exit();
  int steer(int* listSteer, int lenSteer);

  int tell_Info(int* listInt);

protected:
  int state = LOAD;
  int** arrangement = 0;

  EgoApi_driver driver = {};
  EgoApi_priori priori = {};

  HANDLE fh_steer = 0;
  HANDLE mh_steer = 0;
  int* mmap_steer = 0;

  int _do_load_driver(std::string typeDriver, std::string nameDriver);
  int _do_load_priori(std::string pathPriori);
};

#endif