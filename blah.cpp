#include <iostream>
#include <string>
 
class Base
{
public:
    virtual std::string getName() const { return "Base"; }
};
 
class Derived: public Base
{
public:
    ~Derived() { std::cout << "zoo wee mama" << std::endl; }
    virtual std::string getName() const { return "Derived"; }
};

class OtherThing: public Base
{
public:
    ~OtherThing() { std::cout << "whoa" << std::endl; }
    virtual std::string getName() const { return "Other Thing"; }
};

class holder
{
public:
    Base *my_p = new Derived;

    template <class T>
    void changeClass()
    {
        delete my_p;
        my_p = new T;
        return;
    }
};

int main()
{   
    //Derived derived;
    //Base &rBase{ derived };

    Base *pBase = new Derived;
    std::cout << "pBase is a " << pBase->getName() << '\n';
    delete pBase;
    pBase = new OtherThing;
    std::cout << "pBase is a " << pBase->getName() << '\n';

    holder example;
    std::cout << "example is a " << example.my_p->getName() << '\n';
    example.changeClass<OtherThing>();
    std::cout << "example is a " << example.my_p->getName() << '\n';

    return 0;
}