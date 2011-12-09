#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#ifdef _WIN32
# include <windows.h>
#else
# error "define getTickCount"
#endif
