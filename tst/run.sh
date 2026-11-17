#!/bin/sh
# Runs the tests, with no screen: each one is a program under a script
# of actions (the test mode of `iup.atm`), whose output is compared
# with what is expected.
#
#   tst/run.sh [name ...]
#
# A test `name` is:
#
#   tst/name.atm or exs/name.atm    the program (`7guis-x` is
#                                   `exs/7guis/x.atm`)
#   tst/name.script                 the actions, one per line (optional)
#   tst/name.out                    the expected output
#
# A program that includes `uv.atm` (the network, `ia-atmos/env-uv`) is
# built with `-luv`, and is skipped if there is no
# `../env-uv` or no headers of libuv.
#
# Requires `xvfb-run`, and `import` (ImageMagick) for the screenshots,
# which go to `out/`. With `ATMOS_ASAN=1`, the programs run under the
# address sanitizer. GTK and the libraries under it leak on their own,
# so a leak only fails a test if the stack of its allocation has a
# function of the runtime of Atmos or of `iup.atm`; any other error of
# the sanitizer always fails it.

cd "$(dirname "$(realpath "$0")")/.." || exit 1

if [ $# -eq 0 ]; then
    set -- $(ls tst/*.out | sed 's|tst/||; s|\.out$||')
fi

mkdir -p out
pass=0
fail=0
for t in "$@"; do
    prog=tst/$t.atm
    [ -f "$prog" ] || prog=exs/$t.atm
    [ -f "$prog" ] || prog=exs/7guis/${t#7guis-}.atm
    script=
    if [ -f "tst/$t.script" ]; then
        script=$(grep -v '^#' "tst/$t.script" | grep -v '^$' | paste -sd";" -)
    fi
    why=
    # a program with the network of env-uv: its flags, or skip
    flags=
    libs=
    if grep -q 'include ".*uv.atm"' "$prog"; then
        if [ ! -f ../env-uv/uv.atm ] || [ ! -f /usr/include/uv.h ]; then
            echo "SKIP $t    (needs ../env-uv and libuv with its headers)"
            continue
        fi
        flags="-O1"
        libs="-luv"
    fi
    if ! ATMOS_CFLAGS="${flags:-${ATMOS_CFLAGS:--O1}}" ATMOS_LDFLAGS="$libs" \
            ./atmos-iup -o "out/$t.exe" "$prog" > "out/$t.log" 2>&1; then
        why="does not compile (see out/$t.log)"
    else
        GTK_THEME=Adwaita LSAN_OPTIONS=exitcode=0 IUP_ATM_SCRIPT="$script" \
            timeout 60 xvfb-run -a -s "-screen 0 640x480x24" \
            sh -c '"$0" > "$1" 2> "$2"' "out/$t.exe" "out/$t.res" "out/$t.log"
        code=$?
        echo "exit $code" >> "out/$t.res"
        if ! diff "tst/$t.out" "out/$t.res" > "out/$t.diff"; then
            why="unexpected output (see out/$t.diff)"
        elif grep -qE '^ +#[0-9]+ .* in (ati_|atm_|a_)[A-Za-z0-9_]* ' "out/$t.log"; then
            why="leak (see out/$t.log)"
        fi
    fi
    if [ -z "$why" ]; then
        echo "OK   $t"
        pass=$((pass+1))
    else
        echo "FAIL $t    $why"
        fail=$((fail+1))
    fi
done

echo "pass=$pass fail=$fail"
[ $fail -eq 0 ]
