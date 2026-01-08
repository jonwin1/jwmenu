/* SPDX-FileCopyrightText: 2025-2026 Jonatan Wincent <jonatan.wincent@proton.me>
 * SPDX-License-Identifier: MIT
 */
#ifndef MENU_H
#define MENU_H

#include <stddef.h>

/**
 * @struct menu_item
 * @brief A menu item, contains either a command to execute or a sub menu to
 * open.
 */
struct menu_item {
    char *label;               /**< Label shown in the UI. */
    char *command;             /**< Command to execute when selected.
                                    NULL if this item opens a sub menu. */
    struct menu_item *submenu; /**< Sub menu to open when selected.
                                   NULL if this item executes a command. */
    size_t submenu_count;      /**< Number of items in the sub menu. */
};

/**
 * @brief Run a menu and handle user interaction.
 *
 * Displays the provided menu items and allows the user to select
 * commands or navigate into sub menus.
 *
 * @param items     Pointer to an array of menu items.
 * @param count     Number of items in the array.
 *
 * @return
 * - 0 if the menu should return to its parent menu.
 * - 1 if all menus should be closed.
 */
int run_menu(struct menu_item *items, size_t count);

/**
 * @brief Free resources associated with a menu item.
 *
 * This function releases any dynamically allocated memory owned
 * by the menu item, including recursively freeing sub menus.
 *
 * @param item      Menu item to free.
 */
void free_menu_item(struct menu_item item);

#endif /* !MENU_H */
