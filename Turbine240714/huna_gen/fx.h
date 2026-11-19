#pragma once

#include <string>
#include <fstream>
#include <Windows.h>

typedef struct Info
{
  const std::string typePriori = "huna";
  const int numSteer = 1;
  const int numInput = 1;
  const int numOutput = 1;
  const std::string namePriori = "phono";
};

typedef struct Huna
{

};

void writeHeader();
void create_monitor();

// 0 * 1 >> 14 = 1
// 1 * B = shrt_max

// MAX >= a * shrt_max