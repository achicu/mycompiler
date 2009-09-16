#ifndef PLATFORM_H
#define PLATFORM_H

#define PLATFORM(platform) (defined( PLATFORM_##platform ) && PLATFORM_##platform)

#if defined(WIN32) || defined(_WIN32)
#define PLATFORM_WIN 1
#define _CRT_SECURE_NO_WARNINGS 1
#include "stdint.h"
#endif

#if __APPLE__
#define PLATFORM_MAC 1
#endif

#endif