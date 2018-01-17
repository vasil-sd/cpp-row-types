#ifndef __TYPEUNIVERSE__
#define __TYPEUNIVERSE__

namespace typeuniverse
{

struct UU // universe
{
    typedef UU Type;
    typedef UU type;
    typedef UU I; // inhabited by Prop, Set etc...
};

#define DefType(T, ST) \
    struct T \
    { \
        typedef ST Type; \
        typedef T type; \
        typedef T I; \
    }

// type for symbolic names
DefType(Symbol, UU);

#define DefSymbol(Name) \
    struct Name \
    { \
        typedef Symbol Type; \
        typedef Name type; \
        static constexpr char value[] = # Name; \
    }

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

} // end of namespace typeuniverse

#endif // __TYPEUNIVERSE__
