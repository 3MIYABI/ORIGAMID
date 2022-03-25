
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

#ifndef CRYPTOSQLITE_FILE_H
#define CRYPTOSQLITE_FILE_H

#include <type_traits>
#include <vector>
#include <string>
#include "../crypto/Crypto.h"

extern "C" {
#include <sqlite3.h>
};

const int SQLITE_OPEN_MASK = SQLITE_OPEN_MAIN_DB |
                             SQLITE_OPEN_TEMP_DB |
                             SQLITE_OPEN_TRANSIENT_DB |
                             SQLITE_OPEN_MAIN_JOURNAL |
                             SQLITE_OPEN_TEMP_JOURNAL |
                             SQLITE_OPEN_SUBJOURNAL |
                             SQLITE_OPEN_MASTER_JOURNAL |
                             SQLITE_OPEN_WAL;

const int SQLITE_WAL_FRAMEHEADER_SIZE = 24;

class File {
public:
    /* Constructor/destructor can not be used because sqlite3 manages this memory */

    int attach(sqlite3 *db, int nDB);

    int close();
    int read(void* buffer, int count, sqlite3_int64 offset);
    int write(const void* buffer, int count, sqlite3_int64 offset);

protected:
    int readMainDB(void *buffer, int count, sqlite3_int64 offset);
    int readJournal(void *buffer, int count, sqlite3_int64 offset);
    int readWal(void *buffer, int count, sqlite3_int64 offset);

    int writeMainDB(const void *buffer, int count, sqlite3_int64 offset);
    int writeJournal(const void *buffer, int count, sqlite3_int64 offset);
    int writeWal(const void *buffer, int count, sqlite3_int64 offset);

public:
    sqlite3_file mBase;
    /**/
    sqlite3_file *mUnderlying;
    const char *mFileName;
    int mExists;