#include <Windows.h>
#include <iostream>
#include "Ego.h"

using namespace std;

Ego core;
int arrInput[] = { 1 };
int arrOutput[] = { 0 };

int main()
{
  int ret = 0;  

  ret = core.load_priori("C:\\Users\\turbi\\AppData\\Roaming\\Turbine\\pri\\tm revolution\\mereDelay_level_time");
  if (ret != 0) { return 10 + ret; }
  cout << "PRIORI LOADED" << endl;

  ret = core.load_driver("ASIO", "Yamaha Steinberg USB ASIO");
  if (ret != 0) { return 20 + ret; }
  cout << "DRIVER LOADED" << endl;
  
  ret = core.prepare(0);
  if (ret != 0) { return 30 + ret; }
  cout << "PREPARED" << endl;
  
  ret = core.arrange(arrInput, arrOutput);
  if (ret != 0) { return 40 + ret; }
  cout << "ARRANGED" << endl;

  ret = core.launch();
  if (ret != 0) { return 50 + ret; }
  cout << "LAUNCHED" << endl;

  const int numSteer = 2;
  int* a = new int[numSteer];

  while (true)
  {
    for (int i = 0; i < numSteer; i++)
    {
      cin >> a[i];
    }    
    core.steer(a, numSteer);
  }
  
  return 0;
}