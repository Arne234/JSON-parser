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
        JsonValue(const double d) : value(d) {}
        JsonValue(const bool b) : value(b) {}
        JsonValue(const std::nullptr_t n) : value(n) {}
        JsonValue(const std::vector<JsonValue>& v) : value(v) {}
        JsonValue(const std::unordered_map<std::string, JsonValue>& u) : value(u) {}

        bool isString() const;
        bool isNum() const;
        bool isBool() const;
        bool isNull() const;
        bool isArray() const;
        bool isObject() const;

        const std::string& asString () const;
        const double asNum() const;
        const bool asBool() const;
        const std::nullptr_t asNull() const;
        const std::vector<JsonValue>& asArray() const;
        const std::unordered_map<std::string, JsonValue>& asObject() const;

};