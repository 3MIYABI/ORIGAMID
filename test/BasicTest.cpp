
/*
 * Copyright (C) 2017-2018 The ViaDuck Project
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

#include "BasicTest.h"

#include <secure_memory/String.h>
#include <crypto_sqlite/crypto_sqlite.h>
#include <crypto_sqlite/crypto/PlaintextCrypt.h>

#define ASSERT_OK(x) ASSERT_EQ(SQLITE_OK, (x))
#define ASSERT_DONE(x) ASSERT_EQ(SQLITE_DONE, (x))

TEST_F(BasicTest, testNoPW) {
    testReadWrite(nullptr, 0);
}

TEST_F(BasicTest, testNoPWTransact) {
    testReadWrite(nullptr, 0, true);
}

TEST_F(BasicTest, testPlaintext) {
    crypto_sqlite::setCryptoFactory([] (std::unique_ptr<IDataCrypt> &crypt) {
        crypt.reset(new PlaintextCrypt());
    });

    const char *key = "1234";
    int keylen = 4;

    testReadWrite(key, keylen);
}

TEST_F(BasicTest, testPlaintextTransact) {
    crypto_sqlite::setCryptoFactory([] (std::unique_ptr<IDataCrypt> &crypt) {
        crypt.reset(new PlaintextCrypt());
    });

    const char *key = "1234";
    int keylen = 4;

    testReadWrite(key, keylen, true);
}

TEST_F(BasicTest, testPlaintextTransactRekey) {
    crypto_sqlite::setCryptoFactory([] (std::unique_ptr<IDataCrypt> &crypt) {
        crypt.reset(new PlaintextCrypt());
    });

    const char *key = "1234", *newkey = "32818";
    int keylen = 4, newlen = 5;

    testWrite(key, keylen, true);
    testRekey(key, keylen, newkey, newlen);
    testRead(newkey, newlen);
}

TEST_F(BasicTest, testTestCrypt) {
    crypto_sqlite::setCryptoFactory([] (std::unique_ptr<IDataCrypt> &crypt) {
        crypt.reset(new TestCrypt());
    });

    const char *key = "42424242";
    int keylen = strlen(key);

    testReadWrite(key, keylen);
}

TEST_F(BasicTest, testTestCryptTransact) {
    crypto_sqlite::setCryptoFactory([] (std::unique_ptr<IDataCrypt> &crypt) {
        crypt.reset(new TestCrypt());
    });

    const char *key = "42424242";
    int keylen = strlen(key);