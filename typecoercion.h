#ifndef __TYPECOERCION__
#define __TYPECOERCION__

#include "typeuniverse.h"
#include "typeprop.h"
#include "typemorphism.h"
#include "typeprimitive.h"
#include "typerelation.h"

namespace typecoercion
{

using namespace typeuniverse;
using namespace typeprop;
using namespace typelist;
using namespace typerelation;

template<template<typename...> typename C, typename TLfrom, typename TLto = TLfrom>
using GetAllCoercions = CloseRelation<Rel<Filter<TemplateComposition<Inhabited, Essence, CloseTemplateTemplateArg<ApplyToPair,C>::template T>::template T, Product<TLfrom, TLto>>>>;

template<typename F1, typename F2>
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
using Compose = Essence<ComposeAux<F1, F2>>;

template<typename CL>
using CloseCompositions = Essence<FoldLeft<Compose, Head<CL>, Tail<CL>>>;

template<typename Clo, typename TL, template<typename...> typename C>
struct FindCoercionAux
{
    template<typename RP>
    using F = TypesEqual<typename RP::closure, Clo>;

    typedef Head<Filter<F, GetAllCoercions<C, TL>>> rpath;

    typedef Map2P<C, typename rpath::path> coercions;

    typedef CloseCompositions<coercions> type;
};


template<typename From, typename To, typename TL, template<typename, typename> typename C>
using FindCoercion = Essence<FindCoercionAux<TPair<From, To>, TL, C>>;

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
