/*
 * Copyright (c) 2014, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Pipe.h>

Pipe::Pipe(uint32_t size, bool blocking, char *buffer) {
    _blockingWrite = blocking;
    _size = size;
    _data = buffer;
    _head = 0;
    _tail = 0;
    _onWrite = NULL;
}

int Pipe::peek() {
    if (_head == _tail) {
        return -1;
    }
    char chr = _data[_tail];
    return chr;
}

int Pipe::read() {
    if (_head == _tail) {
        return -1;
    }
    char chr = _data[_tail];
    _tail = (_tail + 1) % _size;
    return chr;
}

size_t Pipe::write(uint8_t d) {
    uint32_t newhead = (_head + 1) % _size;
    if (_blockingWrite) {
        while (newhead == _tail);
    }
    if (newhead != _tail) {
        _data[_head] = d;
        _head = newhead;
        if (_onWrite != NULL) _onWrite();
        return 1;
    }
    return 0;
}

int Pipe::available() {
    return (_size + _head - _tail) % _size;
}

void Pipe::flush() {
    _head = _tail = 0;
}

void Pipe::onWrite(void (*func)()) {
    _onWrite = func;
}
