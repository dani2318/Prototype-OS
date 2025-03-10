#include "Debug.hpp"

#define MAX_OUT_DEV 10

namespace {

  static const char* const g_LogSeverity[] = {
    [static_cast<int>(Debug::DebugLevel::LVL_DEBUG)]      = "\033[2;37m",
    [static_cast<int>(Debug::DebugLevel::LVL_INFO)]       = "\033[37m",
    [static_cast<int>(Debug::DebugLevel::LVL_WARN)]       = "\033[1;33m",
    [static_cast<int>(Debug::DebugLevel::LVL_ERROR)]      = "\033[1;31m",
    [static_cast<int>(Debug::DebugLevel::LVL_CRITICAL)]   = "\033[1;37;41m"
  };

  static const char* const ResetColor = "\033[0;37m";

  struct
  {
    TextDevice* device;
    Debug::DebugLevel loglevel;
    bool colored;
  } OutputDevices[MAX_OUT_DEV];

  int OutDevCount;
}

namespace Debug
{


  void AddOutputDevice(TextDevice* device, DebugLevel minLoglevel, bool colorOutput){
    OutputDevices[OutDevCount].device = device;
    OutputDevices[OutDevCount].loglevel = minLoglevel;
    OutputDevices[OutDevCount].colored = colorOutput;
    OutDevCount++;
  }

  static void Log( const char* module,const char* fmt, DebugLevel logLevel, va_list args){

    for (int i = 0; i < OutDevCount; i++){
      if (logLevel < OutputDevices[i].loglevel)
        continue;

      if(OutputDevices[i].colored){
        OutputDevices[i].device->Write(g_LogSeverity[static_cast<int>(logLevel)]);
      }
      OutputDevices[i].device->Format("[%s]",module);
      OutputDevices[i].device->VFormat(fmt, args);

      if(OutputDevices[i].colored){
        OutputDevices[i].device->Write(ResetColor);
      }
      OutputDevices[i].device->Write("\r\n");
    }
  }

  void Debug(const char* module, const char* fmt, ...){
    va_list args;
    va_start(args,fmt);
    Log(module,fmt,DebugLevel::LVL_DEBUG,args);
    va_end(args);
  }

  void Info(const char* module, const char* fmt, ...){
    va_list args;
    va_start(args,fmt);
    Log(module,fmt,DebugLevel::LVL_INFO,args);
    va_end(args);
  }

  void Warn(const char* module, const char* fmt, ...){
    va_list args;
    va_start(args,fmt);
    Log(module,fmt,DebugLevel::LVL_WARN,args);
    va_end(args);
  }

  void Error(const char* module, const char* fmt, ...){
    va_list args;
    va_start(args,fmt);
    Log(module,fmt,DebugLevel::LVL_ERROR,args);
    va_end(args);
  }

  void Critical(const char* module, const char* fmt, ...){
    va_list args;
    va_start(args,fmt);
    Log(module,fmt,DebugLevel::LVL_CRITICAL,args);
    va_end(args);
  }
} // namespace Debug
