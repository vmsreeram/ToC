#include<bits/stdc++.h>
using namespace std;

// source : https://stackoverflow.com/a/2381031
char alphabet[] = {'a', 'b', 'c', 'd'};         /* change as required */
int N_LETTERS = 4;                              /* change as required */

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
    for(int len=1;len<=6/* change as required */;len++)
        for(auto s:get_all_words(len))
            vs.push_back(s);

    cout << vs.size() <<'\n';
    for(auto s:vs)
    {
        if(argc == 1)cout << "union(concat(star(symbol(c)),symbol(a)),union(star(symbol(a)),symbol(c)))" << '\n'<<s<<'\n';
        else cout << argv[1] << '\n'<<s<<'\n';
    }
}