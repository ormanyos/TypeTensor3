This small header implements a "type tensor".

This can be used to store data in a k-dimensional matrix (N1 x N2 x … x Nk), axes of the matrix are indexed by classes. The matrix is created in compile time, by classes static-types and data can be accessed run-time, by dynamic type. The stored data can be function also, then it can be called by a matrix helper member.
In trivial – one dimensional case, k = 1 - case is can do the same working as virtual function, because function stored compile time, but selection of functions is made by dynamic type of the class.

If, the matrix has two dimensions, then then it achieves double dispatch, because the selected function to calls depends on two class dynamic type.
And, to use this, you can easily make triple or more dimensioned dispatches.

To call a stored function the code calls one virtual function per dimension, so double dispatch function calls 3 functions (2 for indexes and one indirection to call stored function).
