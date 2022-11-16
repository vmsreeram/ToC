#include<bits/stdc++.h>
using namespace std;

int nfctr;
class NFA
{
public:
    class State
    {
    public:
        int name;                                // the name of the state
        map<char,vector<State *> > trFn;           // trFn[s] will be the vector of states, alphabet s will lead to from name
  
        State()
        {
            this->name=0;
            this->trFn=map<char,vector<State *> >();
        }
        State(int name_,map<char,vector<State *> > trFn_)
        {
            this->name=name_;
            this->trFn=trFn_;
        }
        void printState()
        {
            cout << "\n... name=" <<name<<'\n'<<"trfn\n=========\n";
            for(auto x:trFn)
            {
                cout << x.first << " := ";
                for(State* y:x.second)
                {
                    cout << y->name << ' ';
                }
                cout<<'\n';
            }
            cout << "``` name=" <<name<<'\n';
        }
    };
    
    State* start;
    vector<State *> finals;
    NFA(string inp)
    {
        vector<string> vs;
        vector<char> oper;
        string cur="";

        for(int i=0;i<inp.size();i++)
        {
            if(inp[i]=='+')
            {
                oper.push_back('+');
                for(int j=0;j<i;j++)
                {
                    cur.push_back(inp[j]);
                }
                vs.push_back(cur);
                cur="";
                for(int j=i+1;j<inp.size();j++)
                {
                    cur.push_back(inp[j]);
                }
                vs.push_back(cur);
                
                cout<<"vs\n";
                for(string ss:vs)
                {
                    cout<<ss<<", ";
                }
                cout<<".\n";
                break;
            }
        }
        if(oper.size()==0)
            for(int i=0;i<inp.size();i++)
            {
                if(inp[i]=='.')
                {
                    oper.push_back('.');
                    for(int j=0;j<i;j++)
                    {
                        cur.push_back(inp[j]);
                    }
                    vs.push_back(cur);
                    cur="";
                    for(int j=i+1;j<inp.size();j++)
                    {
                        cur.push_back(inp[j]);
                    }
                    vs.push_back(cur);
                    break;
                }
            }
        if(oper.size()==0)
        {
            vs.resize(1);
            for(char ch:inp)
                vs[0].push_back(ch);
        }

        if(oper.size()==1 && oper[0]=='+')
        {
            
            NFA* nfa0 = new NFA(vs[0]);
            // cout << nfa0->finals[0]->name <<"$$$" <<'\n';
            NFA* nfa1 = new NFA(vs[1]);
            // cout<< &(nfa0->finals[0]->name)<<"<>"<<endl;
            // cout<< &(nfa1->finals[0]->name)<<"><"<<endl;
            // cout<< &(nfa0->finals[0]->trFn)<<"<>"<<endl;
            // cout<< &(nfa1->finals[0]->trFn)<<"><"<<endl;
            // cout << nfa0->finals[0]->name <<"@@@" <<'\n';
            // cout << nfa1->finals[0]->name <<'$' <<'\n';
            // cout << nfa0->finals[0]->name <<'$' <<nfa1->finals[0]->name <<'\n';

            map<char,vector<State *> > trFn_;
            trFn_['#'].push_back((nfa0->start));
            trFn_['#'].push_back((nfa1->start));
            
            State* newstate = new State((nfctr++),trFn_);
            start = newstate;

            cout << nfa0->finals[0]->name <<'$' <<nfa1->finals[0]->name <<'\n';
            cout << nfa0->finals[0]->name <<'$' <<nfa1->finals[0]->name <<'\n';
            
            for(State* eachfinal: nfa0->finals)
            {
                finals.push_back(eachfinal);
            }
            for(State* eachfinal: nfa1->finals)
            {
                finals.push_back(eachfinal);
            }
            
            cout<<"inp="<<inp<<'\n';
            this->printNFA();
            cout<<"^^^^^^^^^^^^^^^^\n\n";
        }
        else if(oper.size()==1 && oper[0]=='.')
        {
            NFA* nfa0 = new NFA(vs[0]);
            NFA* nfa1 = new NFA(vs[1]);
            // cout<<"nfa0\n";nfa0->printNFA();cout<<'\n';
            // cout<<"nfa1\n";nfa1->printNFA();cout<<'\n';

            for(int i=0;i<(nfa0->finals).size();i++)
            {
                ((nfa0->finals)[i]->trFn)['#'].push_back((nfa1->start));
            }
            cout << "(nfa0->finals[0])->name="<<(nfa0->finals[0])->name<<'\n';
            cout << "(nfa0->finals[0])="<<nfa0->finals[0]<<'\n';
            // cout << (nfa0->finals[0])<<"<>";
            cout<<"'#' := ";
            for(State* s:((nfa0->finals)[0]->trFn)['#'])
            {
                cout<<' '<<s->name;
            }
            cout<<'\n';
            // nfa0.finals.clear();
            start = nfa0->start;
            for(State* eachfinal: nfa1->finals)
            {
                finals.push_back(eachfinal);
            }
            cout<<&(nfa0->start->trFn['a'][0]);
            start->trFn['a'][0]->printState();cout<<"[]";
            cout << start->name;
            cout<<"inp="<<inp<<'\n';
            this->printNFA();
            cout<<"^^^^^^^^^^^^^^^^\n\n";
        }
        else if(oper.size()==0)
        {
            map<char,vector<State *> > emptytrfn_;
            State* lastState1 = new State((nfctr++),emptytrfn_);
            finals.push_back(lastState1);
            
            State* lastState = lastState1;
            for(int i=vs[0].size()-1;i>=0;i--)
            {
                map<char,vector<State *> > emptytrfn_1;
                emptytrfn_1[vs[0][i]].push_back(lastState);
                State* newState = new State((nfctr++),emptytrfn_1);

                if(i==0)
                    start=newState;
                lastState=newState;
            }

            cout<<"inp="<<inp<<'\n';
            this->printNFA();
            cout<<"^^^^^^^^^^^^^^^^\n\n";
        }
        else
        {
            cout<<"Not yet\n";
        }
    }
    void printNFA()
    {
        cout<<"---NFA---\n";
        cout << " start\n=====\n";
        start->printState();
        cout<<"\n\n";
        queue<State *> q;
        for(auto &x : start->trFn)
        {
            for(State * &ss:x.second)
                q.push(ss);
        }
        while (!q.empty())
        {
            State* top = q.front();
            q.pop();
            if(top->name==0)
            {
                cout << "top->name="<<top->name<<'\n';
                cout << "top = "<<(top)<<'\n';
            }
            top->printState();
            for(auto &x : top->trFn)
            {
                for(State * &ss:x.second)
                {
                    q.push(ss);
                }
            }
        }
        cout<<"\n\nfinals= ";
        for(State* finalss:finals)
            cout<<finalss->name<<' ';
        cout<<'\n';
    }
    ~NFA()
    {
        /* ... */
    }
};






string paranthesiser(string s)
{
    string t=s;
    /* TODO */
    return t;
}

int main()
{
    string inp;
    cin >> inp;
    inp=paranthesiser(inp);
    nfctr=0;
    NFA var(inp);
    // var = NFA(inp);
    cout << "\nPrinting final NFA\n==================\n";
    var.printNFA();
}