#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

namespace Util {

    void error(const char* message) {
        printf(message);
        printf("\n");
        exit(-1);
    }

    std::string read_file(const char* path) {
        std::ifstream in_stream(path);
        std::string content((std::istreambuf_iterator<char>(in_stream)),(std::istreambuf_iterator<char>()));
        return content;
    }

    inline bool str_equals(const char* str1, const char* str2) {
        return strcmp(str1, str2) == 0;
    }

    inline bool str_starts_with(const char* str, const char* prefix) {
        const char* pointer = strstr(str, prefix);
        return pointer == str;
    }

    inline std::string substr_till_one_before_end(const char* str, size_t index) {
        std::string cpp_str = str;
        size_t length = cpp_str.length();
        std::string substring = cpp_str.substr(index-1, length-index);
        return substring;
    }

    constexpr const char* NUMS = "0123456789";
    constexpr int NUMS_COUNT = 10;

    inline bool is_number(char c) {
        for (int i = 0; i < NUMS_COUNT; ++i) {
            if (c == NUMS[i])
                return true;
        }
        return false;
    }

    inline bool is_str_number(const char* str) {
        int i = 0;
        bool has_period = false;
        while(str[i] != '\0') {
            if (str[i] == '.') {
                if (has_period)
                    return false;
                else
                    has_period = true;
            } else if (!is_number(str[i])) {
                return false;
            }
            ++i;
        }
        return true;
    }

    inline bool is_str_literal(const char* str) {
        if (str[0] != '"')
            return false;
        int last_index = 0;
        while(str[last_index] != '\0')
            ++last_index;
        --last_index;
        return str[last_index] == '"';
    }

    inline std::string str_strip_ends(const char* str) {
        std::string cpp_str = str;
        cpp_str.erase(0, 1);
        cpp_str.erase(cpp_str.size() - 1);
        return cpp_str;
    }

    inline bool is_whitespace(char c) {
        return c == ' ';
    }

    inline bool is_newline(char c) {
        return c == '\n';
    }

    inline bool is_ignored_character(char c) {
        return is_whitespace(c) || is_newline(c);
    }

    inline const char* bool_to_str(bool flag) {
        return flag ? "true" : "false";
    }

}

namespace Tokenization {

    enum TokenType {
        //zunächst werden alle Namen (die keine Zahl-, String-, oder Boolean Literals sind) zu Placeholdern.
        //im nächsten Schritt werden diese dann durch entsprechende Keyword Tokens ersätzt
        PLACEHOLDER = 0,
        NUM_LITERAL = 1,
        STR_LITERAL = 2,
        BOOL_LITERAL = 3,
        //Nutzerdefinierter identifier (Funktions-/Variablenname etc.)
        NAME = 4,
        //Funktionsdeklarationskeyword. Value ist gleich dem Rückgabewert
        DECL_FUNCTION = 5,
        //Keywords der Sprache
        INTRINSIC = 6,
        LINKED = 7,
        NUM = 8,
        STR = 9,
        BOOL = 10,
        //Zeichen
        SEMICOLON = 11,
        OPEN_CURLY = 12,
        CLOSE_CURLY = 13,
        OPEN_PARA = 14,
        CLOSE_PARA = 15,
        COMMA = 16,
        PERIOD = 17,
        EQUALS = 18,
        //weitere keywords
        TYPE = 19,
        FROM = 20
    };

    const char* type_to_string(TokenType type) {
        switch (type) {
            case PLACEHOLDER: return "Placeholder";
            case NUM_LITERAL: return "Number Literal";
            case STR_LITERAL: return "String Literal";
            case BOOL_LITERAL: return "Boolean Literal";
            case NAME: return "Name";
            case DECL_FUNCTION: return "Function Declaration";
            case INTRINSIC: return "Intrinsic";
            case LINKED: return "Linked";
            case NUM: return "Number Declaration";
            case STR: return "String Declaration";
            case BOOL: return "Boolean Declaration";
            case SEMICOLON: return ";";
            case OPEN_CURLY: return "{";
            case CLOSE_CURLY: return "}";
            case OPEN_PARA: return "(";
            case CLOSE_PARA: return ")";
            case COMMA: return ",";
            case PERIOD: return ".";
            case EQUALS: return "Equals";
            case TYPE: return "Type";
            case FROM: return "From";
        }
        return "";
    }

    union TokenValue {
        bool bool_value;
        int int_value;
        float float_value;
        std::string* str_value;
    };

    struct Token {
        TokenType type;
        TokenValue *value = nullptr;
    };

    const char* get_str_value(Token token) {
        return token.value->str_value->c_str();
    }

    //die Symbole sollten hier in der gleichen Reihenfolge wie in TokenType aufgeführt werden, die get_symbol_type Funktion arbeitet mit dieser Annahme
    constexpr const char* SYMBOLS = ";{}(),.=";
    constexpr const int SYMBOL_COUNT = 8;

    bool is_symbol(char c) {
        for (int i = 0; i < SYMBOL_COUNT; ++i) {
            if (SYMBOLS[i] == c)
                return true;
        }
        return false;
    }

    TokenType get_symbol_type(char c) {
        int symbol_index = -1;
        for (int i = 0; i < SYMBOL_COUNT; ++i) {
            if (SYMBOLS[i] == c) {
                symbol_index = i;
                break;
            }
        }
        if (symbol_index == -1)
            return TokenType::PLACEHOLDER;
        //11 ist der Index des ersten Symbols in TokenType
        return static_cast<TokenType>(11 + symbol_index);
    }

    
    void print_token(Tokenization::Token token) {
        printf("==========\n");
        printf("Type: ");
        printf(type_to_string(token.type));
        printf("\n");
        if (token.value != nullptr) {
            const char* value = nullptr;
            switch (token.type) {
                case Tokenization::TokenType::STR_LITERAL:
                case Tokenization::TokenType::NAME:
                case Tokenization::TokenType::DECL_FUNCTION:
                case Tokenization::TokenType::PLACEHOLDER:
                    value = token.value->str_value->c_str();
                    break;
                case Tokenization::TokenType::NUM_LITERAL:
                    value = std::to_string(token.value->float_value).c_str();
                    break;
                case Tokenization::TokenType::BOOL_LITERAL:
                    value = Util::bool_to_str(token.value->bool_value);
                    break;
            }
            if (value != nullptr) {
                printf("value: ");
                printf(value);
                printf("\n");
            }
        }
    }

    class Tokenizer final {
    public:
        Tokenizer(std::string content) {
            this->content = content;
            this->length = content.size();
            printf("created tokenizer\n");
            printf("content is: \n");
            printf(content.c_str());
            printf("\n");
        }

        std::vector<Token> tokenize() {
            while (!is_eof()) {
                char c = current();
                if (Util::is_ignored_character(c)) {
                    consume();
                } else if (current() == '"') {
                    parse_string_literal();
                } else {
                    parse_name_or_symbol();
                }
            }
            return tokens;
        }

        void print_all_tokens() {
            printf("token count %d\n", tokens.size());
            for (int i = 0; i < tokens.size(); ++i) {
                print_token(tokens[i]);
            }
            printf("\n");
        }


    private:
        std::string content;
        size_t pointer = 0, length;
        std::vector<Token> tokens;

        void move_pointer(int steps) {
            pointer += steps;
        }

        char consume() {
            char c = content[pointer];
            ++pointer;
            return c;
        }

        char current() {
            return content[pointer];
        }

        bool is_eof() {
            return pointer >= length;
        }

        char previous(int skips = 0) {
            size_t new_pointer = this->pointer - skips - 1;
            if (new_pointer < 0)
                new_pointer = 0;
            return content[new_pointer];
        }

        char ahead(int skips = 0) {
            size_t new_pointer = this->pointer + 1 + skips;
            if (new_pointer >= this->length) 
                new_pointer = this->length - 1;
            return content[new_pointer];
        }

        void parse_str_literal() {
            Token token;
            token.type = TokenType::STR_LITERAL;
            std::stringstream string_builder;
            //das erste Anführungszeichen entfernen
            consume();
            while (ahead() != '"') {
                string_builder << consume();
            }
            //das beendende Anführungszeichen entfernen
            consume();
            TokenValue *value = new TokenValue();
            value->str_value = new std::string(string_builder.str());
            token.value = value;
            this->tokens.push_back(token);
        }

        void parse_num_literal() {
            Token token;
            token.type = TokenType::NUM_LITERAL;
            std::stringstream string_builder;
            bool has_period = false;
            while (Util::is_number(ahead()) || (!has_period && ahead() == '.')) {
                char c = consume();
                if (c == '.')
                    has_period = true;
                string_builder << c;
            }
            TokenValue *value = new TokenValue();
            value->float_value = std::stof(string_builder.str());
            token.value = value;
            this->tokens.push_back(token);
        }

        //hat seine eigene Funktion, da Symbole wie Kommata, Semikolons etc innerhalb von Strings ignoriert und als Teil des Strings betrachtet werden müssen
        void parse_string_literal() {
            //erstes Anführungszeichen entfernen
            consume();
            std::stringstream string_builder;
            while(current() != '"') {
                string_builder << consume();
                if (is_eof()) {
                    Util::error("Reached end of file while parsing string literal");
                }
            }
            //Anführungszeichen am Ende entfernen
            consume();
            Token token;
            token.type = TokenType::STR_LITERAL;
            TokenValue *token_value = new TokenValue();
            token_value->str_value = new std::string(string_builder.str());
            token.value = token_value;
            this->tokens.push_back(token);
        }

        void parse_name_or_symbol() {
            Token token;
            char c = consume();
            if (is_symbol(c)) {
                token.type = get_symbol_type(c);
            } else {
                std::stringstream string_builder;
                string_builder << c;
                while (!Util::is_whitespace(current()) && !is_symbol(current()) && !is_eof()) {
                    string_builder << consume();
                }
                TokenValue *token_value = new TokenValue();
                const char* value = string_builder.str().c_str();
                token.value = token_value;
                //printf("got name token with value %s\n", value);
                if (Util::str_starts_with(value, "function")) {
                    token.type = TokenType::DECL_FUNCTION;
                    token.value->str_value = new std::string(Util::substr_till_one_before_end(value, 10).c_str());
                } else if (Util::str_equals(value, "false")) {
                    token.type = TokenType::BOOL_LITERAL;
                    token.value->bool_value = false;
                } else if (Util::str_equals(value, "true")) {
                    token.type = TokenType::BOOL_LITERAL;
                    token.value->bool_value = true;
                } else if (Util::str_equals(value, "intrinsic")) {
                    token.type = TokenType::INTRINSIC;
                    delete token.value;
                } else if (Util::str_equals(value, "linked")) {
                    token.type = TokenType::LINKED;
                    delete token.value;
                } else if (Util::str_equals(value, "bool")) {
                    token.type = TokenType::BOOL;
                    delete token.value;
                } else if (Util::str_equals(value, "string")) {
                    token.type = TokenType::STR;
                    delete token.value;
                } else if (Util::str_equals(value, "number")) {
                    token.type = TokenType::NUM;
                    delete token.value;
                } else if (Util::str_equals(value, "type")) {
                    token.type = TokenType::TYPE;
                    delete token.value;
                } else if (Util::str_equals(value, "from")) {
                    token.type = TokenType::FROM;
                    delete token.value;
                } else if (Util::is_str_number(value)) {
                    float num = std::stof(value);
                    token.type = TokenType::NUM_LITERAL;
                    token.value->float_value = num;
                } else {
                    token.type = TokenType::NAME;
                    token.value->str_value = new std::string(value);
                }
            }
            this->tokens.push_back(token);
        }

    };
}

namespace Parsing {

    using namespace Tokenization;

    struct FieldInfo {
        std::string name;
        std::string type;
    };

    struct TypeInfo {
        std::string name;
        std::string super_type;
        size_t field_count;
        FieldInfo* fields;
    };

    struct FunctionInfo {
        std::string name;
        std::string return_type;
        size_t param_count;
        std::string* param_types;
        std::vector<Token> code;
    };

    struct GlobalVarInfo {
        std::string type;
        std::string name;
    };

    struct ParseInfo {
        size_t type_count;
        TypeInfo* types;
        size_t function_count;
        FunctionInfo* functions;
        size_t global_var_count;
        GlobalVarInfo* global_vars;
    };

    void print_parse_info(ParseInfo info) {
        printf("Parse Info:\n");
        printf("=============\n");
        printf("Types:\n");
        for (int i = 0; i < info.type_count; ++i) {
            TypeInfo type_info = info.types[i];
            printf("type #%d: ",i);
            printf(type_info.name.c_str());
            printf(", super-type: ");
            printf(type_info.super_type.c_str());
            printf("\n");
            for (int j = 0; j < type_info.field_count; j++) {
                printf("   %s %s\n", type_info.fields[i].name.c_str(), type_info.fields[i].type.c_str());
            }
        }
    }

    class Parser final {
    public:
        Parser(std::vector<Token> tokens) {
            this->tokens = tokens;
        }

        ParseInfo parse() {
            while (!is_eof()) {
                Token current_token = current();
                if (current_token.type == TokenType::DECL_FUNCTION) {
                    parse_function();
                } else if (current_token.type == TokenType::TYPE) {
                    parse_type();
                } else {
                    parse_global_var();
                }
            }

            //Ergebnisse in ein ParseInfo Object kopieren und dann zurückgeben
            ParseInfo parse_result;
            //Type Info kopieren
            parse_result.type_count = types.size();
            parse_result.types = new TypeInfo[types.size()];
            for (int i = 0; i < types.size(); ++i)
                parse_result.types[i] = types[i];
            //Function Info kopieren
            parse_result.function_count = functions.size();
            parse_result.functions = new FunctionInfo[functions.size()];
            for (int i = 0; i < functions.size(); ++i)
                parse_result.functions[i] = functions[i];
            //Global Var Info kopieren
            parse_result.global_var_count = global_vars.size();
            parse_result.global_vars = new GlobalVarInfo[global_vars.size()];
            for (int i = 0; i < global_vars.size(); ++i)
                parse_result.global_vars[i] = global_vars[i];
            return parse_result;
        }

    private:
        std::vector<Token> tokens;
        size_t pointer = 0;
        std::vector<std::string> taken_identifiers;
        //zur Zwischenspeicherung der Ergebnisse
        std::vector<TypeInfo> types;
        std::vector<FunctionInfo> functions;
        std::vector<GlobalVarInfo> global_vars;

        Token consume() {
            if (is_eof()) {
                Util::error("Unexpectedly ran into end of file while parsing");
            }
            Token token = current();
            ++pointer;
            return token;
        }

        Token current() {
            return tokens[pointer];
        }

        Token ahead(int skips = 0) {
            int new_pointer = pointer + 1 + skips;
            if (new_pointer >= tokens.size())
                new_pointer = tokens.size()-1;
            return tokens[new_pointer];
        }

        bool is_eof() {
            return pointer >= tokens.size();
        }

        bool identifier_exists(std::string identifier) {
            for (int i = 0; i < taken_identifiers.size(); ++i) {
                if (Util::str_equals(identifier.c_str(), taken_identifiers[i].c_str()))
                    return true;
            }
            return false;
        }

        bool type_exists(std::string name) {
            for (int i = 0; i < types.size(); ++i) {
                if (Util::str_equals(name.c_str(), types[i].name.c_str()))
                    return true;
            }
            return false;
        }

        void parse_type() {
            TypeInfo type_info;
            //"type" keyword entfernen
            consume();
            //zuerst werden Name und Name der Superklasse entnommen
            Token name_token = consume();
            if (name_token.type != TokenType::NAME) {
                Util::error("Expected name after type declaration");
            }
            std::string type_name = *(name_token.value->str_value);
            if (identifier_exists(type_name)) {
                printf("Identifier is already taken: %s", type_name.c_str());
                exit(-1);
            }
            type_info.name = type_name;
            Token next_token = consume();
            if (next_token.type == TokenType::FROM) {
                name_token = consume();
                if (name_token.type != TokenType::NAME) {
                    Util::error("Expected name after from clause");
                }
                type_name = *(name_token.value->str_value);
                if (!type_exists(type_name)) {
                    Util::error("type in from clause does not exist");
                }
                type_info.super_type = type_name;
                next_token = consume();
            }
            if (next_token.type != TokenType::OPEN_CURLY) {
                Util::error("Expected open curly brace after type declaration");
            }

            //nun werden Attribute und Methoden entnommen
            std::vector<FieldInfo> fields;
            next_token = consume();
            while(next_token.type != TokenType::CLOSE_CURLY) {
                if (next_token.type == TokenType::DECL_FUNCTION) {
                    //das Entnehmen von Member Functions wird in eine eigene Methode ausgelagert, da die Member Functions nicht Teil des Datentyps sind, sondern nur einen versteckten Parameter haben
                    parse_member_function(type_info);
                } else {
                    FieldInfo field_info;
                    if (next_token.type == TokenType::NAME) {
                        type_name = *(next_token.value->str_value);
                        if (Util::str_equals(type_name.c_str(), type_info.name.c_str()))
                            field_info.type = type_name;
                        else if (type_exists(type_name))
                            field_info.type = type_name;
                        else {
                            printf("Unknown type: %s", type_name.c_str());
                            exit(-1);
                        }
                    } else if (next_token.type == TokenType::NUM) {
                        field_info.type = "number";
                    } else if (next_token.type == TokenType::STR) {
                        field_info.type = "string";
                    } else if (next_token.type == TokenType::BOOL) {
                        field_info.type = "bool";
                    } else {
                        Util::error("Expected type identifier at beginning of field declaration");
                    }
                    name_token = consume();
                    std::string field_name = *(next_token.value->str_value);
                    printf("got field name %s\n", field_name.c_str());
                    if (name_token.type != TokenType::NAME) {
                        Util::error("Expected identifier after field declaration");
                    }
                    for (int i = 0; i < fields.size(); ++i) {
                        if (Util::str_equals(field_name.c_str(), fields[i].name.c_str())) {
                            printf("Duplicate identifier: %s", field_name.c_str());
                            exit(-1);
                        }
                    }
                    field_info.name = field_name;
                    next_token = consume();
                    if (next_token.type != TokenType::SEMICOLON)
                        Util::error("Expected semicolon after field declaration");
                    fields.push_back(field_info);
                }
                next_token = consume();
            }
            types.push_back(type_info);
        }

        void parse_member_function(TypeInfo type) {

        }

        void parse_function() {

        }

        void parse_global_var() {

        }


    };

}

int main(int argc, const char** argv) {
    if (argc <= 1) {
        printf("Provide a path to a .crl file");
        return -1;
    }
    const char* file_path = argv[1];
    std::string content = Util::read_file(file_path);
    Tokenization::Tokenizer tokenizer(content);

    std::vector<Tokenization::Token> tokens = tokenizer.tokenize();

    tokenizer.print_all_tokens();

    Parsing::Parser parser(tokens);

    Parsing::ParseInfo parse_result = parser.parse();

    Parsing::print_parse_info(parse_result);

    printf("DID NOT SEGFAULT YAYYY!");
    return 0;
}
