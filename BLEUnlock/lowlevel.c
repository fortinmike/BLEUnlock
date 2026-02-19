#include "lowlevel.h"
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/pwr_mgt/IOPMLib.h>
#include <IOKit/IOKitLib.h>

static int assertionCountForType(CFDictionaryRef assertions, CFStringRef assertionType)
{
    CFNumberRef countNumber = (CFNumberRef)CFDictionaryGetValue(assertions, assertionType);
    if (!countNumber) return 0;

    int count = 0;
    if (!CFNumberGetValue(countNumber, kCFNumberIntType, &count)) return 0;
    return count;
}

void wakeDisplay(void)
{
    static IOPMAssertionID assertionID;
    IOPMAssertionDeclareUserActivity(CFSTR("BLEUnlock"), kIOPMUserActiveLocal, &assertionID);
}

void sleepDisplay(void)
{
    io_registry_entry_t reg = IORegistryEntryFromPath(kIOMasterPortDefault, "IOService:/IOResources/IODisplayWrangler");
    if (reg) {
        IORegistryEntrySetCFProperty(reg, CFSTR("IORequestIdle"), kCFBooleanTrue);
        IOObjectRelease(reg);
    }
}

bool isDisplaySleepPrevented(void)
{
    CFDictionaryRef assertions = NULL;
    if (IOPMCopyAssertionsStatus(&assertions) != kIOReturnSuccess || assertions == NULL) return false;

    int count = assertionCountForType(assertions, CFSTR("PreventUserIdleDisplaySleep"));
    if (count == 0) count = assertionCountForType(assertions, CFSTR("NoDisplaySleepAssertion"));
    CFRelease(assertions);
    return count > 0;
}
