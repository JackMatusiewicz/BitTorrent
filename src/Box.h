#ifndef BITTORRENT_STARTER_CPP_BOX_H
#define BITTORRENT_STARTER_CPP_BOX_H

#include <memory>

template<typename T>
class Box {
private:
    std::unique_ptr<T> _data;
public:
    explicit Box(T&& v) : _data{new T(std::move(v))} {}
    explicit Box(const T& v) : _data{new T(v)} {}

    Box(const Box<T>& rhs) : Box(*rhs._data) {}

    Box& operator=(const Box<T>& rhs) {
        *_data = *rhs._data;
        return *this;
    }

    ~Box() = default;

    T &operator*() { return *_data; }
    const T &operator*() const { return *_data; }

    T *operator->() { return _data.get(); }
    const T *operator->() const { return _data.get(); }

};

#endif //BITTORRENT_STARTER_CPP_BOX_H
