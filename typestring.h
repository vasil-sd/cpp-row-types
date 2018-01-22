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

namespace typeprint
{
using namespace typeuniverse;
TypePrinterMacro(Strings, Set);
TypePrinterMacro(ConstStrings, Set);
}


namespace typestring
{

using namespace typeuniverse;
using namespace typeprint;
using namespace typeprop;
using namespace typeprimitive;
using namespace typelist;

template<const char Str[], const unsigned int Idx = 0, const char C = 1>
struct StringToListAux
{
    static constexpr const char c = Str[Idx];
    typedef If<BoolToProp<c != 0>, TCons<cChar<c>, Essence<StringToListAux<Str, Idx + 1, c>>>, Nil> type;
//    typedef TCons<cChar<c>, Essence<StringToListAux<Str, Idx + 1, c>>> type;
};

template<const char Str[], const unsigned int Idx>
struct StringToListAux<Str,Idx,0> : Nil {};

template<const char Str[]>
using StringToList = Essence<StringToListAux<Str>>;

} // end of namespace typestring


namespace typeuniverse 
{

using namespace typeprint;
using namespace typeprop;
using namespace typeprimitive;
using namespace typelist;
using namespace typestring;

template<typename L>
struct String
{
    typedef Strings Type;
    typedef String type;
    typedef String I;

    static constexpr unsigned int length = L::cvalue;
    char value[length];
};

template<const char Str[]>
struct cString : String<cUInt<Length<StringToList<Str>>::value>>
{
    typedef String<cUInt<Length<StringToList<Str>>::value>> string;
    typedef ConstStrings Type;
    typedef cString type;
    typedef cString I;
    static constexpr const char* cvalue = Str;
    typedef StringToList<Str> char_list;

    cString()
    {
        int i = String<cUInt<Length<StringToList<Str>>::value>>::length-1;
        do
        {
            String<cUInt<Length<StringToList<Str>>::value>>::value[i] = cvalue[i];
        }while(i--);
    }
};

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
        f(T::cvalue);
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
