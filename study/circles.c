// 7GUIs, task 6: Circle Drawer. The version with callbacks, in C.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iup.h>
#include "script.h"
#include <iupdraw.h>

#define MAX 1024

typedef struct { int x, y, d; } Circle;
typedef struct { int dia; Circle* c; int old, new; } Entry;    // dia=0: a new circle

static Circle cs[MAX];      // the circles (never moved: the history points to them)
static int    cs_n = 0;
static Entry  hist[MAX];    // the history
static int    hist_n = 0;
static int    done = 0;     // the entries of the history that are applied
static Circle* over = NULL; // the circle under the mouse

static Ihandle *dlg, *cnv, *undo, *redo, *menu, *adj, *sld, *lbl;

// The state of the adjustment in course: the dialog is open between
// `on_adjust` and `on_adjust_close`.
static Circle* adj_c   = NULL;
static int     adj_old = 0;

// The circle that has the point, the nearest one first, or NULL.
static Circle* find (int x, int y) {
    Circle* best = NULL;
    double min = 0;
    for (int i=0; i<cs_n; i++) {
        double dist = hypot(cs[i].x-x, cs[i].y-y);
        if (dist<=cs[i].d/2.0 && (best==NULL || dist<min)) {
            best = &cs[i];
            min  = dist;
        }
    }
    return best;
}

static void buttons (void) {
    IupSetAttribute(undo, "ACTIVE", (done > 0) ? "YES" : "NO");
    IupSetAttribute(redo, "ACTIVE", (done < hist_n) ? "YES" : "NO");
}

// A new entry of the history, which drops what was undone.
static void record (Entry e) {
    if (done < MAX) {
        hist[done++] = e;
        hist_n = done;
    }
    buttons();
}

// Applies or reverts an entry of the history.
static void apply (Entry* e, int fwd) {
    if (!e->dia) {
        if (fwd) {
            cs_n++;
        } else {
            cs_n--;
            over = NULL;
        }
    } else {
        e->c->d = fwd ? e->new : e->old;
    }
}

static int on_draw (Ihandle* h) {
    IupDrawBegin(cnv);
    IupSetAttribute(cnv, "DRAWCOLOR", "255 255 255");
    IupSetAttribute(cnv, "DRAWSTYLE", "FILL");
    IupDrawRectangle(cnv, 0, 0, 2000, 2000);
    for (int i=0; i<cs_n; i++) {
        Circle* c = &cs[i];
        int r = c->d / 2;
        if (c == over) {
            IupSetAttribute(cnv, "DRAWCOLOR", "200 200 200");
            IupSetAttribute(cnv, "DRAWSTYLE", "FILL");
            IupDrawArc(cnv, c->x-r, c->y-r, c->x+r, c->y+r, 0, 360);
        }
        IupSetAttribute(cnv, "DRAWCOLOR", "0 0 0");
        IupSetAttribute(cnv, "DRAWSTYLE", "STROKE");
        IupDrawArc(cnv, c->x-r, c->y-r, c->x+r, c->y+r, 0, 360);
    }
    IupDrawEnd(cnv);
    return IUP_DEFAULT;
}

static int on_motion (Ihandle* h, int x, int y, char* status) {
    Circle* c = find(x, y);
    if (c != over) {
        over = c;
        IupUpdate(cnv);
    }
    return IUP_DEFAULT;
}

static int on_button (Ihandle* h, int but, int press, int x, int y, char* status) {
    if (!press) {
        return IUP_DEFAULT;
    }
    Circle* c = find(x, y);
    if (but==IUP_BUTTON1 && c==NULL && cs_n<MAX) {
        // the circles that were undone are overwritten
        Circle* new = &cs[cs_n++];
        new->x = x;
        new->y = y;
        new->d = 40;
        over = new;
        record((Entry) { 0, new, 0, 0 });
        IupUpdate(cnv);
    }
    if (but==IUP_BUTTON3 && c!=NULL) {
        adj_c = c;                  // for the item of the menu
        IupPopup(menu, IUP_MOUSEPOS, IUP_MOUSEPOS);
    }
    return IUP_DEFAULT;
}

// The item of the menu: opens the dialog that adjusts the diameter.
static int on_adjust (Ihandle* h) {
    adj_old = adj_c->d;
    IupSetStrf(lbl, "TITLE", "Adjust diameter of circle at (%d, %d).", adj_c->x, adj_c->y);
    IupSetInt(sld, "VALUE", adj_c->d);
    IupSetAttribute(dlg, "ACTIVE", "NO");       // the dialog holds the main one
    IupShow(adj);
    return IUP_DEFAULT;
}

static int on_adjust_value (Ihandle* h) {
    adj_c->d = (int) IupGetDouble(sld, "VALUE");
    IupUpdate(cnv);
    return IUP_DEFAULT;
}

static int on_adjust_close (Ihandle* h) {
    IupSetAttribute(dlg, "ACTIVE", "YES");
    if (adj_c->d != adj_old) {
        record((Entry) { 1, adj_c, adj_old, adj_c->d });
    }
    adj_c = NULL;
    return IUP_DEFAULT;
}

static int on_undo (Ihandle* h) {
    done--;
    apply(&hist[done], 0);
    buttons();
    IupUpdate(cnv);
    return IUP_DEFAULT;
}

static int on_redo (Ihandle* h) {
    apply(&hist[done], 1);
    done++;
    buttons();
    IupUpdate(cnv);
    return IUP_DEFAULT;
}

int main (int argc, char** argv) {
    IupOpen(&argc, &argv);
    undo = IupButton("Undo", NULL);
    redo = IupButton("Redo", NULL);
    IupSetAttribute(undo, "SIZE", "50");
    IupSetAttribute(redo, "SIZE", "50");
    cnv = IupCanvas(NULL);
    IupSetAttributes(cnv, "RASTERSIZE=400x250, BORDER=YES");
    Ihandle* top = IupHbox(IupFill(), undo, redo, IupFill(), NULL);
    IupSetAttribute(top, "NGAP", "10");
    Ihandle* box = IupVbox(top, cnv, NULL);
    IupSetAttribute(box, "NGAP", "10");
    dlg = IupDialog(box);
    IupSetAttributes(dlg, "TITLE=\"Circle Drawer\", MARGIN=10x10");

    Ihandle* item = IupItem("Adjust diameter..", NULL);
    menu = IupMenu(item, NULL);

    sld = IupVal(NULL);
    IupSetAttributes(sld, "MIN=10, MAX=200, SIZE=150x15");
    lbl = IupLabel("");
    IupSetAttribute(lbl, "SIZE", "150");
    Ihandle* abox = IupVbox(lbl, sld, NULL);
    IupSetAttribute(abox, "NGAP", "5");
    adj = IupDialog(abox);
    IupSetAttributes(adj, "TITLE=Adjust, MARGIN=10x10");

    IupSetCallback(cnv,  "ACTION",          (Icallback) on_draw);
    IupSetCallback(cnv,  "MOTION_CB",       (Icallback) on_motion);
    IupSetCallback(cnv,  "BUTTON_CB",       (Icallback) on_button);
    IupSetCallback(undo, "ACTION",          on_undo);
    IupSetCallback(redo, "ACTION",          on_redo);
    IupSetCallback(item, "ACTION",          on_adjust);
    IupSetCallback(sld,  "VALUECHANGED_CB", on_adjust_value);
    IupSetCallback(adj,  "CLOSE_CB",        on_adjust_close);

    IupShow(dlg);
    buttons();
    IupSetHandle("cnv", cnv);
    IupSetHandle("undo", undo);
    IupSetHandle("redo", redo);
    IupSetHandle("item", item);
    IupSetHandle("adj", adj);
    IupSetHandle("sld", sld);
    IupSetHandle("dlg", dlg);
    study_script(NULL);
    IupMainLoop();
    IupDestroy(menu);
    IupDestroy(adj);
    IupClose();
    return 0;
}
