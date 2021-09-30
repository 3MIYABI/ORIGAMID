/*
 * Copyright (C) 2017-2020 The ViaDuck Project
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

#include <crypto_sqlite/crypto_sqlite.h>
#include "vfs/VFS.h"

crypto_sqlite::CryptoFactory crypto_sqlite::sFactoryCrypt;

void sqlite3_prepare_open_encrypted(const void *zKey, int nKey) {
    VFS::instance()->prepare(zKey, nKey);
}

int sqlite3_open_encrypted(const char *zFilename, sqlite3 **ppDb, const void *zKey, int nKey) {
    // no key specified
    if (zKey == nullptr || nKey <= 0)
        return sqlite3_open(zFilename, ppDb);

    // prepare the call to open
    sqlite3_prepare_open_encrypted(zKey, nKey);

    int rc = sqlite3_open(zFilename, ppDb);
    if (rc == SQLITE_OK)
        rc = sqlite3_key(*ppDb, nullptr, 0);

    return rc;
}

int sqlite3_rekey_encrypted(const char *zFilename, const void *zKeyOld,