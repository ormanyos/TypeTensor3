#pragma once

#include <array>

/// <summary>
/// TypeTensor K dimensions.
/// 
///  If K=2 and stored data is a function, then it is another DoubleDispatch implementation ("original" with visitor pattern)
/// 
///  If Data is a function then K parametered dispatch (Triple and more)
///  It is a matrix N1xN2x...xNK, which is created in compile time.
///  The matrix's axes are types, every element can be indexed by types.
///  Element of the matrix can be anything.
///  If it is a function for example, which is called when dynamic type of object of axis type points to that.
///  Author:  Tamas Orosz 2020
///  2DO:
///   - generalize indexer virtual function with functor or traits (now fix name and signature: TensorIdx)
///   - maybe possible add constexpr to some places...
///   - hide internal templates from "interface" (place to inside typetensor?)
///   - parameters checks...
/// </summary>

namespace TTDimK {

    // ---------------------------------------------------------- TypeList, helper --------------
    template <typename ... T>
    struct TypeList {
        constexpr static size_t RevIdx() { return 0; }
        constexpr static size_t Len() { return 0; }

        static void Call() { }
    };

    template <typename H, typename... T >
    struct TypeList<H, T...> : TypeList<T...>
    {
        using Head = H;
        using Tail = TypeList<T...>;

        constexpr static size_t Len() { return 1 + Tail::Len(); }

        using Tail::RevIdx;
        constexpr static size_t RevIdx(Head*) {
            return Len();
        }

        static void Call() {
            Head::ClassName();
            Tail::Call();
        }
    };

    // ----------------------------------------------------------- Get index of a class in typelist ---------
    template <typename TypeList, typename Type>
    constexpr size_t TLIdx(void) { return TypeList::Len() - TypeList::RevIdx((Type*)nullptr); }
    // ---------------------------------------------------------- builder of typetensor's raw array data --------------
    template <typename Data, typename ... AxisTail >
    struct Builder {
        using ArrayT = Data;
    };

    template <typename Data, typename AxisHead, typename ... AxisTail >
    struct Builder<Data, AxisHead, AxisTail...>
    {
        using ArrayT = std::array< typename Builder<Data, AxisTail...>::ArrayT, AxisHead::Len() >;
    };
    // ---------------------------------------------------------- Digger by type --------------
    template < typename Data, typename ParamList, typename ... AxisTail >
    struct TypeDigger {
        template <typename ArrayT>
        static Data& Dig(ArrayT& arr) { return arr; }

    };
    template <typename Data, typename ParamList, typename AxisHead, typename ... AxisTail >
    struct TypeDigger<Data, ParamList, AxisHead, AxisTail ... > {

        template <typename ArrayT>
        static Data& Dig(ArrayT& arr) {
            constexpr size_t idx = TLIdx<AxisHead, typename ParamList::Head>();
            return TypeDigger<Data, typename ParamList::Tail, AxisTail...>::Dig(arr[idx]);
        }
    };
    // ---------------------------------------------------------- Digger by objects --------------
    template < typename Data, typename ... ParamTail >
    struct ObjDigger {
        template <typename ArrayT>
        static Data& Dig(ArrayT& arr) { return arr; }
    };
    template <typename Data, typename ParamHead, typename ... ParamTail>
    struct ObjDigger<Data, ParamHead, ParamTail... > {
        template <typename ArrayT>
        static Data& Dig(ArrayT& arr, ParamHead ph, ParamTail... rest) {
            size_t idx = ph->TensorIdx(); // need to be generalized !!!
            return ObjDigger<Data, ParamTail...>::Dig(arr[idx], rest...);
        }
    };
    // ---------------------------------------------------------- The tensor --------------
    template <typename Data, typename ... AllAxes >
    struct TypeTensor
    {
        using ArrayT = typename Builder<Data, AllAxes...>::ArrayT;
        inline static ArrayT arr;

        using MyData = Data;

        template <typename ... Params>
        static Data& at() {
            return TypeDigger<Data, TypeList<Params...>, AllAxes...>::Dig(arr);
        }

        template <typename ... Args>
        static Data& at(Args ... params) {
            return ObjDigger<Data, Args...>::Dig(arr, params...);
        }

        template <typename ... Args>
        static auto Call(Args ... params) {
            return (ObjDigger<Data, Args...>::Dig(arr, params...))(params...);
        }

    };

    // -----------------------------------------------------------------


} // namespace
