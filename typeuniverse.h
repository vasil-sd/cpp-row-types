#ifndef __TYPEUNIVERSE__
#define __TYPEUNIVERSE__

namespace typeuniverse
{

struct UU_Symbol;

struct UU // universe
{
    typedef UU_Symbol type_name;
    typedef UU Type;
    typedef UU type;
    typedef UU I; // inhabited by Prop, Set etc...
};

#define DefType(T, ST) \
    DefTypeSymbol(T); \
    struct T \
    { \
        typedef T##_Symbol type_name; \
        typedef ST Type; \
        typedef T type; \
        typedef T I; \
    }

// type for symbolic names

struct Symbol
{
    typedef Symbol type_name;
    typedef UU Type;
    typedef Symbol type;
    typedef Symbol I;
    static constexpr const unsigned char cvalue[] = "Symbol";
};

#define DefTypeSymbol(Name) \
    struct Name##_Symbol \
    { \
        typedef Name##_Symbol type_name; \
        typedef Symbol Type; \
        typedef Name##_Symbol type; \
        static constexpr const unsigned char cvalue[] = #Name; \
    }

#define DefSymbol(Name) \
    struct Name \
    { \
        typedef Name type_name; \
        typedef Symbol Type; \
        typedef Name type; \
        static constexpr const unsigned char cvalue[] = #Name; \
    }

DefSymbol(UU_Symbol);

//type for propositions
DefType(Prop, UU);

//type for finite data-structures, like lists, records etc.
DefType(Set, UU);

//type for all unknown types
DefType(Unknown, UU);

template <typename F>
using TypeOf = typename F::Type;

template <typename F>
using Essence = typename F::type;

template <typename T>
using Identity = T;

} // end of namespace typeuniverse

#endif // __TYPEUNIVERSE__
