
#import "MacHelper.hpp"

#import "BatteryState.hpp"

#import <Foundation/Foundation.h>

#ifdef IOS
#import  <UIKit/UIKit.h>
#elif defined(OSX)
#import <AppKit/AppKit.h>
#import <IOKit/ps/IOPowerSources.h>
#import <IOKit/ps/IOPSKeys.h>
#endif

namespace utility
{
    std::string translateKey(std::string key);
}

#if defined(IOS) || defined(OSX)

std::string resourcePathApple()
{
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

std::string defaultUserNameApple()
{
#ifdef IOS
    @autoreleasepool
    {
        return std::string([[UIDevice currentDevice].name UTF8String]);
    }
#elif defined(OSX)
    @autoreleasepool
    {
        return std::string([NSUserName() UTF8String]);
    }
#endif
}

void showErrorApple(const std::string& msg)
{
#ifdef IOS
    @autoreleasepool
    {
        auto title = [NSString stringWithCString:utility::translateKey("Error").c_str()  encoding:[NSString defaultCStringEncoding]];
        auto message = [NSString stringWithCString:msg.c_str() encoding:[NSString defaultCStringEncoding]];
        auto button = [NSString stringWithCString:utility::translateKey("gui_alert_button_ok").c_str() encoding:[NSString defaultCStringEncoding]];
        auto alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:nil cancelButtonTitle:button otherButtonTitles: nil];
        [alert show];
    }
#elif defined(OSX)
    @autoreleasepool
    {
        auto alert = [[NSAlert alloc] init];
        [alert setMessageText:[NSString stringWithCString:msg.c_str() encoding:[NSString defaultCStringEncoding]]];
        [alert runModal];
    }
#endif
}

#endif

#ifdef IOS

BatteryState getBatteryStateImpl()
{
    BatteryState state;
    if(![UIDevice currentDevice].batteryMonitoringEnabled)
        [UIDevice currentDevice].batteryMonitoringEnabled = YES;
    
    swith([UIDevice currentDevice].batteryState)
    {
    case UIDeviceBatteryStateUnknown:
        state.state = BatteryState::Unknown;
        break;
        
    case UIDeviceBatteryStateUnplugged:
        state.state = BatteryState::Unplugged;
        break;
        
    case UIDeviceBatteryStateCharging:
        state.state = BatteryState::Charging;
        break;
        
    case UIDeviceBatteryStateFull:
        state.state = BatteryState::Full;
        break;
    }
    state.percent = [UIDevice currentDevice].batteryLevel;
    return state;
}

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

#ifdef OSX

BatteryState getBatteryStateImpl()
{
    auto blob = IOPSCopyPowerSourcesInfo();
    auto sources = IOPSCopyPowerSourcesList(blob);
    
    int numOfSources = CFArrayGetCount(sources);
    if (numOfSources == 0)
        return BatteryState();
    
    for (int i = 0 ; i < numOfSources ; i++)
    {
        auto pSource = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(sources, i));
        if(!pSource)
            return BatteryState();
        
        int curCapacity = 0;
        auto psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSCurrentCapacityKey));
        CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &curCapacity);
        
        int maxCapacity = 0;
        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSMaxCapacityKey));
        CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &maxCapacity);
        
        BatteryState state;
        state.percent = 100.f * curCapacity / maxCapacity;
        
        int isFull = 0;
        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSIsChargedKey));
        if(psValue)
            CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &isFull);
        
        if(isFull)
            state.state = BatteryState::Full;
        else
        {
            psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSPowerSourceStateKey));
            if(psValue)
            {
                char strBuff[100];
                CFStringGetCString((CFStringRef)psValue, strBuff, sizeof(strBuff), kCFStringEncodingUTF8);
                std::string strState(strBuff);
                state.state = strState == "AC Power" ? BatteryState::Charging : BatteryState::Unplugged;
            }
        }

        return state;
    }

    return BatteryState();
}

void minimize(void* handle){
    NSWindow* window = (NSWindow*)handle;
    [window setIsMiniaturized:YES];
}

void maximize(void* handle){
    NSWindow* window = (NSWindow*)handle;
    [window setIsMiniaturized:NO];
}

bool pickFileMac(std::string& path)
{
    @autoreleasepool
    {
        NSOpenPanel *panel = [NSOpenPanel openPanel];
        [panel setCanChooseFiles:YES];
        [panel setCanChooseDirectories:NO];
        [panel setAllowsMultipleSelection:NO];
        
        if ([panel runModal] == NSFileHandlingPanelOKButton) {
            for (NSURL *url in [panel URLs]) {
                path = std::string([[url path] UTF8String]);
                return true;
            }
        }
        return false;
    }
}

#endif
