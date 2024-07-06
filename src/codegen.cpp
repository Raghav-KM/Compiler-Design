#include "./headers/codegen.h"

Codegen::Codegen() {
  data_section = "";
  bss_section = "";
  text_section = "";
  require_print_integer_subroutine = false;
}

int Codegen::count = 0;

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
  push_eax();
  text_section +=
      "    mov eax, [" + variable_name + "]\n    call print_integer\n";
  pop_eax();
}

void Codegen::generate_debug(int value) {
  if (!require_print_integer_subroutine) {
    require_print_integer_subroutine = true;
  }
  push_eax();
  text_section +=
      "    mov eax, " + to_string(value) + "\n    call print_integer\n";
  pop_eax();
}

void Codegen::export_asm() {
  string program = "";

  // --- Data Section --- //
  program += "section .data\n";
  program += data_section + "\n";

  // --- Bss Section --- //
  program += "section .bss\n";
  if (require_print_integer_subroutine) {
    program += "    buffer resb 12\n\n";
  }

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

.convert_loop:
    xor edx, edx
    mov ebx, 10
    div ebx
    add dl, '0'
    mov [ecx], dl
    dec ecx
    test eax, eax 
    jnz .convert_loop
    inc ecx
    mov edx, buffer + 11
    sub edx, ecx
    mov eax, 4
    mov ebx, 1
    int 0x80

    pop eax 
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
  cout << "debug " << var << "\n";
}

void Codegen::traverse_let(NodeLet *let) {
  // declare_variable_data_section(let->identifier->name, let->INT->value);
}

string Codegen::traverse_additive_expression(NodeAdditiveExpression *add_exp) {
  if (add_exp->add_operator == NULL && add_exp->add_exp == NULL) {
    return traverse_multiplicative_expression(add_exp->mul_exp);
  } else {
    string new_temp_var = get_new_temp_variable();

    generate_expressions(new_temp_var,
                         traverse_multiplicative_expression(add_exp->mul_exp),
                         add_exp->add_operator->op,
                         traverse_additive_expression(add_exp->add_exp));

    return new_temp_var;
  }
}

string Codegen::traverse_multiplicative_expression(
    NodeMultiplicativeExpression *mul_exp) {
  if (mul_exp->mul_operator == NULL && mul_exp->mul_exp == NULL) {
    return traverse_expression(mul_exp->exp);
  } else {
    string new_temp_var = get_new_temp_variable();
    generate_expressions(new_temp_var, traverse_expression(mul_exp->exp),
                         mul_exp->mul_operator->op,
                         traverse_multiplicative_expression(mul_exp->mul_exp));
    return new_temp_var;
  }
}

string Codegen::traverse_expression(NodeExpression *exp) {
  if (exp->identifier != NULL) {
    return exp->identifier->name;
  } else {
    return to_string(exp->INT->value);
  }
}

void Codegen::generate_expressions(string new_var_name, string opdA, char op,
                                   string opdB) {
  cout << new_var_name << " = " << opdA << " " << op << " " << opdB << "\n";
}

string Codegen::get_new_temp_variable() {
  count++;
  return "[t_" + to_string(count) + "]";
}