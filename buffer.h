#ifndef PROGA_LABA_7_BUFFER_H
#define PROGA_LABA_7_BUFFER_H

#include <iterator>

template <typename T>
class CircularBuffer {
private:
    T* buffer;
    size_t head;
    size_t tail;
    size_t bufferSize;

public:
    class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
    private:
        size_t pos;
        CircularBuffer* buf;

    public:
        Iterator(CircularBuffer* buf, size_t pos) : pos(pos), buf(buf) {}

        T& operator*() {
            return (*buf)[pos];
        }

        Iterator& operator++() {
            pos = (pos + 1) % buf->capacity();
            return *this;
        }

        Iterator& operator--() {
            pos = (buf->capacity() + pos - 1) % buf->capacity();
            return *this;
        }

        Iterator& operator+=(const size_t& n) {
            pos = (pos + n) % buf->capacity();
            return *this;
        }

        Iterator& operator-=(const size_t& n) {
            pos = (buf->capacity() + pos - n) % buf->capacity();
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return pos == other.pos && buf == other.buf;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    CircularBuffer(size_t size) {
        buffer = new T[size];
        bufferSize = size;
        head = 0;
        tail = 0;
    }

    ~CircularBuffer() {
        delete[] buffer;
    }

    void resize(size_t new_size) {
        T* new_buffer = new T[new_size];
        size_t current_size = this->size();
        for(size_t i = 0; i < current_size; ++i) {
            new_buffer[i] = (*this)[i];
        }
        delete[] buffer;
        buffer = new_buffer;
        bufferSize = new_size;
        head = 0;
        tail = current_size % new_size;
    }

    size_t capacity() const {
        return bufferSize;
    }

    size_t size() const {
        return (bufferSize - head + tail) % bufferSize;
    }

    bool empty() const {
        return head == tail;
    }

    bool full() const {
        return (tail + 1) % bufferSize == head;
    }

    Iterator insert(Iterator pos, const T& value) {
        size_t index = std::distance(begin(), pos);
        if (full()) {
            resize(bufferSize * 1.5);
        }
        if (index < size()) {
            size_t i = size();
            push_back(buffer[(head + i - 1) % bufferSize]);
            while (i-- > index) {
                buffer[(head + i) % bufferSize] = buffer[(head + i - 1) % bufferSize];
            }
        }
        buffer[(head + index) % bufferSize] = value;
        return Iterator(this, index);
    }

    Iterator erase(Iterator pos) {
        if (empty()) {
            return end();
        }
        size_t index = std::distance(begin(), pos);
        if (index < size()) {
            size_t i = index;
            while (++i < size()) {
                buffer[(head + i - 1) % bufferSize] = buffer[(head + i) % bufferSize];
            }
            pop_back();
        }
        return Iterator(this, index);
    }

    void push_back(const T& value) {
        buffer[tail] = value;
        tail = (tail + 1) % bufferSize;
        if (head == tail)
            head = (head + 1) % bufferSize;
    }

    void pop_back() {
        if(!empty())
            tail = (bufferSize + tail - 1) % bufferSize;
    }

    void push_front(const T& value) {
        head = (bufferSize + head - 1) % bufferSize;
        buffer[head] = value;
        if (head == tail)
            tail = (tail + 1) % bufferSize;
    }

    void pop_front() {
        if(!empty())
            head = (head + 1) % bufferSize;
    }

    T& operator[](size_t index) {
        return buffer[(head + index) % bufferSize];
    }

    const T& operator[](size_t index) const {
        return buffer[(head + index) % bufferSize];
    }

    Iterator begin() {
        return Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, size());
    }
};

#endif

