// 7GUIs, task 1: Counter. The version with callbacks, in C.

#include <stdlib.h>
#include <iup.h>
#include "script.h"

static Ihandle* txt;
static int n = 0;

static int on_count (Ihandle* h) {
    n++;
    IupSetInt(txt, "VALUE", n);
    return IUP_DEFAULT;
}

int main (int argc, char** argv) {
    IupOpen(&argc, &argv);
    txt = IupText(NULL);
    IupSetAttributes(txt, "VALUE=0, READONLY=YES, SIZE=50");
    Ihandle* but = IupButton("Count", NULL);
    IupSetAttribute(but, "SIZE", "50");
    IupSetCallback(but, "ACTION", on_count);
    Ihandle* box = IupHbox(txt, but, NULL);
    IupSetAttribute(box, "NGAP", "10");
    Ihandle* dlg = IupDialog(box);
    IupSetAttributes(dlg, "TITLE=Counter, MARGIN=10x10");
    IupShow(dlg);
    IupSetHandle("txt", txt);
    IupSetHandle("but", but);
    IupSetHandle("dlg", dlg);
    study_script(NULL);
    IupMainLoop();
    IupClose();
    return 0;
}
