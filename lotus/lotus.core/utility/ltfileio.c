#include "ltfileio.h"

#include "../platform/ltlogger.h"

b8 ltIsFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) return LOTUS_FALSE;
    fclose(file);
    return LOTUS_TRUE;
}

char* ltReadFile(const char* path) {
    FILE* file = fopen(path, "rb");
    ltSetLogLevel(LOTUS_LOG_ERROR);

    if (!file) {
        ltLogError("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    if (length < 0) {
        ltLogError("Failed to determine file size");
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    if (!content) {
        ltLogError("Failed to allocate memory for file content\n");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(content, sizeof(char), length, file);
    if (bytesRead != (size_t)length) {
        ltLogError("Failed to read the entire file\n");
        free(content);
        fclose(file);
        return NULL;
    }

    content[length] = '\0';
    fclose(file);
    return content;
}

b8 ltDeleteFile(const char* path) {
    if (remove(path) != LOTUS_FALSE) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("Failed to delte file at path: %s\n", path);
        return LOTUS_FALSE;
    }
    return LOTUS_TRUE;
}

long ltGetFileSize(const char* path) {
    FILE* file = fopen(path, "rb");

    if (!file) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to open file at path: %s\n", path);
        return LOTUS_FALSE;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

b8 ltCopyFile(const char* dest, const char* src) {
    FILE* srcFile = fopen(src, "rb");
    if (!src) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("Failed to open source file: %s\n", src);
        return LOTUS_FALSE;
    }

    FILE* destFile = fopen(dest, "wb");
    if (!dest) {
        ltLogError("Failed to open destination file: %s\n", dest);
        fclose(srcFile);
        return LOTUS_FALSE;
    }

    size_t bytesRead;
    char buffer[4096];
    do {
        fwrite(buffer, sizeof(char), bytesRead, destFile);
    } while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), srcFile)) > 0);

    fclose(srcFile);
    fclose(destFile);
    return LOTUS_TRUE;
}

b8 ltAppendFile(const char* data, const char* path, b8 newline) {
    if (newline) ltWriteFile("\n", path, 1);
    return ltWriteFile(data, path, 1);
}

b8 ltWriteFile(const char* data, const char* path, b8 preserve) {
    FILE* file = fopen(path, preserve ? "ab" : "wb");
    ltSetLogLevel(LOTUS_LOG_ERROR);
    if (!file) {
        ltLogError("Failed to open file");
        return LOTUS_FALSE;
    }

    size_t length = strlen(data);
    if (length <= 0) {
        ltLogError("Faield to compute length of write data\n");
        fclose(file);
        return LOTUS_FALSE;
    }
    
    size_t bytesWritten = fwrite(data, sizeof(char), length, file);
    if (bytesWritten != length) {
        ltLogError("Failed to fully write data to file at path: %s\n", path);
        fclose(file);
        return LOTUS_FALSE;
    }

    fclose(file);
    return LOTUS_TRUE;
}

b8 ltProcessFile(void (*processLine)(const char* line), const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("Failed to open file at path: %s\n", path);
        return LOTUS_FALSE;
    }

    char line[1024];
    do {
        processLine(line);
    } while(fgets(line, sizeof(line), file));

    fclose(file);
    return LOTUS_TRUE;
}
