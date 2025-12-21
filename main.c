#define _POSIX_C_SOURCE 200809L

#include "lib/tomlc17.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct menu_item {
    char *label;
    char *command;             // NULL if submenu
    struct menu_item *submenu; // NULL if leaf
    size_t submenu_count;
};

struct menu_item *parse_menu_array(toml_datum_t arr, size_t *out_count);

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

int
show_menu(struct menu_item *items, size_t count, const char *prompt)
{
    int inpipe[2];  // parent -> rofi
    int outpipe[2]; // rofi -> parent

    if (pipe(inpipe) < 0 || pipe(outpipe) < 0) {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Child: rofi
        dup2(inpipe[0], STDIN_FILENO);
        dup2(outpipe[1], STDOUT_FILENO);

        close(inpipe[1]);
        close(outpipe[0]);

        char prompt_arg[128];
        snprintf(prompt_arg, sizeof(prompt_arg), "%s", prompt);

        execlp("rofi", "rofi", "-dmenu", "-format", "i", "-i", "-p", prompt_arg,
               NULL);

        _exit(1);
    }

    // Parent
    close(inpipe[0]);
    close(outpipe[1]);

    // Write menu items
    FILE *to_rofi = fdopen(inpipe[1], "w");
    for (size_t i = 0; i < count; i++) {
        fprintf(to_rofi, "%s\n", items[i].label);
    }
    fclose(to_rofi); // IMPORTANT: signal EOF

    // Read selection
    FILE *from_rofi = fdopen(outpipe[0], "r");
    char buf[16];
    if (!fgets(buf, sizeof(buf), from_rofi)) {
        fclose(from_rofi);
        waitpid(pid, NULL, 0);
        return -1;
    }

    fclose(from_rofi);
    waitpid(pid, NULL, 0);

    int idx = atoi(buf);
    if (idx < 0 || (size_t)idx >= count) {
        return -1;
    }

    return idx;
}

int
run_menu(struct menu_item *items, size_t count)
{
    while (true) {
        int idx = show_menu(items, count, "Menu");
        if (idx < 0) {
            return 0;
        }

        struct menu_item *item = &items[idx];

        if (item->command) {
            (void)system(item->command);
            return 1;
        } else if (item->submenu) {
            int exit = run_menu(item->submenu, item->submenu_count);

            if (exit) {
                return 1;
            }
        }
    }
}

int
main(void)
{
    toml_result_t result = toml_parse_file_ex("config.toml");
    if (!result.ok) {
        fprintf(stderr, "TOML parse error: %s\n", result.errmsg);
        return 1;
    }

    toml_datum_t root_menu = toml_seek(result.toptab, "config.menu");
    if (root_menu.type != TOML_ARRAY) {
        fprintf(stderr, "No config.menu found or invalid\n");
        toml_free(result);
        return 1;
    }

    size_t root_count;
    struct menu_item *root_items = parse_menu_array(root_menu, &root_count);

    run_menu(root_items, root_count);

    toml_free(result);
    return 0;
}
