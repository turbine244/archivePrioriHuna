#ifndef ENUM_RIGBY
#define ENUM_RIGBY

enum
{
  // Process State

  INIT = 0,
  PREP,
  LIVE,

  // Measure

  SPS = 48000,
  SEC = 48000,
  MSEC = 48,

  // Tone source type

  BINARY = 0,
  WAV,

  // Tone length

  AUTO = -1,
};

#endif