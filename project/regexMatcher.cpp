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
        map<char,vector<State> > trFn;           // trFn[s] will be the vector of states, alphabet s will lead to from name
  
        State()
        {
            this->name=0;
            this->trFn=map<char,vector<State> >();
        }
        State(int name_,map<char,vector<State> > trFn_)
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
                for(State y:x.second)
                {
                    cout << y.name << ' ';
                }
                cout<<'\n';
            }
            cout << "``` name=" <<name<<'\n';
        }
    };
    
    State start;
    vector<State> finals;
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
            vector<NFA> nfas;
            nfas.push_back(NFA(vs[0]));
            nfas.push_back(NFA(vs[1]));
            map<char,vector<State> > trFn_;
            for(NFA eachnfa:nfas)
            {
                trFn_['#'].push_back(eachnfa.start);
            }
            State newstate = State((nfctr++),trFn_);
            start = newstate;
            for(NFA eachnfa:nfas)
            {
                for(State eachfinal: eachnfa.finals)
                {
                    finals.push_back(eachfinal);
                }
            }
        }
        else if(oper.size()==1 && oper[0]=='.')
        {
            NFA nfa0(vs[0]);
            NFA nfa1(vs[1]);
            cout<<"nfa0\n";nfa0.printNFA();cout<<'\n';
            cout<<"nfa1\n";nfa1.printNFA();cout<<'\n';

            for(int i=0;i<(nfa0.finals).size();i++)
            {
                ((nfa0.finals)[i].trFn)['#'].push_back(nfa1.start);
            }
            cout<<"'#' := ";
            for(auto s:((nfa0.finals)[0].trFn)['#'])
            {
                cout<<' '<<s.name;
            }
            // nfa0.finals.clear();
            start = nfa0.start;
            for(State &eachfinal: nfa1.finals)
            {
                finals.push_back(eachfinal);
            }
            cout<<"................(*this)\n";(*this).printNFA();cout<<'\n';
        }
        /* 
        else if(oper.size()==1 && oper[0]=='.')
        {
            cout<<"in else if(oper.size()==1 && oper[0]=='.')\n";
            vector<NFA> nfas;
            nfas.push_back(NFA(vs[0]));
            nfas.push_back(NFA(vs[1]));
            cout<<"nfas[0]\n";
            nfas[0].printNFA();
            cout<<"\n\nnfas[1]\n";
            nfas[1].printNFA();
            cout<<"\n\nnfas_printed\n\n";
            for(int i=0;i<(nfas[0].finals).size();i++)
            {
                ((nfas[0].finals)[i].trFn)['#'].push_back(nfas[1].start);
                
            }
            cout<<"\n\ncheck..\n\n";
            cout<<"is empty"<<((nfas[0].finals)[0].trFn).empty()<<'\n';
            cout<<"# := ";
            for(State s:((nfas[0].finals)[0].trFn)['#'])
            {
                cout<<" "<<s.name;
            }
            
            cout<<"\n\nchecked\n\n";
            start = nfas[0].start;
            for(State &eachfinal: nfas[1].finals)
            {
                finals.push_back(eachfinal);
            }

            // for(State &eachfinal: nfas[0].finals)
            // {
            //     eachfinal.trFn['#'].push_back(nfas[1].start);
            // }
        } */
        else if(oper.size()==0)
        {
            map<char,vector<State> > emptytrfn_;
            State lastState = State((nfctr++),emptytrfn_);
            finals.push_back(lastState);
            
            for(int i=vs[0].size()-1;i>=0;i--)
            {
                map<char,vector<State> > emptytrfn_1;
                emptytrfn_1[vs[0][i]].push_back(lastState);
                State newState = State((nfctr++),emptytrfn_1);

                if(i==0)
                    start=newState;
                lastState=newState;
            }

            
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
        start.printState();
        cout<<"\n\n";
        queue<State> q;
        for(auto x : start.trFn)
        {
            for(State ss:x.second)
                q.push(ss);
        }
        while (!q.empty())
        {
            State top = q.front();
            q.pop();
            // if(top.name==0)
            // {
            //     cout<<"0 trfn empty="<<top.trFn.empty()<<'\n';
            // }
            top.printState();
            for(auto x : top.trFn)
            {
                for(State ss:x.second)
                {
                    q.push(ss);
                }
            }
        }
        cout<<"\n\nfinals= ";
        for(State finalss:finals)
            cout<<finalss.name<<' ';
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
    var.printNFA();
}