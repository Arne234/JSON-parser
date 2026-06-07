#pragma once

#include <iostream>
#include <string>



class Reader {
    private:
        std::string text;
        size_t pos = 0;

    public:
        Reader(const std::string& t) : text(t) {}


        char peek() const;
        char advance();
        bool eof() const; 
        size_t getPos() const;
};