#include "lexPL0.h"

bool Number(string str, int* flag, string* value,int* length){
    int N=0,P=0,j=0;
    bool b=false;/*整型数值标记*/
    int e=1;
    double v;
    *length=0;
    while(str[0]<='9'&&str[0]>='0'){
        N=N*10+(str[0]-48);
        str.erase(0, 1);    (*length)++;
    }
    if(str[0] == '.')
        b=false;
    else
        b=true;
    if(!b){
        str=str.erase(0,1);
        (*length)++;
        if(!(str[0]<='9'&&str[0]>='0'))
            return false;
        while(str[0]<='9'&&str[0]>='0'){
            N=N*10+(str[0]-48);
            j++;
            str=str.erase(0,1);
            (*length)++;
        }
    }
    if(str[0]=='e'){
        str=str.erase(0,1);
        (*length)++;
        if(str[0]=='-'){
            e=-1;   str=str.erase(0, 1);    (*length)++;
        }
        else if(str[0]=='+'){
            e=1;    str=str.erase(0, 1);    (*length)++;
        }
        if(!(str[0]<='9'&&str[0]>='0'))
            return false;
        while(str[0]<='9'&&str[0]>='0'){
            P=P*10+(str[0]-48);
            str=str.erase(0, 1);    (*length)++;
            b=false;
        }
    }
    *flag=4;
    v=N*pow(10, e*P-j);
    stringstream ss;
    ss<<v;
    ss>>*value;
    return true;
}

bool Boundary(string* str, int* flag, string* pointer, string* value){
    char Boundary[5]={'(', ')', ',', ';', '.'};
    string _Boundry[5]={"lparen", "rparen", "comma", "semicolon", "period"};

    for(int i=0;i<5;i++)
        if((*str)[0]==Boundary[i]){
            *flag=3;
            *str=str->erase(0,1);
            *pointer=_Boundry[i];
            *value=Boundary[i];
            return true;
        }
    return false;
}

bool Operator(string* str, int* flag,string* pointer, string* value){
    string Operator[11]={"<=", "<", ">=", ">", ":=", "+", "-", "*", "/", "=", "#"};
    string _Operator[11]={"leq", "lss", "geq","gtr", "becomes","plus", "minus", "times", "slash", "sql", "neq"};

    for(int i=0;i<11;i++)
        if(str->substr(0, Operator[i].length())==Operator[i]){
            *str=str->erase(0, Operator[i].length());
            *flag=1;
            *pointer=_Operator[i];
            *value=Operator[i];
            return true;
        }
    return false;
}

bool Keyword(string* str, int* flag, string* pointer, string* value){
    string Keyword[13]={"const", "var", "procedure", "begin", "end", "odd", "if", "then", "call", "while", "do", "read", "write"};
    string _Keyword[13]={"constsym", "varsym", "procsym", "beginsym", "endsym", "oddsym", "ifsym", "thensym", "callsym", "whilesym", "dosym", "readsym", "writesym"};
    for(int i=0;i<13;i++)
        if(str->substr(0, Keyword[i].length())==Keyword[i]&&
           !(((*str)[Keyword[i].length()] <= 'z' && (*str)[Keyword[i].length()] >= 'a') ||
             ((*str)[Keyword[i].length()] <= 'Z' && (*str)[Keyword[i].length()] >= 'A') ||
             ((*str)[Keyword[i].length()] <= '9' && (*str)[Keyword[i].length()] >= '0')||
             ((*str)[Keyword[i].length()]=='_')))
        {
            *flag=0;
            *pointer=_Keyword[i];
            *value=Keyword[i];
            *str=str->erase(0,Keyword[i].length());
            return true;
        }
    return false;
}

bool Ident(string* str, string* value){
    value->clear();
    if(((*str)[0]<='z'&&(*str)[0]>='a')||((*str)[0]<='Z'&&(*str)[0]>='A')){
        while (((*str)[0] <= 'z' && (*str)[0] >= 'a' )|| ((*str)[0] <= 'Z' && (*str)[0] >= 'A') || ((*str)[0] <= '9' && (*str)[0] >= '0'))
        {
            value->push_back((*str)[0]);
            *str=str->erase(0,1);
        }
    }
    return true;
}

int lexPL0(string str, vector<int>* mark,vector<string>* pointer, vector<string>* value) {
    string category[6] = {"keyword", "operator", "ident", "boundary", "number", "ERROR"};

    int length;
    int flag;
    string temp;
    string ttt;
    while (str[0]) {
        flag = -1;
        if (str[0] == ' ')
            str = str.erase(0, 1);
        else if (str[0] <= '9' && str[0] >= '0') {
            if (Number(str, &flag, &temp, &length)) {
                mark->push_back(4);
                pointer->push_back(temp);
                value->push_back(temp);
            }
            else {
                mark->push_back(5);
                pointer->push_back(category[5]);
                value->push_back(category[5]);
            }
            str = str.erase(0, length);
            continue;
        }
        else {
            if (Boundary(&str, &flag, &temp, &ttt)) {
                mark->push_back(3);
                pointer->push_back(temp);
                value->push_back(ttt);
                continue;
            }
            if (Operator(&str, &flag, &temp, &ttt)) {
                mark->push_back(1);
                pointer->push_back(temp);
                value->push_back(ttt);
                continue;
            }
            if (Keyword(&str, &flag, &temp, &ttt)) {
                mark->push_back(0);
                pointer->push_back(temp);
                value->push_back(ttt);
                continue;
            }
            if (Ident(&str, &temp)) {
                mark->push_back(2);
                pointer->push_back(temp);
                value->push_back(temp);
                continue;
            }
        }
    }
    return 0;
}
