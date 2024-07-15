
# Terminals 

- `INT_LIT`, `IDENTIFIER`
- `ADD`, `SUB`, `DIV`, `MUL`, `EQUALS`, `EQUAL_EQUALS`   
- `SEMICOLON`, `BRACKET_CLOSE_CURLY`, `BRACKET_CLOSE_CURLY` 
- `DEBUG`, `LET`

# Non-Terminals

- `<PROGRAM>`
- `<STMT_LIST>`
- `<STMT>`, `<DEBUG_STMT>`, `<LET_STMT>`, `<IF_STMT>`
- `<COMP_EXP>` `<ADD_EXP>`, `<MUL_EXP>`, `<EXP>`

# Language Grammar

```html
<PROGRAM>    → <STMT_LIST>

<STMT_LIST>  → <STMT>*

<STMT>       → <DEBUG_STMT> 
                | <LET_STMT> 
                | <IF_STMT>

<DEBUG_STMT> → DEBUG <COMP_EXP> SEMICOLON

<LET_STMT>   → LET IDENTIFIER EQUALS <COMP_EXP> SEMICOLON

<IF_STMT>    → IF <COMP_EXP> BRACKET_OPEN_CURLY <STMT_LIST> BRACKET_CLOSE_CURLY ELSE BRACKET_OPEN_CURLY <STMT_LIST> BRACKET_CLOSE_CURLY

<COMP_EXP>   → <ADD_EXP> <COMP_OP> <COMP_EXP>
                | <ADD_EXP>

<ADD_EXP>    → <MUL_EXP> <ADD_OP> <ADD_EXP> 
                | <MUL_EXP>

<MUL_EXP>    → <EXP> <MUL_OP> <MUL_EXP> 
                | <EXP>

<EXP>        → IDENTIFIER 
                | INT_LIT
```
