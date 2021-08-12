
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

#ifndef CRYPTOSQLITE_KEYFILE_H
#define CRYPTOSQLITE_KEYFILE_H

#include <crypto_sqlite/crypto_sqlite.h>
#include <cstdio>

class FileWrapper {
public:
    explicit FileWrapper(const std::string &filename) {
        // try to open existing file
        mFile = fopen(filename.c_str(), "r+b");

        if (nullptr == mFile) {
            // mark file as empty
            mEmpty = true;

            // try to create non existing file
            mFile = fopen(filename.c_str(), "w+b");

            // could not create file
            if (nullptr == mFile)
                throw crypto_sqlite_exception("File could not be created");
        }
    }

    ~FileWrapper() {
        if (nullptr != mFile)
            fclose(mFile);
    }
