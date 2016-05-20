//
//  ofxCocoaKeyEvent.h
//
//  Created by TORU URAKAWA on 2014/04/10.
//
//

#pragma once

#include "ofMain.h"
#include <Carbon/Carbon.h>

enum KeyState {
    kDOWN = true,
    kUP = false,
};

class ofxCocoaKeyEvent {
    CGEventSourceRef eventSource;
    
    // https://github.com/percontation/public/blob/master/keystroke.c
    struct KeyCode {
        CGKeyCode code;
        bool isPressShift;
    };
    
public:
    ofxCocoaKeyEvent();
    ~ofxCocoaKeyEvent();
    
    void send(char c, KeyState k) {
        send(c, k, 0);
    }

    void send(int key, KeyState k) {
        CGKeyCode code = getCodeFromOFKey(key);
        CGEventRef ev = CGEventCreateKeyboardEvent(eventSource, code, k);
        CGEventPost(kCGHIDEventTap, ev);
        CFRelease(ev);
    }
    
    void send(char c, KeyState k, int num, ...) {
        KeyCode code = keyCodeForAscii(c);
        CGEventRef ev = CGEventCreateKeyboardEvent(eventSource, code.code, k);
        
        va_list args;
        va_start(args, num);
        
        uint64_t flags = k ? 0 : CGEventGetFlags(ev);
        for (int i = 0; i < num; i++) {
            int key = va_arg(args , int);
            uint64_t flag = getFlagFromOFKey(key);
            if (k == kDOWN) flags |= flag;
            else flags &= ~flag;
        }
        
        if(code.isPressShift) {
            if (k == kDOWN) flags |= kCGEventFlagMaskShift;
            else flags &= ~kCGEventFlagMaskShift;
        }
        
        CGEventSetFlags(ev, static_cast<CGEventFlags>(flags));
        
        CGEventPost(kCGHIDEventTap, ev);
        CFRelease(ev);
    }
    
    void send(int key, KeyState k, int num, ...) {
        CGKeyCode code = getCodeFromOFKey(key);
        CGEventRef ev = CGEventCreateKeyboardEvent(eventSource, code, k);
        
        va_list args;
        va_start(args, num);
        
        uint64_t flags = k ? 0 : CGEventGetFlags(ev);
        for (int i = 0; i < num; i++) {
            int key = va_arg(args , int);
            uint64_t flag = getFlagFromOFKey(key);
            if (k == kDOWN) flags |= flag;
            else flags &= ~flag;
        }
        CGEventSetFlags(ev, static_cast<CGEventFlags>(flags));
        
        CGEventPost(kCGHIDEventTap, ev);
        CFRelease(ev);
    }
    
private:
    
    // https://github.com/percontation/public/blob/master/keystroke.c
    KeyCode keyCodeForAscii(char c) {
        KeyCode ret{0xFFFF, false};
        switch(c) {
            default: ret.code = 0xFFFF; break;
#define S() (ret.isPressShift = true)
            case 'A': S(); case 'a': ret.code = kVK_ANSI_A; break;
            case 'B': S(); case 'b': ret.code = kVK_ANSI_B; break;
            case 'C': S(); case 'c': ret.code = kVK_ANSI_C; break;
            case 'D': S(); case 'd': ret.code = kVK_ANSI_D; break;
            case 'E': S(); case 'e': ret.code = kVK_ANSI_E; break;
            case 'F': S(); case 'f': ret.code = kVK_ANSI_F; break;
            case 'G': S(); case 'g': ret.code = kVK_ANSI_G; break;
            case 'H': S(); case 'h': ret.code = kVK_ANSI_H; break;
            case 'I': S(); case 'i': ret.code = kVK_ANSI_I; break;
            case 'J': S(); case 'j': ret.code = kVK_ANSI_J; break;
            case 'K': S(); case 'k': ret.code = kVK_ANSI_K; break;
            case 'L': S(); case 'l': ret.code = kVK_ANSI_L; break;
            case 'M': S(); case 'm': ret.code = kVK_ANSI_M; break;
            case 'N': S(); case 'n': ret.code = kVK_ANSI_N; break;
            case 'O': S(); case 'o': ret.code = kVK_ANSI_O; break;
            case 'P': S(); case 'p': ret.code = kVK_ANSI_P; break;
            case 'Q': S(); case 'q': ret.code = kVK_ANSI_Q; break;
            case 'R': S(); case 'r': ret.code = kVK_ANSI_R; break;
            case 'S': S(); case 's': ret.code = kVK_ANSI_S; break;
            case 'T': S(); case 't': ret.code = kVK_ANSI_T; break;
            case 'U': S(); case 'u': ret.code = kVK_ANSI_U; break;
            case 'V': S(); case 'v': ret.code = kVK_ANSI_V; break;
            case 'W': S(); case 'w': ret.code = kVK_ANSI_W; break;
            case 'X': S(); case 'x': ret.code = kVK_ANSI_X; break;
            case 'Y': S(); case 'y': ret.code = kVK_ANSI_Y; break;
            case 'Z': S(); case 'z': ret.code = kVK_ANSI_Z; break;
            case ')': S(); case '0': ret.code = kVK_ANSI_0; break;
            case '!': S(); case '1': ret.code = kVK_ANSI_1; break;
            case '@': S(); case '2': ret.code = kVK_ANSI_2; break;
            case '#': S(); case '3': ret.code = kVK_ANSI_3; break;
            case '$': S(); case '4': ret.code = kVK_ANSI_4; break;
            case '%': S(); case '5': ret.code = kVK_ANSI_5; break;
            case '^': S(); case '6': ret.code = kVK_ANSI_6; break;
            case '&': S(); case '7': ret.code = kVK_ANSI_7; break;
            case '*': S(); case '8': ret.code = kVK_ANSI_8; break;
            case '(': S(); case '9': ret.code = kVK_ANSI_9; break;
            case '_': S(); case '-': ret.code = kVK_ANSI_Minus;        break;
            case '+': S(); case '=': ret.code = kVK_ANSI_Equal;        break;
            case '~': S(); case '`': ret.code = kVK_ANSI_Grave;        break;
            case '{': S(); case '[': ret.code = kVK_ANSI_LeftBracket;  break;
            case '}': S(); case ']': ret.code = kVK_ANSI_RightBracket; break;
            case '|': S(); case '\\':ret.code = kVK_ANSI_Backslash;    break;
            case ':': S(); case ';': ret.code = kVK_ANSI_Semicolon;    break;
            case '"': S(); case '\'':ret.code = kVK_ANSI_Quote;        break;
            case '<': S(); case ',': ret.code = kVK_ANSI_Comma;        break;
            case '>': S(); case '.': ret.code = kVK_ANSI_Period;       break;
            case '?': S(); case '/': ret.code = kVK_ANSI_Slash;        break;
#undef S
            case '\n': case '\r': ret.code = kVK_Return; break;
            case '\t':   ret.code = kVK_Tab;           break;
            case ' ':    ret.code = kVK_Space;         break;
            case '\x08': ret.code = kVK_Delete;        break;
            case 127:    ret.code = kVK_ForwardDelete; break;
            case kHomeCharCode:        ret.code = kVK_Home;             break;
            case kEndCharCode:         ret.code = kVK_End;              break;
            case kHelpCharCode:        ret.code = kVK_Help;             break;
            case kPageUpCharCode:      ret.code = kVK_PageUp;           break;
            case kPageDownCharCode:    ret.code = kVK_PageDown;         break;
            case kEscapeCharCode:      ret.code = kVK_Escape;           break;
            case kLeftArrowCharCode:   ret.code = kVK_LeftArrow;        break;
            case kRightArrowCharCode:  ret.code = kVK_RightArrow;       break;
            case kDownArrowCharCode:   ret.code = kVK_DownArrow;        break;
            case kUpArrowCharCode:     ret.code = kVK_UpArrow;          break;
            case kEnterCharCode:       ret.code = kVK_ANSI_KeypadEnter; break;
            case kFunctionKeyCharCode: ret.code = kVK_Function;         break;
                
                // Eh, why not?
            case '\a': ret.code = 0xFFFF; break;
            case 17: ret.code = kVK_Command;             break;
            case 18: ret.code = kVK_Shift;               break;
            case 19: ret.code = kVK_Option;              break;
            case 20: ret.code = kVK_Control;             break;
            case '\x80'+18: ret.code = kVK_RightShift;     break;
            case '\x80'+19: ret.code = kVK_RightOption;    break;
            case '\x80'+20: ret.code = kVK_RightControl;   break;
            case '\x80'+21: ret.code = kVK_CapsLock;       break;
            case '\xe0': ret.code = kVK_F1;                    break;
            case '\xe1': ret.code = kVK_F2;                    break;
            case '\xe2': ret.code = kVK_F3;                    break;
            case '\xe3': ret.code = kVK_F4;                    break;
            case '\xe4': ret.code = kVK_F5;                    break;
            case '\xe5': ret.code = kVK_F6;                    break;
            case '\xe6': ret.code = kVK_F7;                    break;
            case '\xe7': ret.code = kVK_F8;                    break;
            case '\xe8': ret.code = kVK_F9;                    break;
            case '\xe9': ret.code = kVK_F10;                   break;
            case '\xea': ret.code = kVK_F11;                   break;
            case '\xeb': ret.code = kVK_F12;                   break;
            case '\xec': ret.code = kVK_F13;                   break;
            case '\xed': ret.code = kVK_F14;                   break;
            case '\xee': ret.code = kVK_F15;                   break;
            case '\xef': ret.code = kVK_F16;                   break;
            case '\xf0': ret.code = kVK_F17;                   break;
            case '\xf1': ret.code = kVK_F18;                   break;
            case '\xf2': ret.code = kVK_F19;                   break;
            case '\xf3': ret.code = kVK_F20;                   break;
            case '\xd0': ret.code = kVK_Mute;                  break;
            case '\xd1': ret.code = kVK_VolumeUp;              break;
            case '\xd2': ret.code = kVK_VolumeDown;            break;
            case '\xc0': ret.code = kVK_ANSI_KeypadDecimal;    break;
            case '\xc1': ret.code = kVK_ANSI_KeypadMultiply;   break;
            case '\xc2': ret.code = kVK_ANSI_KeypadPlus;       break;
            case '\xc3': ret.code = kVK_ANSI_KeypadClear;      break;
            case '\xc4': ret.code = kVK_ANSI_KeypadDivide;     break;
            case '\xc5': ret.code = kVK_ANSI_KeypadMinus;      break;
            case '\xc6': ret.code = kVK_ANSI_KeypadEquals;     break;
            case '\x80'+'0': ret.code = kVK_ANSI_Keypad0;  break;
            case '\x80'+'1': ret.code = kVK_ANSI_Keypad1;  break;
            case '\x80'+'2': ret.code = kVK_ANSI_Keypad2;  break;
            case '\x80'+'3': ret.code = kVK_ANSI_Keypad3;  break;
            case '\x80'+'4': ret.code = kVK_ANSI_Keypad4;  break;
            case '\x80'+'5': ret.code = kVK_ANSI_Keypad5;  break;
            case '\x80'+'6': ret.code = kVK_ANSI_Keypad6;  break;
            case '\x80'+'7': ret.code = kVK_ANSI_Keypad7;  break;
            case '\x80'+'8': ret.code = kVK_ANSI_Keypad8;  break;
            case '\x80'+'9': ret.code = kVK_ANSI_Keypad9;  break;
        }
        return ret;
    }

    CGKeyCode keyCodeForChar(const char c) {
        CFDataRef currentLayoutData;
        TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardLayoutInputSource();
        
        if (currentKeyboard == NULL) {
            ofLogError("ofxCocoaKeyEvents") << "Could not find keyboard layout";
            return UINT16_MAX;
        }
        
        currentLayoutData = (CFDataRef)TISGetInputSourceProperty(currentKeyboard,
                                                                 kTISPropertyUnicodeKeyLayoutData);
        CFRelease(currentKeyboard);
        if (currentLayoutData == NULL) {
            ofLogError("ofxCocoaKeyEvents") << "Could not find layout data";
            return UINT16_MAX;
        }
        
        return keyCodeForCharWithLayout(c, (const UCKeyboardLayout *)CFDataGetBytePtr(currentLayoutData));
    }
    
    /* Beware! Messy, incomprehensible code ahead!
     * TODO: XXX: FIXME! Please! */
    CGKeyCode keyCodeForCharWithLayout(const char c,
                                       const UCKeyboardLayout *uchrHeader)
    {
        uint8_t *uchrData = (uint8_t *)uchrHeader;
        UCKeyboardTypeHeader *uchrKeyboardList = (UCKeyboardTypeHeader *)uchrHeader->keyboardTypeList;
        
        /* Loop through the keyboard type list. */
        ItemCount i, j;
        for (i = 0; i < uchrHeader->keyboardTypeCount; ++i) {
            /* Get a pointer to the keyToCharTable structure. */
            UCKeyToCharTableIndex *uchrKeyIX = (UCKeyToCharTableIndex *)
            (uchrData + (uchrKeyboardList[i].keyToCharTableIndexOffset));
            
            /* Not sure what this is for but it appears to be a safeguard... */
            UCKeyStateRecordsIndex *stateRecordsIndex;
            if (uchrKeyboardList[i].keyStateRecordsIndexOffset != 0) {
                stateRecordsIndex = (UCKeyStateRecordsIndex *)
                (uchrData + (uchrKeyboardList[i].keyStateRecordsIndexOffset));
                
                if ((stateRecordsIndex->keyStateRecordsIndexFormat) !=
                    kUCKeyStateRecordsIndexFormat) {
                    stateRecordsIndex = NULL;
                }
            } else {
                stateRecordsIndex = NULL;
            }
            
            /* Make sure structure is a table that can be searched. */
            if ((uchrKeyIX->keyToCharTableIndexFormat) != kUCKeyToCharTableIndexFormat) {
                continue;
            }
            
            /* Check the table of each keyboard for character */
            for (j = 0; j < uchrKeyIX->keyToCharTableCount; ++j) {
                UCKeyOutput *keyToCharData =
                (UCKeyOutput *)(uchrData + (uchrKeyIX->keyToCharTableOffsets[j]));
                
                /* Check THIS table of the keyboard for the character. */
                UInt16 k;
                for (k = 0; k < uchrKeyIX->keyToCharTableSize; ++k) {
                    /* Here's the strange safeguard again... */
                    if ((keyToCharData[k] & kUCKeyOutputTestForIndexMask) ==
                        kUCKeyOutputStateIndexMask) {
                        long keyIndex = (keyToCharData[k] & kUCKeyOutputGetIndexMask);
                        if (stateRecordsIndex != NULL &&
                            keyIndex <= (stateRecordsIndex->keyStateRecordCount)) {
                            UCKeyStateRecord *stateRecord = (UCKeyStateRecord *)
                            (uchrData +
                             (stateRecordsIndex->keyStateRecordOffsets[keyIndex]));
                            
                            if ((stateRecord->stateZeroCharData) == c) {
                                return (CGKeyCode)k;
                            }
                        } else if (keyToCharData[k] == c) {
                            return (CGKeyCode)k;
                        }
                    } else if (((keyToCharData[k] & kUCKeyOutputTestForIndexMask)
                                != kUCKeyOutputSequenceIndexMask) &&
                               keyToCharData[k] != 0xFFFE &&
                               keyToCharData[k] != 0xFFFF &&
                               keyToCharData[k] == c) {
                        return (CGKeyCode)k;
                    }
                }
            }
        }
        
        return UINT16_MAX;
    }
    
    CGKeyCode getCodeFromOFKey(int key) {
        switch (key) {
            case OF_KEY_ALT:
                return kVK_Option;
                
            case OF_KEY_COMMAND:
            case OF_KEY_LEFT_COMMAND:
            case OF_KEY_RIGHT_COMMAND:
                return kVK_Command;
                
            case OF_KEY_CONTROL:
                return kVK_Control;
                
            case OF_KEY_DEL:
                return kVK_ForwardDelete;
                
            case OF_KEY_BACKSPACE:
                return kVK_Delete;
                
            case OF_KEY_DOWN:
                return kVK_DownArrow;
                
            case OF_KEY_END:
                return kVK_End;
                
            case OF_KEY_ESC:
                return kVK_Escape;
                
            case OF_KEY_F1:
                return kVK_F1;
            case OF_KEY_F2:
                return kVK_F2;
            case OF_KEY_F3:
                return kVK_F3;
            case OF_KEY_F4:
                return kVK_F4;
            case OF_KEY_F5:
                return kVK_F5;
            case OF_KEY_F6:
                return kVK_F6;
            case OF_KEY_F7:
                return kVK_F7;
            case OF_KEY_F8:
                return kVK_F8;
            case OF_KEY_F9:
                return kVK_F9;
            case OF_KEY_F10:
                return kVK_F10;
            case OF_KEY_F11:
                return kVK_F11;
            case OF_KEY_F12:
                return kVK_F12;
                
            case OF_KEY_HOME:
                return kVK_Home;
                
            case OF_KEY_LEFT:
                return kVK_LeftArrow;
                
            case OF_KEY_PAGE_DOWN:
                return kVK_PageDown;
                
            case OF_KEY_PAGE_UP:
                return kVK_PageUp;
                
            case OF_KEY_RETURN:
                return kVK_Return;
                
            case OF_KEY_RIGHT:
                return kVK_RightArrow;
                
            case OF_KEY_SHIFT:
            case OF_KEY_LEFT_SHIFT:
                return kVK_Shift;
            case OF_KEY_RIGHT_SHIFT:
                return kVK_RightShift;
                
            case OF_KEY_TAB:
                return kVK_Tab;
                
            case OF_KEY_UP:
                return kVK_UpArrow;
                
            default:
                break;
        }
        return 0;
    }
    
    CGEventFlags getFlagFromOFKey(int key) {
        switch (key) {
            case OF_KEY_ALT:
                return kCGEventFlagMaskAlternate;
            case OF_KEY_COMMAND:
                return kCGEventFlagMaskCommand;
            case OF_KEY_CONTROL:
                return kCGEventFlagMaskControl;
            case OF_KEY_SHIFT:
                return kCGEventFlagMaskShift;
            default:
                break;
        }
        return static_cast<CGEventFlags>(0);
    }
};
