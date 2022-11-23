#include<bits/stdc++.h>
using namespace std;

char alphabet[] = {'a', 'b', 'c'};
int N_LETTERS = 3;

std::vector<std::string> get_all_words(int length)
{
  std::vector<int> index(length, 0);
  std::vector<std::string> words;

  while(true)
  {
    string word;
    word.resize(length);
    for (int i = 0; i < length; ++i)
      word[i] = alphabet[index[i]];
    words.push_back(word);

    for (int i = length-1; ; --i)
    { 
      if (i < 0) return words;
      index[i]++;
      if (index[i] == N_LETTERS)
        index[i] = 0;
      else
        break;
    }
  }
}

int main(int argc, char* argv[])
{
    vector<string> vs;
    for(int len=1;len<=7;len++)
        for(auto s:get_all_words(len))
            vs.push_back(s);

    cout << vs.size() <<'\n';
    for(auto s:vs)
    {
        if(argc == 1)cout << "union(concat(star(symbol(b)),symbol(a)),union(star(symbol(a)),symbol(b)))" << '\n'<<s<<'\n';
        else cout << argv[1] << '\n'<<s<<'\n';
    }
}