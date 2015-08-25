#pragma once

#ifndef MAC_HELPER_HPP
#define MAC_HELPER_HPP

#include <string>

#if defined(IOS) || defined(OSX)
std::string resourcePathApple();

void showErrorApple(const std::string& msg);

std::string defaultUserNameApple();
#endif

#ifdef IOS
float iosContentScaleFactor();

std::string documentPathIos();
#endif

#ifdef OSX
void minimize(void* handle);

void maximize(void* handle);

bool pickFileMac(std::string& path);
#endif

#endif // MAC_HELPER_HPP
