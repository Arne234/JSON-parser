#include "Parser.h"
#include "serializer.h"

#include <iostream>
#include <string>

int main() {

    std::string input = "\"Hello World\"";
    std::string test = "[]";


    JSONParser parser(test);

    JsonValue v = parser.parse();

    if (v.isString()) {
        std::cout << v.asString();
    }

    if (v.isArray()) {
        for (const auto& elem : v.asArray()) {
            std::cout << elem.asString(); 
    }
    }


    return 0;
}