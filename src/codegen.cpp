#include "./headers/codegen.h"

Codegen::Codegen() {
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

void Codegen::export_asm() {
  string program = "";

  // --- Data Section --- //
  program += "section .data\n";
  program += data_section + "\n";

  // --- Text Section --- //

  program += "section .text\n    global _start\n\n_start:\n";
  program += text_section;

  text_section = "";
  // --- Program Termination --- //
  program += R"(
    call _print_newline_subroutine
    mov eax, 1
    xor ebx, ebx
    int 0x80
  )";

  while (!function_definations.empty()) {
    traverse_function(function_definations.front());
    function_definations.pop();
  }

  program += text_section;

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

  cout << program << "\n ";

  ofstream outfile("./build/Assembly/program.asm");
  if (outfile.is_open()) {
    outfile << program;
    outfile.close();
  } else {
    cerr << "Error: ASM Code Generation Failed\n";
  }
}

void Codegen::generate_parse_tree(NodeProgram *program) {
  cout << "\n<--- PROGRAM START --->\n\n";
  generate_stmt_list(program->stmt_list);
  cout << "\n<--- PROGRAM END   --->\n\n";
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
    traverse_for(stmt->FOR);
  } else if (stmt->function) {
    function_definations.push(stmt->function);
  } else if (stmt->function_call) {
    traverse_function_call(stmt->function_call);
  }
}

void Codegen::generate_debug(NodeDebug *debug) {
  string rval = generate_comparative_expression(debug->comp_exp);

  cout << "dbg " << rval << "\n\n";

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

  cout << "let " << lvar << " = " << rvar << "\n\n";

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

  cout << "if " << condition << " :\n";

  text_section += "    mov eax, " + condition + "\n";
  text_section += "    cmp eax, 1\n";
  text_section += "    jne " + if_label_start + "\n\n";

  generate_stmt_list(stmt_list_if);

  text_section += "    jmp " + if_label_end + "\n\n";
  text_section += if_label_start + ":\n";

  if (stmt_list_else) {
    cout << "else :\n";
    generate_stmt_list(stmt_list_else);
  }

  text_section += if_label_end + ":\n";
}

void Codegen::generate_assign(NodeAssign *assign) {
  string rval = generate_comparative_expression(assign->comp_exp);
  string lval = "[" + get_identifier_name(assign->identifier) + "]";
  cout << lval << " = " << rval << "\n\n";

  text_section += "    mov eax, " + rval + "\n";
  text_section += "    mov " + lval + ", eax\n";

  Codegen::reset_count();
}

void Codegen::traverse_for(NodeFor *FOR) {
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
}

void Codegen::traverse_function(NodeFunction *function) {
  text_section += "\n";
  text_section += function->function_identifier + ":\n";
  generate_stmt_list(function->stmt_list);
  text_section += "    ret\n";
}

void Codegen::traverse_function_call(NodeFunctionCall *function_call) {
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

  cout << lvar << " = " << opdA << " " << op << " " << opdB << "\n";

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
    return traverse_multiplicative_expression(add_exp->mul_exp);
  }
  string lvar = get_new_temp_variable();

  string opdA = generate_additive_expression(add_exp->add_exp);
  string op = string(1, add_exp->add_operator->op);
  string opdB = traverse_multiplicative_expression(add_exp->mul_exp);

  cout << lvar << " = " << opdA << " " << op << " " << opdB << "\n";

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

string Codegen::traverse_multiplicative_expression(
    NodeMultiplicativeExpression *mul_exp) {
  if (mul_exp->mul_operator == NULL && mul_exp->mul_exp == NULL) {
    return traverse_expression(mul_exp->exp);
  }
  string lvar = get_new_temp_variable();
  string opdA = traverse_multiplicative_expression(mul_exp->mul_exp);
  string op = string(1, mul_exp->mul_operator->op);
  string opdB = traverse_expression(mul_exp->exp);

  cout << lvar << " = " << opdA << " " << op << " " << opdB << "\n";

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

string Codegen::traverse_expression(NodeExpression *exp) {
  if (exp->identifier != NULL) {
    return "[" + get_identifier_name(exp->identifier) + "]";
  } else {
    return to_string(exp->literal->value);
  }
}
