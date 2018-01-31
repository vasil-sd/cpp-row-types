#ifndef __TYPELIST__
#define __TYPELIST__

#include "typeuniverse.h"
#include "typeprint.h"
#include "typeprop.h"
#include "typeprimitive.h"
#include "typeutils.h"

namespace typeuniverse
{
DefType(List, Set);
DefType(Pair, Set);
TypeDiscriminator(List);
TypeDiscriminator(Pair);
} // end of namespace

namespace typelist
{

using namespace typeuniverse;
using namespace typeprint;
using namespace typeprop;
using namespace typeprimitive;

DefTypeSymbol(Nil);
struct Nil
{
    typedef Nil_Symbol type_name;
    typedef List Type;
    typedef Nil type;
};

DefTypeSymbol(TCons);

template<typename Head, typename Tail>
struct TCons
{
    typedef TCons_Symbol type_name;
    typedef Head head;
    typedef Tail tail;
    typedef List Type;
    typedef TCons type;
    typedef TCons I; // inhabited
};

DefTypeSymbol(TPair);

template<typename F, typename S>
struct TPair
{
    typedef TPair_Symbol type_name;
    typedef Pair Type;
    typedef TPair type;
    typedef TPair I;

    typedef F first;
    typedef S second;
};

} // end of namespace

namespace typeprint
{

using namespace typeuniverse;
using namespace typelist;

template<>
struct TypePrinter<Nil, List>
{
    template<typename F>
    static void Print(F f)
    {
        f(reinterpret_cast<const char *>(Nil::type_name::cvalue));
    }

};

template<typename T>
struct TypePrinter<T, List>
{
    template<typename F>
    static void Print(F f)
    {
        f(reinterpret_cast<const char *>(T::type_name::cvalue));
        f("( ");
        TPrinter<typename T::head>::template Print<F>(f);
        f(" , ");
        TPrinter<typename T::tail>::template Print<F>(f);
        f(" )");
    }

};

template<typename P>
struct TypePrinter<P, Pair>
{
    template<typename F>
    static void Print(F f)
    {
        f(reinterpret_cast<const char *>(P::type_name::cvalue));
        f("( ");
        TPrinter<typename P::first>::template Print<F>(f);
        f(" , ");
        TPrinter<typename P::second>::template Print<F>(f);
        f(" )");
    }

};

} // end of namespace

namespace typelist
{

using namespace typeuniverse;
using namespace typeprop;
using namespace typeprint;
using namespace typeprimitive;
using namespace typeutils;

template<typename T>
using Head = typename T::head;

template<typename T>
using Tail = typename T::tail;

template<typename T>
using First = typename T::first;

template<typename T>
using Second = typename T::second;

template<typename L>
struct EmptyAux : False {};

template<>
struct EmptyAux<Nil>: True {};

template<typename L>
using Empty = Essence<EmptyAux<L>>;

// length

template<typename List>
struct LengthAux : cInt<1 + LengthAux<Tail<List>>::cvalue> {};

template<>
struct LengthAux<Nil> : cInt<0> {};

template<typename L, typename = typename InList<L>::I>
using Length = Essence<LengthAux<L>>;

// Some conversion functions

template<typename T, typename ... Args>
struct ToListAux : TCons<T, Essence<ToListAux<Args ...>>> {};

template<typename T>
struct ToListAux<T> : TCons<T, Nil> {};

template<typename ... Args>
using ToList = Essence<ToListAux<Args ...>>;

//**** FoldLeft ****
template<template<typename...> typename F, typename V, typename L>
struct FoldLeftAux : Essence<FoldLeftAux<F, Essence<F<V, Head<L> >>, Tail<L> >> {};

template<template<typename...> typename F, typename V>
struct FoldLeftAux<F, V, Nil> : V {};

template<template<typename...> typename F, typename V, typename L>
using FoldLeft = Essence<FoldLeftAux<F, V, L>>;

//**** FoldRight ****
template<template<typename...> typename F, typename L, typename V>
struct FoldRightAux : Essence<F<Head<L>, Essence<FoldRightAux<F, Tail<L>, V>> >> {};

template<template<typename...> typename F, typename V>
struct FoldRightAux<F, Nil, V> : V {};

template<template<typename...> typename F, typename L, typename V>
using FoldRight = Essence<FoldRightAux<F, L, V>>;

//**** FoldLeftList ****
template<template<typename...> typename F, typename V, typename L>
struct FoldLeftLAux : Essence<FoldLeftLAux<F, Essence<F<V, L>>, Tail<L> >> {};

template<template<typename...> typename F, typename V>
struct FoldLeftLAux<F, V, Nil> : V {};

template<template<typename...> typename F, typename V, typename L>
using FoldLeftL = Essence<FoldLeftLAux<F, V, L>>;

template<typename TL, typename = typename InList<TL>::I>
using Reverse = FoldLeft<SwapTemplateArgs21<TCons>::template T, Nil, TL>;

template<typename L1, typename L2, typename = typename And<InList<L1>, InList<L2>>::I>
using Append = FoldLeft<SwapTemplateArgs21<TCons>::template T, L2, Reverse<L1>>;

// Filter

template<template <typename...> typename P, typename FL>
struct FilterAux
{
    template<typename Acc, typename V>
    using F = If<Inhabited<P<V>>, TCons<V, Acc>, Acc>;
    typedef FoldLeft<F, Nil, FL> type;
};

template<template <typename...> typename P, typename FL>
using Filter = Essence<FilterAux<P, FL>>;

template<template <typename...> typename P, typename FL>
struct FilterNAux
{
    template<typename Acc, typename V>
    using F = If<Inhabited<P<First<Acc>, V>>, TPair< cInt<First<Acc>::cvalue+1>, TCons<V, Second<Acc>>>, TPair< cInt<First<Acc>::cvalue+1>, Second<Acc>>>;
    typedef FoldLeft<F, TPair<cInt<0>, Nil>, FL> type;
};

template<template <typename...> typename P, typename FL>
using FilterN = Second<Essence<FilterNAux<P, FL>>>;

// Make list

template<typename N, typename T>
struct MkListAux : TCons<T, Essence<MkListAux<cInt<N::cvalue - 1>, T>>> {};

template<typename T>
struct MkListAux<cInt<0>, T> : Nil {};

template<typename N, typename T>
using MkList = Essence<MkListAux<N, T>>;



// zip
template<typename L1, typename L2>
struct InterleaveAux : TCons<Head<L1>, TCons<Head<L2>, Essence<InterleaveAux<Tail<L1>, Tail<L2> >>> > {};

template<typename L1>
struct InterleaveAux<L1, Nil> : L1 {};

template<typename L2>
struct InterleaveAux<Nil, L2> : L2 {};

template<>
struct InterleaveAux<Nil, Nil> : Nil {};

template<typename L1, typename L2>
using Interleave = Essence<InterleaveAux<L1, L2>>;

// Interleave3

template<typename L1, typename L2, typename L3>
struct Interleave3Aux : TCons<Head<L1>,
                  TCons<Head<L2>,
                        TCons<Head<L3>,
                              typename Interleave3Aux<Tail<L1>, Tail<L2>, Tail<L3> >::type> > > {};

template<typename L2, typename L3>
struct Interleave3Aux<Nil, L2, L3> : Interleave<L2, L3> {};

template<typename L1, typename L3>
struct Interleave3Aux<L1, Nil, L3> : Interleave<L1, L3> {};

template<typename L1, typename L2>
struct Interleave3Aux<L1, L2, Nil> : Interleave<L1, L2> {};

template<typename L1>
struct Interleave3Aux<L1, Nil, Nil> : L1 {};

template<typename L2>
struct Interleave3Aux<Nil, L2, Nil> : L2 {};

template<typename L3>
struct Interleave3Aux<Nil, Nil, L3> : L3 {};

template<>
struct Interleave3Aux<Nil, Nil, Nil> : Nil {};

template<typename L1, typename L2, typename L3>
using Interleave3 = Essence<Interleave3Aux<L1, L2, L3>>;

// Some helpers

template<typename L, typename Period, typename Pos>
struct PeriodicalSelectAux
{
    template<typename Idx, typename>
    using P = BoolToProp<Idx::cvalue % Period::cvalue == Pos::cvalue>;
    typedef FilterN<P, L> type;
};

template<typename L, typename Period, typename Pos,
         typename = typename And<InList<L>, And<IsIntegralType<Period>, IsIntegralType<Pos>>>::I>
using PeriodicalSelect = Essence<PeriodicalSelectAux<L, Period, Pos>>;

template<typename L>
using SelectEven = PeriodicalSelect<L, cInt<2>, cInt<0>>;

template<typename L>
using SelectOdd = PeriodicalSelect<L, cInt<2>, cInt<1>>;

template<typename L, typename Idx>
using ElementAt = PeriodicalSelect<L, Length<L>, Idx>;

template<typename L, typename From, typename To>
struct SliceAux
{
    template<typename Idx, typename>
    using P = BoolToProp<Idx::cvalue >= From::cvalue && Idx::cvalue <= To::cvalue>;
    typedef FilterN<P, L> type;
};

template<typename L, typename From, typename To,
         typename = typename And<InList<L>, And<IsIntegralType<From>, And<IsIntegralType<To>, BoolToProp<To::cvalue >= From::cvalue>>>>::I>
using Slice = Essence<SliceAux<L, From, To>>;

template<typename Elt, typename L, template<typename...> typename P = TypesEqual,
         typename = typename InList<L>::I>
using IsPresent = Not<Empty<Filter<CloseTemplate<P,Elt>::template T, L> > >;

template<typename Elt, typename L>
using IsPresentType = Not<Empty<Filter<CloseTemplate<TypesEqual,Elt>::template T, L> > >;

template<template<typename...> typename F, typename L, typename = typename InList<L>::I>
using Map = FoldLeft<TemplatePreprocessArgs<SwapTemplateArgs21<TCons>::template T, Identity, TemplateComposition<Essence, F>:: template T>::template T, Nil, Reverse<L>>;

template<template<typename...> typename F, typename L1, typename L2>
struct Map2Aux : TCons<Essence<F<Head<L1>, Head<L2> > >, Essence<Map2Aux<F, Tail<L1>, Tail<L2> >>> {};

template<template<typename...> typename F>
struct Map2Aux<F, Nil, Nil> : Nil {};

template<template<typename...> typename F, typename L1, typename L2,
         typename = typename And<InList<L1>, InList<L2>>::I>
using Map2 = Essence<Map2Aux<F, L1, L2>>;

template<typename L>
using IsListOfPairs = FoldLeft<And, True, Map<InPair, L>>;

template<template<typename...> typename F, typename P,
         typename = typename InPair<P>::I>
using ApplyToPair = Essence<F<First<P>, Second<P>>>;

template<template<typename...> typename F, typename L,
         typename = typename IsListOfPairs<L>::I>
using Map2P = Essence<Map<CloseTemplateTemplateArg<ApplyToPair, F>:: template T, L>>;

template<typename TL1, typename TL2,
         typename = typename And<InList<TL1>, InList<TL2>>::I,
         typename = typename BoolToProp<Length<TL1>::cvalue == Length<TL2>::cvalue>::I>
using Zip = Map2<TPair, TL1, TL2>;

template<typename L1, typename L2,
         typename = typename And<InList<L1>, InList<L2>>::I>
using IsSubset = Essence<FoldLeft<And, True, Map<CloseTemplateLastArgs<IsPresentType, L2>:: template T, L1> >>;

template<typename L1, typename L2,
         typename = typename And<InList<L1>, InList<L2>>::I>
using IsIsomorphic = And<IsSubset<L1, L2>, IsSubset<L2, L1> >;

template<typename L1, typename L2,
         typename = typename And<InList<L1>, InList<L2>>::I>
using ListsEqual = FoldLeft<And, True, Map2<TypesEqual, L1, L2> >;

template<typename T, typename L,
         typename = typename InList<L>::I>
using Intersperse = Interleave<L, MkList<Length<L>, T> >;

template<typename T1, typename T2>
struct ProductAux
{
    template<typename N1>
    using P1 = Map<CloseTemplate<TPair, N1>::template T, T2>;

    typedef FoldLeft<Append, Nil, Map<P1, T1> > type;
};

template<typename L1, typename L2, typename = typename And<InList<L1>, InList<L2>>::I>
using Product = Essence<ProductAux<L1, L2>>;

template<typename T, typename TL, template<typename...> typename P = TypesEqual, typename = typename InList<TL>::I>
using Present = FoldLeft<Or, False, Map<TemplateComposition<Essence, CloseTemplate<P, T>::template T>::template T, TL> >;

template<typename TL, template<typename...> typename P>
struct UniqAux1
{
    template<typename A, typename TLst>
    using F = If<Present<Head<TLst>, Tail<TLst>, P>, A, TCons<Head<TLst>, A> >;

    typedef FoldLeftL<F, Nil, TL> type;
};

template<typename TL, template<typename...> typename P = TypesEqual, typename = typename InList<TL>::I>
using Uniq = Essence<UniqAux1<TL, P>>;


template<typename TL, template<typename...> typename P = TypesEqual,
         typename = typename InList<TL>::I>
using IsUniq = BoolToProp<Length<TL>::cvalue == Length<Uniq<TL, P>>::cvalue>;

template<typename TL1, typename TL2, template<typename...> typename P = TypesEqual>
struct IntersectAux
{
    template<typename T>
    using F = Present<T, TL2, P>;
    typedef Filter<F, TL1> type;
};

template<typename TL1, typename TL2, template<typename...> typename P = TypesEqual,
         typename = typename And<InList<TL1>, InList<TL2>>::I>
using Intersect = Essence<IntersectAux<TL1, TL2, P>>;

template<typename TL1, typename TL2, template<typename...> typename P = TypesEqual>
struct SubtractAux
{
    template<typename T>
    using F = Not<Present<T, TL2, P>>;
    typedef Filter<F, TL1> type;
};

template<typename TL1, typename TL2, template<typename...> typename P = TypesEqual,
         typename = typename And<InList<TL1>,InList<TL2>>::I>
using Subtract = Essence<SubtractAux<TL1, TL2, P>>;

template <template<typename ...> typename T, typename TL, typename ... Args>
struct ApplyToListAux : ApplyToListAux<T, Tail<TL>, Args..., Head<TL>> {};

template <template<typename ...> typename T, typename ... Args>
struct ApplyToListAux<T, Nil, Args...> : T<Args...> {};

template<template <typename ...> typename T, typename TL, typename = typename InList<TL>::I>
using ApplyToList = Essence<ApplyToListAux<T, TL>>;

//assoc

template<typename N, typename TL, template<typename...> typename P = TypesEqual,
         typename = typename InList<TL>::I>
using Assoc = Second<Head<Filter<TemplateComposition<CloseTemplate<P, N>::template T, First>::template T, Zip<SelectEven<TL>, SelectOdd<TL>>>>>;

template<typename From, typename To>
struct MkIntsListAux : TCons<From, Essence<MkIntsListAux<cInt<From::cvalue+1>, To>>> {};

template<typename From>
struct MkIntsListAux<From, From> : Nil {};

template<typename From, typename To,
         typename = typename And<IsIntegralType<From>, And<IsIntegralType<To>, BoolToProp<To::cvalue >= From::cvalue>>>::I>
using MkIntsList = Essence<MkIntsListAux<From, To>>;

template<typename TL>
using Enumerate = Zip<MkIntsList<cInt<0>,Length<TL>>, TL>;

template<template<typename> typename P, typename TL,
         typename = typename InList<TL>::I>
using Search = Essence<First<Head<Filter<TemplateComposition<P, Second>::template T, Enumerate<TL>>>>>;

template<typename ...Args>
using Switch = Search<Identity, ToList<Args...>>;

/* Example of switch usage:

    template<typename T, typename Case = Switch<
             //0
             TypesEqual<typename T::Type, Int>,
             //1 
             TypesEqual<typename T::Type, Char>,
             //2 
             TypesEqual<typename T::Type, Short>
             >>
    struct SwitchExample : False {};

    template<typename T>
    struct SwitchExample<T, cInt<0>> : True {};

    template<typename T>
    struct SwitchExample<T, cInt<1>> : True {};

    template<typename T>
    struct SwitchExample<T, cInt<2>> : True {};
*/

template<typename L, typename = typename InList<L>::I>
using IsListMonomorphic = BoolToProp<Length<Uniq<Map<TypeOf, L>>>::cvalue <= 1>;

template<typename L, typename T, typename = typename IsListMonomorphic<L>::I>
using IsListElementsOfType = TypesEqual<T, TypeOf<Head<L>>>;

} // end of namespace typelist

#endif // __TYPELIST__
