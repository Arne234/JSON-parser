#include "Reader.h"
#include "ErrorClass.h"

#include <iostream>
#include <string>



char Reader::peek() {

    return text[pos];
}


char Reader::advance() {

    if (pos >= text.size()) throw std::out_of_range("advance past end");
    return text[pos++];
}


bool Reader::eof() {
    return pos >= text.size();
}

size_t Reader::getPos() {
    return pos;
}