
A TypeTensor is a K dimensional matrix, that is created in compile type. Its axes are indexed by classes and its elements are accessed be objects in runtime.
If its element is a function, then double, triple (or more) dispatch can be implemented in simple way.

*For example, there are ten classes:*
```C++
class A0 {};  class A1 : public A0 {}; class A2 : public A0 {};
class B0 {};  class B1 : public B0 {}; class B2 : public B0 {}; class B3 : public B0 {};
class C0 {};  class C1 : public C0 {}; class C2 : public C0 {};
```
*Make typelists from classes:*
```C++
typedef TypeList<A0, A1, A2> ALIST;   
typedef TypeList<B0, B1, B2, B3> BLIST;   
typedef TypeList<C0, C1, C2> CLIST;   
```
*Make a TypeTensor from type lists, it stores functions, function pointers are created:*
```C++
using Fun3 = void (*)(A0*, B0*, C0* );
TypeTensor<Fun3, ALIST, BLIST, CLIST> TripleDispatcher;   
```
*Define dispatch handler functions and they are stored to matrix elements:*
```C++
TripleDispatcher.at<A1, B1, C1>() = [](A0* a, B0* b, C0* c) { 
  cout << "A1 - B1 - C1 called" << endl; 
};
TripleDispatcher.at<A2, B3, C1>() = [](A0* a, B0* b, C0* c) { 
  cout << "A2 - B3 - C1 called" << endl; 
};
```
*Define objects:*
```C++
A0* a = new A2;
B0* b = new B3;
C0* c = new C1;
```
*Call via TypeTensor:*
```C++
TripleDispatcher.Call(a, b, c); // A2 - B3 - C1 called
```

The TypeTensorDemo.cpp file has some exmamples.

If, the matrix has two dimensions, then then it achieves double dispatch, because the selected function to calls depends on two class dynamic type.

To call a stored function the code calls one virtual function per dimension, so double dispatch function calls 3 functions (2 for indexes and one indirection to call stored function).
