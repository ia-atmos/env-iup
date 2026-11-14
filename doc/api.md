# API

The environment is the file `iup.atm`, which declares the table `iup`
and runs the program inside the loop of [IUP][iup]. The binding is thin
and generic: it has a few functions over the generic interface of IUP
(`IupCreate`, `IupSetStrAttribute`, `IupGetAttribute`,
`IupSetCallback`), and not one function per control. The classes, the
attributes, and the callbacks are those of the documentation of IUP.

- [Functions](#functions)
- [Events](#events)
- [What a callback returns](#what-a-callback-returns)
- [Handles](#handles)
- [Time and termination](#time-and-termination)
- [Test mode](#test-mode)

## Functions

### `iup.new(class, t)`

Creates a control of a class of IUP, given by its name: `:label`,
`:button`, `:text`, `:list`, `:val`, `:progressbar`, `:canvas`,
`:hbox`, `:vbox`, `:gridbox`, `:scrollbox`, `:fill`, `:dialog`,
`:menu`, `:item`, and any other. Evaluates to its [handle](#handles).

The optional table `t` has the attributes and the children of the
control:

- A named field is an attribute, as in [`iup.set`](#iupseth-name-v).
- A positional field is a child, in order. A child is a handle with no
  parent.

```
val txt = iup.new(:text,   [value=0, readonly=true, size=60])
val but = iup.new(:button, [title="Count"])
val dlg = iup.new(:dialog, [title="Counter", iup.new(:hbox, [txt, but, ngap=10])])
```

Errors: `invalid class`, `invalid attributes`, `invalid child`,
`invalid value`.

### `iup.set(h, name, v)`

Sets an attribute of a control. The name is that of IUP, in any case
(`"title"` is `TITLE`). The value is:

- a string, as it is;
- a number, converted to a string;
- a boolean: `"YES"` or `"NO"`;
- `nil`: the attribute gets its default back.

The name `HANDLENAME` gives the control a global name
(`IupSetHandle`), which the [test mode](#test-mode) uses.

A name that IUP does not know is an attribute of the program, which
`iup.get` reads back. As any attribute of IUP, a control that does not
have it takes it from its parent.

### `iup.get(h, name)`

Reads an attribute of a control: a string, or `nil`. A number comes as
a string: use `tonumber`.

### `iup.show(h)`, `iup.hide(h)`

Shows or hides a dialog.

### `iup.popup(h, x, y)`

Shows a menu or a dialog as a popup of IUP, which holds the other
dialogs until it closes. The position is optional: a menu opens at the
mouse, and a dialog at the center of its parent.

A popup of IUP only returns when it is closed, so it is not shown at
once: it is shown when the program ends its reaction, and the event
`:popup [h]` tells that it was closed.

```
iup.popup(menu)
val e = await <:action [h=item] || :popup [h=menu]>
if e.tag == :action {
    ;; the item was chosen
}
```

### `iup.update(h)`

Asks a control to be drawn again. A canvas then receives `:action`.

### `iup.draw(h, op, ...)`

Draws on a canvas, with the operations of `IupDraw`. The color and the
style are attributes of the canvas (`DRAWCOLOR`, `DRAWSTYLE`,
`DRAWLINEWIDTH`, `DRAWFONT`).

| Operation | |
|-----------|-|
| `iup.draw(h, :begin)` | Starts to draw |
| `iup.draw(h, :end)` | Shows what was drawn |
| `iup.draw(h, :line, x1, y1, x2, y2)` | A line |
| `iup.draw(h, :rectangle, x1, y1, x2, y2)` | A rectangle |
| `iup.draw(h, :arc, x1, y1, x2, y2 [, a1, a2])` | An arc of the ellipse inside a rectangle, between two angles in degrees (the whole of it, by default) |
| `iup.draw(h, :text, str, x, y)` | A text |

A canvas is drawn in the reaction to its `:action`:

```
loop on :action [h=cnv] {
    iup.draw(cnv, :begin)
    iup.set(cnv, "DRAWCOLOR", "255 0 0")
    iup.set(cnv, "DRAWSTYLE", "FILL")
    iup.draw(cnv, :arc, 10, 10, 50, 50)
    iup.draw(cnv, :end)
}
```

## Events

A callback of a control is an event: a table with the tag of the
callback and the handle of the control in the field `h`. The callbacks
are set in every control that `iup.new` creates.

| Event | Callback of IUP | |
|-------|-----------------|-|
| `:action [h]` | `ACTION` | A button was pressed, an item of a menu was chosen, a canvas must be drawn |
| `:value [h]` | `VALUECHANGED_CB` | The user changed a text, a list, a slider |
| `:close [h]` | `CLOSE_CB` | The user closed a dialog |
| `:button [h,but,press,x,y,status]` | `BUTTON_CB` | A button of the mouse (`but` from `1` to `3`) was pressed (`press` is `true`) or released on a canvas |
| `:motion [h,x,y,status]` | `MOTION_CB` | The mouse moved on a canvas |
| `:focus [h,focus]` | `GETFOCUS_CB`, `KILLFOCUS_CB` | A control got or lost the focus of the keyboard |
| `:popup [h]` | | A popup was closed |

```
await :action [h=but]           ;; this button
loop e on :button [h=cnv] {     ;; the mouse on this canvas
    print(e.x, e.y)
}
loop e on :focus {              ;; any control
    print(iup.get(e.h, "HANDLENAME"), e.focus)
}
```

An event is emitted inside its callback, so the program reacts before
IUP goes on. The exception is an event that happens while the program
reacts, because it called a function of IUP that called a callback:
it waits for the reaction to end.

## What a callback returns

Some callbacks return a value to IUP: a `CLOSE_CB` that returns
`IUP_IGNORE` keeps its dialog open. An event returns nothing, so the
program sets what a callback will return before it happens, in an
attribute of the control with the name of the callback and the prefix
`ATM_`:

```
iup.set(dlg, "ATM_CLOSE_CB", "IGNORE")      ;; the dialog does not close
await :close [h=dlg]                        ;; the user tried
```

The values are `DEFAULT` (when not set), `IGNORE`, `CONTINUE`, and
`CLOSE`.

## Handles

A handle is a native value of the language, which holds a control:

- A control with no parent is destroyed when its handle is released:
  when its last reference goes, or when the block of its `pin`
  terminates.
- A control inside another belongs to it: a dialog destroys all its
  controls. Their handles become invalid, and any function raises
  `invalid handle` for them.
- A control outlives its handle while it has a parent: an event gets
  a new handle for it.

A dialog cannot be a `pin` today if it appears in the table of a
pattern, since a pinned value cannot be stored in a table that is not
pinned: `await :close [h=dlg]` raises `invalid escape : pinned value`.
The examples declare it with `val`. With a `pin`, the pattern is
`:close until it.h == dlg`.

## Time and termination

Time passes inside the loop of IUP, so `await 1s` and
`loop dt on :clock` work as usual.

The program terminates when its main chunk terminates, and not when
the last dialog is closed: a program normally ends with a
`watching :close [h=dlg] { ... }`.

## Test mode

With the variable `IUP_ATM_SCRIPT`, the program runs with no person,
under a script of actions at given times, and with a virtual time that
passes in steps of 10ms. It needs a display, such as `xvfb-run`.

The script is a list of actions `ms:action:...`, separated by `;`. A
control is given by its `HANDLENAME`.

| Action | |
|--------|-|
| `ms:click:name[:x,y]` | Presses and releases the left button of the mouse on a control, at its center or at a position inside it |
| `ms:rclick:name[:x,y]` | The same with the right button |
| `ms:move:name[:x,y]` | Moves the mouse |
| `ms:set:name:ATTR=value` | Sets an attribute as if the user did it: the `VALUECHANGED_CB` is called |
| `ms:get:name:ATTR` | Prints an attribute, as `name.ATTR = value` |
| `ms:act:name` | Chooses an item of a menu: its `ACTION` is called, and the menu is closed |
| `ms:close:name` | Closes a dialog: the `CLOSE_CB` is called |
| `ms:shot:file.png` | Saves the screen (needs `import`, of ImageMagick) |
| `ms:quit` | Terminates the program |

```
IUP_ATM_SCRIPT="100:click:but;200:get:txt:VALUE;300:close:dlg" \
    xvfb-run -a ./atmos-iup exs/button-counter.atm
```

The clicks and the moves are real events of the mouse. The others
call the callbacks of IUP directly.

[iup]: https://www.tecgraf.puc-rio.br/iup/
