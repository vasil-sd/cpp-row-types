#ifndef __TYPERECORD__
#define __TYPERECORD__

#include "typeuniverse.h"
#include "typeprop.h"
#include "typelist.h"
#include "typeprint.h"
#include "typeprimitive.h"

/*
    An idea: every physical type has Get/Set accessors, so
    we can wrap it into coercions and get new type with saving ability to see it as old one
    
    template<typename Ffwd, typename Frev, typename = IsFunctor<F>::I, typename = TypesEqual<A, F::from>>
    struct Transport
    {
        typedef F::to::Type Type;
        typedef Transport type;
        typedef declspec(F::to::value) primitive_type;
        primitive_type value;
        ... overload constructors, = and type coercion operators
        Transport(const Ffwd::from& v)  {coerce(v, value)}
        typedef Transport I;
    };
    
    let F1=Functor<Char,Int>, F2=Functor<Int, Char>
    Transport<F1, F2>(a:A):B tr_b
    tr_b = b : B;
    tr_b = a : A;
    etc.
    storage is the same as in B, but tr_B is seen as A and as B simultaneously

    todo:
    Get(T& v) ~ operator T()
    Set(T& v) ~ operator =(T& v)
    T& Access() - dangerous and breaks abstraction
*/

namespace typeuniverse
{

using namespace typeuniverse;
using namespace typeprop;
using namespace typelist;
using namespace typeprimitive;

DefType(Records, Set);
DefType(RecordFields, Set);

TypeDiscriminator(Records);
TypeDiscriminator(RecordFields);

template <typename T>
struct RecordFieldInfo
{
    static constexpr unsigned int field_size = sizeof(T);
    static constexpr unsigned int field_alignment = alignof(T);
    static constexpr unsigned int
    GetAlignedAddr(const unsigned int a)
    {
        return (a + field_alignment - 1) & (~(static_cast<unsigned int>(field_alignment - 1)));
    }
    static constexpr unsigned int
    GetNextAddr(const unsigned int a)
    {
        return GetAlignedAddr(a) + field_size;
    }
};

template<typename R, const unsigned int Off, typename Name, typename Typ>
struct RecordField : RecordFieldInfo<Typ>
{
    typedef RecordFields Type;
    typedef RecordField  type;
    typedef RecordField  I; // inhabited
    typedef R record_type;
    typedef Name field_name;
    typedef RecordFieldInfo<Typ> field_info;

    static constexpr unsigned int field_offset = Off;

    typedef Typ field_type;
    Typ field_value;

    Typ& Access() 
    {
        return field_value;
    }
};

template<const unsigned int V>
struct IntVal
{
    typedef UInt Type;
    typedef IntVal type;
    typedef IntVal I;
    static constexpr unsigned int value = V;
};

template<const unsigned int Off, typename TL>
struct RecordFieldOffsets
{
    typedef TCons<TPair<First<Head<TL>>,
                        IntVal<
                        (Off + RecordFieldInfo<Second<Head<TL>>>::field_alignment - 1) & (~(static_cast<unsigned int>(RecordFieldInfo<Second<Head<TL>>>::field_alignment - 1)))>>,
                  Essence<
                    RecordFieldOffsets<
                      (Off + RecordFieldInfo<Second<Head<TL>>>::field_alignment + RecordFieldInfo<Second<Head<TL>>>::field_size - 1) & (~(static_cast<unsigned int>(RecordFieldInfo<Second<Head<TL>>>::field_alignment - 1))),
                      Tail<TL>
                    >
                  >> type;
};

template<const int Off>
struct RecordFieldOffsets<Off, Nil> : Nil {};

template<typename Top, const unsigned int Off, typename TL>
struct RecordRepresentationAux : RecordField<Top,
                                 (Off + RecordFieldInfo<Second<Head<TL>>>::field_alignment - 1) & (~(static_cast<unsigned int>(RecordFieldInfo<Second<Head<TL>>>::field_alignment - 1))),
                                 First<Head<TL>>, Second<Head<TL>>>,
                                 RecordRepresentationAux<
                                    Top,
                                    (Off + RecordFieldInfo<Second<Head<TL>>>::field_alignment + RecordFieldInfo<Second<Head<TL>>>::field_size - 1) & (~(static_cast<unsigned int>(RecordFieldInfo<Second<Head<TL>>>::field_alignment - 1))),
                                    Tail<TL>>
{};

template<typename Top, const unsigned int Off>
struct RecordRepresentationAux<Top, Off, Nil> 
{
//    char record_data[Off];
};

template<typename TL>
struct RecordAux : RecordRepresentationAux<RecordAux<TL>, 0, TL>
{
    typedef Records Type;
    typedef RecordAux type;
    typedef TL record_fields;
    typedef typename RecordFieldOffsets<0, TL>::type record_field_offsets;
    typedef RecordAux I;
};

template<typename TL, typename = typename IsUniq<Map<First,TL>>::I> // Record field names should be uniq
using MkRecord = RecordAux<TL>;

template<typename ... Args>
using Record = MkRecord<Map2<TPair, SelectEven<ToList<Args ...>>, Map<WrapPrimitiveType, SelectOdd<ToList<Args ...>>>>>;

template<typename R>
using Fields = typename R::record_fields;

template<typename N, typename FL>
struct FieldTypeAux
{
    template<const int, typename Pr>
    using P = TypesEqual<N, First<Pr>>;

    typedef Second<Head<Filter<P, FL>>> type;
};

template<typename N, typename FL>
using FieldType = Essence<FieldTypeAux<N, FL>>;

template<typename N, typename FL>
struct FieldOffsetAux
{
    template<const int, typename Pr>
    using P = TypesEqual<N, First<Pr>>;

    typedef Second<Head<Filter<P, FL>>> type;
};

template<typename N, typename R>
using FieldOffset = typename FieldOffsetAux<N, typename R::record_field_offsets>::type;

}

namespace typeprint
{

using namespace typeuniverse;
using namespace typeprimitive;

TypePrinterMacro(Records, Set);

template<typename>
struct PrintIfNotNil
{
    template<typename F>
    static void Print(F f, const char *s)
    {
        f(s);
    }
};
template<>
struct PrintIfNotNil<Nil>
{
    template<typename F>
    static void Print(F f, const char *s)
    {
        (void) f;
        (void) s;
    }
};


template<typename FLs>
struct FieldPrinter
{
    template<typename F>
    static void Print(F f)
    {
        TypePrinter<First<Head<FLs>>,TypeOf<First<Head<FLs>>>>::Print(f);
        f(" : ");
        TypePrinter<Second<Head<FLs>>,TypeOf<Second<Head<FLs>>>>::Print(f);
        PrintIfNotNil<Tail<FLs>>::Print(f, " ; ");
        FieldPrinter<Tail<FLs>>::Print(f);
    }

};

template<>
struct FieldPrinter<Nil> {
    template<typename F>
    static void Print(F f)
    {
        (void)f;
    }
};

template<typename R>
struct TypePrinter<R, Records>
{
    template<typename F>
    static void Print(F f)
    {
        f("Record{ ");
        FieldPrinter<Fields<R>>::Print(f);
        f(" }");
    }
};

template<typename I>
struct TypePrinter<I, UInt>
{
    template<typename T>
    static void to_string(char* s, T v)
    {
        int l = 0;
        int i = 0;
        do
        {
            s[i++] = v%10 + '0';
            v /= 10;
        }while(v);
        s[i] = 0;
        i--;
        l = i;
        i /= 2;
        do
        {
            char t = s[l-i];
            s[l-i] = s[i];
            s[i] = t;
            i = i ? i-1:i;
        }while(i);
    };
    template<typename F>
    static void Print(F f)
    {
        char str[32];
        f("UInt< ");
        to_string(str, I::value);
        f(" >");
    }
    template<typename F>
    static void print(const I& obj, F f)
    {
        char str[32];
        to_string(str, obj.value);
        f(str);
    }
};

template<typename Fld>
struct TypePrinter<Fld, RecordFields>
{
    template<typename T>
    static void to_string(char* s, T v)
    {
        int l = 0;
        int i = 0;
        do
        {
            s[i++] = v%10 + '0';
            v /= 10;
        }while(v);
        s[i] = 0;
        i--;
        l = i;
        i /= 2;
        do
        {
            char t = s[l-i];
            s[l-i] = s[i];
            s[i] = t;
            i = i ? i-1:i;
        }while(i);
    };

    template<typename F>
    static void Print(F f)
    {
        char str[32];
        f("Field< ");
        f(" Name: ");
        TPrinter<typename Fld::field_name>::Print(f);
        f(" ; Type: ");
        TPrinter<typename Fld::field_type>::Print(f);
        f(" ; Size: ");
        to_string(str, Fld::field_info::field_size);
        f(str);
        f(" ; Alignment: ");
        to_string(str, Fld::field_info::field_alignment);
        f(str);
        f(" ; Offset: ");
        to_string(str, Fld::field_offset);
        f(str);
        f(" >");
    }
};

}

namespace typerecord
{

using namespace typeuniverse;
using namespace typelist;

template<typename N, typename R, typename T = FieldType<N, Fields<R>>, const int O = FieldOffset<N, R>::value>
inline T&
FieldAux(R& r)
{
    return r.R::template RecordField<R, O, N, T>::Access();
}

template<typename Ns, typename P = Empty<Tail<Ns>>>
struct Field
{
    template<typename R>
    static inline auto&
    Access(R& r)
    {
        return Field<Tail<Ns>>::Access(FieldAux<Head<Ns>>(r));
    }
};

template<typename Ns>
struct Field<Ns, True>
{
    template<typename R>
    static inline auto&
    Access(R& r)
    {
        return FieldAux<Head<Ns>>(r);
    }
};

template <typename R, typename ...Args>
auto&
FieldAccess(R& r)
{
    return Field<ToList<Args...>>::Access(r);
}

template<typename R, typename = typename InRecords<R>::I>
using FieldNames = Map<First,Fields<R>>;

template <typename R, typename Flds>
struct FieldsPresentAux
{
    typedef Map<First, Fields<R>> names;
    typedef IsSubset<Flds, names> type;
};

template <typename R, typename Fs, typename = typename InRecords<R>::I >
using FieldsPresent = typename FieldsPresentAux<R, Fs>::type;

template <typename R1, typename R2>
struct UnionRecordsAux
{
    typedef Append<Fields<R1>, Fields<R2>> all_record_fields;
    typedef MkRecord<all_record_fields> type;
};

template <typename R1, typename R2, typename = typename InRecords<R1>::I, typename = typename InRecords<R2>::I,
          typename = typename IsUniq<Append<FieldNames<R1>,FieldNames<R2>>>::I> // Check if field names are uniq
using UnionRecords = Essence<UnionRecordsAux<R1, R2>>;

template <typename R1, typename R2>
struct IntersectRecordsAux
{
    template<typename F1, typename F2>
    using P = TypesEqual<First<F1>, First<F2>>;
    typedef Intersect<Fields<R1>, Fields<R2>, P> fields;
    typedef MkRecord<fields> type;
};

template <typename R1, typename R2, typename = typename InRecords<R1>::I, typename = typename InRecords<R2>::I,
          typename = typename Not<IsUniq<Append<FieldNames<R1>,FieldNames<R2>>>>::I> // There should be some fields in common, empty records are useless, thus not allowed
using IntersectRecords = Essence<IntersectRecordsAux<R1, R2>>;

template <typename R1, typename R2>
struct SumRecordsAux
{
    typedef Append<Fields<R1>, Fields<R2>> all_record_fields;
    typedef MkRecord<all_record_fields> type;
};

template <typename R, typename N, typename = FieldsPresent<R, ToList<N>>> // R should be record and contain field with name N
using SpecificRecordField = typename R::template RecordField<R, FieldOffset<N, R>::value, N, FieldType<N, Fields<R>>>;

template <typename R, typename TL>
struct RecordFieldsListAux
{
    typedef TCons<SpecificRecordField<R,Head<TL>>,
                  Essence<RecordFieldsListAux<R, Tail<TL>>>> type;
};

template <typename R>
struct RecordFieldsListAux<R, Nil> : Nil {};

template <typename R>
using RecordFieldsList = Essence<RecordFieldsListAux<R, FieldNames<R>>>;

/*
template <typename R1, typename R2, typename = typename InRecords<R1>::I, typename = typename InRecords<R2>::I,
          typename = typename IsUniq<Append<FieldNames<R1>,FieldNames<R2>>>::I> // Check if field names are uniq
using UnionRecords = Essence<UnionRecordsAux<R1, R2>>;
*/

template <template<typename, typename> typename P, typename V, typename R>
struct RecordFoldLeftAux
{
    typedef RecordFieldsList<R> fields;
    typedef FoldLeft<P, V, fields> type;
};

template <template<typename, typename> typename P, typename V, typename R, typename = typename InRecords<R>::I>
using RecordFoldLeft = Essence<RecordFoldLeftAux<P, V, R>>;

//TODO update should be done via given explicit coercions
template <typename R, typename R1, typename NL>
struct UpdateFromRecordAux
{
    typedef Head<NL> fname;

    inline static void
    update(R& r, R1& r1)
    {
         FieldAux<fname>(r) = FieldAux<fname>(r1);
         UpdateFromRecordAux<R, R1, Tail<NL>>::update(r, r1);
    }
};

template <typename R, typename R1>
struct UpdateFromRecordAux<R, R1, Nil>
{
    inline static void
    update(R& r, R1& r1)
    {
        (void) r;
        (void) r1;
    }
};

template <typename R, typename R1, typename = typename InRecords<R>::I, typename = typename InRecords<R1>::I>
using UpdateFromRecord = UpdateFromRecordAux<R, R1, Intersect<FieldNames<R>, FieldNames<R1>>>;

template <typename R, typename R1, typename Result = UnionRecords<R, R1>, typename = typename InRecords<R>::I, typename = typename InRecords<R1>::I>
Result inline operator + (const R& r, const R1& r1)
{
    Result result;
    UpdateFromRecord<Result, R>::update(result, const_cast<R&>(r));
    UpdateFromRecord<Result, R1>::update(result, const_cast<R1&>(r1));
    return result;
}

}

/*
 * 2. Predicate is_coercible(R1, R2, CS1) - record R1 may be coerced to R2 (transitively closed, and recursive) by coercion set CS1
 *
 * 3. Remove fields from record
 *
 * 4. Add fields to record
 *
 * 5. replace fields in record
 *
 *
 */

#endif // __TYPERECORD__
