// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <windows.h>
#include <string>

//Additional defines
#define MAX_BUFFER		8196
typedef UCHAR			t_Buffer[MAX_BUFFER];
#define MAX_SHA			20
typedef UCHAR			t_hashbuf[MAX_SHA];
#define KEY_BUF			36
typedef UCHAR			t_keyhash[KEY_BUF];

// reference additional headers your program requires here
#include "DebufferSystem.h"
