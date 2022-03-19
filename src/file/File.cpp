
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

#include <cstring>
#include <cassert>
#include "../vfs/VFS.h"
#include "../csqlite/csqlite.h"
#include "File.h"

sqlite3_io_methods File::gSQLiteIOMethods = {
        3,                          /* iVersion */
        sIoClose,                  /* xClose */
        sIoRead,                   /* xRead */
        sIoWrite,                  /* xWrite */
        sIoTruncate,               /* xTruncate */
        sIoSync,                   /* xSync */
        sIoFileSize,               /* xFileSize */
        sIoLock,                   /* xLock */
        sIoUnlock,                 /* xUnlock */
        sIoCheckReservedLock,      /* xCheckReservedLock */
        sIoFileControl,            /* xFileControl */
        sIoSectorSize,             /* xSectorSize */
        sIoDeviceCharacteristics,  /* xDeviceCharacteristics */
        sIoShmMap,                 /* xShmMap */
        sIoShmLock,                /* xShmLock */
        sIoShmBarrier,             /* xShmBarrier */
        sIoShmUnmap,               /* xShmUnmap */
        sIoFetch,                  /* xFetch */
        sIoUnfetch,                /* xUnfetch */
};

int File::attach(sqlite3 *db, int nDb) {
    // lock while modifying page size
    SQLite3Mutex mutex(csqlite3_get_mutex(db));
    SQLite3LockGuard lock(mutex);

    // TODO: add support for attached dbs

    // Set page size to its default, but add our size to be reserved at the end of the page
    csqlite3_reserve_page(db, nDb, &mPageSize, mCrypto->extraSize());
    mCrypto->resizePageBuffers(mPageSize);
    return SQLITE_OK;
}

int File::close() {
    // clean from list
    if (mOpenFlags & SQLITE_OPEN_MAIN_DB)
        VFS::instance()->removeDatabase(this);

    // cleanup state
    if (!mDB) delete mCrypto;
    mCrypto = nullptr;

    // forward actual close
    return mUnderlying->pMethods->xClose(mUnderlying);
}

int File::read(void *buffer, int count, sqlite3_int64 offset) {
    // forward actual read
    auto rv = FILE_FORWARD(this, xRead, buffer, count, offset);
    if (rv != SQLITE_OK)
        return rv;

    if (mCrypto) {
        switch (mOpenFlags & SQLITE_OPEN_MASK) {
            case SQLITE_OPEN_MAIN_DB:
                return readMainDB(buffer, count, offset);

            case SQLITE_OPEN_MAIN_JOURNAL:
            case SQLITE_OPEN_SUBJOURNAL:
                return readJournal(buffer, count, offset);

            case SQLITE_OPEN_WAL:
                return readWal(buffer, count, offset);

            case SQLITE_OPEN_TEMP_DB:
            case SQLITE_OPEN_TRANSIENT_DB:
            case SQLITE_OPEN_TEMP_JOURNAL:
                // TODO ?
                break;

            case SQLITE_OPEN_MASTER_JOURNAL:
                /** Contains only administrative information, no encryption necessary. **/
            default:
                break;