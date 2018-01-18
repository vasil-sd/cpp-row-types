#ifndef __TYPEPROP__
#define __TYPEPROP__

#include "typeuniverse.h"
#include "typeprint.h"

namespace typeuniverse
{
struct True
{
    typedef Prop Type;
    typedef True type;
    typedef True I; //one inhabitant
    static constexpr bool value = true;
};

struct False
{
    typedef Prop Type;
    typedef False type;
    static constexpr bool value = false;
};

#define TypeDiscriminator(T) \
    template<typename Typ, typename Tp = typename Typ::Type> \
    struct In ## T ## Aux : False {}; \
    template<typename Typ> \
    struct In ## T ## Aux<Typ, T> : True {}; \
    template<typename Typ> \
    using In ## T = Essence<In ## T ## Aux<Typ>>

TypeDiscriminator(UU);
TypeDiscriminator(Symbol);
TypeDiscriminator(Prop);
TypeDiscriminator(Set);
TypeDiscriminator(Unknown);

}

namespace typeprint
{
using namespace typeuniverse;
TypePrinterMacro(True, Prop);
TypePrinterMacro(False, Prop);
}

namespace typeuniverse
{

// type detection is in two stages:
// 1. check if class
// 2. check for Type and type fields

template<typename T>
struct IsTypeAux3
{
    using Yes = char[ 2 ];
    using  No = char[ 1 ];

    template<class U>
    static Yes& itest(int U::*);
    template<class U>
    static No& itest(...);

    static constexpr bool value = sizeof(itest<T>(0)) == sizeof(Yes);
};

template<typename T>
struct IsTypeAux
{
    using Yes = char[ 2 ];
    using  No = char[ 1 ];

    struct Fallback
    {
        struct Type { };
        struct type { };
    };
    struct Derived : T, Fallback { };
    template<class U>
    static No& test( typename U::type*, typename U::Type* );
    template<typename U>
    static Yes& test( U*, U* );

    static constexpr bool value = sizeof(test<Derived>(nullptr, nullptr)) == sizeof(Yes);
};

template<const bool P>
struct IsTypeAux2 : False {};

template<>
struct IsTypeAux2<true>: True {};

template<typename T, const bool P>
struct IsTypeAux4 : False {};

template<typename T>
struct IsTypeAux4<T, true>: IsTypeAux2<IsTypeAux<T>::value>::type {};

template<typename T>
using IsType = typename IsTypeAux4<T, IsTypeAux3<T>::value>::type;

} // end of namespace typeprop

namespace typeprop
{

using namespace typeuniverse;
using namespace typeprint;

template<const bool b>
struct BoolToPropAux : False {};

template<>
struct BoolToPropAux<true>: True {};

template<const bool b>
using BoolToProp = typename BoolToPropAux<b>::type;

template<typename>
struct InhabitedAux;

template<const bool>
struct InhabitedAux2;

template<typename T>
using Inhabited = typename InhabitedAux2<InhabitedAux<T>::value>::type;
// Inhabited is alias to either True or False

template<typename P>
using Not = BoolToProp<!Inhabited<P>::value>;
// Not is alias to either True or False

template<typename L, typename R>
using And = BoolToProp<Inhabited<L>::value && Inhabited<R>::value>;

template<typename L, typename R>
using Or = BoolToProp<Inhabited<L>::value || Inhabited<R>::value>;

template<typename L, typename R>
using ValuesEqual = BoolToProp<L::value == R::value>;

template<typename LT, typename RT>
struct TypesEqualAux : False {};

template<typename T>
struct TypesEqualAux<T, T>: True {};

template<typename LT, typename RT>
using TypesEqual = typename TypesEqualAux<LT, RT>::type;

template<typename T>
struct InhabitedAux
{
    using Yes = char[ 2 ];
    using  No = char[ 1 ];
    struct Fallback
    {
        struct I { };
    };
    struct Derived : T, Fallback { };
    template<class U>
    static No& test( typename U::I* );
    template<typename U>
    static Yes& test( U* );

    static constexpr bool value = sizeof(test<Derived>(nullptr)) == sizeof(Yes);
};

template<const bool P>
struct InhabitedAux2 : False {};

template<>
struct InhabitedAux2<true>: True {};

template<typename Cond, typename TrueBranch, typename FalseBranch>
struct IfAux
{
    typedef TrueBranch type;
};

template<typename TrueBranch, typename FalseBranch>
struct IfAux<False, TrueBranch, FalseBranch>
{
    typedef FalseBranch type;
};

template<typename Cond, typename TrueBranch, typename FalseBranch>
using If = Essence<IfAux<Essence<Cond>, TrueBranch, FalseBranch>>;

} // end of namespace typeprop

#endif // __TYPEPROP__
