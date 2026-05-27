// Haze BinaryExpressionKind.x
// (c) Connor J. Link. All Rights Reserved.

X(MULTIPLY, LEFT, 3, MultiplyExpression, multiply)
X(DIVIDE, LEFT, 3, DivideExpression, divide)
X(MODULO, LEFT, 3, ModuloExpression, modulo)

X(ADD, LEFT, 4, AddExpression, add)
X(SUBTRACT, LEFT, 4, SubtractExpression, subtract)

X(LEFT_SHIFT, LEFT, 5, LeftShiftExpression, left shift)
X(RIGHT_SHIFT, LEFT, 5, RightShiftExpression, right shift)

X(LESS, LEFT, 6, LessExpression, less than)
X(LESS_EQUAL, LEFT, 6, LessEqualExpression, less than or equal to)
X(GREATER, LEFT, 6, GreaterExpression, greater than)
X(GREATER_EQUAL, LEFT, 6, GreaterEqualExpression, greater than or equal to)

X(EQUAL, LEFT, 7, EqualExpression, equal to)
X(NOT_EQUAL, LEFT, 7, NotEqualExpression, not equal to)

X(BITWISE_AND, LEFT, 8, BitwiseAndExpression, bitwise AND)

X(BITWISE_XOR, LEFT, 9, BitwiseXorExpression, bitwise exclusive OR)

X(BITWISE_OR, LEFT, 10, BitwiseOrExpression, bitwise OR)

X(LOGICAL_AND, LEFT, 11, LogicalAndExpression, logical AND)

X(LOGICAL_OR, LEFT, 12, LogicalOrExpression, logical OR)

X(TERNARY, RIGHT, 13, TernaryExpression, ternary)

X(ASSIGNMENT, RIGHT, 14, AssignmentExpression, assignment)
X(ADD_ASSIGNMENT, RIGHT, 14, AddAssignmentExpression, add assignment)
X(SUBTRACT_ASSIGNMENT, RIGHT, 14, SubtractAssignmentExpression, subtract assignment)
X(MULTIPLY_ASSIGNMENT, RIGHT, 14, MultiplyAssignmentExpression, multiply assignment)
X(DIVIDE_ASSIGNMENT, RIGHT, 14, DivideAssignmentExpression, divide assignment)
X(MODULO_ASSIGNMENT, RIGHT, 14, ModuloAssignmentExpression, modulo assignment)
X(LEFT_SHIFT_ASSIGNMENT, RIGHT, 14, LeftShiftAssignmentExpression, left shift assignment)
X(RIGHT_SHIFT_ASSIGNMENT, RIGHT, 14, RightShiftAssignmentExpression, right shift assignment)
X(BITWISE_AND_ASSIGNMENT, RIGHT, 14, BitwiseAndAssignmentExpression, bitwise AND assignment)
X(BITWISE_XOR_ASSIGNMENT, RIGHT, 14, BitwiseXorAssignmentExpression, bitwise exclusive OR assignment)
X(BITWISE_OR_ASSIGNMENT, RIGHT, 14, BitwiseOrAssignmentExpression, bitwise OR assignment)

X(COMMA, LEFT, 15, CommaExpression, comma)
