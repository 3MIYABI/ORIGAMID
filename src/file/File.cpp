
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