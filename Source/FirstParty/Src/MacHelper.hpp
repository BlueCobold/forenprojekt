//
//  MacHelper.hpp
//  RicketyRacquet
//
//  Created by Sebastian Kohl on 12.11.14.
//
//

#ifndef RicketyRacquet_MacHelper_hpp
#define RicketyRacquet_MacHelper_hpp

#include <string>

void minimize(void* handle);
void maximize(void* handle);

std::string resourcePathApple();

void showErrorApple(const std::string& msg);

std::string defaultUserNameApple();

#ifdef IOS
float iosContentScaleFactor();

std::string documentPathIos();
#endif

#endif // RicketyRacquet_MacHelper_hpp
