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
//map
#pragma once

#include "new.h"
#include "utility.h"
#include "hash.h"
#include "string.h"

namespace rgl {

inline size_t next_power_of_two(size_t n) {
    n--;
    n |= n >> 1; n |= n >> 2; n |= n >> 4;
    n |= n >> 8; n |= n >> 16; n |= n >> 32;
    return ++n;
}

template <typename T>
struct default_hasher {
    inline uint64_t operator()(const T& key, uint64_t seed) const {
        if constexpr (is_same_v<T, rgl::string>) {
            return wyhash(reinterpret_cast<const uint8_t*>(key.c_str()), key.size(), seed);
        } else if constexpr (is_same_v<T, string_view>) {
            return wyhash(reinterpret_cast<const uint8_t*>(key.data()), key.size(), seed);
        } else {
            return wyhash(reinterpret_cast<const uint8_t*>(&key), sizeof(T), seed);
        }
    }
};

template<typename K, typename V, typename Hasher = default_hasher<K>>
class map {
private:
    enum State : uint8_t { EMPTY = 0, OCCUPIED = 1, DELETED = 2 };

    struct Entry {
        K key;
        V value;
        State state = EMPTY;
    };

    Entry* table = nullptr;
    size_t table_size = 0;
    size_t element_count = 0;
    uint64_t seed = 0x12345678AB;
    Hasher hash_func;

    static constexpr float MAX_LOAD_FACTOR = 0.7f;

    void rehash(size_t new_size) {
        Entry* old_table = table;
        size_t old_size = table_size;

        table_size = new_size;
        table = new Entry[table_size];
        element_count = 0;

        for (size_t i = 0; i < old_size; ++i) {
            if (old_table[i].state == OCCUPIED) { 
                insert(rgl::move(old_table[i].key), rgl::move(old_table[i].value));
            }
        }
        delete[] old_table;
    }

public:
    class iterator {
        Entry* current;
        Entry* end_ptr;
    public:
        iterator(Entry* ptr, Entry* end) : current(ptr), end_ptr(end) {
            while (current < end_ptr && current->state != OCCUPIED) {
                current++;
            }
        }

        iterator& operator++() {
            do {
                current++;
            } while (current < end_ptr && current->state != OCCUPIED);
            return *this;
        }

        Entry& operator*() const { return *current; }
        Entry* operator->() const { return current; }

        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    map(size_t cap = 16) {
        table_size = next_power_of_two(cap);
        table = new Entry[table_size];
    }

    map(const map& other) : table_size(other.table_size), element_count(other.element_count), 
                            seed(other.seed), hash_func(other.hash_func) {
        table = new Entry[table_size];
        for (size_t i = 0; i < table_size; ++i) {
            if (other.table[i].state == OCCUPIED) {
                table[i].key = other.table[i].key;
                table[i].value = other.table[i].value;
                table[i].state = OCCUPIED;
            }
        }
    }

    map(map&& other) noexcept 
        : table(nullptr), table_size(0), element_count(0), seed(0), hash_func() {
        swap(other);
    }

    map& operator=(const map& other) {
        if (this != &other) {
            map temp(other);
            swap(temp);
        }
        return *this;
    }

    map& operator=(map&& other) noexcept {
        if (this != &other) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    ~map() { 
        delete[] table; 
    }

    void swap(map& other) noexcept {
        rgl::swap(table, other.table);
        rgl::swap(table_size, other.table_size);
        rgl::swap(element_count, other.element_count);
        rgl::swap(seed, other.seed);
        rgl::swap(hash_func, other.hash_func);
    }

    void clear() {
        delete[] table;
        table = nullptr;
        table_size = 0;
        element_count = 0;
    }

    void insert(K key, V value) {
        emplace(rgl::move(key), rgl::move(value));
    }

    V* get(const K& key) {
        if (table_size == 0) return nullptr;
        uint64_t h = hash_func(key, seed);
        size_t mask = table_size - 1;
        size_t idx = h & mask;

        while (table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].key == key) 
                return &table[idx].value;
            idx = (idx + 1) & mask;
        }
        return nullptr;
    }

    bool erase(const K& key) {
        if (table_size == 0) return false;
        
        uint64_t h = hash_func(key, seed);
        size_t mask = table_size - 1;
        size_t idx = h & mask;

        while (table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].key == key) {
                table[idx].state = DELETED;
                element_count--;
                return true;
            }
            idx = (idx + 1) & mask;
        }
        return false;
    }

    bool contains(const K& key) const {
        if (table_size == 0) return false;
        
        uint64_t h = hash_func(key, seed);
        size_t mask = table_size - 1;
        size_t idx = h & mask;

        while (table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].key == key) 
                return true;
            idx = (idx + 1) & mask;
        }
        return false;
    }

    template<typename... Args>
    void emplace(K key, Args&&... args) {
        if ((float)(element_count + 1) / table_size > MAX_LOAD_FACTOR) {
            rehash(table_size * 2);
        }

        uint64_t h = hash_func(key, seed);
        size_t mask = table_size - 1;
        size_t idx = h & mask;

        while (table[idx].state == OCCUPIED) {
            if (table[idx].key == key) {
                table[idx].value = V(rgl::forward<Args>(args)...);
                return;
            }
            idx = (idx + 1) & mask;
        }

        size_t first_deleted = table_size;
        size_t start_idx = idx;
        
        while (table[idx].state != EMPTY) {
            if (table[idx].state == DELETED && first_deleted == table_size) {
                first_deleted = idx;
            }
            idx = (idx + 1) & mask;
            if (idx == start_idx) break;
        }
        
        if (first_deleted != table_size) {
            idx = first_deleted;
        }
        
        table[idx].key = rgl::move(key);
        table[idx].value = V(rgl::forward<Args>(args)...);
        table[idx].state = OCCUPIED;
        element_count++;
    }

    size_t size() const { return element_count; }
    
    bool empty() const { return element_count == 0; }
    
    size_t capacity() const { return table_size; }

    template<typename T = V, typename = enable_if_t<is_default_constructible_v<T>>>
    V& operator[](const K& key) {
        V* val = get(key);
        if (val) return *val;
        insert(key, V());
        return *get(key);
    }

    iterator begin() { return iterator(table, table + table_size); }
    iterator end() { return iterator(table + table_size, table + table_size); }
    
    class const_iterator {
        const Entry* current;
        const Entry* end_ptr;
    public:
        const_iterator(const Entry* ptr, const Entry* end) : current(ptr), end_ptr(end) {
            while (current < end_ptr && current->state != OCCUPIED) {
                current++;
            }
        }
        
        const_iterator& operator++() {
            do {
                current++;
            } while (current < end_ptr && current->state != OCCUPIED);
            return *this;
        }
        
        const Entry& operator*() const { return *current; }
        const Entry* operator->() const { return current; }
        
        bool operator!=(const const_iterator& other) const { return current != other.current; }
    };
    
    const_iterator begin() const { return const_iterator(table, table + table_size); }
    const_iterator end() const { return const_iterator(table + table_size, table + table_size); }
};

} // namespace rgl