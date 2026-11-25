// 7GUIs, task 2: Temperature Converter. The version with callbacks, in C.

#include <stdlib.h>
#include <iup.h>
#include "script.h"

static Ihandle *tc, *tf;

// The text of a field as a number. Returns 0 if it is not one.
static int number (Ihandle* h, double* v) {
    const char* s = IupGetAttribute(h, "VALUE");
    char* end;
    if (s==NULL || *s=='\0') {
        return 0;
    }
    *v = strtod(s, &end);
    return *end == '\0';
}

static int on_celsius (Ihandle* h) {
    double c;
    if (number(tc, &c)) {
        IupSetStrf(tf, "VALUE", "%g", c*(9.0/5) + 32);
    }
    return IUP_DEFAULT;
}

static int on_fahrenheit (Ihandle* h) {
    double f;
    if (number(tf, &f)) {
        IupSetStrf(tc, "VALUE", "%g", (f-32) * (5.0/9));
    }
    return IUP_DEFAULT;
}

int main (int argc, char** argv) {
    IupOpen(&argc, &argv);
    tc = IupText(NULL);
    tf = IupText(NULL);
    IupSetAttribute(tc, "SIZE", "50");
    IupSetAttribute(tf, "SIZE", "50");
    IupSetCallback(tc, "VALUECHANGED_CB", on_celsius);
    IupSetCallback(tf, "VALUECHANGED_CB", on_fahrenheit);
    Ihandle* box = IupHbox(tc, IupLabel("Celsius ="), tf, IupLabel("Fahrenheit"), NULL);
    IupSetAttributes(box, "NGAP=10, ALIGNMENT=ACENTER");
    Ihandle* dlg = IupDialog(box);
    IupSetAttributes(dlg, "TITLE=TempConv, MARGIN=10x10");
    IupShow(dlg);
    IupSetHandle("c", tc);
    IupSetHandle("f", tf);
    IupSetHandle("dlg", dlg);
    study_script(NULL);
    IupMainLoop();
    IupClose();
    return 0;
}
