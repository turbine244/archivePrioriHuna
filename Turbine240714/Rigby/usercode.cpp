#include "Rigby.h"

//====----====----====----====----META

Info info
(
  "C:\\Users\\turbi\\AppData\\Roaming\\Turbine\\pri\\rigby",
  "mereDelay_level_time"
);

Tone bypass(WAV, "C:\\Users\\turbi\\Documents\\On a Star.wav");

//====----====----====----====----FACE

X x_0("Input");
Y y_0("Output");

Steer st_level("level", 4, 2);
Steer st_time("time", 5, 3);

//====----====----====----====----BODY

Buffer echoMemory;

Param par_level({ st_level });
Param par_time({ st_time });

void fx()
{ 
  par_level.set(st_level.get() * 0.25);
  par_time.set(st_time.get() * 200 * MSEC);

  echoMemory.touch(x_0, bypass, AUTO, 0, par_level);
  y_0.touch(x_0, bypass, AUTO, 0, 1.00);
  y_0.touch(echoMemory, bypass, AUTO, par_time, 1.00);
}