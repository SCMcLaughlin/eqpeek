
#ifndef MDS_H
#define MDS_H

#include "eqg_types.h"
#include <stdint.h>

#define MDS_OK 0
#define MDS_OUT_OF_MEMORY -1
#define MDS_CORRUPTED -2
#define MDS_OUT_OF_BOUNDS -3

typedef struct {
    uint32_t        version;
    uint32_t        stringBlockLen;
    uint32_t        materialCount;
    uint32_t        boneCount;
    uint32_t        modelCount;
    uint8_t*        data;
    uint32_t        length;
    const char*     strings;
    uint32_t        materialOffset;
    EQGBone*        bones;
    uint32_t        modelOffset;
} MDS;

typedef struct {
    uint32_t        isMainPiece;
    uint32_t        vertexCount;
    uint32_t        triangleCount;
    uint32_t        boneMappingCount;
    const char*     name;
    void*           vertices;
    EQGTriangle*    triangles;
    EQGBoneMapping* boneMappings;
} MDSModel;

int mds_open(uint8_t* data, uint32_t length, MDS* mds);
int mds_model(MDS* mds, MDSModel* model, uint32_t index);

#endif/*MDS_H*/
