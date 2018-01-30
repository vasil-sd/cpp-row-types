#ifndef __TYPEUTILS__
#define __TYPEUTILS__

#include "typeuniverse.h"
#include "typeprint.h"
#include "typeprop.h"
#include "typeprimitive.h"
#include "typelist.h"

namespace typeutils
{

using namespace typeuniverse;

// Some template adapters

template<template<typename...> typename F>
struct SwapTemplateArgs21
{
    template<typename A, typename B>
    struct T : F<B,A> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs321
{
    template<typename A, typename B, typename C>
    struct T : F<C, B, A> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs213
{
    template<typename A, typename B, typename C>
    struct T : F<B, A, C> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs132
{
    template<typename A, typename B, typename C>
    struct T : F<A, C, B> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs312
{
    template<typename A, typename B, typename C>
    struct T : F<C, A, B> {};
};

template<template<typename, typename, typename> typename F>
struct SwapTemplateArgs231
{
    template<typename A, typename B, typename C>
    struct T : F<B, C, A> {};
};

template<template<typename> typename F>
struct SelectTemplateArg1of2
{
    template<typename A, typename B>
    using T = F<A>;
};

template<template<typename> typename F>
struct SelectTemplateArg2of2
{
    template<typename A, typename B>
    using T = F<B>;
};

template<template<template<typename...>typename, typename...> typename OriginalT, template<typename...> typename CapturedT>
struct CloseTemplateTemplateArg {
    template<typename...Args>
    struct T : OriginalT<CapturedT, Args...> {};
};

template<template<typename...> typename OriginalT, typename ...CapturedArgs>
struct CloseTemplate {
    template<typename...Args>
    struct T : OriginalT<CapturedArgs..., Args...> {};
};

template<template<typename...> typename OriginalT, typename ...CapturedArgs>
struct CloseTemplateLastArgs {
    template<typename...Args>
    struct T : OriginalT<Args..., CapturedArgs...> {};
};


template<template<typename...> typename Temp, template<typename...> typename ...Temps>
struct TemplateComposition
{
    template<typename ...Args>
    using T = Essence<Temp<Essence<typename TemplateComposition<Temps...>::template T<Args...>>>>;
};

template<template<typename...> typename Temp>
struct TemplateComposition<Temp>
{
    template<typename ...Args>
    struct T : Essence<Temp<Args...>> {};
};

template<template<typename...> typename Temp, template<typename...> typename P1, template<typename...> typename P2>
struct TemplatePreprocessArgs
{
    template<typename A1, typename A2>//, typename ...Args>
    using T = Essence <Temp<P1<A1>, P2<A2>/*, Args...*/>>;// {};
};

} // end of namespace typeutils

#endif // __TYPEUTILS__
