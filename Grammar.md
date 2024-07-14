
# Terminals 

- `INT_LIT`, `IDENTIFIER`
- `ADD`, `SUB`, `DIV`, `MUL`, `EQUALS`
- `SEMICOLON`, `BRACKET_CLOSE_CURLY`, `BRACKET_CLOSE_CURLY` 
- `DEBUG`, `LET`

# Non-Terminals

- `<PROGRAM>`
- `<STMT_LIST>`
- `<STMT>`, `<DEBUG_STMT>`, `<LET_STMT>`, `<IF_STMT>`
- `<ADD_EXP>`, `<MUL_EXP>`, `<EXP>`

# Language Grammar

```
<PROGRAM>    → <STMT_LIST>

<STMT_LIST>  → <STMT>*

<STMT>       → <DEBUG_STMT> 
                | <LET_STMT> 
                | <IF_STMT>

<DEBUG_STMT> → DEBUG <ADD_EXP> SEMICOLON

<LET_STMT>   → LET IDENTIFIER EQUALS <ADD_EXP> SEMICOLON

<IF_STMT>    → IF <ADD_EXP> BRACKET_OPEN_CURLY <STMT_LIST> BRACKET_CLOSE_CURLY ELSE BRACKET_OPEN_CURLY <STMT_LIST> BRACKET_CLOSE_CURLY

<ADD_EXP>    → <MUL_EXP> <ADD_OP> <ADD_EXP> 
                | <MUL_EXP>

<MUL_EXP>    → <EXP> <MUL_OP> <MUL_EXP> 
                | <EXP>

<EXP>        → IDENTIFIER 
                | INT_LIT
```
