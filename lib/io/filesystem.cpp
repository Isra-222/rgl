/*
 * Regular Library (rgl)
 * Copyright (C) 2026 [Isra-222]
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 */

// filesystem.cpp
#include "rgl/io/filesystem.h"
#include "rgl/core/utility/move.h"

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

namespace rgl {
    namespace fs {
        
        bool exists(const rgl::string& path) {
            struct stat info;
            return (stat(path.c_str(), &info) == 0);
        }
        
        bool is_regular_file(const rgl::string& path) {
            struct stat info;
            if (stat(path.c_str(), &info) != 0) {
                return false;
            }
            return S_ISREG(info.st_mode);
        }
        
        bool is_directory(const rgl::string& path) {
            struct stat info;
            if (stat(path.c_str(), &info) != 0) {
                return false;
            }
            return S_ISDIR(info.st_mode);
        }
        
        size_t file_size(const rgl::string& path) {
            struct stat info;
            if (stat(path.c_str(), &info) != 0) {
                return 0;
            }
            return static_cast<size_t>(info.st_size);
        }
        

        directory_entry::directory_entry(const rgl::string& full_path) 
            : path_str(full_path) {}
        
        rgl::string directory_entry::path() const {
            return path_str;
        }
        
        void directory_iterator::read_next() {
            if (!dir_stream) return;
            while ((current_entry = readdir(dir_stream)) != nullptr) {
                if (current_entry->d_name[0] == '.' && 
                    (current_entry->d_name[1] == '\0' || 
                     (current_entry->d_name[1] == '.' && current_entry->d_name[2] == '\0'))) {
                    continue;
                }
                rgl::string full_path = base_path;
                if (full_path.length() > 0 && full_path[full_path.length() - 1] != '/') {
                    full_path += "/";
                }
                full_path += current_entry->d_name;
                cached_entry = directory_entry(full_path);
                return;
            }
            cached_entry = directory_entry("");
        }
        
        directory_iterator::directory_iterator() 
            : dir_stream(nullptr), current_entry(nullptr), cached_entry("") {}
        
        directory_iterator::directory_iterator(const rgl::string& path) 
            : dir_stream(nullptr), current_entry(nullptr), base_path(path), cached_entry("") {
            dir_stream = opendir(path.c_str());
            if (dir_stream) {
                read_next();
            }
        }
        
        directory_iterator::directory_iterator(directory_iterator&& other) noexcept 
            : dir_stream(other.dir_stream), 
              current_entry(other.current_entry), 
              base_path(rgl::move(other.base_path)), 
              cached_entry(rgl::move(other.cached_entry)) {
            other.dir_stream = nullptr;
            other.current_entry = nullptr;
        }
        
        directory_iterator& directory_iterator::operator=(directory_iterator&& other) noexcept {
            if (this != &other) {
                if (dir_stream) closedir(dir_stream);
                dir_stream = other.dir_stream;
                current_entry = other.current_entry;
                base_path = rgl::move(other.base_path);
                cached_entry = rgl::move(other.cached_entry);
                other.dir_stream = nullptr;
                other.current_entry = nullptr;
            }
            return *this;
        }
        
        directory_iterator::~directory_iterator() {
            if (dir_stream) {
                closedir(dir_stream);
            }
        }
        
        const directory_entry& directory_iterator::operator*() const {
            return cached_entry;
        }
        
        const directory_entry* directory_iterator::operator->() const {
            return &cached_entry;
        }
        
        directory_iterator& directory_iterator::operator++() {
            read_next();
            return *this;
        }
        
        bool directory_iterator::operator!=(const directory_iterator& other) const {
            bool this_at_end = (dir_stream == nullptr || current_entry == nullptr);
            bool other_at_end = (other.dir_stream == nullptr || other.current_entry == nullptr);
            return this_at_end != other_at_end;
        }
        
        const directory_iterator& directory_iterator::begin() const {
            return *this;
        }
        
        directory_iterator directory_iterator::end() const {
            return directory_iterator();
        }
        
        rgl::string get_filename(const rgl::string& path) {
            size_t lastSlash = path.find_last_of('/');
            return (lastSlash == rgl::string::npos) ? path : path.substr(lastSlash + 1);
        }
        
    } // namespace fs
} // namespace rgl