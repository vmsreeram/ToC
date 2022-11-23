#include<bits/stdc++.h>
using namespace std;

char alphabet[] = {'a', 'b'};
int N_LETTERS = 2;

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

int main()
{
    vector<string> vs;
    for(int len=1;len<=5;len++)
        for(auto s:get_all_words(len))
            vs.push_back(s);

    cout << vs.size() <<'\n';
    for(auto s:vs)
    {
        cout << "union(union(star(symbol(b)),symbol(a)),union(star(symbol(a)),symbol(b))) \n"<<s<<'\n';
    }
}