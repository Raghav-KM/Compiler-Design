#include "./headers/codegen.h"
#include <cerrno>
#include <cstring> // For strerror

Codegen::Codegen() {
  icode = "";
  program_code = "";

  data_section = "";
  bss_section = "";
  text_section = "";
  function_definations = queue<NodeFunction *>();
  require_print_integer_subroutine = false;
  require_comparison_subroutine = false;
  require_print_character_subroutine = false;
}

int Codegen::var_count = 0;
int Codegen::if_count = 0;
int Codegen::for_count = 0;
int Codegen::max_count = 0;
int Codegen::comp_count = 0;

string Codegen::get_icode() { return icode; }
string Codegen::get_asm_code() { return program_code; }

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

int Codegen::get_comp_count() {
  comp_count++;
  return comp_count;
}

void Codegen::reset_count() {
  Codegen::max_count = max(max_count, var_count);
  Codegen::var_count = 0;
}

string Codegen::get_identifier_name(NodeIdentifier *identifier) {
  return identifier->name + "_" + to_string(identifier->scope);
}

void Codegen::declare_variable_bss_section(string variable_name) {
  bss_section += "    " + variable_name + " resd 1\n";
}

void Codegen::export_asm(string dir) {
  ofstream outfile(dir);

  if (!outfile) {
    cerr << "Err!! ASM Code Generation" << endl;
    exit(EXIT_FAILURE);
  }

  outfile << program_code;

  if (outfile.fail()) {
    outfile.close();
    cerr << "Err!! ASM Code Generation" << endl;
    exit(EXIT_FAILURE);
  }

  outfile.close();
  if (outfile.fail()) {
    cerr << "Err!! ASM Code Generation" << endl;
    exit(EXIT_FAILURE);
  }
}

void Codegen::finalize_text_section() {
  text_section += R"(
    call _print_newline_subroutine
    mov eax, 1
    xor ebx, ebx
    int 0x80
  )";

  while (!function_definations.empty()) {
    generate_function(function_definations.front());
    function_definations.pop();
  }

  if (require_print_integer_subroutine) {
    text_section += R"(
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
    text_section += R"(
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

  text_section += R"(
_print_newline_subroutine:
    mov ecx, newline
    mov byte [ecx], 0xA
    mov edx, 1
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret

)";
}

void Codegen::finalize_data_section() {}

void Codegen::finalize_bss_section() {
  if (require_print_integer_subroutine) {
    bss_section += "    buffer  resb 12\n";
  }
  if (require_print_character_subroutine) {
    bss_section += "    char_buffer resb 1\n";
  }
  bss_section += "    newline resb 1\n";

  for (int i = 1; i <= Codegen::max_count; i++) {
    declare_variable_bss_section("_t" + to_string(i));
  }
}

void Codegen::generate_parse_tree(NodeProgram *program) {
  generate_stmt_list(program->stmt_list);

  program_code = "";

  finalize_data_section();
  finalize_text_section();
  finalize_bss_section();

  // --- Data Section --- //
  program_code += "section .data\n";
  program_code += data_section;
  program_code += "\n";

  // --- Bss Section --- //
  program_code += "section .bss\n";
  program_code += bss_section;
  program_code += "\n";

  // --- Text Section --- //
  program_code += "section .text\n    global _start\n\n_start:\n";
  program_code += text_section;
  program_code += "\n";
}

void Codegen::generate_stmt_list(NodeStatementList *stmt_list) {
  for (auto stmt : stmt_list->stmts) {
    generate_stmt(stmt);
  }
}

void Codegen::generate_stmt(NodeStatement *stmt) {
  if (stmt->debug) {
    generate_debug(stmt->debug);
  } else if (stmt->let) {
    generate_let(stmt->let);
  } else if (stmt->IF) {
    generate_if(stmt->IF);
  } else if (stmt->assign) {
    generate_assign(stmt->assign);
  } else if (stmt->FOR) {
    generate_for(stmt->FOR);
  } else if (stmt->function) {
    function_definations.push(stmt->function);
  } else if (stmt->function_call) {
    generate_function_call(stmt->function_call);
  }
}

void Codegen::generate_debug(NodeDebug *debug) {
  string rval = generate_comparative_expression(debug->comp_exp);

  icode += "dbg " + rval + "\n\n";

  if (debug->comp_exp->type == INT) {
    require_print_integer_subroutine = true;

    text_section +=
        "    mov eax, " + rval + "\n    call _print_integer_subroutine\n";

  } else if (debug->comp_exp->type == CHAR) {
    require_print_character_subroutine = true;

    text_section +=
        "    mov al, " + rval + "\n    call _print_character_subroutine\n\n";
  }
  Codegen::reset_count();
}

void Codegen::generate_let(NodeLet *let) {
  declare_variable_bss_section(get_identifier_name(let->identifier));

  string rvar = generate_comparative_expression(let->comp_exp);
  string lvar = "[" + get_identifier_name(let->identifier) + "]";

  icode += "let " + lvar + " = " + rvar + "\n\n";

  text_section += "    mov eax, " + rvar + "\n";
  text_section += "    mov " + lvar + ", eax\n";

  Codegen::reset_count();
}

void Codegen::generate_if(NodeIf *IF) {
  string condition = generate_comparative_expression(IF->comp_exp);
  NodeStatementList *stmt_list_if = IF->stmt_list_if;
  NodeStatementList *stmt_list_else = IF->stmt_list_else;
  int if_count = get_if_count();

  string if_label_start = "_if" + to_string(if_count);
  string if_label_end = "_if" + to_string(if_count) + "_end";

  icode += "if " + condition + " :\n";

  text_section += "    mov eax, " + condition + "\n";
  text_section += "    cmp eax, 1\n";
  text_section += "    jne " + if_label_start + "\n\n";

  generate_stmt_list(stmt_list_if);

  text_section += "    jmp " + if_label_end + "\n\n";
  text_section += if_label_start + ":\n";

  if (stmt_list_else) {
    icode += "else :\n";
    generate_stmt_list(stmt_list_else);
  }

  text_section += if_label_end + ":\n";

  Codegen::reset_count();
}

void Codegen::generate_assign(NodeAssign *assign) {
  string rval = generate_comparative_expression(assign->comp_exp);
  string lval = "[" + get_identifier_name(assign->identifier) + "]";
  icode += lval + " = " + rval + "\n\n";

  text_section += "    mov eax, " + rval + "\n";
  text_section += "    mov " + lval + ", eax\n";

  Codegen::reset_count();
}

void Codegen::generate_for(NodeFor *FOR) {
  int for_count = Codegen::get_for_count();

  generate_let(FOR->let);
  text_section += "_for" + to_string(for_count) + ":\n";
  string condition = generate_comparative_expression(FOR->comp_exp);

  text_section += "    mov eax, " + condition + "\n";
  text_section += "    cmp eax, 0\n";
  text_section += "    jz _for" + to_string(for_count) + "_end\n\n";

  generate_stmt_list(FOR->stmt_list);

  generate_assign(FOR->assign);

  text_section += "    jmp _for" + to_string(for_count) + "\n\n";
  text_section += "_for" + to_string(for_count) + "_end:\n\n";

  Codegen::reset_count();
}

void Codegen::generate_function(NodeFunction *function) {
  text_section += "\n";
  text_section += function->function_identifier + ":\n";
  generate_stmt_list(function->stmt_list);
  text_section += "    ret\n";
}

void Codegen::generate_function_call(NodeFunctionCall *function_call) {
  text_section += "    call " + function_call->function_identifier + "\n";
}

string
Codegen::generate_comparative_expression(NodeComparativeExpression *comp_exp) {
  if (comp_exp->comp_operator == NULL && comp_exp->comp_exp == NULL) {
    return generate_additive_expression(comp_exp->add_exp);
  }

  string lvar = get_new_temp_variable();
  string opdA = generate_comparative_expression(comp_exp->comp_exp);
  string op = comp_exp->comp_operator->op;
  string opdB = generate_additive_expression(comp_exp->add_exp);

  icode += lvar + " = " + opdA + " " + op + " " + opdB + "\n";

  text_section += "    mov eax, " + opdA + "\n";
  text_section += "    mov ebx, " + opdB + "\n";
  text_section += "    cmp eax, ebx\n";
  text_section += "    mov eax, 0\n";

  if (op == "==") {
    text_section += "    je ";
  } else if (op == ">") {
    text_section += "    jle ";
  } else if (op == "<") {
    text_section += "    jge ";
  } else if (op == ">=") {
    text_section += "    jl ";
  } else if (op == "<=") {
    text_section += "    jg ";
  } else if (op == "!=") {
    text_section += "    je ";
  }

  string comp_label = "_cmp" + to_string(get_comp_count()) + "_end";

  text_section += comp_label + "\n";
  text_section += "    mov eax, 1\n";
  text_section += comp_label + ":\n";
  text_section += "    mov " + lvar + ", eax\n";

  return lvar;
}

string Codegen::generate_additive_expression(NodeAdditiveExpression *add_exp) {
  if (add_exp->add_operator == NULL && add_exp->add_exp == NULL) {
    return generate_negative_expression(add_exp->neg_exp);
  }
  string lvar = get_new_temp_variable();

  string opdA = generate_additive_expression(add_exp->add_exp);
  string op = string(1, add_exp->add_operator->op);
  string opdB = generate_negative_expression(add_exp->neg_exp);

  icode += lvar + " = " + opdA + " " + op + " " + opdB + "\n";

  text_section += "    mov eax, " + opdA + "\n";
  if (op == "+") {
    text_section += "    add eax, " + opdB + "\n";
  }
  if (op == "-") {
    text_section += "    sub eax, " + opdB + "\n";
  }
  text_section += "    mov " + lvar + ", eax\n";

  return lvar;
}

string Codegen::generate_negative_expression(NodeNegativeExpression *neg_exp) {
  if (neg_exp->neg_exp == NULL) {
    return generate_multiplicative_expression(neg_exp->mul_exp);
  }
  string lvar = get_new_temp_variable();

  string opd = generate_negative_expression(neg_exp->neg_exp);

  icode += lvar + " = " + "-1 * " + opd + "\n";

  text_section += "    mov eax, " + opd + "\n";
  text_section += "    imul eax, -1\n";
  text_section += "    mov " + lvar + ", eax\n";

  return lvar;
}

string Codegen::generate_multiplicative_expression(
    NodeMultiplicativeExpression *mul_exp) {
  if (mul_exp->mul_operator == NULL && mul_exp->mul_exp == NULL) {
    return generate_expression(mul_exp->exp);
  }
  string lvar = get_new_temp_variable();
  string opdA = generate_multiplicative_expression(mul_exp->mul_exp);
  string op = string(1, mul_exp->mul_operator->op);
  string opdB = generate_expression(mul_exp->exp);

  icode += lvar + " = " + opdA + " " + op + " " + opdB + "\n";

  text_section += "    mov eax, " + opdA + "\n";
  if (op == "*") {
    text_section += "    imul eax, " + opdB + "\n";
  }
  if (op == "/") {
    text_section += "    imul eax, " + opdB + "\n";
  }
  text_section += "    mov " + lvar + ", eax\n";
  return lvar;
}

string Codegen::generate_expression(NodeExpression *exp) {
  if (exp->identifier != NULL) {
    return "[" + get_identifier_name(exp->identifier) + "]";
  } else if (exp->literal != NULL) {
    return to_string(exp->literal->value);
  } else if (exp->comp_exp != NULL) {
    return generate_comparative_expression(exp->comp_exp);
  }
  return "";
}
