#pragma once
#include <plugin.hpp>

#define VER_MAJOR 0
#define VER_MINOR 0
#define VER_REVIS 2
#define VER_BUILD 188
#define VER_STAGE VS_ALPHA
#define VER_INTER MAKEFARVERSION(VER_MAJOR,VER_MINOR,VER_REVIS,VER_BUILD,VER_STAGE)

#define _W(arg) L##arg
#define _STR(arg) _W(#arg)
#define STR(arg) _STR(arg)

#ifdef _WIN64
#define PLATFORM L"x64"
#else
#define PLATFORM L"x86"
#endif

#define PLUGIN_VERSION STR(VER_MAJOR) L"." STR(VER_MINOR) L"." STR(VER_REVIS) L" build " STR(VER_BUILD) L" " PLATFORM
#define PLUGIN_NAME L"FarTcl"
#define PLUGIN_DESC L"FarTcl for Far Manager"
#define PLUGIN_FILENAME L"FarTcl.dll"
#define PLUGIN_AUTHOR L"Konstantin Kushnir <chpock@gmail.com>"
