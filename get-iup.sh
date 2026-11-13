#!/bin/sh
# Fetches the official binaries of IUP for Linux, at a fixed version,
# into `iup/` (ignored by git). Nothing of IUP is committed.
set -e

VER=3.32
TGZ=iup-${VER}_Linux68_64_lib.tar.gz
SHA=bef8e1880cd03df441d7cb16fe72e823db8e981fdfe04a8a86f57220807765e4
URL="https://downloads.sourceforge.net/project/iup/${VER}/Linux%20Libraries/${TGZ}"

cd "$(dirname "$0")"
if [ -f iup/libiup.so ]; then
    echo "iup/ is already there"
    exit 0
fi

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
curl -sS -L -o "$TMP/$TGZ" "$URL"
echo "$SHA  $TMP/$TGZ" | sha256sum -c -
mkdir -p iup
tar xzf "$TMP/$TGZ" -C iup
echo "IUP $VER in iup/"
