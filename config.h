#ifndef CONFIG_H
#define CONFIG_H

/*
 * config.h
 *
 * User configuration options.  Obviously you need to recompile for these to take effect.
 */

// Set to however many workspaces you have on your desktop (designed to work
// with 9 or 10, any other amounts will require modification to the display
// printing code if you want it to look pretty)
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
