# TODO

Everything pending in this session: explicit, unanswered, or unacknowledged.
Kept current as items open and close.

## A. Decisions waiting for the author

- The local remote still points to `fsantanna/iup`: the change to
  `ia-atmos/env-iup` was blocked by the session. Pushes work by the
  redirect.

## B. Work known and not done

- What to build (the author, 2026-11-14): all the examples of
  `lua-atmos/env-iup` and also 7GUIs. `hello` and `button-counter` are
  done; `iup-net` needs the network (`env-uv`, I6).
- I2: counter, temperature converter, flight booker. I3: timer, CRUD.
  I4: circle drawer, cells.
- The callbacks with arguments (the mouse on a canvas, the keys, the
  item of a list): today only `ACTION`, `VALUECHANGED_CB`, and
  `CLOSE_CB` become events, with the handle alone.
- A modal dialog (`IupPopup`): not bound. The events are queued, so a
  nested loop of IUP would hold them until it returns.
- The screenshots are saved and not compared: decide where the pixels
  are stable.
- The Mac: IUP 3.32 distributes binaries for Linux and Windows only;
  the readme says Linux on x86-64. Windows is not tried.
- `www.tecgraf.puc-rio.br` is not reachable from the container (the
  documents of IUP): SourceForge is.
- `doc/api.md`, `doc/guide.md` (I5).

## C. Reported and not acknowledged

- To the compiler session (2026-11-14): a pinned value cannot be in the
  pattern of an await (`pin dlg` then `watching :close [h=dlg]` is
  "invalid escape : pinned value"), and a pattern kept in a variable
  is not accepted. Workaround here: the dialog is a `val`.

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
