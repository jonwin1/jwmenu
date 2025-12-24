#ifndef CONFIG_H
#define CONFIG_H

#include "menu.h"

struct config {
    struct menu_item *menu_items;
    size_t item_count;
};

struct config *load_config(const char *path);

void free_config(struct config *conf);

#endif
