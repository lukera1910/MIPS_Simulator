#include <stdio.h>
#include <string.h>

#define NUM_REGISTERS 32

typedef struct {
    int registers[NUM_REGISTERS];
} MIPS_Registers;

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
    regs->registers[2] = 10; // $2 = 20
    regs->registers[3] = 20; // $3 = 10
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
    } else if (strcmp(operation, "addi") == 0) { // Tipo I
        sscanf(asm_code, "%*s $%d, $%d, %d", &rt, &rs, &imm);
        instr->opcode = 8; // Opcode para addi
        instr->rs = rs;
        instr->rt = rt;
        instr->imm = imm; // Valor imediato
    } else if (strcmp(operation, "sub") == 0) { // Tipo R
        sscanf(asm_code, "%*s $%d, $%d, $%d", &rd, &rs, &rt);
        instr->opcode = 0; // Tipo R
        instr->rs = rs;
        instr->rt = rt;
        instr->rd = rd;
        instr->shamt = 0;
        instr->funct = 34; // Código da função para sub
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

void execute_instruction(const Instruction* instr, MIPS_Registers* regs) {
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
            case 34: // sub
                regs->registers[instr->rd] =
                    regs->registers[instr->rs] - regs->registers[instr->rt];
                printf("Subtração realizada: %d - %d = %d\n",
                       regs->registers[instr->rs], 
                       regs->registers[instr->rt], 
                       regs->registers[instr->rd]);
                break;
        }
    } else if (instr->opcode == 8) { // Tipo I (addi)
        regs->registers[instr->rt] = 
            regs->registers[instr->rs] + instr->imm;
        printf("Operação Imediata: %d + %d = %d\n", 
               regs->registers[instr->rs], 
               instr->imm, 
               regs->registers[instr->rt]);
    } else if (instr->opcode == 2) { // j (jump)
        printf("Salto para o endereço: %d\n", instr->imm);
    } else if (instr->opcode == 3) { // jal (jump and link)
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
    Instruction instr;

    initialize_registers(&regs);

    char asm_code[50];
    printf("Digite uma instrução MIPS: ");
    fgets(asm_code, sizeof(asm_code), stdin);

    decode_instruction(asm_code, &instr);
    execute_instruction(&instr, &regs);

    print_registers(&regs);

    return 0;
}
