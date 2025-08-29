#ifndef __R3_STRING_H__
#define __R3_STRING_H__

#include <r3kit/include/defines.h>

typedef enum String_Header_Field {
	MAX_FIELD,
	SIZE_FIELD,
	COUNT_FIELD,
	STRING_HEADER_FIELDS
} String_Header_Field;

// bare 'wrapper' structure for type safety
// the internal memory block is ALWAYS allocated
// with 1 byte extra for null-termination at string max
// (along with 6 header bytes)
typedef struct String {
	ptr data;
} String;

R3_API u8 r3_str_dealloc(String* in);
R3_API u8 r3_str_alloc(u16 max, String* out);
R3_API u16 r3_str_field(u8 field, String* in);

R3_API u8 r3_str_combine(String* dest, String* src);
R3_API u8 r3_str_extend(u16 count, char* value, String* in);
R3_API u8 r3_str_copy(u16 count, String* dest, String* src);
R3_API u8 r3_str_swap(u16 a, u16 end, char* value, String* in);
R3_API u8 r3_str_slice(u16 start, u16 end, char* value, String* in);

R3_API u8 r3_str_read(u16 count, char* value, String* in);
R3_API u8 r3_str_write(u16 count, char* value, String* in);

R3_API u8 r3_str_read_at(u16 offset, u16 count, char* value, String* in);
R3_API u8 r3_str_write_at(u16 offset, u16 count, char* value, String* in);
R3_API u8 r3_str_copy_at(u16 offset, u16 count, String* dest, String* src);

#endif // __R3_STRING_H__
