#include "serializer.h"

#include <iostream>
#include <format>



std::string Serializer::serialize(const JsonValue& value) {
    
    if (value.isString()) {
        return "\"" + serializeString(value.asString()) + "\"";
    }


    if (value.isNum()) {
        return std::format("{}", value.asNum());
    }


    if (value.isBool()) {
        return value.asBool() ? "true" : "false";
    }


    if (value.isNull()) {
        return "null";
    }


    if (value.isArray()) {
        std::string res = "[";

        const std::vector<JsonValue>& arr = value.asArray();

        if (arr.empty()) {
            res += "]";
            return res;
        }

        for (size_t i = 0; i < arr.size(); i++) {
            res += serialize(arr[i]);

            if (i + 1 < arr.size()) {
                res += ", ";
            }
        }
        res += "]";
        return res;
    }


    if (value.isObject()) {
        std::string res = "{";

        const std::unordered_map<std::string, JsonValue>& object = value.asObject();

        if (object.empty()) {
            res += "}";
            return res;
        }

        bool isFirst = true;

        for (const auto& [key, val] : object) {
            if (!isFirst) {
                res += ",";
            }

            isFirst = false;

            res += "\"" + serializeString(key) + "\"";
            res += ": ";
            res += serialize(val);
        }

        res += "}";
        return res;
    }

    throw std::runtime_error("Unknown JsonValue type");
}

std::string Serializer::serializeString(const std::string& str) {
    std::string res;

    for (char c : str) {
        switch(c) {

            case '"':
                res += "\\\"";
                break;

            case '\\':
                res += "\\\\";
                break;

            case '\n':
                res += "\\n";
                break;

            case '\t':
                res += "\\t";
                break;

            case '\b':
                res += "\\b";
                break;

            case '\r':
                res += "\\r";
                break;

            case '\f':
                res += "\\f";
                break;

            default:
                res += c;
        }
    }
    return res;
}   