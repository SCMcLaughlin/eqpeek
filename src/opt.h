
#ifndef OPT_H
#define OPT_H

#include <stdint.h>

enum OptBit
{
    OPT_MELEE_RADIUS,
    OPT_HELP,
};

typedef struct {
    const char* str;
    int         bit;
} OptArg;

typedef struct {
    uint32_t    flags;
    uint32_t    count;
    OptArg*     args;
} Opt;

typedef struct {
    const char* name;
    int         bit;
} OptHandler;

int opt_process(Opt* opt, const OptHandler* handlers, int argc, const char** argv);
void opt_free(Opt* opt);

#define opt_flag(opt, bit) (((opt)->flags & (1 << (bit))) != 0)

#endif/*OPT_H*/
