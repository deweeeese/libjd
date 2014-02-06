#pragma once

#if defined __OBJC__
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

void NSQuietLog(NSString *format, ...);
NSString * GetMacAppVersionString();
NSString * GetMainBundleIdentifier();

NSString * StringForBoolean( BOOL value );
NSString * StringForUserInterfaceIdiom( UIUserInterfaceIdiom idiom );

// NSArray util: get first object of given class in an array
@interface NSArray(JD)
-(id)firstObjectOfClass:(Class)theClass;
@end

typedef void (^CompletionBlock)(BOOL didNotCancel);
typedef void (^EmptyBlock)();
typedef void (^UIViewBlock)(UIView * view);

#endif

EXTERN_C_BEGIN

bool IsIPad();

void iOSPrintDeviceInfo();
unsigned int iOSGetFreeMemory();

EXTERN_C_END