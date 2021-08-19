#!/bin/sh
set -e

cat <<EOF
rule cxx
    command = $CXX $CFLAGS \$flags -I. -c -o \$out \$in -MMD -MF \$out.d
    description = CXX \$in
    depfile = \$out.d
    deps = gcc
    
rule cc68k
    command = $CC68K $C68KFLAGS \$flags -I. -c -o \$out \$in -MMD -MF \$out.d
    description = CC68K \$in
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

rule link68k
    command = $LD68K $LD68KFLAGS -o \$out \$in \$flags
    description = LINK68K \$in

rule objcopy68k
    command = $OBJCOPY68K \$in \$out \$flags
    description = OBJCOPY68K \$in

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

build68k() {
    local prog
    prog=$1
    shift

    local textaddr
    textaddr=0xffffffc0
    local deps
    deps=
    local hdr
    hdr=
    while true; do
        case $1 in
            -d)
                deps="$deps $2"
                shift
                shift
                ;;

            -text)
                textaddr="$2"
                shift
                shift
                ;;

            -header)
                hdr="$2"
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
        obj="$OBJDIR/opt/${src%%.*}.o"
        oobjs="$oobjs $obj"

        echo "build $obj : cc68k $src"
        echo "  flags = $flags"
    done

    echo "build $prog.elf : link68k $oobjs"
    echo "  flags=-Ttext=$textaddr"
    echo "build $prog.bin : objcopy68k $prog.elf"
    echo "  flags=-O binary"
    if [ "$hdr" != "" ]; then
        echo "build ${hdr} : binencode $prog.bin"
    fi
}

buildstub() {
    local d
    d="$OBJDIR/stubs/$1"

    build68k $d -header ${d}_stub.h -text 0xffffffc0 src/stubs/$1.S
}

buildstub fastmode
buildstub fill
buildstub getsp
buildstub ping
buildstub read
buildstub readregs
buildstub write
buildstub writeregs

buildlibrary libdbz.a \
    src/brecord.cc \
    src/cs.cc \
    src/execute.cc \
    src/fill.cc \
    src/getsp.cc \
    src/main.cc \
    src/mc68328.cc \
    src/ping.cc \
    src/read.cc \
    src/serial.cc \
    src/setreg.cc \
    src/showreg.cc \
    src/utils.cc \
    src/write.cc \

buildprogram dbztool \
    libdbz.a

# vim: sw=4 ts=4 et

