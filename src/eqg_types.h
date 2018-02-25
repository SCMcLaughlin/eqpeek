
#ifndef EQG_TYPES_H
#define EQG_TYPES_H

#include <stdint.h>

#pragma pack(1)

typedef struct {
    uint32_t    typeNameIndex;
    uint32_t    valueType;      /* 0 = float, 2 = string index, 3 = ARGB */
    union {
        uint32_t    asIndex;
        float       asFloat;
        struct {
            uint8_t a, r, g, b;
        } asColor;
    };
} EQGProperty;

typedef struct {
    uint32_t    index;
    uint32_t    nameIndex;
    uint32_t    shaderIndex;
    uint32_t    propertyCount;
    EQGProperty properties[0];
} EQGMaterial;

typedef struct {
    float u, v;
} EQGTexCoord;

typedef struct {
    float x, y, z;
} EQGPos;

typedef struct {
    float x, y, z, w;
} EQGQuat;

typedef struct {
    EQGPos      position;
    EQGPos      normal;
    EQGTexCoord texCoord;
} EQGVertexV1;

typedef struct {
    EQGPos      position;
    EQGPos      normal;
    uint8_t     r, g, b, a;
    EQGTexCoord texCoord;
    float       unknown_f[2];
} EQGVertexV3;

typedef struct {
    uint32_t    index[3];
    int         materialIndex;
    uint32_t    flag;
} EQGTriangle;

typedef struct {
    uint32_t    nameIndex;
    uint32_t    linkBoneIndex;
    uint32_t    flag;
    uint32_t    childBoneIndex;
    EQGPos      position;
    EQGQuat     rotation;
    EQGPos      scale;
} EQGBone;

typedef struct {
    int     boneIndex;
    float   value;
} EQGWeight;

typedef struct {
    uint32_t    count;
    EQGWeight   weights[4];
} EQGBoneMapping;

#pragma pack()

#endif/*EQG_TYPES_H*/
