#include "./headers/codegen.h"

Codegen::Codegen() {
  data_section = "";
  bss_section = "";
  text_section = "";
  require_print_integer_subroutine = false;
  require_comparison_subroutine = false;
  require_print_character_subroutine = false;
}

int Codegen::var_count = 0;
int Codegen::if_count = 0;
int Codegen::for_count = 0;

int Codegen::max_count = 0;

string Codegen::get_new_temp_variable() {
  var_count++;
  return "[_t" + to_string(var_count) + "]";
}
int Codegen::get_if_count() {
  if_count++;
  return if_count;
}
int Codegen::get_for_count() {
  for_count++;
  return for_count;
}

void Codegen::push_eax() { text_section += "    push eax\n"; }

void Codegen::pop_eax() { text_section += "    pop eax\n"; }

void Codegen::declare_variable_data_section(string variable_name, int value) {
  data_section += "    " + variable_name + " dd " + to_string(value) + "\n";
}

void Codegen::declare_variable_bss_section(string variable_name) {
  bss_section += "    " + variable_name + " resd 1\n";
}

void Codegen::generate_debug(string variable_name) {
  cout << "dbg " << variable_name << "\n";

  if (!require_print_integer_subroutine) {
    require_print_integer_subroutine = true;
  }
  text_section += "    mov eax, " + variable_name +
                  "\n    call _print_integer_subroutine\n\n";
  Codegen::reset_count();
}
void Codegen::generate_debug_char(string variable_name) {
  cout << "dbg " << variable_name << "\n";

  if (!require_print_character_subroutine) {
    require_print_character_subroutine = true;
  }
  text_section += "    mov al, " + variable_name +
                  "\n    call _print_character_subroutine\n\n";
  Codegen::reset_count();
}

void Codegen::generate_debug(int value) {
  cout << "dbg " << value << "\n";

  if (!require_print_integer_subroutine) {
    require_print_integer_subroutine = true;
  }
  text_section += "    mov eax, " + to_string(value) +
                  "\n    call _print_integer_subroutine\n";
  Codegen::reset_count();
}

void Codegen::generate_arithemetic_expressions(string new_var_name, string opdA,
                                               char op, string opdB) {
  cout << new_var_name << " = " << opdA << " " << op << " " << opdB << "\n";

  text_section += "    mov eax, " + opdA + "\n";
  if (op == '+') {
    text_section += "    add eax, " + opdB + "\n";
  }
  if (op == '-') {
    text_section += "    sub eax, " + opdB + "\n";
  }
  if (op == '*') {
    text_section += "    imul eax, " + opdB + "\n";
  }
  if (op == '/') {
    text_section += "    imul eax, " + opdB + "\n";
  }
  text_section += "    mov " + new_var_name + ", eax\n";
}

void Codegen::generate_comparative_expression(string new_var_name, string opdA,
                                              string op, string opdB) {
  cout << new_var_name << " = " << opdA << " " << op << " " << opdB << "\n";

  if (!require_comparison_subroutine) {
    require_comparison_subroutine = true;
  }
  text_section += "    mov eax, " + opdA + "\n";
  text_section += "    mov ebx, " + opdB + "\n";
  text_section += "    cmp eax, ebx\n";

  if (op == "==") {
    text_section += "    call _compare_equal_subroutine\n";
  } else if (op == ">") {
    text_section += "    call _compare_greater_subroutine\n";
  } else if (op == "<") {
    text_section += "    call _compare_less_subroutine\n";
  } else if (op == ">=") {
    text_section += "    call _compare_greater_equal_subroutine\n";
  } else if (op == "<=") {
    text_section += "    call _compare_less_equal_subroutine\n";
  } else if (op == "!=") {
    text_section += "    call _compare_not_equal_subroutine\n";
  }

  text_section += "    mov " + new_var_name + ", eax\n";
  cout << new_var_name << " = " << opdA << " " << op << " " << opdB << "\n";
}

void Codegen::generate_let(string lval, string rval) {
  text_section += "    mov eax, " + rval + "\n";
  text_section += "    mov " + lval + ", eax\n";
  cout << "let " << lval << " = " << rval << "\n\n";
  Codegen::reset_count();
}

void Codegen::generate_if(string condition, NodeStatementList *stmt_list_if,
                          NodeStatementList *stmt_list_else, int if_count) {

  cout << "\nif " << condition << "\n";

  text_section += "    mov eax, " + condition + "\n";
  text_section += "    cmp eax, 0\n";
  text_section += "    jnz _if" + to_string(if_count) + "\n\n";
  cout << "<--- false --->\n";
  // Else Statments
  traverse_stmt_list(stmt_list_else);
  text_section += "    jmp _if" + to_string(if_count) + "_end\n\n";
  cout << "<--- true  --->\n";
  // If Statments
  text_section += "_if" + to_string(if_count) + ":\n";
  traverse_stmt_list(stmt_list_if);

  text_section += "_if" + to_string(if_count) + "_end:\n";
  cout << "<--- ----- --->\n\n";
}

void Codegen::generate_assign(string lval, string rval) {
  text_section += "    mov eax, " + rval + "\n";
  text_section += "    mov " + lval + ", eax\n";
  cout << lval << " = " << rval << "\n\n";
  Codegen::reset_count();
}

void Codegen::export_asm() {
  string program = "";

  // --- Data Section --- //
  program += "section .data\n";
  program += data_section + "\n";

  // --- Bss Section --- //
  program += "section .bss\n";
  if (require_print_integer_subroutine) {
    program += "    buffer  resb 12\n";
  }
  if (require_print_character_subroutine) {
    program += "    char_buffer resb 1\n";
  }
  program += "    newline resb 1\n";

  for (int i = 1; i <= Codegen::max_count; i++) {
    declare_variable_bss_section("_t" + to_string(i));
  }
  program += bss_section;
  program += "\n";

  // --- Text Section --- //

  program += "section .text\n    global _start\n\n_start:\n";
  program += text_section;

  // --- Program Termination --- //
  program += R"(
    call _print_newline_subroutine
    mov eax, 1
    xor ebx, ebx
    int 0x80
  )";

  if (require_print_integer_subroutine) {
    program += R"(
_print_integer_subroutine:
    push eax
    mov ecx, buffer + 10

    xor edx, edx
    mov ebx, 10

    cmp eax, 0
    jge ._convert_loop
    neg eax

._convert_loop:
    xor edx, edx
    div ebx
    add dl, '0'
    mov [ecx], dl
    dec ecx
    test eax, eax
    jnz ._convert_loop

    pop eax
    cmp eax, 0
    jge ._print
    mov byte [ecx], '-'
    dec ecx

._print:
    inc ecx
    mov edx, buffer + 12
    sub edx, ecx
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret

)";
  }
  if (require_print_character_subroutine) {
    program += R"(
_print_character_subroutine:
    mov ecx, char_buffer 
    mov [ecx], al
    mov edx, 1 
    mov eax, 4 
    mov ebx, 1 
    int 0x80 
    ret 

)";
  }

  program += R"(
_print_newline_subroutine:
    mov ecx, newline
    mov byte [ecx], 0xA
    mov edx, 1
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret

)";

  if (require_comparison_subroutine) {
    program += R"(
_compare_equal_subroutine:
    je _true
    mov eax, 0 
    ret

_compare_less_subroutine:
    jl _true
    mov eax, 0
    ret

_compare_greater_subroutine:
    jg _true
    mov eax, 0
    ret

_compare_less_equal_subroutine:
    jl _true
    je _true
    mov eax, 0
    ret

_compare_greater_equal_subroutine:
    jg _true
    je _true
    mov eax, 0
    ret

_compare_not_equal_subroutine:
    jne _true
    mov eax, 0
    ret

_true:
    mov eax, 1
    ret
)";
  }

  cout << program << "\n ";

  ofstream outfile("./build/Assembly/program.asm");
  if (outfile.is_open()) {
    outfile << program;
    outfile.close();
  } else {
    cerr << "Error: ASM Code Generation Failed\n";
  }
}

void Codegen::traverse_parse_tree(NodeProgram *program) {
  cout << "\n<--- PROGRAM START --->\n\n";
  traverse_stmt_list(program->stmt_list);
  cout << "<--- PROGRAM END   --->\n\n";
}

void Codegen::traverse_stmt_list(NodeStatementList *stmt_list) {
  for (auto stmt : stmt_list->stmts) {
    traverse_stmt(stmt);
  }
}

void Codegen::traverse_stmt(NodeStatement *stmt) {
  if (stmt->debug) {
    traverse_debug(stmt->debug);
  } else if (stmt->let) {
    traverse_let(stmt->let);
  } else if (stmt->IF) {
    traverse_if(stmt->IF);
  } else if (stmt->assign) {
    traverse_assign(stmt->assign);
  } else if (stmt->FOR) {
    traverse_for(stmt->FOR);
  }
}

void Codegen::traverse_debug(NodeDebug *debug) {
  string var = "";
  if (debug->comp_exp) {
    var = traverse_comparative_expression(debug->comp_exp);
    generate_debug(var);
  } else if (debug->CHAR) {
    var.push_back('\'');
    var.push_back(debug->CHAR->value);
    var.push_back('\'');
    generate_debug_char(var);
  }
}

void Codegen::traverse_let(NodeLet *let) {
  string var = traverse_comparative_expression(let->comp_exp);
  declare_variable_bss_section(let->identifier->name);
  generate_let("[" + let->identifier->name + "]", var);
}

void Codegen::traverse_if(NodeIf *IF) {
  string var = traverse_comparative_expression(IF->comp_exp);
  generate_if(var, IF->stmt_list_if, IF->stmt_list_else,
              Codegen::get_if_count());
}

void Codegen::traverse_assign(NodeAssign *assign) {
  string var = traverse_comparative_expression(assign->comp_exp);
  generate_assign("[" + assign->identifier->name + "]", var);
}

void Codegen::traverse_for(NodeFor *FOR) {
  int for_count = Codegen::get_for_count();
  traverse_let(FOR->let);
  text_section += "_for" + to_string(for_count) + ":\n";
  string condition = traverse_comparative_expression(FOR->comp_exp);
  text_section += "    mov eax, " + condition + "\n";
  text_section += "    cmp eax, 0\n";
  text_section += "    jz _for" + to_string(for_count) + "_end\n\n";
  traverse_stmt_list(FOR->stmt_list);
  traverse_assign(FOR->assign);
  text_section += "    jmp _for" + to_string(for_count) + "\n\n";
  text_section += "_for" + to_string(for_count) + "_end:\n\n";
}

string
Codegen::traverse_comparative_expression(NodeComparativeExpression *comp_exp) {
  if (comp_exp->comp_operator == NULL && comp_exp->comp_exp == NULL) {
    return traverse_additive_expression(comp_exp->add_exp);
  } else {
    string new_temp_var = get_new_temp_variable();

    generate_comparative_expression(
        new_temp_var, traverse_comparative_expression(comp_exp->comp_exp),
        comp_exp->comp_operator->op,
        traverse_additive_expression(comp_exp->add_exp));

    return new_temp_var;
  }
}

string Codegen::traverse_additive_expression(NodeAdditiveExpression *add_exp) {
  if (add_exp->add_operator == NULL && add_exp->add_exp == NULL) {
    return traverse_multiplicative_expression(add_exp->mul_exp);
  } else {
    string new_temp_var = get_new_temp_variable();

    generate_arithemetic_expressions(
        new_temp_var, traverse_additive_expression(add_exp->add_exp),
        add_exp->add_operator->op,
        traverse_multiplicative_expression(add_exp->mul_exp));

    return new_temp_var;
  }
}

string Codegen::traverse_multiplicative_expression(
    NodeMultiplicativeExpression *mul_exp) {
  if (mul_exp->mul_operator == NULL && mul_exp->mul_exp == NULL) {
    return traverse_expression(mul_exp->exp);
  } else {
    string new_temp_var = get_new_temp_variable();
    generate_arithemetic_expressions(
        new_temp_var, traverse_multiplicative_expression(mul_exp->mul_exp),
        mul_exp->mul_operator->op, traverse_expression(mul_exp->exp));
    return new_temp_var;
  }
}

string Codegen::traverse_expression(NodeExpression *exp) {
  if (exp->identifier != NULL) {
    return "[" + exp->identifier->name + "]";
  } else {
    return to_string(exp->INT->value);
  }
}

void Codegen::reset_count() {
  Codegen::max_count = max(max_count, var_count);
  Codegen::var_count = 0;
}
