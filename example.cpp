#include <stdio.h>

#include "example.h"

template<typename A, typename B, typename = typename And<InUInt<A>, InUInt<B>>::I>
struct RR : cUInt<A::cvalue + B::cvalue> {};

int
main()
{
    printf("%d, %d\n", ValuesEqual<True, False>::value, ValuesEqual<True, True>::value);
    printf("%d, %d\n", TypesEqual<int, int>::value, TypesEqual<char, int>::value);
    printf("%d %d\n", Inhabited<True>::value, Inhabited<False>::value);
    printf("%d\n", Length<TCons<int, TCons<char, Nil> > >::cvalue);
    printf("%d\n", Length<ToList<int, char, short, int> >::cvalue);
//    Abc abc;

    Field<ToList<N2>>::Access(Field<ToList<N1>>::Access(r)) = 22;
    
    printf("Field: %d\n", Field<ToList<N1,N2>>::Access(r).value);
    printf("Field: %d\n", FieldAccess<rec, N1,N2>(r).value);
    printf("Field: %d\n", FieldAccess1<N1,N2>::Access(r).value);
    Field<ToList<N2>>::Access(r) = 0;
    
    typedef ToList<N1, N1, N2, N2, N1, int> ll;
    typedef Filter<P, ll> pl;
    typedef FilterN<P1, ll> pl1;
    printf("%d\n", Length<pl>::cvalue);
    printf("%d\n", Length<pl1>::cvalue);
    printf("%d\n", Length<SelectEven<ll> >::cvalue);
    printf("%d\n", Length<SelectOdd<ll> >::cvalue);
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
    TPrinter<Assoc<N1, ToList<N1, char>> >::Print(printf);
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
    
    printf("r2.Next = %d", FieldAccess<rec2, Next>(r2).value);
    

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
    printf("\n");
    typedef ApplyToList<RR, ToList<cUInt<3>, cUInt<5>>> arr;
    TPrinter<arr>::Print(printf);
    printf("\n");
    return 0;
}

