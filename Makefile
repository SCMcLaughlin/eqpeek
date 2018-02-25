
CFLAGS?= -O2 -fomit-frame-pointer -std=gnu89 \
-Wall -Wextra -Wredundant-decls \
-Wno-unused-result -Wno-strict-aliasing

ifdef debug
CFLAGS+= -O0 -g -Wno-format -fno-omit-frame-pointer
CFLAGS+= -DDEBUG
endif

##############################################################################
# Object files
##############################################################################
_OBJECTS= bit dir_posix main mod opt peek_melee_radius pfs str_util wld

OBJECTS= $(patsubst %,build/%.o,$(_OBJECTS))

##############################################################################
# Linker flags
##############################################################################
LDYNAMIC= -lz -lm

##############################################################################
# Util
##############################################################################
Q= @
E= @echo -e
RM= rm -f 

##############################################################################
# Build rules
##############################################################################
.PHONY: default install all clean

default all: eqpeek

eqpeek: $(OBJECTS)
	$(E) "Linking $@"
	$(Q)$(CC) -o $@ $^ $(LDYNAMIC)

build/%.o: src/%.c $($(CC) -M src/%.c)
	$(Q)mkdir -p build
	$(E) "\e[0;32mCC     $@\e(B\e[m"
	$(Q)$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(Q)$(RM) build/*.o
	$(Q)$(RM) eqpeek
	$(E) "Cleaned build directory"

install:
	cp eqpeek /usr/local/bin/
