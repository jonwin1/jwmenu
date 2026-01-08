/* SPDX-FileCopyrightText: 2025-2026 Jonatan Wincent <jonatan.wincent@proton.me>
 * SPDX-License-Identifier: MIT
 */
#ifndef CONFIG_H
#define CONFIG_H

#include "menu.h"

/**
 * @struct config
 * @brief Holds application configuration.
 *
 * Contains the root menu items loaded from a configuration file
 * and the number of items in the array.
 */
struct config {
    struct menu_item
        *menu_items;   /**< Array of menu items loaded from config. */
    size_t item_count; /**< Number of items in the menu_items array. */
};

/**
 * @brief Load configuration from a TOML file.
 *
 * Parses the configuration file at `path` and returns a pointer
 * to a `struct config`. On error, the function prints an error
 * message and terminates the program.
 *
 * @param path  Path to the configuration TOML file.
 * @return      Pointer to a dynamically allocated `struct config`.
 */
struct config *load_config(const char *path);

/**
 * @brief Free a configuration object.
 *
 * Releases all memory allocated for a `struct config`, including
 * menu items and submenus.
 *
 * @param conf  Pointer to the configuration to free.
 */
void free_config(struct config *conf);

#endif /* CONFIG_H */
