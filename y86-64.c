#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const int MAX_MEM_SIZE  = (1 << 13);

void fetchStage(int *icode, int *ifun, int *rA, int *rB, wordType *valC, wordType *valP) {
    wordType pcAddress = getPC();
    byteType firstByte = getByteFromMemory(pcAddress);
    *ifun = firstByte & 0x0f;
    *icode = (firstByte & 0xf0) >> 4;
    if (*icode == NOP) {
        *valP = pcAddress + 1;
    }
    if (*icode == HALT) {
        *valP = pcAddress + 1;
    }
    if (*icode == IRMOVQ) {
        byteType nextByte = getByteFromMemory(pcAddress + 1);
        *rB = nextByte & 0x0f;
        *rA = (nextByte & 0xf0) >> 4;
        *valC = getWordFromMemory(pcAddress + 2);
        *valP = pcAddress + 10;
    }
    if (*icode == RRMOVQ) {
        byteType nextByte = getByteFromMemory(pcAddress + 1);
        *rB = nextByte & 0x0f;
        *rA = (nextByte & 0xf0) >> 4;
        *valP = pcAddress + 2;
    }
    if (*icode == RMMOVQ) {
        byteType nextByte = getByteFromMemory(pcAddress + 1);
        *rB = nextByte & 0x0f;
        *rA = (nextByte & 0xf0) >> 4;
        *valC = getWordFromMemory(pcAddress + 2);
        *valP = pcAddress + 10;
    }
    if (*icode == MRMOVQ) {
        byteType nextByte = getByteFromMemory(pcAddress + 1);
        *rB = nextByte & 0x0f;
        *rA = (nextByte & 0xf0) >> 4;
        *valC = getWordFromMemory(pcAddress + 2);
        *valP = pcAddress + 10;
    }
    if (*icode == OPQ) {
        byteType nextByte = getByteFromMemory(pcAddress + 1);
        *rB = nextByte & 0x0f;
        *rA = (nextByte & 0xf0) >> 4;
        *valP = pcAddress + 2;
    }
    if (*icode == JXX) {
        *valC = getWordFromMemory(pcAddress + 1);
        *valP = pcAddress + 9;
    }
    if (*icode == CALL) {
        *valC = getWordFromMemory(pcAddress + 1);
        *valP = pcAddress + 9;
    }
    if (*icode == RET) {
        *valP = pcAddress + 1;
    }
    if (*icode == PUSHQ) {
        byteType nextByte = getByteFromMemory(pcAddress + 1);
        *rB = nextByte & 0x0f;
        *rA = (nextByte & 0xf0) >> 4;
        *valP = pcAddress + 2;
    }
    if (*icode == POPQ) {
        byteType nextByte = getByteFromMemory(pcAddress + 1);
        *rB = nextByte & 0x0f;
        *rA = (nextByte & 0xf0) >> 4;
        *valP = pcAddress + 2;
    }
}

void decodeStage(int icode, int rA, int rB, wordType *valA, wordType *valB) {
    if (icode == RRMOVQ) {
        *valA = getRegister(rA);
    }
    if (icode == RMMOVQ) {
        *valA = getRegister(rA);
        *valB = getRegister(rB);
    }
    if (icode == MRMOVQ) {
        *valB = getRegister(rB);
    }
    if (icode == OPQ) {
        *valA = getRegister(rA);
        *valB = getRegister(rB);
    }
    if (icode == CALL) {
        *valB = getRegister(RSP);
    }
    if (icode == RET) {
        *valA = getRegister(RSP);
        *valB = getRegister(RSP);
    }
    if (icode == PUSHQ) {
        *valA = getRegister(rA);
        *valB = getRegister(RSP);
    }
    if (icode == POPQ) {
        *valA = getRegister(RSP);
        *valB = getRegister(RSP);
    }
}

void executeStage(int icode, int ifun, wordType valA, wordType valB, wordType valC, wordType *valE, bool *Cnd) {
    if (icode == IRMOVQ) {
        *valE = valC;
    }
    if (icode == RRMOVQ) {
        *valE = valA;
    }
    if (icode == RMMOVQ) {
        *valE = valB + valC;
    }
    if (icode == MRMOVQ) {
        *valE = valB + valC;
    }
    if (icode == OPQ) {
        if (ifun == ADD) {
            wordType result = valA + valB;
            *valE = result;
            bool overflow = FALSE;
            if (valA > 0 && valB > 0 && result < 0)
                overflow = TRUE;
            if (valA < 0 && valB < 0 && result > 0)
                overflow = TRUE;
            setFlags(result > 0, result == 0, overflow);
        }
        if (ifun == SUB) {
            wordType result = valA - valB;
            *valE = result;
            bool overflow = FALSE;
            if (valA > 0 && valB > 0 && result < 0)
                overflow = TRUE;
            if (valA < 0 && valB < 0 && result > 0)
                overflow = TRUE;
            setFlags(result > 0, result == 0, overflow);
        }
        if (ifun == AND) {
            wordType result = valA & valB;
            *valE = result;
            bool overflow = FALSE;
            if (valA > 0 && valB > 0 && result < 0)
                overflow = TRUE;
            if (valA < 0 && valB < 0 && result > 0)
                overflow = TRUE;
            setFlags(result > 0, result == 0, overflow);
        }
        if (ifun == XOR) {
            wordType result = valA ^ valB;
            *valE = result;
            bool overflow = FALSE;
            if (valA > 0 && valB > 0 && result < 0)
                overflow = TRUE;
            if (valA < 0 && valB < 0 && result > 0)
                overflow = TRUE;
            setFlags(result > 0, result == 0, overflow);
        }
    }
    if (icode == JXX) {
        *Cnd = Cond(ifun);
    }
    if (icode == CALL) {
        int negativeEight = -8; // TODO: make sure that this is how you do negative in C
        *valE = valB + negativeEight;
    }
    if (icode == RET) {
        *valE = valB + 8;
    }
    if (icode == PUSHQ) {
        int negativeEight = -8;
        *valE = valB + negativeEight;
    }
    if (icode == POPQ) {
        *valE = valB + 8;
    }
}

void memoryStage(int icode, wordType valA, wordType valP, wordType valE, wordType *valM) {
    if (icode == RMMOVQ) {
        setWordInMemory(valE, valA);
    }
    if (icode == MRMOVQ) {
        *valM = getWordFromMemory(valE);
    }
    if (icode == CALL) {
        setWordInMemory(valE, valP);
    }
    if (icode == RET) {
        *valM = getWordFromMemory(valA);
    }
    if (icode == PUSHQ) {
        setWordInMemory(valE, valA);
    }
    if (icode == POPQ) {
        *valM = getWordFromMemory(valA);
    }
}

void writebackStage(int icode, int rA, int rB, wordType valE, wordType valM) {
    if (icode == IRMOVQ) {
        setRegister(rB, valE);
    }
    if (icode == RRMOVQ) {
        setRegister(rB, valE);
    }
    if (icode == MRMOVQ) {
        setRegister(rA, valM);
    }
    if (icode == OPQ) {
        setRegister(rB, valE);
    }
    if (icode == CALL) {
        setRegister(RSP, valE);
    }
    if (icode == RET) {
        setRegister(RSP, valE);
    }
    if (icode == PUSHQ) {
        setRegister(RSP, valE);
    }
    if (icode == POPQ) {
        setRegister(RSP, valE);
        setRegister(rA, valM);
    }
}

void pcUpdateStage(int icode, wordType valC, wordType valP, bool Cnd, wordType valM) {
    if (icode == NOP) {
        setPC(valP);
    }
    if (icode == HALT) {
        setStatus(STAT_HLT);
        setPC(valP);
    }
    if (icode == IRMOVQ) {
        setPC(valP);
    }
    if (icode == RRMOVQ) {
        setPC(valP);
    }
    if (icode == RMMOVQ) {
        setPC(valP);
    }
    if (icode == MRMOVQ) {
        setPC(valP);
    }
    if (icode == OPQ) {
        setPC(valP);
    }
    if (icode == JXX) {
        if (Cnd == TRUE) {
            setPC(valC);
        }
        else {
            setPC(valP);
        }
    }
    if (icode == CALL) {
        setPC(valP);
    }
    if (icode == RET) {
        setPC(valP);
    }
    if (icode == PUSHQ) {
        setPC(valP);
    }
    if (icode == POPQ) {
        setPC(valP);
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
