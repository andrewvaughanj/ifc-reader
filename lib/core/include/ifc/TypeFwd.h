#pragma once

#include <cstdint>

namespace ifc
{
    enum class TypeSort;
    using TypeIndex = AbstractReference<5, TypeSort>;

    struct FundamentalType;
    struct DesignatedType;
    struct TorType;
    struct SyntacticType;
    struct ExpansionType;
    struct PointerType;
    struct QualifiedType;
    struct RvalueReference;
    struct LvalueReference;
    struct TupleType;
    struct FunctionType;
    struct MethodType;
    struct ForallType;

    enum class Qualifiers : std::uint8_t;

    enum class FunctionTypeTraits : std::uint8_t;
}
