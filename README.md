# CryptoSQLite
Generic SQLite3 encryption codec C++ interface for encrypting database pages
plus SQLite3 header.

## Features
* transparent usage
* interface: no hard-coded list of ciphers, use your crypto lib of choice
* full page encryption
* SQLite3 file header encryption
* key file support: long encryption key is stored separately, can be pro