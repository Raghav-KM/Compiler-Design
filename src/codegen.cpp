#include "./headers/codegen.h"

Codegen::Codegen() {
  data_section = "";
  bss_section = "";
  text_section = "";
  require_print_integer_subroutine = false;
}

int Codegen::count = 0;
int Codegen::max_count = 0;

void Codegen::push_eax() { text_section += "    push eax\n"; }

void Codegen::pop_eax() { text_section += "    pop eax\n"; }

void Codegen::declare_variable_data_section(string variable_name, int value) {
  data_section += "    " + variable_name + " dd " + to_string(value) + "\n";
}

void Codegen::declare_variable_bss_section(string variable_name) {
  bss_section += "    " + variable_name + " resd 1\n";
}

void Codegen::generate_debug(string variable_name) {
  if (!require_print_integer_subroutine) {
    require_print_integer_subroutine = true;
  }
  text_section +=
      "    mov eax, " + variable_name + "\n    call print_integer\n\n";
  Codegen::reset_count();
}

void Codegen::generate_debug(int value) {
  if (!require_print_integer_subroutine) {
    require_print_integer_subroutine = true;
  }
  text_section +=
      "    mov eax, " + to_string(value) + "\n    call print_integer\n";
  Codegen::reset_count();
}

void Codegen::generate_expressions(string new_var_name, string opdA, char op,
                                   string opdB) {
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
  cout << new_var_name << " = " << opdA << " " << op << " " << opdB << "\n";
}

void Codegen::generate_let(string lval, string rval) {
  text_section += "    mov eax, " + rval + "\n";
  text_section += "    mov " + lval + ", eax\n";
  cout << lval << " = " << rval << "\n\n";
  Codegen::reset_count();
}

string Codegen::get_new_temp_variable() {
  count++;
  return "[t_" + to_string(count) + "]";
}

void Codegen::export_asm() {
  string program = "";

  // --- Data Section --- //
  program += "section .data\n";
  program += data_section + "\n";

  // --- Bss Section --- //
  program += "section .bss\n";
  if (require_print_integer_subroutine) {
    program += "    buffer resb 12\n";
  }
  for (int i = 1; i <= Codegen::max_count; i++) {
    declare_variable_bss_section("t_" + to_string(i));
  }
  program += bss_section;
  program += "\n";

  // --- Text Section --- //

  program += "section .text\n    global _start\n\n_start:\n";
  program += text_section;

  // --- Program Termination --- //
  program += R"(
    mov eax, 1
    xor ebx, ebx
    int 0x80
  )";

  if (require_print_integer_subroutine) {
    program += R"(
print_integer:
    push eax
    mov ecx, buffer + 10
    mov byte [ecx], 0xA
    dec ecx

    xor edx, edx
    mov ebx, 10

    cmp eax, 0
    jge .convert_loop
    neg eax

.convert_loop:
    xor edx, edx
    div ebx
    add dl, '0'
    mov [ecx], dl
    dec ecx
    test eax, eax
    jnz .convert_loop

    pop eax
    cmp eax, 0
    jge .print
    mov byte [ecx], '-'
    dec ecx

.print:
    inc ecx
    mov edx, buffer + 11
    sub edx, ecx
    mov eax, 4
    mov ebx, 1
    int 0x80
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
  for (auto stmt : program->stmts) {
    traverse_stmt(stmt);
  }
}
void Codegen::traverse_stmt(NodeStatement *stmt) {
  if (stmt->debug) {
    traverse_debug(stmt->debug);
  } else if (stmt->let) {
    traverse_let(stmt->let);
  }
}

void Codegen::traverse_debug(NodeDebug *debug) {
  string var = traverse_additive_expression(debug->add_exp);
  // cout << "debug " << var << "\n";
  generate_debug(var);
}

void Codegen::traverse_let(NodeLet *let) {
  string var = traverse_additive_expression(let->add_exp);
  declare_variable_bss_section(let->identifier->name);
  generate_let("[" + let->identifier->name + "]", var);
}

string Codegen::traverse_additive_expression(NodeAdditiveExpression *add_exp) {
  if (add_exp->add_operator == NULL && add_exp->add_exp == NULL) {
    return traverse_multiplicative_expression(add_exp->mul_exp);
  } else {
    string new_temp_var = get_new_temp_variable();

    generate_expressions(new_temp_var,
                         traverse_additive_expression(add_exp->add_exp),
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
    generate_expressions(
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
  Codegen::max_count = max(max_count, count);
  Codegen::count = 0;
}
