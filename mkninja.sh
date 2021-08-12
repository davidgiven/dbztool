#!/bin/sh
set -e

cat <<EOF
rule cxx
    command = $CXX $CFLAGS \$flags -I. -c -o \$out \$in -MMD -MF \$out.d
    description = CXX \$in
    depfile = \$out.d
    deps = gcc
    
rule cxx68k
    command = $CXX68K $C68KFLAGS \$flags -I. -c -o \$out \$in -MMD -MF \$out.d
    description = CXX68K \$in
    depfile = \$out.d
    deps = gcc
    
rule binencode
    command = xxd -i \$in > \$out
    description = XXD \$in
    restat = true

rule library
    command = $AR \$out \$in && $RANLIB \$out
    description = AR \$in

rule link
    command = $CXX $LDFLAGS -o \$out \$in \$flags $LIBS
    description = LINK \$in

rule strip
    command = cp -f \$in \$out && $STRIP \$out
    description = STRIP \$in

rule mktable
    command = sh scripts/mktable.sh \$kind \$words > \$out
    description = MKTABLE \$kind
    restat = true
EOF

buildlibrary() {
    local lib
    lib=$1
    shift

    local flags
    flags=
    local deps
    deps=
    while true; do
        case $1 in
            -d)
                deps="$deps $2"
                shift
                shift
                ;;

            -*)
                flags="$flags $1"
                shift
                ;;

            *)
                break
        esac
    done

    local oobjs
    local dobjs
    oobjs=
    dobjs=
    for src in "$@"; do
        local obj
        obj="$OBJDIR/opt/${src%%.c*}.o"
        oobjs="$oobjs $obj"

        echo "build $obj : cxx $src | $deps"
        echo "    flags=$flags $COPTFLAGS"

        obj="$OBJDIR/dbg/${src%%.c*}.o"
        dobjs="$dobjs $obj"

        echo "build $obj : cxx $src | $deps"
        echo "    flags=$flags $CDBGFLAGS"
    done

    echo build $OBJDIR/opt/$lib : library $oobjs
    echo build $OBJDIR/dbg/$lib : library $dobjs
}

buildprogram() {
    local prog
    prog=$1
    shift

    local flags
    flags=
    while true; do
        case $1 in
            -*)
                flags="$flags $1"
                shift
                ;;

            *)
                break
        esac
    done

    local oobjs
    local dobjs
    oobjs=
    dobjs=
    for src in "$@"; do
        oobjs="$oobjs $OBJDIR/opt/$src"
        dobjs="$dobjs $OBJDIR/dbg/$src"
    done

    echo build $prog-debug$EXTENSION : link $dobjs
    echo "    flags=$flags $LDDBGFLAGS"

    echo build $prog$EXTENSION-unstripped : link $oobjs
    echo "    flags=$flags $LDOPTFLAGS"

    echo build $prog$EXTENSION : strip $prog$EXTENSION-unstripped
}

buildsimpleprogram() {
    local prog
    prog=$1
    shift

    local flags
    flags=
    while true; do
        case $1 in
            -*)
                flags="$flags $1"
                shift
                ;;

            *)
                break
        esac
    done

    local src
    src=$1
    shift

    buildlibrary lib$prog.a $flags $src
    buildprogram $prog lib$prog.a "$@"
}

buildlibrary libdbz.a \
    src/main.cc \
    src/utils.cc \
    src/serial.cc \
    src/read.cc \
    src/write.cc \
    src/execute.cc \
    src/brecord.cc \

buildprogram dbztool \
    libdbz.a

# vim: sw=4 ts=4 et

