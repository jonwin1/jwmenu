#define _POSIX_C_SOURCE 200809L

#include "config.h"
#include "../lib/tomlc17.h"
#include "menu.h"
#include "util.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct menu_item *parse_menu_array(toml_datum_t arr, size_t *out_count);

struct config *
load_config(const char *path)
{
    struct config *conf = safe_calloc(1, sizeof(*conf));

    toml_result_t result = toml_parse_file_ex(path);

    if (!result.ok) {
        free(conf);
        error(result.errmsg, 0);
    }

    toml_datum_t root_menu = toml_seek(result.toptab, "config.menu");
    if (root_menu.type != TOML_ARRAY) {
        free(conf);
        toml_free(result);
        error("missing or invalid 'config.menu' property in config", 0);
    }

    conf->menu_items = parse_menu_array(root_menu, &conf->item_count);

    toml_free(result);
    return conf;
}

void
free_config(struct config *conf)
{
    for (size_t i = 0; i < conf->item_count; i++) {
        free_menu_item(conf->menu_items[i]);
    }
    free(conf->menu_items);
    free(conf);
}

struct menu_item *
parse_menu_table(toml_datum_t tbl)
{
    struct menu_item *item = calloc(1, sizeof(*item));
    toml_datum_t label = toml_get(tbl, "label");
    if (label.type == TOML_STRING) { // TODO: Should be error otherwise?
        item->label = strdup(label.u.s);
    }

    toml_datum_t cmd = toml_get(tbl, "command");
    toml_datum_t submenu = toml_get(tbl, "submenu");

    if (cmd.type == TOML_STRING) {
        item->command = strdup(cmd.u.s);
        item->submenu = NULL;
        item->submenu_count = 0;
    } else if (submenu.type == TOML_ARRAY) {
        item->submenu = parse_menu_array(submenu, &item->submenu_count);
        item->command = NULL;
    } else { // TODO: Error?
        item->command = NULL;
        item->submenu = NULL;
        item->submenu_count = 0;
    }

    return item;
}

struct menu_item *
parse_menu_array(toml_datum_t arr, size_t *out_count)
{
    size_t n = arr.u.arr.size;
    struct menu_item *items = calloc(n, sizeof(*items));
    for (size_t i = 0; i < n; i++) {
        toml_datum_t elem = arr.u.arr.elem[i];
        if (elem.type != TOML_TABLE) {
            continue;
        }
        struct menu_item *mi = parse_menu_table(elem);
        items[i] = *mi;
        free(mi); // free temporary pointer
    }
    *out_count = n;
    return items;
}
