#ifndef HAZE_BINARYEXPRESSION_H
#define HAZE_BINARYEXPRESSION_H

#include "Expression.h"

#include "BinaryExpressionType.h"

namespace hz
{
	class BinaryExpression : public Expression
	{
	public:
		Expression* left;
		Expression* right;

	public:
		BinaryExpression(Expression* left, Expression* right)
			: left(left), right(right)
		{
		}

	public:
		virtual BinaryExpressionType btype() const = 0;
		virtual ExpressionType etype() const final override
		{
			return ExpressionType::BINARY;
		}
	};

	class PlusBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual std::string string() const final override;
		virtual PlusBinaryExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
	};

	class MinusBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual std::string string() const final override;
		virtual MinusBinaryExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
	};

	class TimesBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual std::string string() const final override;
		virtual TimesBinaryExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
	};
}

#endif //HAZE_BINARYEXPRESSION_H
