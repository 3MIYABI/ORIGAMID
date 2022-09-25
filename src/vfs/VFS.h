
/*
 * Copyright (C) 2020 The ViaDuck Project
 *
 * This file is part of CryptoSQLite.
 *
 * CryptoSQLite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CryptoSQLite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CryptoSQLite.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CRYPTOSQLITE_VFS_H
#define CRYPTOSQLITE_VFS_H

#include "../file/File.h"
#include "../csqlite/SQLite3Mutex.h"

class VFS {
public:
    static VFS *instance() {
        return &sInstance;
    }

    /**
     * Call before opening main db to prepare reading the encrypted file header
     * Sets this VFS as default. Only one db can be prepared at a time
     *
     * @param zKey Optional key pointer
     * @param nKey Optional key size
     */
    void prepare(const void *zKey, int nKey);

    /**
     * Automatically called on opening any file (db, journal, wal, ...)
     *
     * @param zName Optional file name
     * @param pFile Pointer to preallocated file structure(s)
     * @param flags Opening flags
     * @param pOutFlags Return flags
     * @return Standard sqlite error code
     */
    int open(const char* zName, sqlite3_file* pFile, int flags, int* pOutFlags);

    /**