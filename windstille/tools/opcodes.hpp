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

#ifndef HEADER_OPCODES_HPP
#define HEADER_OPCODES_HPP

enum {
  VM_HALT,
  VM_PAUSE,
  VM_CONTINUE,
  VM_ERROR
};

enum {
  OPCODE_LESS,
  OPCODE_LESSEQUAL,
  OPCODE_GREATER,
  OPCODE_GREATEREQUAL,
  OPCODE_NOTEQUAL,
  OPCODE_EQUAL,
  OPCODE_ADD,
  OPCODE_SUB,
  OPCODE_SHIFTLEFT,
  OPCODE_SHIFTRIGHT,
  OPCODE_OROR,
  OPCODE_ANDAND,
  OPCODE_OR,
  OPCODE_AND,
  OPCODE_XOR,
  OPCODE_DIV,
  OPCODE_MOD,
  OPCODE_MULT ,

  OPCODE_PUSH_INT_CONST,

  OPCODE_SET_VARIABLE,
  OPCODE_PUSH_VARIABLE,

  OPCODE_SET_STACK_VARIBALE,
  OPCODE_PUSH_STACK_VARIBALE,

  OPCODE_HALT,

  OPCODE_JUMP_FALSE,
  OPCODE_JUMP_TRUE,
  OPCODE_JUMP,

  OPCODE_DUPLICATE,
  OPCODE_PRINT,

  OPCODE_TRACE
};

#endif 

/* EOF */
