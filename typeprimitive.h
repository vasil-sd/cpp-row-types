#ifndef __TYPEPRIMITIVE__
#define __TYPEPRIMITIVE__

#include "typeuniverse.h"
#include "typeprop.h"
#include "typeprint.h"

namespace typeuniverse
{

DefType(Integral, Set);

#define DefTypePrimitive(Prim, T) \
    struct T \
    { \
        typedef Integral Type; \
        typedef T type; \
        typedef Prim primitive_type; \
        typedef T I; \
        primitive_type value; \
        T() = default; \
        T(const primitive_type &v) : value(v) {} \
        T(const type &v) : value(v.value) {} \
        operator primitive_type&() {return value;} \
        T& operator = (const primitive_type& v) \
        { \
           value = v; \
           return *this; \
        } \
        T& operator = (const T& v) \
        { \
           value = v.value; \
           return *this; \
        } \
    }



} // end of namespace

namespace typeprint
{

using namespace typeuniverse;

TypePrinterMacro(Integral, Set);

#define TypePrimitivePrinterMacro(T) \
    template<> \
    struct TypePrinter<T, Integral> \
    { \
        template<typename F> \
        static void Print(F f) \
        { \
            f(# T); \
        } \
        template<typename F> \
        static void print(const T& o, F f) \
        { \
            f(# T); \
        } \
    };

} // end of namespace

namespace typeprimitive
{

using namespace typeuniverse;
using namespace typeprint;
using namespace typeprop;

// Wrapper for primitive types
// it returns either original type or wrapped primitive type

template<typename T, typename Pt>
struct WrapPrimitiveTypeAux : Unknown {};

template<typename T>
struct WrapPrimitiveTypeAux<T, True>: T {};

#define TypePrimitiveWrapperMacro(Prim, T) \
    template<> \
    struct WrapPrimitiveTypeAux<Prim, False>: T {}

template<typename T>
using WrapPrimitiveType = typename WrapPrimitiveTypeAux<T, IsType<T> >::type;

} // end of namespace typesymbols

namespace typeprint
{
using namespace typeuniverse;
using namespace typeprint;
using namespace typeprimitive;

// TPrinter is a wrapper to print ordinary, or user types
template<typename T>
struct TPrinter
{
    template<typename F>
    static void Print(F f)
    {
        TypePrinter<typename WrapPrimitiveType<T>::type, typename WrapPrimitiveType<T>::Type>::template Print(f);
    };
};
}

#define DefPrimitiveType(Prim, T) \
    namespace typeuniverse { DefTypePrimitive(Prim, T); } \
    namespace typeprint { using namespace typeuniverse; TypePrimitivePrinterMacro(T); } \
    namespace typeprimitive { using namespace typeuniverse; using namespace typeprint; TypePrimitiveWrapperMacro( \
                                  Prim, \
                                  T); }

//Here actual definitions
DefPrimitiveType(int, Int);
DefPrimitiveType(unsigned int, UInt);
DefPrimitiveType(char, Char);
DefPrimitiveType(unsigned char, UChar);
DefPrimitiveType(short, Short);
DefPrimitiveType(unsigned short, UShort);

#endif // __TYPEPRIMITIVE__
