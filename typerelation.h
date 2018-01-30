#ifndef __TYPERELATION__
#define __TYPERELATION__

#include "typeuniverse.h"
#include "typeprint.h"
#include "typeprop.h"
#include "typeprimitive.h"
#include "typelist.h"
#include "string.h"

namespace typeuniverse
{
DefType(Relations, Set);
TypeDiscriminator(Relations);

DefType(RelationPaths, Set);
TypeDiscriminator(RelationPaths);
}

namespace typerelation
{
    using namespace typeuniverse;
    using namespace typeprop;
    using namespace typelist;

    template <typename L>
    struct IsPairsListAux : FoldLeft<And, True, Map<InPair,L>> {};

    template <typename L, typename = typename InList<L>::I>
    using IsPairsList = Essence<IsPairsListAux<L>>;

    template <typename L>
    struct IsRelPathAux
    {
        template <typename Acc, typename V>
        using P = If<TypesEqual<First<Acc>, First<V>>, TPair<Second<V>, True>, TPair<False, False>>;
        typedef Second<FoldLeft<P, TPair<Second<Head<L>>, True>, Tail<L>>> type;
    };

    template <typename L, typename = typename IsPairsList<L>::I>
    using IsRelPath = Essence<IsRelPathAux<L>>;
}

namespace typeuniverse 
{

using namespace typeprint;
using namespace typeprop;
using namespace typeprimitive;
using namespace typelist;
using namespace typerelation;

DefTypeSymbol(Rel);

//TODO: may be keep user-specified equality?
template<typename L, typename = typename IsPairsList<L>::I>
struct Rel
{
    typedef Rel_Symbol type_name;
    typedef Relations Type;
    typedef Rel type;
    typedef Rel I;

    typedef L relations;
};

DefTypeSymbol(RelPath);

template<typename L, typename = typename IsRelPath<L>::I>
struct RelPath
{
    typedef RelPath_Symbol type_name;
    typedef RelationPaths Type;
    typedef RelPath type;
    typedef RelPath I;

    typedef TPair<First<Head<L>>, Second<Head<Reverse<L>>>> closure;

    typedef L path;
};

} // end of namespace typestring


namespace typeprint
{

using namespace typeuniverse;
using namespace typeprint;
using namespace typeprop;

template<typename T>
struct TypePrinter<T, Relations>
{
    template<typename F>
    static void Print(F f)
    {
        f("Relation{");
        TypePrinter<typename T::relations, typename T::relations::Type>::Print(f);
        f("}");
    }
};

template<typename T>
struct TypePrinter<T, RelationPaths>
{
    template<typename F>
    static void Print(F f)
    {
        f("RelationPath( ");
        TPrinter<First<typename T::closure>>::Print(f);
        f(" -> ");
        TPrinter<Second<typename T::closure>>::Print(f);
        f(" ) {");
        TPrinter<typename T::path>::Print(f);
        f("}");
    }
};


} // end of namespace typeprint

namespace typerelation
{

using namespace typeuniverse;
using namespace typeprop;
using namespace typelist;

template<typename R, typename = typename InRelations<R>::I>
using RelationToPrimitivePaths = Map<RelPath, Map<ToList, typename R::relations>>;

template<typename RPL,
         typename = typename InList<RPL>::I,
         typename = typename InRelationPaths<Head<RPL>>::I>
using IsListOfRelPaths = IsListMonomorphic<RPL>;

template<typename P, typename Ps,
         typename = typename InList<Ps>::I,
         typename = typename IsListOfRelPaths<TCons<P, Ps>>::I>
struct ExtendRelPath
{
    typedef First<typename P::closure> domain;
    typedef Second<typename P::closure> codomain;
    template<typename Pth>
    using F = And<Not<TypesEqual<domain, Second<typename Pth::closure>>>, TypesEqual<codomain, First<typename Pth::closure>>>;
    typedef Filter<F, Ps> extensions;
    template<typename Ext>
    using Func = RelPath<Append<typename P::path, typename Ext::path>>;
    typedef Map<Func, extensions> new_paths;
    typedef Uniq<Append<TCons<P, Ps>, new_paths>> type;
};

template<typename R>
struct CloseRelationAux
{
    typedef Map<TemplateComposition<RelPath, ToList>::template T, typename R::relations> Ps;
    template<typename P>
    using P = Uniq<FoldLeft<Append, Nil, Map<CloseTemplate<SwapTemplateArgs21<ExtendRelPath>::template T, P>::template T, P>>>;
    typedef Fixpoint<P, Ps, IsIsomorphic> type;
};

template<typename R, typename = typename InRelations<R>::I>
using CloseRelation = Essence<CloseRelationAux<R>>;

} // end of namespace typerelation

#endif // __TYPERELATION__
