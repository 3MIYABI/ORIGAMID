
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

cmake_minimum_required(VERSION 3.1)
project(crypto_sqlite)

# set cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/secure_memory/cmake-modules")

# add sqlite amalgamation subdirectory and tests
add_subdirectory(sqlite)
add_subdirectory(test)

if (NOT TARGET secure_memory)
    add_subdirectory(external/secure_memory)
endif()

if (NOT TARGET icu)
    add_subdirectory(external/icu-cmake)
endif()

# glob files
file(GLOB_RECURSE CS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
file(GLOB_RECURSE CS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

# set up library target
add_library(crypto_sqlite STATIC ${CS_INCLUDES} ${CS_SOURCES})

# require and enable c++14 support
set_target_properties(crypto_sqlite PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)

# compile options
target_compile_options(crypto_sqlite PRIVATE
    -Wall
    -Wextra
    -Wno-sign-compare
    -Wno-unused-parameter
    -Wno-implicit-fallthrough
    -Wno-unused-function
    )

# link options
target_link_libraries(crypto_sqlite secure_memory icu)
if (NOT WIN32)
    target_link_libraries(crypto_sqlite dl)
endif()

# add compile definitions to match this specific use case
target_compile_definitions(crypto_sqlite PUBLIC