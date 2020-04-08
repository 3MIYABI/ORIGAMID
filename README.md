# CryptoSQLite
Generic SQLite3 encryption codec C++ interface for encrypting database pages
plus SQLite3 header.

## Features
* transparent usage
* interface: no hard-coded list of ciphers, use your crypto lib of choice
* full page encryption
* SQLite3 file header encryption
* key file support: long encryption key is stored separately, can be protected
with a password)

## Setup
1. Initialize Git submodules: `git submodule update --init --recursive`
2. Link against the `crypto_sqlite` CMake library target.
3. Implement `IDataCrypt` interface (for an example see
[test/TestCrypt.h](test/TestCrypt.h) or
[test/PlaintextCrypt.h](test/PlaintextCrypt.h))

## Usage
Either:

1. `int sqlite3_open_encrypted(const char *fileName, sqlite3 **db, const void
*key, int keySize)`
Or:

1. `voi