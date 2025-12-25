#define _POSIX_C_SOURCE 200809L

#include "menu.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static int show_menu(struct menu_item *items, size_t count);

static int
show_menu(struct menu_item *items, size_t count)
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

        execlp("rofi", "rofi", "-dmenu", "-format", "i", "-i", NULL);

        exit(EXIT_SUCCESS);
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
        int selected = show_menu(items, count);
        if (selected < 0) {
            return 0;
        }

        struct menu_item *item = &items[selected];

        if (item->command) {
            system(item->command);
            return 1;
        } else if (item->submenu) {
            int exit = run_menu(item->submenu, item->submenu_count);

            if (exit) {
                return 1;
            }
        }
    }
}

void
free_menu_item(struct menu_item item)
{
    free(item.label);

    if (item.command != NULL) {
        free(item.command);
        return;
    }

    for (size_t i = 0; i < item.submenu_count; i++) {
        free_menu_item(item.submenu[i]);
    }
    free(item.submenu);
}
