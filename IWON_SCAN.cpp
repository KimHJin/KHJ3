///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP SCAN
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_SCAN.h"

// 센서 써미스터 특정 (소수점 연산을 위해서 1000 배수 한 것이다.)
// 범위 바뀌면 헤더 파일의 NTC_MIN, NTC_MAX 적용해야 함
const static INT32 NTC_TBL[] = {
    4251000, // -40
    3962000, // -39
    3695000, // -38
    3447000, // -37
    3218000, // -36
    3005000, // -35
    2807000, // -34
    2624000, // -33
    2454000, // -32
    2296000, // -31
    2149000, // -30
    2012000, // -29
    1885000, // -28
    1767000, // -27
    1656000, // -26
    1554000, // -25
    1458000, // -24
    1369000, // -23
    1286000, // -22
    1208000, // -21
    1135000, // -20
    1068000, // -19
    1004000, // -18
    945000,  // -17
    889600,  // -16
    837800,  // -15
    789300,  // -14
    743900,  // -13
    701300,  // -12
    661500,  // -11
    624100,  // -10
    589000,  // -9
    556200,  // -8
    525300,  // -7
    496300,  // -6
    469100,  // -5
    443500,  // -4
    419500,  // -3
    396900,  // -2
    375600,  // -1
    355600,  // 0
    336800,  // 1
    319100,  // 2
    302400,  // 3
    286700,  // 4
    271800,  // 5
    257800,  // 6
    244700,  // 7
    232200,  // 8
    220500,  // 9
    209400,  // 10
    198900,  // 11
    189000,  // 12
    179700,  // 13
    170900,  // 14
    162500,  // 15
    154600,  // 16
    147200,  // 17
    140100,  // 18
    133400,  // 19
    127000,  // 20
    121000,  // 21
    115400,  // 22
    110000,  // 23
    104800,  // 24
    100000,  // 25
    95400,   // 26
    91040,   // 27
    86900,   // 28
    82970,   // 29
    79230,   // 30
    75690,   // 31
    72320,   // 32
    69120,   // 33
    66070,   // 34
    63180,   // 35
    60420,   // 36
    57810,   // 37
    55310,   // 38
    52940,   // 39
    50680,   // 40
    48530,   // 41
    46490,   // 42
    44530,   // 43
    42670,   // 44
    40900,   // 45
    39210,   // 46
    37600,   // 47
    36060,   // 48
    34600,   // 49
    33190,   // 50
    31860,   // 51
    30580,   // 52
    29360,   // 53
    28200,   // 54
    27090,   // 55
    26030,   // 56
    25010,   // 57
    24040,   // 58
    23110,   // 59
    22220,   // 60
    21370,   // 61
    20560,   // 62
    19780,   // 63
    19040,   // 64
    18320,  // 65
    17640,  // 66
    16990,  // 67
    16360,  // 68
    15760,  // 69
    15180,  // 70
    14630,  // 71
    14100,  // 72
    13600,  // 73
    13110,  // 74
    12640,  // 75
    12190,  // 76
    11760,  // 77
    11350,  // 78
    10960,  // 79
    10580,  // 80
    10210,  // 81
    9859,   // 82
    9522,   // 83
    9198,   // 84
    8887,   // 85
    8587,   // 86
    8299,   // 87
    8022,   // 88
    7756,   // 89
    7500,   // 90
    7254,   // 91
    7016,   // 92
    6788,   // 93
    6568,   // 94
    6357,   // 95
    6153,   // 96
    5957,   // 97
    5768,   // 98
    5586,   // 99
    5410,   // 100
    5241,   // 101
    5078,   // 102
    4921,   // 103
    4769,   // 104
    4623,   // 105
    4482,   // 106
    4346,   // 107
    4215,   // 108
    4088,   // 109
    3966,   // 110
    3848,   // 111
    3734,   // 112
    3624,   // 113
    3518,   // 114
    3415,   // 115
    3316,   // 116
    3220,   // 117
    3128,   // 118
    3038,   // 119
    2952,   // 120
    2868,   // 121
    2787,   // 122
    2709,   // 123
    2634,   // 124
    2561    // 125
};

// 28.1 ~ 38.9
// 범위 바뀌면 헤더 파일의 TB_MIN, TB_MAX 적용해야 함
const static INT8 TB_TBL[] = {
    53,53,53,53,53,53,53,52,51,50,50,50,50,50,50,50,50,50,49,48,
    48,47,47,47,46,46,46,46,46,46,46,46,46,45,45,45,44,43,43,42,
    42,40,40,39,38,38,37,36,36,35,34,34,33,32,31,30,29,28,27,26,
    25,24,23,22,21,21,20,19,18,17,16,15,15,15,14,16,15,14,13,12,
    12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,11,10,
    10,10,10,10,10,10,10,10,10
};

  

// 생성자
IWON_TEMP_SCAN::IWON_TEMP_SCAN() {
	TADJ0 = DEFINED_TADJ0;
	VADJ1 = DEFINED_VADJ1;
	VADJ2 = DEFINED_VADJ2;

	TSUMN = DEFINED_TSUMN; // 온도 평균값을 만들기 위하여 몇개의 방을 써야할지 결정
	TSUMC = 0;
	TSUMB = NULL;

	InitTSUMB();
}
IWON_TEMP_SCAN::IWON_TEMP_SCAN(INT8 tsumn) {
	TADJ0 = DEFINED_TADJ0;
	VADJ1 = DEFINED_VADJ1;
	VADJ2 = DEFINED_VADJ2;

	TSUMN = tsumn;        // 온도 평균값을 만들기 위하여 몇개의 방을 써야할지 결정
	TSUMC = 0;
	TSUMB = NULL;

	InitTSUMB();
}



// 소멸자
IWON_TEMP_SCAN::~IWON_TEMP_SCAN() {
	if(TSUMB!=NULL) free(TSUMB);
}



// 온도 평균값 구하는 링 메모리를 생성한다.
VOID IWON_TEMP_SCAN::InitTSUMB(VOID) {
	if(TSUMB!=NULL) free(TSUMB);
	TSUMB = (INT16 *)malloc(sizeof(INT16) * TSUMN);  
}


// TOBJ 를 이 값만큼 빼준다.
VOID IWON_TEMP_SCAN::SetTADJ0(INT16 tadj0) {
	TADJ0 = tadj0;
}
INT16 IWON_TEMP_SCAN::GetTADJ0(VOID) {
	return TADJ0;
}
  
// delta 값 변경
VOID IWON_TEMP_SCAN::SetVADJ1(INT16 vadj1) {
	VADJ1 = vadj1;
}
INT16 IWON_TEMP_SCAN::GetVADJ1(VOID) {
	return VADJ1;
}
  
// shaft v 값
VOID IWON_TEMP_SCAN::SetVADJ2(INT16 vadj2) {
	VADJ2 = vadj2;
}
INT16 IWON_TEMP_SCAN::GetVADJ2(VOID) {
	return VADJ2;
}






//-----------------------------------------------------------------------------
// NTC 기준 테이블 인덱스 넘버를 찾는다.
// 입력 : MRES = 센서의 써미스터 저항 값
// 출력 : MRES 가 위치하는 NTC 테이블 인덱스 위치
//-----------------------------------------------------------------------------
INT16 IWON_TEMP_SCAN::GetNTCIndex(INT32 MRES) {
    INT16 index = -1;
    INT16 tbl_count = (sizeof(NTC_TBL) / sizeof(INT32)) - 1;
    for(;;) {
        index++;
        if(index==tbl_count) break;
        if(MRES>NTC_TBL[index]) return index;
    }
    return -1;
}

// NTC 기준 인덱스 번호화 실측된 값을 이용해서 해당 구간내의 보정 위치의 퍼센트를 계산한다.
// 그 이유는 실제 측정된 온도값의 저항값의 인덱스는 해당 NTC 구간의 1도 분해능이기 때문에
// 그 사이의 정확한 온도를 계산하기 위해서 측정된 값이 해당 구간의 어느 위치에 있는지 알아야 하기 때문이다.
// 입력 : MRES 센서의 써미스터 저항 값
//       MRES 의 index 해당 NTC 인덱스 위치 값
// 출력 : 해당 NTC 구간의 입력된 MRES 의 위치 퍼센트 값
INT16 IWON_TEMP_SCAN::GetNTCValueRatio(INT32 MRES, INT16 index) {
    INT32 NTCRL = NTC_TBL[index];
    INT32 NTCRH = NTC_TBL[index-1];

    INT32 RM = MRES - NTCRL;
    INT32 RD = NTCRH - NTCRL;

    INT16 PR = (INT16)(RM * 100 / RD);
    return PR;
}


// 사물 온도를 인체 온도로 변환
// 입력 : T_OBJ 는 측정된 사물 온도를 10 배수한 정수이다.
// 출력 : -1 = LO
//       -2 = HI
INT8 IWON_TEMP_SCAN::GetTBLValue(INT16 T_OBJ) {
    if(T_OBJ < TB_MIN) return -1;
    if(T_OBJ > TB_MAX) return -2;
    return TB_TBL[T_OBJ-TB_MIN];
} 

VOID IWON_TEMP_SCAN::ClearTSUMB() {
    TSUMC = 0;
    for(INT16 i=0;i<TSUMN;i++) {
        TSUMB[i] = -999;
    }
}
INT16 IWON_TEMP_SCAN::GetTSUMB() {
    INT32 B = 0;
    INT16 C = 0;
    for(INT8 i=0;i<TSUMN;i++) {
        if(TSUMB[i]==-999) continue;
        B += TSUMB[i];
        C++;
    }
    if(C==0) return -999;
    return (INT16)(B / C);
}
INT16 IWON_TEMP_SCAN::AddTSUMB(INT16 B) {
    TSUMB[TSUMC] = B;
    TSUMC++;
    if(TSUMC==TSUMN) TSUMC = 0;
    return GetTSUMB();
}



