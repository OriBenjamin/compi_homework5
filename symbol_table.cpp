#include "symbol_table.h"
#include "hw3_output.hpp"
#include <iostream>
#include <algorithm>
#include <c++/4.8.2/bits/algorithmfwd.h>
extern int yylineno;

void SymbolTable::insert(const string& name, const string& type, int offset) {
    SymbolRecord record(name, type, offset);
    symbols_table[name] = record;
}

bool SymbolTable::contains(const string& name) {
    return symbols_table.count(name) > 0;
}

SymbolRecord SymbolTable::getValue(const string& name) {
    return symbols_table[name];
}

void TableStack::insert_symbol(const string& name, const string& type) 
{
    int current_offset = offsets.back();
    offsets.pop_back();

    SymbolTable* currentTable = table_stack.back();
    currentTable->insert(name, type, current_offset);
    offsets.push_back(current_offset + 1);
}

void TableStack::makeNewScope(bool is_loop) {
    SymbolTable* newTable = new SymbolTable();
    table_stack.push_back(newTable);
    offsets.push_back(offsets.back());
    // curr_offset = offsets.back();
}

void TableStack::popScope() {
    SymbolTable* currentTable = table_stack.back();
    table_stack.pop_back();
    //print the scope 
    output::endScope();
    // create a vector of symbols ordered by offset
    vector<SymbolRecord> symbols;
    for (auto it = currentTable->symbols_table.begin(); it != currentTable->symbols_table.end(); ++it) {
        symbols.push_back(it->second);
    }
    sort(symbols.begin(), symbols.end(), [](SymbolRecord a, SymbolRecord b) {
        return a.offset < b.offset;
    });
    // if it is the outer scope
    if (table_stack.empty()) {
        output::printID("print", 0, output::makeFunctionType("STRING", {"VOID"}));
        output::printID("printi", 0, output::makeFunctionType("INT", {"VOID"}));
        output::printID("readi", 0, output::makeFunctionType("INT", {"INT"}));
    }
    // get the global offset
    // int global_offset = 0;
    // for (int i = 0; i < offsets.size(); i++) {
    //     global_offset += offsets[i];
    // }
    // print the symbols
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        output::printID(it->name, it->offset, it->type);
    }
    // update global offset
    delete currentTable;
    offsets.pop_back();
    // output::endScope();
}

SymbolTable* TableStack::current_scope() {
    return table_stack.back();
}

int TableStack::current_offset() {
    return offsets.back();
}

bool TableStack::symbol_exists(const string& name) {
    for (int i = table_stack.size() - 1; i >= 0; i--) {
        if (table_stack[i]->contains(name)) {
            return true;
        }
    }
    // check if it is a library function
    if (name == "print" || name == "printi" || name == "readi") {
        return true;
    }
    return false;
}

SymbolRecord TableStack::get_symbol(const string& name) {
    for (int i = table_stack.size() - 1; i >= 0; i--) {
        if (table_stack[i]->contains(name)) {
            return table_stack[i]->getValue(name);
        }
    }
    output::errorUndef(yylineno, name);
    exit(0);
    //maybe we wants to throw an error here?
    return SymbolRecord("", "", 0);
}