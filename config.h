#ifndef CONFIG_H
#define CONFIG_H

/*
 * config.h
 *
 * User configuration options.  Obviously you need to recompile for these to take effect.
 */

// The number of rows you want the workspaces circles to be spread across.
// If NUM_ROWS does not divide your number of workspaces evenly, the
// final row will have fewer circles.
#define NUM_ROWS 3

// If your WM/DE does not expose or otherwise correctly report the
// '_NET_NUMBER_OF_DESKTOPS' atom, you may opt to manually specify the number of
// workspaces you have.
#define STATIC_WORKSPACE_COUNT 0

// Set to however many workspaces you have on your desktop (designed to work
// with 9 or 10, any other amounts will require modification to the display
// printing code if you want it to look pretty). Setting this value has no
// effect if STATIC_WORKSPACE_COUNT is set to 0.
#define WORKSPACE_COUNT 10

// Colours
#define COLOUR_ACTIVE "#00ddff" 
#define COLOUR_HAS_WINDOW "#00aa00"
#define COLOUR_GREY "#444444"

// Workspace dot display character
#define CIRCLE "&#x23FA;"

// Amount of space between dots
#define SPACE "    "

#endif
