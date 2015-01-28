#ifndef math_parser_h
#define math_parser_h

#include <stack>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#define DEBUG_MATH_PARSER 1

typedef double scalar;
typedef double (*FunctionPointer)(...);
typedef double (*UnaryFunctionPointer)(double);
typedef double (*BinaryFunctionPointer)(double, double);

struct Function {
    FunctionPointer pointer;
    unsigned num_params;
};

struct OperatorInfo {
    unsigned precedence;
    bool rtl_associativity;
    Function function;
};

struct Element {
    enum class Type {
        NONE,
        BRACKET,
        OPERATOR,
        SCALAR,
        FUNCTION,
        VARIABLE
    };
#ifdef DEBUG_MATH_PARSER
    std::string identifier;
#endif
    Type type;
    union {
        Function function;
        scalar scalar;
        OperatorInfo op;
        char variable;
    };
    inline Element(Type t = Type::NONE) : type(t) {}
};

std::vector<Element> parse_expression(std::stringstream &ss);
double evaluate(const std::vector<Element> &buffer, const std::map<char, double> &variables);

#endif
