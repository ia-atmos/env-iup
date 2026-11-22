# TODO

Everything pending in this session: explicit, unanswered, or unacknowledged.
Kept current as items open and close.

Items are `#N` (rule of the author, 2026-11-18): unique in this session,
in order, never reused; from another session, `session#N`. The index is
below; the sections refer to it.

## Index

| # | Title | State |
|---|-------|-------|
| #1 | The versions in Lua of 7GUIs to compare with | closed: the author (2026-11-22): nothing new in Lua; only if the code existed, and it does not |
| #2 | The local remote points to `fsantanna/iup` | open, blocked by the session (pushes work by the redirect) |
| #3 | The test mode by a global set in a `test` block (`envs` item 58) | open: the shape is set by env-sdl (a C global `IUP_TEST` set in a `test` block; no dialog shown; IUP opened after the main chunk starts; `IUP_ATM_SCRIPT` keeps working). Not done yet |
| #4 | The C versions of the study are not driven by a script | open |
| #5 | A real click on an item of a menu does nothing in the test mode | done: the events that IUP synthesizes have no device of GDK, and a menu of GTK ignores them; the clicks now go through XTest |
| #6 | The flight booker shows its message in a label, not a dialog | done: a modal dialog (`iup.popup`) |
| #7 | The cells: no double click to edit, no infix operators | done: a double click edits; the formulas keep functions only, as the 7GUIs reference does (by the session, to be confirmed) |
| #8 | The keys (`K_ANY`) and the items of a list are not events | done: `:key` and the item of a list in `:action` |
| #9 | The screenshots are saved and not compared | done: the animations of GTK off, references in `tst/shots`, identical pixel by pixel (stable in this container; another machine with other fonts records its own) |
| #10 | The Mac and Windows: not supported / not tried | open |
| #11 | `www.tecgraf.puc-rio.br` is not reachable from the container | open, informative |
| #12 | What to build: all the examples of `lua-atmos/env-iup` and 7GUIs | closed, done |
| #13 | The interface of an environment in Atmos (`atmos.env`), pilot with `env-uv` | closed, done |
| #14 | The numeric options of the build (`ATMOS_REAL=float`, `ATMOS_INT=32`, `ATMOS_TIME=ms`) | done: the time is emitted in the unit of the build; the suite passes under each option (with `float`, only the printed digits of the timer differ) |
| #15 | A test of `iup.atm` as a secondary environment (`atmos-envs#13`) | done: `tst/secondary` |
| #16 | The rename of the numeric options (`A_TIME_US` -> `A_CLOCK_US`, atmos-float#21a) | closed: lang `float` merged (d19d40f), the bridge removed, 15/15 |
| #17 | Experiment: the modal of the circle drawer with `grab` (atmos-lang#112a) | done on a local branch `exp-grab` (3f0eadf, not pushed, not to merge); reported to atmos-grab and atmos-lang |

## A. Decisions waiting for the author

- (none)

## B. Work known and not done

- #3: instead of `IUP_ATM_SCRIPT` alone.
- #4: a script of actions for them would make the comparison of behavior
  a fact.
- #10: IUP 3.32 distributes binaries for Linux and Windows only; the
  readme says Linux on x86-64. Windows is not tried.

## C. Reported and not acknowledged

- (none)

## Context

- Findings about `pin` go to the session `atmos-pin`
  (session_011eECogSDhkVfyt7qme5DLh), with a small program; anything
  else of the compiler to `atmos-lang` (session_011eECogKVQ7ZzmWCoMST2wK).
- Repository: `ia-atmos/env-iup` (formerly `fsantanna/iup`).
- Subject: an environment for graphical user interfaces in Atmos over the IUP
  library (Tecgraf/PUC-Rio), as a direct binding to its C API.
- Related: `lua-atmos/env-iup` (same idea for the Lua runtime, over iup-lua).
- Compiler: `ia-atmos/lang`, branch `main`.
