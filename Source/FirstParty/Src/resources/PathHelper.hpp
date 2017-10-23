#pragma once

#ifndef RicketyRacquet_PathHelper_hpp
#define RicketyRacquet_PathHelper_hpp

#include <string>

std::string resourcePath();

#ifdef ANDROID
std::string packedPath();
#else
#define packedPath resourcePath
#endif

std::string documentPath();

#endif
