import std;

#include <allocator/Allocation.h>
#include <ast/BinaryExpression.h>
#include <ast/IdentifierExpression.h>
#include <ast/IntegerLiteralExpression.h>
#include <ast/StringExpression.h>
#include <error/CommonErrors.h>
#include <runtime/Context.h>
#include <runtime/Evaluator.h>
#include <toolchain/Generator.h>
#include <utility/PlatformVariables.h>

// Haze BinaryExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	void generate_error(const std::string& op, Token token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("unsupported compiler binary operator `{}`", op), token);
	}

	const IntegerLiteral& match_type(IntegerLiteral* matchee, native_int value)
	{
		using enum IntegerLiteralType;
		switch (matchee->itype())
		{
			case UBYTE: return UnsignedByteIntegerLiteral{ static_cast<std::uint8_t>(value) };
			case SBYTE: return SignedByteIntegerLiteral{ static_cast<std::int8_t>(value) };

			case UWORD: return UnsignedWordIntegerLiteral{ static_cast<std::uint16_t>(value) };
			case SWORD: return SignedWordIntegerLiteral{ static_cast<std::int16_t>(value) };

			case UDWORD: return UnsignedDoubleWordIntegerLiteral{ static_cast<std::uint32_t>(value)};
			case SDWORD: return SignedDoubleWordIntegerLiteral{ static_cast<std::int32_t>(value) };

			case UQWORD: return UnsignedQuadWordIntegerLiteral{ static_cast<std::uint64_t>(value) };
			case SQWORD: return SignedQuadWordIntegerLiteral{ static_cast<std::int64_t>(value) };
		}
	}
}

namespace hz
{
	BinaryExpressionType PlusBinaryExpression::btype() const
	{
		return BinaryExpressionType::PLUS;
	}

	BinaryExpressionType MinusBinaryExpression::btype() const
	{
		return BinaryExpressionType::MINUS;
	}

	BinaryExpressionType TimesBinaryExpression::btype() const
	{
		return BinaryExpressionType::TIMES;
	}

	BinaryExpressionType OrBinaryExpression::btype() const
	{
		return BinaryExpressionType::OR;
	}

	BinaryExpressionType XorBinaryExpression::btype() const
	{
		return BinaryExpressionType::XOR;
	}

	BinaryExpressionType AndBinaryExpression::btype() const
	{
		return BinaryExpressionType::AND;
	}

	BinaryExpressionType LeftShiftBinaryExpression::btype() const
	{
		return BinaryExpressionType::LSHIFT;
	}

	BinaryExpressionType RightShiftBinaryExpression::btype() const
	{
		return BinaryExpressionType::RSHIFT;
	}

	BinaryExpressionType AssignBinaryExpression::btype() const
	{
		return BinaryExpressionType::ASSIGN;
	}

	BinaryExpressionType EqualityBinaryExpression::btype() const
	{
		return BinaryExpressionType::EQUALITY;
	}

	BinaryExpressionType InequalityBinaryExpression::btype() const
	{
		return BinaryExpressionType::INEQUALITY;
	}

	BinaryExpressionType GreaterBinaryExpression::btype() const
	{
		return BinaryExpressionType::GREATER;
	}

	BinaryExpressionType LessBinaryExpression::btype() const
	{
		return BinaryExpressionType::LESS;
	}


	PlusBinaryExpression* PlusBinaryExpression::copy() const
	{
		return new PlusBinaryExpression{ *this };
	}

	MinusBinaryExpression* MinusBinaryExpression::copy() const
	{
		return new MinusBinaryExpression{ *this };
	}

	TimesBinaryExpression* TimesBinaryExpression::copy() const
	{
		return new TimesBinaryExpression{ *this };
	}

	OrBinaryExpression* OrBinaryExpression::copy() const
	{
		return new OrBinaryExpression{ *this };
	}

	XorBinaryExpression* XorBinaryExpression::copy() const
	{
		return new XorBinaryExpression{ *this };
	}

	AndBinaryExpression* AndBinaryExpression::copy() const
	{
		return new AndBinaryExpression{ *this };
	}

	LeftShiftBinaryExpression* LeftShiftBinaryExpression::copy() const
	{
		return new LeftShiftBinaryExpression{ *this };
	}

	RightShiftBinaryExpression* RightShiftBinaryExpression::copy() const
	{
		return new RightShiftBinaryExpression{ *this };
	}

	AssignBinaryExpression* AssignBinaryExpression::copy() const
	{
		return new AssignBinaryExpression{ *this };
	}

	EqualityBinaryExpression* EqualityBinaryExpression::copy() const
	{
		return new EqualityBinaryExpression{ *this };
	}

	InequalityBinaryExpression* InequalityBinaryExpression::copy() const
	{
		return new InequalityBinaryExpression{ *this };
	}

	GreaterBinaryExpression* GreaterBinaryExpression::copy() const
	{
		return new GreaterBinaryExpression{ *this };
	}

	LessBinaryExpression* LessBinaryExpression::copy() const
	{
		return new LessBinaryExpression{ *this };
	}



	void PlusBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_add(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void MinusBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_subtract(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void TimesBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_multiplication(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void OrBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_bitor(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void XorBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_bitxor(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void AndBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_bitand(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void LeftShiftBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_bitlshift(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void RightShiftBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_bitrshift(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void AssignBinaryExpression::generate(Allocation* received_allocation)
	{
		if (left->etype() == ExpressionType::IDENTIFIER)
		{
			auto identifier_expression = AS_IDENTIFIER_EXPRESSION(left);

			right->generate(received_allocation);
			// figure out how to assign the lhs
			REQUIRE_SAFE(RuntimeAllocator)->write_local(identifier_expression->name, received_allocation->read());
		}

		else
		{
			CommonErrors::must_be_lvalue("the left-hand operand of an assignment expression", left->_token);
		}
	}

	void EqualityBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_equality(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void InequalityBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};
		
		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_inequality(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void GreaterBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_greater(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void LessBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		REQUIRE_SAFE(Generator)->compute_less(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}


	Expression* PlusBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized)
		{
			left_optimized = AS_EXPRESSION(left->copy());
			right_optimized = AS_EXPRESSION(right->copy());

			if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
				TEST_VALUE_EQUALS(left_optimized, 0))
			{
				return right_optimized;
			}

			else if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 TEST_VALUE_EQUALS(right_optimized, 0))
			{
				return left_optimized;
			}

			else if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{
				const auto left_value = VALUE_OF(left_optimized);
				const auto right_value = VALUE_OF(right_optimized);

				if (left_value->itype() != right_value->itype())
				{
					CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
					return nullptr;
				}

				return new IntegerLiteralExpression
				{
					left_value->plus(right_value),
					_token,
				};
			}

			else
			{
				// Neither side could be optimized
				return nullptr;
			}
		}

		else
		{
			if (!left_optimized)
			{
				return new PlusBinaryExpression
				{ 
					AS_EXPRESSION(left->copy()), 
					right_optimized,
					_token,
				};
			}

			else if (!right_optimized)
			{
				return new PlusBinaryExpression
				{ 
					left_optimized, 
					AS_EXPRESSION(right->copy()),
					_token,
				};
			}

			else
			{
				return new PlusBinaryExpression
				{ 
					left_optimized, 
					right_optimized,
					_token,
				};
			}
		}
	}

	Expression* MinusBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized)
		{
			left_optimized = AS_EXPRESSION(left->copy());
			right_optimized = AS_EXPRESSION(right->copy());

			if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
				TEST_VALUE_EQUALS(left_optimized, 0))
			{
				//(0 - x) simplifies to -x
				VALUE_OF(right_optimized) = VALUE_OF(right_optimized)->times(VALUE_OF(right_optimized)->from_value(-1));
				return right_optimized;
			}

			else if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 TEST_VALUE_EQUALS(right_optimized, 0))
			{
				return left_optimized;
			}

			else if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{
				const auto left_value = VALUE_OF(left_optimized);
				const auto right_value = VALUE_OF(right_optimized);

				if (left_value->itype() != right_value->itype())
				{
					CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
					return nullptr;
				}

				return new IntegerLiteralExpression
				{
					left_value->minus(right_value),
					_token,
				};
			}

			else
			{
				//return new MinusBinaryExpression{ left_optimized, right_optimized };
				return nullptr;
			}
		}

		else
		{
			if (!left_optimized)
			{
				return new MinusBinaryExpression
				{
					AS_EXPRESSION(left->copy()), 
					right_optimized,
					_token,
				};
			}

			else if (!right_optimized)
			{
				return new MinusBinaryExpression
				{ 
					left_optimized, 
					AS_EXPRESSION(right->copy()),
					_token,
				};
			}

			else
			{
				return new MinusBinaryExpression
				{ 
					left_optimized, 
					right_optimized,
					_token,
				};
			}
		}
	}

	Expression* TimesBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized)
		{
			left_optimized = AS_EXPRESSION(left->copy());
			right_optimized = AS_EXPRESSION(right->copy());

			if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{
				auto left_value = VALUE_OF(left_optimized);
				auto result = left_optimized;

				if (integer_literal_equals(left_value, EI(0)))
				{
					left_value->assign(left_value->from_value(0));
				}

				else if (integer_literal_equals(left_value, EI(1)))
				{
					delete result;
					result = right_optimized;
				}

				else
				{
					if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
					{
						auto right_value = VALUE_OF(right_optimized);
						VALUE_OF(result)->assign(left_value->times(right_value));
					}

					else
					{
						USE_SAFE(ErrorReporter)->post_error("unsupported compiler code generation for multiplication", right_optimized->_token);
						return nullptr;
					}
				}

				return result;
			}

			else if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{
				auto right_value = AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value;
				auto result = right_optimized;

				if (integer_literal_equals(right_value, EI(0)))
				{
					right_value->assign(right_value->from_value(0));
				}

				else if (integer_literal_equals(right_value, EI(1)))
				{
					delete result;
					result = left_optimized;
				}

				else
				{
					if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL)
					{
						auto left_value = VALUE_OF(left_optimized);
						VALUE_OF(result)->assign(right_value->times(left_value));
					}

					else
					{
						USE_SAFE(ErrorReporter)->post_error("unsupported compiler code generation for multiplication", left_optimized->_token);
						return nullptr;
					}
				}

				return result;
			}

			else
			{
				return nullptr;
			}
		}

		else
		{
			if (!left_optimized)
			{
				return new TimesBinaryExpression
				{ 
					AS_EXPRESSION(left->copy()), 
					right_optimized,
					_token,
				};
			}

			else if (!right_optimized)
			{
				return new TimesBinaryExpression
				{ 
					left_optimized, 
					AS_EXPRESSION(right->copy()),
					_token,
				};
			}

			else
			{
				return new TimesBinaryExpression
				{
					left_optimized, 
					right_optimized,
					_token,
				};
			}
		}
	}

	Expression* OrBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* XorBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* AndBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* LeftShiftBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* RightShiftBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* AssignBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* EqualityBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* InequalityBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* GreaterBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* LessBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized)
		{
			left_optimized = AS_EXPRESSION(left->copy());
			right_optimized = AS_EXPRESSION(right->copy());

			if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
				right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{
				const auto left_value = VALUE_OF(left_optimized);
				const auto right_value = VALUE_OF(right_optimized);

				if (left_value->itype() != right_value->itype())
				{
					CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
					return nullptr;
				}

				return new IntegerLiteralExpression
				{
					left_value->less(right_value),
					_token,
				};
			}

			else
			{
				// Neither side could be optimized
				return nullptr;
			}
		}

		else
		{
			if (!left_optimized)
			{
				return new LessBinaryExpression
				{
					AS_EXPRESSION(left->copy()),
					right_optimized,
					_token,
				};
			}

			else if (!right_optimized)
			{
				return new LessBinaryExpression
				{
					left_optimized,
					AS_EXPRESSION(right->copy()),
					_token,
				};
			}

			else
			{
				return new LessBinaryExpression
				{
					left_optimized,
					right_optimized,
					_token,
				};
			}
		}

		return nullptr;
	}



	Node* PlusBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->plus(right_value),
				_token,
			};
		}

		else if (left_evaluated->etype() == ExpressionType::STRING &&
				 right_evaluated->etype() == ExpressionType::STRING)
		{
			const auto& left_message = MESSAGE_OF(left_evaluated);
			const auto& right_message = MESSAGE_OF(right_evaluated);

			return new StringExpression
			{
				std::format("{}{}", left_message, right_message),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* MinusBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = AS_INTEGER_LITERAL_EXPRESSION(left_evaluated)->value;
			const auto right_value = AS_INTEGER_LITERAL_EXPRESSION(right_evaluated)->value;

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->minus(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* TimesBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = AS_INTEGER_LITERAL_EXPRESSION(left_evaluated)->value;
			const auto right_value = AS_INTEGER_LITERAL_EXPRESSION(right_evaluated)->value;

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->times(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* OrBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->bitwiseor(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* XorBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->bitwisexor(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* AndBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->bitwiseand(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* LeftShiftBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->bitwiselshift(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* RightShiftBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->bitwisershift(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* AssignBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left);
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() != ExpressionType::IDENTIFIER)
		{
			CommonErrors::must_be_lvalue("the left-hand operand of an assignment expression", left->_token);
			return nullptr;
		}

		if (right_evaluated->etype() == ExpressionType::INTEGER_LITERAL ||
			right_evaluated->etype() == ExpressionType::IDENTIFIER)
		{
			context->define_variable(AS_IDENTIFIER_EXPRESSION(left_evaluated)->name, 
				node_to_variable(right_evaluated));
			return left_evaluated;
		}

		else
		{
			CommonErrors::must_be_rvalue("the right-hand operand of an assignment expression", right->_token);
			return nullptr;
		}
	}

	Node* EqualityBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->equals(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* InequalityBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->notequals(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* GreaterBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->greater(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}

	Node* LessBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = AS_EXPRESSION(left->evaluate(context));
		const auto right_evaluated = AS_EXPRESSION(right->evaluate(context));

		if (left_evaluated->etype() == ExpressionType::INTEGER_LITERAL &&
			right_evaluated->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_value = VALUE_OF(left_evaluated);
			const auto right_value = VALUE_OF(right_evaluated);

			if (left_value->itype() != right_value->itype())
			{
				CommonErrors::integer_size_mismatch(left_value->itype(), right_value->itype(), _token);
				return nullptr;
			}

			return new IntegerLiteralExpression
			{
				left_value->less(right_value),
				_token,
			};
		}

		else
		{
			CommonErrors::invalid_binary_expression(left_evaluated->etype(), right_evaluated->etype(), btype(), _token);
			return nullptr;
		}
	}
}