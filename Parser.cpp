#include "Parser.h"
#include "ErrorClass.h"

#include <iostream>
#include <string>


void JSONParser::skipWhiteSpaces() {
    while (!reader.eof()) {
        char c = reader.peek();

        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            reader.advance();
        }
        else {
            break;
        }
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
        throw ParseError("No data", reader.getPos());
    }


    char c = reader.peek();
    
    if (c == '"') {
        return parseString(); 
    }

    if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
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

    throw ParseError("No matching datatype", reader.getPos());

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
                throw ParseError("Invalid escape sequence", reader.getPos());
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
                    throw ParseError("Invalid escape sequence", reader.getPos());
                    break;

            }
            reader.advance();     
        }
        
        else {
            if (static_cast<unsigned char>(c) < 0x20) {
                throw ParseError("Invalid control character in string", reader.getPos());
            }

            newString += c;
            reader.advance();
        }
    }
    throw ParseError("Unterminated string", reader.getPos());
}


double JSONParser::parseNum() {
    std::string res;
    
    if (reader.peek() == '-') {
        res += reader.advance();

        if (reader.eof()) {
            throw ParseError("Invalid number", reader.getPos());
        }
    }

    if (reader.peek() == '0') {
        res += reader.advance();

        if (!reader.eof() && std::isdigit(static_cast<unsigned char>(reader.peek()))) {
            throw ParseError("Leading zeros not allowed", reader.getPos());
        }
    }

    else if (!reader.eof() && reader.peek() >= '1' && reader.peek() <= '9') {
        while (!reader.eof() && std::isdigit(static_cast<unsigned char>(reader.peek()))) {
            res += reader.advance();
        }
    }

    else {
        throw ParseError("Invalid number", reader.getPos());
    }

    if (!reader.eof() && reader.peek() == '.') {
        res += reader.advance();

        if (reader.eof() || !std::isdigit(static_cast<unsigned char>(reader.peek()))) {
            throw ParseError("Invalid number", reader.getPos());
        }

        while (!reader.eof() && std::isdigit(static_cast<unsigned char>(reader.peek()))) {
            res += reader.advance();
        }
    }

    if (!reader.eof() &&
        (reader.peek() == 'e' || reader.peek() == 'E')) {

        res += reader.advance();

        if (!reader.eof() &&
            (reader.peek() == '+' || reader.peek() == '-')) {
            res += reader.advance();
        }

        if (reader.eof() ||
            !std::isdigit(static_cast<unsigned char>(reader.peek()))) {
            throw ParseError("Invalid exponent", reader.getPos());
        }

        while (!reader.eof() &&
               std::isdigit(static_cast<unsigned char>(reader.peek()))) {
            res += reader.advance();
        }
    }

    return std::stod(res);
}


bool JSONParser::parseBool() {

    if (reader.peek() == 't') {
        std::string t = "true";

        for (char c : t) {
            if (reader.eof() || c != reader.peek()) {
                throw ParseError("Invalid boolean value", reader.getPos());
            }
            else {
                reader.advance();
            }
        }
        if (!reader.eof()) {
            char c = reader.peek();

            if (c != ',' && c != ']' && c != '}' && !std::isspace(c)) {
                throw ParseError("Invalid boolean termination", reader.getPos());
            }
        }
        return true;
    }

    else {
        std::string f = "false";

        for (char c : f) {
            if (reader.eof() || c != reader.peek()) {
                throw ParseError("Invalid boolean value", reader.getPos());
            }
            else {
                reader.advance();
            }
        }
        if (!reader.eof()) {
            char c = reader.peek();

            if (c != ',' && c != ']' && c != '}' && !std::isspace(c)) {
                throw ParseError("Invalid boolean termination", reader.getPos());
            }
        }
        return false;
    }
}


std::nullptr_t JSONParser::parseNull() {
    std::string n = "null";

    for (char c : n) {
        if (reader.eof() || c != reader.peek()) {
            throw ParseError("Invalid null value", reader.getPos());
        }
        else {
            reader.advance();
        }        
    }
    if (!reader.eof()) {
        char c = reader.peek();

        if (c != ',' && c != ']' && c != '}' && !std::isspace(c)) {
            throw ParseError("Invalid null termination", reader.getPos());
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
            throw ParseError("Wrong key type", reader.getPos());
        }

        std::string key = parseString();

        skipWhiteSpaces();

        if (reader.eof() || reader.peek() != ':') {
            throw ParseError("Wrong key type", reader.getPos());
        }

        reader.advance();

        skipWhiteSpaces();

        auto [it, inserted] =map.emplace(key, parseValue());
        if (!inserted) {
            throw ParseError("Duplicate key in object", reader.getPos());
        }

        skipWhiteSpaces();

        if (reader.eof()) {
            break;
        }

        if (reader.peek() == '}') {
            reader.advance();
            return map;
        }

        if (reader.peek() == ',') {
            reader.advance();
            skipWhiteSpaces();
            continue;
        }

        throw ParseError("Invalid comma seperation in object", reader.getPos());
    }
    throw ParseError("No closing brackets", reader.getPos());
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

        skipWhiteSpaces();

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

        throw ParseError("Invalid comma seperation in array", reader.getPos());

    }
    throw ParseError("No closing brackets", reader.getPos());
}