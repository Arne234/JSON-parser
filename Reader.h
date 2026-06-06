#pragma once

#include <iostream>
#include <string>



class Reader {
    private:
        std::string text;
        size_t pos = 0;

    public:
        Reader(const std::string& t) : text(t) {}


        char peek();
        char advance();
        bool eof(); 
        size_t getPos();
};