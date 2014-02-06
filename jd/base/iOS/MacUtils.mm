#include "stdafx.h"

#include <cstdarg>

#include <jd/base/log.h>

#include <string>
#include <sys/time.h>

#import <mach/mach.h>
#import <mach/mach_host.h>

using namespace std;


//unsigned int GetSystemTimeTick()
//{
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//  return tv.tv_usec/1000 + tv.tv_sec*1000;
//}

@implementation NSArray(JD)

-(id)firstObjectOfClass:(Class)theClass
{
    for( NSObject * obj in self )
    {
        if ( [obj isKindOfClass:theClass] )
        {
            return obj;
        }
    }
    return nil;
}

@end

static NSString * sAppBundleId = nil;

NSString * GetMainBundleIdentifier()
{
    if ( !sAppBundleId )
    {
        NSDictionary * infoDict = [[NSBundle mainBundle] infoDictionary];
        sAppBundleId = [infoDict valueForKey:@"CFBundleIdentifier"];
        ASSERT( sAppBundleId && sAppBundleId.length > 0 );
    }
    
    return sAppBundleId;
}

NSString * GetMacAppVersionString()
{
    NSDictionary * infoDict = [NSBundle mainBundle].infoDictionary;
    NSString * versionStr = [infoDict valueForKey:@"CFBundleVersion"];
    return versionStr;
}

NSString * StringForBoolean( BOOL value )
{
    return value ? @"YES" : @"NO";
}


NSString * StringForUserInterfaceIdiom( UIUserInterfaceIdiom idiom )
{
    switch ( idiom )
    {
        case UIUserInterfaceIdiomPhone:     return @"Phone";  break;
        case UIUserInterfaceIdiomPad:       return @"Pad";    break;
    }
    return @"(unknown)";
}

EXTERN_C_BEGIN
	
bool IsIPad()
{
	return UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPad;
}

void iOSPrintDeviceInfo()
{
    UIDevice * device = [UIDevice currentDevice];
    BOOL multitaskingSupported = NO;
    if ([device respondsToSelector:@selector(isMultitaskingSupported)])
    {
        multitaskingSupported = device.multitaskingSupported;
    }
    
    NSLOG(@"=================");
    NSLOG(@"");
    
    NSLOG(@"iOS UIDevice Info");
    // linking against UIDevice uniqueIdentifier causes app rejection (2013.05.17)
    NSLOG(@"  uniqueIdentifier: (no longer publicly available)");
    //NSLOG(@"  uniqueIdentifier: %@", device.uniqueIdentifier);
    NSLOG(@"  name: %@", device.name);
    NSLOG(@"  systemName: %@", device.systemName);
    NSLOG(@"  systemVersion: %@", device.systemVersion);
    NSLOG(@"  model: %@", device.model );
    NSLOG(@"  localizedModel: %@", device.localizedModel );
    NSLOG(@"  multitaskingSupported: %@", StringForBoolean(multitaskingSupported));
    NSLOG(@"  userInterfaceIdiom: %@", StringForUserInterfaceIdiom(device.userInterfaceIdiom) );
    NSLOG(@"  ");
    
    // NSProcessInfo
    NSProcessInfo * proc = [NSProcessInfo processInfo];
    NSLOG(@"NSProcessInfo (process)");
    NSLOG(@"  processName: %@", [proc processName] );
    NSLOG(@"  arguments: %@", [proc arguments] );
    //NSLOG(@"  environment: %@", [proc environment] );  // verbose
    NSLOG(@"  processId: %d", [proc processIdentifier] );
    NSLOG(@"  globallyUniqueString: %@", [proc globallyUniqueString] );
    NSLOG(@"  ");
    
    // host info (from NSProcessInfo)
    //NSLOG(@"NSProcessInfo (host)");
    //NSLOG(@"  hostName: %@", [proc hostName] );   // blocks until host resolved!!!
    //NSLOG(@"  operatingSystemName: %@", [proc operatingSystemName] );
    //NSLOG(@"  operatingSystemVersionString: %@", [proc operatingSystemVersionString] );
    //NSLOG(@"  ");
    
    // computer info (from NSProcessInfo)
    NSLOG(@"NSProcessInfo (computer info)");
    NSLOG(@"  processorCount: %d", [proc processorCount] );
    NSLOG(@"  activeProcessorCount: %d", [proc activeProcessorCount] );
    NSLOG(@"  physicalMemory: %qu bytes (%.3f MiB)", [proc physicalMemory], [proc physicalMemory]/1000000.0 );
    NSLOG(@"  systemUptime: %.2f hours", [proc systemUptime] / (60.0 * 60.0) ); // oh well, NSDate doesn't seem to print relative times
    NSLOG(@"  ");
    
    // iPhone 4 / hi res display support:
    UIScreen * screen = [UIScreen mainScreen];
    float ui_w = screen.bounds.size.width;
    float ui_h = screen.bounds.size.height;
    NSLOG(@"UIScreen info");
    NSLOG(@"  Pixel scale: %.3f", screen.scale);
    NSLOG(@"  Pixel dimensions: (%d x %d)", (int)(ui_w*screen.scale), (int)(ui_h*screen.scale) );
    NSLOG(@"  UI Dimensions: (%d x %d)", (int)ui_w, (int)ui_h ); 
    NSLOG(@"  ");
    
    NSLOG(@"=================");
    NSLOG(@"");
}

//Snippet from http://landonf.bikemonkey.org/2008/12/06
unsigned int iOSGetFreeMemory ()
{
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;
    
    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);        
    
    vm_statistics_data_t vm_stat;
    
    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS)
    {
        LOG("Failed to fetch vm statistics");
    }
    
    /* Stats in bytes */ 
    natural_t mem_free = vm_stat.free_count * pagesize;
    //natural_t mem_total = mem_used + mem_free;
    //natural_t mem_used = (vm_stat.active_count + vm_stat.inactive_count + vm_stat.wire_count) * pagesize;
    // LOG("Mem used: %u free: %u total: %u", mem_used, mem_free, mem_total);
    return mem_free;
}

EXTERN_C_END
