//
// Created by dyf on 2019/12/4.
//

#include <iomanip>
#include "SLR1PL0.h"
void SLR1PL0:: readFile(string& target){
    fstream in;
    in.open("../target.txt", ios::in);
    getline(in, target);
    in.close();
    cout<<"target:\t\t"<<target<<endl;
}

bool SLR1PL0:: SLR(string& target) {
    string trans;
    vector<string>::iterator itS;
    vector<char>::iterator itC;

    S.push(0);
    T.push("#");
    act ACT;
    string temp;
    int count = 0;
    char C;
    int GOTOHead;
    while (1) {
        for (int i = 0; i < Vt.size(); i++) {
            count = 0;
            if (target.substr(0, Vt[i].size()) == Vt[i]) {
                ACT = ACTION[S.top()][i];
                if (ACT.tag == 'S') {
                    T.push(Vt[i]);
                    target.erase(0, Vt[i].size());
                    S.push(ACT.action);
                    break;
                } else if (ACT.tag == 'R') {
                    if (ACT.action == -1) {
                        cout << "ACCEPT" << endl;
                        return 1;
                    }
                    temp = STR[ACT.action];
                    C = temp[0];
                    itC = find(Vn.begin(), Vn.end(), C);
                    GOTOHead = distance(Vn.begin(), itC);
                    temp.erase(0, 3);
                    while (!temp.empty()) {
                        itC = find(Vn.begin(), Vn.end(), temp[0]);
                        if (itC != Vn.end()) {
                            count++;
                            temp.erase(0, 1);
                            continue;
                        }
                        itS = find(Vt.begin(), Vt.end(), temp.substr(0, 1));
                        if (itS != Vt.end()) {
                            count++;
                            temp.erase(0, 1);
                            continue;
                        }
                        itS = find(Vt.begin(), Vt.end(), temp.substr(0, 2));
                        count++;
                        temp.erase(0, 2);
                        continue;
                    }
                    for (int j = 0; j < count; j++) {
                        S.pop();
                        T.pop();
                    }
                    S.push(GOTO[S.top()][GOTOHead].next_l);
                    trans = C;
                    T.push(trans);
                } else {
                    cout << "FAILED" << endl;
                    return false;
                }
            }
        }
    }
}

void SLR1PL0::scan() {
    vector<int> mark;
    vector<string> pointer;
    vector<string> value;
    fstream in;
    in.open("../test.txt", ios::in);
    string  temp;

    while(!in.eof()){
        getline(in, temp);
        STR.push_back(temp);
    }
    in.close();
    first fi;
    follow fo;
    for(int i=0;i<STR.size();i++){
        temp=STR[i];

        Vn.push_back(temp[0]);
        temp.erase(0, 3);
        mark.clear();
        pointer.clear();
        value.clear();
        lexPL0(temp, &mark, &pointer, &value);
        for(int j=0;j<mark.size();j++){
            if(mark[j]==0)
                Vt.push_back(value[j]);
            else if(mark[j]==1)
                Vt.push_back(value[j]);
            else if (mark[j]==2)
                if(isupper(value[j][0]))
                    Vn.push_back(value[j][0]);
                else
                    Vt.push_back(value[j]);
            else if (mark[j]==3)
                Vt.push_back(value[j]);
            else if(mark[j]==4)
                Vt.push_back(value[j]);
            }
        }
    Vt.push_back("#");
    sort(Vt.begin(), Vt.end());
    sort(Vn.begin(), Vn.end());

    Vt.erase(unique(Vt.begin(), Vt.end()), Vt.end());
    Vn.erase(unique(Vn.begin(), Vn.end()), Vn.end());

    for(int i=0;i<Vn.size();i++){
        fi.Vn=Vn[i];
        FIRST.push_back(fi);
        fo.Vn=Vn[i];
        FOLLOW.push_back(fo);
    }

    vector<vector<int>> m;

    for(int i=0;i<STR.size();i++) {
        for (int j = 0; j < Vt.size(); j++) {
            if (STR[i].substr(3, Vt[j].length()) == Vt[j])
                for (int k = 0; k < Vn.size(); k++) {
                    if (FIRST[k].Vn == STR[i][0])
                        FIRST[k].Vt.push_back(Vt[j]);
                }
        }
    }

    for(int i=0;i<STR.size();i++)
        for(int j=0;j<Vn.size();j++){
            if(Vn[j]==STR[i][0])
                continue;
            if(STR[i][3]==Vn[j]){
                mark.clear();
                mark.push_back(STR[i][0]);
                mark.push_back(Vn[j]);
                m.push_back(mark);
            }
        }
    int p=m.size();
    for(int i=0;i<p;i++){
        mark=m[i];
        m.push_back(mark);
    }

    for(int i=0;i<m.size();i++)
        for(int j=0;j<Vn.size();j++)
            for(int k=0;k<Vn.size();k++)
                if(m[i][0]==FIRST[j].Vn&&m[i][1]==FIRST[k].Vn)
                    FIRST[j].Vt.insert(FIRST[j].Vt.end(), FIRST[k].Vt.begin(), FIRST[k].Vt.end());
    for(int i=0;i<FIRST.size();i++){
        sort(FIRST[i].Vt.begin(), FIRST[i].Vt.end());
        FIRST[i].Vt.erase(unique(FIRST[i].Vt.begin(), FIRST[i].Vt.end()), FIRST[i].Vt.end());
    }

    for(int i=0;i<Vn.size();i++)
        if(FOLLOW[i].Vn=='S')
            FOLLOW[i].Vt.emplace_back("#");

    m.clear();
    mark.clear();

    for(int i=0;i<Vn.size();i++){
        for(int j=0;j<STR.size();j++)
            for(int k=0;k<STR[j].size();k++)
                if(STR[j][k]==Vn[i]&&STR[j].size()>(k+1))
                    for(int n=0;n<Vt.size();n++) {
                        if (STR[j].substr(k + 1, Vt[n].size()) == Vt[n])
                            FOLLOW[i].Vt.push_back(Vt[n]);
                    }
                else if(STR[j][k]==Vn[i]&&STR[j].size()==k+1){
                    mark.clear();
                    mark.push_back(STR[j][0]);
                    mark.push_back(Vn[i]);
                    m.push_back(mark);
                }
    }
    int t=0;
    char qt;
    queue<char> qch;

    while(!qch.empty())
        qch.pop();
    qch.push('S');

    while(!qch.empty()){
        qt=qch.front();
        for(int i=0;i<m.size();i++){
            if(t>=m.size())
                break;
            if(qt==m[i][0]){
                qch.push(m[i][1]);
                swap(m[i], m[t++]);
            }
        }
        qch.pop();
    }

    for(int i=0;i<m.size();i++)
        for(int j=0;j<Vn.size();j++)
            for(int k=0;k<Vn.size();k++)
                if(FOLLOW[j].Vn==m[i][0]&&FOLLOW[k].Vn==m[i][1])
                    FOLLOW[k].Vt.insert(FOLLOW[k].Vt.end(), FOLLOW[j].Vt.begin(), FOLLOW[j].Vt.end());
    for(int i=0;i<FOLLOW.size();i++){
        sort(FOLLOW[i].Vt.begin(), FOLLOW[i].Vt.end());
        FOLLOW[i].Vt.erase(unique(FOLLOW[i].Vt.begin(), FOLLOW[i].Vt.end()), FOLLOW[i].Vt.end());
    }
    cout<<endl;
    cout<<"Vt:\t\t";
    for(int i=0;i<Vt.size();i++)
        cout<<Vt[i]<<'\t';
    cout<<"\nVn:\t\t";
    for(int i=0;i<Vn.size();i++)
        cout<<Vn[i]<<'\t';
    cout<<endl;

    cout<<"FIRST: \n";
    for(int i=0;i<FIRST.size();i++) {
        cout << '\t' << FIRST[i].Vn << '\t' << ": ";
        for (int j = 0; j < FIRST[i].Vt.size(); j++)
            cout << FIRST[i].Vt[j] << ',';
        cout << endl;
    }
    cout<<"FOLLOW: \n";
    for(int i=0;i<Vn.size();i++) {
        cout << '\t' << FOLLOW[i].Vn << '\t' << ": ";
        for (int j = 0; j < FOLLOW[i].Vt.size(); j++)
            cout << FOLLOW[i].Vt[j] << ',';
        cout << endl;
    }

}

void SLR1PL0:: table(){
    sentence sen;
    vector<sentence> vs;
    I ii;
    I jj;
    int number=0;
    ii.number=number;
    int count=0;
    char qt;
    queue<char> qch;
    vector<char>vVn;
    vector<string>vVt;
    vector<string> timeS;
    vector<string>::iterator itS;
    vector<char>::iterator itC;
    vector<I>::iterator itL;
    qch.push('S');
    while(!qch.empty()){
        qt=qch.front();
        for(int i=0;i<STR.size();i++){
            if(STR[i][0]==qt) {
                sen.str = STR[i];
                ii.l.push_back(sen);
                if(isupper(STR[i][sen.tag]))
                    qch.push(STR[i][sen.tag]);
            }
        }
        qch.pop();
    }
    DFA.push_back(ii);
    int flag=0;
    int ppp=0;
    next nt;
    for(int i=0;i<DFA.size();i++){
        vVn.clear();
        vVt.clear();
        timeS.clear();
        for(int j=0;j<DFA[i].l.size()&&i>0;j++)
            for(int k=0;k<Vn.size();k++)
                if(!DFA[i].l[j].finish&&DFA[i].l[j].str[DFA[i].l[j].tag]==Vn[k])
                    for(int m = 0; m < STR.size(); m++)
                        if(STR[m][0]==Vn[k]) {
                            itS = find(timeS.begin(), timeS.end(), STR[m]);
                            if (itS != timeS.end())
                                continue;
                            sen.finish = false;
                            sen.tag = 3;
                            sen.str = STR[m];
                            timeS.push_back(STR[m]);
                            DFA[i].l.push_back(sen);
                        }
        itL=DFA.begin()+i;
        for(int j=0;j<i;j++) {
            flag=0;
            if(DFA[j].l.size()!=DFA[i].l.size())
                continue;
            for (int k = 0; k < DFA[j].l.size(); k++) {
                if(DFA[j].l[k].finish==DFA[i].l[k].finish)
                    flag++;
                else
                    break;
                if(DFA[j].l[k].tag==DFA[i].l[k].tag)
                    flag++;
                else
                    break;
                if(DFA[j].l[k].str==DFA[i].l[k].str)
                    flag++;
                else
                    break;
            }
            if(flag==3*DFA[j].l.size()){
                flag=1;
                ppp=j;
                break;
            } else
                flag=0;
        }
        if(flag) {
            for (int j = 0; j < i; j++) {
                for(int k=0;k<DFA[j].next_list.size();k++){
                    if(DFA[j].next_list[k].point==i+count)
                        DFA[j].next_list[k].point=ppp;
                }
            }
            DFA.erase(itL);
            count++;
            i--;
            continue;
        }
//        DFA[i].number=distance(DFA.begin(), itL);

        vs=DFA[i].l;

        for(int j=0;j<vs.size();j++){
            flag=0;
            for(int k=0;k<Vn.size();k++){
                if(!vs[j].finish&&vs[j].str[vs[j].tag]==Vn[k]) {
                    vVn.push_back(Vn[k]);
                    flag=1;
                    break;
                }
            }
            if(flag==1)
                continue;
            for(int k=0;k<Vt.size();k++){
                if(!vs[j].finish&&vs[j].str.substr(vs[j].tag, Vt[k].size())==Vt[k]){
                    vVt.push_back(Vt[k]);
                    break;
                }
            }
        }
        sort(vVn.begin(), vVn.end());
        vVn.erase(unique(vVn.begin(), vVn.end()), vVn.end());

        sort(vVt.begin(), vVt.end());
        vVt.erase(unique(vVt.begin(), vVt.end()), vVt.end());

        timeS.clear();


        for(int k=0;k<vVn.size();k++) {
            ii.number = ++number;
            ii.l.clear();
            for (int j = 0; j < vs.size(); j++) {
                if (!vs[j].finish && vs[j].str[vs[j].tag] == vVn[k]) {
                    sen.str = vs[j].str;
                    sen.tag = vs[j].tag + 1;
                    if (sen.tag == sen.str.size())
                        sen.finish = true;
                    else
                        sen.finish = false;
                    ii.l.push_back(sen);
                }
            }
            nt.isVt = false;
            nt.n = vVn[k];
            ii.number=DFA.size();
            nt.point = ii.number;
            DFA[i].next_list.push_back(nt);
            DFA.push_back(ii);
        }
        for(int k=0;k<vVt.size();k++) {
            ii.number = ++number;
            ii.l.clear();
            for (int j = 0; j < vs.size(); j++) {
                if (!vs[j].finish && vs[j].str.substr(vs[j].tag, vVt[k].size()) == vVt[k]) {
                    sen.str = vs[j].str;
                    sen.tag = vs[j].tag + vVt[k].size();
                    if (sen.tag == sen.str.size())
                        sen.finish = true;
                    else
                        sen.finish = false;
                }
            }
            ii.l.push_back(sen);
            nt.isVt = true;
            nt.t = vVt[k];
            nt.point = ii.number;
            DFA[i].next_list.push_back(nt);
            DFA.push_back(ii);
        }
    }

    for(int i=0;i<DFA.size();i++){
        if(i==DFA[i].number)
            continue;
        for(int j=0;j<i;j++){
            for(int k=0;k<DFA[j].next_list.size();k++){
                if(DFA[j].next_list[k].point==DFA[i].number)
                    DFA[j].next_list[k].point=i;
            }
        }
        DFA[i].number=i;
    }

    ACTION=new act*[DFA.size()];
    for(int i=0;i<DFA.size();i++){
        ACTION[i]=new act[Vt.size()];
    }
    GOTO = new go*[DFA.size()];
    for(int i=0;i<DFA.size();i++) {
        GOTO[i] = new go[Vn.size()];
    }
    ppp=0;
    flag=0;
    int Distance = 0;
    vector<string>::iterator itSTR;
    for(int i=0;i<DFA.size();i++){
        ppp=0;
        for(int j=0;j<DFA[i].next_list.size();j++){
            if(!DFA[i].next_list[j].isVt){
                itC=find(Vn.begin(), Vn.end(), DFA[i].next_list[j].n);
                ppp=distance(Vn.begin(), itC);
                GOTO[i][ppp]={i, *itC, DFA[i].next_list[j].point};
                continue;
            }
            else{
                itS=find(Vt.begin(), Vt.end(), DFA[i].next_list[j].t);
                ppp=distance(Vt.begin(), itS);
                ACTION[i][ppp]={i, *itS, 'S', DFA[i].next_list[j].point};
                continue;
            }
        }
        ppp=0;
        Distance=0;
        for(int j=0;j<DFA[i].l.size();j++){
            if(DFA[i].l[j].finish){
                itC=find(Vn.begin(), Vn.end(), DFA[i].l[j].str[0]);
                ppp=distance(Vn.begin(), itC);
                for(int k=0;k<FOLLOW[ppp].Vt.size();k++){
                    itSTR=find(STR.begin(), STR.end(), DFA[i].l[j].str);
                    Distance=distance(STR.begin(), itSTR);
                    itS=find(Vt.begin(), Vt.end(), FOLLOW[ppp].Vt[k]);
                    flag=distance(Vt.begin(), itS);
                    ACTION[i][flag]={i, *itS, 'R', Distance};
                }
                if(DFA[i].l[j].str[0]=='S'){
                    itS=find(Vt.begin(), Vt.end(), "#");
                    flag=distance(Vt.begin(), itS);
                    ACTION[i][flag]={i, "#", 'R', -1};
                }
            }
        }
    }
    for(int i=0;i<DFA.size();i++){
        printf("%d:\n", DFA[i].number);
        for(int j=0;j<DFA[i].l.size();j++)
            cout<<'\t'<<DFA[i].l[j].str<<endl;
        for(int j=0;j<DFA[i].next_list.size();j++) {
            if (DFA[i].next_list[j].isVt)
                cout<<"\t\t"<<DFA[i].next_list[j].t<<": "<<DFA[i].next_list[j].point<<endl;
            else if(!DFA[i].next_list[j].isVt)
                printf("\t\t%c: %d\n", DFA[i].next_list[j].n, DFA[i].next_list[j].point);
        }
    }
    cout<<"\n\n";
//    cout<<"          ";
    for(int i=0;i<Vt.size();i++){
        cout<<left<<setw(10)<<Vt[i];
    }
    for(int i=0;i<Vn.size();i++){
        cout<<left<<setw(10)<<Vn[i];
    }
    cout<<endl;
    for(int i=0;i<DFA.size();i++){
        cout<<left<<setw(10)<<i;
        for(int j=0;j<Vt.size();j++){
            cout<<ACTION[i][j].tag<<','<<left<<setw(8)<<ACTION[i][j].action;
        }
        for(int j=0;j<Vn.size();j++){
            cout<<left<<setw(10)<<GOTO[i][j].next_l;
        }
        cout<<endl;
    }
}

void SLR1PL0:: test(){
    scan();
    table();
    string target;
    readFile(target);
    cout<<SLR(target)<<endl;
}
