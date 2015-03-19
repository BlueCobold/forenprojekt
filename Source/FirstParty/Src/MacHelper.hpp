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

std::string resourcePathMac();

void showErrorMac(const std::string& msg);

#ifdef IOS
float iosContentScaleFactor();
#endif

#endif // RicketyRacquet_MacHelper_hpp
