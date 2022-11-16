#include<bits/stdc++.h>
using namespace std;

vector<string> parse(string s)
{
    vector<string> t;
    int k = s.find('(');
    string sf=s.substr(0,k);

    t.push_back(sf);
    s.pop_back();
    string s1="";
    for(int i=k+1;i<s.size();i++)
        s1+=s[i];

    if(sf == "star" || sf == "symbol")
    {
        t.push_back(s1);
        return t;
    }

    int op = 0,loc=-1;
    for(int i=0;i<s1.size();i++)
    {
        if(s1[i]=='(') op++;
        else if(s1[i]==')') op--;
        else if(s1[i]==',' && op==0)
        {
            assert(loc==-1);            // only one `,` is permitted
            loc=i;
        }
    }
    assert(loc!=-1);                    // no `,` is not permitted
    assert(op==0);                      // string must be well formed

    string cur="";
    for(int i=0;i<loc;i++)cur+=s1[i];
    t.push_back(cur);

    cur="";
    for(int i=loc+1;i<s1.size();i++)cur+=s1[i];
    t.push_back(cur);
    return t;
}

int nfctr;
class NFA
{
public:
    class State
    {
    public:
        int name;                                  // the name of the state
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
        vector<string> parsed = parse(inp);
        if(parsed[0] == "union")
        {
            NFA* nfa0 = new NFA(parsed[1]);
            NFA* nfa1 = new NFA(parsed[2]);

            map<char,vector<State *> > trFn_;
            trFn_['#'].push_back((nfa0->start));
            trFn_['#'].push_back((nfa1->start));
            
            State* newstate = new State((nfctr++),trFn_);
            start = newstate;
            
            for(State* eachfinal: nfa0->finals)
            {
                finals.push_back(eachfinal);
            }
            for(State* eachfinal: nfa1->finals)
            {
                finals.push_back(eachfinal);
            }
            
            // cout<<"inp="<<inp<<'\n';
            // this->printNFA();
            // cout<<"^^^^^^^^^^^^^^^^\n\n";
        }
        else if(parsed[0] == "concat")
        {
            NFA* nfa0 = new NFA(parsed[1]);
            NFA* nfa1 = new NFA(parsed[2]);

            for(int i=0;i<(nfa0->finals).size();i++)
            {
                ((nfa0->finals)[i]->trFn)['#'].push_back((nfa1->start));
            }

            start = nfa0->start;
            for(State* eachfinal: nfa1->finals)
            {
                finals.push_back(eachfinal);
            }

            // cout<<"inp="<<inp<<'\n';
            // this->printNFA();
            // cout<<"^^^^^^^^^^^^^^^^\n\n";
        }
        else if(parsed[0] == "star"){}
        else if(parsed[0] == "symbol")
        {
            map<char,vector<State *> > emptytrfn_;
            State* lastState1 = new State((nfctr++),emptytrfn_);
            finals.push_back(lastState1);
            
            State* lastState = lastState1;

            map<char,vector<State *> > emptytrfn_1;
            emptytrfn_1[parsed[1][0]].push_back(lastState);
            State* newState = new State((nfctr++),emptytrfn_1);

            start=newState;
            

            // cout<<"inp="<<inp<<'\n';
            // this->printNFA();
            // cout<<"^^^^^^^^^^^^^^^^\n\n";
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

int main()
{
    string inp;
    cin >> inp;
    nfctr=0;
    NFA var(inp);
    cout << "\nPrinting final NFA\n==================\n";
    var.printNFA();
}