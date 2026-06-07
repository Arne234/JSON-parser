#include "Reader.h"
#include "JsonValue.h"



class JSONParser {
    private:
        Reader reader;

        void skipWhiteSpaces();

    public:
        JSONParser(const std::string& text) : reader(text) {}

        JsonValue parse();
        JsonValue parseValue();
        std::string parseString();
        double parseNum();
        bool parseBool();
        std::nullptr_t parseNull();
        std::vector<JsonValue> parseArray();
        std::unordered_map<std::string, JsonValue> parseObject();
};