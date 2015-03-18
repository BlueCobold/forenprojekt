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

void showErrorMac(const std::string& msg)
{
#ifdef IOS
#else
    @autoreleasepool
    {
        auto alert = [[NSAlert alloc] init];
        [alert setMessageText:[NSString stringWithCString:msg.c_str() encoding:[NSString defaultCStringEncoding]]];
        [alert runModal];
    }
#endif
}
