#include "typeprop.h"
#include "typelist.h"
#include "typemorphism.h"
#include "typecoercion.h"
#include "typerecord.h"
#include "typestring.h"
#include "typeprimitivemath.h"

using namespace typeuniverse;
using namespace typeprop;
using namespace typelist;
using namespace typecoercion;
using namespace typerecord;
using namespace typestring;

DefSymbol(N1);
DefSymbol(N2);
DefSymbol(N3);
DefSymbol(N4);
DefSymbol(N5);
DefSymbol(N6);
DefSymbol(N7);
DefSymbol(N8);
DefSymbol(N9);
DefSymbol(N10);
DefSymbol(N11);
DefSymbol(N12);
DefSymbol(Next);
DefSymbol(Str);

typedef Record<N2, char> rec1;
typedef Record<Next, cChar<34>> rec2;
constexpr const unsigned char teststr[] = "Test String!";

typedef Record<N1, rec1, N2, int, Str, cString<CStringToCharList<teststr>>> rec;
struct M1 : Functor<M1, N1, N2>{};

typedef Assoc<N1, ToList<N2, int, N1, char>> t;

template<typename T>
using P = Not<TypesEqual<N2, T> >;

template<typename Idx, typename>
struct P1 : BoolToProp<Idx::cvalue % 3 == 0>{};

/*
Code to calculate 'uniq' id of record, and add it as a filed.
This can help to identify records in runtime.
*/


template<typename Chars>
struct CalcCharListHash
{
    template<typename Sum, typename V>
    using P = TPair<cUInt<Add<V, Mul<First<Sum>, Add<Second<Sum>, V>>>::cvalue&0xFFFFFFFF>, cUInt<Mul<Second<Sum>, cUInt<13>>::cvalue&0xFFFFFFFF >>;
    typedef First<FoldLeft<P, TPair<cUInt<0x77331155UL>, cUInt<0x55AACCBBUL>>, Chars>> type;
};

template<typename R>
struct CalcRecordFieldNamesHashAux
{
    template<typename Sym>
    using SymToChars = CStringToCharList<Sym::cvalue>;
    typedef Essence<CalcCharListHash<FoldLeft<Append, Nil, Map<SymToChars, FieldNames<R>>>>> type;
};

template<typename R>
using CalcRecordFieldNamesHash = Essence<CalcRecordFieldNamesHashAux<R>>;

DefSymbol(Id);

template<typename R>
using GenerateId = UnionRecords<Record<Id, CalcRecordFieldNamesHash<R>>, R>;


//template<typename R>
//using GenerateId = R;

rec r;
rec1 r1;
rec2 r2;
auto r3 = r1 + r2 + Record<N3, UInt>();
auto r4 = r - r1  + Record<N4, UInt>();
auto r5 = r4 + r1 + Record<N5, UInt>();
auto r6 = r - r4  + Record<N6, UInt>();
auto r7 = r3 - r1 + Record<N7, UInt>();
auto r8 = r7 + r1 + Record<N8, UInt>();


typedef GenerateId<rec> new_rec;
typedef GenerateId<decltype(r1)> new_r1;
typedef GenerateId<decltype(r2)> new_r2;
typedef GenerateId<decltype(r3)> new_r3;
typedef GenerateId<decltype(r4)> new_r4;
typedef GenerateId<decltype(r5)> new_r5;
typedef GenerateId<decltype(r6)> new_r6;
typedef GenerateId<decltype(r7)> new_r7;
typedef GenerateId<decltype(r8)> new_r8;


