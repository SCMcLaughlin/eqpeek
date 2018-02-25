
#include "eqg_common.h"
#include "eqg_types.h"
#include "mds.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma pack(1)

typedef struct {
    uint32_t    signature;
    uint32_t    version;
    uint32_t    stringBlockLen;
    uint32_t    materialCount;
    uint32_t    boneCount;
    uint32_t    modelCount;
} MDSHeader;

typedef struct {
    uint32_t    mainPiece;  /* 1 = yes, 0 = no; yes applies to both the main body and default "head" */
    uint32_t    nameIndex;
    uint32_t    vertexCount;
    uint32_t    triangleCount;
    uint32_t    boneMappingCount;
} MDSModelHeader;

#pragma pack()

static int mds_check_header(uint8_t* data, uint32_t length, MDSHeader* outHeader)
{
    MDSHeader* header = (MDSHeader*)data;
    uint32_t sig = *((uint32_t*)"EQGS");
    
    if (length < sizeof(MDSHeader))
    {
        fprintf(stderr, "error: data with length %u is too short to be a MDS header\n", length);
        return MDS_CORRUPTED;
    }
    
    if (header->signature != sig)
    {
        fprintf(stderr, "error: MDS file signature mismatch\n");
        return MDS_CORRUPTED;
    }
    
    *outHeader = *header;
    return MDS_OK;
}

int mds_open(uint8_t* data, uint32_t length, MDS* mds)
{
    MDSHeader header;
    uint32_t p, add;
    int rc;
    
    rc = mds_check_header(data, length, &header);
    if (rc) return rc;
    
    mds->version = header.version;
    mds->stringBlockLen = header.stringBlockLen;
    mds->materialCount = header.materialCount;
    mds->boneCount = header.boneCount;
    mds->modelCount = header.modelCount;
    mds->data = data;
    mds->length = length;
    
    /* String block */
    p = sizeof(MDSHeader);
    mds->strings = (const char*)(data + p);
    p += header.stringBlockLen;
    if (length < p) goto fail_len;
    
    /* Materials */
    mds->materialOffset = p;
    add = eqg_skip_materials(data + p, length - p, header.materialCount);
    if (add == 0xffffffff) goto fail_len;
    p += add;
    
    /* Bones */
    mds->bones = (EQGBone*)(data + p);
    p += sizeof(EQGBone) * header.boneCount;
    if (length < p) goto fail_len;
    
    /* Models */
    mds->modelOffset = p;
    
    return MDS_OK;
    
fail_len:
    fprintf(stderr, "error: MDS file too short for length of data indicated\n");
    return MDS_CORRUPTED;
}

int mds_model(MDS* mds, MDSModel* model, uint32_t index)
{
    uint32_t version = mds->version;
    const char* strings = mds->strings;
    uint8_t* data = mds->data;
    uint32_t length = mds->length;
    uint32_t p = mds->modelOffset;
    uint32_t n = mds->modelCount;
    uint32_t i, len;
    
    for (i = 0; i <= index && i < n; i++)
    {
        MDSModelHeader* header = (MDSModelHeader*)(data + p);
        
        p += sizeof(MDSModelHeader);
        if (length < p) goto fail_len;
        
        model->isMainPiece = header->mainPiece;
        model->vertexCount = header->vertexCount;
        model->triangleCount = header->triangleCount;
        model->boneMappingCount = header->boneMappingCount;
        
        model->name = strings + header->nameIndex;
        model->vertices = data + p;
        
        len = (version >= 3) ? sizeof(EQGVertexV3) : sizeof(EQGVertexV1);
        len *= header->vertexCount;
        p += len;
        if (length < p) goto fail_len;
        
        model->triangles = (EQGTriangle*)(data + p);
        p += sizeof(EQGTriangle) * header->triangleCount;
        if (length < p) goto fail_len;
        
        model->boneMappings = (EQGBoneMapping*)(data + p);
        p += sizeof(EQGBoneMapping) * header->boneMappingCount;
        if (length < p) goto fail_len;
    }
    
    return MDS_OK;
    
fail_len:
    fprintf(stderr, "error: MDS file too short for length of data indicated\n");
    return MDS_CORRUPTED;
}