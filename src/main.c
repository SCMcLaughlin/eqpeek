
#include "dir.h"
#include "opt.h"
#include "peek.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage()
{
    printf(
       /*                                                                                */
        "Usage: eqpeek OPTIONS PATH\n"
        "\n"
        "  -m, --melee-radius   List melee radius for all mob models in PATH\n"
        "      --source-file    Display the source file being peeked\n"
        "      --source-archive Display the source archive being peeked\n"
        "  -h, --help           Display this dialog\n"
    );
}

static int for_all_model_pfs(const char* path, Opt* opt, void(*func)(Opt* opt, const char* path))
{
    if (path_is_dir(path))
    {
        int n, i, offset;
        char* files;
        const char* cur;
        char buf[1024];
        
        offset = snprintf(buf, sizeof(buf), "%s/", path);
        
        if (offset <= 0 || offset >= (int)sizeof(buf))
        {
            fprintf(stderr, "error: string format error while writing path '%s', path may be too long\n", path);
            return EXIT_FAILURE;
        }
        
        files = dir_model_pfs_files(path, &n);
        if (!files) return EXIT_FAILURE;
        cur = files;
        
        for (i = 0; i < n; i++)
        {
            int len = *cur++;
            
            snprintf(buf + offset, sizeof(buf) - offset, "%s", cur);
            func(opt, buf);
            
            cur += len + 1;
        }
        
        free(files);
    }
    else
    {
        func(opt, path);
    }
    
    return EXIT_SUCCESS;
}

int main(int argc, const char** argv)
{
    const char* path;
    Opt opt;
    int rc = EXIT_FAILURE;
    
    static const OptHandler optHandlers[] = {
        { "m",              OPT_MELEE_RADIUS    },
        { "melee-radius",   OPT_MELEE_RADIUS    },
        { "source-archive", OPT_SOURCE_ARCHIVE  },
        { "source-file",    OPT_SOURCE_FILE     },
        { "h",              OPT_HELP            },
        { "help",           OPT_HELP            },
        { NULL,             0                   }
    };
    
    if (argc < 2)
    {
        usage();
        return EXIT_SUCCESS;
    }
    
    rc = opt_process(&opt, optHandlers, argc, argv);
    if (rc) return EXIT_FAILURE;
    
    if (opt_flag(&opt, OPT_HELP))
    {
        usage();
        rc = EXIT_SUCCESS;
        goto done;
    }
    
    path = argv[argc - 1];
    opt.count--;
    
    if (opt_flag(&opt, OPT_MELEE_RADIUS))
    {
        rc = for_all_model_pfs(path, &opt, peek_melee_radius);
        goto done;
    }
    
    /* Default action */
    usage();
    
done:
    opt_free(&opt);
    return rc;
}
