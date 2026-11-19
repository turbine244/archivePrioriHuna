#include "huna.h"
#include <iostream>
using namespace std;

//====----====----====----====----====----====----====----====----VAL

enum {
  LOAD = 0, READY, LIVE,
};

//====----====----====----====----====----====----====----====----VAR

// Stream-Info
std::string dir = "";

HunaInfo hunaInfo = { 0 };
CertInfo certInfo = { 0 };
AngleInfo angleInfo = { 0 };
AffairInfo affairInfo = { 0 };
RecipeFort recipeBfort = { 0 };
RecipeFort recipeQfort = { 0 };

// Memory-Mapped-File
HANDLE** fh_offsetStamp = 0;
HANDLE** mh_offsetStamp = 0;
LARGE_INTEGER*** mmap_offsetStamp = 0;

HANDLE** fh_dataStamp = 0;
HANDLE** mh_dataStamp = 0;
void*** mmap_dataStamp = 0;

HANDLE fh_tone = 0;
HANDLE mh_tone = 0;
float* mmap_tone = 0;

// Stream-Dataspace
int* tray = 0;
LARGE_INTEGER* fort = 0;

int** fox = 0;
int*** foxScope = 0;

//====----====----====----====----====----====----====----====----FDEF

//====----====----====----====----PUBLIC

int priori_do_load(std::string path, int numSteer, int numInput, int numOutput)
{
  int ret = _check_hdr_binary(path, numSteer, numInput, numOutput);
  dir = (ret == 0 ? path : "");

  return ret;
}

int priori_tell_initSteer(int* initSteer)
{
  if (certInfo.init == 0) { return 1; }

  for (int i = 0; i < hunaInfo.numCert; i++)
  {
    initSteer[i] = certInfo.init[i];
  }

  return 0;
}

int priori_tell_listCaption(std::string* listCaptionSteer, std::string* listCaptionInput, std::string* listCaptionOutput, int numSteer, int numInput, int numOutput)
{
  if (dir == "") { return 1; }

  ifstream fi(dir + "caption.txt");
  if (!fi.is_open()) { return 2; }

  int numLine = numSteer + numInput + numOutput + 3;
  string* buffer = new string[numLine];

  for (int i = 0; i < numLine; i++)
  {
    if (fi.eof())
    {
      fi.close();
      delete buffer;
      return 2;
    }
    getline(fi, buffer[i]);
  }

  int ret;
  if (fi.eof()
    && buffer[0] == "STEER:"
    && buffer[1 + numSteer] == "INPUT:"
    && buffer[2 + numSteer + numInput] == "OUTPUT:")
  {
    for (int i = 0; i < numSteer; i++)
    {
      listCaptionSteer[i] = buffer[1 + i];
    }
    for (int i = 0; i < numInput; i++)
    {
      listCaptionInput[i] = buffer[2 + numSteer + i];
    }
    for (int i = 0; i < numOutput; i++)
    {
      listCaptionOutput[i] = buffer[3 + numSteer + numInput + i];
    }
    
    ret = 0;
  }
  else
  {
    ret = 2;
  }
  fi.close();
  delete buffer;

  return ret;
}

int priori_tell_latency(double* latency, int spb)
{
  int* test_steer = new int[hunaInfo.numCert];

  int** test_input = new int* [hunaInfo.numIncert];
  for (int i = 0; i < hunaInfo.numIncert; i++)
  {
    test_input[i] = new int[spb];
  }

  int** test_output = new int* [hunaInfo.numTested];
  for (int i = 0; i < hunaInfo.numTested; i++)
  {
    test_output[i] = new int[spb];
  }

  auto start = std::chrono::high_resolution_clock::now();
  priori_bufferAction(test_steer, test_input, test_output, spb);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> elapsed = end - start;
  *latency = elapsed.count();

  return 0;
}

int priori_do_init()
{
  return 0;
}

int priori_do_clear()
{
  return 1;
}

int priori_do_launch()
{
  // Read file : huna.bin
  if (true)
  {



    // Open file
    HANDLE fh_bin = CreateFile((dir + "\\huna.bin").data(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    if (fh_bin == 0 || fh_bin == INVALID_HANDLE_VALUE) { return 2; }

    LARGE_INTEGER fileSize;
    GetFileSizeEx(fh_bin, &fileSize);
    HANDLE mh_bin = CreateFileMapping(fh_bin, NULL, PAGE_READONLY, fileSize.HighPart, fileSize.LowPart, NULL);
    if (mh_bin == 0 || mh_bin == INVALID_HANDLE_VALUE) { return 2; }

    int* mmap_bin = (int*)MapViewOfFile(mh_bin, FILE_MAP_READ, 0, 0, 0);
    if (mmap_bin == 0) { return 2; }
    ptrdiff_t cur = 0;

    // Set Huna-info
    if (*(mmap_bin + cur++) != 0x41'4E'55'48) { return 1; }
    memcpy(&hunaInfo, mmap_bin + cur, sizeof(HunaInfo));
    cur += 8;

    // Set Cert-info
    if (*(mmap_bin + cur++) != 0x54'52'45'43) { return 1; }
    _readChunk(certInfo.max, hunaInfo.numCert, mmap_bin, cur);
    _readChunk(certInfo.init, hunaInfo.numCert, mmap_bin, cur);
    
    // Set Angle-info
    if (*(mmap_bin + cur++) != 0x20'47'4E'41) { return 1; }
    angleInfo.val = new int [hunaInfo.numUniqueSpan]();
    _readChunk(angleInfo.cycle, hunaInfo.numUniqueSpan, mmap_bin, cur);
    
    // Set Affair-info
    if (*(mmap_bin + cur++) != 0x20'46'46'41) { return 1; }
    _readChunk(affairInfo.numTouch, hunaInfo.numAffair, mmap_bin, cur);
    _readChunk(affairInfo.idxAngle, hunaInfo.numAffair, mmap_bin, cur);
    _readChunk(affairInfo.idxTest, hunaInfo.numAffair, mmap_bin, cur);  
    _readChunkChunk(affairInfo.idxFort, hunaInfo.numAffair, affairInfo.numTouch, mmap_bin, cur);
    _readChunkChunk(affairInfo.idxAbil, hunaInfo.numAffair, affairInfo.numTouch, mmap_bin, cur);
    
    // Set B-fort Recipes
    if (*(mmap_bin + cur++) != 0x20'46'20'42) { return 1; }
    _readChunk(recipeBfort.idxImFort, hunaInfo.numBfort, mmap_bin, cur);
    _readChunk(recipeBfort.idxExFort, hunaInfo.numBfort, mmap_bin, cur);
    _readChunk(recipeBfort.complexity, hunaInfo.numBfort, mmap_bin, cur);
    _readChunkChunk(recipeBfort.idxSpice, hunaInfo.numBfort, recipeBfort.complexity, mmap_bin, cur);
    _readChunkChunk(recipeBfort.valWeight, hunaInfo.numBfort, recipeBfort.complexity, mmap_bin, cur);
    
    // Set Q-fort Recipes
    if (*(mmap_bin + cur++) != 0x20'46'20'51) { return 1; }
    _readChunk(recipeQfort.idxImFort, hunaInfo.numQfort, mmap_bin, cur);
    _readChunk(recipeQfort.idxExFort, hunaInfo.numQfort, mmap_bin, cur);
    _readChunk(recipeQfort.complexity, hunaInfo.numQfort, mmap_bin, cur);
    _readChunkChunk(recipeQfort.idxSpice, hunaInfo.numQfort, recipeQfort.complexity, mmap_bin, cur);
    _readChunkChunk(recipeQfort.valWeight, hunaInfo.numQfort, recipeQfort.complexity, mmap_bin, cur);
    
    // Close file
    if (*(mmap_bin + cur++) != 0x20'44'4E'45) { return 1; }
    UnmapViewOfFile(mmap_bin);
    CloseHandle(mh_bin);
    CloseHandle(fh_bin);
  }
  
  // Set yet unset global-vars
  if (true)
  {
    // mmap
    if (true)
    {
      // data | offset
      fh_offsetStamp = new HANDLE* [hunaInfo.numAffair]();
      mh_offsetStamp = new HANDLE* [hunaInfo.numAffair]();
      mmap_offsetStamp = new LARGE_INTEGER** [hunaInfo.numAffair]();

      fh_dataStamp = new HANDLE* [hunaInfo.numAffair]();
      mh_dataStamp = new HANDLE* [hunaInfo.numAffair]();
      mmap_dataStamp = new void** [hunaInfo.numAffair]();
      
      for (int i = 0; i < hunaInfo.numAffair; i++)
      {
        fh_offsetStamp[i] = new HANDLE [affairInfo.numTouch[i]]();
        fh_dataStamp[i] = new HANDLE [affairInfo.numTouch[i]]();
        mmap_offsetStamp[i] = new LARGE_INTEGER* [affairInfo.numTouch[i]]();

        mh_offsetStamp[i] = new HANDLE [affairInfo.numTouch[i]]();
        mh_dataStamp[i] = new HANDLE [affairInfo.numTouch[i]]();
        mmap_dataStamp[i] = new void* [affairInfo.numTouch[i]]();

        for (int j = 0; j < affairInfo.numTouch[i]; j++)
        { 
          LARGE_INTEGER fileSize;

          // offset
          fh_offsetStamp[i][j] = CreateFile((dir + "\\_offset_" + to_string(i) + "_" + to_string(j) + ".bin").data(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
          if (fh_offsetStamp[i][j] == 0 || fh_offsetStamp[i][j] == INVALID_HANDLE_VALUE) { return 2; }

          GetFileSizeEx(fh_offsetStamp[i][j], &fileSize);
          mh_offsetStamp[i][j] = CreateFileMapping(fh_offsetStamp[i][j], NULL, PAGE_READONLY, fileSize.HighPart, fileSize.LowPart, NULL);
          if (mh_offsetStamp[i][j] == 0 || mh_offsetStamp[i][j] == INVALID_HANDLE_VALUE) { return 2; }
          
          mmap_offsetStamp[i][j] = (LARGE_INTEGER*)MapViewOfFile(mh_offsetStamp[i][j], FILE_MAP_READ, 0, 0, 0);
          if (mmap_offsetStamp[i][j] == 0) { return 2; }
          
          // data
          fh_dataStamp[i][j] = CreateFile((dir + "\\_data_" + to_string(i) + "_" + to_string(j) + ".bin").data(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
          if (fh_dataStamp[i][j] == 0 || fh_dataStamp[i][j] == INVALID_HANDLE_VALUE) { return 2; }
          
          GetFileSizeEx(fh_dataStamp[i][j], &fileSize);
          mh_dataStamp[i][j] = CreateFileMapping(fh_dataStamp[i][j], NULL, PAGE_READONLY, 0, 0, NULL);
          if (mh_dataStamp[i][j] == 0 || mh_dataStamp[i][j] == INVALID_HANDLE_VALUE) { return 2; }
          
          mmap_dataStamp[i][j] = MapViewOfFile(mh_dataStamp[i][j], FILE_MAP_READ, 0, 0, 0);
          if (mmap_dataStamp[i][j] == 0) { return 2; }
        }        
      }      

      // tone
      fh_tone = CreateFile((dir + "\\_tone.bin").data(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
      if (fh_tone == 0 || fh_tone == INVALID_HANDLE_VALUE) { return 2; }

      LARGE_INTEGER fileSize;
      GetFileSizeEx(fh_tone, &fileSize);
      mh_tone = CreateFileMapping(fh_tone, NULL, PAGE_READONLY, fileSize.HighPart, fileSize.LowPart, NULL);
      if (mh_tone == 0 || mh_tone == INVALID_HANDLE_VALUE) { return 2; }

      mmap_tone = (float*)MapViewOfFile(mh_tone, FILE_MAP_READ, 0, 0, 0);
      if (mmap_tone == 0) { return 2; }
    }
    
    // tray
    if (true)
    {
      int lenTray = hunaInfo.numCert + 1 + hunaInfo.numIncert + hunaInfo.numAffair;

      tray = new int[lenTray]();
      tray[hunaInfo.numCert] = 1;
    }

    // fort
    if (true)
    {
      fort = new LARGE_INTEGER[1 + hunaInfo.numUniqueFort]();
    }

    // fox, foxScope
    if (true)
    {
      fox = new int* [hunaInfo.numAffair];
      foxScope = new int** [hunaInfo.numAffair];

      for (int i = 0; i < hunaInfo.numAffair; i++)
      {
        int lenBody = angleInfo.cycle[affairInfo.idxAngle[i]];
        fox[i] = new int[lenBody]();

        int lenScope = 2 * lenBody - 1;
        foxScope[i] = new int* [lenScope];
        _aimFox(i, lenBody);
      }
    }
  }

  return 0;
}

int priori_do_exit(int state)
{
  switch (state)
  {
  case LIVE:
    _clear();
    break;
  case READY:
    break;
  case LOAD:
    break;
  default:
    break;
  }

  return 0;
}

void priori_bufferAction(int* steer, int** input, int** output, int spb)
{
  // 1 : Set Certs on the Tray  
  memcpy(tray, steer, sizeof(int) * hunaInfo.numCert);  
  for (int i = 0; i < hunaInfo.numCert; i++)
  {
    if (tray[i] < 0)
    {
      tray[i] = 0;
    }
    else if (tray[i] > certInfo.max[i])
    {
      tray[i] = certInfo.max[i];
    }
  }
  
  // 2 : Make all B-Forts
  for (int i = 0; i < hunaInfo.numBfort; i++)
  {
    _cookFort(recipeBfort, i);
  }
  
  // 3 : Procss all Sample-Turns
  for (int i = 0; i < spb; i++)
  {
    // 3-A : Set Incerts on the Tray
    for (int j = 0; j < hunaInfo.numIncert; j++)
    {
      tray[hunaInfo.numCert + 1 + j] = input[j][i];
    }

    // 3-B : Play all Affairs
    for (int j = 0; j < hunaInfo.numAffair; j++)
    {
      _playAffair(j);
    }

    // 3-C : Test the Affairs

    for (int j = 0; j < hunaInfo.numTested; j++)
    {
      output[j][i] = tray[hunaInfo.numCert + 1 + hunaInfo.numIncert + j];
    }

    // 3-D : Turn the Angle
    for (int j = 0; j < hunaInfo.numUniqueSpan; j++)
    {
      angleInfo.val[j] =
        (angleInfo.val[j] + 1) % angleInfo.cycle[j];
    }
  }
}

//====----====----====----====----PRIVATE

// bufferAction 

LARGE_INTEGER _cookFort(RecipeFort recipe, int page)
{
  // 0 : Localize
  int _idxExFort = recipe.idxExFort[page];
  int _idxImFort = recipe.idxImFort[page];
  int _complexity = recipe.complexity[page];  
  
  // 1 : Load imfort val
  fort[_idxExFort].QuadPart = fort[_idxImFort].QuadPart;
  
  // 2 : Do all Spices
  for (int i = 0; i < _complexity; i++)
  {
    // 2-0 : Localize
    int _idxSpice = recipe.idxSpice[page][i];
    int _valWeight = recipe.valWeight[page][i];

    // 2-1 : Do the Spice
    fort[_idxExFort].QuadPart += tray[_idxSpice] * _valWeight;
  }

  return fort[_idxExFort];
}

void _playAffair(int idxAffair)
{
  // 0 : Localize
  int _valAngle = angleInfo.val[affairInfo.idxAngle[idxAffair]];
  int* _ptrTray = &tray[hunaInfo.numCert + 1 + hunaInfo.numIncert + affairInfo.idxTest[idxAffair]];

  // 1 : Apply all Touchs
  for (int i = 0; i < affairInfo.numTouch[idxAffair]; i++)
  {
    _applyTouch(idxAffair, i, _valAngle);
  }

  // 2 : Set Uncerts on the Tray
  *_ptrTray = *foxScope[idxAffair][_valAngle];
  *foxScope[idxAffair][_valAngle] = 0;
}

void _applyTouch(int idxAffair, int idxTouch, int _valAngle)
{  
  // 0 : Localize
  char* dataTouch = static_cast<char*>(mmap_dataStamp[idxAffair][idxTouch]);
  int ability = tray[hunaInfo.numCert + affairInfo.idxAbil[idxAffair][idxTouch]];
  
  // 1 : Cook Q-Fort

  LARGE_INTEGER qFort = _cookFort(recipeQfort, affairInfo.idxFort[idxAffair][idxTouch]);
  dataTouch +=
    (
      mmap_offsetStamp[idxAffair][idxTouch]
      + qFort.QuadPart
    )
    ->QuadPart;

  // 2 : Apply all Tones
  int numTone = *reinterpret_cast<int*>(dataTouch);
  for (int i = 0; i < numTone; i++)
  {
    LARGE_INTEGER offsetTone = *reinterpret_cast<LARGE_INTEGER*>(dataTouch + (i * 24) + 4);
    int lenTone = *reinterpret_cast<int*>(dataTouch + (i * 24) + 12);    
    int offsetPart = *reinterpret_cast<int*>(dataTouch + (i * 24) + 16);
    //cout << offsetPart << endl;
    float weight = *reinterpret_cast<float*>(dataTouch + (i * 24) + 20);
    
    float* toneData = mmap_tone + offsetTone.QuadPart;    
    for (int j = 0; j < lenTone; j++)
    {
      *foxScope[idxAffair][_valAngle + offsetPart + j]
        += *(toneData++) * weight * ability;
    }
  }
}

void _readChunk(int*& dest, int length, int* _mmap_bin, ptrdiff_t& _cur)
{
  dest = new int [length];
  memcpy(dest, _mmap_bin + _cur, length * sizeof(int));
  _cur += length;
}

void _readChunkChunk(int**& dest, int lengthRow, int* lengthCol, int* _mmap_bin, ptrdiff_t& _cur)
{
  dest = new int* [lengthRow];
  for (int i = 0; i < lengthRow; i++)
  {
    dest[i] = new int [lengthCol[i]];
    memcpy(dest[i], _mmap_bin + _cur, lengthCol[i] * sizeof(int));
    _cur += lengthCol[i];
  }
}

void _aimFox(int idxFox, int _lenBody)
{
  for (int i = 0; i < _lenBody - 1; i++)
  {
    foxScope[idxFox][i] = &fox[idxFox][i];
    foxScope[idxFox][_lenBody + i] = &fox[idxFox][i];
  }
  foxScope[idxFox][_lenBody - 1] = &fox[idxFox][_lenBody - 1];
}

// misc

int _check_hdr_binary(std::string path, int numSteer, int numInput, int numOutput)
{
  ifstream fi(path + "\\huna.bin", ios::binary);
  if (!fi.is_open()) { return 1; }

  int buffer[10];
  fi.read(reinterpret_cast<char*>(buffer), sizeof(int) * 10);
  fi.close();

  if (buffer[0] == 0x41'4E'55'48
    && buffer[9] == 0x54'52'45'43
    && buffer[1] == numSteer
    && buffer[2] == numInput
    && buffer[3] == numOutput)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

void _clear()
{
  //
  for (int i = 0; i < hunaInfo.numAffair; i++)
  {
    UnmapViewOfFile(mmap_dataStamp[i]);
    CloseHandle(mh_dataStamp[i]);
    CloseHandle(fh_dataStamp[i]);
    delete[] mmap_dataStamp[i];

    UnmapViewOfFile(mmap_offsetStamp[i]);
    CloseHandle(mh_offsetStamp[i]);
    CloseHandle(fh_offsetStamp[i]);
    delete[] mmap_dataStamp[i];

    delete[] fox[i];
    delete[] foxScope[i];
  }
  delete[] mmap_dataStamp;
  delete[] mh_dataStamp;
  delete[] fh_dataStamp;
  delete[] mmap_offsetStamp;
  delete[] mh_offsetStamp;
  delete[] fh_offsetStamp;
  delete[] foxScope;
  delete[] fox;

  //
  for (int i = 0; i < hunaInfo.numQfort; i++)
  {
    delete[] recipeQfort.valWeight;
    delete[] recipeQfort.idxSpice;
  }
  delete[] recipeQfort.complexity;
  delete[] recipeQfort.idxExFort;
  delete[] recipeQfort.idxImFort;

  //
  for (int i = 0; i < hunaInfo.numBfort; i++)
  {
    delete[] recipeBfort.valWeight;
    delete[] recipeBfort.idxSpice;
  }
  delete[] recipeBfort.complexity;
  delete[] recipeBfort.idxExFort;
  delete[] recipeBfort.idxImFort;

  //
  for (int i = 0; i < hunaInfo.numAffair; i++)
  {
    delete[] affairInfo.idxAbil[i];
  }
  delete[] affairInfo.idxTest;
  delete[] affairInfo.idxAngle;
  delete[] affairInfo.numTouch;

  //
  delete[] angleInfo.cycle;
  delete[] angleInfo.val;

  delete[] certInfo.max;
  delete[] certInfo.init;

  delete[] fort;
  delete[] tray;
}