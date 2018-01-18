#include "typeprop.h"
#include "typelist.h"
#include "typemorphism.h"
#include "typecoercion.h"
#include "typerecord.h"

using namespace typeuniverse;
using namespace typeprop;
using namespace typelist;
using namespace typecoercion;
using namespace typerecord;

DefSymbol(N1);
DefSymbol(N2);
DefSymbol(Next);

typedef Record<N2, char> rec1;

typedef Record<Next, char> rec2;

typedef Record<N1, rec1, N2, int> rec;

struct M1 : Functor<M1, N1, N2>{};

#include <stdio.h>

typedef Assoc<ToList<N2, int, N1, char>, N1> t;

template<const int, typename T>
using P = Not<TypesEqual<N2, T> >;

template<const int Idx, typename>
struct P1 : BoolToProp<Idx % 3 == 0>{};

template <typename R, typename P = FieldsPresent<R, ToList<N1>>>
void PP (R& r)
{
    TPrinter<P>::Print(printf);
}

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
    typedef Filter<P1, ll> pl1;
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
    
    auto r3 = r1 + r2;
    auto r4 = r - r1;

    printf("\n r3 = r1 + r2\n");
    printf("r1 = "); TPrinter<decltype(r1)>::Print(printf); printf("\n");
    printf("r2 = "); TPrinter<decltype(r2)>::Print(printf); printf("\n");
    printf("r3 = "); TPrinter<decltype(r3)>::Print(printf); printf("\n");
    printf("\n");
    printf("\n r4 = r - r1\n");
    printf("r = "); TPrinter<decltype(r)>::Print(printf); printf("\n");
    printf("r1 = "); TPrinter<decltype(r1)>::Print(printf); printf("\n");
    printf("r4 = "); TPrinter<decltype(r4)>::Print(printf); printf("\n");
    printf("\n");
    return 0;
}

