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
		virtual Node* evaluate(Context*) const final override;
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
		virtual Node* evaluate(Context*) const final override;
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
		virtual Node* evaluate(Context*) const final override;
	};

	class AssignBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual std::string string() const final override;
		virtual AssignBinaryExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class EqualityBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual std::string string() const final override;
		virtual EqualityBinaryExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class InequalityBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual std::string string() const final override;
		virtual InequalityBinaryExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class GreaterBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual std::string string() const final override;
		virtual GreaterBinaryExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class LessBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual std::string string() const final override;
		virtual LessBinaryExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif //HAZE_BINARYEXPRESSION_H
