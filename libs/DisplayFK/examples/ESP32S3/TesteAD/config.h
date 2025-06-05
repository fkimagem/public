#include <Arduino.h>
#include <TecladoExterno.h>

// Escolha o tipo de teclado
#define TECLADO_20



#ifdef TECLADO_20
#define pinA 17 // AD2 metade direita das teclas
#define pinB 14 // AD1 metade esquerda das teclas
#define QTD_KEYPAD 2
#define keyPerKeyboard 10
#endif

#ifdef TECLADO_16
#define pinA 17 // AD2 metade de baixo
#define pinB 14 // AD1 metade de cima
#define QTD_KEYPAD 2
#define keyPerKeyboard 8
#endif

#ifdef TECLADO_14
#define pinB 14 // AD1
#define QTD_KEYPAD 1
#define keyPerKeyboard 14
#endif

#ifdef TECLADO_4
#define pinB 14 // AD1
#define QTD_KEYPAD 1
#define keyPerKeyboard 4
#endif

#ifdef TECLADO_20
keyExtern_t keys2[keyPerKeyboard] = {
    {4094, 'L'},
    {3888, '7'},
    {3545, '0'},
    {3206, '8'},
    {3015, '4'},
    {2654, '5'},
    {2491, '1'},
    {2229, '2'},
    {2011, 'A'},
    {1882, 'B'},
};

keyExtern_t keys1[keyPerKeyboard] = {
    {4094, 'R'},
    {3892, '9'},
    {3534, 'E'},
    {3182, 'S'},
    {2983, '6'},
    {2636, 'D'},
    {2458, '3'},
    {2206, 'U'},
    {1998, '#'},
    {1867, '*'},
};
#endif

#ifdef TECLADO_16
keyExtern_t keys2[keyPerKeyboard] = {
    {4094, '1'},
    {3887, '2'},
    {3544, '4'},
    {3207, '5'},
    {2991, '3'},
    {2675, '6'},
    {2450, 'A'},
    {2240, 'B'}
};

keyExtern_t keys1[keyPerKeyboard] = {
    {4094, '7'},
    {3883, '8'},
    {3529, '*'},
    {3198, '0'},
    {2960, '9'},
    {2660, '#'},
    {2440, 'C'},
    {2238, 'D'}
};
#endif

#ifdef TECLADO_14
keyExtern_t keys2[keyPerKeyboard] = {
    {4094, '1'},
    {4060, '2'},
    {3512, '3'},
    {3100, '4'},
    {2794, '5'},
    {2585, '6'},
    {2359, '7'},
    {2092, '8'},
    {1879, '9'},
    {1550, 'A'},
    {1318, 'B'},
    {1100, 'C'},
    {861, 'D'},
    {573, 'E'}
};
#endif

#ifdef TECLADO_4
keyExtern_t keys2[keyPerKeyboard] = {
    {4094, '2'},
    {3004, '1'},
    {2407, '4'},
    {2011, '3'}
};
#endif


/* ORIGNAL 8bits

#ifdef TECLADO_20
keyExtern_t keys2[keyPerKeyboard] = {
    {64516, 'L'},
    {51984, '7'},
    {41616, '0'},
    {33856, '8'},
    {30976, '4'},
    {23409, '5'},
    {20449, '1'},
    {16384, '2'},
    {13689, 'A'},
    {11449, 'B'},
};

keyExtern_t keys1[keyPerKeyboard] = {
    {64250, 'R'},
    {51076, '9'},
    {41209, 'E'},
    {33489, 'S'},
    {30625, '6'},
    {23104, 'D'},
    {20449, '3'},
    {16384, 'U'},
    {13689, '#'},
    {11236, '*'},
};
#endif

#ifdef TECLADO_16
keyExtern_t keys2[keyPerKeyboard] = {
    {64516, '1'},
    {51984, '2'},
    {41616, '4'},
    {33856, '5'},
    {30976, '3'},
    {23409, '6'},
    {20736, 'A'},
    {16384, 'B'}
};

keyExtern_t keys1[keyPerKeyboard] = {
    {64516, '7'},
    {51529, '8'},
    {40804, '*'},
    {33124, '0'},
    {29929, '9'},
    {22801, '#'},
    {19881, 'C'},
    {16129, 'D'}
};
#endif

#ifdef TECLADO_14
keyExtern_t keys2[keyPerKeyboard] = {
    {64516, '1'},
    {63001, '2'},
    {46656, '3'},
    {36100, '4'},
    {29584, '5'},
    {24964, '6'},
    {20736, '7'},
    {16384, '8'},
    {12996, '9'},
    {9216, 'A'},
    {6724, 'B'},
    {4489, 'C'},
    {2601, 'D'},
    {1225, 'E'}
};
#endif

#ifdef TECLADO_4
keyExtern_t keys2[keyPerKeyboard] = {
    {64516, '2'},
    {34225, '1'},
    {21904, '4'},
    {15129, '3'},
};
#endif

 */