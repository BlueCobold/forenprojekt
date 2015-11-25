//
//  PathHelper.cpp
//  RicketyRacquet
//
//  Created by Sebastian Kohl on 13.11.14.
//
//

#include "PathHelper.hpp"

#if defined(IOS) || defined(OSX)
#include "../MacHelper.hpp"
#elif defined(ANDROID)
#include "../AndroidHelper.hpp"
#endif

std::string resourcePath()
{
#ifdef WINDOWS
    return "";
#elif defined(IOS) || defined(OSX)
    return resourcePathApple();
#elif defined(ANDROID)
    return resourcePathAndroid();
#else
    return "/home/parallels/Desktop/Parallels Shared Folders/Host/Users/skohl1/Applications (Parallels)/ubuntu_share/forenprojekt/Source/FirstParty/Game/";
#endif
}

#ifdef ANDROID
std::string packedPath()
{
    return "";
}
#endif

std::string documentPath()
{
#ifdef IOS
    return documentPathIos();
#elif defined(ANDROID)
    return resourcePathAndroid();
#else
    return resourcePath();
#endif
}
