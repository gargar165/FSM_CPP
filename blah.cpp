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
    virtual std::string getName() const { return "Derived"; }
};

class OtherThing: public Base
{
public:
    virtual std::string getName() const { return "Other Thing"; }
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
 
    return 0;
}