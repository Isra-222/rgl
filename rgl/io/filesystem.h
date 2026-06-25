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

// filesystem.h
#pragma once

#include "rgl/string/string.h"
#include <dirent.h>

namespace rgl {
    namespace fs {
        
        bool exists(const rgl::string& path);
        bool is_regular_file(const rgl::string& path);
        bool is_directory(const rgl::string& path);
        size_t file_size(const rgl::string& path);
        
        struct directory_entry {
            rgl::string path_str;
            
            directory_entry(const rgl::string& full_path);
            rgl::string path() const;
        };
        
        class directory_iterator final {
            DIR* dir_stream;
            struct dirent* current_entry;
            rgl::string base_path;
            directory_entry cached_entry;
            
            void read_next();
            
        public:
            directory_iterator();
            explicit directory_iterator(const rgl::string& path);
            
            directory_iterator(const directory_iterator&) = delete;
            directory_iterator& operator=(const directory_iterator&) = delete;
            
            directory_iterator(directory_iterator&& other) noexcept;
            directory_iterator& operator=(directory_iterator&& other) noexcept;
            
            ~directory_iterator();
            
            const directory_entry& operator*() const;
            const directory_entry* operator->() const;
            
            directory_iterator& operator++();
            bool operator!=(const directory_iterator& other) const;
            
            const directory_iterator& begin() const;
            directory_iterator end() const;
        };
        
        rgl::string get_filename(const rgl::string& path);
        
    } // namespace fs
} // namespace rgl