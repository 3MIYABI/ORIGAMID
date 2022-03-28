
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
    int mOpenFlags;
    Crypto *mCrypto;
    File *mDB;
    int mPageSize;
    int mPageNo;

    static sqlite3_io_methods gSQLiteIOMethods;
};

namespace {
    #define FILE_REAL(x) reinterpret_cast<File *>(x)->mUnderlying
    #define FILE_FORWARD(f, fn, ...) FILE_REAL(f)->pMethods->fn(FILE_REAL(f), ## __VA_ARGS__)
    #define FILE_INTERCEPT(f, fn, x...) reinterpret_cast<File *>(f)->fn(x)

    int sIoClose(sqlite3_file* pFile) {
        return FILE_INTERCEPT(pFile, close);
    }
    int sIoRead(sqlite3_file* pFile, void* buf, int iAmt, sqlite3_int64 iOfst) {
        return FILE_INTERCEPT(pFile, read, buf, iAmt, iOfst);
    }
    int sIoWrite(sqlite3_file* pFile, const void* buf, int iAmt, sqlite3_int64 iOfst) {
        return FILE_INTERCEPT(pFile, write, buf, iAmt, iOfst);
    }
    int sIoTruncate(sqlite3_file* pFile, sqlite3_int64 size) {
        return FILE_FORWARD(pFile, xTruncate, size);
    }
    int sIoSync(sqlite3_file* pFile, int flags) {
        return FILE_FORWARD(pFile, xSync, flags);
    }
    int sIoFileSize(sqlite3_file* pFile, sqlite3_int64* pSize) {
        return FILE_FORWARD(pFile, xFileSize, pSize);
    }
    int sIoLock(sqlite3_file* pFile, int lock) {
        return FILE_FORWARD(pFile, xLock, lock);
    }
    int sIoUnlock(sqlite3_file* pFile, int lock) {