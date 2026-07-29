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

#include "rgl/memory/new.h"
#include "rgl/core/hash.h"
#include "rgl/core/utility/pair.h"
#include "rgl/core/error.h"

namespace rgl {

inline size_t next_power_of_two(size_t n) {
    n--;
    n |= n >> 1; n |= n >> 2; n |= n >> 4;
    n |= n >> 8; n |= n >> 16; n |= n >> 32;
    return ++n;
}


template<typename K, typename V, typename Hasher = hasher<K>>
class map final{
public:
    using value_type = rgl::pair<const K, V>;
private:
    enum State : uint8_t { EMPTY = 0, OCCUPIED = 1, DELETED = 2 };


    struct Entry {
        value_type data;
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
        table = (Entry*)::operator new[](table_size * sizeof(Entry));

        for (size_t i = 0; i < table_size; ++i) {
            table[i].state = EMPTY;
        }

        element_count = 0;

        for (size_t i = 0; i < old_size; ++i) {
            if (old_table[i].state == OCCUPIED) { 
                insert(rgl::move(old_table[i].data.first), rgl::move(old_table[i].data.second));
                old_table[i].data.~value_type();
            }
        }

        ::operator delete[](old_table);
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

        value_type* operator->() const { return &current->data; }
        value_type& operator*() const { return current->data; }

        bool operator!=(const iterator& other) const { return current != other.current; }
        bool operator==(const iterator& other) const { return current == other.current; }
    };

    map(size_t cap = 16) {
        if(cap == 0) cap = 16;
        table_size = next_power_of_two(cap);
        table = (Entry*)::operator new[](table_size * sizeof(Entry));
        
        for (size_t i = 0; i < table_size; ++i) {
            table[i].state = EMPTY;
        }
    }

    map(const map& other) : table_size(other.table_size), element_count(other.element_count), 
                            seed(other.seed), hash_func(other.hash_func) {
        table = (Entry*)::operator new[](table_size * sizeof(Entry));

        for (size_t i = 0; i < table_size; ++i) {
            if (other.table[i].state == OCCUPIED) {
                new (&table[i].data) value_type(other.table[i].data);
                table[i].state = OCCUPIED;
            } else {
                table[i].state = EMPTY;
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
        for(size_t i = 0; i < table_size; ++i) {
            if (table[i].state == OCCUPIED) {
                table[i].data.~value_type();
            }
        }
        ::operator delete[](table);
    }

    void swap(map& other) noexcept {
        rgl::swap(table, other.table);
        rgl::swap(table_size, other.table_size);
        rgl::swap(element_count, other.element_count);
        rgl::swap(seed, other.seed);
        rgl::swap(hash_func, other.hash_func);
    }

    void clear() {
        for(size_t i = 0; i < table_size; ++i) {
            if (table[i].state == OCCUPIED) {
                table[i].data.~value_type();
            }
            table[i].state = EMPTY;
        }
        element_count = 0;
    }

    rgl::pair<iterator, bool> insert(K key, V value) {
        return emplace(rgl::move(key), rgl::move(value));
    }

    rgl::pair<iterator, bool> insert(const value_type& val) {
        return emplace(val.first, val.second);
    }
    size_t count(const K& key) const {
        return contains(key) ? 1 : 0;
    }

    V* get(const K& key) {
        if (table_size == 0 || table == nullptr) return nullptr;
        uint64_t h = hash_func(key, seed);
        size_t mask = table_size - 1;
        size_t idx = h & mask;

        while (table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].data.first == key) 
                return &table[idx].data.second;
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
            if (table[idx].state == OCCUPIED && table[idx].data.first == key) {
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
            if (table[idx].state == OCCUPIED && table[idx].data.first == key) 
                return true;
            idx = (idx + 1) & mask;
        }
        return false;
    }

    template<typename... Args>
    pair<iterator, bool> emplace(K key, Args&&... args) {
        if ((float)(element_count + 1) / table_size > MAX_LOAD_FACTOR) {
            rehash(table_size * 2);
        }

        uint64_t h = hash_func(key, seed);
        size_t mask = table_size - 1;
        size_t idx = h & mask;
        size_t first_deleted = table_size;
        size_t start_idx = idx;

        while (table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED) {
                if (table[idx].data.first == key) {
                    table[idx].data.second.~V();
                    new (&table[idx].data.second) V(rgl::forward<Args>(args)...);
                    return {iterator(&table[idx], table + table_size), false};
                }
            } else if (table[idx].state == DELETED && first_deleted == table_size) {
                first_deleted = idx;
            }
            
            idx = (idx + 1) & mask;
            if (idx == start_idx) break;
        }

        if (first_deleted != table_size) {
            idx = first_deleted;
        }

        new (&table[idx].data) value_type(rgl::move(key), V(rgl::forward<Args>(args)...));
        table[idx].state = OCCUPIED;
        element_count++;

        return {iterator(&table[idx], table + table_size), true};
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

    V& at(const K& key){
        V* val = get(key);
        if(!val)
            panic("rgl::map::at: key not found");
        return *val;
    }
    const V& at(const K& key) const {
        const_iterator it = find(key);
        if (it == end()) 
            panic("rgl::map::at: key not found");
        return it->second;
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

        const value_type& operator*() const { return current->data; }

        const value_type* operator->() const { return &current->data; }
        
        bool operator!=(const const_iterator& other) const { return current != other.current; }
        bool operator==(const const_iterator& other) const { return current == other.current; }
    };
    
    const_iterator find(const K& key) const {
        if (table_size == 0) return end();
        
        uint64_t h = hash_func(key, seed);
        size_t mask = table_size - 1;
        size_t idx = h & mask;
        size_t start_idx = idx;

        while (table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].data.first == key) {
                return const_iterator(&table[idx], table + table_size);
            }
            idx = (idx + 1) & mask;
            if (idx == start_idx) break;
        }
        return end();
    }
    iterator find(const K& key) {
        if (table_size == 0) return end();
        
        uint64_t h = hash_func(key, seed);
        size_t mask = table_size - 1;
        size_t idx = h & mask;
        size_t start_idx = idx;

        while (table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].data.first == key) {
                return iterator(&table[idx], table + table_size);
            }
            idx = (idx + 1) & mask;
            if (idx == start_idx) break;
        }
        return end();
    }

    const_iterator begin() const { return const_iterator(table, table + table_size); }
    const_iterator end() const { return const_iterator(table + table_size, table + table_size); }

    using key_type = K;
    using mapped_type = V;
    using size_type = size_t;
    using reference = V&;
    using const_reference = const V&;
};

} // namespace rgl