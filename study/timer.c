// 7GUIs, task 4: Timer. The version with callbacks, in C.

#include <stdlib.h>
#include <time.h>
#include <iup.h>

static Ihandle *bar, *lbl, *sld;
static double elapsed  = 0;     // both in seconds
static double duration = 15;
static double prv;              // the time of the last tick

static double now (void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec/1e9;
}

static void show (void) {
    IupSetStrf(lbl, "TITLE", "%.1fs", elapsed);
    IupSetDouble(bar, "VALUE", (duration > 0) ? elapsed/duration : 1);
}

static int on_tick (Ihandle* h) {
    double cur = now();
    double dt  = cur - prv;
    prv = cur;
    if (elapsed < duration) {
        elapsed += dt;
        if (elapsed > duration) {
            elapsed = duration;
        }
        show();
    }
    return IUP_DEFAULT;
}

static int on_duration (Ihandle* h) {
    duration = IupGetDouble(sld, "VALUE");
    show();
    return IUP_DEFAULT;
}

static int on_reset (Ihandle* h) {
    elapsed = 0;
    show();
    return IUP_DEFAULT;
}

int main (int argc, char** argv) {
    IupOpen(&argc, &argv);
    bar = IupProgressBar();
    IupSetAttributes(bar, "MIN=0, MAX=1, SIZE=120");
    lbl = IupLabel("0.0s");
    IupSetAttribute(lbl, "SIZE", "120");
    sld = IupVal(NULL);
    IupSetAttributes(sld, "MIN=0, MAX=30, VALUE=15, SIZE=120x15");
    Ihandle* rst = IupButton("Reset", NULL);
    IupSetAttribute(rst, "SIZE", "120");
    IupSetCallback(sld, "VALUECHANGED_CB", on_duration);
    IupSetCallback(rst, "ACTION", on_reset);
    Ihandle* h1 = IupHbox(IupLabel("Elapsed Time:"), bar, NULL);
    Ihandle* h2 = IupHbox(IupLabel("Duration:"), sld, NULL);
    IupSetAttribute(h1, "NGAP", "10");
    IupSetAttribute(h2, "NGAP", "10");
    Ihandle* box = IupVbox(h1, lbl, h2, rst, NULL);
    IupSetAttribute(box, "NGAP", "5");
    Ihandle* dlg = IupDialog(box);
    IupSetAttributes(dlg, "TITLE=Timer, MARGIN=10x10");
    IupShow(dlg);

    Ihandle* tick = IupTimer();
    IupSetAttribute(tick, "TIME", "10");
    IupSetCallback(tick, "ACTION_CB", on_tick);
    prv = now();
    IupSetAttribute(tick, "RUN", "YES");

    IupMainLoop();
    IupDestroy(tick);
    IupClose();
    return 0;
}
