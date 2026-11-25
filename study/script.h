// The test mode of the C versions of the study: the same script of
// actions as `iup.atm` (`IUP_ATM_SCRIPT`), so that both versions of a
// task run under the same script and print the same trace.
//
//      study_script(tick)  after the dialogs are shown: if the variable
//                          is set, a timer of 1ms of real time runs the
//                          actions due, then calls `tick` (may be NULL)
//                          as 10ms of virtual time; returns 1 if so
//      study_now()         the time in seconds: virtual under a script
//
// The actions: click, dclick, rclick, move, set, get, key, act, close,
// shot, quit (see `iup.atm`). The clicks go through XTest.

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <iup.h>

static const char* STUDY_SCRIPT = NULL;
static int         STUDY_MS     = 0;
static Icallback   STUDY_TICK   = NULL;
static int         STUDY_WAIT   = 0;    // ticks with no time: events in course

static double study_now (void) {
    if (STUDY_SCRIPT != NULL) {
        return STUDY_MS / 1000.0;
    }
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec/1e9;
}

static void study_settle (int ms) {
    for (int i=0; i<ms; i+=5) {
        IupFlush();
        usleep(5000);
    }
    IupFlush();
}

static int study_xtest (int x, int y, int but, int press) {
    typedef int (*FMotion) (void*, int, int, int, unsigned long);
    typedef int (*FButton) (void*, unsigned int, int, unsigned long);
    typedef int (*FFlush)  (void*);
    static int ok = -1;
    static FMotion motion;
    static FButton button;
    static FFlush  flush;
    if (ok == -1) {
        void* xt  = dlopen("libXtst.so.6", RTLD_LAZY);
        void* x11 = dlopen("libX11.so.6", RTLD_LAZY);
        motion = xt  ? (FMotion) dlsym(xt, "XTestFakeMotionEvent") : NULL;
        button = xt  ? (FButton) dlsym(xt, "XTestFakeButtonEvent") : NULL;
        flush  = x11 ? (FFlush)  dlsym(x11, "XFlush")              : NULL;
        ok = (motion!=NULL && button!=NULL && flush!=NULL && IupGetGlobal("XDISPLAY")!=NULL);
    }
    if (!ok) {
        return 0;
    }
    // the events are only sent, and processed by the loop of IUP after
    // the timer returns: a callback that opens a popup (which only
    // returns when it closes) must not run inside the timer, which would
    // not fire again meanwhile
    void* dpy = (void*) IupGetGlobal("XDISPLAY");
    motion(dpy, -1, x, y, 0);
    if (press != -1) {
        button(dpy, (unsigned) but, press != 0, 0);
    }
    flush(dpy);
    STUDY_WAIT = 50;            // the time stops until IUP has the events
    return 1;
}

static void study_mouse (Ihandle* h, const char* pos, int but, int press) {
    int x, y, w, hh, dx, dy;
    IupGetIntInt(h, "SCREENPOSITION", &x, &y);
    IupGetIntInt(h, "RASTERSIZE", &w, &hh);
    if (pos!=NULL && sscanf(pos, "%d,%d", &dx, &dy)==2) {
        x += dx;
        y += dy;
    } else {
        x += w/2;
        y += hh/2;
    }
    if (study_xtest(x, y, but, press)) {
        return;
    }
    IupSetfAttribute(NULL, "CURSORPOS", "%dx%d", x, y);
    study_settle(20);
    IupSetfAttribute(NULL, "MOUSEBUTTON", "%dx%d %d %d", x, y, but, press);
    study_settle(20);
}

static int study_call (Ihandle* h, const char* cb) {
    Icallback f = IupGetCallback(h, cb);
    return (f == NULL) ? IUP_DEFAULT : f(h);
}

static void study_actions (const char* s, int ms) {
    while (s!=NULL && *s!='\0') {
        char item[256];
        size_t n = strcspn(s, ";");
        snprintf(item, sizeof(item), "%.*s", (int)n, s);
        s += n + (s[n] == ';');
        char* act = strchr(item, ':');
        if (act==NULL || atoi(item)!=ms) {
            continue;
        }
        act++;
        char* arg = strchr(act, ':');
        if (arg != NULL) {
            *arg++ = '\0';
        }
        if (!strcmp(act, "quit")) {
            IupExitLoop();
        } else if (!strcmp(act, "shot") && arg!=NULL) {
            // the screenshots are those of the Atmos version: not taken
        } else if (arg != NULL) {
            char* val = strchr(arg, ':');
            if (val != NULL) {
                *val++ = '\0';
            }
            Ihandle* h = IupGetHandle(arg);
            if (h == NULL) {
                fprintf(stderr, "study : no control named %s\n", arg);
            } else if (!strcmp(act, "click")) {
                study_mouse(h, val, 1, 1);
                study_mouse(h, val, 1, 0);
            } else if (!strcmp(act, "dclick")) {
                study_mouse(h, val, 1, 1);
                study_mouse(h, val, 1, 0);
                study_mouse(h, val, 1, 2);
                study_mouse(h, val, 1, 0);
            } else if (!strcmp(act, "rclick")) {
                study_mouse(h, val, 3, 1);
                study_mouse(h, val, 3, 0);
            } else if (!strcmp(act, "move")) {
                study_mouse(h, val, 1, -1);
            } else if (!strcmp(act, "set") && val!=NULL) {
                char* eq = strchr(val, '=');
                if (eq != NULL) {
                    *eq++ = '\0';
                    IupSetStrAttribute(h, val, eq);
                    study_call(h, "VALUECHANGED_CB");
                }
            } else if (!strcmp(act, "get") && val!=NULL) {
                const char* v = IupGetAttribute(h, val);
                printf("%s.%s = %s\n", arg, val, (v==NULL) ? "nil" : v);
                fflush(stdout);
            } else if (!strcmp(act, "close")) {
                if (study_call(h, "CLOSE_CB") != IUP_IGNORE) {
                    IupHide(h);
                }
            }
        }
    }
}

static int study_timer (Ihandle* t) {
    (void) t;
    if (STUDY_WAIT > 0) {
        STUDY_WAIT--;
        return IUP_DEFAULT;
    }
    study_actions(STUDY_SCRIPT, STUDY_MS);
    STUDY_MS += 10;
    if (STUDY_TICK != NULL) {
        STUDY_TICK(NULL);
    }
    return IUP_DEFAULT;
}

static int study_script (Icallback tick) {
    STUDY_SCRIPT = getenv("IUP_ATM_SCRIPT");
    if (STUDY_SCRIPT == NULL) {
        return 0;
    }
    STUDY_TICK = tick;
    study_settle(200);
    Ihandle* t = IupTimer();
    IupSetAttribute(t, "TIME", "1");
    IupSetCallback(t, "ACTION_CB", study_timer);
    IupSetAttribute(t, "RUN", "YES");
    return 1;
}
