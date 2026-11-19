#ifndef HUNA_STRUCT
#define HUNA_STRUCT

#include <string>

typedef struct HunaInfo
{
  int numCert;
  int numIncert;
  int numTested;
  int numAffair;
  int numBfort;
  int numQfort;
  int numUniqueSpan;
  int numUniqueFort;
};

typedef struct CertInfo
{
  int* init; // [numCert]
  int* max; // [numCert]
};

typedef struct AngleInfo
{
  int* val;  // [numUniqueSpan]
  int* cycle; // [numUniqueSpan]
};

typedef struct AffairInfo
{
  int* numTouch;  // [numAffair]
  int* idxAngle;  // [numAffair]
  int* idxTest;  // [numAffair]
  int** idxFort;  // [numAffair] * [numTouch[i]]
  int** idxAbil;  // [numAffair] * [numTouch[i]]
};

typedef struct RecipeFort
{
  int* idxImFort; // [num?fort]
  int* idxExFort; // [num?fort]  
  int* complexity; // [num?fort]
  int** idxSpice; // [num?fort] * [complexity[i]]
  int** valWeight; // [num?fort] * [complexity[i]]
};

#endif