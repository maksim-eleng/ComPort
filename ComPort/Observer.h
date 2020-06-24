#pragma once
#include <list>
#include <string>


using namespace std;

class SupervisedString;
class IObserver
{
public:
  virtual void handleEvent(const SupervisedString&) = 0;
};

class SupervisedString// Observable class
{
  string _str;
  list<IObserver*> _observers;

  void _Notify()
  {
    for (auto& observer : _observers)
    {
      observer->handleEvent(*this);
    }
  }

public:
  void add(IObserver& ref)
  {
    _observers.push_back(&ref);
  }

  void remove(IObserver& ref)
  {
    _observers.remove(&ref);
  }

  const string& get() const
  {
    return _str;
  }

  void reset(string str)
  {
    _str = str;
    _Notify();
  }
};

class Reflector : public IObserver // Prints the observed string into cout
{
public:
  virtual void handleEvent(const SupervisedString& ref) override
  {
    
    cout << ref.get() << endl;
  }
};

class Counter : public IObserver // Prints the length of observed string into cout
{
public:
  virtual void handleEvent(const SupervisedString& ref) override
  {
    cout << "length = " << ref.get().length() << endl;
  }
};

class Core :public IObserver
{
public:
  virtual void handleEvent(const SupervisedString&) override
  {
    cout << "Core event";
  }
};