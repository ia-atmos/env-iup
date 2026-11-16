# TODO

Everything pending in this session: explicit, unanswered, or unacknowledged.
Kept current as items open and close.

## A. Decisions waiting for the author

- The versions in Lua of 7GUIs to compare with (asked 2026-11-16):
  none found in `lua-atmos` or `atmos-lang`. Where are they, or
  should they be written over `lua-atmos/env-iup`?
- The local remote still points to `fsantanna/iup`: the change to
  `ia-atmos/env-iup` was blocked by the session. Pushes work by the
  redirect.

## B. Work known and not done

- What to build (the author, 2026-11-14): all the examples of
  `lua-atmos/env-iup` and also 7GUIs. All done.
- The C versions of the study (`study/*.c`) are compiled and started,
  not tested: a script of actions for them would make the comparison
  of behavior a fact.
- In the test mode, a real click on an item of a menu does nothing
  (the item is highlighted and not activated; cause not found). The
  action `act` calls the callback and unmaps the menu instead.
- The flight booker shows its message in a label, not in a dialog.
- The cells have no double click to edit (the focus does it), and the
  formulas have functions only, no infix operators.
- The callbacks of the keys (`K_ANY`) and of the items of a list with
  arguments are not events yet.
- The screenshots are saved and not compared: decide where the pixels
  are stable.
- The Mac: IUP 3.32 distributes binaries for Linux and Windows only;
  the readme says Linux on x86-64. Windows is not tried.
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
