#ifndef _Constant_h
#define _Constant_h
#include "arduino.h"

class Constant {
  public:
    Constant();
    char* ssid;
    char* password;
    String token;
};

#endif
