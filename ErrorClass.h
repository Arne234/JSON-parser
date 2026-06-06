#pragma once

#include <stdexcept>

class ParseError : public std::runtime_error {
    private:
        size_t position;
    
    public:
        ParseError(const std::string& message, size_t pos) : 
            std::runtime_error(message), position(pos) {}


        size_t getPos() {
            return position;
        }
};