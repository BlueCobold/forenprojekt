//
//  PathHelper.cpp
//  RicketyRacquet
//
//  Created by Sebastian Kohl on 13.11.14.
//
//

#include "PathHelper.hpp"

#ifndef WINDOWS
#include "../MacHelper.hpp"
#endif

std::string resourcePath()
{
#ifdef WINDOWS
    return "";
#elif defined(IOS) || defined(OSX)
    return resourcePathApple();
#else
    return "/home/parallels/Desktop/Parallels Shared Folders/Host/Users/skohl1/Applications (Parallels)/ubuntu_share/forenprojekt/Source/FirstParty/Game/";
#endif
}

std::string documentPath()
{
#ifdef IOS
    return documentPathIos();
#else
    return resourcePath();
#endif
}
