#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const int MAX_MEM_SIZE  = (1 << 13);

void fetchStage(int *icode, int *ifun, int *rA, int *rB, wordType *valC, wordType *valP) {
    // valC and valP are placeholders for values
    // if it is 8 bits instead of 1, use getWord instead of getByte
    wordType pcAddress = getPC();   // get the address of the PC
    // grab the first byte from the address
    byteType firstByte = getByteFromMemory(pcAddress);  // get the first byte of the address
    *ifun = firstByte & 0x0f;
    *icode = (firstByte & 0xf0) >> 4;
    if (*icode == NOP) {

    }
    if (*icode == HALT) {

    }
    if (*icode == IRMOVQ) {

    }
    if (*icode == RRMOVQ) {

    }
    if (*icode == RMMOVQ) {

    }
    if (*icode == MRMOVQ) {

    }
    if (*icode == OPQ) {

    }
    if (*icode == JXX) {

    }
    if (*icode == CALL) {

    }
    if (*icode == RET) {

    }
    if (*icode == PUSHQ) {

    }
    if (*icode == POPQ) {

    }
}

void decodeStage(int icode, int rA, int rB, wordType *valA, wordType *valB) {
    if (icode == IRMOVQ) {

    }
    if (icode == RRMOVQ) {

    }
    if (icode == RMMOVQ) {

    }
    if (icode == MRMOVQ) {

    }
    if (icode == OPQ) {

    }
    if (icode == JXX) {

    }
    if (icode == CALL) {

    }
    if (icode == RET) {

    }
    if (icode == PUSHQ) {

    }
    if (icode == POPQ) {

    }
}

void executeStage(int icode, int ifun, wordType valA, wordType valB, wordType valC, wordType *valE, bool *Cnd) {
    if (icode == IRMOVQ) {

    }
    if (icode == RRMOVQ) {

    }
    if (icode == RMMOVQ) {

    }
    if (icode == MRMOVQ) {

    }
    if (icode == OPQ) {

    }
    if (icode == JXX) {

    }
    if (icode == CALL) {

    }
    if (icode == RET) {

    }
    if (icode == PUSHQ) {

    }
    if (icode == POPQ) {

    }
}

void memoryStage(int icode, wordType valA, wordType valP, wordType valE, wordType *valM) {
    if (icode == IRMOVQ) {

    }
    if (icode == RRMOVQ) {

    }
    if (icode == RMMOVQ) {

    }
    if (icode == MRMOVQ) {

    }
    if (icode == OPQ) {

    }
    if (icode == JXX) {

    }
    if (icode == CALL) {

    }
    if (icode == RET) {

    }
    if (icode == PUSHQ) {

    }
    if (icode == POPQ) {

    }
}

void writebackStage(int icode, int rA, int rB, wordType valE, wordType valM) {
    if (icode == IRMOVQ) {

    }
    if (icode == RRMOVQ) {

    }
    if (icode == RMMOVQ) {

    }
    if (icode == MRMOVQ) {

    }
    if (icode == OPQ) {

    }
    if (icode == JXX) {

    }
    if (icode == CALL) {

    }
    if (icode == RET) {

    }
    if (icode == PUSHQ) {

    }
    if (icode == POPQ) {

    }
}

void pcUpdateStage(int icode, wordType valC, wordType valP, bool Cnd, wordType valM) {
    if (icode == NOP) {

    }
    if (icode == HALT) {

    }
    if (icode == IRMOVQ) {

    }
    if (icode == RRMOVQ) {

    }
    if (icode == RMMOVQ) {

    }
    if (icode == MRMOVQ) {

    }
    if (icode == OPQ) {

    }
    if (icode == JXX) {

    }
    if (icode == CALL) {

    }
    if (icode == RET) {

    }
    if (icode == PUSHQ) {

    }
    if (icode == POPQ) {

    }
}

void stepMachine(int stepMode) {
    /* FETCH STAGE */
    int icode = 0, ifun = 0;
    int rA = 0, rB = 0;
    wordType valC = 0;
    wordType valP = 0;

    /* DECODE STAGE */
    wordType valA = 0;
    wordType valB = 0;

    /* EXECUTE STAGE */
    wordType valE = 0;
    bool Cnd = 0;

    /* MEMORY STAGE */
    wordType valM = 0;

    fetchStage(&icode, &ifun, &rA, &rB, &valC, &valP);
    applyStageStepMode(stepMode, "Fetch", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

    decodeStage(icode, rA, rB, &valA, &valB);
    applyStageStepMode(stepMode, "Decode", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

    executeStage(icode, ifun, valA, valB, valC, &valE, &Cnd);
    applyStageStepMode(stepMode, "Execute", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

    memoryStage(icode, valA, valP, valE, &valM);
    applyStageStepMode(stepMode, "Memory", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

    writebackStage(icode, rA, rB, valE, valM);
    applyStageStepMode(stepMode, "Writeback", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

    pcUpdateStage(icode, valC, valP, Cnd, valM);
    applyStageStepMode(stepMode, "PC update", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

    incrementCycleCounter();
}

/**
 * main
 * */
int main(int argc, char **argv) {
    int stepMode = 0;
    FILE *input = parseCommandLine(argc, argv, &stepMode);

    initializeMemory(MAX_MEM_SIZE);
    initializeRegisters();
    loadMemory(input);

    applyStepMode(stepMode);
    while (getStatus() != STAT_HLT) {
        stepMachine(stepMode);
        applyStepMode(stepMode);
#ifdef DEBUG
        printMachineState();
    printf("\n");
#endif
    }
    printMachineState();
    return 0;
}
