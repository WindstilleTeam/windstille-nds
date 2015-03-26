#include <iostream>
#include "virtual_machine.hpp"
#include "opcodes.hpp"

int main()
{
  int fib_code[] = { 
    // Fibonacci Numbers
    OPCODE_PUSH_INT_CONST,  0,
    OPCODE_SET_VARIABLE,    0, 

    OPCODE_PUSH_INT_CONST,  1,
    OPCODE_SET_VARIABLE,    1, 
    
    OPCODE_PUSH_VARIABLE,   0,
    OPCODE_PRINT,

    // Loop start
    OPCODE_PUSH_VARIABLE,   0,
    OPCODE_PUSH_VARIABLE,   1,
    OPCODE_ADD,

    OPCODE_PUSH_VARIABLE,   1,
    OPCODE_SET_VARIABLE,    0,

    OPCODE_SET_VARIABLE,    1,

    OPCODE_PUSH_INT_CONST, 10000,
    OPCODE_PUSH_VARIABLE,   0,
    OPCODE_LESS,

    OPCODE_JUMP_TRUE,       8,

    OPCODE_HALT 
  };

  int prim_code[] = {
    OPCODE_PUSH_INT_CONST,  1,
    OPCODE_SET_VARIABLE,    0,

    OPCODE_PUSH_VARIABLE,   0,
    OPCODE_PRINT,

    OPCODE_PUSH_VARIABLE,   0,
    OPCODE_PUSH_INT_CONST,  1,
    OPCODE_ADD,

    OPCODE_SET_VARIABLE,    0,

    OPCODE_JUMP,    4,
    OPCODE_HALT 
  };

  int primtest_code[] = {
    OPCODE_PUSH_INT_CONST,  45,
    
    OPCODE_PUSH_INT_CONST,  1,
    OPCODE_SET_VARIABLE,    0,

    OPCODE_PUSH_VARIABLE,   0,
    OPCODE_PRINT,

    // Label
    OPCODE_PUSH_VARIABLE,   0,
    OPCODE_PUSH_INT_CONST,  1,
    OPCODE_ADD,

    OPCODE_SET_VARIABLE,    0,
    
    OPCODE_PUSH_INT_CONST, 1000,
    OPCODE_PUSH_VARIABLE,   0,
    OPCODE_LESS,

    OPCODE_JUMP_TRUE,       6,

    OPCODE_HALT 
  };

  VirtualMachine vm(primtest_code);
  switch (vm.run())
    {
    case VM_PAUSE:
      std::cout << "success: pause" << std::endl;
      break;

    case VM_HALT:
      std::cout << "success: halt " << std::endl;
      break;

    case VM_ERROR:
      std::cout << "error: error" << std::endl;
      break;

    default:
      std::cout << "Unknown return state" << std::endl;
    }
}

/* EOF */
