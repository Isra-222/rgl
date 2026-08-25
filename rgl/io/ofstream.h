/*
 * Regular Library (rgl)
 * Copyright (C) 2026 [Isra-222]
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
*/

//ofstream
#pragma once
#include "ostream.h"
#include "rgl/string/string_view.h"

namespace rgl {
    class ofstream : public ostream {
        bool file_is_open;
    public:
        ofstream() : ostream(-1), file_is_open(false) {}
        explicit ofstream(rgl::string_view path);
        explicit ofstream(const char* path);
        ~ofstream() override;

        void open(const char* path);
        void open(rgl::string_view path);
        void close();
        bool is_open() const;

        ofstream(const ofstream&) = delete;
        ofstream& operator=(const ofstream&) = delete;
    };
}