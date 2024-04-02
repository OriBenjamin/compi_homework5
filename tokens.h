#ifndef TOKENS_H333
#define TOKENS_H333

#include <string>

using std::string;

class type_token;
class call_token;


class base_token {
public:
    string value;
    string type;
    string var;
    string op;
    string true_label;
    string false_label;
    string next_label;
    base_token() = default;
    base_token(const string value);
    base_token(const base_token &base);
    virtual ~base_token() = default;
};

#define YYSTYPE base_token*
class exp_token : public base_token {
public:
    // bool is_id = false;
    // Exp: NUM / STRING / TRUE   
    exp_token(base_token *token, string type);
    // Exp: (Exp)
    exp_token(exp_token *token);
    // OP1
    exp_token(string operation, exp_token *exp, string operationVal);
    // OP2
    exp_token(string operation, exp_token *exp1, exp_token *exp2, string operationVal);
    // Cast
    exp_token(type_token *type, exp_token *exp);
    // ID cases
    exp_token(call_token *call);
    virtual ~exp_token() = default;
};

// class id_token : public base_token {
// public:
//     // Exp: ID
//     id_token(base_token *token);
//     virtual ~id_token() = default;
// };
class type_token : public base_token {
public:
    type_token(const string& type);
    virtual ~type_token() = default;
};


class call_token : public base_token {
public:
    call_token(base_token *func_id, exp_token *exp);
    virtual ~call_token() = default;
};

class statement_token : public base_token {
public:
    bool in_while;
    statement_token(bool in_while);
    virtual ~statement_token() = default;
};

class program_token : public base_token {
public:
    program_token(statement_token *token);
    virtual ~program_token() = default;
};

#endif // TOKENS_H333