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
        f("<unknown>");
    }

};


#define TypePrinterMacro(T, ST) \
    template<> \
    struct TypePrinter<T, ST> \
    { \
        template<typename F> \
        static void Print(F f) \
        { \
            f(# T); \
        } \
        template<typename F> \
        static void print(F f, const T& o) \
        { \
            f(# T); \
        } \
    }

TypePrinterMacro(UU, UU);
TypePrinterMacro(Unknown, UU);
TypePrinterMacro(Prop, UU);
TypePrinterMacro(Set, UU);
TypePrinterMacro(Symbol, UU);

template<typename S>
struct TypePrinter<S, Symbol>
{
    template<typename F>
    static void Print(F f)
    {
        f(S::value);
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
