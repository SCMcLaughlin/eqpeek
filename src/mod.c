
#include "eqg_common.h"
#include "eqg_types.h"
#include "mod.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int mod_check_header(uint8_t* data, uint32_t length, MOD* outHeader)
{
    MOD* header = (MOD*)data;
    uint32_t sig = *((uint32_t*)"EQGM");
    
    if (length < sizeof(MOD))
    {
        fprintf(stderr, "error: data with length %u is too short to be a MOD header\n", length);
        return MOD_CORRUPTED;
    }
    
    if (header->signature != sig)
    {
        fprintf(stderr, "error: MOD file signature mismatch\n");
        return MOD_CORRUPTED;
    }
    
    *outHeader = *header;
    return MOD_OK;
}

int mod_open_skip_to_vertices(uint8_t* data, uint32_t length, MOD* header, void** outVertices)
{
    uint32_t len;
    int rc;
    
    rc = mod_check_header(data, length, header);
    if (rc) return rc;
    
    /* Skip string block */
    len = sizeof(MOD) + header->stringBlockLen;
    if (length < len) goto fail_len;
    
    data += len;
    length -= len;
    
    /* Skip materials */
    len = eqg_skip_materials(data, length, header->materialCount);
    if (len == 0xffffffff) goto fail_len;
    
    data += len;
    length -= len;
    
    len = (header->version >= 3) ? sizeof(EQGVertexV3) : sizeof(EQGVertexV1);
    len *= header->vertexCount;
    
    if (length < len) goto fail_len;
    
    *outVertices = data;
    return MOD_OK;
    
fail_len:
    fprintf(stderr, "error: MOD file too short for length of data indicated\n");
    return MOD_CORRUPTED;
}
