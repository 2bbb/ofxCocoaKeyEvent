//
//  ofxMacKeyboard.mm
//
//  Created by TORU URAKAWA on 2014/04/10.
//
//

#include "ofxCocoaKeyEvent.h"

void dumpKeyboardLayouts() {
    {
        TISInputSourceRef source = TISCopyCurrentKeyboardLayoutInputSource();
        NSString *sourceID = (__bridge NSString *)(TISGetInputSourceProperty(source, kTISPropertyInputSourceID));
        NSString *localizedName = (__bridge NSString *)(TISGetInputSourceProperty(source, kTISPropertyLocalizedName));
        
        NSLog(@"[keyboard layout] * %@ : %@", localizedName, sourceID);
        CFRelease(source);
    }
    
    NSDictionary *ref = @{
        (NSString *)kTISPropertyInputSourceType : (NSString *)kTISTypeKeyboardLayout
    };
    
    CFArrayRef sourceList = TISCreateInputSourceList ((__bridge CFDictionaryRef)(ref),true);
    for(int i = 0; i < CFArrayGetCount(sourceList); i++) {
        
        TISInputSourceRef source = (TISInputSourceRef)(CFArrayGetValueAtIndex(sourceList, i));
        
        NSString* sourceID = (__bridge NSString *)(TISGetInputSourceProperty(source, kTISPropertyInputSourceID));
        NSString* localizedName = (__bridge NSString *)(TISGetInputSourceProperty(source, kTISPropertyLocalizedName));
        
        NSLog(@"[keyboard layout]   %@ : %@", localizedName, sourceID);
        if(10 < i) {
            NSLog(@"[keyboard layout]   and other %ld keyboard layouts exists", CFArrayGetCount(sourceList) - i);
            break;
        }
    }
}

ofxCocoaKeyEvent::ofxCocoaKeyEvent() {
    dumpKeyboardLayouts();
    eventSource = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
}

ofxCocoaKeyEvent::~ofxCocoaKeyEvent() {
    CFRelease(eventSource);
}
