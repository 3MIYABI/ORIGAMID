# Copyright (c) 2017-2023 The ViaDuck Project
#
# This file is part of CryptoSQLite.
#
# CryptoSQLite is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CryptoSQLite is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with CryptoSQLite.  If not, see <http://www.gnu.org/licenses/>.
#

include(ApplyPatches)

# set up cache variables
set(SQLITE_BUILD_VERSION "3410100" CACHE STRING "SQLite version string as used in the URL")
set(SQLITE_BUILD_HASH "df0d54bf246521360c8148f64e7e5ad07a4665b4f902339e844f4c493d535ff5" CACHE STRING
        "SQLite amalgamation zip sha256 hash used to check the download")

# build url from cache variables
set(SQLITE_URL "https://mirror.viaduck.org/sqlite/sqlite-amalgamation-${SQLITE_BUILD_VERSION}.zip")
set(SQLITE_SUBDI