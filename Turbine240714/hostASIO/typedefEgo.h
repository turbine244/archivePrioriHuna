#ifndef TYPEDEF_EGO
#define TYPEDEF_EGO

typedef void (*BUFFERACTION) (int* steer, int** input, int** output, int spb);

typedef struct EgoInfo
{
  int numSteer;
  int numInput;
  int numOutput;
  int spb;

  int* steer;
  int*** ptrInput;
  int*** ptrOutput;
  BUFFERACTION bufferAction;
};

#endif