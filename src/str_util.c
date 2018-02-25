
#include <stdint.h>
#include <string.h>

const char* filename_from_path(const char* path, size_t len)
{
    const char* ret = path;
    size_t i;
    
    if (len == 0)
        len = strlen(path);
    
    for (i = 0; i < len; i++)
    {
        char c = *path++;
        
        if (c == '/' || c == '\\')
        {
            ret = path;
        }
    }
    
    return ret;
}

uint32_t str_hash(const char* key, uint32_t len)
{
    uint32_t h = len;
    uint32_t step = (len >> 5) + 1;
    uint32_t i;
    
    for (i = len; i >= step; i -= step)
    {
        h = h ^ ((h << 5) + (h >> 2) + (key[i - 1]));
    }
    
    return h;
}
