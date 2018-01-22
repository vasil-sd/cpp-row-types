#ifndef __TYPEUTILS__
#define __TYPEUTILS__

#include "typeuniverse.h"
#include "typeprint.h"
#include "typeprop.h"
#include "typeprimitive.h"
#include "typelist.h"

namespace typeutils
{

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

template<template<typename> typename F>
struct SelectTemplateArg1of2
{
    template<typename A, typename B>
    struct Template : F<A> {};
};

template<template<typename> typename F>
struct SelectTemplateArg2of2
{
    template<typename A, typename B>
    struct Template : F<B> {};
};

} // end of namespace typeutils

#endif // __TYPEUTILS__
