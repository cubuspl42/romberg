#include "math_parser.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <map>

static double mul(double lhs, double rhs) {
    return lhs * rhs;
}

static double _div(double lhs, double rhs) {
    return lhs / rhs;
}

static double add(double lhs, double rhs) {
    return lhs + rhs;
}

static double sub(double lhs, double rhs) {
    return lhs - rhs;
}

static double ctg(double x) {
    return 1/tan(x);
}

static double arcctg(double x) {
    return M_PI_2 - atan(x);
}

static std::map<char, OperatorInfo> operator_info {
    {'^', {0, true, {(FunctionPointer)pow, 2}}},
    {'*', {1, false, {(FunctionPointer)mul, 2}}},
    {'/', {1, false, {(FunctionPointer)_div, 2}}},
    {'+', {2, false, {(FunctionPointer)add, 2}}},
    {'-', {2, false, {(FunctionPointer)sub, 2}}},
};

static std::map<std::string, Function> function_info {
    {"log", {(FunctionPointer)log, 1}},
    {"sin", {(FunctionPointer)sin, 1}},
    {"cos", {(FunctionPointer)cos, 1}},
    {"tg", {(FunctionPointer)tan, 1}},
    {"ctg", {(FunctionPointer)ctg, 1}},
    {"arcsin", {(FunctionPointer)asin, 1}},
    {"arccos", {(FunctionPointer)acos, 1}},
    {"arctg", {(FunctionPointer)atan, 1}},
    {"arcctg", {(FunctionPointer)arcctg, 1}},
    {"sqrt", {(FunctionPointer)sqrt, 1}},
};

static std::string parse_identifier(std::stringstream &stream) {
    std::string id;
    while(stream && isalnum(stream.peek())) {
        id += stream.get();
    }
    return id;
}

Element convert_operator(const Element &op_element) {
    assert(op_element.type == Element::Type::OPERATOR);
    Element function_element = op_element;
    function_element.type = Element::Type::FUNCTION;
    function_element.function = op_element.op.function;
    return function_element;
}

std::vector<Element> parse_expression(std::stringstream &stream) {
    std::vector<Element> out_buffer;
    std::stack<Element> st;
    while(stream.good()) {
        char c = stream.peek();
        assert(!stream.fail());
        if(c == ')' || c == ']') {
            stream.get();
            while (1) {
                assert(!st.empty());
                Element element = st.top();
                st.pop();
                if(element.type == Element::Type::BRACKET) {
                    break;
                } else {
                    out_buffer.push_back(convert_operator(element));
                }
            }
            if(!st.empty() && st.top().type == Element::Type::FUNCTION) {
                out_buffer.push_back(st.top());
                st.pop();
            }
        } else if(c == '(' || c == '[') {
            stream.get();
            Element element { Element::Type::BRACKET };
#ifdef DEBUG_MATH_PARSER
            element.identifier = "(";
#endif
            st.push(element);
        } else if(isalpha(c)) {
            Element element;
            std::string id = parse_identifier(stream);
#ifdef DEBUG_MATH_PARSER
            element.identifier = id;
#endif
            if(function_info.find(id) != function_info.end()) {
                element.type = Element::Type::FUNCTION;
                element.function = function_info[id];
                st.push(element);
            } else {
                assert(id.length() == 1);
                element.type = Element::Type::VARIABLE;
                element.variable = id[0];
                out_buffer.push_back(element);
            }
        } else if(isnumber(c)) {
            Element element { Element::Type::SCALAR };
            stream >> element.scalar;
            out_buffer.push_back(element);
        } else if(operator_info.find(c) != operator_info.end()) {
            Element current { Element::Type::OPERATOR };
#ifdef DEBUG_MATH_PARSER
            current.identifier = std::string(1, c);
#endif
            current.op = operator_info[stream.get()];
            while(!st.empty() && st.top().type == Element::Type::OPERATOR &&
                  st.top().op.precedence + st.top().op.rtl_associativity <= current.op.precedence) {
                out_buffer.push_back(convert_operator(st.top()));
                st.pop();
            }
            st.push(current);
        } else if(isspace(c)) {
            stream.get();
        }
    }
    assert(!stream.fail());
    while(!st.empty()) {
        out_buffer.push_back(convert_operator(st.top()));
        st.pop();
    }
    return out_buffer;
}

static double take_scalar(std::stack<Element> &st) {
    assert(st.top().type == Element::Type::SCALAR);
    double a = st.top().scalar;
    st.pop();
    return a;
}

double evaluate(const std::vector<Element> &buffer, const std::map<char, double> &variables) {
    std::stack<Element> st;
    for(const Element &element : buffer) {
        if(element.type == Element::Type::SCALAR) {
            st.push(element);
        }
        else if(element.type == Element::Type::VARIABLE) {
            auto it = variables.find(element.variable);
            assert(it != variables.end());
            Element element { Element::Type::SCALAR };
            element.scalar = it->second;
            st.push(element);
        }
        else {
            assert(element.type == Element::Type::FUNCTION && st.size() >= element.function.num_params);
            double a = take_scalar(st);
            Element return_value { Element::Type::SCALAR };
            if(element.function.num_params == 1) {
                UnaryFunctionPointer f = (UnaryFunctionPointer)element.function.pointer;
                return_value.scalar = f(a);
            } else {
                assert(element.function.num_params == 2);
                double b = take_scalar(st);
                BinaryFunctionPointer f = (BinaryFunctionPointer)element.function.pointer;
                return_value.scalar = f(b, a);
            }
            st.push(return_value);
        }
    }
    assert(st.size() == 1 && st.top().type == Element::Type::SCALAR);
    return st.top().scalar;
}
