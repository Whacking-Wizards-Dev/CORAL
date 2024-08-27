#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Util {
    std::string read_file(const char* path) {
        std::ifstream in_stream(path);
        std::string content((std::istreambuf_iterator<char>(in_stream)),(std::istreambuf_iterator<char>()));
        return content;
    }

    inline bool is_number(char c) {
        return c >= 30 && c <= 39;
    }

    inline bool is_whitespace(char c) {
        return c == ' ';
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
        RTIMPL = 6,
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
        PERIOD = 17
    };

    union TokenValue {
        int int_value;
        float float_value;
        const char* str_value;
    };

    struct Token {
        TokenType type;
        TokenValue *value;
    };

    const char* get_str_value(Token token) {
        return token.value->str_value;
    }

    //die Symbole sollten hier in der gleichen Reihenfolge wie in TokenType aufgeführt werden, die get_symbol_type Funktion arbeitet mit dieser Annahme
    constexpr const char* SYMBOLS = ";{}(),.";
    constexpr const int SYMBOL_COUNT = 7;

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

    class Tokenizer final {
    public:
        Tokenizer(std::string content) {
            this->content = content;
            this->length = content.size();
        }

        std::vector<Token> tokenize() {
            while (!is_eof()) {
                char c = ahead();
                if (Util::is_whitespace(c))
                    continue;
                if (c == '"') {
                    parse_str_literal();
                } else if (Util::is_number(c)) {
                    parse_num_literal();
                } else {
                    parse_name_or_symbol();
                }
            }
            return parse_keywords();
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
            value->str_value = string_builder.str().c_str();
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
            this->tokens.push_back(token);
        }

        void parse_name_or_symbol() {
            Token token;
            char c = consume();
            if (is_symbol) {
                token.type = get_symbol_type(c);
            } else {
                std::stringstream string_builder;
                while (!Util::is_whitespace(ahead()) && !is_symbol(ahead()) && !is_eof()) {
                    string_builder << consume();
                }
                token.type = TokenType::PLACEHOLDER;
                TokenValue *value = new TokenValue();
                value->str_value = string_builder.str().c_str();
            }
            this->tokens.push_back(token);
        }

        //weist den generischen PLACEHOLDER Tokens aus dem tokens vector konkrete Keywords der Sprache zu
        //gibt einen neuen vector zurück
        std::vector<Token> parse_keywords() {
            std::vector<Token> out;

            printf("TODO: parse_keywords implementieren");

            return out;
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

    return 0;
}
