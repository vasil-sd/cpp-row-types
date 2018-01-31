#ifndef __TYPEPRIMITIVE__
#define __TYPEPRIMITIVE__

#include "typeuniverse.h"
#include "typeprop.h"
#include "typeprint.h"

namespace typeuniverse
{

using namespace typeprop;

DefType(Integral, Set);
TypeDiscriminator(Integral);

template<typename T>
using IsIntegralType = Or<InIntegral<T>, InIntegral<TypeOf<T>>>;

#define DefTypePrimitive(Prim, T) \
    DefTypeSymbol(T);\
    struct T \
    { \
        typedef T##_Symbol type_name; \
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
    }; \
    DefTypeSymbol(c##T); \
    template<const Prim Val> \
    struct c##T : T \
    { \
        typedef c##T##_Symbol type_name; \
        typedef T Type; \
        static constexpr Prim cvalue = Val; \
        typedef c##T type; \
        template<const Prim Arg> \
        using type_template = c##T<Arg>; \
        c##T() : T(cvalue) {} \
        c##T(const primitive_type &v) : T(value) {} \
        c##T(const type &v) : T(v.value) {} \
    }

} // end of namespace

namespace typeprint
{

using namespace typeuniverse;

#define TypePrimitivePrinterMacro(T) \
    template<> \
    struct TypePrinter<T, Integral> \
    { \
        template<typename V> \
        static void to_string(char* s, V v) \
        { \
            int l = 0; int i = 0; \
            do { s[i++] = v%10 + '0'; v /= 10; }while(v); \
            s[i] = 0; i--; l = i; i /= 2; \
            do { char t = s[l-i]; s[l-i] = s[i]; s[i] = t;}while(i--); \
        }; \
        template<typename F> \
        static void Print(F f) \
        { \
            f(# T); \
        } \
        template<typename F> \
        static void print(const T& o, F f) \
        { \
            char str[32]; \
            f(#T "<"); \
            to_string(str, o.value); f(str); \
            f(">"); \
        } \
    }; \
    template<typename cT> \
    struct TypePrinter<cT, T> \
    { \
        template<typename V> \
        static void to_string(char* s, V v) \
        { \
            int l = 0; int i = 0; \
            do { s[i++] = v%10 + '0'; v /= 10; }while(v); \
            s[i] = 0; i--; l = i; i /= 2; \
            do { char t = s[l-i]; s[l-i] = s[i]; s[i] = t;}while(i--); \
        }; \
        template<typename F> \
        static void Print(F f) \
        { \
            char str[32]; \
            f("c" # T); \
            f("("); \
            to_string(str, cT::cvalue); f(str); \
            f(")"); \
        } \
        template<typename F> \
        static void print(const cT& o, F f) \
        { \
            char str[32]; \
            f("c" #T "<"); \
            to_string(str, o.value); f(str); \
            f("("); \
            to_string(str, cT::cvalue); f(str); \
            f(")"); \
            f(">"); \
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
        TypePrinter<Essence<WrapPrimitiveType<T>>, typename WrapPrimitiveType<T>::Type>::template Print(f);
    };
};
}

#define ConstTypeDiscriminator(T) \
template<typename C, typename ST = typename C::Type> \
struct Inc##T##Aux : False {}; \
template<typename C> \
struct Inc##T##Aux<C, T> : True {}; \
template<typename C> \
using Inc##T = Essence<Inc##T##Aux<C>>

#define DefPrimitiveType(Prim, T) \
    namespace typeuniverse { DefTypePrimitive(Prim, T); } \
    namespace typeprint { using namespace typeuniverse; TypePrimitivePrinterMacro(T); } \
    namespace typeprimitive { using namespace typeuniverse; using namespace typeprint; TypePrimitiveWrapperMacro( \
                                  Prim, \
                                  T); \
                              TypeDiscriminator(T); \
                              ConstTypeDiscriminator(T);}

//Here actual definitions
DefPrimitiveType(int, Int);
DefPrimitiveType(unsigned int, UInt);
DefPrimitiveType(char, Char);
DefPrimitiveType(unsigned char, UChar);
DefPrimitiveType(short, Short);
DefPrimitiveType(unsigned short, UShort);

#endif // __TYPEPRIMITIVE__
