#ifndef __TYPELIST__
#define __TYPELIST__

#include "typeuniverse.h"
#include "typeprint.h"
#include "typeprop.h"
#include "typeprimitive.h"

namespace typeuniverse
{
DefType(List, Set);
DefType(Pair, Set);
TypeDiscriminator(List);
TypeDiscriminator(Pair);
} // end of namespace

namespace typeprint
{
using namespace typeuniverse;
TypePrinterMacro(List, Set);
TypePrinterMacro(Pair, Set);
} // end of namespace

namespace typelist
{

using namespace typeuniverse;
using namespace typeprint;
using namespace typeprop;
using namespace typeprimitive;

struct Nil
{
    typedef List Type;
    typedef Nil type;
};

template<typename Head, typename Tail>
struct TCons
{
    typedef Head head;
    typedef Tail tail;
    typedef List Type;
    typedef TCons type;
    typedef TCons I; // inhabited
};

template<typename F, typename S>
struct TPair
{
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

TypePrinterMacro(Nil, List);

template<typename T>
struct TypePrinter<T, List>
{
    template<typename F>
    static void Print(F f)
    {
        f("TCons( ");
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
        f("TPair( ");
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
struct Length
{
    static constexpr int value = 1 + Length<Tail<List> >::value;
};

template<>
struct Length<Nil>
{
    static constexpr int value = 0;
};

// Some conversion functions

template<typename T, typename ... Args>
struct ToListAux
{
    typedef TCons<T, Essence<ToListAux<Args ...>>> type;
};

template<typename T>
struct ToListAux<T>
{
    typedef TCons<T, Nil> type;
};

template<typename ... Args>
using ToList = Essence<ToListAux<Args ...>>;

//assoc

template<typename TL, typename Type, typename Name, typename N>
struct AssocAux
{
    typedef Essence<AssocAux<Tail<Tail<TL> >, Head<Tail<TL> >, Head<TL>, N>> type;
};

template<typename TL, typename Type, typename N>
struct AssocAux<TL, Type, N, N>
{
    typedef Type type;
};

template<typename Type, typename N>
struct AssocAux<Nil, Type, N, N>
{
    typedef Type type;
};

template<typename T, typename N, typename M>
struct AssocAux<Nil, T, N, M>
{};

template<typename TL, typename N>
using Assoc = Essence<AssocAux<Tail<Tail<TL> >, Head<Tail<TL> >, Head<TL>, N>>;

// Make list

template<const int N, typename T>
struct MkListAux
{
    typedef TCons<T, Essence<MkListAux<N - 1, T>>> type;
};

template<typename T>
struct MkListAux<0, T> : Nil {};

template<const int N, typename T>
using MkList = typename MkListAux<N, T>::type;

// zip

template<typename L1, typename L2>
struct ZipAux
{
    typedef TCons<Head<L1>,
                  TCons<Head<L2>, Essence<ZipAux<Tail<L1>, Tail<L2> >>> > type;
};

template<typename L1>
struct ZipAux<L1, Nil>
{
    typedef L1 type;
};

template<typename L2>
struct ZipAux<Nil, L2>
{
    typedef L2 type;
};

template<>
struct ZipAux<Nil, Nil>
{
    typedef Nil type;
};

template<typename L1, typename L2>
using Zip = Essence<ZipAux<L1, L2>>;

// Zip3

template<typename L1, typename L2, typename L3>
struct Zip3Aux
{
    typedef TCons<Head<L1>,
                  TCons<Head<L2>,
                        TCons<Head<L3>,
                              typename Zip3Aux<Tail<L1>, Tail<L2>, Tail<L3> >::type> > >
        type;
};

template<typename L2, typename L3>
struct Zip3Aux<Nil, L2, L3>
{
    typedef Zip<L2, L3> type;
};

template<typename L1, typename L3>
struct Zip3Aux<L1, Nil, L3>
{
    typedef Zip<L1, L3> type;
};

template<typename L1, typename L2>
struct Zip3Aux<L1, L2, Nil>
{
    typedef Zip<L1, L2> type;
};

template<typename L1>
struct Zip3Aux<L1, Nil, Nil>
{
    typedef L1 type;
};

template<typename L2>
struct Zip3Aux<Nil, L2, Nil>
{
    typedef L2 type;
};

template<typename L3>
struct Zip3Aux<Nil, Nil, L3>
{
    typedef L3 type;
};

template<>
struct Zip3Aux<Nil, Nil, Nil>
{
    typedef Nil type;
};

template<typename L1, typename L2, typename L3>
using Zip3 = typename Zip3Aux<L1, L2, L3>::type;

// Filter

template<const int, template<const int, typename> typename F, typename L>
struct FilterAux2;

template<typename, const int Idx, template<const int, typename> typename F, typename L>
struct FilterAux
{
    typedef typename FilterAux2<Idx + 1, F, Tail<L> >::type type;
};

template<const int Idx, template<const int, typename> typename F, typename L>
struct FilterAux<True, Idx, F, L>
{
    typedef TCons<Head<L>, typename FilterAux2<Idx + 1, F, Tail<L> >::type> type;
};

template<const int Idx, template<const int, typename> typename F, typename L>
struct FilterAux2
{
    typedef typename FilterAux<Inhabited<typename WrapPrimitiveType<F<Idx, Head<L> > >::type>, Idx, F,
                               L>::type type;
};

template<const int Idx, template<const int, typename> typename F>
struct FilterAux2<Idx, F, Nil> : Nil {};

template<template<const int, typename> typename F, typename L>
struct FilterAux3
{
    typedef typename FilterAux2<0, F, L>::type type;
};

template<template<const int, typename> typename F, typename L>
using Filter = typename FilterAux3<F, L>::type;

// Some helpers

template<typename L, const int Period, const int Pos>
struct PeriodicalSelectAux
{
    template<const int Idx, typename>
    using P = BoolToProp<Idx % Period == Pos>;
    typedef Filter<P, L> type;
};

template<typename L, const int Period, const int Pos>
using PeriodicalSelect = typename PeriodicalSelectAux<L, Period, Pos>::type;

template<typename L>
using SelectEven = PeriodicalSelect<L, 2, 0>;

template<typename L>
using SelectOdd = PeriodicalSelect<L, 2, 1>;

template<typename L, const int Idx>
using ElementAt = PeriodicalSelect<L, Length<L>::value, Idx>;

template<typename L, const int From, const int To>
struct SliceAux
{
    template<const int Idx, typename>
    using P = BoolToProp<Idx >= From && Idx <= To>;
    typedef Filter<P, L> type;
};

template<typename L, const int From, const int To>
using Slice = Essence<SliceAux<L, From, To>>;

template<typename Elt, typename L>
struct IsPresentAux
{
    template<const int, typename N>
    using P = TypesEqual<Elt, N>;
    typedef Not<Empty<Filter<P, L> > > type;
};

template<typename Elt, typename L>
using IsPresent = Essence<IsPresentAux<Elt, L>>;

template<template<typename> typename F, typename L>
struct MapAux
{
    typedef TCons<Essence<WrapPrimitiveType<F<Head<L> > >>, Essence<MapAux<F, Tail<L> >>> type;
};

template<template<typename> typename F>
struct MapAux<F, Nil> : Nil {};

template<template<typename> typename F, typename L>
using Map = Essence<MapAux<F, L>>;

template<template<typename, typename> typename F, typename L1, typename L2>
struct Map2Aux
{
    typedef TCons<Essence<WrapPrimitiveType<F<Head<L1>, Head<L2> > >>,
                  Essence<Map2Aux<F, Tail<L1>, Tail<L2> >>> type;
};

template<template<typename, typename> typename F>
struct Map2Aux<F, Nil, Nil> : Nil {};

template<template<typename, typename> typename F, typename L1, typename L2>
using Map2 = Essence<Map2Aux<F, L1, L2>>;

template<template<typename, typename> typename F, typename V, typename L>
struct FoldLeftAux
{
    typedef Essence<F<V, Head<L> >> R;
    typedef Essence<FoldLeftAux<F, R, Tail<L> >> type;
};

template<template<typename, typename> typename F, typename V>
struct FoldLeftAux<F, V, Nil>
{
    typedef V type;
};

template<template<typename, typename> typename F, typename V, typename L>
using FoldLeft = Essence<FoldLeftAux<F, V, L>>;

template<template<typename, typename> typename F, typename L, typename V>
struct FoldRightAux
{
    typedef Essence<F<Head<L>, Essence<FoldRightAux<F, Tail<L>, V>> >> type;
};

template<template<typename, typename> typename F, typename V>
struct FoldRightAux<F, Nil, V> : V {};

template<template<typename, typename> typename F, typename L, typename V>
using FoldRight = Essence<FoldRightAux<F, L, V>>;


template<template<typename, typename> typename F, typename V, typename L>
struct FoldLeftLAux
{
    typedef Essence<F<V, L>> R;
    typedef Essence<FoldLeftLAux<F, R, Tail<L> >> type;
};

template<template<typename, typename> typename F, typename V>
struct FoldLeftLAux<F, V, Nil> : V {};

template<template<typename, typename> typename F, typename V, typename L>
using FoldLeftL = Essence<FoldLeftLAux<F, V, L>>;

template<typename L1, typename L2>
struct IsSubsetAux
{
    template<typename N>
    using P = IsPresent<N, L2>;

    typedef FoldLeft<And, True, Map<P, L1> > type;
};

template<typename L1, typename L2>
using IsSubset = Essence<IsSubsetAux<L1, L2>>;

template<typename L1, typename L2>
using IsIsomorphic = And<IsSubset<L1, L2>, IsSubset<L2, L1> >;

template<typename L1, typename L2>
using ListsEqual = FoldLeft<And, True, Map2<TypesEqual, L1, L2> >;

template<typename T, typename L>
using Intersperse = Zip<L, MkList<Length<L>::value, T> >;

template<typename L1, typename L2>
struct AppendAux
{
    typedef TCons<Head<L1>, Essence<AppendAux<Tail<L1>, L2>>> type;
};

template<typename L2>
struct AppendAux<Nil, L2> : L2 {};

template<typename L1, typename L2>
using Append = Essence<AppendAux<L1, L2>>;

template<typename T1, typename T2>
struct ProductAux
{
    template<typename N1>
    struct P1aux
    {
        template<typename N2>
        using P2 = TPair<N1, N2>;

        typedef Map<P2, T2> type;
    };

    template<typename T>
    using P1 = Essence<P1aux<T>>;

    typedef FoldLeft<Append, Nil, Map<P1, T1> > type;
};

template<typename L1, typename L2, typename = typename And<InList<L1>, InList<L2>>::I>
using Product = Essence<ProductAux<L1, L2>>;

template<typename N, typename TL, template<typename, typename> typename P = TypesEqual>
struct PresentAux
{
    template<typename T>
    using P1 = Essence<P<N, T>>;
    typedef FoldLeft<Or, False, Map<P1, TL> > type;
};

template<typename T, typename TL, template<typename, typename> typename P = TypesEqual, typename = typename InList<TL>::I>
using Present = Essence<PresentAux<T, TL, P>>;

template<typename TL, template<typename, typename> typename P>
struct UniqAux1
{
    template<typename A, typename TLst>
    using F = If<Present<Head<TLst>, Tail<TLst>, P>, A, TCons<Head<TLst>, A> >;

    typedef FoldLeftL<F, Nil, TL> type;
};

template<typename TL, template<typename, typename> typename P = TypesEqual, typename = typename InList<TL>::I>
using Uniq = Essence<UniqAux1<TL, P>>;

// Some template adapters

template<template<typename, typename> typename F>
struct SwapTemplateArgs21
{
    template<typename A, typename B>
    struct Template : F<B,A> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs321
{
    template<typename A, typename B, typename C>
    struct Template : F<C, B, A> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs213
{
    template<typename A, typename B, typename C>
    struct Template : F<B, A, C> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs132
{
    template<typename A, typename B, typename C>
    struct Template : F<A, C, B> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs312
{
    template<typename A, typename B, typename C>
    struct Template : F<C, A, B> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs231
{
    template<typename A, typename B, typename C>
    struct Template : F<B, C, A> {};
};

//Reverse

template<typename TL>
using Reverse = FoldLeft<SwapTemplateArgs21<TCons>::Template, Nil, TL>;


template<typename TL, template<typename, typename> typename P = TypesEqual>
using IsUniq = BoolToProp<Length<TL>::value == Length<Uniq<TL, P>>::value>;

template<typename TL1, typename TL2, template<typename, typename> typename P = TypesEqual>
struct IntersectAux
{
    template<const int, typename T>
    using F = Present<T, TL2, P>;

    typedef Filter<F, TL1> type;
};

template<typename TL1, typename TL2, template<typename, typename> typename P = TypesEqual,
         typename = typename And<InList<TL1>, InList<TL2>>::I>
using Intersect = Essence<IntersectAux<TL1, TL2, P>>;

template<typename TL1, typename TL2, template<typename, typename> typename P = TypesEqual>
struct SubtractAux
{
    template<const int, typename T>
    using F = Not<Present<T, TL2, P>>;
    typedef Filter<F, TL1> type;
};

template<typename TL1, typename TL2, template<typename, typename> typename P = TypesEqual,
         typename = typename And<InList<TL1>,InList<TL2>>::I>
using Subtract = Essence<SubtractAux<TL1, TL2, P>>;

} // end of namespace typelist

#endif // __TYPELIST__
