// 7GUIs, task 3: Flight Booker. The version with callbacks, in C.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iup.h>
#include "script.h"

static Ihandle *cmb, *t1, *t2, *but, *msg, *box;

// The date `dd.mm.yyyy` as a number that can be compared, or 0.
static int date (const char* s) {
    int d, m, y, n;
    if (s==NULL || sscanf(s, "%d.%d.%4d%n", &d, &m, &y, &n)!=3 || s[n]!='\0' || y<1000) {
        return 0;
    }
    int leap = (y%4==0 && y%100!=0) || y%400==0;
    int days[] = { 31, leap ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (m<1 || m>12 || d<1 || d>days[m-1]) {
        return 0;
    }
    return y*10000 + m*100 + d;
}

// Shows the constraints in the controls.
static int update (Ihandle* h) {
    int ret = IupGetInt(cmb, "VALUE") == 2;
    int d1  = date(IupGetAttribute(t1, "VALUE"));
    int d2  = date(IupGetAttribute(t2, "VALUE"));
    IupSetAttribute(t2, "ACTIVE", ret ? "YES" : "NO");
    IupSetAttribute(t1, "BGCOLOR", d1 ? "255 255 255" : "255 0 0");
    IupSetAttribute(t2, "BGCOLOR", (d2 || !ret) ? "255 255 255" : "255 0 0");
    IupSetAttribute(but, "ACTIVE", (d1 && (!ret || (d2 && d2>=d1))) ? "YES" : "NO");
    return IUP_DEFAULT;
}

static int on_book (Ihandle* h) {
    const char* a = IupGetAttribute(t1, "VALUE");
    if (IupGetInt(cmb, "VALUE") == 2) {
        IupSetStrf(msg, "TITLE", "You have booked a return flight on %s and %s.",
            a, IupGetAttribute(t2, "VALUE"));
    } else {
        IupSetStrf(msg, "TITLE", "You have booked a one-way flight on %s.", a);
    }
    IupPopup(box, IUP_CENTERPARENT, IUP_CENTERPARENT);  // the confirmation
    return IUP_DEFAULT;
}

static int on_ok (Ihandle* h) {
    return IUP_CLOSE;           // closes the popup
}

int main (int argc, char** argv) {
    IupOpen(&argc, &argv);
    cmb = IupList(NULL);
    IupSetAttributes(cmb, "DROPDOWN=YES, SIZE=90, 1=\"one-way flight\", 2=\"return flight\", VALUE=1");
    t1  = IupText(NULL);
    t2  = IupText(NULL);
    IupSetAttributes(t1, "VALUE=27.03.2014, SIZE=90");
    IupSetAttributes(t2, "VALUE=27.03.2014, SIZE=90");
    but = IupButton("Book", NULL);
    IupSetAttribute(but, "SIZE", "90");
    msg = IupLabel("");
    IupSetAttribute(msg, "SIZE", "260");
    Ihandle* ok = IupButton("OK", NULL);
    IupSetAttribute(ok, "SIZE", "40");
    IupSetCallback(ok, "ACTION", on_ok);
    Ihandle* mbox = IupVbox(msg, ok, NULL);
    IupSetAttributes(mbox, "NGAP=10, ALIGNMENT=ACENTER");
    box = IupDialog(mbox);
    IupSetAttributes(box, "TITLE=Booked, MARGIN=10x10");
    IupSetCallback(cmb, "VALUECHANGED_CB", update);
    IupSetCallback(t1,  "VALUECHANGED_CB", update);
    IupSetCallback(t2,  "VALUECHANGED_CB", update);
    IupSetCallback(but, "ACTION", on_book);
    Ihandle* vbox = IupVbox(cmb, t1, t2, but, NULL);
    IupSetAttribute(vbox, "NGAP", "5");
    Ihandle* dlg = IupDialog(vbox);
    IupSetAttributes(dlg, "TITLE=\"Book Flight\", MARGIN=10x10");
    IupShow(dlg);
    update(NULL);
    IupSetHandle("cmb", cmb);
    IupSetHandle("t1", t1);
    IupSetHandle("t2", t2);
    IupSetHandle("but", but);
    IupSetHandle("msg", msg);
    IupSetHandle("ok", ok);
    IupSetHandle("box", box);
    IupSetHandle("dlg", dlg);
    study_script(NULL);
    IupMainLoop();
    IupDestroy(box);
    IupClose();
    return 0;
}
