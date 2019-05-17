#pragma once
#ifndef __EXPORT
#define __dll __declspec(dllimport)
#else
#define __dll __declspec(dllexport)
#endif