#include "cn1_globals.h"

#include "java_lang_Object.h"
#include "java_lang_Boolean.h"
#include "java_lang_String.h"
#include "java_lang_Integer.h"
#include "java_lang_Byte.h"
#include "java_lang_Short.h"
#include "java_lang_Character.h"
#include "java_lang_Thread.h"
#include "java_lang_Long.h"
#include "java_lang_Double.h"
#include "java_lang_Float.h"
#include "java_lang_Runnable.h"
#include "java_lang_Throwable.h"
#include "java_lang_StringBuilder.h"
#include "java_util_HashMap.h"
#include "java_util_HashMap_Entry.h"
#include "java_lang_NullPointerException.h"
#include "java_nio_ByteBuffer.h"
#include "java_util_Date.h"
#include "java_text_DateFormat.h"
#include "java_lang_StringToReal.h"
#include "java_lang_System.h"
#include "java_util_zip_Inflater.h"
#include "java_lang_reflect_Field.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <sys/param.h>
#include <zlib.h>

#ifdef __SWITCH__
#include <switch.h>
#endif

// Todo: Proper logging
#define NSLog(...)

extern JAVA_BOOLEAN lowMemoryMode;

// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1
#define USE_DFA_UTF8_DECODER

static const uint8_t utf8d[] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
        8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
        0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
        0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
        0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
        1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
        1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
        1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};

pthread_t gcThread;

JAVA_OBJECT java_lang_Class_getName___R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls);

static uint32_t inline
decode(uint32_t* state, uint32_t* codep, uint32_t byte) {
    uint32_t type = utf8d[byte];

    *codep = (*state != UTF8_ACCEPT) ?
             (byte & 0x3fu) | (*codep << 6) :
             (0xff >> type) & (byte);

    *state = utf8d[256 + *state*16 + type];
    return *state;
}

JAVA_OBJECT java_nio_ByteBuffer_allocateDirect___int_R_java_nio_ByteBuffer(CODENAME_ONE_THREAD_STATE, JAVA_INT __cn1Arg1) {
    JAVA_OBJECT o = __NEW_java_nio_ByteBuffer(threadStateData);
    void *data = malloc(__cn1Arg1);
    java_nio_ByteBuffer___INIT_____long_int(threadStateData, o, (JAVA_LONG)data, __cn1Arg1);
    return o;
}

JAVA_VOID java_nio_ByteBuffer_deallocate___long(CODENAME_ONE_THREAD_STATE, JAVA_LONG __cn1Arg1) {
    free((void *)__cn1Arg1);
}

/*
 * The class representing classes
 */
struct clazz ClazzClazz = {
        DEBUG_GC_INIT 0, 999999, 0, 0, 0, 0, 0, 0, 0, 0, cn1_array_start_offset, "java.lang.Class", JAVA_FALSE, 0, 0, JAVA_FALSE, &class__java_lang_Object, EMPTY_INTERFACES, 0, 0, 0
};


JAVA_BOOLEAN compareStringToCharArray(const char* str, JAVA_ARRAY_CHAR* chrs, int length) {
    if(strlen(str) != length) {
        return JAVA_FALSE;
    }
    for(int iter = 0 ; iter < length ; iter++) {
        if(toupper(chrs[iter]) != str[iter]) {
            return JAVA_FALSE;
        }
    }
    return JAVA_TRUE;
}

JAVA_VOID java_lang_String_releaseNativeString___long(CODENAME_ONE_THREAD_STATE, JAVA_LONG string) {
    if(string != 0)
        free((void *) string);
}

JAVA_BOOLEAN java_lang_String_equals___java_lang_Object_R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_OBJECT __cn1Arg1) {
    if(__cn1ThisObject == __cn1Arg1) {
        return JAVA_TRUE;
    }
#ifdef CN1_INCLUDE_NPE_CHECKS
    if(__cn1ThisObject == JAVA_NULL) {
        THROW_NULL_POINTER_EXCEPTION();
    }
#endif
    if(__cn1Arg1 == JAVA_NULL || __cn1Arg1->__codenameOneParentClsReference->classId != __cn1ThisObject->__codenameOneParentClsReference->classId) {
        return JAVA_FALSE;
    }
    struct obj__java_lang_String* t = (struct obj__java_lang_String*)__cn1ThisObject;
    struct obj__java_lang_String* o = (struct obj__java_lang_String*)__cn1Arg1;
    if(t->java_lang_String_count != o->java_lang_String_count) {
        return JAVA_FALSE;
    }

    JAVA_ARRAY_CHAR* oa = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)o->java_lang_String_value)->data;
    JAVA_ARRAY_CHAR* ta = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)t->java_lang_String_value)->data;
    JAVA_INT oo = o->java_lang_String_offset;
    JAVA_INT to = t->java_lang_String_offset;

    for(int iter = 0 ; iter < t->java_lang_String_count ; iter++) {
        if(oa[iter+oo] != ta[iter+to]) {
            return JAVA_FALSE;
        }
    }
    return JAVA_TRUE;
}

JAVA_INT java_lang_Character_toLowerCase___int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_INT __cn1Arg1) {
    if ('A' <= __cn1Arg1 && __cn1Arg1 <= 'Z') {
        return (JAVA_CHAR) (__cn1Arg1 + ('a' - 'A'));
    }
    return __cn1Arg1;
}

JAVA_CHAR java_lang_Character_toLowerCase___char_R_char(CODENAME_ONE_THREAD_STATE, JAVA_INT __cn1Arg1) {
    if ('A' <= __cn1Arg1 && __cn1Arg1 <= 'Z') {
        return (JAVA_CHAR) (__cn1Arg1 + ('a' - 'A'));
    }
    return __cn1Arg1;
}

JAVA_BOOLEAN java_lang_String_equalsIgnoreCase___java_lang_String_R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_OBJECT __cn1Arg1) {
    if(__cn1ThisObject == __cn1Arg1) {
        return JAVA_TRUE;
    }
#ifdef CN1_INCLUDE_NPE_CHECKS
    if(__cn1ThisObject == JAVA_NULL) {
        THROW_NULL_POINTER_EXCEPTION();
    }
#endif
    if(__cn1Arg1 == JAVA_NULL || __cn1Arg1->__codenameOneParentClsReference->classId != __cn1ThisObject->__codenameOneParentClsReference->classId) {
        return JAVA_FALSE;
    }
    struct obj__java_lang_String* t = (struct obj__java_lang_String*)__cn1ThisObject;
    struct obj__java_lang_String* o = (struct obj__java_lang_String*)__cn1Arg1;
    if(t->java_lang_String_count != o->java_lang_String_count) {
        return JAVA_FALSE;
    }

    JAVA_ARRAY_CHAR* oa = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)o->java_lang_String_value)->data;
    JAVA_ARRAY_CHAR* ta = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)t->java_lang_String_value)->data;
    JAVA_INT oo = o->java_lang_String_offset;
    JAVA_INT to = t->java_lang_String_offset;

    for(int iter = 0 ; iter < t->java_lang_String_count ; iter++) {
        JAVA_ARRAY_CHAR jo = oa[iter+oo];
        JAVA_ARRAY_CHAR jt = ta[iter+oo];
        if ('A' <= jo && jo <= 'Z') {
            jo = (JAVA_ARRAY_CHAR) (jo + ('a' - 'A'));
        }
        if ('A' <= jt && jt <= 'Z') {
            jt = (JAVA_ARRAY_CHAR) (jt + ('a' - 'A'));
        }
        if(jo != jt) {
            return JAVA_FALSE;
        }
    }
    return JAVA_TRUE;
}

JAVA_INT java_lang_String_hashCode___R_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject) {
    struct obj__java_lang_String* t = (struct obj__java_lang_String*)__cn1ThisObject;
    JAVA_INT hash = t->java_lang_String_hashCode;
    if (hash == 0) {
        if (t->java_lang_String_count == 0) {
            return 0;
        }
        JAVA_INT end = t->java_lang_String_count + t->java_lang_String_offset;
        JAVA_ARRAY_CHAR* chars = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)t->java_lang_String_value)->data;
        for (JAVA_INT i = t->java_lang_String_offset; i < end; ++i) {
            hash = 31*hash + chars[i];
        }
        t->java_lang_String_hashCode = hash;
    }
    return hash;
}

JAVA_OBJECT java_lang_reflect_Array_newInstanceImpl___java_lang_Class_int_R_java_lang_Object(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls, JAVA_INT len) {
    enteringNativeAllocations();
    struct clazz* clz = (struct clazz*)cls;
    if (clz->arrayClass == 0) {
        JAVA_OBJECT ex = __NEW_java_lang_RuntimeException(CN1_THREAD_STATE_PASS_SINGLE_ARG);
        java_lang_RuntimeException___INIT_____java_lang_String(CN1_THREAD_STATE_PASS_ARG ex, newStringFromCString(CN1_THREAD_STATE_PASS_ARG "Attempt to create array with reflection, but the component class has no registered array class"));
        finishedNativeAllocations();
        throwException(threadStateData, ex);
        return NULL;
    }
    JAVA_OBJECT out = allocArray(CN1_THREAD_STATE_PASS_ARG len, clz->arrayClass, sizeof(JAVA_OBJECT), 1);
    finishedNativeAllocations();
    return out;
}

JAVA_VOID java_lang_reflect_Array_set___java_lang_Object_int_java_lang_Object(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT __cn1Arg1, JAVA_INT __cn1Arg2, JAVA_OBJECT __cn1Arg3) {

}

JAVA_OBJECT java_lang_reflect_Array_get___java_lang_Object_int_R_java_lang_Object(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT __cn1Arg1, JAVA_INT __cn1Arg2) {
    return NULL;
}

JAVA_INT java_lang_reflect_Array_getLength___java_lang_Object_R_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT __cn1Arg1) {
    return 0;
}

JAVA_OBJECT java_lang_String_bytesToChars___byte_1ARRAY_int_int_java_lang_String_R_char_1ARRAY(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT b, JAVA_INT off, JAVA_INT len, JAVA_OBJECT encoding) {
    enteringNativeAllocations();
    JAVA_ARRAY_BYTE* sourceData = (JAVA_ARRAY_BYTE*)((JAVA_ARRAY)b)->data;
    sourceData += off;
    JAVA_OBJECT destArr = __NEW_ARRAY_JAVA_CHAR(threadStateData, len);
    JAVA_ARRAY_CHAR* dest = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)destArr)->data;
    for (int i = 0; i < len; i++)
        dest[i] = (unsigned char)sourceData[i];
    finishedNativeAllocations();
    return destArr;

//     enteringNativeAllocations();
//     JAVA_ARRAY_BYTE* sourceData = (JAVA_ARRAY_BYTE*)((JAVA_ARRAY)b)->data;
//     sourceData += off;
//     NSStringEncoding enc;
//     struct obj__java_lang_String* encString = (struct obj__java_lang_String*)encoding;
//     JAVA_ARRAY_CHAR* encArr = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)encString->java_lang_String_value)->data;
//     int arrLength = encString->java_lang_String_count;
//     if(encoding == JAVA_NULL || compareStringToCharArray("UTF-8", encArr, arrLength)) {
//         enc = NSUTF8StringEncoding;
//     } else {
//         if(compareStringToCharArray("US-ASCII", encArr, arrLength)) {
//             JAVA_OBJECT destArr = __NEW_ARRAY_JAVA_CHAR(threadStateData, len);
//             JAVA_ARRAY_CHAR* dest = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)destArr)->data;
//             for(int iter = 0 ; iter < len ; iter++) {
//                 dest[iter] = sourceData[iter];
//             }
//             finishedNativeAllocations();
//             return destArr;
//         } else {
//             if(compareStringToCharArray("UTF-16", encArr, arrLength)) {
//                 enc = NSUTF16StringEncoding;
//             } else {
//                 if(compareStringToCharArray("ISO-8859-1", encArr, arrLength)) {
//                     enc = NSISOLatin1StringEncoding;
//                 } else {
//                     if(compareStringToCharArray("ISO-8859-2", encArr, arrLength)) {
//                         enc = NSISOLatin1StringEncoding;
//                     } else {
    // need to throw an exception...
//                         enc = NSUTF8StringEncoding;
//                     }
//                 }
//             }
//         }
//     }
//
    // first try to optimize encoding in case of US-ASCII characters
//     if(enc == NSUTF8StringEncoding) {
// #ifdef USE_DFA_UTF8_DECODER
//         size_t count;
//         uint32_t codepoint;
//         uint32_t state = 0;
//         JAVA_ARRAY_BYTE* s = sourceData;
//         JAVA_ARRAY_BYTE* end = s + len;
//         for (count=0; s < end; s = s + 1)
//             if (!decode(&state, &codepoint, (uint8_t)*s)) {
//                 if (codepoint > 65535) {
//                     count +=2;
//                 } else {
//                     count+=1;
//                 }
//             }
//
//         if (state != UTF8_ACCEPT) {
    // Need to throw an exception here.
//             JAVA_OBJECT ex = __NEW_java_lang_RuntimeException(CN1_THREAD_STATE_PASS_SINGLE_ARG);
//             java_lang_RuntimeException___INIT_____java_lang_String(CN1_THREAD_STATE_PASS_ARG ex, newStringFromCString(CN1_THREAD_STATE_PASS_ARG "Decoding Error"));
//             finishedNativeAllocations();
//             throwException(threadStateData, ex);
//             return NULL;
//         }
//         JAVA_OBJECT destArr = __NEW_ARRAY_JAVA_CHAR(threadStateData, count);
//         JAVA_ARRAY_CHAR* dest = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)destArr)->data;
//         state = UTF8_ACCEPT;
//         codepoint = 0;
//         s = sourceData;
    //MIN_SUPPLEMENTARY_CODE_POINT + ((highSurrogate & 1023) << 10) + (lowSurrogate & 1023);
//         for (; s < end; s = s+1)
//             if (!decode(&state, &codepoint, (uint8_t)*s)) {
//
//                 if (codepoint > 65535) {
    //(char) (MIN_HIGH_SURROGATE + (((codePoint - MIN_SUPPLEMENTARY_CODE_POINT) >> 10) & 1023));
//                     *dest = 55296 + (((codepoint - 0x10000) >> 10) & 1023);
//                     dest = dest + 1;
    //(MIN_LOW_SURROGATE + ((codePoint - MIN_SUPPLEMENTARY_CODE_POINT) & 1023))
//                     *dest = 56320 + ((codepoint - 0x10000) & 1023);
//                     dest = dest+1;
//
//                 } else {
//                     *dest = (JAVA_CHAR)codepoint;
//                     dest= dest + 1;
//                 }
//             }
//
//         finishedNativeAllocations();
//         return destArr;
// #else
//         JAVA_BOOLEAN ascii = JAVA_TRUE;
//         for(int iter = 0 ; iter < len ; iter++) {
//             if(sourceData[iter] < 0) {
//                 ascii = JAVA_FALSE;
//                 break;
//             }
//         }
//         if(ascii) {
//             JAVA_OBJECT destArr = __NEW_ARRAY_JAVA_CHAR(threadStateData, len);
//             JAVA_ARRAY_CHAR* dest = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)destArr)->data;
//             for(int iter = 0 ; iter < len ; iter++) {
//                 dest[iter] = sourceData[iter];
//             }
//             finishedNativeAllocations();
//             return destArr;
//         }
// #endif
//     }


    // this allows emojii to work with the Strings properly
//     NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
//     NSString* nsStr = [[NSString alloc] initWithBytes:sourceData length:len encoding:enc];
//     if (nsStr == nil) {
//         nsStr = [[NSString alloc] initWithBytes:sourceData length:len encoding:NSISOLatin1StringEncoding];
//         if (nsStr == nil) {
//             JAVA_OBJECT ex = __NEW_java_lang_RuntimeException(CN1_THREAD_STATE_PASS_SINGLE_ARG);
//             java_lang_RuntimeException___INIT_____java_lang_String(CN1_THREAD_STATE_PASS_ARG ex, newStringFromCString(CN1_THREAD_STATE_PASS_ARG "Encoding Error"));
//             finishedNativeAllocations();
//             throwException(threadStateData, ex);
//
//             return NULL;
//         }
//     }
//
//     JAVA_OBJECT destArr = __NEW_ARRAY_JAVA_CHAR(threadStateData, [nsStr length]);
//     __block JAVA_ARRAY_CHAR* dest = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)destArr)->data;
//     __block int length = 0;
//     [nsStr enumerateSubstringsInRange:NSMakeRange(0, [nsStr length])
//                                options:NSStringEnumerationByComposedCharacterSequences
//                             usingBlock:^(NSString *substring, NSRange substringRange, NSRange enclosingRange, BOOL *stop) {
//                                 unichar ch = [nsStr characterAtIndex:length];
//                                 dest[length] = (JAVA_ARRAY_CHAR)ch;
//                                 length++;
//                                 if([substring length] > 1) {
    // we have surrogate pairs here...
//                                     ch = [substring characterAtIndex:1];
//                                     dest[length] = (JAVA_ARRAY_CHAR)ch;
//                                     length++;
//                                 }
//                             }];
//
//     [nsStr release];
//     [pool release];
//     finishedNativeAllocations();
//     return destArr;
}

JAVA_OBJECT java_io_InputStreamReader_bytesToChars___byte_1ARRAY_int_int_java_lang_String_R_char_1ARRAY(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT b, JAVA_INT off, JAVA_INT len, JAVA_OBJECT encoding) {
    return java_lang_String_bytesToChars___byte_1ARRAY_int_int_java_lang_String_R_char_1ARRAY(threadStateData, b, off, len, encoding);
}

JAVA_BOOLEAN isAsciiArray(JAVA_ARRAY sourceArr) {
    JAVA_ARRAY_CHAR* arr = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)sourceArr)->data;
    for(int iter = 0 ; iter < sourceArr->length ; iter++) {
        if(arr[iter] > 127) {
            return JAVA_FALSE;
        }
    }
    return JAVA_TRUE;
}

JAVA_OBJECT java_lang_String_charsToBytes___char_1ARRAY_char_1ARRAY_R_byte_1ARRAY(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT arr, JAVA_OBJECT encoding) {
    JAVA_ARRAY sourceArr = (JAVA_ARRAY)arr;
    JAVA_OBJECT destArr = __NEW_ARRAY_JAVA_BYTE(threadStateData, sourceArr->length);
    JAVA_ARRAY_CHAR* chars = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)sourceArr)->data;
    JAVA_ARRAY_BYTE* dest = (JAVA_ARRAY_BYTE*)((JAVA_ARRAY)destArr)->data;
    for(int i = 0; i < sourceArr->length; i++)
        dest[i] = (JAVA_ARRAY_BYTE)chars[i];
    return destArr;

//    JAVA_ARRAY sourceArr = (JAVA_ARRAY)arr;
//    if(isAsciiArray(sourceArr)) {
//        JAVA_OBJECT destArr = __NEW_ARRAY_JAVA_BYTE(threadStateData, sourceArr->length);
//        JAVA_ARRAY_CHAR* arr = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)sourceArr)->data;
//        JAVA_ARRAY_BYTE* dest = (JAVA_ARRAY_BYTE*)((JAVA_ARRAY)destArr)->data;
//        for(int iter = 0 ; iter < sourceArr->length ; iter++) {
//            dest[iter] = (JAVA_ARRAY_BYTE)arr[iter];
//        }
//        return destArr;
//    }
//    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
//    NSString* nsStr = [[NSString alloc] initWithCharacters:sourceArr->data length:sourceArr->length];
//    NSStringEncoding enc;
//    JAVA_ARRAY_CHAR* encArr = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)encoding)->data;
//    int arrLength = ((JAVA_ARRAY)encoding)->length;
//    if(encoding == JAVA_NULL || compareStringToCharArray("UTF-8", encArr, arrLength)) {
//        enc = NSUTF8StringEncoding;
//    } else {
//        if(compareStringToCharArray("US-ASCII", encArr, arrLength)) {
//            enc = NSASCIIStringEncoding;
//        } else {
//            if(compareStringToCharArray("UTF-16", encArr, arrLength)) {
//                enc = NSUTF16StringEncoding;
//            } else {
//                if(compareStringToCharArray("ISO-8859-1", encArr, arrLength)) {
//                    enc = NSISOLatin1StringEncoding;
//                } else {
//                    if(compareStringToCharArray("ISO-8859-2", encArr, arrLength)) {
//                        enc = NSISOLatin1StringEncoding;
//                    } else {
//                        // need to throw an exception...
//                        enc = NSUTF8StringEncoding;
//                    }
//                }
//            }
//        }
//    }
//
//    NSData* data = [nsStr dataUsingEncoding:enc];
//    JAVA_OBJECT destArr = __NEW_ARRAY_JAVA_BYTE(threadStateData, [data length]);
//    JAVA_ARRAY_BYTE* dest = (JAVA_ARRAY_BYTE*)((JAVA_ARRAY)destArr)->data;
//    [data getBytes:dest length:[data length]];
//
//    [nsStr release];
//    [pool release];
//    return destArr;
}

JAVA_VOID java_lang_Throwable_fillInStack__(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject) {
    if (get_field_java_lang_Throwable_stack(__cn1ThisObject) == JAVA_NULL) {
        set_field_java_lang_Throwable_stack(threadStateData, java_lang_Throwable_getStack___R_java_lang_String(threadStateData, __cn1ThisObject), __cn1ThisObject);
    }

}

JAVA_OBJECT newline = JAVA_NULL;
JAVA_OBJECT dot = JAVA_NULL;
JAVA_OBJECT colon = JAVA_NULL;
JAVA_OBJECT indent = JAVA_NULL;


JAVA_OBJECT java_lang_Throwable_getStack___R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT me) {
    JAVA_OBJECT bld = __NEW_INSTANCE_java_lang_StringBuilder(threadStateData);
    JAVA_OBJECT classObj = java_lang_Object_getClass___R_java_lang_Class(threadStateData, me);
    JAVA_OBJECT className = java_lang_Class_getName___R_java_lang_String(threadStateData, classObj);
    java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, bld, className);
    if(newline == JAVA_NULL) {
        newline = newStringFromCString(threadStateData, "\n");
        newline->__codenameOneReferenceCount = 999999;
        dot = newStringFromCString(threadStateData, ".");
        dot->__codenameOneReferenceCount = 999999;
        colon = newStringFromCString(threadStateData, ":");
        colon->__codenameOneReferenceCount = 999999;
        indent = newStringFromCString(threadStateData, "    at ");
        indent->__codenameOneReferenceCount = 999999;
        removeObjectFromHeapCollection(threadStateData, newline);
        removeObjectFromHeapCollection(threadStateData, dot);
        removeObjectFromHeapCollection(threadStateData, colon);
        removeObjectFromHeapCollection(threadStateData, indent);
        removeObjectFromHeapCollection(threadStateData, ((struct obj__java_lang_String*)newline)->java_lang_String_value);
        removeObjectFromHeapCollection(threadStateData, ((struct obj__java_lang_String*)dot)->java_lang_String_value);
        removeObjectFromHeapCollection(threadStateData, ((struct obj__java_lang_String*)colon)->java_lang_String_value);
        removeObjectFromHeapCollection(threadStateData, ((struct obj__java_lang_String*)indent)->java_lang_String_value);
    }
    java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, bld, newline);

    for(int iter = threadStateData->callStackOffset - 1 ; iter >= 0 ; iter--) {
        java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, bld, indent);

        int classId = threadStateData->callStackClass[iter];
        int methodId = threadStateData->callStackMethod[iter];
        int line = threadStateData->callStackLine[iter];

        java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, bld, STRING_FROM_CONSTANT_POOL_OFFSET(classId));

        java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, bld, dot);

        java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, bld, STRING_FROM_CONSTANT_POOL_OFFSET(methodId));

        java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, bld, colon);

        java_lang_StringBuilder_append___int_R_java_lang_StringBuilder(threadStateData, bld, line);

        java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, bld, newline);
    }
    JAVA_OBJECT o = java_lang_StringBuilder_toString___R_java_lang_String(threadStateData, bld);
    o->__codenameOneReferenceCount = 0;
    return o;
}

JAVA_VOID java_io_NSLogOutputStream_write___byte_1ARRAY_int_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT me, JAVA_OBJECT b, JAVA_INT off, JAVA_INT len) {
    JAVA_ARRAY a = (JAVA_ARRAY)b;
    JAVA_ARRAY_BYTE* arr = (JAVA_ARRAY_BYTE*)(*a).data;
    printf("%.*s", len, arr + off);

//    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
//    JAVA_ARRAY a = (JAVA_ARRAY)b;
//    JAVA_ARRAY_BYTE* arr = (JAVA_ARRAY_BYTE*)(*a).data;
//    NSData * data = [NSData dataWithBytes:arr length:len];
//    NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
//
//    // otherwise this produces a security warning in the compiler
//    NSLog(@"%@", str);
//
//    // if we disable arc we will need to re-enable these
//    [str release];
//    [pool release];
}

JAVA_VOID java_lang_System_arraycopy___java_lang_Object_int_java_lang_Object_int_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT src, JAVA_INT srcOffset, JAVA_OBJECT dst, JAVA_INT dstOffset, JAVA_INT length) {
    __STATIC_INITIALIZER_java_lang_System(threadStateData);
    JAVA_ARRAY srcArr = (JAVA_ARRAY)src;
    JAVA_ARRAY dstArr = (JAVA_ARRAY)dst;
    if (src == JAVA_NULL || dst == JAVA_NULL) {
        THROW_NULL_POINTER_EXCEPTION();
        return;
    }
    if (srcOffset < 0 || dstOffset < 0 || srcOffset + length > srcArr->length || dstOffset + length > dstArr->length || length < 0) {
        THROW_ARRAY_INDEX_EXCEPTION(-1);
        return;
    }
    struct clazz* cls = (*srcArr).__codenameOneParentClsReference;
    int byteSize = byteSizeForArray(cls);
    memcpy( (*dstArr).data + (dstOffset * byteSize), (*srcArr).data  + (srcOffset * byteSize), length * byteSize);
}

JAVA_LONG java_lang_System_currentTimeMillis___R_long(CODENAME_ONE_THREAD_STATE) {
    __STATIC_INITIALIZER_java_lang_System(threadStateData);
    struct timeval time;
    gettimeofday(&time, NULL);
    JAVA_LONG l = (((JAVA_LONG)time.tv_sec) * 1000) + (time.tv_usec / 1000);
    return l;
}

JAVA_DOUBLE java_lang_Double_longBitsToDouble___long_R_double(CODENAME_ONE_THREAD_STATE, JAVA_LONG n1)
{
    union {
        JAVA_DOUBLE d;
        JAVA_LONG   l;
    } u;

    u.l = n1;
    return u.d;
}

JAVA_LONG java_lang_Double_doubleToLongBits___double_R_long(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE n1) {
    union {
        JAVA_DOUBLE d;
        JAVA_LONG   l;
    } u;

    u.d = n1;
    return u.l;
}

JAVA_LONG java_lang_Double_doubleToRawLongBits___double_R_long(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE n1) {
    union {
        JAVA_DOUBLE d;
        JAVA_LONG   l;
    } u;

    u.d = n1;
    return u.l;
}

JAVA_FLOAT java_lang_Float_intBitsToFloat___int_R_float(CODENAME_ONE_THREAD_STATE, JAVA_INT n1)
{
    union {
        JAVA_FLOAT  f;
        JAVA_INT    i;
    } u;

    u.i = n1;
    return u.f;
}

JAVA_INT java_lang_Float_floatToIntBits___float_R_int(CODENAME_ONE_THREAD_STATE, JAVA_FLOAT n1)
{
    union {
        JAVA_FLOAT  f;
        JAVA_INT    i;
    } u;

    u.f = n1;
    return u.i;
}


JAVA_OBJECT java_lang_Double_toStringImpl___double_boolean_R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE d, JAVA_BOOLEAN b) {
    char s[32];
    if ( !b ){
        snprintf(s, 32, "%lf", d);
    } else {
        snprintf(s, 32, "%1.20E", d);
    }

    // We need to match the format of Java spec.  That includes:
    // No "+" for positive exponent.
    // No leading zeroes in positive exponents.
    // No trailing zeroes in decimal portion.
    int j=0;
    int i=32;
    char s2[32];
    BOOL inside=NO;
    while (i-->0 && j < 32){
        if (inside){
            if (s[i]=='.'){
                s2[j++]='0';
            }
            if (s[i]!='0'){
                inside=NO;
                s2[j++]=s[i];
            }

        } else {
            if (s[i]=='E'){
                inside=YES;
            }
            if (s[i]=='+'){
                // If a positive exponent, we don't need leading zeroes in
                // the exponent
                while (s2[--j]=='0'){

                }
                j++;
                continue;
            }
            s2[j++]=s[i];
        }
    }
    i=0;
    while (j-->0){
        s[i++]=s2[j];
        if (s[i]=='\0'){
            break;
        }
    }
    if (strcmp(s, "NAN") == 0) {
        s[1] = 'a';
    }
    return newStringFromCString(threadStateData, s);
}

JAVA_OBJECT java_lang_Float_toStringImpl___float_boolean_R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_FLOAT d, JAVA_BOOLEAN b) {
    char s[32];
    if ( !b ){
        sprintf(s, "%f", d);
    } else {
        sprintf(s, "%1.20E", d);
    }
    // We need to match the format of Java spec.  That includes:
    // No "+" for positive exponent.
    // No leading zeroes in positive exponents.
    // No trailing zeroes in decimal portion.
    int j=0;
    int i=32;
    char s2[32];
    BOOL inside=NO;
    while (i-->0){
        if (inside){
            if (s[i]=='.'){
                s2[j++]='0';
            }
            if (s[i]!='0'){
                inside=NO;
                s2[j++]=s[i];
            }

        } else {
            if (s[i]=='E'){
                inside=YES;
            }
            if (s[i]=='+'){
                // If a positive exponent, we don't need leading zeroes in
                // the exponent
                while (s2[--j]=='0'){

                }
                j++;
                continue;
            }
            s2[j++]=s[i];
        }
    }
    i=0;
    while (j-->0){
        s[i++]=s2[j];
        if (s[i]=='\0'){
            break;
        }
    }
    if (strcmp(s, "NAN") == 0) {
        s[1] = 'a';
    }
    return newStringFromCString(threadStateData, s);
}


JAVA_OBJECT java_lang_Integer_toString___int_R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_INT d) {
    char s[12];
    sprintf(s, "%i", d);
    return newStringFromCString(threadStateData, s);
}

char *ltostr (char *str, long long val, unsigned base) {
    ldiv_t r;           /* result of val / base */

    /* no conversion if wrong base */
    if (base > 36) {
        str = '\0';
        return str;
    }
    if (val < 0)    *str++ = '-';
    r = ldiv (labs(val), base);

    /* output digits of val/base first */

    if (r.quot > 0)  str = ltostr (str, r.quot, base);

    /* output last digit */

    *str++ = "0123456789abcdefghijklmnopqrstuvwxyz"[(int)r.rem];
    *str   = '\0';
    return str;
}

JAVA_OBJECT java_lang_Integer_toString___int_int_R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_INT d, JAVA_INT radix) {
    char s[12];
    ltostr(s, d, radix);
    return newStringFromCString(threadStateData, s);
}

JAVA_OBJECT java_lang_Long_toString___long_int_R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_LONG d, JAVA_INT radix) {
    char str[256];
    switch(radix) {
        case 10:
            sprintf(str, "%lld", d);
            return newStringFromCString(threadStateData, str);
        case 16:
            sprintf(str, "%llx", d);
            return newStringFromCString(threadStateData, str);
    }
    ltostr(str, d, radix);
    return newStringFromCString(threadStateData, str);
}

JAVA_DOUBLE java_lang_Math_cos___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
     return cos(a);
}

JAVA_DOUBLE java_lang_Math_sin___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
     return sin(a);
}

JAVA_DOUBLE java_lang_Math_abs___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    if(a < 0) {
        return a * -1;
    }
    return a;
}

JAVA_FLOAT java_lang_Math_abs___float_R_float(CODENAME_ONE_THREAD_STATE, JAVA_FLOAT a) {
    if(a < 0) {
        return a * -1;
    }
    return a;
}

JAVA_INT java_lang_Math_abs___int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_INT a) {
    if(a < 0) {
        return a * -1;
    }
    return a;
}

JAVA_LONG java_lang_Math_abs___long_R_long(CODENAME_ONE_THREAD_STATE, JAVA_LONG a) {
    if(a < 0) {
        return a * -1;
    }
    return a;
}

JAVA_DOUBLE java_lang_Math_ceil___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    JAVA_LONG la = (JAVA_LONG)a;
    if ( a == la || a < 0) return la;
    return la+1;
}

JAVA_DOUBLE java_lang_Math_floor___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    JAVA_LONG la = (JAVA_LONG)a;
    if ( a >= 0 || a == la ) return la;
    return la-1;
}

JAVA_DOUBLE java_lang_Math_max___double_double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a, JAVA_DOUBLE b){
    if(a > b) return a;
    return b;
}

JAVA_DOUBLE java_lang_Math_pow___double_double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a, JAVA_DOUBLE b){
     return pow(a, b);
}

JAVA_FLOAT java_lang_Math_max___float_float_R_float(CODENAME_ONE_THREAD_STATE, JAVA_FLOAT a, JAVA_FLOAT b){
    if(a > b) return a;
    return b;
}

JAVA_INT java_lang_Math_max___int_int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_INT a, JAVA_INT b){
    if(a > b) return a;
    return b;
}

JAVA_LONG java_lang_Math_max___long_long_R_long(CODENAME_ONE_THREAD_STATE, JAVA_LONG a, JAVA_LONG b){
    if(a > b) return a;
    return b;
}

JAVA_DOUBLE java_lang_Math_min___double_double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a, JAVA_DOUBLE b){
    if(a < b) return a;
    return b;
}

JAVA_FLOAT java_lang_Math_min___float_float_R_float(CODENAME_ONE_THREAD_STATE, JAVA_FLOAT a, JAVA_FLOAT b){
    if(a < b) return a;
    return b;
}

JAVA_INT java_lang_Math_min___int_int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_INT a, JAVA_INT b){
    if(a < b) return a;
    return b;
}

JAVA_LONG java_lang_Math_min___long_long_R_long(CODENAME_ONE_THREAD_STATE, JAVA_LONG a, JAVA_LONG b){
    if(a < b) return a;
    return b;
}

JAVA_DOUBLE java_lang_Math_cbrt___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    return cbrt(a);
}

JAVA_DOUBLE java_lang_Math_sqrt___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    return sqrt(a);
}

JAVA_DOUBLE java_lang_Math_tan___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    return tan(a);
}

JAVA_DOUBLE java_lang_Math_atan___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    return atan(a);
}

JAVA_DOUBLE java_lang_Math_asin___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    return asin(a);
}

JAVA_DOUBLE java_lang_Math_atan2___double_double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a, JAVA_DOUBLE b) {
    return atan2(a, b);
}

JAVA_DOUBLE java_lang_Math_log___double_R_double(CODENAME_ONE_THREAD_STATE, JAVA_DOUBLE a) {
    return log(a);
}

JAVA_BOOLEAN isClassNameEqual(const char * clsName, JAVA_ARRAY_CHAR* chrs, int length) {
    for(int i = 0 ; i < length ; i++) {
        if(clsName[i] != chrs[i]) return JAVA_FALSE;
    }
    return JAVA_TRUE;
}

JAVA_OBJECT java_lang_Class_forNameImpl___java_lang_String_R_java_lang_Class(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT className) {
    int length = java_lang_String_length___R_int(threadStateData, className);
    JAVA_ARRAY arrayData = (JAVA_ARRAY)java_lang_String_toCharNoCopy___R_char_1ARRAY(threadStateData, className);
    JAVA_ARRAY_CHAR* chrs = arrayData->data;

    for(int iter = 0 ; iter < classListSize ; iter++) {
        if(strlen(classesList[iter]->clsName) == length) {
            if(!isClassNameEqual(classesList[iter]->clsName, chrs, length)) {
                continue;
            }
            return (JAVA_OBJECT)classesList[iter];
        }
    }
    return JAVA_NULL;
}

JAVA_OBJECT java_lang_Class_getComponentType___R_java_lang_Class(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    if (((struct clazz*)cls)->isArray) {
        return (JAVA_OBJECT)((struct clazz*)cls)->arrayType;
    }
    return JAVA_NULL;
}

JAVA_OBJECT java_lang_Class_getName___R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    return newStringFromCString(threadStateData, clz->clsName);
}

JAVA_BOOLEAN java_lang_Class_isArray___R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    return clz->isArray;
}

JAVA_BOOLEAN java_lang_Class_isAssignableFrom___java_lang_Class_R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls, JAVA_OBJECT cls2) {
    struct clazz* clz1 = (struct clazz*)cls;
    struct clazz* clz2 = (struct clazz*)cls2;
    return instanceofFunction(clz1->classId, clz2->classId);
}

JAVA_BOOLEAN java_lang_Class_isInstance___java_lang_Object_R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls, JAVA_OBJECT obj) {
    struct clazz* clz1 = (struct clazz*)cls;
    struct clazz* clz2 = (struct clazz*)obj->__codenameOneParentClsReference;
    return instanceofFunction(clz2->classId, clz1->classId);
}

JAVA_BOOLEAN java_lang_Class_isInterface___R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    return clz->isInterface;
}

JAVA_BOOLEAN java_lang_Class_isSynthetic___R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    return clz->isSynthetic;
}

JAVA_BOOLEAN java_lang_Class_isPrimitive___R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    return clz->primitiveType;
}

JAVA_BOOLEAN java_lang_Class_isAnonymousClass___R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    return clz->isAnonymous;
}

JAVA_OBJECT java_lang_Class_getDeclaredFields___R_java_lang_reflect_Field_1ARRAY(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    JAVA_OBJECT fields = __NEW_ARRAY_java_lang_reflect_Field(threadStateData, clz->fieldCount);
    JAVA_ARRAY array = (JAVA_ARRAY)fields;
    for (int i = 0; i < clz->fieldCount; i++) {
        JAVA_OBJECT field = __NEW_java_lang_reflect_Field(threadStateData);
        struct Field fieldStruct = clz->fields[i];
        java_lang_reflect_Field___INIT_____int_java_lang_Class_java_lang_Class_java_lang_String_int(threadStateData, field, i, (JAVA_OBJECT)clz,
                (JAVA_OBJECT)fieldStruct.type, fromNativeString(threadStateData, fieldStruct.name), fieldStruct.modifiers);
        ((JAVA_OBJECT *)array->data)[i] = field;
    }
    return (JAVA_OBJECT)fields;
}

JAVA_BOOLEAN java_lang_Class_isAnnotation___R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    return clz->isAnnotation;
}

JAVA_BOOLEAN java_lang_Class_isEnum___R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    return (clz->enumValueOfFp != 0);
}

JAVA_OBJECT java_lang_Class_newInstanceImpl___R_java_lang_Object(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls) {
    struct clazz* clz = (struct clazz*)cls;
    newInstanceFunctionPointer f = clz->newInstanceFp;
    return f(threadStateData);
}

JAVA_OBJECT java_lang_Class_getSuperclass___R_java_lang_Class(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  cls) {
    struct clazz* clz = (struct clazz*)cls;
    return (JAVA_OBJECT)clz->baseClass;
}

JAVA_OBJECT java_lang_reflect_Field_get___java_lang_Object_R_java_lang_Object(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT fieldObj, JAVA_OBJECT object) {
    struct obj__java_lang_reflect_Field* field = (struct obj__java_lang_reflect_Field*)fieldObj;
    struct Field *fieldStruct = &(((struct clazz *)field->java_lang_reflect_Field_declaringClass)->fields)[field->java_lang_reflect_Field_index];

    if (fieldStruct->type == &class__java_lang_Byte) {
        if (object == JAVA_NULL)
            return java_lang_Byte_valueOf___byte_R_java_lang_Byte(threadStateData, ((JAVA_BYTE (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData));
        else
            return java_lang_Byte_valueOf___byte_R_java_lang_Byte(threadStateData, ((JAVA_BYTE (*)(JAVA_OBJECT)) fieldStruct->getter)(object));
    } else if (fieldStruct->type == &class__java_lang_Character) {
        if (object == JAVA_NULL)
            return java_lang_Character_valueOf___char_R_java_lang_Character(threadStateData, ((JAVA_CHAR (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData));
        else
            return java_lang_Character_valueOf___char_R_java_lang_Character(threadStateData, ((JAVA_CHAR (*)(JAVA_OBJECT)) fieldStruct->getter)(object));
    } else if (fieldStruct->type == &class__java_lang_Short) {
        if (object == JAVA_NULL)
            return java_lang_Short_valueOf___short_R_java_lang_Short(threadStateData, ((JAVA_SHORT (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData));
        else
            return java_lang_Short_valueOf___short_R_java_lang_Short(threadStateData, ((JAVA_SHORT (*)(JAVA_OBJECT)) fieldStruct->getter)(object));
    } else if (fieldStruct->type == &class__java_lang_Integer) {
        if (object == JAVA_NULL)
            return java_lang_Integer_valueOf___int_R_java_lang_Integer(threadStateData, ((JAVA_INT (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData));
        else
            return java_lang_Integer_valueOf___int_R_java_lang_Integer(threadStateData, ((JAVA_INT (*)(JAVA_OBJECT)) fieldStruct->getter)(object));
    } else if (fieldStruct->type == &class__java_lang_Long) {
        if (object == JAVA_NULL)
            return java_lang_Long_valueOf___long_R_java_lang_Long(threadStateData, ((JAVA_LONG (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData));
        else
            return java_lang_Long_valueOf___long_R_java_lang_Long(threadStateData, ((JAVA_LONG (*)(JAVA_OBJECT)) fieldStruct->getter)(object));
    } else if (fieldStruct->type == &class__java_lang_Boolean) {
        if (object == JAVA_NULL)
            return java_lang_Boolean_valueOf___boolean_R_java_lang_Boolean(threadStateData, ((JAVA_BOOLEAN (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData));
        else
            return java_lang_Boolean_valueOf___boolean_R_java_lang_Boolean(threadStateData, ((JAVA_BOOLEAN (*)(JAVA_OBJECT)) fieldStruct->getter)(object));
    } else if (fieldStruct->type == &class__java_lang_Float) {
        if (object == JAVA_NULL)
            return java_lang_Float_valueOf___float_R_java_lang_Float(threadStateData, ((JAVA_FLOAT (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData));
        else
            return java_lang_Float_valueOf___float_R_java_lang_Float(threadStateData, ((JAVA_FLOAT (*)(JAVA_OBJECT)) fieldStruct->getter)(object));
    } else if (fieldStruct->type == &class__java_lang_Double) {
        if (object == JAVA_NULL)
            return java_lang_Double_valueOf___double_R_java_lang_Double(threadStateData, ((JAVA_DOUBLE (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData));
        else
            return java_lang_Double_valueOf___double_R_java_lang_Double(threadStateData, ((JAVA_DOUBLE (*)(JAVA_OBJECT)) fieldStruct->getter)(object));
    } else
        return object == JAVA_NULL ? ((JAVA_OBJECT (*)(struct ThreadLocalData *)) fieldStruct->getter)(threadStateData) : ((JAVA_OBJECT (*)(JAVA_OBJECT)) fieldStruct->getter)(object);
}

JAVA_VOID java_lang_reflect_Field_set___java_lang_Object_java_lang_Object(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT fieldObj, JAVA_OBJECT object, JAVA_OBJECT value) {
    struct obj__java_lang_reflect_Field* field = (struct obj__java_lang_reflect_Field*)fieldObj;
    struct Field *fieldStruct = &(((struct clazz *)field->java_lang_reflect_Field_declaringClass)->fields)[field->java_lang_reflect_Field_index];

    if (fieldStruct->type == &class__java_lang_Byte) {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_BYTE, JAVA_OBJECT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Byte *)value)->java_lang_Byte_value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_BYTE))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Byte *)value)->java_lang_Byte_value);
    } else if (fieldStruct->type == &class__java_lang_Character) {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_CHAR, JAVA_OBJECT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Character *)value)->java_lang_Character_value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_CHAR))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Character *)value)->java_lang_Character_value);
    } else if (fieldStruct->type == &class__java_lang_Short) {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_SHORT, JAVA_OBJECT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Short *)value)->java_lang_Short_value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_SHORT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Short *)value)->java_lang_Short_value);
    } else if (fieldStruct->type == &class__java_lang_Integer) {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_INT, JAVA_OBJECT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Integer *)value)->java_lang_Integer_value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_INT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Integer *)value)->java_lang_Integer_value);
    } else if (fieldStruct->type == &class__java_lang_Long) {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_LONG, JAVA_OBJECT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Long *)value)->java_lang_Long_value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_LONG))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Long *)value)->java_lang_Long_value);
    } else if (fieldStruct->type == &class__java_lang_Boolean) {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_BOOLEAN, JAVA_OBJECT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Boolean *)value)->java_lang_Boolean_value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_BOOLEAN))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Boolean *)value)->java_lang_Boolean_value);
    } else if (fieldStruct->type == &class__java_lang_Float) {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_FLOAT, JAVA_OBJECT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Float *)value)->java_lang_Float_value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_FLOAT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Float *)value)->java_lang_Float_value);
    } else if (fieldStruct->type == &class__java_lang_Double) {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_DOUBLE, JAVA_OBJECT))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Double *)value)->java_lang_Double_value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_DOUBLE))fieldStruct->setter)(threadStateData, ((struct obj__java_lang_Double *)value)->java_lang_Double_value);
    } else {
        if (object != JAVA_NULL)
            ((void (*)(struct ThreadLocalData *, JAVA_OBJECT, JAVA_OBJECT))fieldStruct->setter)(threadStateData, value, object);
        else if (fieldStruct->setter)
            ((void (*)(struct ThreadLocalData *, JAVA_OBJECT))fieldStruct->setter)(threadStateData, value);
    }
}

JAVA_OBJECT java_lang_Enum_valueOf___java_lang_Class_java_lang_String_R_java_lang_Enum(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT cls, JAVA_OBJECT value) {
    struct clazz* clz = (struct clazz*)cls;
    enumValueOfFunctionPointer f = clz->enumValueOfFp;
    if (f == 0) {
        return JAVA_NULL;
    }
    return f(threadStateData, value);
}

JAVA_OBJECT java_lang_Object_toString___R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    if (obj == JAVA_NULL) {
        return newStringFromCString(threadStateData, "null");
    } else {
        struct clazz* cls = obj->__codenameOneParentClsReference;
        const char* className = cls->clsName;
        char s[strlen(className) + 32];
        sprintf(s, "%s@%llX", className, ((JAVA_LONG)obj));
        return newStringFromCString(threadStateData, s);
    }
}

void initClazzClazz() {
    if(!ClazzClazz.initialized) {
        ClazzClazz.initialized = JAVA_TRUE;

        ClazzClazz.vtable = malloc(sizeof(void*) *10);
        ClazzClazz.vtable[0] = &java_lang_Object_equals___java_lang_Object_R_boolean;
        ClazzClazz.vtable[1] = &java_lang_Object_getClass___R_java_lang_Class;
        ClazzClazz.vtable[2] = &java_lang_Object_hashCode___R_int;
        ClazzClazz.vtable[3] = &java_lang_Object_notify__;
        ClazzClazz.vtable[4] = &java_lang_Object_notifyAll__;
        ClazzClazz.vtable[5] = &java_lang_Object_toString___R_java_lang_String;
        ClazzClazz.vtable[6] = &java_lang_Object_wait__;
        ClazzClazz.vtable[7] = &java_lang_Object_wait___long;
        ClazzClazz.vtable[8] = &java_lang_Object_wait___long_int;
    }
}

JAVA_OBJECT java_lang_Object_getClassImpl___R_java_lang_Class(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    initClazzClazz();
    if(!obj->__codenameOneParentClsReference) {
        return (JAVA_OBJECT)(&ClazzClazz);
    }
    obj->__codenameOneParentClsReference->__codenameOneParentClsReference = &ClazzClazz;
    return (JAVA_OBJECT)obj->__codenameOneParentClsReference;
}

JAVA_INT java_lang_Class_hashCode___R_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    return (JAVA_INT)(long)obj;
}

JAVA_INT java_lang_Object_hashCode___R_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    return (JAVA_INT)(long)obj;
}

struct ThreadLocalData** allThreads = 0;
int nThreadsToKill = 0;         // the number of threads we expect to be finalized, eventually

pthread_key_t   threadIdKey = 0;
JAVA_LONG threadKeyCounter = 1;
struct ThreadLocalData* getThreadLocalData() {
    if(threadIdKey == 0) {
        pthread_key_create(&threadIdKey, NULL);
    }
    struct ThreadLocalData* i = pthread_getspecific(threadIdKey);
    if(i == NULL) {
        JAVA_LONG nativeThreadId = threadKeyCounter;
        threadKeyCounter++;
        i = malloc(sizeof(struct ThreadLocalData));
        i->threadId = nativeThreadId;
        i->tryBlockOffset = 0;

        i->lightweightThread = JAVA_FALSE;
        i->threadBlockedByGC = JAVA_FALSE;
        i->threadActive = JAVA_FALSE;
        i->threadKilled = JAVA_FALSE;

        i->currentThreadObject = 0;

        i->utf8Buffer = 0;
        i->utf8BufferSize = 0;
        i->threadObjectStack = malloc(CN1_MAX_OBJECT_STACK_DEPTH * sizeof(struct elementStruct));
        memset(i->threadObjectStack, 0, CN1_MAX_OBJECT_STACK_DEPTH * sizeof(struct elementStruct));
        i->threadObjectStackOffset = 0;

        i->callStackClass = malloc(CN1_MAX_STACK_CALL_DEPTH * sizeof(int));
        memset(i->callStackClass, 0, CN1_MAX_STACK_CALL_DEPTH * sizeof(int));

        i->callStackLine = malloc(CN1_MAX_STACK_CALL_DEPTH * sizeof(int));
        memset(i->callStackLine, 0, CN1_MAX_STACK_CALL_DEPTH * sizeof(int));

        i->callStackMethod = malloc(CN1_MAX_STACK_CALL_DEPTH * sizeof(int));
        memset(i->callStackMethod, 0, CN1_MAX_STACK_CALL_DEPTH * sizeof(int));

        i->callStackOffset = 0;

        i->pendingHeapAllocations = malloc(PER_THREAD_ALLOCATION_COUNT * sizeof(void *));
        memset(i->pendingHeapAllocations, 0, PER_THREAD_ALLOCATION_COUNT * sizeof(void *));
        i->heapAllocationSize = 0;
        i->threadHeapTotalSize = PER_THREAD_ALLOCATION_COUNT;

        i->blocks = malloc(500 * sizeof(struct TryBlock));
        pthread_setspecific(threadIdKey, i);

        i->thread = pthread_self();

        if(!allThreads) {
            allThreads = malloc(NUMBER_OF_SUPPORTED_THREADS * sizeof(struct ThreadLocalData*));
            memset(allThreads, 0, NUMBER_OF_SUPPORTED_THREADS * sizeof(struct ThreadLocalData*));
        }
        int threadOffset = -1;
        lockCriticalSection();
        for(int iter = 0 ; iter < NUMBER_OF_SUPPORTED_THREADS ; iter++) {
            if(allThreads[iter] == 0) {
                threadOffset = iter;
                break;
            }
        }
        CODENAME_ONE_ASSERT(threadOffset > -1);
        allThreads[threadOffset] = i;
        unlockCriticalSection();
        //NSLog(@"Thread slot %d assigned to thread %d",threadOffset,(int)i->threadId);
    }
    return i;
}

JAVA_LONG currentThreadId() {
    struct ThreadLocalData* i = getThreadLocalData();
    return i->threadId;
}

pthread_mutex_t* criticalSection = NULL;
pthread_mutex_t* getCriticalSection() {
    if(criticalSection == NULL) {
        criticalSection = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(criticalSection, NULL);
    }
    return criticalSection;
}

void lockCriticalSection() {
    pthread_mutex_lock(getCriticalSection());
}

void unlockCriticalSection() {
    pthread_mutex_unlock(criticalSection);
}
pthread_mutex_t* threadHeapMutex = NULL;
pthread_mutex_t* getThreadHeapMutex() {
    if(threadHeapMutex == NULL) {
        threadHeapMutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(threadHeapMutex, NULL);
    }
    return threadHeapMutex;
}

void lockThreadHeapMutex() {
    pthread_mutex_lock(getThreadHeapMutex());
}

void unlockThreadHeapMutex() {
    pthread_mutex_unlock(getThreadHeapMutex());
}

extern void flushReleaseQueue();
long gcThreadId = -1;
JAVA_VOID java_lang_System_gcLight__(CODENAME_ONE_THREAD_STATE) {
    gcThreadId = (long)threadStateData->threadId;
    flushReleaseQueue();
}

JAVA_BOOLEAN firstTimeGcThread = JAVA_TRUE;
JAVA_BOOLEAN gcCurrentlyRunning = JAVA_FALSE;
JAVA_VOID java_lang_System_gcMarkSweep__(CODENAME_ONE_THREAD_STATE) {
    gcCurrentlyRunning = JAVA_TRUE;
    if(firstTimeGcThread) {
        firstTimeGcThread = JAVA_FALSE;

        // reduce thread priority
//        int policy;
//        struct sched_param param;
//        pthread_getschedparam(pthread_self(), &policy, &param);
//        param.sched_priority--;
//        pthread_setschedparam(pthread_self(), policy, &param);
    }
    flushReleaseQueue();
    codenameOneGCMark();
    codenameOneGCSweep();
    flushReleaseQueue();
    lowMemoryMode = JAVA_FALSE;
    gcCurrentlyRunning = JAVA_FALSE;
}

JAVA_VOID java_lang_System_exit0___int(CODENAME_ONE_THREAD_STATE, JAVA_INT code) {
#ifdef __SWITCH__
    pthread_t self = pthread_self();
    for (int i = 0; i < NUMBER_OF_SUPPORTED_THREADS; i++) {
        struct ThreadLocalData* t = allThreads[i];
        if (t && t->thread && t->thread != self)
            pthread_join(t->thread, NULL);
    }
    pthread_join(gcThread, NULL);
#else
    exit(code);
#endif
}

JAVA_VOID monitorEnter(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    int err = 0;
    // we need to synchronize the mutex initialization since there might be a race condition here
    if(!obj->__codenameOneThreadData) {
        // double locking to avoid race condition and improve performance
        lockCriticalSection();
        if(!obj->__codenameOneThreadData) {
            obj->__codenameOneThreadData = malloc(sizeof(struct CN1ThreadData));
            memset(obj->__codenameOneThreadData, 0, sizeof(struct CN1ThreadData));
            pthread_mutex_init(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneMutex, NULL);
            pthread_cond_init(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneCondition, NULL);
        }
        unlockCriticalSection();
        err = pthread_mutex_lock(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneMutex);
        ((struct CN1ThreadData*)obj->__codenameOneThreadData)->ownerThread = threadStateData->threadId;
        ((struct CN1ThreadData*)obj->__codenameOneThreadData)->counter++;
    } else {
        JAVA_LONG own = threadStateData->threadId;
        JAVA_LONG currentlyHeldBy = ((struct CN1ThreadData*)obj->__codenameOneThreadData)->ownerThread;

        // we already own the lock...
        if(currentlyHeldBy == own) {
            ((struct CN1ThreadData*)obj->__codenameOneThreadData)->counter++;
            return;
        }
        threadStateData->threadActive = JAVA_FALSE;
        err = pthread_mutex_lock(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneMutex);
        ((struct CN1ThreadData*)obj->__codenameOneThreadData)->counter++;
        ((struct CN1ThreadData*)obj->__codenameOneThreadData)->ownerThread = own;
        while (threadStateData->threadBlockedByGC) {
            usleep(100);
        }
        threadStateData->threadActive = JAVA_TRUE;


    }
    //NSLog(@"Locking mutex %i started from %@", (int)obj->__codenameOneMutex, [NSThread callStackSymbols]);
    //NSLog(@"Locking mutex %i completed", (int)obj->__codenameOneMutex);
    if(err != 0) {
        NSLog(@"Error with lock %i EINVAL %i, ETIMEDOUT %i, EPERM %i", err, EINVAL, ETIMEDOUT, EPERM);
    }
}

// monitorEnterBlock is used for synchronized methods because the JVM bytecode
// doesn't actually generate the "try/catch" blocks for us like they do with
// synchronized blocks.  monitorEnterBlock will add a "block" to the block
// stack so that throwException() can exit the block in the case that an exception
// is thrown.
JAVA_VOID monitorEnterBlock(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    monitorEnter(threadStateData, obj);
    threadStateData->blocks[threadStateData->tryBlockOffset].monitor = obj;
    threadStateData->tryBlockOffset++;
}

JAVA_VOID monitorExit(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    //NSLog(@"Unlocked mutex %i ", (int)obj->__codenameOneMutex);
    // remove the ownership of the thread
    ((struct CN1ThreadData*)obj->__codenameOneThreadData)->counter--;
    if(((struct CN1ThreadData*)obj->__codenameOneThreadData)->counter > 0) {
        return;
    }
    ((struct CN1ThreadData*)obj->__codenameOneThreadData)->ownerThread = 0;
    int err = pthread_mutex_unlock(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneMutex);
    if(err != 0) {
        NSLog(@"Error with unlock %i EINVAL %i, ETIMEDOUT %i, EPERM %i", err, EINVAL, ETIMEDOUT, EPERM);
    }
}

// monitorEnterBlock is used for synchronized methods because the JVM bytecode
// doesn't actually generate the "try/catch" blocks for us like they do with
// synchronized blocks.  monitorEnterBlock will add a "block" to the block
// stack so that throwException() can exit the block in the case that an exception
// is thrown.
JAVA_VOID monitorExitBlock(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    threadStateData->tryBlockOffset--;
    monitorExit(threadStateData, obj);
}

JAVA_VOID java_lang_Object_wait___long_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj, JAVA_LONG timeout, JAVA_INT nanos) {
    //NSLog(@"Waiting on mutex %i with timeout %i started", (int)obj->__codenameOneMutex, (int)timeout);
    threadStateData->threadActive = JAVA_FALSE;

    int counter;
    counter = ((struct CN1ThreadData*)obj->__codenameOneThreadData)->counter;

    // remove the ownership of the thread
    ((struct CN1ThreadData*)obj->__codenameOneThreadData)->ownerThread = 0;
    ((struct CN1ThreadData*)obj->__codenameOneThreadData)->counter = 0;

    int errCode = 0;
    if(timeout == 0 && nanos == 0) {
        errCode = pthread_cond_wait(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneCondition, &((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneMutex);
        if(errCode != 0) {
            NSLog(@"Error with wait %i EINVAL %i, ETIMEDOUT %i, EPERM %i", errCode, EINVAL, ETIMEDOUT, EPERM);
        }
    } else {
        struct timeval   tv;
        gettimeofday(&tv, NULL);
        struct timespec   ts;
        ts.tv_sec = tv.tv_sec + (long)(timeout / 1000);
        ts.tv_nsec = tv.tv_usec * 1000 + (timeout % 1000) * 1000000 + nanos;
        if ( ts.tv_nsec > 1000000000 ){
            ts.tv_nsec -= 1000000000;
            ts.tv_sec++;
        }
        pthread_cond_timedwait(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneCondition, &((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneMutex, &ts);
    }

    while(threadStateData->threadBlockedByGC) {
        struct timeval   tv;
        gettimeofday(&tv, NULL);
        struct timespec   ts;
        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = (tv.tv_usec * 1000) + 2000000;
        if ( ts.tv_nsec > 1000000000 ){
            ts.tv_nsec -= 1000000000;
            ts.tv_sec++;
        }
        pthread_cond_timedwait(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneCondition, &((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneMutex, &ts);
    }

    // restore the ownership of the thread
    ((struct CN1ThreadData*)obj->__codenameOneThreadData)->ownerThread = threadStateData->threadId;
    ((struct CN1ThreadData*)obj->__codenameOneThreadData)->counter = counter;

    threadStateData->threadActive = JAVA_TRUE;
    //NSLog(@"Waiting on mutex %i with timeout %i finished", (int)obj->__codenameOneMutex, (int)timeout);
}

JAVA_VOID java_lang_Object_notify__(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    //NSLog(@"Notifying mutex %i", (int)obj->__codenameOneMutex);
    pthread_cond_signal(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneCondition);
}

JAVA_VOID java_lang_Object_notifyAll__(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT obj) {
    //NSLog(@"Notifying all mutex threads %i", (int)obj->__codenameOneMutex);
    pthread_cond_broadcast(&((struct CN1ThreadData*)obj->__codenameOneThreadData)->__codenameOneCondition);
}

JAVA_VOID java_lang_Thread_setPriorityImpl___int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT t, JAVA_INT p) {
}

JAVA_VOID java_lang_Thread_releaseThreadNativeResources___long(CODENAME_ONE_THREAD_STATE, JAVA_LONG nativeThreadStruct) {
    // if a thread object was created and never started, it will still become garbage
    // and will still be finalized.  In that case, it never had resources allocated at all.
    if(nativeThreadStruct!=0)
    {
        struct ThreadLocalData *head = (struct ThreadLocalData *)nativeThreadStruct;
        free(head->blocks);
        free(head->threadObjectStack);
        free(head->callStackClass);
        free(head->callStackLine);
        free(head->callStackMethod);
        free(head->pendingHeapAllocations);
        free(head);
        nThreadsToKill--;
    }
}

JAVA_VOID java_lang_Thread_sleep___long(CODENAME_ONE_THREAD_STATE, JAVA_LONG millis) {
    threadStateData->threadActive = JAVA_FALSE;
    usleep((JAVA_INT)(millis * 1000));
    while(threadStateData->threadBlockedByGC) {
        usleep(1000);
    }
    threadStateData->threadActive = JAVA_TRUE;
}

JAVA_OBJECT java_lang_Thread_currentThread___R_java_lang_Thread(CODENAME_ONE_THREAD_STATE) {
    if(threadStateData->currentThreadObject == JAVA_NULL) {
        threadStateData->currentThreadObject = __NEW_INSTANCE_java_lang_Thread(threadStateData);
    }
    return threadStateData->currentThreadObject;
}
extern void collectThreadResources(struct ThreadLocalData *current);
void markDeadThread(struct ThreadLocalData *d)
{
    lockCriticalSection();
    int found = -1;
    for(int iter = 0 ; iter < NUMBER_OF_SUPPORTED_THREADS ; iter++) {
        if(allThreads[iter] == d) {
            allThreads[iter] = 0;
            d->threadKilled = JAVA_TRUE;
            d->threadActive = JAVA_FALSE;
            found = iter;
            nThreadsToKill++;
            collectThreadResources(d);
            break;
        }
    }
    unlockCriticalSection();

    if(found>=0)
    {
        //  NSLog(@"Deleting thread slot %i id %d", found,(int)d->threadId);
    }
    else
    {
        NSLog(@"Thread %d not found !!",(int)d->threadId);
    }

}
void* threadRunner(void *x)
{
    JAVA_OBJECT t = (JAVA_OBJECT)x;
    struct ThreadLocalData* d = getThreadLocalData();
    d->lightweightThread = JAVA_TRUE;
    d->threadActive = JAVA_TRUE;
    d->currentThreadObject = t;

    // NSLog(@"launching thread %d",(int)d->threadId);
    java_lang_Thread_runImpl___long(d, t, (long)d); // pass the actual structure as threadid
    // NSLog(@"terminate thread %d",(int)d->threadId);

    // we remove the thread here since this is the only place we can do this
    // we add the thread in the getThreadLocalData() method to handle native threads
    // too. Hopefully we won't spawn too many of those...

    markDeadThread(d);

    /*free(d->blocks);
    free(d->threadObjectStack);
    free(d->callStackClass);
    free(d->callStackLine);
    free(d->callStackMethod);
    free(d);*/

    return NULL;
}

JAVA_VOID java_lang_Thread_start__(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT th) {
    // disable reference counting on the thread object to prevent the gap between thread start and actual thread running
    th->__codenameOneReferenceCount = 999999;
    pthread_t pt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // create the thread detached, as we never join.  This
    // fixes the "error 35" problem that occurred after a
    // finite number of threads. [ddyer 5/2017]
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int rc = pthread_create(&pt, &attr, threadRunner, (void *)th);
    if (rc != 0) {
        NSLog("ERROR creating thread. Return code: %i", rc);
        exit(-1);
    }
    pthread_attr_destroy(&attr);

    // First thread created will be GC thread
    if (!gcThread)
        gcThread = pt;
}

JAVA_LONG java_lang_Thread_getNativeThreadId___R_long(CODENAME_ONE_THREAD_STATE) {
    return currentThreadId();
}

JAVA_DOUBLE java_lang_StringToReal_parseDblImpl___java_lang_String_int_R_double(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT s, JAVA_INT e) {
    int length = java_lang_String_length___R_int(threadStateData, s);
    JAVA_ARRAY arrayData = (JAVA_ARRAY)java_lang_String_toCharNoCopy___R_char_1ARRAY(threadStateData, s);
    JAVA_ARRAY_CHAR* chrs = arrayData->data;
    char data[length + 1];
    for(int iter = 0 ; iter < length ; iter++) {
        data[iter] = (char)chrs[iter];
    }
    data[length] = 0;
    char *err;
    JAVA_DOUBLE db = strtod(data, &err);
    if (data == err) {
        JAVA_OBJECT numberFormatException = java_lang_StringToReal_invalidReal___java_lang_String_boolean_R_java_lang_NumberFormatException(threadStateData, s, JAVA_TRUE);
        throwException(threadStateData,numberFormatException);
    }
    JAVA_LONG exp = 1;
    if(e != 0) {
        if(e < 0) {
            while (e < -18) {
                // Long accumulator will overflow past 18 digits so we do
                // floating point math until we get there.
                // fixes https://github.com/codenameone/CodenameOne/issues/3250
                e++;
                db /= 10;
            }
            while(e < 0) {
                e++;
                exp *= 10;
            }
            db /= exp;
        } else {
            while (e > 18) {
                // Long accumulator will overflow past 18 digits so we do
                // floating point math until we get there.
                // fixes https://github.com/codenameone/CodenameOne/issues/3250
                e--;
                db /= 10;
            }
            while(e > 0) {
                e--;
                exp *= 10;
            }
            db *= exp;
        }
    }
    return db;
}

void initMethodStack(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT __cn1ThisObject, int stackSize, int localsStackSize, int classNameId, int methodNameId) {
#ifdef CN1_INCLUDE_NPE_CHECKS
    if(__cn1ThisObject == JAVA_NULL) {
        THROW_NULL_POINTER_EXCEPTION();
    }
#endif
    memset(&threadStateData->threadObjectStack[threadStateData->threadObjectStackOffset], 0, sizeof(struct elementStruct) * (localsStackSize + stackSize));
    threadStateData->threadObjectStackOffset += localsStackSize + stackSize;
    CODENAME_ONE_ASSERT(threadStateData->callStackOffset < CN1_MAX_STACK_CALL_DEPTH - 1);
    threadStateData->callStackClass[threadStateData->callStackOffset] = classNameId;
    threadStateData->callStackMethod[threadStateData->callStackOffset] = methodNameId;
    threadStateData->callStackOffset++;
}

void releaseForReturn(CODENAME_ONE_THREAD_STATE, int cn1LocalsBeginInThread) {
    threadStateData->threadObjectStackOffset = cn1LocalsBeginInThread;
    threadStateData->callStackOffset--;
}

void releaseForReturnInException(CODENAME_ONE_THREAD_STATE, int cn1LocalsBeginInThread, int methodBlockOffset) {
    threadStateData->tryBlockOffset = methodBlockOffset;
    threadStateData->threadObjectStackOffset = cn1LocalsBeginInThread;
    threadStateData->callStackOffset--;
}

// Todo: Replace with actual values
JAVA_LONG java_lang_Runtime_totalMemoryImpl___R_long(CODENAME_ONE_THREAD_STATE) {
    return 100000000;
//    return [NSProcessInfo processInfo].physicalMemory;
}

JAVA_LONG java_lang_Runtime_freeMemoryImpl___R_long(CODENAME_ONE_THREAD_STATE) {
    return 100000000;
//     struct task_basic_info info;
//     mach_msg_type_number_t size = sizeof(info);
//     kern_return_t kerr = task_info(mach_task_self(),
//                                    TASK_BASIC_INFO,
//                                    (task_info_t)&info,
//                                    &size);
//     return [NSProcessInfo processInfo].physicalMemory - info.resident_size;
}


JAVA_BOOLEAN java_util_HashMap_areEqualKeys___java_lang_Object_java_lang_Object_R_boolean(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT __cn1Arg1, JAVA_OBJECT __cn1Arg2) {
    if(__cn1Arg1 == __cn1Arg2) {
        return JAVA_TRUE;
    }
    return virtual_java_lang_Object_equals___java_lang_Object_R_boolean(threadStateData, __cn1Arg1, __cn1Arg2);
}

JAVA_OBJECT java_util_HashMap_findNonNullKeyEntry___java_lang_Object_int_int_R_java_util_HashMap_Entry(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_OBJECT key, JAVA_INT index, JAVA_INT keyHash) {
    struct obj__java_util_HashMap* t = (struct obj__java_util_HashMap*)__cn1ThisObject;
    JAVA_ARRAY_OBJECT* obj = ((JAVA_ARRAY)t->java_util_HashMap_elementData)->data;
    struct obj__java_util_HashMap_Entry* m = (struct obj__java_util_HashMap_Entry*)obj[index];
    while (m != 0
           && (m->java_util_HashMap_Entry_origKeyHash != keyHash || !java_util_HashMap_areEqualKeys___java_lang_Object_java_lang_Object_R_boolean(threadStateData, key, m->java_util_MapEntry_key))) {
        m = (struct obj__java_util_HashMap_Entry*)m->java_util_HashMap_Entry_next;
    }
    return (JAVA_OBJECT)m;
}

JAVA_OBJECT java_util_Locale_getOSLanguage___R_java_lang_String(CODENAME_ONE_THREAD_STATE) {
#ifdef __SWITCH__
    u64 languageCode;
    setGetSystemLanguage(&languageCode);
    return fromNativeString(threadStateData, (char *)&languageCode);
#else
    return fromNativeString(threadStateData, "en-US");
#endif
}

// Todo: Fix
JAVA_OBJECT java_text_DateFormat_format___java_util_Date_java_lang_StringBuffer_R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_OBJECT __cn1Arg1, JAVA_OBJECT __cn1Arg2) {
    return fromNativeString(threadStateData, "Date?");
//    struct obj__java_text_DateFormat* df = (struct obj__java_text_DateFormat*)__cn1ThisObject;
//     POOL_BEGIN();
//#ifndef CN1_USE_ARC
//    NSDateFormatter *formatter = [[[NSDateFormatter alloc] init] autorelease];
//#else
//    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
//#endif
//    struct obj__java_util_Date* dateObj = (struct obj__java_util_Date*)__cn1Arg1;
//    NSDate* date = [NSDate dateWithTimeIntervalSince1970:(dateObj->java_util_Date_date / 1000)];
//
//    switch (df->java_text_DateFormat_dateStyle) {
//        // no date - only time
//        case -1:
//            [formatter setDateStyle:NSDateFormatterNoStyle];
//            switch (df->java_text_DateFormat_dateStyle) {
//                // long time format
//                case 1:
//                    [formatter setTimeStyle:NSDateFormatterLongStyle];
//                    break;
//
//                // medium time format
//                case 2:
//                    [formatter setTimeStyle:NSDateFormatterMediumStyle];
//                    break;
//
//                // short time format
//                case 3:
//                    [formatter setTimeStyle:NSDateFormatterShortStyle];
//                    break;
//
//                // full time format
//                default:
//                    [formatter setTimeStyle:NSDateFormatterFullStyle];
//                    break;
//            }
//            break;
//
//        // long date format
//        case 1:
//            [formatter setDateStyle:NSDateFormatterLongStyle];
//            break;
//
//        // medium date length
//        case 2:
//            [formatter setDateStyle:NSDateFormatterMediumStyle];
//            break;
//
//        // short date length
//        case 3:
//            [formatter setDateStyle:NSDateFormatterShortStyle];
//            break;
//
//        // full date
//        default:
//            [formatter setDateStyle:NSDateFormatterFullStyle];
//            break;
//    }
//    JAVA_OBJECT str = fromNSString(CN1_THREAD_STATE_PASS_ARG [formatter stringFromDate:date]);
//     POOL_END();
//
//    return str;
}


JAVA_CHAR java_lang_String_charAt___int_R_char(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_INT __cn1Arg1) {
    //DEFINE_INSTANCE_METHOD_STACK(3, 2, 0, 752, 755);
    struct obj__java_lang_String* encString = (struct obj__java_lang_String*)__cn1ThisObject;
    JAVA_ARRAY arr =(JAVA_ARRAY)(encString->java_lang_String_value);
    if(__cn1Arg1 < 0 || __cn1Arg1 >= arr->length) { THROW_ARRAY_INDEX_EXCEPTION(__cn1Arg1); }
    JAVA_ARRAY_CHAR* encArr = (JAVA_ARRAY_CHAR*)arr->data;
    JAVA_INT index = get_field_java_lang_String_offset(__cn1ThisObject)+__cn1Arg1;
    //releaseForReturn(threadStateData, cn1LocalsBeginInThread, stackPointer - 1, 2, stack, locals);
    return encArr[index];

}

JAVA_INT java_lang_String_indexOf___int_int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_INT ch, JAVA_INT fromIndex) {
    fromIndex = MAX(0, fromIndex);
    struct obj__java_lang_String* encString = (struct obj__java_lang_String*)__cn1ThisObject;
    JAVA_ARRAY_CHAR* encArr = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)(encString->java_lang_String_value))->data;
    //releaseForReturn(threadStateData, cn1LocalsBeginInThread, stackPointer - 1, 2, stack, locals);
    int off = get_field_java_lang_String_offset(__cn1ThisObject);
    int endOff = off+encString->java_lang_String_count;
    for (int i=off+fromIndex; i<endOff; i++) {
        if (encArr[i] == ch) {
            return i-off;
        }
    }
    return -1;
}

JAVA_OBJECT java_lang_String_toString___R_java_lang_String(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject) {
    return __cn1ThisObject;
}

JAVA_CHAR java_lang_StringBuilder_charAt___int_R_char(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_INT index) {
    struct obj__java_lang_StringBuilder* t = (struct obj__java_lang_StringBuilder*)__cn1ThisObject;
    if(index < 0 || index >= ((JAVA_ARRAY)t->java_lang_StringBuilder_value)->length) { THROW_ARRAY_INDEX_EXCEPTION(index); }
    JAVA_ARRAY_CHAR* dat = ((JAVA_ARRAY)t->java_lang_StringBuilder_value)->data;
    return dat[index];
}

JAVA_OBJECT java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_OBJECT str) {
    enteringNativeAllocations();
    if (str == JAVA_NULL) {
        java_lang_StringBuilder_appendNull__(threadStateData, __cn1ThisObject);
        finishedNativeAllocations();
        return __cn1ThisObject;
    }
    int length = java_lang_String_length___R_int(threadStateData, str);
    struct obj__java_lang_StringBuilder* t = (struct obj__java_lang_StringBuilder*)__cn1ThisObject;
    int newCount = t->java_lang_StringBuilder_count + length;
    if (newCount > ((JAVA_ARRAY)t->java_lang_StringBuilder_value)->length) {
        java_lang_StringBuilder_enlargeBuffer___int(threadStateData, __cn1ThisObject, newCount);
    }
    java_lang_String_getChars___int_int_char_1ARRAY_int(threadStateData, str, 0, length, t->java_lang_StringBuilder_value, t->java_lang_StringBuilder_count);
    t->java_lang_StringBuilder_count = newCount;
    finishedNativeAllocations();
    return __cn1ThisObject;

}

JAVA_VOID java_lang_StringBuilder_getChars___int_int_char_1ARRAY_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_INT start, JAVA_INT end, JAVA_OBJECT dst, JAVA_INT dstStart) {
    struct obj__java_lang_StringBuilder* t = (struct obj__java_lang_StringBuilder*)__cn1ThisObject;
    java_lang_System_arraycopy___java_lang_Object_int_java_lang_Object_int_int(threadStateData, t->java_lang_StringBuilder_value, start, dst, dstStart, end - start);
}

JAVA_OBJECT java_lang_StringBuilder_append___java_lang_Object_R_java_lang_StringBuilder(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_OBJECT obj) {
    if(obj == JAVA_NULL) {
        java_lang_StringBuilder_appendNull__(threadStateData, __cn1ThisObject);
        return __cn1ThisObject;
    }
    return java_lang_StringBuilder_append___java_lang_String_R_java_lang_StringBuilder(threadStateData, __cn1ThisObject, virtual_java_lang_Object_toString___R_java_lang_String(threadStateData, obj));
}

JAVA_OBJECT java_lang_StringBuilder_append___char_R_java_lang_StringBuilder(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_CHAR __cn1Arg1) {
    enteringNativeAllocations();
    JAVA_INT len = get_field_java_lang_StringBuilder_count(__cn1ThisObject);
    JAVA_OBJECT value = get_field_java_lang_StringBuilder_value(__cn1ThisObject);
    JAVA_INT valueLen = ((JAVA_ARRAY)value)->length;
    if (len==valueLen) {
        java_lang_StringBuilder_enlargeBuffer___int(threadStateData, __cn1ThisObject, len+1);
        value = get_field_java_lang_StringBuilder_value(__cn1ThisObject);
    }
    JAVA_ARRAY_CHAR* d = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)value)->data;
    d[len] = __cn1Arg1;
    set_field_java_lang_StringBuilder_count(threadStateData, len+1, __cn1ThisObject);
    finishedNativeAllocations();
    return __cn1ThisObject;
}

JAVA_VOID java_lang_String_getChars___int_int_char_1ARRAY_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_INT __cn1Arg1, JAVA_INT __cn1Arg2, JAVA_OBJECT __cn1Arg3, JAVA_INT __cn1Arg4) {

    JAVA_INT offset = get_field_java_lang_String_offset(__cn1ThisObject);
    JAVA_ARRAY srcArr = (JAVA_ARRAY)get_field_java_lang_String_value(__cn1ThisObject);
    JAVA_ARRAY_CHAR* src = (JAVA_ARRAY_CHAR*)srcArr->data;
    JAVA_ARRAY_CHAR* dst = (JAVA_ARRAY_CHAR*)((JAVA_ARRAY)__cn1Arg3)->data;
    int start = offset + __cn1Arg1;
    int end = offset + __cn1Arg2;
    for (JAVA_INT i=start; i<end; i++) {
        dst[__cn1Arg4++] = src[i];
    }
}

JAVA_LONG java_util_zip_Inflater_init___boolean_R_long(CODENAME_ONE_THREAD_STATE, JAVA_BOOLEAN nowrap) {
    z_streamp stream = (z_streamp)malloc(sizeof(z_streamp));
    inflateInit(stream);
    return (JAVA_LONG)stream;
}

JAVA_INT java_util_zip_Inflater_inflateBytes___long_byte_1ARRAY_int_int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_OBJECT  __cn1ThisObject, JAVA_LONG address, JAVA_OBJECT b, JAVA_INT off, JAVA_INT len) {
    z_streamp stream = (z_streamp)address;
    stream->next_out = ((JAVA_ARRAY)b)->data;
    stream->avail_out = len;
    JAVA_ARRAY buf = (JAVA_ARRAY)((struct obj__java_util_zip_Inflater*)__cn1ThisObject)->java_util_zip_Inflater_buf;
    stream->next_in = buf->data;
    stream->avail_in = buf->length;
    inflate(stream, Z_FINISH);
}

JAVA_INT java_util_zip_Inflater_getAdler___long_R_int(CODENAME_ONE_THREAD_STATE, JAVA_LONG address) {
    return (JAVA_INT)((z_streamp)address)->adler;
}

JAVA_VOID java_util_zip_Inflater_reset___long(CODENAME_ONE_THREAD_STATE, JAVA_LONG address) {
    inflateReset((z_streamp)address);
}

JAVA_VOID java_util_zip_Inflater_end___long(CODENAME_ONE_THREAD_STATE, JAVA_LONG address) {
    z_streamp stream = (z_streamp)address;
    inflateEnd(stream);
    free(stream);
}

JAVA_INT java_util_zip_CRC32_update___int_int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_INT crc, JAVA_INT b) {
    unsigned char byte = b;
    crc32(crc, &byte, 1);
}

JAVA_INT java_util_zip_CRC32_updateBytes___int_byte_1ARRAY_int_int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_INT crc, JAVA_OBJECT b, JAVA_INT off, JAVA_INT len) {
    crc32(crc, (unsigned char *)((JAVA_ARRAY)b)->data + off, len);
}

JAVA_INT java_util_zip_CRC32_updateByteBuffer___int_java_nio_ByteBuffer_int_int_R_int(CODENAME_ONE_THREAD_STATE, JAVA_INT crc, JAVA_OBJECT buffer, JAVA_INT off, JAVA_INT len) {
    crc32(crc, (unsigned char*)((struct obj__java_nio_ByteBuffer*)buffer)->java_nio_Buffer_address + off, len);
}
