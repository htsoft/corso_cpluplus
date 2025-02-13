#include <iostream>
#include <memory>

struct MyObj
{
    MyObj() { std::cout << "MyObj constructed\n"; }

    ~MyObj() { std::cout << "MyObj destructed\n"; }
};

struct Container : std::enable_shared_from_this<Container> // note: public inheritance
{
    std::shared_ptr<MyObj> memberObj;

    void CreateMember() { memberObj = std::make_shared<MyObj>(); }

    std::shared_ptr<MyObj> GetAsMyObj()
    {
        // Use an alias shared ptr for member
        return std::shared_ptr<MyObj>(shared_from_this(), memberObj.get());
    }
};

#define COUT(str) std::cout << '\n' << str << '\n'

#define DEMO(...) std::cout << #__VA_ARGS__ << " = " << __VA_ARGS__ << '\n'

int main()
{
    COUT("Creating shared container");
    std::shared_ptr<Container> cont = std::make_shared<Container>();
    DEMO(cont.use_count());
    DEMO(cont->memberObj.use_count());

    COUT("Creating member");
    cont->CreateMember();
    DEMO(cont.use_count());
    DEMO(cont->memberObj.use_count());

    COUT("Creating another shared container");
    std::shared_ptr<Container> cont2 = cont;
    DEMO(cont.use_count());
    DEMO(cont->memberObj.use_count());
    DEMO(cont2.use_count());
    DEMO(cont2->memberObj.use_count());

    COUT("GetAsMyObj");
    std::shared_ptr<MyObj> myobj1 = cont->GetAsMyObj();
    DEMO(myobj1.use_count());
    DEMO(cont.use_count());
    DEMO(cont->memberObj.use_count());
    DEMO(cont2.use_count());
    DEMO(cont2->memberObj.use_count());

    COUT("Copying alias obj");
    std::shared_ptr<MyObj> myobj2 = myobj1;
    DEMO(myobj1.use_count());
    DEMO(myobj2.use_count());
    DEMO(cont.use_count());
    DEMO(cont->memberObj.use_count());
    DEMO(cont2.use_count());
    DEMO(cont2->memberObj.use_count());

    COUT("Resetting cont2");
    cont2.reset();
    DEMO(myobj1.use_count());
    DEMO(myobj2.use_count());
    DEMO(cont.use_count());
    DEMO(cont->memberObj.use_count());

    COUT("Resetting myobj2");
    myobj2.reset();
    DEMO(myobj1.use_count());
    DEMO(cont.use_count());
    DEMO(cont->memberObj.use_count());

    COUT("Resetting cont");
    cont.reset();
    DEMO(myobj1.use_count());
    DEMO(cont.use_count());
}

/*
Creating shared container
cont.use_count() = 1
cont->memberObj.use_count() = 0

Creating member
MyObj constructed
cont.use_count() = 1
cont->memberObj.use_count() = 1

Creating another shared container
cont.use_count() = 2
cont->memberObj.use_count() = 1
cont2.use_count() = 2
cont2->memberObj.use_count() = 1

GetAsMyObj
myobj1.use_count() = 3
cont.use_count() = 3
cont->memberObj.use_count() = 1
cont2.use_count() = 3
cont2->memberObj.use_count() = 1

Copying alias obj
myobj1.use_count() = 4
myobj2.use_count() = 4
cont.use_count() = 4
cont->memberObj.use_count() = 1
cont2.use_count() = 4
cont2->memberObj.use_count() = 1

Resetting cont2
myobj1.use_count() = 3
myobj2.use_count() = 3
cont.use_count() = 3
cont->memberObj.use_count() = 1

Resetting myobj2
myobj1.use_count() = 2
cont.use_count() = 2
cont->memberObj.use_count() = 1

Resetting cont
myobj1.use_count() = 1
cont.use_count() = 0
MyObj destructed
*/