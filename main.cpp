#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>
#include <chrono>
#include <string>
#include <utility>

using namespace std;

namespace Task_1_2_1
{
  void Thread1() { cout << 1 << endl; }
  void Thread2() { cout << 2 << endl; }

  void run()
  {
    thread t1(Thread1);
    thread t2(Thread2);
  }
}

namespace Task_1_2_2
{
  void Thread1() { cout << 1 << endl; }
  void Thread2() { cout << 2 << endl; }

  void run()
  {
    thread t1(Thread1);
    thread t2(Thread2);

    t1.detach();
    t2.detach();
  }
}

namespace Task_1_2_3
{
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

  void run()
  {
    int initialVal = 42;
    thread t1(AddToList, initialVal);
    thread t2(ListContains, initialVal);

    t1.join();
    t2.join();
  }
}

namespace Task_1_2_4
{
  list<int> l;
  mutex mtx;

  void AddToList(int startVal)
  {
    for (int i = 0; i < 10; ++i)
    {
      int valToAdd = startVal + i;
      mtx.lock();
      l.push_back(valToAdd);
      cout << "[AddToList] An element has been added: " << valToAdd << endl;
      mtx.unlock();
    }
  }

  void ListContains(int targetVal)
  {
    for (int i = 0; i < 10; ++i)
    {
      mtx.lock();
      auto it = find(l.begin(), l.end(), targetVal);
      if (it != l.end())
      {
        cout << "[ListContains] Attempt " << i + 1 << ": the element " << targetVal << " is in the list" << endl;
      }
      else
      {
        cout << "[ListContains] Attempt " << i + 1 << ": the element " << targetVal << " is not in the list" << endl;
      }
      mtx.unlock();
    }
  }

  void run()
  {
    int initialVal = 42;
    thread t1(AddToList, initialVal);
    thread t2(ListContains, initialVal);

    t1.join();
    t2.join();
  }
}

namespace Task_1_2_5
{
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

  void run()
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
  }
}

namespace Task_1_2_6
{
  class someData
  {
  public:
    string firstName, lastName, address;
    int age;
    someData() : firstName(""), lastName(""), address(""), age(0) {}
    void print() const
    {
      cout << "Name: " << firstName << " " << lastName << ", Address: " << address << ", Age: " << age << endl;
    }
  };

  class exchangePerson
  {
  public:
    someData data;
    mutex mtx;

    static void JohnDoe(exchangePerson &person)
    {
      lock_guard<mutex> lock(person.mtx);
      person.data.firstName = "John";
      person.data.lastName = "Doe";
      person.data.address = "Unknown";
      person.data.age = 120;
      cout << "[JohnDoe Thread] Data initialized for John Doe" << endl;
    }

    static void JacobSmith(exchangePerson &person)
    {
      lock_guard<mutex> lock(person.mtx);
      person.data.firstName = "Jacob";
      person.data.lastName = "Smith";
      person.data.address = "Known";
      person.data.age = 1;
      cout << "[JacobSmith Thread] Data initialized for Jacob Smith" << endl;
    }

    static void Swap(exchangePerson &a, exchangePerson &b)
    {
      if (&a == &b)
        return;

      lock(a.mtx, b.mtx);
      lock_guard<mutex> lockA(a.mtx, adopt_lock);
      lock_guard<mutex> lockB(b.mtx, adopt_lock);

      cout << "\n--- Before Swap ---" << endl;
      cout << "Person 1: ";
      a.data.print();
      cout << "Person 2: ";
      b.data.print();

      swap(a.data, b.data);

      cout << "\n--- After Swap ---" << endl;
      cout << "Person 1: ";
      a.data.print();
      cout << "Person 2: ";
      b.data.print();
      cout << "------------------\n"
           << endl;
    }
  };

  void run()
  {
    exchangePerson p1, p2;
    thread t1(exchangePerson::JohnDoe, ref(p1));
    thread t2(exchangePerson::JacobSmith, ref(p2));

    t1.detach();
    t2.detach();

    thread tSwap(exchangePerson::Swap, ref(p1), ref(p2));
    tSwap.join();
  }
}

namespace Task_1_2_7
{
  using Task_1_2_6::someData;

  class exchangePerson
  {
  public:
    someData data;
    mutex mtx;

    static void JohnDoe(exchangePerson &person)
    {
      lock_guard<mutex> lock(person.mtx);
      person.data.firstName = "John";
      person.data.lastName = "Doe";
      person.data.address = "Unknown";
      person.data.age = 120;
      cout << "[JohnDoe Thread] Data initialized for John Doe" << endl;
    }

    static void JacobSmith(exchangePerson &person)
    {
      lock_guard<mutex> lock(person.mtx);
      person.data.firstName = "Jacob";
      person.data.lastName = "Smith";
      person.data.address = "Known";
      person.data.age = 1;
      cout << "[JacobSmith Thread] Data initialized for Jacob Smith" << endl;
    }

    static void Swap(exchangePerson &a, exchangePerson &b)
    {
      if (&a == &b)
        return;

      unique_lock<mutex> lockA(a.mtx, defer_lock);
      unique_lock<mutex> lockB(b.mtx, defer_lock);

      lock(lockA, lockB);

      cout << "\n--- Before Swap ---" << endl;
      cout << "Person 1: ";
      a.data.print();
      cout << "Person 2: ";
      b.data.print();

      swap(a.data, b.data);

      cout << "\n--- After Swap ---" << endl;
      cout << "Person 1: ";
      a.data.print();
      cout << "Person 2: ";
      b.data.print();
      cout << "------------------\n"
           << endl;
    }
  };

  void run()
  {
    exchangePerson p1, p2;
    thread t1(exchangePerson::JohnDoe, ref(p1));
    thread t2(exchangePerson::JacobSmith, ref(p2));

    t1.detach();
    t2.detach();

    thread tSwap(exchangePerson::Swap, ref(p1), ref(p2));
    tSwap.join();
  }
}

int main()
{
  cout << "Select task to run (1-7): ";
  int choice;
  if (!(cin >> choice))
    return 0;

  switch (choice)
  {
  case 1:
    Task_1_2_1::run();
    break;
  case 2:
    Task_1_2_2::run();
    break;
  case 3:
    Task_1_2_3::run();
    break;
  case 4:
    Task_1_2_4::run();
    break;
  case 5:
    Task_1_2_5::run();
    break;
  case 6:
    Task_1_2_6::run();
    break;
  case 7:
    Task_1_2_7::run();
    break;
  default:
    cout << "Invalid choice" << endl;
    break;
  }

  return 0;
}