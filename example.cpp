#include "typeprop.h"
#include "typelist.h"
#include "typemorphism.h"
#include "typecoercion.h"
#include "typerecord.h"
#include "typestring.h"

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

constexpr const char teststr[] = "Test String!";

typedef Record<N1, rec1, N2, int, Str, cString<teststr>> rec;

struct M1 : Functor<M1, N1, N2>{};

#include <stdio.h>

typedef Assoc<ToList<N2, int, N1, char>, N1> t;

template<typename T>
using P = Not<TypesEqual<N2, T> >;

template<typename Idx, typename>
struct P1 : BoolToProp<Idx::cvalue % 3 == 0>{};

template <typename R, typename P = FieldsPresent<R, ToList<N1>>>
void PP (R& r)
{
    TPrinter<P>::Print(printf);
}

/*
Code to calculate 'uniq' id of record, and add it as a filed.
This can help to identify records in runtime.
*/

template<typename Chars>
struct CalcCharListHash
{
    template<typename Sum, typename V>
    using P = TPair<cUInt<(V::cvalue + First<Sum>::cvalue * (Second<Sum>::cvalue + V::cvalue))&0xFFFFFFFF>, cUInt<(Second<Sum>::cvalue * 13)&0xFFFFFFFF >>;
    typedef First<FoldLeft<P, TPair<cUInt<0x77331155UL>, cUInt<0x55AACCBBUL>>, Chars>> type;
};

template<typename R>
struct CalcRecordFieldNamesHashAux
{
    template<typename Sym>
    using SymToChars = StringToList<Sym::value>;
    typedef Essence<CalcCharListHash<FoldLeft<Append, Nil, Map<SymToChars, FieldNames<R>>>>> type;
};

template<typename R>
using CalcRecordFieldNamesHash = Essence<CalcRecordFieldNamesHashAux<R>>;

DefSymbol(Id);

template<typename R>
using GenerateId = UnionRecords<Record<Id, cUInt<CalcRecordFieldNamesHash<R>::cvalue>>, R>;

int
main()
{
    printf("%d, %d\n", ValuesEqual<True, False>::value, ValuesEqual<True, True>::value);
    printf("%d, %d\n", TypesEqual<int, int>::value, TypesEqual<char, int>::value);
    printf("%d %d\n", Inhabited<True>::value, Inhabited<False>::value);
    printf("%d\n", Length<TCons<int, TCons<char, Nil> > >::value);
    printf("%d\n", Length<ToList<int, char, short, int> >::value);
//    Abc abc;

    rec r;
    Field<ToList<N2>>::Access(Field<ToList<N1>>::Access(r)) = 22;
    
    printf("Field: %d\n", Field<ToList<N1,N2>>::Access(r).value);
    printf("Field: %d\n", FieldAccess<rec, N1,N2>(r).value);
    printf("Field: %d\n", FieldAccess1<N1,N2>::Access(r).value);
    Field<ToList<N2>>::Access(r) = 0;
    
    printf("FieldPP:\n");
    PP(r);
    printf("FieldPP:\n");
    PP(FieldAccess<rec,N1>(r));
    typedef ToList<N1, N1, N2, N2, N1, int> ll;
    typedef Filter<P, ll> pl;
    typedef FilterN<P1, ll> pl1;
    printf("%d\n", Length<pl>::value);
    printf("%d\n", Length<pl1>::value);
    printf("%d\n", Length<SelectEven<ll> >::value);
    printf("%d\n", Length<SelectOdd<ll> >::value);
    printf("%d\n", IsPresent<Next, ll>::value);
    printf("FOLD: %d\n", FoldLeft<And, True, ToList<True, False, True> >::value);
    printf("SUBSET: %d\n", IsSubset<ToList<False, False, N1>, ToList<N1, True, False, True> >::value);
    printf("LISTSEQ: %d\n", ListsEqual<ToList<False, False, N1>, ToList<False, False, N2> >::value);
    TPrinter<ll>::Print(printf);
    printf("\n");
    PrintFullTypeOf<Next>::Print(printf);
    printf("\n");
    PrintFullTypeOf<True>::Print(printf);
    printf("\n");
    printf("\n");
    TPrinter<M1>::Print(printf);
    printf("\n");
    TPrinter<SelectCoercions<ToList<unsigned char, unsigned short, unsigned int, int, char, N1, short, N2>,
                             StaticCast::Coerce, StaticCast::Coerce, StaticCast::Coerce> >::Print(printf);
    printf("\n");
    TPrinter<Assoc<ToList<N1, char>, N1> >::Print(printf);
    printf("\n");
    TPrinter<Present<Next, ll> >::Print(printf);
    printf("\n");
    TPrinter<Present<int, ToList<Nil, int> > >::Print(printf);
    printf("\n");
    TPrinter<Uniq<Nil> >::Print(printf);
    printf("\n-------\n");
    typedef FindCoercion<unsigned char, int, ToList<char, int, short, unsigned int, Next, N1>,
                         StaticCast::Coerce> coercion;
    TPrinter<coercion>::Print(printf);
    WrapPrimitiveType<unsigned char> c(123);
    WrapPrimitiveType<int> cc;
    cc = cc + 15;
    printf("\nCoercion: %d\n", (coercion::functor::convert(c, cc), cc.value));
    printf("\nREVERSE-------\n");
    TPrinter<Reverse<ll> >::Print(printf);
    printf("\n");
    TPrinter<rec>::Print(printf);
    printf("\n-------\n");
    TPrinter<IsUniq<Uniq<ll>>>::Print(printf);
    printf("\n");
    printf("\n-------\n");
    TPrinter<InSet<Records>>::Print(printf);
    printf("\n");
    printf("\n");
    TPrinter<UnionRecords<rec1,rec2>>::Print(printf);
    printf("\n-------\n");
    printf("\n");
    TPrinter<IntersectRecords<rec1,rec>>::Print(printf);
    printf("\n-------\n");
    printf("\n-------\n");
    TPrinter<RecordFieldsList<rec>>::Print(printf);
    printf("\n");
    
    rec1 r1;
    rec2 r2;
    
    printf("r2.Next = %d", FieldAccess<rec2, Next>(r2).value);
    
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

    printf("\n r3 = r1 + r2\n");
    printf("r1 = "); TPrinter<decltype(r1)>::Print(printf); printf("\n");
    printf("r2 = "); TPrinter<decltype(r2)>::Print(printf); printf("\n");
    printf("r3 = "); TPrinter<decltype(r3)>::Print(printf); printf("\n");
    printf("\n");
    printf("\n r4 = r - r1\n");
    printf("r = "); TPrinter<decltype(r)>::Print(printf); printf("\n");
    printf("r1 = "); TPrinter<decltype(r1)>::Print(printf); printf("\n");
    printf("r4 = "); TPrinter<decltype(r4)>::Print(printf); printf("\n");
    printf("new_rec = "); TPrinter<new_rec>::Print(printf); printf("\n");
    printf("new_r1 = "); TPrinter<new_r1>::Print(printf); printf("\n");
    printf("new_r2 = "); TPrinter<new_r2>::Print(printf); printf("\n");
    printf("new_r3 = "); TPrinter<new_r3>::Print(printf); printf("\n");
    printf("new_r4 = "); TPrinter<new_r4>::Print(printf); printf("\n");
    printf("new_r5 = "); TPrinter<new_r5>::Print(printf); printf("\n");
    printf("new_r6 = "); TPrinter<new_r6>::Print(printf); printf("\n");
    printf("new_r7 = "); TPrinter<new_r7>::Print(printf); printf("\n");
    printf("new_r8 = "); TPrinter<new_r8>::Print(printf); printf("\n");
    printf("\n");
    static constexpr const char abb[] = "abc";
    TPrinter<cString<abb>>::Print(printf);
    return 0;
}

