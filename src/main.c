#define _POSIX_C_SOURCE 200809L

#include "config.h"
#include "menu.h"
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
        fprintf(stderr, "get_config_path: XDG_CONFIG_HOME is not set\n");
        return NULL;
    }

    size_t len = strlen(xdg) + 1 + strlen(file) + 1;

    char *path = malloc(len);
    if (!path) {
        fprintf(stderr, "get_config_path: malloc failed to allocate memory\n");
        return NULL;
    }

    snprintf(path, len, "%s/%s", xdg, file);

    if (access(path, R_OK) != 0) {
        fprintf(stderr, "Cannot access config file: %s\n", path);
        free(path);
        return NULL;
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

    struct config conf = load_config(path);
    // free(path);

    run_menu(conf.menu_items, conf.item_count);

    return 0;
}
