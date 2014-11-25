
#include <stdio.h>
#include <stdint.h>

typedef union REG {
    intptr_t i;
    int _i;
    void *v;
    union REG *r;
} REG;

#define LPAREN (
#define RPAREN )
#define MACRO(N) ); N##_MACRO LPAREN

#define push MACRO(PUSH)
#define pop  MACRO(POP)
#define mov  MACRO(MOV)
#define sub  MACRO(SUB)
#define add  MACRO(ADD)
#define imul MACRO(IMUL)
#define cmp  MACRO(CMP)
#define jge  MACRO(JGE)
#define jmp  MACRO(JMP)
#define call MACRO(CALL)
#define ret  MACRO(RET) _
#define label MACRO(LABEL)

#define NO_OP(X) 

#define PUSH_MACRO(VAL) \
    *(esp -= 4) = (REG)(VAL)

#define POP_MACRO(DST) (DST) = (typeof(DST))(esp->i); esp += 4

#define MOV_MACRO(VAL, DST) (DST) = (typeof(DST))((REG)VAL).i;

#define SUB_MACRO(VAL, DST) \
    CMP_MACRO(VAL, DST); \
    (DST) = (typeof(DST))(((REG)DST).i - ((REG)VAL).i)

#define ADD_MACRO(VAL, DST) \
    DST = (typeof(DST))(((REG)DST).i + ((REG)VAL).i); \
    ZF = ((REG)DST).i == 0; \
    OF = 0; \
    SF = ((REG)DST).i < 0

#define IMUL_MACRO(VAL, DST) \
    DST = (typeof(DST))(((REG)DST).i * ((REG)VAL).i); \
    ZF = ((REG)DST).i == 0; \
    OF = 0; \
    SF = ((REG)DST).i < 0

#define CMP_MACRO(L, R) \
    CMP_MACRO_(((REG)L).i, ((REG)R).i)

#define CMP_MACRO_(L, R) \
    (OF = 0, ZF = L == R, SF = (R - L) < 0)

#define JGE_MACRO(TGT) \
    if (SF == OF) \
    { \
        goto TGT; \
    } \
    else \
    { \
        \
    }

#define JMP_MACRO(TGT) \
    goto TGT;

#define CALL_MACRO(PROC) \
    CALL_MACRO_(PROC, __COUNTER__)

#define CALL_MACRO_(PROC, CTR) \
    PUSH_MACRO(CTR - STARTIP); \
    goto PROC; \
    case CTR - STARTIP:

#define RET_MACRO(_) \
    eip = esp->i; \
    esp += 4; \
    if (eip) \
    { \
        continue; \
    } \
    else \
    { \
        goto *finalreturn; \
    }

#define LABEL_MACRO(NAME) \
    NAME

#define MY_ASM(X) \
    do \
    { \
        enum{ STARTIP = __COUNTER__}; \
        switch(eip) \
        { \
            case 0: \
                MY_ASM_1 X \
        } \
    } while (1);

#define MY_ASM_1(X) \
    MY_ASM_2(NO_OP LPAREN 0 X RPAREN;)

#define MY_ASM_2(X) \
    X

#define CAT(L, R) \
    _CAT(L, R)

#define _CAT(L, R) \
    L##R

#define callASM(F) \
    callASM_(F, CAT(_TMP_, __COUNTER__))

#define callASM_(F, LABEL) \
    (\
        ({ \
            PUSH_MACRO(0); \
            stackbase = esp; \
            finalreturn = &&LABEL; \
            goto F; \
            LABEL:; \
        }), \
        (intptr_t)eax \
    )


/*
const int STACKSIZE = 4096;
*/
enum{STACKSIZE = 4096};
REG callstack[STACKSIZE];
REG* stackbase;
REG* eax;
REG* ecx;
REG* edx;
REG* ebx;
REG* esi;
REG* edi;
REG* esp;
REG* ebp;
int SF;
int ZF;
int OF;
int eip;
void* finalreturn;

int main(void)
{
    eax = ecx = edx = ebx = esi = edi = esp = ebp = &callstack[STACKSIZE - 1];
    eip = 0;
    finalreturn = &&TOP; TOP:

    PUSH_MACRO(10);
    int a = callASM(_fac);
    PUSH_MACRO(10);
    int b = callASM(_fib);

    printf("%d %d\n", a, b);
    return 0;


    MY_ASM((
        label _fac:                                   // @fac
            push ebp
            mov esp, ebp
            sub 24, esp
            mov 8[ebp], eax
            mov eax, (-8)[ebp]
            cmp 1, (-8)[ebp]
            jge LBB0_2
            mov 1, (-4)[ebp]
            jmp LBB0_3
        label LBB0_2:
            mov (-8)[ebp], eax
            mov (-8)[ebp], ecx
            sub 1, ecx
            mov ecx, *esp
            mov eax, (-12)[ebp]         // 4-byte Spill
            call _fac
            mov (-12)[ebp], ecx         // 4-byte Reload
            imul eax, ecx
            mov ecx, (-4)[ebp]
        label LBB0_3:
            mov (-4)[ebp], eax
            add 24, esp
            pop ebp
            ret

        label _fib:                                   // @fib
            push ebp
            mov esp, ebp
            sub 24, esp
            mov 8[ebp], eax
            mov eax, (-8)[ebp]
            cmp 2, (-8)[ebp]
            jge LBB1_2
            mov (-8)[ebp], eax
            mov eax, (-4)[ebp]
            jmp LBB1_3
        label LBB1_2:
            mov (-8)[ebp], eax
            sub 1, eax
            mov eax, *esp
            call _fib
            mov (-8)[ebp], ecx
            sub 2, ecx
            mov ecx, *esp
            mov eax, (-12)[ebp]         // 4-byte Spill
            call _fib
            mov (-12)[ebp], ecx         // 4-byte Reload
            add eax, ecx
            mov ecx, (-4)[ebp]
        label LBB1_3:
            mov (-4)[ebp], eax
            add 24, esp
            pop ebp
            ret
    ))
}

