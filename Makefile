export CFLAGS = -g

export CXX = g++
export AR = ar rc
export RANLIB = ranlib
export STRIP = strip
export LIBS =

export CC68K = m68k-linux-gnu-gcc
export LD68K = m68k-linux-gnu-ld
export OBJCOPY68K = m68k-linux-gnu-objcopy

export C68KFLAGS = -g -Os -fomit-frame-pointer -march=68000
export OBJDIR = .obj

all: .obj/build.ninja
	@ninja -f .obj/build.ninja

clean:
	@echo CLEAN
	@rm -rf $(OBJDIR)

.obj/build.ninja: mkninja.sh Makefile
	@echo MKNINJA $@
	@mkdir -p $(OBJDIR)
	@sh $< > $@

