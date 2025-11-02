#include "vm.h"

Vm vm;  // Static variable

void initVM() {

}

void freeVM() {

}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}


static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)

    while(1) {
        
    }

#undef READ_BYTE
}