// Haze PostfixExpressionKind.x
// (c) Connor J. Link. All Rights Reserved.

X(POSTFIX_INCREMENT, LEFT, 1, PostfixIncrementExpression, postfix increment)
X(POSTFIX_DECREMENT, LEFT, 1, PostfixDecrementExpression, postfix decrement)
X(FUNCTION_CALL, LEFT, 1, FunctionCallExpression, function call)
X(SUBSCRIPT, LEFT, 1, SubscriptExpression, subscript)
X(MEMBER_ACCESS, LEFT, 1, MemberAccessExpression, member access)
X(MEMBER_ACCESS_THROUGH_POINTER, LEFT, 1, MemberAccessThroughPointerExpression, member access through pointer)
X(COMPOUND_LITERAL, LEFT, 1, CompoundLiteralExpression, compound literal)
