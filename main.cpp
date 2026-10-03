#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <utility>

using namespace std;

class someData
{
public:
  string firstName;
  string lastName;
  string address;
  int age;

  someData() : firstName(""), lastName(""), address(""), age(0) {}

  void print() const
  {
    cout << "Name: " << firstName << " " << lastName
         << ", Address: " << address
         << ", Age: " << age << endl;
  }
};

class exchangePerson
{
public:
  someData data;
  mutex mtx;

  exchangePerson() = default;

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
    {
      cout << "[Swap] Objects have the same address. Swap aborted." << endl;
      return;
    }

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

int main()
{
  exchangePerson p1;
  exchangePerson p2;

  thread t1(exchangePerson::JohnDoe, ref(p1));
  thread t2(exchangePerson::JacobSmith, ref(p2));

  t1.detach();
  t2.detach();

  thread tSwap(exchangePerson::Swap, ref(p1), ref(p2));

  tSwap.join();

  return 0;
}