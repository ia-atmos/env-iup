# TODO

Everything pending in this session: explicit, unanswered, or unacknowledged.
Kept current as items open and close.

## A. Decisions waiting for the author

- `pin` or `val` for a dialog. `pin dlg` then `await :close [h=dlg]` is
  an error (a pinned value in the table of a pattern). The compiler
  session fixed a related defect and sent the question to the session
  "Pin rules discussion". Until the author decides, the examples use
  `val`; the other way is `pin` with `:close until it.h == dlg`.
- The local remote still points to `fsantanna/iup`: the change to
  `ia-atmos/env-iup` was blocked by the session. Pushes work by the
  redirect.

## B. Work known and not done

- What to build (the author, 2026-11-14): all the examples of
  `lua-atmos/env-iup` and also 7GUIs. Done: `hello`, `button-counter`,
  and the seven tasks. Not done: `iup-net`, which needs the network
  (`env-uv`, I6, to decide with the author).
- I5: `doc/api.md`, `doc/guide.md`, and the study of each task against
  a version with callbacks.
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
- A modal dialog as a case for `toggle` or `grab` (issue 22 of
  `atmos-lang/atmos`): the circle drawer does it with `ACTIVE=NO` on
  the main dialog. To report to the compiler session with the code.
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
