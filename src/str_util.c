
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
