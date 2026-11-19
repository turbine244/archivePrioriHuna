#include "fx.h"
using namespace std;

ofstream fo;

void writeHeader(int* hdr)
{
  const int top = -1;

  fo.open("huna.bin", ios::binary);  
  fo.write(reinterpret_cast<const char*>(&top), sizeof(int));
  fo.write(reinterpret_cast<const char*>(hdr), 8 * sizeof(int));
  fo.close();
}

void create_monitor()
{
  writeHeader();

  fo.open("offset_0_0.bin", ios::binary);
  LARGE_INTEGER list0[] = { 0 };
  size_t len0 = sizeof(list0) / sizeof(list0[0]);
  fo.write(reinterpret_cast<const char*>(list0), len0 * sizeof(LARGE_INTEGER));
  fo.close(); 

  fo.open("data_0_0.bin", ios::binary);
  float list1[] = { 1, 1.00 };
  size_t len1 = sizeof(list1) / sizeof(list1[0]);
  fo.write(reinterpret_cast<const char*>(list1), len1 * sizeof(float));
  fo.close();


}