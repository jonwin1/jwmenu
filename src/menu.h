#ifndef MENU_H
#define MENU_H

#include <stddef.h>

/*
 * A menu item, contains a label to show in the menu and either a command to
 * execute or a submenu to open. If the menu item has a command, submenu must be
 * null and vice versa. submenu_count is the number of menu items in the
 * submenu.
 */
struct menu_item {
    char *label;
    char *command;             // NULL if submenu
    struct menu_item *submenu; // NULL if command
    size_t submenu_count;
};

int run_menu(struct menu_item *items, size_t count);

#endif // !MENU_H
