# DebufferSystem

```
#ifdef _DEBUG
	#pragma comment(lib, "libs/debug/DebufferSystem.lib")
#else
	#pragma comment(lib, "libs/release/DebufferSystem.lib")
#endif

//Defines
#define MAX_BUFFER		8196
typedef UCHAR			t_Buffer[MAX_BUFFER];
#define MAX_SHA			20
typedef UCHAR			t_hashbuf[MAX_SHA];
#define KEY_BUF			36
typedef UCHAR			t_keyhash[KEY_BUF];

#include "libs/DebufferSystem.h"
```
