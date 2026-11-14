# env-iup

An environment for graphical user interfaces in [Atmos][lang], over
[IUP][iup] (Tecgraf, PUC-Rio), as a direct binding to its C interface.

A callback of a control becomes an event, so the logic of a dialog is
written as a sequence in time:

```
val txt = iup.new(:text,   [value=0, readonly=true, size=60])
val but = iup.new(:button, [title="Count", size=60])
val dlg = iup.new(:dialog, [
    title  = "Button - Counter",
    margin = "10x10",
    iup.new(:hbox, [txt, but, ngap=10]),
])

iup.show(dlg)

watching :close [h=dlg] {
    loop on :action [h=but] {
        iup.set(txt, "VALUE", tonumber(iup.get(txt, "VALUE")) + 1)
    }
}
```

![The counter after three presses](doc/button-counter.png)

Status: experimental. The environment (`iup.atm`) and two examples
exist; the plan is in `.claude/plans/261113-env-iup.md`.

## Build

It needs the compiler ([`ia-atmos/lang`][lang]) built in `../lang`
(or `LANG_DIR=<dir>`), and the GTK 3 of the system (`libgtk-3-0`).
It is for Linux on x86-64.

```
make                                ;; fetches IUP, builds `atmos-iup`
./atmos-iup exs/button-counter.atm
```

`atmos-iup` is the compiler of Atmos dedicated to this environment: it
includes `iup.atm` before every program and passes the flags of IUP to
the C compiler. A program may also have `include "iup.atm"` and use
the plain compiler, with the flags in `ATMOS_CFLAGS` and
`ATMOS_LDFLAGS`.

IUP is not packaged in Ubuntu. `./get-iup.sh` fetches its official
binaries for Linux, at a fixed version (3.32, for x86-64 and GTK 3),
checks them by SHA-256, and extracts them into `iup/`, which git
ignores. Nothing of IUP is committed.

## Interface

The binding is thin and generic, as the C interface of IUP itself. The
header of `iup.atm` describes it in full.

| Function | |
|----------|-|
| `iup.new(class, t)` | A control of the given class of IUP (`:button`, `:text`, `:hbox`, `:dialog`, ...). The named fields of `t` are its attributes, and the positional ones are its children |
| `iup.set(h, name, v)` | Sets an attribute |
| `iup.get(h, name)` | Reads an attribute, as a string or `nil` |
| `iup.show(h)`, `iup.hide(h)` | Shows or hides a dialog |

| Event | Callback of IUP |
|-------|-----------------|
| `:action [h]` | `ACTION` (a button was pressed, ...) |
| `:value [h]` | `VALUECHANGED_CB` (a text was edited, ...) |
| `:close [h]` | `CLOSE_CB` (a dialog was closed) |

A handle is a native value: a control with no parent is destroyed when
its handle is released, and a dialog destroys its controls.

A known limitation: a dialog cannot be a `pin`, since a pinned value
cannot appear in the pattern of an `await` (`:close [h=dlg]`). It is a
`val`, and is destroyed when its last reference goes.

## Tests

```
make test
```

Each test is a program under a script of actions at given times (press
this button, set this value, close this dialog, save the screen), with
no person and no screen (`xvfb-run`), and with a virtual time. Its
output is compared with what is expected. They run plain and under the
address sanitizer. `make i0` is the first check of the plan: a program
in C that opens a dialog and presses its button.

[lang]: https://github.com/ia-atmos/lang
[iup]: https://www.tecgraf.puc-rio.br/iup/
