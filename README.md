```ebnf
<expression> ::= <term> | <expression> "+" <term> | <expression> "-" <term>
<term>       ::= <factor> | <term> "*" <factor> | <term> "/" <factor>
<factor>     ::= <number> | "(" <expression> ")"
<number>     ::= <digit> | <number> <digit>
<digit>      ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
