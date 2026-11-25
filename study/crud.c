// 7GUIs, task 5: CRUD. The version with callbacks, in C.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iup.h>
#include "script.h"

#define MAX 256

typedef struct { char name[64]; char surname[64]; } Person;

static Person db[MAX] = {
    { "Hans", "Emil" }, { "Max", "Mustermann" }, { "Roman", "Tisch" },
};
static int db_n = 3;
static int shown[MAX];      // the indexes in `db` of the items of the list

static Ihandle *flt, *lst, *tnam, *tsur, *bupd, *bdel;

// The index in `db` of the selected item, or -1.
static int selected (void) {
    int i = IupGetInt(lst, "VALUE");
    return (i > 0) ? shown[i-1] : -1;
}

// The buttons that apply.
static void buttons (void) {
    const char* sel = (selected() >= 0) ? "YES" : "NO";
    IupSetAttribute(bupd, "ACTIVE", sel);
    IupSetAttribute(bdel, "ACTIVE", sel);
}

// Fills the list with the names that pass the filter.
static void refresh (void) {
    const char* pre = IupGetAttribute(flt, "VALUE");
    size_t len = (pre == NULL) ? 0 : strlen(pre);
    int n = 0;
    IupSetAttribute(lst, "REMOVEITEM", "ALL");
    for (int i=0; i<db_n; i++) {
        if (len==0 || !strncmp(db[i].surname, pre, len)) {
            IupSetStrf(lst, "APPENDITEM", "%s, %s", db[i].surname, db[i].name);
            shown[n++] = i;
        }
    }
    buttons();
}

// The name in the two fields.
static void fields (Person* p) {
    const char* n = IupGetAttribute(tnam, "VALUE");
    const char* s = IupGetAttribute(tsur, "VALUE");
    snprintf(p->name,    sizeof(p->name),    "%s", (n == NULL) ? "" : n);
    snprintf(p->surname, sizeof(p->surname), "%s", (s == NULL) ? "" : s);
}

static int on_filter (Ihandle* h) {
    refresh();
    return IUP_DEFAULT;
}

static int on_select (Ihandle* h) {
    int i = selected();
    if (i >= 0) {
        IupSetStrAttribute(tnam, "VALUE", db[i].name);
        IupSetStrAttribute(tsur, "VALUE", db[i].surname);
    }
    buttons();
    return IUP_DEFAULT;
}

static int on_create (Ihandle* h) {
    if (db_n < MAX) {
        fields(&db[db_n++]);
        refresh();
    }
    return IUP_DEFAULT;
}

static int on_update (Ihandle* h) {
    int i = selected();
    if (i >= 0) {
        fields(&db[i]);
        refresh();
    }
    return IUP_DEFAULT;
}

static int on_delete (Ihandle* h) {
    int i = selected();
    if (i >= 0) {
        memmove(&db[i], &db[i+1], (db_n-i-1)*sizeof(Person));
        db_n--;
        refresh();
    }
    return IUP_DEFAULT;
}

static Ihandle* field (const char* title, Ihandle* txt) {
    Ihandle* lbl = IupLabel(title);
    IupSetAttribute(lbl, "SIZE", "40");
    IupSetAttribute(txt, "SIZE", "80");
    return IupHbox(lbl, txt, NULL);
}

int main (int argc, char** argv) {
    IupOpen(&argc, &argv);
    flt  = IupText(NULL);
    lst  = IupList(NULL);
    tnam = IupText(NULL);
    tsur = IupText(NULL);
    Ihandle* bcre = IupButton("Create", NULL);
    bupd = IupButton("Update", NULL);
    bdel = IupButton("Delete", NULL);
    IupSetAttribute(flt, "SIZE", "60");
    IupSetAttribute(lst, "SIZE", "120x60");
    IupSetCallback(flt,  "VALUECHANGED_CB", on_filter);
    IupSetCallback(lst,  "VALUECHANGED_CB", on_select);
    IupSetCallback(bcre, "ACTION", on_create);
    IupSetCallback(bupd, "ACTION", on_update);
    IupSetCallback(bdel, "ACTION", on_delete);

    Ihandle* top = IupHbox(IupLabel("Filter prefix:"), flt, NULL);
    Ihandle* fs  = IupVbox(field("Name:", tnam), field("Surname:", tsur), NULL);
    Ihandle* mid = IupHbox(lst, fs, NULL);
    Ihandle* bot = IupHbox(bcre, bupd, bdel, NULL);
    IupSetAttribute(top, "NGAP", "10");
    IupSetAttribute(fs,  "NGAP", "5");
    IupSetAttribute(mid, "NGAP", "10");
    IupSetAttribute(bot, "NGAP", "10");
    Ihandle* box = IupVbox(top, mid, bot, NULL);
    IupSetAttribute(box, "NGAP", "10");
    Ihandle* dlg = IupDialog(box);
    IupSetAttributes(dlg, "TITLE=CRUD, MARGIN=10x10");
    IupShow(dlg);
    refresh();
    IupSetHandle("flt", flt);
    IupSetHandle("lst", lst);
    IupSetHandle("name", tnam);
    IupSetHandle("surname", tsur);
    IupSetHandle("create", bcre);
    IupSetHandle("update", bupd);
    IupSetHandle("delete", bdel);
    IupSetHandle("dlg", dlg);
    study_script(NULL);
    IupMainLoop();
    IupClose();
    return 0;
}
