#pragma once

#include "FileHeader.h"
#include "Partition.h"

#include "ChartFwd.h"
#include "ExpressionFwd.h"
#include "DeclarationFwd.h"
#include "Literal.h"
#include "NameFwd.h"
#include "SyntaxTreeFwd.h"
#include "TypeFwd.h"

#include <boost/iostreams/device/mapped_file.hpp>

#include <optional>
#include <span>
#include <unordered_map>

namespace ifc
{
    class File
    {
    public:
        FileHeader const & header() const;

        const char * get_string(TextOffset) const;

        ScopeDescriptor global_scope() const;

        ScopePartition scope_descriptors() const;

#define DECLARE_PARTITION_GETTER(ElementType, IndexType, Property)  \
    public:                                                         \
    Partition<ElementType, IndexType> Property() const;             \
    private:                                                        \
        mutable std::optional<Partition<ElementType, IndexType>> cached_ ## Property ## _ ;

        // Declarations
        DECLARE_PARTITION_GETTER(Declaration, Index, declarations)

#define DECLARE_DECL_PARTITION_GETTER(DeclType, DeclName) \
    DECLARE_PARTITION_GETTER(DeclType, DeclIndex, DeclName)

        DECLARE_DECL_PARTITION_GETTER(ScopeDeclaration,      scope_declarations)
        DECLARE_DECL_PARTITION_GETTER(TemplateDeclaration,   template_declarations)
        DECLARE_DECL_PARTITION_GETTER(UsingDeclaration,      using_declarations)
        DECLARE_DECL_PARTITION_GETTER(Enumeration,           enumerations)
        DECLARE_DECL_PARTITION_GETTER(AliasDeclaration,      alias_declarations)
        DECLARE_DECL_PARTITION_GETTER(DeclReference,         decl_references)
        DECLARE_DECL_PARTITION_GETTER(FunctionDeclaration,   functions)
        DECLARE_DECL_PARTITION_GETTER(MethodDeclaration,     methods)
        DECLARE_DECL_PARTITION_GETTER(Constructor,           constructors)
        DECLARE_DECL_PARTITION_GETTER(Destructor,            destructors)
        DECLARE_DECL_PARTITION_GETTER(VariableDeclaration,   variables)
        DECLARE_DECL_PARTITION_GETTER(ParameterDeclaration,  parameters)
        DECLARE_DECL_PARTITION_GETTER(FieldDeclaration,      fields)
        DECLARE_DECL_PARTITION_GETTER(Concept,               concepts)

#undef DECLARE_DECL_PARTITION_GETTER

        // Types
#define DECLARE_TYPE_PARTITION_GETTER(Type, TypeName) \
    DECLARE_PARTITION_GETTER(Type, TypeIndex, TypeName)

        DECLARE_TYPE_PARTITION_GETTER(FundamentalType,    fundamental_types)
        DECLARE_TYPE_PARTITION_GETTER(DesignatedType,     designated_types)
        DECLARE_TYPE_PARTITION_GETTER(TorType,            tor_types)
        DECLARE_TYPE_PARTITION_GETTER(SyntacticType,      syntactic_types)
        DECLARE_TYPE_PARTITION_GETTER(ExpansionType,      expansion_types)
        DECLARE_TYPE_PARTITION_GETTER(PointerType,        pointer_types)
        DECLARE_TYPE_PARTITION_GETTER(FunctionType,       function_types)
        DECLARE_TYPE_PARTITION_GETTER(MethodType,         method_types)
        DECLARE_TYPE_PARTITION_GETTER(BaseType,           base_types)
        DECLARE_TYPE_PARTITION_GETTER(TupleType,          tuple_types)
        DECLARE_TYPE_PARTITION_GETTER(LvalueReference,    lvalue_references)
        DECLARE_TYPE_PARTITION_GETTER(RvalueReference,    rvalue_references)
        DECLARE_TYPE_PARTITION_GETTER(QualifiedType,      qualified_types)
        DECLARE_TYPE_PARTITION_GETTER(ForallType,         forall_types)
        DECLARE_TYPE_PARTITION_GETTER(SyntaxType,         syntax_types)
        DECLARE_TYPE_PARTITION_GETTER(PlaceholderType,    placeholder_types)

#undef DECLARE_TYPE_PARTITION_GETTER

        // Expressions
#define DECLARE_EXPR_PARTITION_GETTER(ExprType, ExprName) \
    DECLARE_PARTITION_GETTER(ExprType, ExprIndex, ExprName)

        DECLARE_EXPR_PARTITION_GETTER(LiteralExpression, literal_expressions)
        DECLARE_EXPR_PARTITION_GETTER(TypeExpression,    type_expressions)
        DECLARE_EXPR_PARTITION_GETTER(NamedDecl,         decl_expressions)
        DECLARE_EXPR_PARTITION_GETTER(UnqualifiedId,     unqualified_id_expressions)
        DECLARE_EXPR_PARTITION_GETTER(TemplateId,        template_ids)
        DECLARE_EXPR_PARTITION_GETTER(DyadExpression,    dyad_expressions)
        DECLARE_EXPR_PARTITION_GETTER(SizeofExpression,  sizeof_expressions)
        DECLARE_EXPR_PARTITION_GETTER(AlignofExpression, alignof_expressions)
        DECLARE_EXPR_PARTITION_GETTER(TupleExpression,   tuple_expressions)

        DECLARE_EXPR_PARTITION_GETTER(PackedTemplateArguments, packed_template_arguments)

#undef DECLARE_EXPR_PARTITION_GETTER

        // Heaps
        DECLARE_PARTITION_GETTER(TypeIndex, Index, type_heap)
        DECLARE_PARTITION_GETTER(ExprIndex, Index, expr_heap)

        // Names
        DECLARE_PARTITION_GETTER(OperatorFunctionName, NameIndex, operator_names)

        // Charts
        DECLARE_PARTITION_GETTER(ChartUnilevel,     ChartIndex, unilevel_charts)
        DECLARE_PARTITION_GETTER(ChartMultilevel,   ChartIndex, multilevel_charts)

        // Literals
        DECLARE_PARTITION_GETTER(IntegerLiteral,    LitIndex,   integer_literals)
        DECLARE_PARTITION_GETTER(FPLiteral,         LitIndex,   fp_literals)

        // Syntax Trees
#define DECLARE_SYNTAX_PARTITION_GETTER(SyntaxType, SyntaxName) \
    DECLARE_PARTITION_GETTER(SyntaxType, SyntaxIndex, SyntaxName)

        DECLARE_SYNTAX_PARTITION_GETTER(SimpleTypeSpecifier,        simple_type_specifiers)
        DECLARE_SYNTAX_PARTITION_GETTER(TypeSpecifierSeq,           type_specifier_seq_syntax_trees)
        DECLARE_SYNTAX_PARTITION_GETTER(TypeIdSyntax,               typeid_syntax_trees)
        DECLARE_SYNTAX_PARTITION_GETTER(DeclaratorSyntax,           declarator_syntax_trees)
        DECLARE_SYNTAX_PARTITION_GETTER(PointerDeclaratorSyntax,    pointer_declarator_syntax_trees)
        DECLARE_SYNTAX_PARTITION_GETTER(ExpressionSyntax,           expression_syntax_trees)

#undef DECLARE_SYNTAX_PARTITION_GETTER

        Partition<DeclIndex> deduction_guides() const;

#undef DECLARE_PARTITION_GETTER

    public:
        File const & get_imported_module(ModuleReference) const;

        File(std::string const &, class Environment*);

    private:
        struct Structure;

        Structure const * structure() const;

        template<typename T, typename Index = uint32_t>
        Partition<T, Index> get_partition() const;

        template<typename T, typename Index>
        Partition<T, Index> get_partition_with_cache(std::optional<Partition<T, Index>> & cache) const;

        template<typename T, typename Index>
        Partition<T, Index> get_partition(std::string_view name) const;

        template<typename T>
        T const* get_pointer(ByteOffset offset) const;

        void const* get_raw_pointer(ByteOffset) const;

        size_t calc_size() const;

        std::span<PartitionSummary const> table_of_contents() const;

        void fill_table_of_contents();

    private:
        Environment* env_;
        boost::iostreams::mapped_file_source fmap_;
        std::unordered_map<std::string_view, PartitionSummary const*> table_of_contents_;
    };
}
