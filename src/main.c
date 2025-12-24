#define _POSIX_C_SOURCE 200809L

#include "config.h"
#include "menu.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *
get_config_path()
{
    const char *file = "jwmenu/config.toml";

    const char *xdg = getenv("XDG_CONFIG_HOME");
    if (!xdg) {
        error("get_config_path: XDG_CONFIG_HOME is not set", 0);
    }

    size_t len = strlen(xdg) + 1 + strlen(file) + 1;

    char *path = safe_calloc(1, len);

    snprintf(path, len, "%s/%s", xdg, file);

    if (access(path, R_OK) != 0) {
        error("Cannot access config file: ", path);
    }

    return path;
}

int
main(void)
{
    char *path = "config.toml";
    // char *path = get_config_path();
    if (!path) {
        return 1;
    }

    struct config *conf = load_config(path);
    // free(path);

    run_menu(conf->menu_items, conf->item_count);

    return 0;
}
