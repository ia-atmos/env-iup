// I0: the check. A dialog with a button, under a virtual display.
// The program presses the button with a real mouse event and takes a
// screenshot. Exits with 0 if the callback of the button ran once.

#include <stdio.h>
#include <stdlib.h>
#include <iup.h>

static Ihandle *btn, *lbl;
static int clicks = 0, step = 0;

static int on_action (Ihandle* h) {
    clicks++;
    IupSetfAttribute(lbl, "TITLE", "clicks: %d", clicks);
    printf("action: clicks=%d\n", clicks);
    fflush(stdout);
    return IUP_DEFAULT;
}

static int on_timer (Ihandle* t) {
    int x, y, w, h;
    IupGetIntInt(btn, "SCREENPOSITION", &x, &y);
    IupGetIntInt(btn, "RASTERSIZE", &w, &h);
    x += w/2;
    y += h/2;
    switch (step++) {
        case 0: IupSetfAttribute(NULL, "CURSORPOS", "%dx%d", x, y); break;
        case 1: IupSetfAttribute(NULL, "MOUSEBUTTON", "%dx%d 1 1", x, y); break;
        case 2: IupSetfAttribute(NULL, "MOUSEBUTTON", "%dx%d 1 0", x, y); break;
        case 3: if (system("import -window root out/i0.png") != 0) { clicks = -1; } break;
        default: return IUP_CLOSE;
    }
    return IUP_DEFAULT;
}

int main (int argc, char** argv) {
    IupOpen(&argc, &argv);
    printf("IUP %s, driver %s\n", IupVersion(), IupGetGlobal("DRIVER"));
    lbl = IupLabel("clicks: 0");
    btn = IupButton("Press", NULL);
    IupSetCallback(btn, "ACTION", (Icallback)on_action);
    Ihandle* dlg = IupDialog(IupVbox(lbl, btn, NULL));
    IupSetAttribute(dlg, "TITLE", "I0");
    IupSetAttribute(dlg, "SIZE", "120x60");
    IupShowXY(dlg, 50, 50);
    Ihandle* t = IupTimer();
    IupSetAttribute(t, "TIME", "300");
    IupSetCallback(t, "ACTION_CB", (Icallback)on_timer);
    IupSetAttribute(t, "RUN", "YES");
    IupMainLoop();
    IupClose();
    printf("end: clicks=%d\n", clicks);
    return clicks == 1 ? 0 : 1;
}
