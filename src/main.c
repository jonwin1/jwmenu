/* SPDX-FileCopyrightText: 2025-2026 Jonatan Wincent <jonatan.wincent@proton.me>
 * SPDX-License-Identifier: MIT
 */
#include "config.h"
#include "menu.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Main entry point of the program.
 *
 * Usage: `jwmenu 'path/to/config.toml'`
 *
 * @param argc  Argument count.
 * @param argv  Argument values.
 * @return      Exit code (0 on success, exits with failure on errors).
 */
int
main(const int argc, const char **argv)
{
    if (argc != 2) {
        printf("ERROR: Invalid arguments, usage:\n");
        printf("\tjwmenu 'path/to/config.toml'\n");
        exit(EXIT_FAILURE);
    }

    const char *path = argv[1];

    if (access(path, R_OK) != 0) {
        error("Cannot access config file: ", path);
    }

    struct config *conf = load_config(path);

    run_menu(conf->menu_items, conf->item_count);

    free_config(conf);

    return 0;
}
