import std;

#include <ast/Expression.h>
#include <ast/IntegerLiteralExpression.h>
#include <ast/IdentifierExpression.h>
#include <error/CommonErrors.h>
#include <symbol/Symbol.h>
#include <symbol/SymbolDatabase.h>
#include <type/Type.h>

// Haze TypeCheck.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Type* resolve_type(Expression* expression)
	{
		using enum TypeQualifier;
		using enum TypeSignedness;
		using enum TypeStorage;

		using enum ExpressionType;
		switch (expression->etype())
		{
			case INTEGER_LITERAL:
			{
				const auto integer_literal_expression = AS_INTEGER_LITERAL_EXPRESSION(expression);
				const auto integer_literal = integer_literal_expression->value;

				using enum IntTypeType;

				using enum IntegerLiteralType;
				switch (integer_literal->itype())
				{
					case UBYTE: return new IntType{ IMMUTABLE, UNSIGNED, INT8, VALUE };
					case SBYTE: return new IntType{ IMMUTABLE, SIGNED, INT8, VALUE };

					case UWORD: return new IntType{ IMMUTABLE, UNSIGNED, INT16, VALUE };
					case SWORD: return new IntType{ IMMUTABLE, SIGNED, INT16, VALUE };

					case UDWORD: return new IntType{ IMMUTABLE, UNSIGNED, INT32, VALUE };
					case SDWORD: return new IntType{ IMMUTABLE, SIGNED, INT32, VALUE };

					case UQWORD: return new IntType{ IMMUTABLE, UNSIGNED, INT64, VALUE };
					case SQWORD: return new IntType{ IMMUTABLE, SIGNED, INT64, VALUE };
				}
			} break;

			case IDENTIFIER:
			{
				const auto identifier_expression = AS_IDENTIFIER_EXPRESSION(expression);
				const auto& identifier = identifier_expression->name;

				const auto symbol_type = USE_UNSAFE(SymbolDatabase)->query_symbol_type(identifier, expression->_token);

				using enum SymbolType;
				switch (symbol_type)
				{
					case ARGUMENT:
					{
						const auto argument_symbol = USE_UNSAFE(SymbolDatabase)->reference_argument(identifier, expression->_token);
						return argument_symbol->type;
					} break;

					case VARIABLE:
					{
						const auto variable_symbol = USE_UNSAFE(SymbolDatabase)->reference_variable(identifier, expression->_token);
						return variable_symbol->type;
					} break;

					case DEFINE:
					{
						const auto define_symbol = USE_UNSAFE(SymbolDatabase)->reference_define(identifier, expression->_token);
						return define_symbol->type;
					} break;

					// this would only be valid if there were a decltype expression
					//case STRUCT:
					//{
					//	const auto struct_symbol = _parser->reference_struct(identifier, expression->_token);
					//	// structure types are implitly of pointer type (but are still statically allocated)
					//	return new StructType{ IMMUTABLE, identifier, PTR };
					//} break;

					// same for this case
					//case FUNCTION:
					//{
					//	const auto function_symbol = _parser->reference_function(identifier, expression->_token);
					//	return function_symbol->return_type;
					//} break;

					default:
					{
						CommonErrors::invalid_symbol_type(symbol_type, expression->_token);
						return nullptr;
					} break;
				}
			} break;
		}

		return nullptr;
	}

	bool check_type(Expression* expression, Type* reference)
	{
		using enum TypeType;
		switch (reference->ttype())
		{
			case INT:
			{
				const auto reference_int_type = AS_INT_TYPE(reference);

#pragma message("TODO: permit other options here, like functions that evluate to int()")

				if (expression->etype() != ExpressionType::INTEGER_LITERAL)
				{
					CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
					return false;
				}

				// no need to check mutability since both are pure r-value integer literals

				// integer literals are not implicitly convertible to pointer type
				if (reference_int_type->storage != TypeStorage::VALUE)
				{
					CommonErrors::type_storage_mismatch(reference, expression, expression->_token);
					return false;
				}

				const auto integer_literal_expression = AS_INTEGER_LITERAL_EXPRESSION(expression);
				const auto integer_literal = integer_literal_expression->value;

				switch (integer_literal->itype())
				{
					case IntegerLiteralType::UBYTE:
					{
						if (reference_int_type->int_type != IntTypeType::INT8)
						{
							CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
							return false;
						}

						if (reference_int_type->signedness != TypeSignedness::UNSIGNED)
						{
							CommonErrors::type_signedness_mismatch(reference, expression, expression->_token);
							return false;
						}
					} break;

					case IntegerLiteralType::SBYTE:
					{
						if (reference_int_type->int_type != IntTypeType::INT8)
						{
							CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
							return false;
						}

						if (reference_int_type->signedness != TypeSignedness::SIGNED)
						{
							CommonErrors::type_signedness_mismatch(reference, expression, expression->_token);
							return false;
						}
					} break;

					case IntegerLiteralType::UWORD:
					{
						if (reference_int_type->int_type != IntTypeType::INT16)
						{
							CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
							return false;
						}

						if (reference_int_type->signedness != TypeSignedness::UNSIGNED)
						{
							CommonErrors::type_signedness_mismatch(reference, expression, expression->_token);
							return false;
						}
					} break;
					
					case IntegerLiteralType::SWORD:
					{
						if (reference_int_type->int_type != IntTypeType::INT16)
						{
							CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
							return false;
						}

						if (reference_int_type->signedness != TypeSignedness::SIGNED)
						{
							CommonErrors::type_signedness_mismatch(reference, expression, expression->_token);
							return false;
						}
					} break;

					case IntegerLiteralType::UDWORD:
					{
						if (reference_int_type->int_type != IntTypeType::INT32)
						{
							CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
							return false;
						}

						if (reference_int_type->signedness != TypeSignedness::UNSIGNED)
						{
							CommonErrors::type_signedness_mismatch(reference, expression, expression->_token);
							return false;
						}
					} break;

					case IntegerLiteralType::SDWORD:
					{
						if (reference_int_type->int_type != IntTypeType::INT32)
						{
							CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
							return false;
						}

						if (reference_int_type->signedness != TypeSignedness::SIGNED)
						{
							CommonErrors::type_signedness_mismatch(reference, expression, expression->_token);
							return false;
						}
					} break;

					case IntegerLiteralType::UQWORD:
					{
						if (reference_int_type->int_type != IntTypeType::INT64)
						{
							CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
							return false;
						}

						if (reference_int_type->signedness != TypeSignedness::UNSIGNED)
						{
							CommonErrors::type_signedness_mismatch(reference, expression, expression->_token);
							return false;
						}
					} break;

					case IntegerLiteralType::SQWORD:
					{
						if (reference_int_type->int_type != IntTypeType::INT64)
						{
							CommonErrors::int_type_specifier_mismatch(reference, expression, expression->_token);
							return false;
						}

						if (reference_int_type->signedness != TypeSignedness::SIGNED)
						{
							CommonErrors::type_signedness_mismatch(reference, expression, expression->_token);
							return false;
						}
					} break;
				}
			} break;

			case STRUCT:
			{

			} break;

			case STRING:
			{
				const auto reference_string_type = AS_STRING_TYPE(reference);
			
				switch (expression->etype())
				{
					case ExpressionType::STRING:
					{
						// no need to check mutability since both are pure r-value integer literals

						//static_assert(false);
						//if (reference_string_type->)
#pragma message("TODO: finish type checking here")
						return true;
					} break;
				}
			} break;

			case VOID:
			{
				// if the reference is void, then nothing can bind
				if (expression != nullptr)
				{
					CommonErrors::invalid_expression_type(expression->etype(), expression->_token);
					return false;
				}
			} break;
		}

		// if none of the above tests failed, then the types match
		return true;
	}
}
