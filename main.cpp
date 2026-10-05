#include "Parser.h"
#include "Serializer.h"
#include "ErrorClass.h"

#include <iostream>
#include <string>

int main() {
    std::string input = R"({
        "name": "Max",
        "age": 17,
        "active": true,
        "grades": [1.0, 2.0, 1.5],
        "address": null
    })";

    try {
        JSONParser parser(input);
        JsonValue value = parser.parse();

        std::cout << "Parsed successfully:\n";
        std::cout << Serializer::serialize(value) << '\n';
    }
    catch (const ParseError& e) {
        std::cerr << "Parse error at position "
                  << e.getPos()
                  << ": "
                  << e.what()
                  << '\n';

        return 1;
    }

    return 0;
}