#ifndef __TYPECOERCION__
#define __TYPECOERCION__

#include "typeuniverse.h"
#include "typeprop.h"
#include "typemorphism.h"
#include "typeprimitive.h"

/*
 * TODO: make generic typerel, make closure finding in it
 * and rebase typecoercion on typerel
 */

namespace typecoercion
{

using namespace typeuniverse;
using namespace typeprop;
using namespace typelist;

template<template<typename, typename> typename C, typename TLfrom, typename TLto = TLfrom>
struct SelectCoercionsAux
{
    typedef Product<TLfrom, TLto> prod; // get all types combination

    template<typename P>
    using F = If<Inhabited<typename C<First<P>, Second<P> >::type>,
                 True,
                 False>;

    typedef Filter<F, prod> coercion_pairs;

    template<typename A, typename B>
    using Coerce = typename C<A, B>::type;

    typedef Map2<Coerce, Map<First, coercion_pairs>, Map<Second, coercion_pairs> > type;
};

template<typename TL, template<typename, typename> typename CL, template<typename, typename> typename ... CLs>
struct SelectCoercionsAux2
{
    typedef  Append<typename SelectCoercionsAux<CL, TL, TL>::type,
                    typename SelectCoercionsAux2<TL, CLs ...>::type> type;
};

template<typename TL, template<typename, typename> typename CL>
struct SelectCoercionsAux2<TL, CL>
{
    typedef typename SelectCoercionsAux<CL, TL, TL>::type type;
};

template<typename TL, template<typename, typename> typename ... CLs>
struct SelectCoercionsAux3
{
    template<typename F1, typename F2>
    using P = And<TypesEqual<typename F1::from, typename F2::from>, TypesEqual<typename F1::to, typename F2::to> >;
    typedef typename SelectCoercionsAux2<TL, CLs ...>::type functors;
    typedef Uniq<functors, P> type;
};

template<typename TL, template<typename, typename> typename ... CLs>
using SelectCoercions = typename SelectCoercionsAux3<TL, CLs ...>::type;

/*
 * [[A, From, To, Fs]]
 * ->
 * [A + {x : FS | x::from == From}}
 */

template<typename A, typename From, typename To, typename Fs>
struct FindCoercionAux
{
    template<typename F>
    using P = TypesEqual<From, typename F::from>;

    template<typename F>
    using NP = Not<TypesEqual<From, typename F::from> >;

    template<typename F>
    using TO = typename F::to;

    typedef Filter<P, Fs> selected;

    template<typename F>
    struct C
    {
        template<typename F1>
        using Exclude
                  = Not<And<TypesEqual<typename F::from, typename F1::from>,
                            TypesEqual<typename F::to, typename F1::to> > >;
        typedef ToList<TCons<F, A>, TO<F>, To, Filter<Exclude, Fs> > type;
    };

    typedef Map<C, selected> type;
};

template<typename A, typename From, typename Fs>
struct FindCoercionAux<A, From, From, Fs>
{
    typedef A type;
};

template<typename A, typename P = False>
struct FindCoercionAux2
{
    typedef Head<Head<A> > type;
};

template<typename A>
struct FindCoercionAux1
{

    template<typename D>
    using Step
              = typename FindCoercionAux<Head<D>, Head<Tail<D> >, Head<Tail<Tail<D> > >,
                                         Head<Tail<Tail<Tail<D> > > > >::type;

    template<typename L>
    using NE = Not<Empty<L> >;

    typedef Filter<NE, FoldLeft<Append, Nil, Map<Step, A> > > new_A;

    template<typename D>
    using Goal = TypesEqual<Head<Tail<D> >, Head<Tail<Tail<D> > > >;

    typedef Filter<Goal, new_A> goals;

    typedef typename FindCoercionAux2<new_A, Empty<goals> >::type type;
};

template<typename A>
struct FindCoercionAux2<A, True>
{
    typedef typename FindCoercionAux1<A>::type type;
};

template<>
struct FindCoercionAux2<Nil, True>
{
    typedef Nil type;
};

template<>
struct FindCoercionAux1<Nil>
{
    typedef Nil type;
};

template<typename From, typename To, typename TL, template<typename, typename> typename ... CLs>
using FindCoercionAux3
          = typename FindCoercionAux1<ToList<ToList<
                                                 Nil, WrapPrimitiveType<From>, WrapPrimitiveType<To>,
                                                 SelectCoercions<Map<WrapPrimitiveType,
                                                                     Uniq<TCons<From, TCons<To, TL> > > >,
                                                                 CLs ...> > > >::type;

template<typename F1, typename F2, typename = TypesEqual<typename F1::to, typename F2::from> >
struct ComposeAux
{
    struct Coerce
    {
        static void convert(const typename F1::from& from,
                            typename F2::to        & to)
        {
            typename F1::to tmp;
            F1::functor::convert(from, tmp);
            F2::functor::convert(tmp, to);
        };
    };

    typedef Functor<Coerce, typename F1::from, typename F2::to> type;
};

template<typename F1, typename F2>
using Compose = typename ComposeAux<F1, F2>::type;

template<typename C, typename CL>
struct CloseCompositionsAux
{
    typedef typename CloseCompositionsAux<Compose<C, Head<CL> >, Tail<CL> >::type type;
};

template<typename C>
struct CloseCompositionsAux<C, Nil>
{
    typedef C type;
};

template<typename CL, typename l = Length<CL>>
struct CloseCompositionsAux1
{
    typedef typename CloseCompositionsAux<Head<CL>, Tail<CL> >::type type;
};

template<>
struct CloseCompositionsAux1<Nil, cInt<0>>: False {};

template<typename CL>
struct CloseCompositionsAux1<CL, cInt<1>>
{
    typedef Head<CL> type;
};

template<typename CL>
using CloseCompositions = typename CloseCompositionsAux1<CL>::type;

template<typename From, typename To, typename TL, template<typename, typename> typename ... CLs>
struct FindCoercionAux4
{
    typedef Reverse<FindCoercionAux3<From, To, TL, CLs ...> > closure;
    typedef CloseCompositions<closure> type;
};

template<typename From, typename To, typename TL, template<typename, typename> typename ... CLs>
using FindCoercion = typename FindCoercionAux4<From, To, TL, CLs ...>::type;

} // end of namespace typecoercion

namespace typeprimitive
{

using namespace typeuniverse;
using namespace typeprimitive;
/*
 * Coercion of primitive integral types
 * TODO: make check of integral type sizes, sometimes int = long, sometimes not
 */
struct StaticCast
{
    template<typename From, typename To>
    struct Coerce : False {}; // no default coercion
};

#define StaticCastCoerceMacro(From, To) \
    template<> \
    struct StaticCast::Coerce<WrapPrimitiveType<From>, \
                              WrapPrimitiveType<To> >: Functor<StaticCast::Coerce<WrapPrimitiveType<From>, \
                                                                                  WrapPrimitiveType<To> >, \
                                                               WrapPrimitiveType<From>, WrapPrimitiveType<To> > \
    { \
        static void convert(const WrapPrimitiveType<From>&from, WrapPrimitiveType<To>&to) \
        { \
            to.value = static_cast<typename WrapPrimitiveType<To>::primitive_type>(from.value); \
        } \
    }

StaticCastCoerceMacro(char, short);
StaticCastCoerceMacro(unsigned char, short);
StaticCastCoerceMacro(short, int);
StaticCastCoerceMacro(unsigned short, int);

} // end of namespace typeprimitive

#endif // __TYPECOERCION__
