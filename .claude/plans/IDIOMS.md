# Idioms

What the author corrected or approved in the Atmos code of this
repository, as rules to follow in new code. Each rule has the item of
`TODO.md` it came from, and a before and an after. Sent to the session
`atmos` to be gathered with those of the other projects.

## In a program

### A condition that only filters an event goes in the pattern (#22)

The loop does not wake for the events it would discard.

```
;; before
loop e on :button [h=cnv] {
    if e.press {
        <...>
    }
}

;; after
loop e on :button [h=cnv] until e.press {
    <...>
}
```

The predicate names the event by the variable of the loop (`e`, not
`it`), since lang `9d4561a`. It does not apply when the condition needs something computed inside
the body. A predicate with more than one term goes in parentheses
(#20):

```
loop e on :button until (e.press && (e.but == :left) && e.status.double) {
    <...>
}
```

### Exclusive cases go in an `ifs` (#22)

```
;; before
if (e.but == :left) && (c == nil) { <...> }
if (e.but == :right) && (c != nil) { <...> }

;; after
ifs {
    (e.but == :left) && (c == nil)  => { <...> }
    (e.but == :right) && (c != nil) => { <...> }
}
```

### A loop with no variable has none

`loop in n {...}`, not `loop _ in n {...}` (the author, 2026-11-22). A
`loop _,v in t` that uses the value stays.

### A statement takes its argument with no parentheses (#18)

`emit`, as `await`, `throw`, and `return`, is a statement, not a call.

```
;; before
emit(:action [h=but])

;; after
emit :action [h=but]
```

### The idiom of the library is fine (#23)

A datum of the program kept in the control, as IUP does
(`cell=id` at creation, `iup.get(e.h, "CELL")` in the event), is not a
workaround: the C version does the same.

## In an environment

### An event does not carry an encoding of the library (#19, #21)

What the library packs in a string of fixed positions or in a bare
number is decoded by the environment, with the tests of the library
itself: a table of booleans, a tag.

```
;; before
(string.sub(e.status, 5, 5) == "D")     ;; the 6th byte of a string of IUP
(e.but == 1)                            ;; the left button

;; after
e.status.double
(e.but == :left)
```

The same thing has one name everywhere: the buttons are `left`,
`middle`, `right` both in `e.but` and in `e.status`.
