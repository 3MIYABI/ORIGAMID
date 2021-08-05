
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
#include "Crypto.h"

#include "FileWrapper.h"
#include <crypto_sqlite/crypto_sqlite.h>

Crypto::Crypto(const std::string &dbFileName, const void *fileKey, int keylen, int exists)
        : mFileName(dbFileName + "-keyfile") {
    crypto_sqlite::makeDataCrypt(mDataCrypt);

    if (!exists) {
        // generate new key and wrap it to buffer
        mDataCrypt->generateKey(mKey);
        wrapKey(fileKey, keylen);
    }
    else {
        // read existing keyfile and unwrap key
        readKeyFile();
        unwrapKey(fileKey, keylen);