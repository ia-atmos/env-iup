// 7GUIs, task 7: Cells. The version with callbacks, in C.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <iup.h>
#include "script.h"

#define COLS 26
#define ROWS 100
#define N    (COLS*ROWS)

typedef enum { EMPTY, NUM, TXT, ERR } Kind;

typedef struct {
    char     src[128];      // what the user typed
    Kind     kind;          // the value
    double   num;
    Ihandle* h;             // the control
    char     deps[N];       // the cells that its formula reads
    int      busy;          // it is being computed (a cycle)
} Cell;

static Cell cells[N];
static int  focus = -1;     // the cell with the focus

// The formula being evaluated.
static const char* src;
static Cell*       cur;
static jmp_buf     fail;

static void skip (void) {
    while (*src == ' ') {
        src++;
    }
}

// The number in a cell: an empty one is 0, and a text is an error.
static double cell (int col, int row) {
    Cell* c = &cells[row*COLS + col];
    cur->deps[row*COLS + col] = 1;
    if (c->kind==TXT || c->kind==ERR) {
        longjmp(fail, 1);
    }
    return (c->kind == NUM) ? c->num : 0;
}

// A cell, as its column and row.
static void ref (int* col, int* row) {
    skip();
    if (!isupper((unsigned char)*src) || !isdigit((unsigned char)src[1])) {
        longjmp(fail, 1);
    }
    *col = *src++ - 'A';
    *row = (int) strtol(src, (char**)&src, 10);
    if (*row > 99) {
        longjmp(fail, 1);
    }
}

// An expression, as numbers appended to a vector. Returns how many.
static int expr (double* ns, int max) {
    skip();
    if (isdigit((unsigned char)*src) || *src=='.' || *src=='-') {
        char* end;
        double n = strtod(src, &end);
        if (end==src || max<1) {
            longjmp(fail, 1);
        }
        src = end;
        ns[0] = n;
        return 1;
    }
    if (isupper((unsigned char)*src)) {
        int c1, r1, c2, r2;
        ref(&c1, &r1);
        skip();
        if (*src != ':') {
            if (max < 1) {
                longjmp(fail, 1);
            }
            ns[0] = cell(c1, r1);
            return 1;
        }
        src++;
        ref(&c2, &r2);
        int n = 0;
        for (int c=c1; c<=c2; c++) {
            for (int r=r1; r<=r2; r++) {
                if (n >= max) {
                    longjmp(fail, 1);
                }
                ns[n++] = cell(c, r);
            }
        }
        return n;
    }
    char name[8];
    int len = 0;
    while (islower((unsigned char)*src) && len<7) {
        name[len++] = *src++;
    }
    name[len] = '\0';
    skip();
    if (*src != '(') {
        longjmp(fail, 1);
    }
    double as[N];
    int n = 0;
    do {
        src++;                      // the `(` or the `,`
        n += expr(as+n, N-n);
        skip();
    } while (*src == ',');
    if (*src!=')' || n<2 || max<1) {
        longjmp(fail, 1);
    }
    src++;
    double r;
    if (!strcmp(name, "sum")) {
        r = 0;
        for (int i=0; i<n; i++) { r += as[i]; }
    } else if (!strcmp(name, "prod")) {
        r = 1;
        for (int i=0; i<n; i++) { r *= as[i]; }
    } else if (!strcmp(name, "add")) {
        r = as[0] + as[1];
    } else if (!strcmp(name, "sub")) {
        r = as[0] - as[1];
    } else if (!strcmp(name, "mul")) {
        r = as[0] * as[1];
    } else if (!strcmp(name, "div")) {
        r = as[0] / as[1];
    } else {
        longjmp(fail, 1);
    }
    ns[0] = r;
    return 1;
}

// Shows the value of a cell, unless the user is editing it.
static void show (int id) {
    Cell* c = &cells[id];
    if (id == focus) {
        return;
    }
    if (c->kind == NUM) {
        IupSetStrf(c->h, "VALUE", "%g", c->num);
    } else if (c->kind == ERR) {
        IupSetAttribute(c->h, "VALUE", "#ERR");
    } else {
        IupSetStrAttribute(c->h, "VALUE", (c->kind == TXT) ? c->src : "");
    }
}

// Computes the value of a cell from what the user typed, and then the
// cells that read it.
static void compute (int id) {
    Cell* c = &cells[id];
    if (c->busy) {
        return;                     // a cycle
    }
    c->busy = 1;
    memset(c->deps, 0, sizeof(c->deps));
    if (c->src[0] == '=') {
        double n;
        src = c->src + 1;
        cur = c;
        if (setjmp(fail) == 0) {
            if (expr(&n, 1) != 1) {
                longjmp(fail, 1);
            }
            skip();
            if (*src != '\0') {
                longjmp(fail, 1);
            }
            c->kind = NUM;
            c->num  = n;
        } else {
            c->kind = ERR;
        }
    } else if (c->src[0] == '\0') {
        c->kind = EMPTY;
    } else {
        char* end;
        c->num  = strtod(c->src, &end);
        c->kind = (*end == '\0') ? NUM : TXT;
    }
    show(id);
    for (int i=0; i<N; i++) {
        if (cells[i].deps[id]) {
            compute(i);
        }
    }
    c->busy = 0;
}

// A double click edits the formula of a cell.
static int on_button (Ihandle* h, int but, int press, int x, int y, char* status) {
    if (but==IUP_BUTTON1 && press && iup_isdouble(status)) {
        int id = IupGetInt(h, "CELL");
        focus = id;
        IupSetAttribute(h, "READONLY", "NO");
        IupSetStrAttribute(h, "VALUE", cells[id].src);
    }
    return IUP_DEFAULT;
}

// The edition ends when the cell loses the focus.
static int on_killfocus (Ihandle* h) {
    int id = IupGetInt(h, "CELL");
    if (id != focus) {
        return IUP_DEFAULT;
    }
    IupSetAttribute(h, "READONLY", "YES");
    const char* v = IupGetAttribute(h, "VALUE");
    focus = -1;
    if (v == NULL) {
        v = "";
    }
    if (strcmp(v, cells[id].src) != 0) {
        snprintf(cells[id].src, sizeof(cells[id].src), "%s", v);
        compute(id);
    }
    show(id);
    return IUP_DEFAULT;
}

int main (int argc, char** argv) {
    IupOpen(&argc, &argv);
    Ihandle* grid = IupGridBox(IupLabel(""), NULL);
    for (int c=0; c<COLS; c++) {
        char s[2] = { 'A'+c, '\0' };
        Ihandle* l = IupLabel(NULL);
        IupSetStrAttribute(l, "TITLE", s);
        IupAppend(grid, l);
    }
    for (int r=0; r<ROWS; r++) {
        Ihandle* l = IupLabel(NULL);
        IupSetInt(l, "TITLE", r);
        IupAppend(grid, l);
        for (int c=0; c<COLS; c++) {
            Ihandle* t = IupText(NULL);
            IupSetAttributes(t, "SIZE=40, READONLY=YES");
            IupSetInt(t, "CELL", r*COLS + c);
            IupSetCallback(t, "BUTTON_CB",    (Icallback) on_button);
            IupSetCallback(t, "KILLFOCUS_CB", on_killfocus);
            char name[8];
            snprintf(name, sizeof(name), "%c%d", 'A'+c, r);
            IupSetHandle(name, t);
            cells[r*COLS + c].h = t;
            IupAppend(grid, t);
        }
    }
    IupSetAttributes(grid, "NUMDIV=27, SIZELIN=1, SIZECOL=1, ALIGNMENTCOL=ACENTER");
    Ihandle* dlg = IupDialog(IupScrollBox(grid));
    IupSetAttributes(dlg, "TITLE=Cells, RASTERSIZE=620x400");
    IupShow(dlg);
    IupSetHandle("dlg", dlg);
    study_script(NULL);
    IupMainLoop();
    IupClose();
    return 0;
}
