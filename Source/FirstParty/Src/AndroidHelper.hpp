#pragma once

#ifndef ANDROID_HELPER_HPP
#define ANDROID_HELPER_HPP

#ifdef ANDROID

#include <string>

void init();
std::string resourcePathAndroid();
void showErrorAndroid(const std::string& msg);
int getMemoryClass();

#endif

#endif // ANDROID_HELPER_HPP
