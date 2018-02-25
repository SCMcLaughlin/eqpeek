
#include "eqg_types.h"
#include "mod.h"
#include "pfs.h"
#include "str_util.h"
#include "wld.h"
#include "wld_types.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void handle_wld(uint8_t* data, uint32_t length)
{
    WLD* wld;
    WldFrag** frags;
    uint32_t count, i;
    int rc;
    
    rc = wld_open_from_memory_no_copy(&wld, data, length);
    if (rc)
    {
        fprintf(stderr, "error: failed to open WLD file, errcode %i\n", rc);
        return;
    }
    
    frags = wld_frags(wld, &count);
    
    for (i = 0; i < count; i++)
    {
        WldFrag* frag = frags[i];
        uint32_t flag;
        uint8_t* raw;
        
        if (!wld_frag_is_type(frag, WLD_FRAGTYPE_SKELETON))
            continue;
        
        flag = ((WldFragSkeleton*)frag)->flag;
        raw = (uint8_t*)frag;
        raw += sizeof(WldFragSkeleton);
        
        if ((flag & 1) != 0)
            raw += sizeof(int) * 3;
        
        if ((flag & 2) != 0)
        {
            double radius = *((float*)raw);
            const char* name = wld_string_by_frag(wld, frag);
            
            fprintf(stdout, "%c%c%c %.14g\n", name[0], name[1], name[2], radius);
        }
    }
    
    wld_close(wld);
}

static void handle_mod(const char* fname, uint8_t* data, uint32_t length)
{
    MOD mod;
    int rc;
    void* vertices;
    float highX = -999999;
    float highY = -999999;
    float highZ = -999999;
    double radius;
    
    rc = mod_open_skip_to_vertices(data, length, &mod, &vertices);
    if (rc) return;
    
    if (mod.version >= 3)
    {
        EQGVertexV3* v3 = (EQGVertexV3*)vertices;
        uint32_t i;
        
        for (i = 0; i < mod.vertexCount; i++)
        {
            float x = v3->position.x;
            float y = v3->position.y;
            float z = v3->position.z;
            
            if (x > highX) highX = x;
            if (y > highY) highY = y;
            if (z > highZ) highZ = z;
            
            v3++;
        }
    }
    else
    {
        EQGVertexV1* v1 = (EQGVertexV1*)vertices;
        uint32_t i;
        
        for (i = 0; i < mod.vertexCount; i++)
        {
            float x = v1->position.x;
            float y = v1->position.y;
            float z = v1->position.z;
            
            if (x > highX) highX = x;
            if (y > highY) highY = y;
            if (z > highZ) highZ = z;
            
            v1++;
        }
    }
    
    radius = sqrtf(highX * highX + highY * highY + highZ * 0.5 * (highZ * 0.5));
    fprintf(stdout, "%c%c%c %.14g\n", toupper(fname[0]), toupper(fname[1]), toupper(fname[2]), radius);
}

void peek_melee_radius(const char* pfspath)
{
    char buf[256];
    PFS* pfs;
    uint32_t index = 0;
    int len;
    int rc;
    
    rc = pfs_open(&pfs, pfspath);
    if (rc)
    {
        fprintf(stderr, "error: failed to open PFS file '%s', errcode: %i\n", pfspath, rc);
        return;
    }
    
    pfspath = filename_from_path(pfspath, 0);
    len = snprintf(buf, sizeof(buf), "%s", pfspath);
    len -= 4;
    buf[len] = '\0'; /* Write over the '.' for the file extension with a null terminator */
    
    for (;;)
    {
        const char* fname = pfs_file_name(pfs, index++);
        
        if (!fname) break;
        
        if (strstr(fname, buf) == fname)
        {
            uint8_t* data;
            uint32_t length;
            int type = 0;
            
            if (strcmp(fname + len, ".wld") == 0)
            {
                type = 1;
            }
            else if (strcmp(fname + len, ".mod") == 0)
            {
                type = 2;
            }
            else if (strcmp(fname + len, ".mds") == 0)
            {
                type = 3;
            }
            
            if (!type) continue;
            
            rc = pfs_file_data(pfs, fname, &data, &length);
            if (rc)
            {
                fprintf(stderr, "error: failed to open internal file '%s', errcode: %i\n", fname, rc);
                return;
            }
            
            switch (type)
            {
            case 1:
                handle_wld(data, length);
                break;
            
            case 2:
                handle_mod(fname, data, length);
                break;
            
            case 3:
                break;
            }
            
            free(data);
        }
    }
    
    pfs_close(pfs);
}
