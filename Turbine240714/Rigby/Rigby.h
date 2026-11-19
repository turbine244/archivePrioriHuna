#ifndef RIGBY
#define RIGBY

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <initializer_list>
#include <Windows.h>
#include "enumRigby.h"

//====----====----====----====---- FDEC



//====----====----====----====---- STRUCT

typedef struct AID
{
  int type;
  int idx;
};

//====----====----====----====---- CLASS

class Info
{
public:
  Info(std::string dir, std::string fileName);

  std::string getDir();
  std::string getFileName();

private:
  std::string dir;
  std::string fileName;
};

class Tone
{
public:
  Tone(std::initializer_list<float> listFloat);
  Tone(int typeSrc, std::string path);

  LARGE_INTEGER getOffset();
  int getLength();

private:
  LARGE_INTEGER offset;
  int length;
};

class Abil
{
public:
  AID getId();

protected:
  AID id;
};

class Steer
{
public:
  Steer(std::string caption, int max, int init);

  int get();
private:
  std::string caption;
  int init;
  int max;
};

class Param
{
public:
  Param(std::initializer_list<Steer> factorSet);

  float get();
  void set(float data);

  void tellFactorSet(std::vector<Steer> vtray);

private:
  std::vector<int> idxFactor;
};

class Affair : public Abil
{
public:
  void touch(Abil abil, Tone tone, int length, int offset, float weight);
  void touch(Abil abil, Tone tone, Param length, int offset, float weight);
  void touch(Abil abil, Tone tone, int length, Param offset, float weight);
  void touch(Abil abil, Tone tone, Param length, Param offset, float weight);
  void touch(Abil abil, Tone tone, int length, int offset, Param weight);
  void touch(Abil abil, Tone tone, Param length, int offset, Param weight);
  void touch(Abil abil, Tone tone, int length, Param offset, Param weight);
  void touch(Abil abil, Tone tone, Param length, Param offset, Param weight);

private:
  std::vector<AID> idAbil;
  std::vector<std::vector<int>> idxFactor;
};

class Buffer : public Affair
{
public:
  Buffer();
};

class X : public Abil
{
public:
  X(std::string caption);
    
private:
  std::string caption;
};

class Y : public Affair
{
public:
  Y(std::string caption);
private:
  std::string caption;
};


void core_touch(Abil abil, Tone tone, int length, int offset, float weight);

#endif