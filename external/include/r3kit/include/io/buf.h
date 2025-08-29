#ifndef __R3_FILE_H__
#define __R3_FILE_H__

#include <r3kit/include/defines.h>

typedef enum Buffer_Header_Field {
	BUFFER_SIZE_FIELD,
	BUFFER_READ_FIELD,
	BUFFER_WROTE_FIELD,
	BUFFER_READS_FIELD,
	BUFFER_SAVES_FIELD,
	BUFFER_SAVED_FIELD,
	BUFFER_LOADS_FIELD,
	BUFFER_LOADED_FIELD,
	BUFFER_WRITES_FIELD,
	BUFFER_CURSOR_FIELD,
	BUFFER_HEADER_FIELDS
} Buffer_Header_Field;

// bare 'wrapper' structure for type saftey
// the internal `data` field is ALWAYS allocated
// with 20 header bytes
typedef struct Buffer {
	ptr data;
} Buffer;

R3_API u8 r3_buf_dealloc(Buffer* in);
R3_API u8 r3_buf_alloc(u16 size, Buffer* out);
R3_API u16 r3_buf_field(u8 field, Buffer* in);

R3_API u8 r3_buf_rewind(Buffer* in);
R3_API u8 r3_buf_seek(u16 offset, Buffer* in);

R3_API u8 r3_buf_load(char* path, Buffer* in);
R3_API u8 r3_buf_save(char* path, Buffer* in);

R3_API u8 r3_buf_read(u16 count, char* value, Buffer* in);
R3_API u8 r3_buf_write(u16 count, char* value, Buffer* in);
R3_API u8 r3_buf_copy(u16 count, Buffer* dest, Buffer* src);

#endif // __R3_FILE_H__
