#ifndef __TYPEMORPHISM__
#define __TYPEMORPHISM__

#include "typeuniverse.h"
#include "typeprint.h"

namespace typeuniverse
{
DefType(Morphism, Set);

DefSymbol(Functor_Symbol);

template<typename C, typename From, typename To>
struct Functor
{
    typedef Functor_Symbol type_name;
    typedef Morphism Type;
    typedef Functor type;
    typedef From from;
    typedef To to;
    typedef C functor;
    typedef Functor I;
};

}

namespace typeprint
{

using namespace typeuniverse;

TypePrinterMacro(Morphism, Set);

template<typename Functor>
struct TypePrinter<Functor, Morphism>
{
    template<typename F>
    static void Print(F f)
    {
        f("Functor( ");
        TPrinter<typename Functor::from>::Print(f);
        f(" -> ");
        TPrinter<typename Functor::to>::Print(f);
        f(" )");
    }

};

}

#endif // __TYPEMORPHISM__
