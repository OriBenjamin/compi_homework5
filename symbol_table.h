#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


#include <string>
#include <vector>
#include <unordered_map>


using std::string;
using std::vector;
using std::unordered_map;

struct NameTypeInfo {
std::vector<std::string> names;
std::string type;
};

class SymbolRecord {
public:
    string name;
    string type;
    int offset;

    SymbolRecord() : name(""), type(""), offset(1) {}
    SymbolRecord(const string name, const string type, int offset) :
    name(name),
    type(type),
    offset(offset)
    {}
    SymbolRecord(const SymbolRecord& symbol_record){
        name = symbol_record.name;
        type = symbol_record.type;
        offset = symbol_record.offset;
    }
    ~SymbolRecord() = default;
};

class SymbolTable {
public:
    unordered_map<string, SymbolRecord> symbols_table;
    SymbolTable() : symbols_table() {}    
    void insert(const string& name, const string& type, int offset);
    bool contains(const string& name);
    SymbolRecord getValue(const string& name);
    ~SymbolTable() = default;
};

class TableStack {
private:
    vector<SymbolTable*> table_stack;
    vector<int> offsets;

public:
    TableStack() : table_stack(), offsets() {
        offsets.push_back(0);
        makeNewScope();
        // insert_symbol("print", "function"); //, true, false, {"STRING"}
        // insert_symbol("printi", "function"); //, true, false, {"INT"}
        // insert_symbol("readi", "function"); //true, false, {"INT"}
    }
    
    ~TableStack(){

        for(int i = 0; i < table_stack.size(); i++){
            table_stack.pop_back();
        }
        for(int i = 0; i < offsets.size(); i++){
            offsets.pop_back();
        }
    }
    void insert_symbol(const string &name, const string &type);
    void makeNewScope(bool is_loop = false); //its like push new scope
    void popScope();
    SymbolTable *current_scope();
    bool symbol_exists(const string &name);
    SymbolRecord get_symbol(const string &name);
    int current_offset();
};

#endif //SYMBOL_TABLE_H