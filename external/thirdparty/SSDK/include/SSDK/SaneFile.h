#ifndef __SANEFIO_H__
#define __SANEFIO_H__

#include <include/SSDK/globals.h>

typedef struct SaneFile {
    SaneModule module;
    
    byte (*exists)(cstr path);
    u64 (*size)(cstr path);
    byte (*remove)(cstr path);
    str (*read)(cstr path, u64 size);
    byte (*copy)(cstr src, cstr dest, u64 size);
    byte (*copyb)(cstr src, cstr dest, u64 size);
    byte (*readb)(cstr path, u64 size, void* out);
    byte (*write)(cstr path, cstr lines, byte preserve);
    byte (*append)(cstr path, cstr lines, byte newline);
    byte (*writeb)(cstr path, u64 size, byte preserve, void* ptr);
    byte (*appendb)(cstr path, u64 size, byte newline, void* ptr);
    byte (*process)(cstr path, u64 lines, void (*callback)(cstr line));
} SaneFile;
extern SaneFile* saneFile;

SSDK_FUNC byte ssdkInitFile(none);
SSDK_FUNC none ssdkExitFile(none);

#endif  // __SANEFIO_H__