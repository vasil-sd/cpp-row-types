#ifndef __TYPEPRIMITIVEMATH__
#define __TYPEPRIMITIVEMATH__

#include "typeuniverse.h"
#include "typeprop.h"
#include "typeprint.h"
#include "typerelation.h"
#include "typeprimitive.h"
#include "stdio.h"

namespace typeprimitive
{
using namespace typeuniverse;
using namespace typeprop;
using namespace typerelation;
using namespace typeprint;

// TODO: add overflow/underflow checks

typedef MakeReflexive<Rel<ToList<
                            TPair<Char,   Short>,
                            TPair<UChar,  Short>,
                            TPair<UChar,  UShort>,
                            TPair<Short,  Int>,
                            TPair<UShort, Int>,
                            TPair<UShort, UInt>
                            >>> PrimitiveTypesSubtyping;

typedef Rel<Map<RelPathGetClosure, CloseRelation<PrimitiveTypesSubtyping>>> AllPrimitiveTypesSubtypings;

typedef Rel<ToList<
                TPair<Char,   cChar<0>>,
                TPair<UChar,  cUChar<0>>,
                TPair<Short,  cShort<0>>,
                TPair<UShort, cUShort<0>>,
                TPair<Int,    cInt<0>>,
                TPair<UInt,   cUInt<0>>
                >> PrimitiveTypesToConstantTypes;

template<typename T1, typename T2, typename R = TPair<AllPrimitiveTypesSubtypings::template R<T1, T2>, AllPrimitiveTypesSubtypings::template R<T2, T1>>>
struct GetBiggestTypeAux : False {};

template<typename T1, typename T2>
struct GetBiggestTypeAux<T1, T2, TPair<False, True>> : T1 {};

template<typename T1, typename T2>
struct GetBiggestTypeAux<T1, T2, TPair<True, False>> : T2 {};

template<typename T1, typename T2>
struct GetBiggestTypeAux<T1, T2, TPair<True, True>> : T1 {};

template<typename T1, typename T2>
using GetBiggestType = Essence<GetBiggestTypeAux<T1, T2>>;

template <typename V1, typename V2, typename R = PrimitiveTypesToConstantTypes::template F<GetBiggestType<TypeOf<V1>, TypeOf<V2>>> >
struct AddAux : R::template type_template<V1::cvalue + V2::cvalue> {};

template <typename V1, typename V2>
using Add = Essence<AddAux<V1, V2>>;

template <typename V1, typename V2, typename R = PrimitiveTypesToConstantTypes::template F<GetBiggestType<TypeOf<V1>, TypeOf<V2>>> >
struct MulAux : R::template type_template<V1::cvalue * V2::cvalue> {};

template <typename V1, typename V2>
using Mul = Essence<MulAux<V1, V2>>;

} // end of namespace typeprimitive
#endif // __TYPEPRIMITIVEMATH__
