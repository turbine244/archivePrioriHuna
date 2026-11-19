#ifndef CLI_TOOL
#define CLI_TOOL

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>

#include "enumEgo.h"

class CliTool
{
public:
  void flush();
  void setCursor(DWORD ptSize, BOOL flag);
  void setColor(int fore, int back);
  void gotoxy(SHORT x, SHORT y);
  void printLine(std::string str, int indent, bool br);
  void br();
  char waitKey(SHORT x, SHORT y, const char* list);
  std::vector<std::string> split(std::string input, std::string delimiter);

private:
  int colorFore;
  int colorBack;
};

#endif