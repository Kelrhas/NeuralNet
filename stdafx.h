// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <algorithm>

#define TODO __debugbreak()


namespace Debug
{
	static bool s_bOutput = false;
	static unsigned int s_uDebugSeed = 7;
}


// TODO: reference additional headers your program requires here
