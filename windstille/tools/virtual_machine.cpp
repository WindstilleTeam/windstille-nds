/*  $Id: windstille_main.hpp 1274 2006-09-05 19:10:18Z grumbel $
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <iostream>
#include "opcodes.hpp"
#include "virtual_machine.hpp"

// This code is in some parts inspired by PSL from Steve Baker

VirtualMachine::VirtualMachine(int* code_) 
  : code(code_), pc(0), sp(-1)
{
  for(int i = 0; i < 64; ++i)
    variables[i] = 0;
}

void
VirtualMachine::error(const char* msg)
{
  std::cout << "Error: " << msg << std::endl;
  for(int i = 0; i <= sp; ++i)
    std::cout << "  Stack " << i << ": " << stack[i] << std::endl;
  std::cout << std::endl;
}

int
VirtualMachine::peekInt()
{
  return stack[sp];
}

int
VirtualMachine::popInt()
{  
  return stack[sp--];
}

void
VirtualMachine::pushInt(int i)
{  
  stack[++sp] = i;
}

int
VirtualMachine::step()
{
  switch(code[pc])
    {
    case OPCODE_LESS:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs < rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_LESSEQUAL:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs <= rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_GREATER:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs > rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_GREATEREQUAL:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs >= rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_NOTEQUAL:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs != rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_EQUAL:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs == rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_ADD:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs + rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_SUB:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs - rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_SHIFTLEFT:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs << rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_SHIFTRIGHT:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs >> rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_OROR:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs || rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_ANDAND: 
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs && rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_OR: 
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs | rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_AND:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs & rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_XOR:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs ^ rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_DIV:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs / rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_MOD:
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs % rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_MULT: 
      {
        int lhs = popInt();
        int rhs = popInt();
        pushInt(lhs * rhs);
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_PUSH_INT_CONST:
      {
        pushInt(code[pc+1]);
        pc += 2;
      }
      return VM_CONTINUE;

    case OPCODE_SET_VARIABLE:
      {
        int var   = code[pc+1];
        int value = popInt();
        variables[var] = value;
        pc += 2;
      }
      return VM_CONTINUE;

    case OPCODE_PUSH_VARIABLE:
      {
        pushInt(variables[code[pc+1]]);
        pc += 2;
      }
      return VM_CONTINUE;

    case OPCODE_SET_STACK_VARIBALE:
      {
        int v = popStack();
        stack[sp - code[pc+1]] = v;
        pc += 2;
      }
      break;
      
    case OPCODE_PUSH_STACK_VARIBALE:
      {
        pushInt(stack[sp - code[pc+1]]);
        pc += 2;
      }
      break; 

    case OPCODE_HALT:
      return VM_HALT;

    case OPCODE_JUMP_TRUE:
      {
        if (popInt())
          pc = code[pc+1];
        else
          pc += 2;
      }     
      return VM_CONTINUE;

    case OPCODE_JUMP_FALSE:
      {
        if (!popInt())
          pc = code[pc+1];
        else
          pc += 2;
      }
      return VM_CONTINUE;
    
    case OPCODE_JUMP:
      {
        pc = code[pc+1];
      }
      return VM_CONTINUE;

    case OPCODE_PRINT:
      {
        std::cout << popInt() << std::endl;
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_DUPLICATE:
      {
        pushInt(peekInt());
        pc += 1;
      }
      return VM_CONTINUE;

    case OPCODE_TRACE:
      {
        std::cout << "Trace: " << std::endl;
        for(int i = 0; i <= sp; ++i)
          std::cout << "  Stack " << i << ": " << stack[i] << std::endl;
        std::cout << std::endl;

        for(int i = 0; i < 3; ++i)
          std::cout << "  Variable[" << i << "] = " << variables[i] << std::endl;
        pc += 1;
      }
      return VM_CONTINUE;

    default:
      // Unknown opcode
      return VM_ERROR;
    }
}

int
VirtualMachine::run()
{
  int ret;
  while((ret = step()) == VM_CONTINUE);
  return ret;
}

/* EOF */
