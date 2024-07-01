#include "./headers/codegen.h"

Codegen::Codegen() {
  data_section = "";
  bss_section = "";
  text_section = "";
  print_integer_routine = false;
  symbol_table = SymbolTable::get_instance();
}

void Codegen::push_eax() { text_section += "    push eax\n"; }

void Codegen::pop_eax() { text_section += "    pop eax\n"; }

void Codegen::declare_variable_data_section(string variable_name, int value) {
  data_section += "    " + variable_name + " dd " + to_string(value) + "\n";
}

void Codegen::generate_debug(string variable_name) {
  if (!print_integer_routine) {
    print_integer_routine = true;
  }
  push_eax();
  text_section +=
      "    mov eax, [" + variable_name + "]\n    call print_integer\n";
  pop_eax();
}

void Codegen::generate_debug(int value) {
  if (!print_integer_routine) {
    print_integer_routine = true;
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
  if (print_integer_routine) {
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

  if (print_integer_routine) {
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
  // if (debug->identifier) {
  //   generate_debug(debug->identifier->name);
  // } else if (debug->INT) {
  //   generate_debug(debug->INT->value);
  // }
}

void Codegen::traverse_let(NodeLet *let) {
  // declare_variable_data_section(let->identifier->name, let->INT->value);
}