#include <stdio.h>

#include "typerelation.h"

#include "example.h"

using namespace typerelation;

template<typename A, typename B, typename = typename And<InUInt<A>, InUInt<B>>::I>
struct RR : cUInt<A::cvalue + B::cvalue> {};
    struct Tmp
    {
        template <typename N>
        using P = BoolToProp<N::cvalue == 'c'>;
    };

template<typename N>
using Prep = cInt<N::cvalue + 10>;

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
    typedef ToList<Next, int, N2> ll1;
    printf("APPEND\n");    
    printf("ll = \n");
    TPrinter<ll>::Print(printf);
    printf("ll1 = \n");
    TPrinter<ll1>::Print(printf);
    printf("append ll ll1 = \n");
    TPrinter<Append<ll, ll1>>::Print(printf);
    printf("\n");
    typedef Filter<P, ll> pl;
    typedef FilterN<P1, ll> pl1;
    printf("%d\n", Length<pl>::cvalue);
    printf("%d\n", Length<pl1>::cvalue);
    printf("%d\n", Length<SelectEven<ll> >::cvalue);
    printf("%d\n", Length<SelectOdd<ll> >::cvalue);
    printf("PRESENT:%d\n", IsPresent<Next, ll>::value);
    printf("PRESENT:%d\n", IsPresent<N2, ll>::value);
    printf("FOLD: %d\n", FoldLeft<And, True, ToList<True, False, True> >::value);
    printf("SUBSET: %d\n", IsSubset<ToList<False, False, N1>, ToList<N1, True, N2, False, True> >::value);
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
    TPrinter<GetAllCoercions<StaticCast::Coerce, ToList<unsigned char, unsigned short, unsigned int, int, char, N1, short, N2>
                             > >::Print(printf);
    printf("\n");
    TPrinter<Assoc<N1, ToList<N1, char>> >::Print(printf);
    printf("\n");
    TPrinter<Present<Next, ll> >::Print(printf);
    printf("\n");
    TPrinter<Present<int, ToList<Nil, int> > >::Print(printf);
    printf("\n");
    TPrinter<Uniq<Nil> >::Print(printf);
    printf("\nCOERSION:\n-------\n");
    typedef FindCoercion<UChar, Int, ToList<UChar, Char, Int, Short, UInt, Next, N1>,
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
    static constexpr const unsigned char abb[] = "abc";
    TPrinter<cString<CStringToCharList<abb>>>::Print(printf);
    printf("\n");
    typedef ApplyToList<RR, ToList<cUInt<3>, cUInt<5>>> arr;
    TPrinter<arr>::Print(printf);
    printf("\n");
    static constexpr char unsigned sss[] = "abc";
    printf("%s\n", CharListToCString<CStringToCharList<sss>>::cvalue);
    printf("%d\n", Search<Tmp::P, CStringToCharList<sss>>::cvalue);
    TPrinter<IsListMonomorphic<ToList<Int, Int, cInt<0>>>>::Print(printf);
    printf("\n");
    TPrinter<Map<Prep, ToList<cInt<0>, cInt<1>, cInt<2>>>>::Print(printf);
    printf("\n");
    typedef Rel<ToList<TPair<N1, N2>, TPair<N2, N3>, TPair<N3, N4>, TPair<N4, N5>>> rel;
    TPrinter<rel>::Print(printf);
    printf("\n");
    typedef RelPath<ToList<TPair<N1, N2>, TPair<N2, N3>>> relpath;
    typedef RelPath<ToList<TPair<N2, N3>, TPair<N3, N4>, TPair<N4, N5>>> relpath1;
    typedef RelPath<ToList<TPair<N4, N5>, TPair<N5, N6>>> relpath2;
    typedef RelPath<ToList<TPair<N6, N7>, TPair<N7, N7>>> relpath3;
    typedef RelPath<ToList<TPair<N3, N4>>> relpath4;
    TPrinter<relpath>::Print(printf);
    printf("\n");
    typedef ExtendRelPath<relpath, ToList<relpath1, relpath2, relpath3, relpath4>>::type ext;
    TPrinter<CloseRelation<rel>>::Print(printf);
    printf("\n");
    return 0;
}

