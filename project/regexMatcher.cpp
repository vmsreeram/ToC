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

class DFA
{
public:
    void printState(set<int> name)
    {
        cout << '{';
        if(name.size()!=0)
            for(int i:name) cout << i << ' ';
        cout << '}';
    }

    set<int> start;
    set<set<int> > finals;
    map<pair<set<int>,char>,set<int> > delta;
    set<char> Sigma;

    DFA()
    {
        start = set<int>();
        finals = set<set<int> >();
        delta = map<pair<set<int>,char>,set<int> >();
        // setint2Statestar = map<set<int>, State*>();
    }

    set<NFA::State*> epsilonReachableStates(NFA::State* cur)
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
        set<NFA::State*> curr;
        set<NFA::State*> startepStates = epsilonReachableStates(nfa->start);
        for(NFA::State* eachState : startepStates)
        {
            start.insert(eachState->name);
        }
        queue<set<NFA::State*> > q;
        q.push(startepStates);
        while(!q.empty())
        {
            curr = q.front();
            q.pop();
            set<int> curr_int;
            for(auto x:curr)
                curr_int.insert(x->name);

            if(Sigma.size()!=0)
            for(char ch:Sigma)
            {
                if(delta.find(make_pair(curr_int,ch))!=delta.end())
                    continue;

                set<NFA::State *> nexts;
                set<int> nexts_int;
                if(curr.size()!=0)
                for(NFA::State *cur : curr)
                {
                    if(cur->trFn[ch].size()!=0)
                    for(NFA::State * eachh : cur->trFn[ch])
                    {
                        for(NFA::State * ea : epsilonReachableStates(eachh))
                        {
                            nexts.insert(ea);
                            nexts_int.insert(ea->name);
                        }
                    }
                }
                q.push(nexts);
                delta[make_pair(curr_int,ch)]=nexts_int;
            }
            set<int> nfafinalset;
            for(auto nfafinal : nfa->finals)
                nfafinalset.insert(nfafinal->name);
            
            for(auto pr : delta)
            {
                for(auto finalstate:nfafinalset)
                {
                    if(pr.first.first.find(finalstate)!=pr.first.first.end())
                    {
                        finals.insert(pr.first.first);
                    }
                    if(pr.second.find(finalstate)!=pr.second.end())
                    {
                        finals.insert(pr.second);
                    }
                }
            }

        }
        set<NFA::State*> starts = epsilonReachableStates(nfa->start);
    }
    void printDFA()
    {
        cout << "... [DFA]\n";
        cout << "Start  : ";printState(start);cout<<'\n';
        cout << "Finals : \n";
        if(finals.empty())cout << "<empty>\n";
        else
        {
            for(auto s:finals)
            {
                printState(s);
                cout << "; \n";
            }
            cout << '\n';
        }

        cout << "\ndelta\n=====\n";
        if(delta.empty())cout << "<empty>\n";
        else
        for(auto x:delta)
        {
            printState(x.first.first);
            cout << ' ';
            cout << '\''<<x.first.second<< '\''<<" --> ";
            printState(x.second);
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
    cout << "\nPrinting final NFA\n==================\n";
    var.printNFA();
    DFA _dfa_(&var);
    cout << "\n\n\n\nafterDFA, Printing final NFA\n==================\n";
    var.printNFA();
    cout << "\n\n\n\nPrinting final DFA\n==================\n";
    _dfa_.printDFA();
    
}