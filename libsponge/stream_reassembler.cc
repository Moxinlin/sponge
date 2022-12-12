#include "stream_reassembler.hh"
#include <iostream>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity), 
                                                              _re_asm_idx(0), _end_idx(capacity), _eof(false),
                                                              _buffer({}), _buf_size(0),
                                                              _bitmap({}) {
    _buffer.resize(_capacity);
    _bitmap.resize(_capacity, false);
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) { 
    size_t d_begin = 0, d_end = data.size();
    if (_re_asm_idx > index) {
        d_begin = _re_asm_idx - index;
    }
    if (_re_asm_idx + (_capacity - _output.buffer_size()) - index < data.size()) {
        d_end = _re_asm_idx + (_capacity - _output.buffer_size()) - index;
    }

    for (size_t i = d_begin; i < d_end; ++i) {
        if (_bitmap[(i + index) % _capacity] == false) {
            _buffer[(i + index) % _capacity] = data[i];
            _bitmap[(i + index) % _capacity] = true;
            _buf_size++;
        }
    }

    while (_bitmap[_re_asm_idx % _capacity]) {
        std::string s(&_buffer[_re_asm_idx % _capacity], 1);
        _output.write(s);
        _bitmap[_re_asm_idx % _capacity] = false;
        _re_asm_idx++;
        _buf_size--;
    }
    
    if (eof) {
       _eof = true;
       _end_idx = index + data.size();
    }
    if (_eof && _re_asm_idx == _end_idx) {
        _output.end_input();
    }
   
}

size_t StreamReassembler::unassembled_bytes() const { return _buf_size; }

bool StreamReassembler::empty() const { return _buf_size == 0; }
