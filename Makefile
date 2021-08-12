export CFLAGS = -g

export CXX = g++
export AR = ar rc
export RANLIB = ranlib
export STRIP = strip
export LIBS =

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

