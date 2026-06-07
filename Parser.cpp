#include "Parser.h"
#include "ErrorClass.h"

#include <iostream>
#include <string>


void JSONParser::skipWhiteSpaces() {
    while (!reader.eof() && std::isspace(static_cast<unsigned char>(reader.peek()))) {
        reader.advance();
    }
}

JsonValue JSONParser::parse() {
    JsonValue res = parseValue();

    skipWhiteSpaces();

    if (!reader.eof()) {
        throw ParseError("Unexpected characters after JSON", reader.getPos());
    }

    return res;
}


JsonValue JSONParser::parseValue() {
    
    skipWhiteSpaces();

    if (reader.eof()) {
        throw ParseError("No data at ", reader.getPos());
    }


    char c = reader.peek();
    
    if (c == '"') {
        return parseString(); 
    }

    if (c == '-' || std::isdigit(c)) {
        return parseNum(); 
    }

    if (c == 't' || c == 'f') {
        return parseBool();
    }

    if (c =='n') {
        return parseNull();
    }

    if (c == '[') {
        return parseArray(); 
    }

    if (c == '{') {
        return parseObject(); 
    }

    throw ParseError("No matching datatype at ", reader.getPos());

}



std::string JSONParser::parseString() {
    std::string newString;

    reader.advance();

    while (!reader.eof()) {

        char c = reader.peek();

        if (c == '"') {
            reader.advance();
            return newString;
        }

        if (c == '\\') {
            reader.advance();

            if (reader.eof()) {
                throw ParseError("Invalid escape sequence at ", reader.getPos());
            }

            char n = reader.peek();

            
            switch(n) {
                case '"':
                    newString += '"';
                    break;

                case '\\':
                    newString += '\\';
                    break;

                case 'n':
                    newString += '\n';
                    break;

                case 't':
                    newString += '\t';
                    break;

                case '/':
                    newString += '/';
                    break;

                case 'b':
                    newString += '\b';
                    break;

                case 'r':
                    newString += '\r';
                    break;

                case 'f':
                    newString += '\f';
                    break;

                case 'u': {
                    std::string s;
                    reader.advance();

                    for (int i = 0; i < 4; i++) {
                        if (reader.eof() || !std::isxdigit(static_cast<unsigned char>(reader.peek()))) {
                            throw ParseError("Invalid unicode escape", reader.getPos());
                        }
                        s += reader.advance();
                    }

                    int code = std::stoi(s, nullptr, 16);

                    if (code <= 0x7f) {
                        newString += static_cast<char>(code);
                    }

                    else if (code <= 0x7ff) {
                        newString += static_cast<char>(0xC0 | (code >> 6));
                        newString += static_cast<char>(0x80 | (0x3f & code));
                    }

                    else {
                        newString += static_cast<char>(0xE0 | (code >> 12));
                        newString += static_cast<char>(0x80 | ((code >> 6) & 0x3f));
                        newString += static_cast<char>(0x80 | (code & 0x3f));
                    }
                    break;
                    }

                default:
                    throw ParseError("Invalid escape sequence at ", reader.getPos());
                    break;

            }
            reader.advance();     
        }
        
        else {
            newString += c;
            reader.advance();
        }
    }
    throw ParseError("Unterminated string at ", reader.getPos());
}


double JSONParser::parseNum() {
    std::string res;
    
    if (reader.peek() == '-') {
        res += reader.advance();

        if (reader.eof()) {
            throw ParseError("Invalid number at ", reader.getPos());
        }
    }

    if (reader.peek() == '0') {
        res += reader.advance();

        if (!reader.eof() && std::isdigit(static_cast<unsigned char>(reader.peek()))) {
            throw ParseError("Leading zeros not allowed at ", reader.getPos());
        }
    }

    while (!reader.eof() && std::isdigit(static_cast<unsigned char>(reader.peek()))) {
        res += reader.advance();
    }


    if (!reader.eof() && reader.peek() == '.') {
        res += reader.advance();

        if (reader.eof() || !std::isdigit(static_cast<unsigned char>(reader.peek()))) {
            throw ParseError("Invalid number at ", reader.getPos());
        }

        while (!reader.eof() && std::isdigit(reader.peek())) {
            res += reader.advance();
        }
    }

    if (!reader.eof()) {
        char c = reader.peek();

        if (c != ',' && c != ']' && c != '}' && !std::isspace(c)) {
            throw ParseError("Invalid number termination at ", reader.getPos());
        }
    }

    return std::stod(res);
}


bool JSONParser::parseBool() {

    if (reader.peek() == 't') {
        std::string t = "true";

        for (char c : t) {
            if (reader.eof() || c != reader.peek()) {
                throw ParseError("Invalid boolean value at ", reader.getPos());
            }
            else {
                reader.advance();
            }
        }
        if (!reader.eof()) {
            char c = reader.peek();

            if (c != ',' && c != ']' && c != '}' && !std::isspace(c)) {
                throw ParseError("Invalid boolean termination at ", reader.getPos());
            }
        }
        return true;
    }

    else {
        std::string f = "false";

        for (char c : f) {
            if (reader.eof() || c != reader.peek()) {
                throw ParseError("Invalid boolean value at ", reader.getPos());
            }
            else {
                reader.advance();
            }
        }
        if (!reader.eof()) {
            char c = reader.peek();

            if (c != ',' && c != ']' && c != '}' && !std::isspace(c)) {
                throw ParseError("Invalid boolean termination at ", reader.getPos());
            }
        }
        return false;
    }
}


std::nullptr_t JSONParser::parseNull() {
    std::string n = "null";

    for (char c : n) {
        if (reader.eof() || c != reader.peek()) {
            throw ParseError("Invalid null value at ", reader.getPos());
        }
        else {
            reader.advance();
        }        
    }
    if (!reader.eof()) {
        char c = reader.peek();

        if (c != ',' && c != ']' && c != '}' && !std::isspace(c)) {
            throw ParseError("Invalid null termination at ", reader.getPos());
        }
    }
    return nullptr;
}


std::unordered_map<std::string, JsonValue> JSONParser::parseObject() {
    reader.advance();
    skipWhiteSpaces();

    std::unordered_map<std::string, JsonValue> map;

    if (!reader.eof() && reader.peek() == '}') {
        reader.advance();
        return map;
    }

    while (!reader.eof()) {
        
        if (reader.peek() != '"') {
            throw ParseError("Wrong key type at ", reader.getPos());
        }

        std::string key = parseString();

        skipWhiteSpaces();

        if (reader.eof() || reader.peek() != ':') {
            throw ParseError("Wrong key type at ", reader.getPos());
        }

        reader.advance();

        skipWhiteSpaces();

        map.emplace(key, parseValue());

        if (reader.eof()) {
            break;
        }

        if (reader.peek() == '}') {
            reader.advance();
            return map;
        }

        if (reader.peek() == ',') {
            reader.advance();
            continue;
        }

        throw ParseError("Invalid comma seperation in object at ", reader.getPos());
    }
    throw ParseError("No closing brackets at ", reader.getPos());
}


std::vector<JsonValue> JSONParser::parseArray() {
    reader.advance();
    skipWhiteSpaces();
    
    std::vector<JsonValue> res;

    if (!reader.eof() && reader.peek() == ']') {
        reader.advance();
        return res;
    }

    while (!reader.eof()) {

        skipWhiteSpaces();

        res.push_back(parseValue());

        if (reader.eof()) {
            break;
        }
        
        if (reader.peek() == ']') {
            reader.advance();
            return res;
        }

        if (reader.peek() == ',') {
            reader.advance();
            continue;
        }

        throw ParseError("Invalid comma seperation in array at ", reader.getPos());

    }
    throw ParseError("No closing brackets at ", reader.getPos());
}