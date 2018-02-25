
#include "bit.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    const char* substr;
    uint32_t    maxLength;
} DirMatch;

static char* dir_files(const char* dirPath, int* count, DirMatch* matches, uint32_t matchCount)
{
    char* ret = NULL;
    char* ptr;
    int num = 0;
    size_t total = 0;
    DIR* dir;
    
    dir = opendir(dirPath);
    if (!dir)
    {
        fprintf(stderr, "error: could not open directory '%s'\n", dirPath);
        return NULL;
    }
    
    for (;;)
    {
        struct dirent* ent = readdir(dir);
        size_t len, prev, pow;
        uint32_t i;
        int found = 0;
        
        if (!ent) break;
        
        if (ent->d_type != DT_REG)
            continue;
        
        len = strlen(ent->d_name);
        if (len > 127) continue;
        
        for (i = 0; i < matchCount; i++)
        {
            if (len <= matches[i].maxLength && strstr(ent->d_name, matches[i].substr) != NULL)
            {
                found = 1;
                break;
            }
        }
        
        if (!found) continue;
        
        prev = total;
        total += len + 2;
        pow = bit_pow2_greater_or_equal(total);
        
        if (bit_pow2_greater_or_equal(prev) < pow)
        {
            char* newret = realloc(ret, pow);
            
            if (!newret)
            {
                fprintf(stderr, "error: out of memory reading directory '%s'\n", dirPath);
                free(ret);
                ret = NULL;
                num = 0;
                break;
            }
            
            ret = newret;
        }
        
        ptr = ret + prev;
        
        *ptr++ = (char)len;
        snprintf(ptr, (size_t)((ret + total) - ptr), "%s", ent->d_name);
        num++;
    }
    
    closedir(dir);
    
    *count = num;
    return ret;
}

char* dir_pfs_files(const char* dirPath, int* count)
{
    static DirMatch matches[] = {
        { ".s3d", 256 },
        { ".eqg", 256 }
    };
    
    return dir_files(dirPath, count, matches, sizeof(matches) / sizeof(DirMatch));
}

char* dir_model_pfs_files(const char* dirPath, int* count)
{
    static DirMatch matches[] = {
        { "_chr.s3d",   256 },
        { ".eqg",       7   }
    };
    
    return dir_files(dirPath, count, matches, sizeof(matches) / sizeof(DirMatch));
}

int path_is_dir(const char* path)
{
    struct stat st;
    
    if (!stat(path, &st) && S_ISDIR(st.st_mode))
    {
        return 1;
    }
    
    return 0;
}
