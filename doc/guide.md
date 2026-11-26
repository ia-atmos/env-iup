# Guide

This guide builds a few small programs with the environment. The
[API](api.md) has the details, and `exs/` has the complete programs.

## A program

A program is run with the compiler of the environment:

```
./atmos-iup exs/hello.atm
```

The simplest one has no dialog, only the time:

```
watching 5s {
    loop on 500ms {
        print("Hello World!")
    }
}
```

## A dialog

A control is created with `iup.new`, from the name of its class in
IUP and a table with its attributes and its children. A dialog is a
control like the others, and `iup.show` puts it on the screen:

```
pin txt = iup.new(:text,   [value=0, readonly=true, size=60])
pin but = iup.new(:button, [title="Count", size=60])
pin dlg = iup.new(:dialog, [
    title  = "Counter",
    margin = "10x10",
    iup.new(:hbox, [txt, but, ngap=10]),
])
iup.show(dlg)
```

The names and the values of the attributes are those of the
documentation of IUP. They are read and set with `iup.get` and
`iup.set`, always as strings.

## Events

In IUP, a control calls a function when something happens to it. Here
it emits an event, and the program awaits it:

```
watching :close [h=dlg] {
    var n = 0
    loop on :action [h=but] {
        set n = n + 1
        iup.set(txt, "VALUE", n)
    }
}
```

The program lives while the dialog is open: the `watching` terminates
when the user closes it, and with it the loop inside. The count is a
local variable of the block that uses it.

## Concurrent reactions

The reactions that are independent go in the branches of a `par`. In
the temperature converter, each field converts to the other:

```
watching :close [h=dlg] {
    par {
        loop on :value [h=tc] {
            val c = tonumber(iup.get(tc, "VALUE"))
            if c {
                iup.set(tf, "VALUE", string.format("%g", c*(9/5) + 32))
            }
        }
    } with {
        loop on :value [h=tf] {
            val f = tonumber(iup.get(tf, "VALUE"))
            if f {
                iup.set(tc, "VALUE", string.format("%g", (f-32) * (5/9)))
            }
        }
    }
}
```

A value that the program sets does not emit `:value`: only the user
does. So the two branches do not call each other.

## Time

The time is an event as the others. In the timer, one branch lets the
time pass and the others change what it counts:

```
par {
    loop dt on :clock {
        if elapsed < duration {
            set elapsed = math.min(elapsed+dt, duration)
            show()
        }
    }
} with {
    loop on :value [h=sld] {
        set duration = tonumber(iup.get(sld, "VALUE")) * 1s
        show()
    }
} with {
    loop on :action [h=rst] {
        set elapsed = 0
        show()
    }
}
```

## A dialog in sequence

A dialog that asks something and returns is a task: it shows the
dialog, awaits what happens in it, and terminates. Who needs the answer
awaits the task, and does not react to anything else meanwhile. In the
circle drawer, a right click on a circle opens a dialog that adjusts
its diameter:

```
task adjust (c) {
    iup.set(sld, "VALUE", c.d)
    iup.set(dlg, "ACTIVE", false)       ;; holds the main dialog
    iup.show(adj)
    watching :close [h=adj] {
        loop on :value [h=sld] {
            set c.d = math.floor(tonumber(iup.get(sld, "VALUE")))
            iup.update(cnv)
        }
    }
    iup.set(dlg, "ACTIVE", true)
}

loop e on :button [h=cnv] {
    ...
    await adjust(c)                     ;; until the dialog closes
    ...
}
```

The other branches of the program stay alive, so the main dialog is
disabled while the other one is open.

## A canvas

A canvas emits `:action` when it must be drawn, and the events of the
mouse. The program keeps what it draws, and asks for a new drawing
with `iup.update` when that changes:

```
par {
    loop on :action [h=cnv] {
        iup.draw(cnv, :begin)
        loop _,c in circles {
            iup.draw(cnv, :arc, c.x-c.r, c.y-c.r, c.x+c.r, c.y+c.r)
        }
        iup.draw(cnv, :end)
    }
} with {
    loop e on :button [h=cnv] {
        if e.press && (e.but == :left) {
            vector.insert(circles, [x=e.x, y=e.y, r=20])
            iup.update(cnv)
        }
    }
}
```

## Many tasks

In the cells, each formula is a task in a pool. It computes its value,
tells the others that it changed, and awaits a change in a cell that it
reads:

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

A new formula for a cell aborts the task of the old one. A cycle
between formulas does not hang the program: a task does not react to
an event while it emits its own.

## Tests

A program is tested with no person, under a script of actions:

```
IUP_ATM_SCRIPT="100:click:but;200:click:but;300:get:txt:VALUE;400:close:dlg" \
    xvfb-run -a ./atmos-iup exs/7guis/counter.atm
```

The controls that the script uses have a `HANDLENAME`. The time is
virtual, so a test of the timer does not wait for it. `tst/run.sh`
runs the tests of the repository, each as a program, a script, and the
expected output.
