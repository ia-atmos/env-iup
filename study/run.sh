#!/bin/sh
# Runs the C versions of the study under the scripts of the tests of
# their Atmos versions (`tst/7guis-<task>.script`), and compares their
# traces with what the Atmos versions print (`tst/7guis-<task>.out`).
#
#   study/run.sh [task ...]
#
# Requires `xvfb-run` and the binaries of IUP in `iup/`.

cd "$(dirname "$(realpath "$0")")/.." || exit 1
IUP=$PWD/iup
mkdir -p out
if [ $# -eq 0 ]; then
    set -- counter temperature flight timer crud circles cells
fi
same=0
diff=0
for t in "$@"; do
    if ! gcc -Wall -Wno-unused-parameter -Wno-unused-function -o "out/c-$t" "study/$t.c" \
            -I"$IUP/include" -L"$IUP" -liup -lm -Wl,-rpath,"$IUP" 2> "out/c-$t.log"; then
        echo "FAIL $t    does not compile (see out/c-$t.log)"
        diff=$((diff+1))
        continue
    fi
    script=$(grep -v '^#' "tst/7guis-$t.script" | grep -v '^$' | paste -sd";" -)
    XDG_CONFIG_HOME=$PWD/tst/gtk GTK_THEME=Adwaita IUP_ATM_SCRIPT="$script" \
        timeout 60 xvfb-run -a -s "-screen 0 640x480x24" \
        sh -c '"$0" > "$1" 2>> "$2"' "out/c-$t" "out/c-$t.res" "out/c-$t.log"
    grep -v '^exit ' "tst/7guis-$t.out" > "out/c-$t.exp"
    if diff "out/c-$t.exp" "out/c-$t.res" > "out/c-$t.diff"; then
        echo "SAME $t"
        same=$((same+1))
    else
        echo "DIFF $t    (see out/c-$t.diff)"
        diff=$((diff+1))
    fi
done
echo "same=$same diff=$diff"
[ $diff -eq 0 ]
