//
// Created by dyf on 19-11-28.
//

#ifndef COMPILER_PL0_LEXPL0_H
#define COMPILER_PL0_LEXPL0_H
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <sstream>

using namespace std;
bool Number(string str, int* flag, string* value,int* length);
bool Boundary(string* str, int* flag, string* pointer, string* value);
bool Operator(string* str, int* flag, string* pointer, string* value);
bool Keyword(string* str, int* flag, string* pointer, string* value);
bool Ident(string* str, string* value);
int lexPL0(string str, vector<int>* mark,vector<string>* pointer, vector<string>* value);
#endif //COMPILER_PL0_LEXPL0_H
