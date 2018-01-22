#ifndef __TYPEPRINT__
#define __TYPEPRINT__

#include "typeuniverse.h"

namespace typeprint
{

using namespace typeuniverse;

template<typename T, typename ST = typename T::Type>
struct TypePrinter
{
    template<typename F>
    static void Print(F f)
    {
        f(T::type_name::value);
    }
    template<typename F>
    static void print(F f, const T& o)
    {
        f("value_of(");
        f(T::type_name::value);
        f(")");
    }
};

template<typename T>
struct PrintFullTypeOf
{
    template<typename F>
    static void Print(F f)
    {
        TypePrinter<typename T::type, typename T::Type>::template Print(f);
        f(" : ");
        PrintFullTypeOf<typename T::Type>::template Print(f);
    };
};

template<>
struct PrintFullTypeOf<UU>
{
    template<typename F>
    static void Print(F f)
    {
        f("UU");
    };
};

} // end of namespace typeprint

#endif // __TYPEPRINT__
