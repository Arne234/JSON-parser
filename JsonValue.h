#pragma once

#include <unordered_map>
#include <vector>
#include <variant>
#include <string>


class JsonValue {

    public:
    
        using ValueType = std::variant<
                                    std::string,
                                    double,
                                    bool,
                                    std::nullptr_t,
                                    std::vector<JsonValue>,
                                    std::unordered_map<std::string, JsonValue>   
                                >;

    private:
        ValueType value;

    public:

        JsonValue(const std::string& s) : value(s) {}
        JsonValue(double d) : value(d) {}
        JsonValue(bool b) : value(b) {}
        JsonValue(std::nullptr_t n) : value(n) {}
        JsonValue(std::vector<JsonValue>&& v) : value(v) {}
        JsonValue(std::unordered_map<std::string, JsonValue>&& u) : value(u) {}

        bool isString() const;
        bool isNum() const;
        bool isBool() const;
        bool isNull() const;
        bool isArray() const;
        bool isObject() const;

        std::string& asString () const;
        double asNum() const;
        bool asBool() const;
        std::nullptr_t asNull() const;
        std::vector<JsonValue>& asArray() const;
        std::unordered_map<std::string, JsonValue>& asObject() const;

};