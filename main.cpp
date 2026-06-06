#include "Parser.h"

#include <iostream>
#include <string>

int main() {

    std::string input = "hello there";

    JSONParser parser(input);

    JsonValue value = parser.parseValue();

    if (value.isString()) {
        value.asString();
    }


    return 0;
}