#include <stdio.h>
#include <string.h>

#define NUM_REGISTERS 32
#define MEMORY_SIZE 1024

typedef struct {
    int registers[NUM_REGISTERS];
} MIPS_Registers;

typedef struct {
    int memory[MEMORY_SIZE];
} MIPS_Memory;

typedef struct {
    int opcode;
    int rs;
    int rt;
    int rd;
    int shamt;
    int funct;
    int imm;
} Instruction;

void initialize_registers(MIPS_Registers* regs) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        regs->registers[i] = 0;
    }
    regs->registers[2] = 10; // $2 = 10
    regs->registers[3] = 20; // $3 = 20
}

void initialize_memory(MIPS_Memory* mem) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        mem->memory[i] = i * 2; // Exemplo: Preenche a memória com valores conhecidos
    }
}

void decode_instruction(const char* asm_code, Instruction* instr) {
    char operation[10];
    int rs, rt, rd, imm;

    sscanf(asm_code, "%s", operation);

    if (strcmp(operation, "add") == 0) { // Tipo R
        sscanf(asm_code, "%*s $%d, $%d, $%d", &rd, &rs, &rt);
        instr->opcode = 0; // Tipo R
        instr->rs = rs;
        instr->rt = rt;
        instr->rd = rd;
        instr->shamt = 0;
        instr->funct = 32; // Código da função para add
    } else if (strcmp(operation, "and") == 0) { // Tipo R
        sscanf(asm_code, "%*s $%d, $%d, $%d", &rd, &rs, &rt);
        instr->opcode = 0; // Tipo R
        instr->rs = rs;
        instr->rt = rt;
        instr->rd = rd;
        instr->shamt = 0;
        instr->funct = 36; // Código da função para and
    } else if (strcmp(operation, "addi") == 0) { // Tipo I
        sscanf(asm_code, "%*s $%d, $%d, %d", &rt, &rs, &imm);
        instr->opcode = 8; // Opcode para addi
        instr->rs = rs;
        instr->rt = rt;
        instr->imm = imm; // Valor imediato
    } else if (strcmp(operation, "lw") == 0) { // Tipo I
        sscanf(asm_code, "%*s $%d, %d($%d)", &rt, &imm, &rs);
        instr->opcode = 35; // Opcode para lw
        instr->rs = rs;
        instr->rt = rt;
        instr->imm = imm;
    } else if (strcmp(operation, "beq") == 0) { // Tipo I
        sscanf(asm_code, "%*s $%d, $%d, %d", &rs, &rt, &imm);
        instr->opcode = 4; // Opcode para beq
        instr->rs = rs;
        instr->rt = rt;
        instr->imm = imm;
    } else if (strcmp(operation, "j") == 0) { // Tipo J
        sscanf(asm_code, "%*s %d", &imm);
        instr->opcode = 2; // Opcode para j
        instr->imm = imm; // Alvo do salto
    } else if (strcmp(operation, "jal") == 0) { // Tipo J
        sscanf(asm_code, "%*s %d", &imm);
        instr->opcode = 3; // Opcode para jal
        instr->imm = imm; // Alvo do salto
    }
}

void execute_instruction(const Instruction* instr, MIPS_Registers* regs, MIPS_Memory* mem) {
    if (instr->opcode == 0) { // Tipo R
        switch (instr->funct) {
            case 32: // add
                regs->registers[instr->rd] =
                    regs->registers[instr->rs] + regs->registers[instr->rt];
                printf("Soma realizada: %d + %d = %d\n",
                       regs->registers[instr->rs],
                       regs->registers[instr->rt],
                       regs->registers[instr->rd]);
                break;
            case 36: // and
                regs->registers[instr->rd] =
                    regs->registers[instr->rs] & regs->registers[instr->rt];
                printf("Operação AND: %d & %d = %d\n",
                       regs->registers[instr->rs],
                       regs->registers[instr->rt],
                       regs->registers[instr->rd]);
                break;
        }
    } else if (instr->opcode == 8) { // addi
        regs->registers[instr->rt] =
            regs->registers[instr->rs] + instr->imm;
        printf("Operação Imediata: %d + %d = %d\n",
               regs->registers[instr->rs],
               instr->imm,
               regs->registers[instr->rt]);
    } else if (instr->opcode == 35) { // lw
        regs->registers[instr->rt] = mem->memory[regs->registers[instr->rs] + instr->imm];
        printf("Carregando da memória: Mem[%d] = %d\n",
               regs->registers[instr->rs] + instr->imm,
               regs->registers[instr->rt]);
    } else if (instr->opcode == 4) { // beq
        if (regs->registers[instr->rs] == regs->registers[instr->rt]) {
            printf("Branch taken: Saltando para o offset %d\n", instr->imm);
        } else {
            printf("Branch not taken: Não saltando\n");
        }
    } else if (instr->opcode == 2) { // j
        printf("Salto para o endereço: %d\n", instr->imm);
    } else if (instr->opcode == 3) { // jal
        printf("Salto para o endereço: %d, registrando retorno\n", instr->imm);
        regs->registers[31] = instr->imm;
    }
}

void print_registers(const MIPS_Registers* regs) {
    printf("\n--- Estado Atual dos Registradores ---\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("$%d: %d\n", i, regs->registers[i]);
    }
}

int main() {
    MIPS_Registers regs;
    MIPS_Memory mem;
    Instruction instr;

    initialize_registers(&regs);
    initialize_memory(&mem);

    char asm_code[50];
    printf("Digite uma instrução MIPS: ");
    fgets(asm_code, sizeof(asm_code), stdin);

    decode_instruction(asm_code, &instr);
    execute_instruction(&instr, &regs, &mem);

    print_registers(&regs);

    return 0;
}
