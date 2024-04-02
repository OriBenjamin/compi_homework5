#include "helper.hpp"
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;


string Helper::freshVar(){
    std::stringstream var;
    var << "%" << ++register_num;
    return var.str();
}

Helper &Helper::instance() {
	static Helper inst;//only instance
	return inst;
}

void Helper::generateCode(exp_token *exp) {
    // if(exp->operation == "binop")
    // if (exp->type == "BOOL") {
    //     CodeBuffer::instance().emit("li $t0, 0");
    //     CodeBuffer::instance().emit("li $t1, 1");
    //     CodeBuffer::instance().emit("beq $t0, $t1, " + exp->var);
    // }
    // else if (exp->type == "BYTE") {
    //     CodeBuffer::instance().emit("li $t0, " + exp->value);
    //     CodeBuffer::instance().emit("move " + exp->var + ", $t0");
    // }
    // else {
    //     CodeBuffer::instance().emit("move " + exp->var + ", " + exp->value);
    // }
}