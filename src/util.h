/* SPDX-FileCopyrightText: 2025-2026 Jonatan Wincent <jonatan.wincent@proton.me>
 * SPDX-License-Identifier: MIT
 */
#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

/**
 * @brief Print an error message and exit the program.
 *
 * Writes the combined message `msg` and optional `msg1` to `stderr` and
 * terminates the program with `EXIT_FAILURE`.
 *
 * @param msg   Primary error message.
 * @param msg1  Optional secondary message, can be NULL.
 */
void error(const char *msg, const char *msg1);

/**
 * @brief Allocate zero-initialized memory safely.
 *
 * Wrapper around `calloc` that exits the program if allocation fails.
 *
 * @param n     Number of elements to allocate.
 * @param size  Size of each element in bytes.
 * @return      Pointer to allocated and zero-initialized memory.
 */
void *safe_calloc(size_t n, size_t size);

#endif /* UTIL_H */
