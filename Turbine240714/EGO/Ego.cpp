#include "Ego.h"
#include <iostream>
using namespace std;

//====----====----====----====----====----====----====----====----VAL

#define DIR std::string(getenv("APPDATA")) + "\\Turbine\\Ego"

//====----====----====----====----====----====----====----====----FDEF

//====----====----====----====----PUBLIC

Ego::Ego()
{

}

Ego::~Ego()
{

}

int Ego::load_priori(std::string pathPriori)
{
  if (state != LOAD) { return -1; }

  return _do_load_priori(pathPriori);
}

int Ego::load_driver(std::string typeDriver, std::string nameDriver)
{
  if (state != LOAD) { return -1; }

  return _do_load_driver(typeDriver, nameDriver);
}

int Ego::prepare(std::string** tableString)
{
  if (state != LOAD) { return -1; }

  if (tableString != 0)
  {
    int ret = 0;
    ret += priori.tell_listCaption(tableString[0], tableString[1], tableString[2], priori.get_numSteer(), priori.get_numOutput(), priori.get_numSteer());
    ret += driver.tell_listChannel(tableString[3], tableString[4], driver.get_numInput(), driver.get_numOutput());

    if (ret != 0) { return 1; }
  }

  if (arrangement != 0) { return 2; }

  arrangement = new int* [2];
  arrangement[0] = new int [priori.get_numInput()]();
  arrangement[1] = new int [priori.get_numOutput()]();

  ifstream fi(priori.get_path() + "\\ego.bin", ios::binary | ios::ate);
  if (fi.is_open() && fi.tellg() == (priori.get_numInput() + priori.get_numOutput()) * sizeof(int))
  {
    fi.seekg(0, ios::beg);
    fi.read(reinterpret_cast<char*>(arrangement[0]), priori.get_numInput() * sizeof(int));
    fi.read(reinterpret_cast<char*>(arrangement[1]), priori.get_numOutput() * sizeof(int));
    fi.close();
  }  

  state = READY;
  return 0;
}

int Ego::arrange(int* arrInput, int* arrOutput)
{
  if (state != READY) { return -1; }

  if (arrInput == 0) { return 1; }
  if (arrOutput == 0) { return 1; }

  memcpy(arrangement[0], arrInput, driver.get_numInput() * sizeof(int));
  memcpy(arrangement[1], arrOutput, driver.get_numOutput() * sizeof(int));

  return 0;
}

int Ego::launch()
{
  if (state != READY) { return -1; }

  int ret = 0;

  // LAUNCH PRIORI
  ret = priori.do_launch();
  if (ret != 0) { return ret; }

  // SET STEER FILE
  if (priori.get_numSteer() != 0)
  {
    fh_steer = CreateFile((DIR + "\\steer.bin").data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fh_steer == 0 || fh_steer == INVALID_HANDLE_VALUE) { return 2; }

    DWORD fileSize = priori.get_numSteer() * sizeof(int);
    DWORD written = 0;
    int* initSteer = new int[priori.get_numSteer()];
    priori.tell_initSteer(initSteer);
    WriteFile(fh_steer, initSteer, fileSize, &written, 0);

    mh_steer = CreateFileMapping(fh_steer, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    if (mh_steer == 0 || mh_steer == INVALID_HANDLE_VALUE) { return 2; }

    mmap_steer = (int*)MapViewOfFile(mh_steer, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (mmap_steer == 0) { return 2; }
  }  
  
  // LAUNCH DRIVER
  ret = driver.do_launch(mmap_steer, priori.get_numInput(), priori.get_numOutput(), arrangement, priori.bufferAction);
  if (ret != 0) { return ret; }

  state = LIVE;
  return 0;
}

int Ego::exit()
{
  if (state == LOAD) { return -1; }

  if (state == LIVE)
  {
    UnmapViewOfFile(mmap_steer);
    CloseHandle(mh_steer);
    CloseHandle(fh_steer);
  }

  int ret = 0;
  ret += priori.do_exit(state);
  ret += driver.do_exit(state);

  state = LOAD;
  return (ret == 0 ? 0 : 1);
}

int Ego::steer(int* listInt, int lenList)
{
  if (state != LIVE) { return -1; }

  if (priori.get_numSteer() != 0 && lenList != 0)
  {
    memcpy(mmap_steer, listInt, lenList * sizeof(int));
  }
  
  return 0;
}

int Ego::tell_Info(int* listInt)
{
  if (listInt == 0) { return 1; }

  listInt[0] = priori.get_numSteer();
  listInt[1] = priori.get_numInput();
  listInt[2] = priori.get_numOutput();
  listInt[3] = driver.get_numInput();
  listInt[4] = driver.get_numOutput();

  return 0;
}

//====----====----====----====----PRIVATE

int Ego::_do_load_driver(std::string typeDriver, std::string nameDriver)
{
  int ret = driver.loadApi(typeDriver);
  if (ret != 0) { return ret; }

  int buffer[2];
  ret = driver.do_load(&buffer[0], &buffer[1], nameDriver);
  if (ret != 0) { return 2; }
  
  driver.set_name(nameDriver + "(" + typeDriver + ")");
  driver.set_numInput(buffer[0]);
  driver.set_numOutput(buffer[1]);
  return 0;
}

int Ego::_do_load_priori(std::string pathPriori)
{
  // CHECK FILE EXISTANCE
  ifstream fi(pathPriori + "\\info.txt");
  if (fi.is_open() == false) { return 2; }

  // CHECK FILE NUM_LINE : MORE THAN 5
  string line;
  for (int i = 0; i < 4; i++)
  {
    getline(fi, line);
    if (fi.eof()) { return 2; }
  }
  fi.seekg(0);

  // CHECK API EXISTANCE
  string typePriori;
  getline(fi, typePriori);
  int ret = priori.loadApi(typePriori);
  if (ret != 0) { return ret; }

  // CHECK FILE VALIDITY
  int buffer[3];
  string temp;

  for (int i = 0; i < 3; i++)
  {
    getline(fi, temp);
    buffer[i] = stoi(temp);
  }

  ret = priori.do_load(pathPriori, buffer[0], buffer[1], buffer[2]);
  if (ret != 0) { return ret; }
  
  // DONE
  string namePriori;
  getline(fi, namePriori);
  priori.set_path(pathPriori);
  priori.set_name(namePriori + "(" + typePriori + ")");
  priori.set_numSteer(buffer[0]);
  priori.set_numInput(buffer[1]);
  priori.set_numOutput(buffer[2]);

  return 0;
}