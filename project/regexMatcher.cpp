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
    vector<set<int> > States;

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
        set<set<int> > states_set;
        if(!((delta).empty()))
        for(auto pr:delta)
        {
            states_set.insert(pr.first.first);
        }

        for(auto i:states_set)States.push_back(i);
    }
    
    // dsu https://cp-algorithms.com/data_structures/disjoint_set_union.html
    #define MAXN 5000
    vector<int> lst[MAXN];
    int parent[MAXN];
    void make_set(int v) {
        lst[v] = vector<int>(1, v);
        parent[v] = v;
    }
    int find_set(int v) {
        return parent[v];
    }
    void union_sets(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (lst[a].size() < lst[b].size())
                swap(a, b);
            while (!lst[b].empty()) {
                int v = lst[b].back();
                lst[b].pop_back();
                parent[v] = a;
                lst[a].push_back (v);
            }
        }
    }
    // 

    DFA(DFA* dfa)
    {
        cerr << "inside DFA(DFA* dfa)\n";

        int sz = dfa->States.size();
        cerr << "sz="<<sz<<"\n";

        vector<vector<bool> > mat(sz,vector<bool>(sz,false));
        map<set<int>,int> set2int;
        map<int,set<int>> int2set;
        for(int i=0;i<sz;i++)
        {
            set2int[dfa->States[i]]=i;
            int2set[i]=dfa->States[i];
        }
        for(int i=0;i<sz;i++)
        {
            for(int j=i+1;j<sz;j++)
            {
                if(
                    ((dfa->finals.find(int2set[i])!=dfa->finals.end())&&(dfa->finals.find(int2set[j])==dfa->finals.end()))
                    ||
                    ((dfa->finals.find(int2set[i])==dfa->finals.end())&&(dfa->finals.find(int2set[j])!=dfa->finals.end()))
                  )
                {
                    mat[i][j]=true;
                }
            }
        }
        cerr << "MAT___1\n";
        for(int i=0;i<sz;i++)
        {
            for(int j=0;j<sz;j++)
            {
                cerr << mat[i][j] << ' ';
            }
            cerr <<'\n';
        }
        cerr<<'\n';

        bool ok=true;
        while(ok)
        {
            ok=false;
            for(int i=0;i<sz;i++)
            {
                for(int j=i+1;j<sz;j++)
                {
                    if(mat[i][j]==true)continue;
                    for(char ch:dfa->Sigma)
                    {
                        int I,J;
                        I = set2int[dfa->delta[make_pair(int2set[i],ch)]];
                        J = set2int[dfa->delta[make_pair(int2set[j],ch)]];
                        if(mat[I][J]==true)
                        {
                            mat[i][j]=true;
                            ok=true;
                        }
                    }
                }
            }
        }

        cerr << "MAT___2\n";
        for(int i=0;i<sz;i++)
        {
            for(int j=0;j<sz;j++)
            {
                cerr << mat[i][j] << ' ';
            }
            cerr <<'\n';
        }
        cerr<<'\n';

        for(int i=0;i<sz;i++)
            make_set(i);
        for(int i=0;i<sz;i++)
        {
            for(int j=i+1;j<sz;j++)
            {
                if(mat[i][j]==false)
                {
                    union_sets(i,j);
                }
            }
        }

        map<int,set<int> >clpsd;
        for(int i=0;i<sz;i++)
        {
            clpsd[find_set(i)].insert(i);
        }

        cerr<<"clpsd__\n";
        for(auto clp:clpsd)
        {
            cerr << clp.first << " -=-> ";printState(clp.second); cerr<<'\n';
        }
        cerr<<'\n';


        int oldStart = set2int[dfa->start];
        cerr << "oldStart = "<<oldStart<<'\n';

        start = clpsd[oldStart];/////////////////////
        cerr << "start = ";printState(start);cerr<<'\n';

        set<int> oldFinals;
        for(auto s:dfa->finals)
        {
            oldFinals.insert(set2int[s]);
        }
        for(int s:oldFinals)
        {
            finals.insert(clpsd[s]);/////////////////////
        }


        set<pair<pair<set<int>,char>,set<int> > > newDelta;
        for(pair<pair<set<int>,char>,set<int> > pr:dfa->delta)
        {
            newDelta.insert(make_pair(make_pair(clpsd[set2int[pr.first.first]],pr.first.second), clpsd[set2int[pr.second]]));
        }
        for(pair<pair<set<int>,char>,set<int> > pr:newDelta)
        {
            delta[pr.first]=pr.second;
        }

        Sigma = dfa->Sigma;


        if(!((delta).empty()))
        for(auto pr:delta)
        {
            States.push_back(pr.first.first);
        }
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

bool checkString(DFA* dfa, string &s)
{
    if(s.size() == 0)
    {
        return (dfa->finals.find(dfa->start)!=dfa->finals.end());
    }
    set<int> cur = (dfa->start);
    for(int i=0;i<s.size();i++)
    {
        cur = dfa->delta[make_pair(cur,s[i])];
    }
    return (dfa->finals.find(cur)!=dfa->finals.end());
}

void testcase()
{
    string inp;
    cin >> inp;
    nfctr=0;
    NFA var(inp);
    DFA _dfa_(&var);
    // cout << "_dfa_ \n";
    // _dfa_.printDFA();
    // cout << "$$$$$ \n\n";
    DFA __dfa__(&_dfa_);
    cout << "_dfa_ \n";
    _dfa_.printDFA();
    cout << "$$$$$ \n\n";
    cout << "__dfa__ \n";
    __dfa__.printDFA();
    cout << "$$$$$ \n\n";

    // cout << "Num of tests : ";
    int nTests = 1; 
    // cin >> nTests;
    while(nTests--) {
        // cout <<'[' << nTests << ']' << " Enter the string to test : ";
        string s; cin>>s;
        bool ans = checkString(&__dfa__,s);
        if(ans)
        cout << "Yes";
        else
            cout << "No";
        if(ans == checkString(&_dfa_,s))cout << '\n';
        else cout <<" <-- Mismatch found\n";
    }
}

int main()
{
    int nt;cin>>nt;while(nt--)
        testcase();
    /**
     
     * @brief Testing
     * 
     * can use `diff` in command line for testing
     * 
     */
}