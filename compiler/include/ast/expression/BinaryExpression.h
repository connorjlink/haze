#ifndef HAZE_BINARYEXPRESSION_H
#define HAZE_BINARYEXPRESSION_H

#include "Expression.h"
#include "BinaryExpressionType.h"

// Haze BinaryExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class BinaryExpression : public Expression
	{
	public:
		Expression* left;
		Expression* right;

	public:
		BinaryExpression(Expression* left, Expression* right, const Token& token)
			: Expression{ token }, left{ left }, right{ right }
		{
		}

	public:
		virtual BinaryExpressionType btype() const = 0;
		virtual ExpressionType etype() const final override
		{
			return ExpressionType::BINARY;
		}
		virtual TypeType ttype() const final override
		{
			// NOTE: always the type of the left hand side
			// this will support things like ("1234" + 1) in the future -> string "234"
			return left->ttype();
		}
	};

	class PlusBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
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
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class OrBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class XorBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class AndBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class LeftShiftBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};

	class RightShiftBinaryExpression : public BinaryExpression
	{
	public:
		using BinaryExpression::BinaryExpression;

	public:
		virtual BinaryExpressionType btype() const final override;
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
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
