#ifndef OPCODES_H
#define OPCODES_H

#define         undef       0x00        // No action
#define         null        0x01        // Push a null reference onto the stack

#define         i_0         0x02        // Push 0 onto the stack
#define         i_1         0x03        // Push 1 onto the stack
#define         i_2         0x04        // Push 2 onto the stack
#define         i_3         0x05        // Push 3 onto the stack

#define         f_0         0x06        // Push 0.0f onto the stack
#define         f_1         0x07        // Push 1.0f onto the stack
#define         f_2         0x08        // Push 2.0f onto the stack
#define         f_3         0x09        // Push 3.0f onto the stack

#define         b_0         0x0a        // Push single byte 0 onto the stack
#define         b_1         0x0b        // Push single byte 1 onto the stack

#define         cload_0     0x0c        // Push constant from constant pool at index 0 onto the stack
#define         cload_1     0x0d        // Push constant from constant pool at index 1 onto the stack
#define         cload_2     0x0e        // Push constant from constant pool at index 2 onto the stack
#define         cload_3     0x0f        // Push constant from constant pool at index 3 onto the stack
#define         cload       0x10        // Push constant from constant pool at #index onto the stack

#define         lload_0     0x11        // Push value from the local scope at index 0 onto the stack
#define         lload_1     0x12        // Push value from the local scope at index 1 onto the stack
#define         lload_2     0x13        // Push value from the local scope at index 2 onto the stack
#define         lload_3     0x14        // Push value from the local scope at index 3 onto the stack
#define         lload       0x15        // Push value from the local scope at index 0 onto the stack

#define         oload_0     0x16        // Push the value at index 0 in the last object in the stack onto the stack
#define         oload_1     0x17        // Push the value at index 1 in the last object in the stack onto the stack
#define         oload_2     0x18        // Push the value at index 2 in the last object in the stack onto the stack
#define         oload_3     0x19        // Push the value at index 3 in the last object in the stack onto the stack
#define         oload       0x1a        // Push the value at #index in the last object in the stack onto the stack

#define         lstore_0    0x1b        // Store the last value on the stack in the local scope at index 0
#define         lstore_1    0x1c        // Store the last value on the stack in the local scope at index 1
#define         lstore_2    0x1d        // Store the last value on the stack in the local scope at index 2
#define         lstore_3    0x1e        // Store the last value on the stack in the local scope at index 3
#define         lstore      0x1f        // Store the last value on the stack in the local scopet at #index

#define         ostore_0    0x20        // Store the last value on the stack in the next object at index 0
#define         ostore_1    0x21        // Store the last value on the stack in the next object at index 1
#define         ostore_2    0x22        // Store the last value on the stack in the next object at index 2
#define         ostore_3    0x23        // Store the last value on the stack in the next object at index 3
#define         ostore      0x24        // Store the last value on the stack in the next object at #index

#define         add_i       0x25        // Push the sum of the last two integers in the stack onto the stack
#define         add_f       0x26        // Push the sum of the last two floats in stack onto the stack
#define         sub_i       0x27        // Push the difference of the last two integers in the stack onto the stack
#define         sub_f       0x28        // Push the difference of the last two floats in stack onto the stack
#define         mul_i       0x29        // Push the product of the last two integers in the stack onto the stack
#define         mul_f       0x2a        // Push the product of the last two floats in stack onto the stack
#define         div_i       0x2b        // Push the quotient of the last two integers in the stack onto the stack
#define         div_f       0x2c        // Push the quotient of the last two floats in stack onto the stack

#define         itof        0x2d        // Cast the last int on the stack to a float
#define         foi         0x2e        // Cast the last float on the stack to an int

#define         new_o       0x2f        // Push a new instance of the class at #index in the constant pool onto the stack
#define         new_a       0x30        // Push a new array with size given by int representation of the last value on the stack

#define         invoke_c    0x31        // Call function with signature at #index of constant pool
#define         invoke_a    0x32        // Call function with signature at top of the stack

#define         ret         0x33        // Return from function
#define         yield       0x34        // Return from process; Preserve state
#define         add_s       0x35        // Concatenate two strings

#define         dup_0       0x36        // Duplicate the last reference on the stack
#define         dup_1       0x37        // Duplicate the last raw object on the stack

#define         incr        0x38        // Increase the last value on the stack
#define         decr        0x39        // Decrease the last value on the stack

#define         ifeq        0x3a        // Push a bool indicating comparing == the last two values on the stack onto the stack
#define         iflt        0x3b        // Push a bool indicating comparing < the last two values on the stack onto the stack
#define         ifgt        0x3c        // Push a bool indicating comparing > the last two values on the stack onto the stack
#define         ifgq        0x3d        // Push a bool indicating comparing >= the last two values on the stack onto the stack
#define         iflq        0x3e        // Push a bool indicating comparing <= the last two values on the stack onto the stack
#define         ifnot       0x3f        // Negate the last bool on stack
#define         jif         0x40        // Jump to #index in file if last bool on stack is true
#define         jump        0x41        // Jump to #index in file

#define         incr_l      0x42        // Increate the current line holder (for call-stack tracing)



#endif