/* SPDX-FileCopyrightText: 2025-2026 Jonatan Wincent <jonatan.wincent@proton.me>
 * SPDX-License-Identifier: MIT
 */
#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

void error(const char *msg, const char *msg1);

void *safe_calloc(size_t n, size_t size);

#endif
