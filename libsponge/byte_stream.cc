#include "byte_stream.hh"
#include <iostream>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {
    _buf_cap = capacity > 0 ? capacity : 1;
    _buffer.resize(_buf_cap);
    _read_idx = 0;
    _buf_size = 0;
    _read_size = 0;
    _write_size = 0;
}

size_t ByteStream::write(const string &data) {
    size_t i = 0;
    while (_buf_size < _buf_cap && i < data.size()) {
        _buffer[(_read_idx + _buf_size) % _buf_cap] = data[i];
        _buf_size++;
        i++;
        _write_size++;
    }
    
    return i;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    std::string data = "";
    size_t t = std::min(len, _buf_size);
    for (size_t i = 0; i < t; ++i) {
        data.append(&_buffer[(_read_idx + i) % _buf_cap], 1);
    }
    return data;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t t = std::min(len, _buf_size);
    _read_idx += t;
    _buf_size -= t;
    _read_size += t;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    std::string data;
    size_t read_len = 0;
    while (_buf_size > 0 && read_len < len) {
        data.append(&_buffer[_read_idx], 1);
        _read_idx = (_read_idx + 1) % _buf_cap;
        _buf_size--;
        _read_size++;
        read_len++;
    }
    return data;
}

void ByteStream::end_input() { _input_end = true;}

bool ByteStream::input_ended() const { return _input_end; }

size_t ByteStream::buffer_size() const { return _buf_size; }

bool ByteStream::buffer_empty() const { 
    return _buf_size == 0; 
}

bool ByteStream::eof() const { return _input_end && _buf_size == 0; }

size_t ByteStream::bytes_written() const { return _write_size; }

size_t ByteStream::bytes_read() const { return _read_size; }

size_t ByteStream::remaining_capacity() const { return _buf_cap - _buf_size; }
