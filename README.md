
A type tensor is a K dimensional matrix, that is created in compile type. Its axes are indexed by classes and its elements are accessed be objects in runtime.
If its element is a function, then double, triple (or more) dispatch can be implemented in elegant way.

For example, there are seven classes:

class A0 {};  class A1 : public A0 {}; class A2 : public A0 {};   
class B0 {};  class B1 : public B0 {};    
class C0 {};  class C1 : public C0 {};   

Make typelists from classes:

typedef TypeList<A0, A1, A2> ALIST;   
typedef TypeList<B0, B1> BLIST;   
typedef TypeList<C0, C1> CLIST;   

Make a typetensor from type lists, it holds functions:

using Fun3 = void (*)(B0*, C0*, C0* );   
TypeTensor<Fun3, ALIST, BLIST, CLIST> TripleDispatcher;   


access data by type:

ttInt.at<A1, B1>() = 42;

access data by object:

A0* aa = new A1; B0 bb = new B1;
cout << ttInt.at(aa, bb); // -> 42

TypeTensorDemo.cpp 


This can be used to store data in a k-dimensional matrix (N1 x N2 x … x Nk), axes of the matrix are indexed by classes. The matrix is created in compile time, by classes static-types and data can be accessed run-time, by dynamic type. The stored data can be function also, then it can be called by a matrix helper member.
In trivial – one dimensional case, k = 1 - case is can do the same working as virtual function, because function stored compile time, but selection of functions is made by dynamic type of the class.

If, the matrix has two dimensions, then then it achieves double dispatch, because the selected function to calls depends on two class dynamic type.
And, to use this, you can easily make triple or more dimensioned dispatches.

To call a stored function the code calls one virtual function per dimension, so double dispatch function calls 3 functions (2 for indexes and one indirection to call stored function).
