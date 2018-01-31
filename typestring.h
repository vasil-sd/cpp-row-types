#ifndef __TYPESTRING__
#define __TYPESTRING__

#include "typeuniverse.h"
#include "typeprint.h"
#include "typeprop.h"
#include "typeprimitive.h"
#include "typelist.h"
#include "string.h"

namespace typeuniverse
{
DefType(Strings, Set);
TypeDiscriminator(Strings);
DefType(ConstStrings, Set);
TypeDiscriminator(ConstStrings);
}

namespace typestring
{

using namespace typeuniverse;
using namespace typeprint;
using namespace typeprop;
using namespace typeprimitive;
using namespace typelist;

template<const unsigned char Str[], const unsigned int Idx = 0, const unsigned char C = Str[0]>
struct CStringToCharListAux :If<BoolToProp<Str[Idx] != 0>, TCons<cUChar<Str[Idx]>, Essence<CStringToCharListAux<Str, Idx + 1, Str[Idx]>>>, Nil> {};

template<const unsigned char Str[], const unsigned int Idx>
struct CStringToCharListAux<Str,Idx,0> : Nil {};

template<const unsigned char Str[]>
using CStringToCharList = Essence<CStringToCharListAux<Str>>;

template<typename TL, typename Len, typename ...Args>
struct CharListToCStringAux : CharListToCStringAux<Tail<TL>, Len, Args..., Head<TL>> {};

template<typename Len, typename ...Args>
struct CharListToCStringAux<Nil, Len, Args...>
{
    static constexpr const unsigned char cvalue[Len::cvalue+1] = {Args::cvalue..., '\0'};
};

template<typename TL>
using IsCharList = FoldLeft<And, True, Map<IncUChar, TL>>;

template<typename TL,
         typename = typename IsCharList<TL>::I> 
using CharListToCString = CharListToCStringAux<TL, Length<TL>>;

} // end of namespace typestring


namespace typeuniverse 
{

using namespace typeprint;
using namespace typeprop;
using namespace typeprimitive;
using namespace typelist;
using namespace typestring;

DefTypeSymbol(String);

template<typename L, typename = typename IsIntegralType<L>::I>
struct String
{
    typedef String_Symbol type_name;
    typedef Strings Type;
    typedef String type;
    typedef String I;

    static constexpr const unsigned int length = L::cvalue;
    unsigned char value[length];

    template <typename nL>
    using type_template = String<nL>;
};

DefTypeSymbol(ConstString);

template<typename CL, typename = typename IsCharList<CL>::I>
struct cString : String<Length<CL>>
{
    typedef ConstString_Symbol type_name;
    typedef String<Length<CL>> string;
    typedef ConstStrings Type;
    typedef cString type;
    typedef cString I;
    static constexpr const unsigned char* cvalue = CharListToCString<CL>::cvalue;
    typedef CL char_list;

    cString()
    {
        int i = String<Length<CL>>::length-1;
        do
        {
            String<Length<CL>>::value[i] = cvalue[i];
        }while(i--);
    }

    template <typename nCL>
    using type_template = cString<nCL>;
};

} // end of namespace typeuniverse


namespace typestring
{

using namespace typeuniverse;
using namespace typeprint;
using namespace typeprop;
using namespace typeprimitive;
using namespace typelist;

template<typename S1, typename S2, typename = typename IsListElementsOfType<ToList<S1,S2>, ConstStrings>::I>
using StringAppend = cString<Append<typename S1::char_list, typename S2::char_list>>;

template<typename SL, typename = typename IsListElementsOfType<SL, ConstStrings>::I>
using AppendStrings = FoldLeft<StringAppend, Head<SL>, Tail<SL>>;

} // end of namespace typestring

namespace typeprint
{

using namespace typeuniverse;
using namespace typeprint;
using namespace typeprop;
using namespace typestring;

template<typename T>
struct TypePrinter<T, Strings>
{
    static void to_string(char* s, unsigned int v)
    {
        int l = 0; int i = 0;
        do { s[i++] = v%10 + '0'; v /= 10; }while(v);
        s[i] = 0; i--; l = i; i /= 2;
        do { char t = s[l-i]; s[l-i] = s[i]; s[i] = t;}while(i--);
    };

    template<typename F>
    static void Print(F f)
    {
        char s[32];
        f("String{");
        to_string(s, T::length);
        f(s);
        f("}");
    }

    template<typename F>
    static void print(const T& o, F f)
    {
        char s[32];
        Print(f);
        f("<\"");
        f(o.value);
        f("\">");
    }
};

template<typename T>
struct TypePrinter<T, ConstStrings>
{
    static void to_string(char* s, unsigned int v)
    {
        int l = 0; int i = 0;
        do { s[i++] = v%10 + '0'; v /= 10; }while(v);
        s[i] = 0; i--; l = i; i /= 2;
        do { char t = s[l-i]; s[l-i] = s[i]; s[i] = t;}while(i--);
    };

    template<typename F>
    static void Print(F f)
    {
        char s[32];
        f("ConstString{");
        to_string(s, T::length);
        f(s);
        f("}");
        f("(\"");
        f(reinterpret_cast<const char *>(T::cvalue));
        f("\")");
    }

    template<typename F>
    static void print(const T& o, F f)
    {
        char s[32];
        Print(f);
        f("<\"");
        f(o.value);
        f("\">");
    }
};


}


#endif // __TYPESTRING__
