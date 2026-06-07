#include "JsonValue.h"

#include <variant>



bool JsonValue::isString() const{
    return std::holds_alternative<std::string>(value);
}


bool JsonValue::isNum() const{
    return std::holds_alternative<double>(value);
    
}


bool JsonValue::isBool() const{
    return std::holds_alternative<bool>(value);
}


bool JsonValue::isNull() const{
    return std::holds_alternative<std::nullptr_t>(value);
}


bool JsonValue::isArray() const{
    return std::holds_alternative<std::vector<JsonValue>>(value);
}


bool JsonValue::isObject() const{
    return std::holds_alternative<std::unordered_map<std::string, JsonValue>>(value);
}


const std::string& JsonValue::asString() const {
    return std::get<std::string>(value);
}


double JsonValue::asNum() const {
    return std::get<double>(value);
}


bool JsonValue::asBool() const {
    return std::get<bool>(value);
}


std::nullptr_t JsonValue::asNull() const {
    return std::get<std::nullptr_t>(value);
}


const std::vector<JsonValue>& JsonValue::asArray() const {
    return std::get<std::vector<JsonValue>>(value);
}


const std::unordered_map<std::string, JsonValue>& JsonValue::asObject() const {
    return std::get<std::unordered_map<std::string, JsonValue>>(value);
}