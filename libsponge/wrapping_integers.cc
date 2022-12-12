#include "wrapping_integers.hh"
#include <iostream>

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    WrappingInt32 seqno = isn + n;
    return seqno;
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t offset = static_cast<uint64_t>(n - isn);
    uint64_t INT32_RANGE = (1UL << 32);
    uint64_t wrap_num = checkpoint / INT32_RANGE;
    uint64_t max_wrap_num = (1UL << 32) - 1;

    uint64_t left_seqno = (wrap_num > 1 ? wrap_num-1: wrap_num) * INT32_RANGE + offset;
    uint64_t mid_seqno = wrap_num * INT32_RANGE + offset;
    uint64_t right_seqno =  (wrap_num + 1 > max_wrap_num ? max_wrap_num : wrap_num + 1) * INT32_RANGE + offset;
    
    uint64_t left_diff = (checkpoint > left_seqno ? checkpoint - left_seqno : left_seqno - checkpoint);
    uint64_t mid_diff = (checkpoint > mid_seqno ? checkpoint - mid_seqno : mid_seqno - checkpoint);
    uint64_t right_diff = (checkpoint > right_seqno ? checkpoint - right_seqno : right_seqno - checkpoint);

    uint64_t min_diff = std::min(std::min(left_diff, mid_diff), right_diff);
    if (min_diff == left_diff) {
        return left_seqno;
    }
    if (min_diff == mid_diff) {
        return mid_seqno;
    }
    return right_seqno;
}
