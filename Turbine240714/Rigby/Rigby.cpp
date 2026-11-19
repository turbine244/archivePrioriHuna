#include "Rigby.h"

using namespace std;

//====----====----====----====----====----====----====----====----VAR

int procState = 0;

Info* ptr_info = 0;
vector<Tone*> ptr_list_tone;

vector<X*> ptr_list_x;
vector<Y*> ptr_list_y;
vector<Steer*> ptr_list_steer;

vector<int> vtray;

vector<Buffer*> ptr_list_buffer;

//====----====----====----====----====----====----====----====----FDEF

//====----====----====----====----CLASS::META

//====----Info

Info::Info(std::string dir, std::string fileName)
{
  this->dir = dir;
  this->fileName = fileName;

  ptr_info = this;
}

std::string Info::getDir()
{
  return dir;
}

std::string Info::getFileName()
{
  return fileName;
}

//====----Tone

Tone::Tone(std::initializer_list<float> listFloat)
{
  ofstream fo("ToneBank.bin", ios::binary | ios::app);

  length = listFloat.size();
  offset.QuadPart = fo.tellp();

  for (float f : listFloat)
  {
    fo.write(reinterpret_cast<const char*>(&f), sizeof(float));
  }

  ptr_list_tone.push_back(this);
}

Tone::Tone(int typeSrc, std::string path)
{
  if (typeSrc == WAV)
  {
    ifstream fi(path, ios::binary);
    fi.seekg(40);

    unsigned int sizeChunk;
    fi.read(reinterpret_cast<char*>(&sizeChunk), sizeof(unsigned int));
    unsigned int numChunk = sizeChunk / 4;

    ofstream fo("ToneBank.bin", ios::binary | ios::app);

    length = numChunk;
    offset.QuadPart = fo.tellp();

    while (numChunk--)
    {
      short shortSample;
      fi.read(reinterpret_cast<char*>(&shortSample), sizeof(short));
      float floatSample = static_cast<float>(shortSample);

      fo.write(reinterpret_cast<const char*>(&shortSample), sizeof(float));
    }
  }
}

LARGE_INTEGER Tone::getOffset()
{
  return offset;
}

int Tone::getLength()
{
  return length;
}

//====----====----====----====----CLASS::PARENT

//====----Affair

void Affair::touch(Abil abil, Tone tone, int length, int offset, float weight)
{  
  if (procState == LIVE)
  {
    core_touch(abil, tone, length, offset, weight);
  }
  else
  {
    int idxTouch = -1;
    for (int i = 0; i < this->idAbil.size(); i++)
    {
      if (abil.getId().type == this->idAbil[i].type
        && abil.getId().idx == this->idAbil[i].idx)
      {
        idxTouch = i;
        break;
      }
    }

    if (idxTouch == -1)
    {
      this->idAbil.push_back(abil.getId());
    }
  }
}

void Affair::touch(Abil abil, Tone tone, Param length, int offset, float weight)
{
  if (procState == LIVE)
  {
    core_touch(abil, tone, length.get(), offset, weight);
  }
  else
  {
    int idxTouch = -1;

    for (int i = 0; i < idAbil.size(); i++)
    {
      if (abil.getId().type == idAbil[i].type
        && abil.getId().idx == idAbil[i].idx)
      {
        idxTouch = i;
        break;
      }
    }

    if (idxTouch == -1)
    {
      idAbil.push_back(abil.getId());
      idxTouch = vtray.size();
      vtray.push_back({});
    }

    length.tellFactorSet()

    bool checked = false;

    for (int i = 0; i < vtray[idxTouch].size(); i++)
    {
      if (vtray[idxTouch][i] == length.tellFactorSet)
    }
  }
}

void Affair::touch(Abil abil, Tone tone, int length, Param offset, float weight)
{
  if (procState == LIVE)
  {
    core_touch(abil, tone, length, offset.get(), weight);
  }
  else
  {

  }
}

void Affair::touch(Abil abil, Tone tone, Param length, Param offset, float weight)
{
  core_touch(abil, tone, length.get(), offset.get(), weight);
}

void Affair::touch(Abil abil, Tone tone, int length, int offset, Param weight)
{
  core_touch(abil, tone, length, offset, weight.get());
}

void Affair::touch(Abil abil, Tone tone, Param length, int offset, Param weight)
{
  core_touch(abil, tone, length.get(), offset, weight.get());
}

void Affair::touch(Abil abil, Tone tone, int length, Param offset, Param weight)
{
  core_touch(abil, tone, length, offset.get(), weight.get());
}

void Affair::touch(Abil abil, Tone tone, Param length, Param offset, Param weight)
{
  core_touch(abil, tone, length.get(), offset.get(), weight.get());
}

//====----====----====----====----CLASS::FACE

//====----X

X::X(std::string caption)
{  
  this->caption = caption;
  
  ptr_list_x.push_back(this);
}

//====----Y

Y::Y(std::string caption)
{
  this->caption = caption;

  ptr_list_y.push_back(this);
}

//====----Steer

Steer::Steer(std::string caption, int max, int init)
{
  this->caption = caption;
  this->max = max;
  this->init = init;

  ptr_list_steer.push_back(this);
  vtray.push_back(0);
}

int Steer::get()
{
  return 0;
}

//====----====----====----====----CLASS::BODY

//====----Buffer

Buffer::Buffer()
{
  ptr_list_buffer.push_back(this);
}

//====----Param

Param::Param(std::initializer_list<Steer> factorSet)
{
  for (Steer f : factorSet)
  {
    this->factorSet.push_back(f);
  }
}

float Param::get()
{
  return 0;
}

void Param::set(float data)
{

}

void Param::tellFactorSet(std::vector<Steer> vtray)
{
}

//====----====----====----====----GLOBAL

void core_touch(Abil abil, Tone tone, int length, int offset, float weight)
{

}


