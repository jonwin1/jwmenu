/* SPDX-FileCopyrightText: 2025-2026 Jonatan Wincent <jonatan.wincent@proton.me>
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include <stdlib.h>

void
error(const char *msg, const char *msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
    exit(EXIT_FAILURE);
}

void *
safe_calloc(size_t n, size_t size)
{
    void *ptr = calloc(n, size);
    if (!ptr) {
        perror("safe_calloc");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
