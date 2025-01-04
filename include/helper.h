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

/*
Posible log levels:
  4 - DEBUG - shows all logs
  3 - INFO - shows only info, warnings and error logs
  2 - WARNING - shows only warnings and errors
  1 - ERROR - shows only errors (default after 1.0)
  0 - NONE - shows no logs
*/

#ifndef GLOBAL_UI_LOG_LEVEL
  #define GLOBAL_UI_LOG_LEVEL 4
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

inline void slog(String file, int line, String functionname, String messageType, String message, String sender){
  String msg;
  msg = "["+file+":"+line+":<"+functionname+">] {"+sender+"} "+messageType+": "+message;
  Serial.println(msg);
};



#define Slog(message) slog(__FILE__,__LINE__,__FUNCTION_NAME__,message);
#define S_log(message, sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,message, sender);



#if (GLOBAL_UI_LOG_LEVEL == 4)
  #define UI_ERROR(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"ERROR",message,sender);
  #define UI_WARNING(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"WARNING",message,sender);
  #define UI_INFO(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"INFO",message,sender);
  #define UI_DEBUG(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"DEBUG",message,sender);
#elif (GLOBAL_UI_LOG_LEVEL == 3)
  #define UI_ERROR(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"ERROR",message,sender);
  #define UI_WARNING(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"WARNING",message,sender);
  #define UI_INFO(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"INFO",message,sender);
  #define UI_DEBUG(message,sender)
#elif (GLOBAL_UI_LOG_LEVEL == 2)
  #define UI_ERROR(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"ERROR",message,sender);
  #define UI_WARNING(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"WARNING",message,sender);
  #define UI_INFO(message,sender)
  #define UI_DEBUG(message,sender)
#elif (GLOBAL_UI_LOG_LEVEL == 1)
  #define UI_ERROR(message,sender) slog(__FILE__,__LINE__,__FUNCTION_NAME__,"ERROR",message,sender);
  #define UI_WARNING(message,sender)
  #define UI_INFO(message,sender)
  #define UI_DEBUG(message,sender)
#else
  #define UI_ERROR(message,sender)
  #define UI_WARNING(message,sender)
  #define UI_INFO(message,sender)
  #define UI_DEBUG(message,sender)
#endif





