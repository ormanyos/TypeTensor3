/// <summary>
///  TypeTensor demo:
///   - store integers by static type, retrieve by dynamic type
///   - Double Dispatch
///   - Triple Dispatch
/// </summary>

#include <iostream>
#include <string>
#include "TypeTensor.hpp"

using namespace std;

// preparation, create inheritances
// A0 -> A1 -> A2
// B0 -> B1 -> B2 -> B3
// C0 -> C1 -> C2 -> C3

struct A0;
struct A1;
struct A2;

struct B0;
struct B1;
struct B2;
struct B3;

struct C0;
struct C1;
struct C2;
struct C3;

// typelists, for axes of tensor(s)
typedef TTDimK::TypeList<A0, A1, A2> ALIST;
typedef TTDimK::TypeList<B0, B1, B2, B3> BLIST;
typedef TTDimK::TypeList<C0, C1, C2, C3> CLIST;

struct A0 {
    
    // tensor index of class, depends only typelist, need not change when inheritance changed!
    virtual size_t TensorIdx() { return TTDimK::TLIdx<ALIST, A0>(); }

    // static type, static function (non-virtual would be enough)
    static string StaticTypeName() { return "A0"; }

};
struct B0 {
    virtual size_t TensorIdx() { return TTDimK::TLIdx<BLIST, B0>(); }
    static string StaticTypeName() { return "B0"; }
};
struct C0 {
    virtual size_t TensorIdx() { return TTDimK::TLIdx<CLIST, C0>(); }
    static string StaticTypeName() { return "C0"; }
};


#define CreateInherited(BASE, ACTUAL, LIST)                                     \
    struct ACTUAL : public BASE {                                               \
    virtual size_t TensorIdx() { return TTDimK::TLIdx<LIST, ACTUAL>(); }        \
    static string StaticTypeName() { return #ACTUAL; }                          \
    };

CreateInherited(A0, A1, ALIST)
CreateInherited(A1, A2, ALIST)

CreateInherited(B0, B1, BLIST)
CreateInherited(B1, B2, BLIST)
CreateInherited(B2, B3, BLIST)

CreateInherited(C0, C1, CLIST)
CreateInherited(C1, C2, CLIST)
CreateInherited(C2, C3, CLIST)

// --------------------------------------------- main entry
int main()
{

    // ----------------------------------------- N1xN2 tensor, with integer items
    TTDimK::TypeTensor<int, ALIST, BLIST> intK2;
    // set by static types:
    intK2.at<A1, B2>() = 12;
    intK2.at<A2, B2>() = 22;

    vector<A0*> as{ new A0, new A1, new A2 };
    vector<B0*> bs{ new B0, new B1, new B2, new B3 };
    vector<C0*> cs{ new C0, new C1, new C2, new C3 };

    // access by dynamic types:
    cout << intK2.at(as[0], bs[0]) << endl; // => 0 (default array value)
    cout << intK2.at(as[1], bs[2]) << endl; // => 12

    // ----------------------------------------- N1xN2 tensor, with functions (DoubleDispatch)
    using Fun2 = void (*)(B0*, C0*);
    TTDimK::TypeTensor<Fun2, BLIST, CLIST> DoubleDispatcher;

    DoubleDispatcher.at<B1, C2>() = [](B0* b, C0* c) { 
        cout << "Static types " << b->StaticTypeName () << " " << c->StaticTypeName() << " (B1 - C2 called)" << endl; 
    };
    DoubleDispatcher.at<B2, C3>() = [](B0* b, C0* c) { 
        cout << "Static types " << b->StaticTypeName() << " " << c->StaticTypeName() << " (B2 - C3 called)" << endl;
    };

    DoubleDispatcher.Call(bs[1], cs[2]); // => Static types B0 C0 (B1 - C2 called)
    DoubleDispatcher.Call(bs[2], cs[3]); // => Static types B0 C0 (B2 - C3 called)

    // ------------------------------------------- N1xN2xN3 tensor, with functions (TripleDispatch) 
    using Fun3 = void (*)(A0*, B0*, C0*);
    TTDimK::TypeTensor<Fun3, ALIST, BLIST, CLIST> TripleDispatcher;

    TripleDispatcher.at<A1, B1, C2>() = [](A0* a, B0* b, C0* c) { 
        cout << "Static types " << a->StaticTypeName() << " " << b->StaticTypeName() << " " << c->StaticTypeName() << " (A1 - B1 - C2 called)" << endl;
    };

    TripleDispatcher.Call(as[1], bs[1], cs[2]); // => Static types A0 B0 C0 (A1 - B1 - C2 called)
    // ....

}
