#pragma once

struct PARAMETERS
{
  HWND window;
  char* sendingData;
};
struct TextFields {
  HWND defaultText;
  HWND sortedText;
};
struct GeneratorParams
{
  char**& strMatr;
  TextFields fields;
};