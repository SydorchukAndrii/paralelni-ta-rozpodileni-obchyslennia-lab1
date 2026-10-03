#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>
#include <chrono>

using namespace std;

list<int> l;
mutex mtx;

void AddToList(int valToAdd)
{
  lock_guard<mutex> lock(mtx);
  l.push_back(valToAdd);
  cout << "[AddToList] An element has been added: " << valToAdd << endl;
}

void ListContains(int targetVal, int attempt)
{
  lock_guard<mutex> lock(mtx);
  auto it = find(l.begin(), l.end(), targetVal);
  if (it != l.end())
  {
    cout << "[ListContains] Attempt " << attempt << ": the element " << targetVal << " is in the list" << endl;
  }
  else
  {
    cout << "[ListContains] Attempt " << attempt << ": the element " << targetVal << " is not in the list" << endl;
  }
}

int main()
{
  int initialVal = 42;

  for (int i = 0; i < 10; ++i)
  {
    thread tAdd(AddToList, initialVal + i);
    thread tContains(ListContains, initialVal, i + 1);

    tAdd.detach();
    tContains.detach();
  }

  this_thread::sleep_for(chrono::milliseconds(500));

  return 0;
}