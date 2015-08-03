//
//  MacHelper.m
//  RicketyRacquet
//
//  Created by Sebastian Kohl on 12.11.14.
//
//

#import <Foundation/Foundation.h>
#ifdef IOS
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif

#import "MacHelper.hpp"

namespace utility
{
    std::string translateKey(std::string key);
}

void minimize(void* handle){
#ifdef IOS
#else
    NSWindow* window = (NSWindow*)handle;
    [window setIsMiniaturized:YES];
#endif
}

void maximize(void* handle){
#ifdef IOS
#else
    NSWindow* window = (NSWindow*)handle;
    [window setIsMiniaturized:NO];
#endif
}

std::string resourcePathMac()
{
#ifdef _DEBUG
    return "";
#endif
    std::string path;
    @autoreleasepool
    {
        if(auto bundle = [NSBundle mainBundle])
        {
            NSString* resourcePath = [bundle resourcePath];
            path = [resourcePath UTF8String] + std::string("/");
        }
    }
    return path;
}

#ifdef IOS
float iosContentScaleFactor()
{
    static bool inited = false;
    static float scaleFactor = 1;
    if(!inited)
    {
        id data = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"NSHighResolutionCapable"];
        if(data && [data boolValue])
            scaleFactor = [[UIScreen mainScreen] scale];
        inited = true;
    }
    return scaleFactor;
}

std::string documentPathIos()
{
    std::string path;
    @autoreleasepool
    {
        if(NSString* dir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0])
            path = [dir UTF8String] + std::string("/");
        else
            return "";
    }
    return path;
}
#endif

void showErrorMac(const std::string& msg)
{
#ifdef IOS
    @autoreleasepool {
        auto title = [NSString stringWithCString:utility::translateKey("Error").c_str()  encoding:[NSString defaultCStringEncoding]];
        auto message = [NSString stringWithCString:msg.c_str() encoding:[NSString defaultCStringEncoding]];
        auto button = [NSString stringWithCString:utility::translateKey("gui_alert_button_ok").c_str() encoding:[NSString defaultCStringEncoding]];
        auto alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:nil cancelButtonTitle:button otherButtonTitles: nil];
        [alert show];
    }
#else
    @autoreleasepool
    {
        auto alert = [[NSAlert alloc] init];
        [alert setMessageText:[NSString stringWithCString:msg.c_str() encoding:[NSString defaultCStringEncoding]]];
        [alert runModal];
    }
#endif
}
