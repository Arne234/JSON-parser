#pragma once

#include "JsonValue.h"


class Serializer {
    public:
        static std::string serialize(const JsonValue& value);

    private:
        static std::string serializeString(const std::string& str);
};