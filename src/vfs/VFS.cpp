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

#include <algorithm>
#include "VFS.h"

VFS VFS::sInstance;

VFS::VFS() : mBase(), mDBs(new std::vector<File *>()) {
    // find default VFS
    mUnderlying = sqlite3_vfs_find(nullptr);

    // set base parameters based on underlying VFS
    mBase = {
            3,                      /* iVersion */
            static_cast<int>(sizeof(File) + mUnderlying->szOsFile),                      /* szOsFile */
            mUnderlying->mxPathname,                   /* mxPathname */
            nullptr,                /* pNext */
            "CryptoSQLite",         /* zName */
            nullptr,                /* pAppData */
            sVfsOpen,               /* xOpen */
            sVfsDelete,             /* xDelete */
            sVfsAccess,             /* xAccess */
            sVfsFullPathname,       /* xFullPathname */
            sVfsDlOpen,             /* xDlOpen */
            sVfsDlError,            /* xDlError */
            sVfsDlSym,              /* xDlSym */
            sVfsDlClose,            /* xDlClose */
            sVfsRandomness,         /* xRandomness */
            sVfsSleep,              /* xSleep */
            sVfsCurrentTime,        /* xCurrentTime */
            sVfsGetLastError,       /* xGetLastError */
            sVfsCurrentTimeInt64,   /* xCurrentTimeInt64 */
            sVfsSetSystemCall,      /* xSetSystemCall */
            sVfsGetSystemCall,      /* xGetSystemCall */
            sVfsNextSystemCall      /* xNextSystemCall */
    };
}

VFS::~VFS() {
    delete mDBs;
}

void VFS::prepare(const void *zKey, int nKey) {
    // make custom VFS default before opening
    sqlite3_vfs_register(base(), 1);
    // cache key in instance for open()
    mFileKey = zKey;
    mFileKeySize = nKey;
}

int VFS::open(const char *zName, sqlite3_file *pFile, int flags, int *pOutFlags) {
    