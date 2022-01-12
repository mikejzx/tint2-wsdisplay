/*
 * main.c
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "config.h"

// Clamping
#if WORKSPACE_COUNT > 16
#  undef WORKSPACE_COUNT
#  define WORKSPACE_COUNT 16
#  warning "Workspace count clamped to maximum 16"
#elif WORKSPACE_COUNT < 1
#  undef WORKSPACE_COUNT
#  define WORKSPACE_COUNT 1
#  warning "Workspace count clamped to minimum of 1"
#endif

struct workspaces_info
{
    // Bitfield of which workspaces have windows
    uint16_t w;
    
    // Index of active workspace
    uint16_t active;
} workspaces, workspaces_prev;

static inline bool
workspaces_info_equal(struct workspaces_info a, struct workspaces_info b)
{
    return a.w == b.w && a.active == b.active;
}

Display *d;
int screen;
Window root;
Atom atom_current_desktop, atom_client_list, atom_wm_desktop;

static void do_client_list(void);
static void do_workspace(void);
static void print_output(void);

int 
main (int argc, char *argv[])
{
    // Zero everything
    workspaces.w = 
    workspaces.active = 
    workspaces_prev.w = 0;
    workspaces_prev.active = -1; // So we run at startup too

    d = XOpenDisplay(0);
    screen = XDefaultScreen(d);
    if (!d)
    {
        fprintf(stderr, "XOpenDisplay failed\n");
        return -1;
    }
    root = XRootWindow(d, screen);

    atom_current_desktop = XInternAtom(d, "_NET_CURRENT_DESKTOP", 1);
    atom_client_list = XInternAtom(d, "_NET_CLIENT_LIST_STACKING", 1);
    atom_wm_desktop = XInternAtom(d, "_NET_WM_DESKTOP", 1);

    // Set initial state
    do_workspace();
    do_client_list();
    print_output();

    // Based off xprop code
    XEvent e;
    XSelectInput(d, root, PropertyChangeMask | StructureNotifyMask);

    while(true)
    {
        fflush(stdout);
        XNextEvent(d, &e);
        if (e.type == DestroyNotify)
        {
            break;
        }
        if (e.type != PropertyNotify)
        {
            continue;
        }

        // Current desktop changed
        if (e.xproperty.atom == atom_current_desktop)
        {
            // Get current desktop of root window
            do_workspace();
            goto do_output;
        }

        // Client list updated
        if (e.xproperty.atom == atom_client_list)
        {
            do_client_list();
            goto do_output;
        }

        // Output to the executor
        continue;
    do_output:
        print_output();
    }

    XCloseDisplay(d);

    return 0;
}

static void 
do_client_list(void)
{
    // Dummies
    static Atom da;
    static int di;
    static unsigned long dl;

    // Zero the workspace list before we refresh it
    workspaces.w = 0;

    // Get client list
    unsigned char *data = NULL;
    long n_wins = 0;
    if (XGetWindowProperty(d, root, 
        atom_client_list, 0L, 1024L,
        False, XA_WINDOW, &da, &di, &n_wins, &dl, &data) != Success || 
        !data) 
    {
        return;
    }

    // Iterate over windows
    for (int w = 0; w < (int)n_wins; ++w)
    {
        // Get _NET_WM_DESKTOP of each window
        unsigned char *data2 = NULL;
        if (XGetWindowProperty(d, ((Window *)data)[w], 
            atom_wm_desktop, 0L, sizeof(long),
            False, XA_CARDINAL, &da, &di, &dl, &dl, &data2) 
                != Success || !data2) 
        {
            continue;
        }

        // The desktop this window is on
        int ws = (int)*((long *)data2);

        // Invalid workspace
        if (ws < 0 || ws >= WORKSPACE_COUNT) 
        {
            goto window_next;
        }

        // Set bitfield flag on this workspace
        workspaces.w |= 1 << ws;

    window_next:
        if (data2) XFree(data2);
    }

    if (data) XFree(data);
}

static void 
do_workspace(void)
{
    // Dummies
    static Atom da;
    static int di;
    static unsigned long dl;

    // Get current workspace
    unsigned char *data = NULL;
    if (XGetWindowProperty(d, root, 
        atom_current_desktop, 0L, sizeof(long),
        False, XA_CARDINAL, &da, &di, &dl, &dl, &data) != Success || 
        !data) 
    {
        return;
    }

    // Update current workspace index
    workspaces.active = (uint16_t)*((long *)data);

    if (data) XFree(data);
}

static void
print_output(void)
{
    // Don't output if there was no change from old state
    if (workspaces_info_equal(workspaces, workspaces_prev))
    {
        return;
    }

    // Update old state
    workspaces_prev = workspaces;

#if 0
    // Debug output
    unsigned i;
    printf("out: %d  [ ", workspaces.active + 1);
    for (i = 0; i < workspaces.active; ++i)
    {
        printf("%d ", (workspaces.w & (1 << i)) != 0);
    }
    printf("X ");
    ++i;
    for (; i < WORKSPACE_COUNT; ++i)
    {
        printf("%d ", (workspaces.w & (1 << i)) != 0);
    }
    printf(" ]\n");
#else
    /*
     * Print dots to represent workspace state
     */
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            int i = x + y * 3;
            bool has_win = workspaces.w & (1 << i);
            const char *colour = 
                i == workspaces.active 
                ? COLOUR_ACTIVE
                : (has_win ? COLOUR_HAS_WINDOW : COLOUR_GREY);
            printf(SPACE "<span foreground='%s'>" CIRCLE "</span>", colour);
        }
        printf("\n");
    }
#endif

    fflush(stdout);
}
