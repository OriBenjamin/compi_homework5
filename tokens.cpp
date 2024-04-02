#include "tokens.h"
#include "hw3_output.hpp"
#include "symbol_table.h"
#include "bp.hpp"
#include "helper.hpp"
#include <iostream>

extern TableStack tables;
extern int yylineno;

using std::string;


base_token::base_token(const string value) : value(value), type(""), var("") {}
base_token::base_token(const base_token &base) : value(base.value), type(base.type), var("") {}

// 
exp_token::exp_token(base_token *token, string type) : base_token() {
    if (type == "ID") {
        if (!tables.symbol_exists(token->value)) {
            output::errorUndef(yylineno, token->value);
            exit(0);
        }
        this->type = tables.get_symbol(token->value).type;
    }
    else if (type == "BYTE")
    {
        if (stoi(token->value) > 255)
        {
            output::errorByteTooLarge(yylineno, token->value);
            exit(0);
        }
        this->type = type;
    }
    else 
    {
        this->type = type;
    }
    this->value = token->value;
}

exp_token::exp_token(exp_token *token) : base_token() {
    this->value = token->value;
    this->type = token->type;
}

exp_token::exp_token(call_token *call) : base_token() {
    this->type = call->type;
    this->value = call->value;
}

// NOT Exp
exp_token::exp_token(string operation, exp_token *exp, string operationVal) : base_token() {
    this->op = operationVal;
    if (operation == "logic" && exp->type == "BOOL") {
        this->type = "BOOL";
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
    // this->var = Helper::freshVar();
    // Helper::generateCode(this);
}
// Exp OP Exp
// AND, ADD, <
exp_token::exp_token(string operation, exp_token *exp1, exp_token *exp2, string operationVal) : base_token() {
    this->op = operationVal;
    if (operation == "relop" && (exp1->type == "INT" || exp1->type == "BYTE") && (exp2->type == "INT" || exp2->type == "BYTE")) {
        this->type = "BOOL";
    } else if (operation == "logic" && exp1->type == "BOOL" && exp2->type == "BOOL") {
        this->type = "BOOL";      
    } else if (operation == "binop" && (exp1->type == "BYTE" && exp2->type == "BYTE")) {
        this->type = "BYTE";
    } else if (operation == "binop" && ((exp1->type == "INT" || exp1->type == "BYTE") && (exp2->type == "INT" || exp2->type == "BYTE"))) {
        this->type = "INT";
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
    // Helper::instance().generateCode(this);
    // code generation
    if (operation == "binop") {
        this->var = Helper::instance().freshVar();
        string llvmOP;
        if (operationVal == "ADD") {
            llvmOP = "add";
        } else if (operationVal == "SUB") {
            llvmOP = "sub";
        } else if (operationVal == "MUL") {
            llvmOP = "mul";
        } else if (operationVal == "DIV") {
            if (exp2->value == "0") {
                // output::errorDivByZero(yylineno);
                // exit(0);
            }
            llvmOP = "sdiv";
        }
        CodeBuffer::instance().emit(this->var + " = " + operationVal + " i32 " + exp1->var + ", " + exp2->var);
    } else if (operation == "relop") {
            this->true_label = CodeBuffer::instance().genLabel();
            this->false_label = CodeBuffer::instance().genLabel();
            CodeBuffer::instance().emit("br i1 " + exp1->var + ", label %" + this->true_label + ", label %" + this->false_label);
    } else if (operation == "logic") {
            this->true_label = exp2->true_label;
            this->false_label = exp2->false_label;
            // goto b.true
            CodeBuffer::instance().emit("br label %" + exp1->true_label);
            
        string tmplabel = CodeBuffer::instance().genLabel();
        CodeBuffer::instance().emit(this->var + " = 0");
        // operationVal = opposite of operationVal(?)
        CodeBuffer::instance().emit("if " + exp1->var + " " + operationVal + " " + exp2->var + " goto " + tmplabel);
        CodeBuffer::instance().emit(this->var + " = 1");
        CodeBuffer::instance().emit(tmplabel + ":");
    }
}

// Cast
exp_token::exp_token(type_token *type, exp_token *exp) : base_token() {
    if (exp->type == "INT" || exp->type == "BYTE") {
        this->type = type->type;
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

type_token::type_token(const string& type) : base_token() {
    this->type = type;
}

call_token::call_token(base_token *func_id, exp_token *exp) : base_token() {
    this->value = func_id->value;
    if (func_id->value == "print")
    {
        if( exp->type == "STRING") {
            this->type = "VOID";
        } else {
            output::errorPrototypeMismatch(yylineno, func_id->value, "STRING");
            exit(0);
        }
    } else if (func_id->value == "printi") 
    {
        if (exp->type == "INT"||exp->type == "BYTE") {
            this->type = "VOID";
        } else {
            output::errorPrototypeMismatch(yylineno, func_id->value, "INT");
            exit(0);
        }
    }
    else if (func_id->value == "readi") 
    {
        if (exp->type == "INT"||exp->type == "BYTE") {
            this->type = "INT";
        } else {
            output::errorPrototypeMismatch(yylineno, func_id->value, "INT");
            exit(0);
        }
    }
    else {
        output::errorUndefFunc(yylineno, func_id->value);
        exit(0);
    }
}
    
statement_token::statement_token(bool in_while) : base_token() {
    this->in_while = in_while;
}