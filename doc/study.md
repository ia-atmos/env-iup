# Study: 7GUIs with events in sequence and with callbacks

The seven tasks of [7GUIs][7guis] are written twice over the same
library, IUP 3.32, with the same controls, the same layout, and the
same behavior:

- `exs/7guis/*.atm`: in Atmos, with this environment. A callback is an
  event, and the program awaits it.
- `study/*.c`: in C, with the callbacks of IUP. A callback is a
  function, and the state that it shares with the others is global.

This document compares them. It is a small study by who wrote both
versions, not a measurement with other people: the numbers are those
of these fourteen files and no more.

## What was compared, and how

- **Lines**: the lines of each file that are neither blank nor only a
  comment.
- **Callbacks**: the functions that the C version registers in IUP.
  The Atmos version has none; its counterpart is the number of
  `loop on` and `await` that react to the controls.
- **State**: the variables that hold the state of the program between
  two reactions, and where they are declared. The handles of the
  controls are not counted.

What holds for both versions: the Atmos versions are tested (a script
of actions each, in `tst/`), and the C versions were only compiled with
`-Wall` and started under `xvfb-run`, not driven by a script. So the
claim that the two behave the same comes from writing one from the
other, function by function, and not from a test.

## Numbers

| Task | Lines, Atmos | Lines, C | Callbacks in C | Reactions in Atmos |
|------|-------------:|---------:|---------------:|-------------------:|
| Counter | 16 | 25 | 1 | 1 |
| Temperature converter | 30 | 43 | 2 | 2 |
| Flight booker | 63 | 63 | 2 | 2 |
| Timer | 43 | 70 | 3 | 3 |
| CRUD | 98 | 117 | 5 | 5 |
| Circle drawer | 148 | 175 | 8 | 6, and 2 awaits in sequence |
| Cells | 168 | 228 | 2 | 1, and a task per formula |
| Total | 566 | 721 | 23 | |

The Atmos versions have about 80% of the lines of the C ones. Most of
the difference is not in the logic: C declares, converts, and creates
the controls with more text (`IupSetAttribute` one at a time, `"YES"`
and `"NO"`, the `main` with `IupOpen` and `IupClose`). The part that
comes from the model is smaller, and is in the next sections.

| Task | State in C | State in Atmos |
|------|------------|----------------|
| Counter | `n`, global | `n`, local to the block that counts |
| Temperature converter | none | none |
| Flight booker | none | none |
| Timer | `elapsed`, `duration`, `prv`, global | `elapsed`, `duration`, top-level |
| CRUD | `db`, `db_n`, `shown`, global | `db`, `shown`, top-level |
| Circle drawer | `cs`, `cs_n`, `hist`, `hist_n`, `done`, `over`, `adj_c`, `adj_old`, global | `cs`, `hist`, `done`, `over`, top-level; `old`, local to the task of the dialog |
| Cells | `cells` (with `deps` and `busy` in each), `focus`, and `src`, `cur`, `fail` of the parser, global | `srcs`, `vals`, `hs`, `focus`, `run`, top-level; the state of the parser, local to `eval` |

## Where the sequence made a difference

**The life of the program.** In C the program lives while
`IupMainLoop` runs, and what a callback may touch is everything, always.
In Atmos the program is `watching :close [h=dlg] { ... }`: the
reactions exist while the dialog does, and what belongs to them is
declared inside. In the counter this is the whole difference: `n` is a
local of the block.

**The timer.** The C version needs a timer of IUP, the time of the
last tick (`prv`), and the arithmetic of the interval. In Atmos the
time is an event like the others (`loop dt on :clock`), in a branch
next to the slider and the reset. The three callbacks became three
branches of a `par`, with the same bodies.

**The dialog of the circle drawer.** This is the clearest case. In C,
adjusting a diameter is spread over four callbacks: the right click
stores the circle in `adj_c` and opens the menu; the item of the menu
reads `adj_c`, saves the old diameter in `adj_old`, and opens the
dialog; the slider changes the circle; the close of the dialog compares
with `adj_old`, records the history, and clears `adj_c`. The two
globals only exist to carry the circle from one callback to the next,
and are only valid between two of them.

In Atmos it is one sequence in one place:

```
iup.popup(menu)
val x = await <:action [h=item] || :popup [h=menu]>
if x.tag == :action {
    await adjust(c)
}
```

and `adjust` is a task with the circle as its argument and the old
diameter as a local: show the dialog, react to the slider until the
dialog closes, record the history. Nothing is stored to be found by
another function later.

**The cells.** In C the propagation is written by hand: each cell has
the set of cells that its formula reads (`deps`), a change computes the
cell and then, recursively, the cells that read it, and a flag (`busy`)
stops a cycle. In Atmos a formula is a task that computes, tells that
it changed, and awaits a change in what it reads:

```
task Formula (id, src) {
    loop {
        pin v, deps = eval(src)
        set vals@id = v
        show(id)
        emit @:global :cell [id=id]
        await :cell until deps@(it.id)
    }
}
```

There is no traversal and no flag for cycles: a task does not react to
an event while it emits its own. A new formula aborts the task of the
old one. The price is one task for each formula, and an order of
computation that is that of the tasks, not of the dependencies: a cell
that reads two others may be computed twice for one change.

## Where it made none

**The flight booker** has the same lines in both. It is a set of
constraints over the present values of four controls: one function
computes them all, and any change calls it. There is no state and no
sequence, and a callback is as direct as an event.

**The temperature converter and the CRUD** are the same programs in the
two versions, a function for a loop: each reaction is independent of
the others and of what came before.

## What the Atmos version pays

- **Attributes are strings.** A value is read with
  `tonumber(iup.get(h, "VALUE"))`, and a boolean that is `nil` resets
  an attribute instead of setting it to `"NO"` (the flight booker
  needs `!!ok`). C has `IupGetInt` and `IupSetInt`; the binding is thin
  and does not.
- **The rules of `pin`.** A handle is declared with `pin`, and a value
  created in a reaction for a collection of the program is built
  inside the insert (the circle drawer). The C version has no such
  rule, and no protection either: the history points into an array.
- **A popup of IUP cannot be called inside a reaction**, since it only
  returns when it closes. `iup.popup` defers it, and the program awaits
  its end. In C it is a plain call.
- **A modal dialog is not modal by the language alone.** The branch
  that awaits `adjust` does not react meanwhile, but its siblings (undo,
  redo) do: the program disables the main dialog in IUP, as the C
  version also does.

## What was not measured

- The time and the memory of each version. The cells create 2600
  controls in both; the Atmos one also has a task per formula.
- Other people. Whether the sequence is easier to read or to change is
  argued above from the code, not observed.
- Other libraries. IUP has attributes as strings and synchronous
  popups; another toolkit would move some of the costs above.

[7guis]: https://eugenkiss.github.io/7guis/
