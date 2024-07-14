# Lexical Analyzer
----
## Token Types

1. #### Keywords 
    - let
    - dbg
    - if, else

2. #### Identers
    - Can only start with a letter [ a-z, A-Z ] 
    - Can only contain a Alphanumeric character [ a-z, A-Z, 0-9 ]

3. #### Literals
    - Can only be positive 32 bit Integers

4. #### Symbols
    - Semicolon **;** 
    - Open Brackets Curly **{**
    - Close Brackets Curly **}**

6. #### Operators
    - Equals ( = )
    - Additive ( + , - )
    - Multiplicative ( * , / )
    - Comparison ( == )
----
## Input 
Contents of input_file to compile as a string 

## Output 
A list of TOKENS 

-----
## Algorithm
<!-- - while current_position != end_of_file

    - if current_character is whitespace
        - ignore

    - else if current_character is letter
        - while current_character is alpha_numeric
            - buffer.append(current_character)
            - current_position++
        - if buffer is in list of **keywords**
            - token = get_keyword_type(buffer)
        - else 
            - token = get_identifier_type(buffer)
        -token_list.append(token)

    - else if current_character is digit
        - while current_character is digit
            - buffer.append(current_character)
            - current_position++
        - token_list.append(Token(INTEGET,buffer))

    - else if current_character is operator
        - token = get_operator_type(current_character)
        - token_list.append(token)

    - else if current_character is symbol
        - token = get_symbol_type(current_character)
        - token_list.append(token)

    - current_position++

- return token_list -->


- while current_position != end_of_file

    - if current_character is whitespace
        - ignore

    - else if current_character is letter
        - while current_character is alpha_numeric
            - buffer.append(current_character)
            - current_position++
        - if buffer is in list of **keywords**
            - token = get_keyword_type(buffer)
        - else 
            - token = get_identifier_type(buffer)
        -token_list.append(token)

    - else if current_character is digit
        - while current_character is digit
            - buffer.append(current_character)
            - current_position++
        - token_list.append(Token(INTEGET,buffer))

    - else if current_character is operator
        - while current_character is operator
            - buffer.append(current_character)
            - current_position++
        - token = get_operator_type(current_character)
        - token_list.append(token)

    - else if current_character is symbol
        - token = get_symbol_type(current_character)
        - token_list.append(token)

    - current_position++

- return token_list
-----