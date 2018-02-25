
#ifndef MOD_H
#define MOD_H

#include <stdint.h>

#define MOD_OK 0
#define MOD_OUT_OF_MEMORY -1
#define MOD_CORRUPTED -2

#pragma pack(1)
typedef struct {
    uint32_t    signature;
    uint32_t    version;
    uint32_t    stringBlockLen;
    uint32_t    materialCount;
    uint32_t    vertexCount;
    uint32_t    triangleCount;
    uint32_t    boneCount;
} MOD;
#pragma pack()

int mod_open_skip_to_vertices(uint8_t* data, uint32_t length, MOD* header, void** outVertices);

#endif/*MOD_H*/
