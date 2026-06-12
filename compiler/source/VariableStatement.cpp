import std;

#include <ast/statement/Statement.h>
#include <type/Type.h>

// Haze VariableStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementKind VariableStatement::stype() const
	{
		return StatementKind::VARIABLE;
	}

	void VariableStatement::generate(ValueHandle)
	{
		if (value)
		{
			AutoStackAllocation temp{};
			value->generate(temp.source());
			REQUIRE_SAFE(Allocator)->define_local(name, temp.source()->read());
		}
		else
		{
			REQUIRE_SAFE(Allocator)->define_local(name);
		}
	}

	Statement* VariableStatement::optimize()
	{
		if (auto value_optimized = value->optimize())
		{
			return new VariableStatement{ type, name, AS_EXPRESSION(value_optimized), allocation, _token };
		}

		return nullptr;
	}

	Node* VariableStatement::evaluate(Context* context) const
	{
		if (value)
		{
			context->define_variable(name, node_to_variable(value->evaluate(context)));
		}

		else
		{
			Variable* variable = nullptr;

			using enum TypeKind;
			switch (type->tag_type())
			{
				case INT:
				{
					auto int_variable = AS_INT_TYPE(type);
					
					using enum TypeSignedness;
					using enum IntTypeKind;
					switch (int_variable->int_kind)
					{
						case INT8:
						{
							if (int_variable->signedness == UNSIGNED) variable = new UnsignedByteVariable{ 0 };
							else variable = new SignedByteVariable{ 0 };
						} break;

						case INT16:
						{
							if (int_variable->signedness == UNSIGNED) variable = new UnsignedWordVariable{ 0 };
							else variable = new SignedWordVariable{ 0 };
						} break;

						case INT32:
						{
							if (int_variable->signedness == UNSIGNED) variable = new UnsignedDoubleWordVariable{ 0 };
							else variable = new SignedDoubleWordVariable{ 0 };
						} break;
					}
				} break;

				case STRUCT:
				{
					auto struct_type = AS_STRUCT_TYPE(type);
					variable = new StructVariable{ struct_type->tag };
				} break;

				case STRING:
				{
					USE_SAFE(ErrorReporter)->post_warning(std::format("generated `{}` initializer value `\"\"` for `{}`", _type_type_map.at(type->tag_type()), name), _token);
					variable = new StringVariable{ "" };
				} break;

				case VOID: 
				{
					USE_SAFE(ErrorReporter)->post_error(std::format("invalid variable type `{}` for `{}`", _type_type_map.at(type->tag_type()), name), _token);
					return nullptr;
				} break;
			}

			context->define_variable(name, variable);
		}

		return nullptr;
	}
}
