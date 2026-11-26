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
| #3 | The test mode by a global set in a `test` block (`envs` item 58) | done: a `test` block of `iup.atm` sets `ATI_TEST`; dialogs mapped, not shown; popups closed at once (`tst/unit`). IUP is still opened at the include (after the flag), as the manual allows |
| #4 | The C versions of the study are not driven by a script | done: `study/script.h` (the test mode in C) and `study/run.sh` (`make study`): the seven C versions run under the scripts of the Atmos versions and print the same traces; the C flight booker got its modal dialog and the C cells their double click, as the Atmos versions |
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
| #18 | Excerpt, idiomatic: a test drives the program by emitting the events of IUP (`tst/unit.atm:15-20`) | shown to the author 2026-11-26 |
| #19 | Excerpt, bad: a double click read from the 6th byte of a status string (`exs/7guis/cells.atm:178`) | closed: the author said yes (2026-11-26) to `status` as a table of booleans; `e.status.double` |
| #20 | Excerpt, idiomatic: `e.status.double` (`exs/7guis/cells.atm:178`) | shown to the author 2026-11-26 |
| #21 | Excerpt, bad: `e.but == 1`, a bare number for the left button | closed: the author said yes (2026-11-26); `e.but` is a tag (`:left`, `:middle`, `:right`), and the buttons of `status` have the same names |
| #22 | Excerpt: the two rules of the canvas of the circle drawer (`exs/7guis/circles.atm:136`) | closed: rewritten as the author suggested (2026-11-26), the press as a predicate of the loop (`until it.press`) and the exclusive cases in an `ifs` |
| #23 | Excerpt: the id of a cell kept in an attribute of its control (`exs/7guis/cells.atm:177`) | closed: the author finds it ok; the C version does the same (`IupSetInt`/`IupGetInt` of `CELL`) |

## A. Decisions waiting for the author

- (none)

## B. Work known and not done

- #10: IUP 3.32 distributes binaries for Linux and Windows only; the
  readme says Linux on x86-64. Windows is not tried.

## C. Reported and not acknowledged

- (none)

## Context

- At each big step (a milestone, a feature that changes the screen),
  show the author a few screenshots in the report (his recommendation,
  2026-11-26).
- For each group of changes, show the author one pair of Atmos excerpts
  from that work, one idiomatic and one bad (awkward, verbose, a
  workaround), short, with file and line (his rule, 2026-11-26). Each
  excerpt takes a number `#N` of this index, and a bad one says why and
  what would make it better (the language, the environment, the port).
- Coordination, status and questions for the author go to the session
  `atmos` (session_011eECohhJbc4GMpAj3yyeEo), since 2026-11-24;
  measurements on the ports and merges into lang `main` stay with
  `atmos-lang`. Paused until next week's credits: no new work, only
  what arrives.
- Findings about `pin` go to the session `atmos-pin`
  (session_011eECogSDhkVfyt7qme5DLh), with a small program; anything
  else of the compiler to `atmos-lang` (session_011eECogKVQ7ZzmWCoMST2wK).
- Repository: `ia-atmos/env-iup` (formerly `fsantanna/iup`).
- Subject: an environment for graphical user interfaces in Atmos over the IUP
  library (Tecgraf/PUC-Rio), as a direct binding to its C API.
- Related: `lua-atmos/env-iup` (same idea for the Lua runtime, over iup-lua).
- Compiler: `ia-atmos/lang`, branch `main`.
