// Haze PrimaryExpressionKind.x
// (c) Connor J. Link. All Rights Reserved.

// NOTE: associativity is meaningless for primary expressions so a default of LEFT is safe
X(IDENTIFIER, LEFT, 0, IdentifierExpression, identifier)
X(INTEGER_LTIERAL, LEFT, 0, IntegerLiteralExpression, integer literal)
X(FLOAT_LITERAL, LEFT, 0, FloatLiteralExpression, float literal)
X(CHARACTER_LITERAL, LEFT, 0, CharacterLiteralExpression, character literal)
X(STRING_LITERAL, LEFT, 0, StringLiteralExpression, string literal)
X(PARENTHESIZED, LEFT, 0, ParenthesizedExpression, parenthesized)
