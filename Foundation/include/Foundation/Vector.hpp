#pragma once

#include "Allocator.hpp"
#include "Assert.hpp"

#include <cstdlib>

namespace Foundation {

template<typename T>
class Vector {
public:
    int m_size;
    int m_capacity;
    T *m_data;

    // Provide standard typedefs but we don't use them ourselves.
    typedef T value_type;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;

    // Constructors, destructor
    inline Vector() {
        m_size = m_capacity = 0;
        m_data = NULL;
    }

    inline Vector(const Vector<T> &src) {
        m_size = m_capacity = 0;
        m_data = NULL;
        operator=(src);
    }

    inline Vector<T> &operator=(const Vector<T> &src) {
        clear();
        resize(src.m_size);
        memcpy(m_data, src.m_data, (size_t)m_size * sizeof(T));
        return *this;
    }

    inline ~Vector() {
        clear();
    }

    inline void clear() {
        for (int n = 0; n < m_size; n++) {
            m_data[n].~T();
        }
        if (m_data) {
            m_size = m_capacity = 0;
            FREE(getAllocator(), m_data);
            m_data = NULL;
        }
    }

    inline bool isEmpty() const {
        return m_size == 0;
    }

    inline int size() const {
        return m_size;
    }

    inline int sizeInBytes() const {
        return m_size * (int)sizeof(T);
    }

    inline int capacity() const {
        return m_capacity;
    }

    inline T &operator[](int i) {
        PND_ASSERT(i >= 0 && i < m_size, "index out of bounds");
        return m_data[i];
    }

    inline const T &operator[](int i) const {
        PND_ASSERT(i >= 0 && i < m_size, "index out of bounds");
        return m_data[i];
    }

    inline T *begin() {
        return m_data;
    }

    inline const T *begin() const {
        return m_data;
    }

    inline T *end() {
        return m_data + m_size;
    }

    inline const T *end() const {
        return m_data + m_size;
    }

    inline T &front() {
        PND_ASSERT(m_size > 0, "vector is empty");
        return m_data[0];
    }

    inline const T &front() const {
        PND_ASSERT(m_size > 0, "vector is empty");
        return m_data[0];
    }

    inline T &back() {
        PND_ASSERT(m_size > 0, "vector is empty");
        return m_data[m_size - 1];
    }

    inline const T &back() const {
        PND_ASSERT(m_size > 0, "vector is empty");
        return m_data[m_size - 1];
    }

    inline int growCapacity(int sz) const {
        int new_capacity = m_capacity ? (m_capacity + m_capacity / 2) : 8;
        return new_capacity > sz ? new_capacity : sz;
    }

    inline void resize(int new_size) {
        if (new_size > m_capacity)
            reserve(growCapacity(new_size));
        m_size = new_size;
    }

    inline void reserve(int new_capacity) {
        if (new_capacity <= m_capacity)
            return;
        T *new_data = (T *)ALLOC(getAllocator(), (size_t)new_capacity * sizeof(T));
        if (m_data) {
            memcpy(new_data, m_data, (size_t)m_size * sizeof(T));
            FREE(getAllocator(), m_data);
        }
        m_data = new_data;
        m_capacity = new_capacity;
    }

    inline void reserve_discard(int new_capacity) {
        if (new_capacity <= m_capacity)
            return;
        if (m_data)
            FREE(getAllocator(), m_data);
        m_data = (T *)ALLOC(getAllocator(), (size_t)new_capacity * sizeof(T));
        m_capacity = new_capacity;
    }

    // NB: It is illegal to call push_back/push_front/insert with a reference pointing inside the
    // ImVector data itself! e.g. v.push_back(v[10]) is forbidden.
    inline void push_back(const T &v) {
        if (m_size == m_capacity)
            reserve(growCapacity(m_size + 1));
        memcpy(&m_data[m_size], &v, sizeof(v));
        m_size++;
    }

    inline void pop_back() {
        PND_ASSERT(m_size > 0, "vector is empty");
        m_size--;
    }

    inline void push_front(const T &v) {
        if (m_size == 0)
            push_back(v);
        else
            insert(m_data, v);
    }

    inline T *erase(const T *it) {
        PND_ASSERT(it >= m_data && it < m_data + m_size, "element not found");
        const ptrdiff_t off = it - m_data;
        memmove(m_data + off, m_data + off + 1, ((size_t)m_size - (size_t)off - 1) * sizeof(T));
        m_size--;
        return m_data + off;
    }

    inline T *erase(const T *it, const T *it_last) {
        PND_ASSERT(
            it >= m_data && it < m_data + m_size && it_last >= it && it_last <= m_data + m_size,
            "element not found"
        );
        const ptrdiff_t count = it_last - it;
        const ptrdiff_t off = it - m_data;
        memmove(
            m_data + off,
            m_data + off + count,
            ((size_t)m_size - (size_t)off - (size_t)count) * sizeof(T)
        );
        m_size -= (int)count;
        return m_data + off;
    }

    inline T *erase_unsorted(const T *it) {
        PND_ASSERT(it >= m_data && it < m_data + m_size, "element not found");
        const ptrdiff_t off = it - m_data;
        if (it < m_data + m_size - 1)
            memcpy(m_data + off, m_data + m_size - 1, sizeof(T));
        m_size--;
        return m_data + off;
    }

    inline T *insert(const T *it, const T &v) {
        PND_ASSERT(it >= m_data && it <= m_data + m_size, "element not found");
        const ptrdiff_t off = it - m_data;
        if (m_size == m_capacity)
            reserve(growCapacity(m_size + 1));
        if (off < (int)m_size)
            memmove(m_data + off + 1, m_data + off, ((size_t)m_size - (size_t)off) * sizeof(T));
        memcpy(&m_data[off], &v, sizeof(v));
        m_size++;
        return m_data + off;
    }

    inline bool contains(const T &v) const {
        const T *data = m_data;
        const T *data_end = m_data + m_size;
        while (data < data_end)
            if (*data++ == v)
                return true;
        return false;
    }

    inline T *find(const T &v) {
        T *data = m_data;
        const T *data_end = m_data + m_size;
        while (data < data_end)
            if (*data == v)
                break;
            else
                ++data;
        return data;
    }

    inline const T *find(const T &v) const {
        const T *data = m_data;
        const T *data_end = m_data + m_size;
        while (data < data_end)
            if (*data == v)
                break;
            else
                ++data;
        return data;
    }

    inline bool find_erase(const T &v) {
        const T *it = find(v);
        if (it < m_data + m_size) {
            erase(it);
            return true;
        }
        return false;
    }

    inline bool find_erase_unsorted(const T &v) {
        const T *it = find(v);
        if (it < m_data + m_size) {
            erase_unsorted(it);
            return true;
        }
        return false;
    }

    inline int index_from_ptr(const T *it) const {
        PND_ASSERT(it >= m_data && it < m_data + m_size, "element not found");
        const ptrdiff_t off = it - m_data;
        return (int)off;
    }
};
} // namespace Foundation
