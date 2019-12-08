//
// Created by dyf on 2019/12/4.
//

#ifndef COMPILER_PL0_SLR1PL0_H
#define COMPILER_PL0_SLR1PL0_H


#include <stack>
#include <fstream>
#include <algorithm>
#include <queue>
#include "lexPL0.h"
class SLR1PL0{
public:
    SLR1PL0(){};
    ~SLR1PL0(){};
    void test();
private:
    /*tian*/
    struct sentence{
        /*产生式的表达*/
        bool finish=false;       //是否结束
        int tag=3;          //当前圆点的位置，由于包含有产生式左部以及‘->’存在，故从3开始
        string str;         //产生式
    };

    struct act{
        /*Action表*/
        int l;  //当前状态编号
        string Vt;//终结符
        char tag='-';//r或者s
        int action=-2;//动作
    };

    struct go{
        /*Goto表*/
        int I; //当前状态编号
        char Vn;    //非终结符
        int next_l=-2;//转移状态
    };

    struct next{
        bool isVt;  //0为Vn，1为Vt
        char n;
        string t;
        int point;
    };
    struct I{
        /*状态的列表*/
        int number; //编号
        vector<sentence> l;
        vector<next> next_list;
    };

    struct  first{
        char Vn;
        vector<string> Vt;
    };

    struct follow{
        char Vn;
        vector<string> Vt;
    };

    vector<first> FIRST;
    vector<follow> FOLLOW;

    vector<string> Vt;    //终结符
    vector<char> Vn;    //非终结符

    vector<I> DFA;  //状态集
    act** ACTION;
    go** GOTO;

    stack<int> S;       //分析栈
    stack<string> T;    //符号栈

    vector<string> STR; //生成式列表
    void readFile(string& target);
    bool SLR(string& input);
    void scan();
    void table();
};

#endif //COMPILER_PL0_SLR1PL0_H
