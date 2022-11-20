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
        else if(parsed[0] == "star")
        {
            NFA* nfa0 = new NFA(parsed[1]);

            for(int i=0;i<(nfa0->finals).size();i++)
            {
                ((nfa0->finals)[i]->trFn)['#'].push_back((nfa0->start));
            }

            map<char,vector<State *> > trFn_;
            trFn_['#'].push_back(nfa0->start);

            State* newstate = new State((nfctr++),trFn_);
            start=newstate;

            for(int i=0;i<(nfa0->finals).size();i++)
                finals.push_back(nfa0->finals[i]);
            finals.push_back(newstate);
        }
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

        set<int> visited;
        visited.insert(start->name);

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

            if(visited.find(top->name)!=visited.end())
                continue;
            visited.insert(top->name);

            top->printState();
            for(auto &x : top->trFn)
            {
                for(State * &ss:x.second)
                {
                    if(visited.find(ss->name)==visited.end())
                        q.push(ss);
                }
            }
        }
        cout<<"\n\nfinals = ";
        for(State* finalss:finals)
            cout<<finalss->name<<' ';
        cout<<'\n';
    }
    set<char> findSigma()
    {
        set<char> ans;
        set<int> visited;
        visited.insert(start->name);
        queue<State *> q;
        for(auto &x : start->trFn)
        {
            ans.insert(x.first);
            for(State * &ss:x.second)
                q.push(ss);
        }
        while (!q.empty())
        {
            State* top = q.front();
            q.pop();

            if(visited.find(top->name)!=visited.end())
                continue;
            visited.insert(top->name);

            for(auto &x : top->trFn)
            {
                ans.insert(x.first);
                for(State * &ss:x.second)
                {
                    if(visited.find(ss->name)==visited.end())
                        q.push(ss);
                }
            }
        }
        if(ans.count('#')!=0)
            ans.erase('#');
        return ans;
    }
    ~NFA()
    {
        /* ... */
    }
};

int dfctr;
class DFA
{
public:
    class State
    {
    public:
        set<int> name;                             // the name of the state

        State()
        {
            this->name=set<int>();
        }
        State(set<int> name_)
        {
            this->name=name_;
        }
        void printState()
        {
            cout << '{';
            if(name.size()!=0)
                for(int i:name) cout << i << ' ';
            cout << '}';
        }
    };

    State* start;
    vector<State*> finals;
    map<pair<State*,char>,State*> delta;
    // map<set<int>, State*> setint2Statestar;
    set<char> Sigma;

    DFA()
    {
        start = new State();
        finals = vector<State*>();
        delta = map<pair<State*,char>,State*>();
        // setint2Statestar = map<set<int>, State*>();
    }

    set<NFA::State*> getEpsilonReachableStates(NFA::State* cur)
    {
        set<NFA::State*> v;
        v.insert(cur);
        queue<NFA::State *> q;
        q.push(cur);
        while(!q.empty())
        {
            NFA::State * top = q.front();
            q.pop();
            if(top->trFn['#'].size()!=0)
                for(auto s : top->trFn['#'])
                {
                    if(v.find(s)==v.end())
                    {
                        v.insert(s);
                        q.push(s);
                    }
                }
        }
        return v;
    }

    DFA(NFA* nfa)
    {
        Sigma = nfa->findSigma();
        // set<set<int> > visited;
        queue<set<NFA::State* > > queueue;
        //
        set<NFA::State*> start_states=getEpsilonReachableStates(nfa->start);
        set<int> start_statess;
        for(auto &ss:start_states)
        {
            start_statess.insert(ss->name);
        }
        start = new State(start_statess);
        // visited.insert(start->name);
        queueue.push(start_states);
        while(!queueue.empty())
        {
            set<NFA::State* > top = queueue.front();
            for(char ch:Sigma)
            {
                set<int> nextState_int;
                for(auto eachState : top)
                    nextState_int.insert(eachState->name);
                    
                State* nextState = new State(nextState_int);
            }
        }

        ///start


        ///finals

        //
    }
    void printDFA()
    {
        cout << "... [DFA]\n";
        cout << "Start  : ";start->printState();cout<<'\n';
        cout << "Finals : \n";
        if(finals.empty())cout << "<empty>\n";
        else
        {
            for(auto s:finals)
            {
                s->printState();
                cout << "; \n";
            }
            cout << '\n';
        }

        cout << "\ndelta\n=====\n";
        if(delta.empty())cout << "<empty>\n";
        else
        for(auto x:delta)
        {
            x.first.first->printState();
            cout << ' ';
            cout << '\''<<x.first.second<< '\''<<" --> ";
            x.second->printState();
            cout <<'\n';
        }
        cout << "``` [DFA]\n";
    }
};

int main()
{
    string inp;
    cin >> inp;
    nfctr=0;
    NFA var(inp);
    // cout << "\nPrinting final NFA\n==================\n";
    // var.printNFA();
    DFA _dfa_(&var);
}