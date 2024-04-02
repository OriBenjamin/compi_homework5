#ifndef EX5_CODE_HELPER
#define EX5_CODE_HELPER

#include <string>
#include "tokens.h"
#include "bp.hpp"

using std::string;
using std::pair;

class Helper {

public:
    int register_num = 0;
    std::string freshVar();
    static void generateCode(exp_token *exp);
    static Helper &instance();
};


#endif // EX5_CODE_HELPER

