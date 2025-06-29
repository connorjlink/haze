import std;

#include "VariableStatement.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Evaluator.h"
#include "Symbol.h"
#include "ErrorReporter.h"

// Haze VariableStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType VariableStatement::stype() const
	{
		return StatementType::VARIABLE;
	}

	VariableStatement* VariableStatement::copy() const
	{
		return new VariableStatement{ *this };
	}

	void VariableStatement::generate(Allocation*)
	{
		// NOTE: old method
		// Make some space on the heap and notify the parser
		// also will need to support byte sizes other than 4
		/*allocation = new HeapAllocation{ 4 };
		AS_VARIABLE_SYMBOL(_parser->reference_symbol(SymbolType::VARIABLE, name, NULL_TOKEN))->allocation = allocation;

		if (value)
		{
			AutoStackAllocation temp{};
			value->generate(temp.source());
			temp.source()->copy_into(allocation);
		}*/


		if (value)
		{
			AutoStackAllocation temp{};
			value->generate(temp.source());
			_generator->define_local(name, temp.source()->read());
		}

		else
		{
			_generator->define_local(name);
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

			using enum TypeType;
			switch (type->ttype())
			{
				case INT:
				{
					auto int_variable = AS_INT_TYPE(type);
					
					using enum TypeSignedness;
					using enum IntTypeType;
					switch (int_variable->int_type)
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

						case INT64:
						{
							if (int_variable->signedness == UNSIGNED) variable = new UnsignedQuadWordVariable{ 0 };
							else variable = new SignedQuadWordVariable{ 0 };
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
					USE_SAFE(ErrorReporter).post_warning(std::format("generated `{}` initializer value `\"\"` for `{}`", _type_type_map.at(type->ttype()), name), _token);
					variable = new StringVariable{ "" };
				} break;

				case VOID: 
				{
					USE_SAFE(ErrorReporter).post_error(std::format("invalid variable type `{}` for `{}`", _type_type_map.at(type->ttype()), name), _token);
					return nullptr;
				} break;
			}

			context->define_variable(name, variable);
		}

		return nullptr;
	}
}
