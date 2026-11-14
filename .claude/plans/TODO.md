# TODO

Everything pending in this session: explicit, unanswered, or unacknowledged.
Kept current as items open and close.

## A. Decisions waiting for the author

- The thin and generic binding (confirm): asked on 2026-11-13.
- 7GUIs as what to build (confirm): to ask after that.
- The local remote still points to `fsantanna/iup`: the change to
  `ia-atmos/env-iup` was blocked by the session. Pushes work by the
  redirect.

## B. Work known and not done

- I1: the environment `iup.atm`. Waits for the answer on the binding.
  Before it: read `doc/manual.md` of the compiler.
- `atmos-iup`: the build makes the dedicated compiler (`make env` of
  `ia-atmos/lang`); the runner of the tests, the examples, and the
  documents use it. With I1, when `iup.atm` exists.
- I2 to I6: see `261113-env-iup.md`.
- The Mac: verify whether IUP has a driver, and say so in the readme.
- `www.tecgraf.puc-rio.br` is not reachable from the container (the
  documents of IUP): SourceForge is.

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
