#include <iostream>
#include <thread>
#include <list>
#include <algorithm>

using namespace std;

list<int> l;

void AddToList(int startVal)
{
  for (int i = 0; i < 10; ++i)
  {
    int valToAdd = startVal + i;
    l.push_back(valToAdd);
    cout << "[AddToList] An element has been added: " << valToAdd << endl;
  }
}

void ListContains(int targetVal)
{
  for (int i = 0; i < 10; ++i)
  {
    auto it = find(l.begin(), l.end(), targetVal);
    if (it != l.end())
    {
      cout << "[ListContains] Attempt " << i + 1 << ": the element " << targetVal << " is in the list" << endl;
    }
    else
    {
      cout << "[ListContains] Attempt " << i + 1 << ": the element " << targetVal << " is not in the list" << endl;
    }
  }
}

int main()
{
  int initialVal = 42;

  thread t1(AddToList, initialVal);
  thread t2(ListContains, initialVal);

  t1.join();
  t2.join();

  return 0;
}