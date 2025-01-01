#pragma once
#include <Arduino.h>
#include <vector>

#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif
#endif


inline void slog(String file, int line, String functionname, String message){
  String msg;
  msg = "["+file+":"+line+":<"+functionname+">] "+message;
  Serial.println(msg);
};

inline void slog(String file, int line, String functionname, String message, String sender){
  String msg;
  msg = "["+file+":"+line+":<"+functionname+">] {"+sender+"} "+message;
  Serial.println(msg);
};



#define Slog(message) slog(__FILE__,__LINE__,__FUNCTION_NAME__,message);
#define S_log(message, sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,message, sender);


