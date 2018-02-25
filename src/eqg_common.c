
#include "eqg_types.h"

uint32_t eqg_skip_materials(uint8_t* data, uint32_t length, uint32_t count)
{
    uint32_t p = 0;
    uint32_t i;
    
    for (i = 0; i < count; i++)
    {
        EQGMaterial* mat = (EQGMaterial*)(data + p);
        
        p += sizeof(EQGMaterial);
        
        if (p > length) return 0xffffffff;
        
        p += sizeof(EQGProperty) * mat->propertyCount;
        
        if (p > length) return 0xffffffff;
    }
    
    return p;
}
