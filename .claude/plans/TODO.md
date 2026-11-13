# TODO

Everything pending in this session: explicit, unanswered, or unacknowledged.
Kept current as items open and close.

## A. Decisions waiting for the author

- How IUP gets into the container (asked on 2026-11-13). Recommended: a
  script of the repository fetches the official binaries of IUP 3.32 for
  Linux (`iup-3.32_Linux68_64_lib.tar.gz`, SourceForge), checked by
  SHA-256. No package to install: they link against the GTK 3 that the
  container already has. Building from the sources needs `libgtk-3-dev`.
- Permission to build and run a C program against those binaries: the
  session blocked the run of the I0 check.
- The thin and generic binding (confirm): to ask next.
- 7GUIs as what to build (confirm): to ask after that.
- The local remote still points to `fsantanna/iup`: the change to
  `ia-atmos/env-iup` was blocked by the session. Pushes work by the
  redirect.

## B. Work known and not done

- I0, the check. Found so far, with nothing installed: SourceForge is
  reachable (`www.tecgraf.puc-rio.br` is not); `Xvfb`, `xvfb-run`, and
  `import` (ImageMagick) are present; `xdotool` and the headers of GTK
  are not; `libiup.so` of the binaries resolves all its libraries. The
  check program is written and was not run.
- I1 to I6: see `261113-env-iup.md`.

## C. Reported and not acknowledged

- (none)

## D. Older items

- (none)

## Context

- Repository: `ia-atmos/env-iup` (formerly `fsantanna/iup`).
- Subject: an environment for graphical user interfaces in Atmos over the IUP
  library (Tecgraf/PUC-Rio), as a direct binding to its C API.
- Related: `lua-atmos/env-iup` (same idea for the Lua runtime, over iup-lua).
- Compiler: `ia-atmos/lang` (formerly `fsantanna/ceu-atmos`), branch `main`.
  Defects, missing features, and doubts on the semantics go to the compiler
  session, with a small program that reproduces them.
