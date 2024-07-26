# Lexical Analyzer

## Token Types

1. #### Keywords 
    - `let`
    - `dbg`
    - `if`, `else`
    - `for`
    - `endl`
    - `int`
    - `char`

2. #### Identifiers
    - Can only start with a letter ie `a-z`, `A-Z` 
    - Can only contain a alphanumeric characters and underscore ie `a-z`, `A-Z`, `0-9` and  `_`

3. #### Literals
    - Can only be positive `32 Bit Integers`
    - Character Literals `ASCII`

4. #### Symbols
    - Semicolon `;` 
    - Colon `:`
    - Open Brackets Curly `{`
    - Close Brackets Curly `}`
    - Open Brackets `(`
    - Close Brackets `)`
    - Single Quotes `'`

6. #### Operators
    - Equals  `=` 
    - Additive  `+` , `-` 
    - Multiplicative `*` , `/`
    - Comparison  `==` , `>` , `<`, `>=` , `<=`, `!=`

## Input 
Contents of **input_file** (as a string) to compile 

## Output 
A list of **tokens** 


## Algorithm
```js
while current_position != end_of_file:
    if current_character is whitespace
        ignore

    else if current_character is letter
        while current_character is alpha_numeric:
            buffer.append(current_character)
            current_position++

        if buffer is in list of **keywords**
            token = get_keyword_type(buffer)
        else 
            token = get_identifier_type(buffer)
        token_list.append(token)

    else if current_character is digit
        while current_character is digit
            buffer.append(current_character)
            current_position++
        token_list.append(Token(INTEGER,buffer))

    else if current_character is operator
        while current_character is operator
            buffer.append(current_character)
            current_position++
        token = get_operator_type(current_character)
        token_list.append(token)

    else if current_character is symbol
        if current_character is SINGLE_QUOTES
            current_position++
            while current_character != SINGLE_QUOTES
                buffer.append(current_character)
                current_position++
            token = TOKEN(CHARACTER,buffer)
        else
            token = get_symbol_type(current_character)
        token_list.append(token)

    current_position++

return token_list
```
 