/////////////////////////////////////////////////
//LOL_Users_League 게임
//작성자 : 미디어기술콘텐츠학과 201321705 이기인
//작성일 : 2013.12.05
//Verson 2.0
/////////////////////////////////////////////////


//x 총 54 >> 마지막 53
//y 총 39 >> 마지막 38
//map[39][54]


//+ 색깔 넣기.
//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),색깔수);
/*
검정 0		어두운파랑 1		어두운 초록 2	어두운 하늘 3	어두운 빨강 4
어두운 보라 5	어두운 노랑 6	회색 7		어두운 회색 8	파랑 9
초록 10	하늘 11	빨강 12	보라 13	노랑 14	하양 15
*/
//http://blog.naver.com/fflash4?Redirect=Log&logNo=30148370314

//세이브기능 추가

#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>



//함수 나열

/////////////////////////////////////////////////////////////////////////////////
void setCursor ( int x, int y );		//커서 설정 함수

void cmdExpand ( );					//cmd창 확대 함수
void Title ( );						//제목 함수

void charInit ( );					//영웅 설정 함수
int selectChar ( );					//영웅 선택 함수
void current_State ( int charNum );

void mainGame ( int charNum );						//메인 게임 작동 관련 함수
void main_MapViewer ( );							//메인 게임 맵 그리기 함수
void main_movingChar ( char ( *map )[54] );			//캐릭터 움직이게 하는 함수.
													//void main_clearBoss(char pointX, int pointY);	//보스 클리어할 경우
void main_Teleport ( int num, char ( *map )[54] );	//텔레포트 함수
void main_clearWall ( char ( *map )[54] );			//보스 클리어할 경우 벽 무력화.

void monsterInit ( int monsterNum );					//몬스터 정보 초기화 함수
void BossInit ( );						//보스 정보 초기화 함수

void miniGame ( int charNum );						//미니 게임 함수
void mini_Map_Viewer ( int charNum );				//미니 게임 맵 그리기(싸움)
void mini_MapWhite ( );							//창 비우기	(몬스터 출현!)
void mini_MapBlack ( );							//창 비우기	(몬스터 출현!)
int mini_Control ( int charNum );					//미니 게임 컨트롤 함수
void mini_monsterControl ( int charNum, int menuSelect, int monsterNum );		//몬스터->유저 공격 함수
void showCharState ( int charNum, int BossNum );				//캐릭터, 보스 상태창 출력
void mini_stateTableClear ( );					//공격 한 번 끝날 경우 미니게임 상태창 비우는 함수
void charLevelUp ( int charNum, int surplusEX );					//캐릭터 레벨 업!
int mini_Fight ( int charNum, int menuSelect, int monsterNum );		//공격 전개 함수.
void mini_BossMapViewer ( int charNum, int BossNum, char ( *map )[54] );		//보스 출현 맵
int mini_Fight_Boss ( int charNum, int menuSelect, int BossNum );
void mini_BossControl ( int charNum, int menuSelect, int BossNum );

void charSkill_ForBoss ( int charNum, int BossNum );
void charSkill ( int charNum, int monsterNum );			//영웅 스킬 함수

void gameClear ( );
////////////////////////////////////////////////////////////////////////////////////////////////




//초기화
int Legends_charNum = 0;		//영웅 번호
int charLevel = 1;
int count = 0;		//이긴 보스의 수
int current_point = 0;
int monsterNum = 0;




//커서 설정 구조체
///////////////////////////////////////////////////////////////////////////////////////////////
COORD getCursor ( void )		//커서 설정.
{
	COORD cur;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;
	GetConsoleScreenBufferInfo (
		GetStdHandle ( STD_OUTPUT_HANDLE ), &curInfo );
	cur.X = curInfo.dwCursorPosition.X;
	cur.Y = curInfo.dwCursorPosition.Y;
	return cur;
}

//커서 초기화
struct CHARPOS		//커서 위치
{
	int x;
	int y;
};
CHARPOS char_POS;		//구조체 변수 하나 생성
CHARPOS char_POS_mini;		//구조체 변수_미니게임용 하나생성

							//커서 위치 설정
void setCursor ( int x, int y )
{
	COORD pos = { x,y };
	SetConsoleCursorPosition (
		GetStdHandle ( STD_OUTPUT_HANDLE ), pos );
}
///////////////////////////////////////////////////////////////////////////////////////




//메인 함수
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void main ( void )
{
	cmdExpand ( );	//콘솔창을 확대합니다.
	Title ( );				//제목 타이틀 화면을 출력합니다.
	mainGame ( Legends_charNum );		//메인 게임 호출합니다.
	gameClear ( );
	getch ( );
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////




//캐릭터 정보 구조체.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//User char 구조체
struct Legends_char
{
	int CharNum;					//캐릭터 번호			1.King		2.Knight		3.Magician		4.Rogue
	int level;
	//	int skill;						//영웅 스킬	
	double damageMin;				//영웅 공격력			=2			=3				=1				=2
	double damageMax;				//						=4			=5				=3				=4
	int life;						//영웅 현재 체력
	int MaxLife;					//영웅 체력				=25			=30				=15				=20
	double levelUpBonus;			//가중치				=1.4		=1.3			=1.6			=1.5
	int experiencePoint;			//경험치				레벨*10+(레벨-1)*5
	int currentEX;					//영웅 현재 경험치 량
	int hasItem;					//아이템 가지고 있는 개수
};
Legends_char King;
Legends_char Knight;
Legends_char Magician;
Legends_char Rogue;

//charInit() 함수가 캐릭터 정보 초기화 시킴.


//몬스터 정보 구조체
struct Monster
{
	int level;		//몬스터 레벨
	double damageMin;		//몬스터 공격력
	double damageMax;		//
	int life;		//몬스터 체력
	int giveEX;		//몬스터 잡을 경우 주는 경험치량
	char* name;
}; Monster MonsterNum[3];

Monster BossNum1;
Monster BossNum2;
Monster BossNum3;

//monsterInit()함수에서 몬스터 정보 초기화
//bossInit()함수에서 보스 정보 초기화

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//1단계 main_Map
char main_Map[39][54] = {
	//0:  , 1:■ (벽), 2:♣ (나무), 3:▨ (위에서 아래로 뛰어넘는 벽), 4:⌄ (잔디), 5(1):░ (단계벽), 
	//6(2):░ (단계벽), 7(3):░ (단계벽), 8(1):★ (보스), 9(2):★ (보스), 10(3):★ (보스),
	//11:●, 12:  (출발지점)&(세이브), 13:  (미끌어지는 곳), 14: ◎(End)
	//21:○(순간이동 - 1), 22:○(순간이동 - 2), 23:○(순간이동 - 3)
	//▲ char
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,1,2,0,0,0,0,2,0,4,4,2,0,0,4,2,2,2,2,2,2,2,4,4,4,2,0,0,0,2,4,4,4,2,0,0,0,2,2,0,6,4,4,4,2,4,4,4,2,4,4,4,0,1,
	0,1,2,21,2,2,0,2,0,2,4,2,0,2,4,4,4,4,4,0,2,2,4,2,4,2,0,2,4,2,4,2,4,2,0,2,0,2,2,4,2,4,2,4,4,4,2,4,2,4,2,2,0,1,
	0,1,2,2,2,2,0,2,0,2,4,2,0,4,2,2,2,2,2,0,2,2,4,2,4,2,0,2,4,2,4,2,4,2,0,2,0,2,2,4,2,4,2,2,2,2,2,4,4,4,4,2,9,1,
	0,1,2,0,0,0,0,2,0,2,4,4,2,4,2,2,4,4,4,0,4,4,4,2,0,0,0,2,4,4,4,2,4,2,4,2,4,4,4,4,2,4,4,4,4,4,2,2,2,2,4,2,2,1,
	0,1,2,0,2,2,0,2,0,2,2,4,2,4,2,2,4,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,4,2,4,2,2,2,2,2,2,4,2,2,2,4,2,0,0,2,4,4,4,1,
	0,1,0,0,2,0,0,0,0,0,2,4,2,4,2,4,4,2,0,0,2,2,2,2,2,2,4,4,4,4,4,4,4,2,4,4,4,4,4,0,2,4,4,4,4,4,2,4,4,4,2,2,4,1,
	0,1,0,2,0,0,2,2,2,0,2,0,2,0,2,4,2,2,3,3,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,0,2,4,2,2,2,4,4,4,2,4,4,2,2,1,
	0,1,0,0,0,0,0,0,0,0,2,0,2,0,2,4,4,2,0,0,2,2,2,2,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,2,4,4,4,2,4,2,4,2,2,4,4,4,1,
	0,1,2,2,2,2,0,2,2,2,2,0,2,0,2,2,4,2,0,0,0,0,2,2,13,13,13,13,13,13,2,13,13,13,13,13,13,13,13,13,2,2,2,4,2,4,2,4,4,2,4,2,2,1,
	0,1,0,0,0,0,0,2,22,0,2,0,0,0,2,2,4,4,4,0,0,0,2,2,13,13,13,13,13,13,13,13,13,13,13,13,13,2,13,13,2,4,4,4,4,4,4,4,4,2,4,2,4,1,
	0,1,0,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,4,4,0,0,2,2,13,2,13,13,13,13,13,13,13,13,13,13,13,13,13,13,2,4,2,2,2,4,2,2,2,2,4,2,4,1,
	0,1,0,2,23,2,0,0,0,0,2,0,10,2,0,0,0,2,4,4,0,0,2,2,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,2,4,2,4,4,4,4,4,4,2,4,4,4,1,
	0,1,0,2,0,2,0,2,2,2,2,0,2,2,0,2,0,2,4,4,0,0,2,2,13,13,13,13,13,2,13,13,13,13,13,13,13,13,13,13,2,4,2,4,2,2,2,2,4,2,2,4,2,1,
	0,1,0,2,0,2,0,0,0,0,2,0,0,0,7,2,0,2,4,4,3,3,2,2,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,2,2,4,4,4,2,2,4,4,4,4,4,4,4,1,
	0,1,2,0,0,2,0,2,0,2,2,2,4,2,2,2,0,2,4,4,0,0,2,2,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,2,2,2,2,2,2,5,5,2,2,2,2,2,1,
	0,1,0,0,2,2,2,0,0,2,2,4,4,4,2,2,0,2,2,4,0,0,2,2,13,13,13,13,13,13,13,13,13,13,13,13,13,13,2,13,2,2,2,2,2,2,5,5,2,2,0,0,0,1,
	0,1,0,2,0,0,2,0,2,0,2,4,2,4,2,2,0,2,2,4,0,0,2,2,2,2,2,2,2,2,7,7,2,2,2,2,2,2,2,2,0,0,0,0,0,0,5,5,0,2,0,2,0,1,
	0,1,0,0,2,0,2,0,2,0,2,4,4,4,2,2,0,2,2,4,0,0,2,2,2,2,2,2,2,2,7,7,2,2,2,2,2,2,2,2,0,0,0,0,0,0,5,5,0,2,11,2,0,1,
	0,1,2,0,0,0,2,0,0,0,2,2,4,2,2,2,0,7,7,7,6,6,0,0,0,0,0,0,2,2,4,4,4,4,4,4,4,4,4,4,4,4,2,2,0,0,2,2,0,0,2,2,0,1,
	0,1,0,0,2,0,0,0,2,2,2,4,4,4,2,2,0,7,7,7,6,6,0,0,0,0,0,0,2,2,4,4,4,4,4,4,4,4,4,4,2,2,2,2,0,0,2,2,0,0,2,2,0,1,
	0,1,0,2,2,2,2,2,0,0,2,4,2,4,2,2,0,2,2,2,0,0,2,0,0,0,0,0,0,0,4,4,4,4,2,2,4,4,0,0,0,0,2,2,0,0,2,2,2,0,0,0,0,1,
	0,1,0,0,2,0,0,0,0,0,2,4,4,4,2,2,0,2,2,2,0,0,2,0,0,0,0,0,0,0,4,4,4,4,2,2,4,4,0,0,0,0,2,2,0,0,0,0,2,2,2,2,2,1,
	0,1,2,2,2,0,2,2,2,0,2,2,4,2,2,2,0,2,2,2,0,0,2,4,4,2,2,2,2,2,2,2,4,4,2,2,4,4,2,2,0,0,2,2,0,0,0,0,2,2,2,2,2,1,
	0,1,0,0,2,0,0,0,2,0,2,4,4,4,2,2,0,2,2,2,0,0,2,4,4,2,2,2,2,2,2,2,4,4,2,2,4,4,2,2,0,0,2,2,2,2,4,4,4,4,4,4,4,1,
	0,1,0,2,0,2,0,2,2,0,2,4,2,4,2,2,0,2,2,2,0,0,2,4,4,4,4,4,4,4,2,2,4,4,4,4,4,4,2,2,0,0,2,2,2,2,4,4,4,4,4,4,4,1,
	0,1,0,2,0,2,0,2,2,0,4,4,4,4,2,2,0,2,2,2,0,0,2,4,4,4,4,4,4,4,2,2,4,4,4,4,4,4,2,2,0,0,4,4,2,2,2,2,0,2,2,4,4,1,
	0,1,0,0,0,2,0,0,0,2,2,2,2,2,2,2,0,2,2,2,3,3,2,2,2,2,2,2,4,4,2,2,4,4,4,5,5,5,5,5,0,0,4,4,2,2,2,2,0,2,2,4,4,1,
	0,1,0,2,2,2,2,2,0,2,2,2,2,2,2,2,0,2,2,2,0,0,2,2,2,2,2,2,4,4,2,2,4,4,4,5,2,2,2,2,4,4,4,4,0,0,2,2,0,2,2,4,4,1,
	0,1,0,0,0,0,0,2,0,0,2,2,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,5,2,2,2,2,4,4,4,4,0,0,2,2,0,2,2,4,4,1,
	0,1,2,2,0,2,0,2,2,0,2,2,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,5,2,2,0,0,0,0,2,2,5,5,2,4,4,5,5,0,0,1,
	0,1,2,2,0,0,0,2,0,0,2,2,0,0,2,2,2,2,2,2,6,2,2,2,2,2,2,2,2,2,0,0,5,5,5,5,2,2,0,0,0,0,2,2,5,5,2,4,4,5,5,0,0,1,
	0,1,0,0,0,2,0,2,0,2,2,2,0,0,2,6,6,6,6,6,6,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,4,4,2,2,2,2,5,5,2,4,4,2,2,0,0,1,
	0,1,0,0,0,2,0,2,0,0,0,2,0,0,2,6,2,2,2,2,6,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,4,4,2,2,5,5,5,5,2,4,4,2,2,0,0,1,
	0,1,0,2,2,0,0,2,2,2,0,2,0,0,2,6,6,6,11,2,6,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,4,4,4,4,2,2,5,5,5,5,2,4,4,2,2,0,0,1,
	0,1,0,2,2,0,2,2,0,0,0,2,0,0,2,2,2,2,2,2,6,2,0,0,0,0,0,2,2,2,2,2,2,4,4,4,4,4,4,4,2,2,0,0,2,2,2,4,4,2,2,0,0,1,
	0,1,24,2,2,0,0,0,0,2,2,2,0,0,6,6,6,6,6,6,6,2,0,0,0,0,0,2,11,4,4,4,4,4,4,4,4,4,2,2,2,2,0,0,4,4,4,4,4,2,2,0,8,1,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,12,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};



/////////////////////////////////////////
//
//	함수 시작
//
/////////////////////////////////////////


void cmdExpand ( )				//cmd창 확대 함수
{
	system ( "mode con: cols=110 lines=46" );
}

void Title ( )					//제목 함수
{
	//0:  , 1:■
	char title_Drawing[26][52] = {		//제목 그리기
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,1,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,1,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	for ( int i = 0; i<26; i++ )
	{
		for ( int j = 0; j<52; j++ )
		{
			if ( title_Drawing[i][j] == 0 )
				printf ( "  " );
			else if ( title_Drawing[i][j] == 1 ) {
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
				printf ( "■" );
			}
		}
		printf ( "\n" );
	}

	int menuSelect;			//메뉴 선택 입력 받을 변수
	setCursor ( 50, 28 );
	printf ( "1. 시작" );
	setCursor ( 50, 29 );
	printf ( "2. 종료" );
	setCursor ( 49, 31 );
	printf ( "선택 : " );
	scanf ( "%d", &menuSelect );
	while ( 1 )
	{
		if ( menuSelect == 1 )		//게임 시작 버튼 누르면
		{
			//Legends_charNum은 캐릭터 번호 들어갈 전역 변수
			Legends_charNum = selectChar ( );			//영웅 설정 함수를 불러옵니다.

			if ( Legends_charNum == 1 )
				charLevel = King.level;
			else if ( Legends_charNum == 2 )
				charLevel = Knight.level;
			else if ( Legends_charNum == 3 )
				charLevel = Magician.level;
			else if ( Legends_charNum == 4 )
				charLevel = Rogue.level;

			break;
		}
		else if ( menuSelect == 2 )	//종료 버튼 누르면
		{
			setCursor ( 38, 32 );
			exit ( 0 );		//프로그램이 종료됩니다.
		}
		//이상한 값 입력했을 경우
		else
		{
			setCursor ( 56, 31 );
			printf ( " " );
			setCursor ( 56, 31 );
			scanf ( "%d", &menuSelect );
		}
	}
}



void charInit ( )					//영웅 설정 함수
{
	//영웅 선택 받을 변수를 일단 전역 변수로 선언하고,
	//Legends_charNum (get_Cursor 함수 위에)

	//영웅정보를 입력합니다.
	//King
	King.CharNum = 1;
	King.level = 1;
	//	King.skill = 1;
	King.levelUpBonus = 1.4;
	King.damageMin = 3 + (King.level - 1) * King.levelUpBonus;
	King.damageMax = 5 + (King.level - 1) * King.levelUpBonus;
	King.life = 30 + King.level * 7 * King.levelUpBonus;
	King.MaxLife = King.life;
	if ( King.life < 0 )
		King.life = 0;
	King.experiencePoint = King.level * 10 + (King.level - 1) * 5;
	King.hasItem = 0;
	King.currentEX = 0;

	//Knight
	Knight.CharNum = 2;
	Knight.level = 1;
	//	Knight.skill = 2;
	Knight.levelUpBonus = 1.3;
	Knight.damageMin = 4 + (Knight.level - 1) * Knight.levelUpBonus;
	Knight.damageMax = 6 + (Knight.level - 1) * Knight.levelUpBonus;
	Knight.life = 35 + Knight.level * 7 * Knight.levelUpBonus;
	Knight.MaxLife = Knight.life;
	if ( Knight.life < 0 )
		Knight.life = 0;
	Knight.experiencePoint = Knight.level * 10 + (Knight.level - 1) * 5;
	Knight.hasItem = 0;
	Knight.currentEX = 0;

	//Magician
	Magician.CharNum = 3;
	Magician.level = 1;
	//	Magician.skill = 3;
	Magician.levelUpBonus = 1.6;
	Magician.damageMin = 2 + (Magician.level - 1) * Magician.levelUpBonus;
	Magician.damageMax = 4 + (Magician.level - 1) * Magician.levelUpBonus;
	Magician.life = 22 + Magician.level * 7 * Magician.levelUpBonus;
	Magician.MaxLife = Magician.life;
	if ( Magician.life < 0 )
		Magician.life = 0;
	Magician.experiencePoint = Magician.level * 10 + (Magician.level - 1) * 5;
	Magician.hasItem = 0;
	Magician.currentEX = 0;

	//Rogue
	Rogue.CharNum = 4;
	Rogue.level = 1;
	//	Rogue.skill = 4;
	Rogue.levelUpBonus = 1.5;
	Rogue.damageMin = 3 + (Rogue.level - 1) * Rogue.levelUpBonus;
	Rogue.damageMax = 5 + (Rogue.level - 1) * Rogue.levelUpBonus;
	Rogue.life = 26 + Rogue.level * 7 * Rogue.levelUpBonus;
	Rogue.MaxLife = Rogue.life;
	if ( Rogue.life < 0 )
		Rogue.life = 0;
	Rogue.experiencePoint = Rogue.level * 10 + (Rogue.level - 1) * 5;
	Rogue.hasItem = 0;
	Rogue.currentEX = 0;
}

int selectChar ( )	//캐릭터 선택창 출력 및 선택 함수
{
	//일단 cmd창을 비웁니다.
	system ( "cls" );

	charInit ( );
	int select;		//캐릭터 선택 변수

					//0:  , 1:■,
	char CharViewer[26][54] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
		0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,
		0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,1,1,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
		0,0,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,
		0,1,0,0,1,1,0,1,1,0,0,1,0,1,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,1,0,1,1,0,0,1,0,0,0,1,0,0,1,1,0,1,1,0,0,1,0,0,
		0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,
		0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,
		0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,
		0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};
	for ( int i = 0; i<26; i++ )
	{
		for ( int j = 0; j<54; j++ )
		{
			if ( CharViewer[i][j] == 0 )
				printf ( "  " );
			else if ( CharViewer[i][j] == 1 ) {
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
				printf ( "■" );
			}
		}
		printf ( "\n" );
	}

	//King
	setCursor ( 10, 20 );
	printf ( "King" );
	setCursor ( 9, 21 );
	printf ( "공격력:2*" );
	setCursor ( 7, 22 );
	printf ( "스킬:Taking" );

	//Knight
	setCursor ( 34, 20 );
	printf ( "Knight" );
	setCursor ( 33, 21 );
	printf ( "공격력:3*" );
	setCursor ( 31, 22 );
	printf ( "스킬:Demasia" );

	//Magician
	setCursor ( 61, 20 );
	printf ( "Magician" );
	setCursor ( 61, 21 );
	printf ( "공격력:1*" );
	setCursor ( 60, 22 );
	printf ( "스킬:Piber!" );

	//Rogue
	setCursor ( 90, 20 );
	printf ( "Rogue" );
	setCursor ( 89, 21 );
	printf ( "공격력:2*" );
	setCursor ( 86, 22 );
	printf ( "스킬:Death Lotus" );

	setCursor ( 45, 27 );
	printf ( "선택 : " );
	scanf ( "%d", &select );
	while ( 1 )
	{
		if ( select>4 || select <= 0 )
		{
			setCursor ( 52, 27 );
			printf ( " " );
			setCursor ( 52, 27 );
			scanf ( "%d", &select );
		}
		else
			break;
	}
	return select;
}

void current_State ( int charNum )		//현재 상태 출력(맵 아래에)
{
	SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
	setCursor ( 2, 40 );
	printf ( "현재 상태 창" );
	setCursor ( 2, 41 );
	if ( charNum == 1 ) {
		printf ( "King		Lv : %d  ", King.level );
		setCursor ( 2, 42 );
		printf ( "HP : %d  /  %d  ,	EX : %d  /  %d   ", King.life, King.MaxLife, King.currentEX, King.experiencePoint );
		setCursor ( 2, 43 );
		printf ( "Item : %d", King.hasItem );
	}
	else if ( charNum == 2 ) {
		printf ( "Knight		Lv : %d  ", Knight.level );
		setCursor ( 2, 42 );
		printf ( "HP : %d  /  %d  ,	EX : %d  /  %d   ", Knight.life, Knight.MaxLife, Knight.currentEX, Knight.experiencePoint );
		setCursor ( 2, 43 );
		printf ( "Item : %d", Knight.hasItem );
	}
	else if ( charNum == 3 ) {
		printf ( "Magician	Lv : %d  ", Magician.level );
		setCursor ( 2, 42 );
		printf ( "HP : %d  /  %d  ,	EX : %d  /  %d   ", Magician.life, Magician.MaxLife, Magician.currentEX, Magician.experiencePoint );
		setCursor ( 2, 43 );
		printf ( "Item : %d", Magician.hasItem );
	}
	else if ( charNum == 4 ) {
		printf ( "Rogue		Lv : %d  ", Rogue.level );
		setCursor ( 2, 42 );
		printf ( "HP : %d  /  %d  .	EX : %d  /  %d   ", Rogue.life, Rogue.MaxLife, Rogue.currentEX, Rogue.experiencePoint );
		setCursor ( 2, 43 );
		printf ( "Item : %d", Rogue.hasItem );
	}
	setCursor ( 2, 44 );
	printf ( "x: %d  , y: %d  ", char_POS.x, char_POS.y );

}



void mainGame ( int charNum )	//메인 게임 작동 관련 함수
{
	system ( "cls" );	//일단 창을 비우고,
	srand ( (unsigned int)time ( NULL ) );	//랜덤 함수 사용하기 위해.

	char_POS.x = 48;		//x좌표 설정
	char_POS.y = 37;		//y좌표 설정

	BossInit ( );

	main_MapViewer ( );	//메인 맵을 불러옵니다.
}

void main_MapViewer ( )		//메인 게임 맵 그리기 함수
{
	if ( count>0 )
		main_clearWall ( main_Map );

	for ( int i = 0; i<39; i++ )
	{
		for ( int j = 0; j<54; j++ )
		{
			if ( main_Map[i][j] == 0 )
			{
				printf ( "  " );
			}
			else if ( main_Map[i][j] == 1 )
			{
				printf ( "■" );
			}
			else if ( main_Map[i][j] == 2 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0a );
				printf ( "♣" );
			}
			else if ( main_Map[i][j] == 3 )
				printf ( "▨" );
			else if ( main_Map[i][j] == 4 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
			}
			else if ( main_Map[i][j] == 5 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x9f );
				printf ( "▒" );
			}
			else if ( main_Map[i][j] == 6 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0xdf );
				printf ( "▒" );
			}
			else if ( main_Map[i][j] == 7 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0xcf );
				printf ( "▒" );
			}
			else if ( main_Map[i][j] == 8 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x09 );
				printf ( "★" );
			}
			else if ( main_Map[i][j] == 9 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0d );
				printf ( "★" );
			}
			else if ( main_Map[i][j] == 10 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0c );
				printf ( "★" );
			}
			else if ( main_Map[i][j] == 11 )
			{
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0b );
				printf ( "●" );
			}
			else if ( main_Map[i][j] == 12 )	//출발 지점
			{
				printf ( "  " );
			}
			else if ( main_Map[i][j] == 13 )	//미끌어지는 곳
			{
				printf ( "  " );
			}
			else if ( main_Map[i][j] == 14 )	//End
			{
				printf ( "◎" );
			}

			else if ( main_Map[i][j] == 21 )
			{
				printf ( "○" );
			}
			else if ( main_Map[i][j] == 22 )
			{
				printf ( "○" );
			}
			else if ( main_Map[i][j] == 23 )
			{
				printf ( "○" );
			}
			else if ( main_Map[i][j] == 24 )
			{
				printf ( "○" );
			}
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
		}
		printf ( "\n" );
	}

	current_State ( Legends_charNum );	//현재 상태 출력 함수 호출

	setCursor ( char_POS.x, char_POS.y );

	if ( count == 3 )
		goto end;

	while ( 1 )
	{
		main_movingChar ( main_Map );
	}
end:
	gameClear ( );
}

void main_movingChar ( char ( *map )[54] )		//캐릭터 움직이게 하는 함수
{
	setCursor ( char_POS.x, char_POS.y );

	int keyNum;
	int monsterAppear_probability[10] = { 1,0,0,0,0,0,0,0,0,0 };		//몬스터를 만날 확률 '10%'

	SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
	printf ( "▲" );

	keyNum = getch ( );		//방향키 입력받은 정보 저장.

							//우측으로 이동
	if ( keyNum == 77 ) {
		if ( map[char_POS.y][char_POS.x / 2 + 1] == 0 ) {
			if ( map[char_POS.y][char_POS.x / 2] == 4 )
			{
				char_POS.x += 2;
				setCursor ( char_POS.x - 2, char_POS.y );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( monsterAppear_probability[rand ( ) % 10] == 1 )
				{
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					miniGame ( Legends_charNum );
				}
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 0 )
			{
				char_POS.x += 2;
				setCursor ( char_POS.x - 2, char_POS.y );
				printf ( "  " );		//잔상 제거.
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 13 )
			{
				char_POS.x += 2;
				setCursor ( char_POS.x - 2, char_POS.y );
				printf ( "  " );
			}
		}
		else if ( map[char_POS.y][char_POS.x / 2 + 1] == 4 ) {
			if ( map[char_POS.y][char_POS.x / 2] == 4 )
			{
				char_POS.x += 2;
				setCursor ( char_POS.x - 2, char_POS.y );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( monsterAppear_probability[rand ( ) % 10] == 1 )
				{
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					miniGame ( Legends_charNum );
				}
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 0 )
			{
				char_POS.x += 2;
				setCursor ( char_POS.x - 2, char_POS.y );
				printf ( "  " );
			}
		}
		else if ( map[char_POS.y][char_POS.x / 2 + 1] == 13 ) {
			while ( map[char_POS.y][char_POS.x / 2 + 1] == 13 ) {
				char_POS.x += 2;
				setCursor ( char_POS.x - 2, char_POS.y );
				printf ( "  " );
				Sleep ( 20 );
			}
		}
		else if ( map[char_POS.y][char_POS.x / 2 + 1] == 8 )
		{
			char_POS.x += 2;
			setCursor ( char_POS.x - 2, char_POS.y );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 1, map );
		}
		else if ( map[char_POS.y][char_POS.x / 2 + 1] == 9 )
		{
			char_POS.x += 2;
			setCursor ( char_POS.x - 2, char_POS.y );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 2, map );
		}
		else if ( map[char_POS.y][char_POS.x / 2 + 1] == 10 )
		{
			char_POS.x += 2;
			setCursor ( char_POS.x - 2, char_POS.y );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 3, map );
		}
		else if ( map[char_POS.y][char_POS.x / 2 + 1] == 11 )
		{
			if ( map[char_POS.y][char_POS.x / 2] == 0 ) {
				char_POS.x += 2;
				setCursor ( char_POS.x - 2, char_POS.y );
				printf ( " " );
				if ( Legends_charNum == 1 ) {
					King.hasItem++;
				}
				else if ( Legends_charNum == 2 ) {
					Knight.hasItem++;
				}
				else if ( Legends_charNum == 3 ) {
					Magician.hasItem++;
				}
				else if ( Legends_charNum == 4 ) {
					Rogue.hasItem++;
				}
				map[char_POS.y][char_POS.x / 2] = 0;
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 4 ) {
				char_POS.x += 2;
				setCursor ( char_POS.x - 2, char_POS.y );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( Legends_charNum == 1 ) {
					King.hasItem++;
				}
				else if ( Legends_charNum == 2 ) {
					Knight.hasItem++;
				}
				else if ( Legends_charNum == 3 ) {
					Magician.hasItem++;
				}
				else if ( Legends_charNum == 4 ) {
					Rogue.hasItem++;
				}
				map[char_POS.y][char_POS.x / 2] = 0;
			}
		}
	}

	//좌측으로 이동
	else if ( keyNum == 75 ) {
		if ( map[char_POS.y][char_POS.x / 2 - 1] == 0 ) {
			if ( map[char_POS.y][char_POS.x / 2] == 4 )
			{
				char_POS.x -= 2;
				setCursor ( char_POS.x + 2, char_POS.y );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( monsterAppear_probability[rand ( ) % 10] == 1 )
				{
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					miniGame ( Legends_charNum );
				}
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 0 )
			{
				char_POS.x -= 2;
				setCursor ( char_POS.x + 2, char_POS.y );
				printf ( "  " );		//잔상 제거.
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 13 )
			{
				char_POS.x -= 2;
				setCursor ( char_POS.x + 2, char_POS.y );
				printf ( "  " );
			}
		}
		else if ( map[char_POS.y][char_POS.x / 2 - 1] == 22 )
		{
			setCursor ( char_POS.x, char_POS.y );
			printf ( "  " );
			main_Teleport ( 22, map );
		}
		else if ( map[char_POS.y][char_POS.x / 2 - 1] == 4 ) {
			if ( map[char_POS.y][char_POS.x / 2] == 4 )
			{
				char_POS.x -= 2;
				setCursor ( char_POS.x + 2, char_POS.y );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( monsterAppear_probability[rand ( ) % 10] == 1 )
				{
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					miniGame ( Legends_charNum );
				}
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 0 )
			{
				char_POS.x -= 2;
				setCursor ( char_POS.x + 2, char_POS.y );
				printf ( "  " );
			}
		}
		else if ( map[char_POS.y][char_POS.x / 2 - 1] == 13 ) {
			while ( map[char_POS.y][char_POS.x / 2 - 1] == 13 ) {
				char_POS.x -= 2;
				setCursor ( char_POS.x + 2, char_POS.y );
				printf ( "  " );
				Sleep ( 20 );
			}
		}
		else if ( map[char_POS.y][char_POS.x / 2 - 1] == 8 )
		{
			char_POS.x -= 2;
			setCursor ( char_POS.x + 2, char_POS.y );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 1, map );
		}
		else if ( map[char_POS.y][char_POS.x / 2 - 1] == 9 )
		{
			char_POS.x -= 2;
			setCursor ( char_POS.x + 2, char_POS.y );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 2, map );
		}
		else if ( map[char_POS.y][char_POS.x / 2 - 1] == 10 )
		{
			char_POS.x -= 2;
			setCursor ( char_POS.x + 2, char_POS.y );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 3, map );
		}
		else if ( map[char_POS.y][char_POS.x / 2 - 1] == 11 )
		{
			if ( map[char_POS.y][char_POS.x / 2] == 0 ) {
				char_POS.x -= 2;
				setCursor ( char_POS.x + 2, char_POS.y );
				printf ( " " );
				if ( Legends_charNum == 1 ) {
					King.hasItem++;
				}
				else if ( Legends_charNum == 2 ) {
					Knight.hasItem++;
				}
				else if ( Legends_charNum == 3 ) {
					Magician.hasItem++;
				}
				else if ( Legends_charNum == 4 ) {
					Rogue.hasItem++;
				}
				map[char_POS.y][char_POS.x / 2] = 0;
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 4 ) {
				char_POS.x -= 2;
				setCursor ( char_POS.x + 2, char_POS.y );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( Legends_charNum == 1 ) {
					King.hasItem++;
				}
				else if ( Legends_charNum == 2 ) {
					Knight.hasItem++;
				}
				else if ( Legends_charNum == 3 ) {
					Magician.hasItem++;
				}
				else if ( Legends_charNum == 4 ) {
					Rogue.hasItem++;
				}
				map[char_POS.y][char_POS.x / 2] = 0;
			}
		}
	}

	//위로 이동
	else if ( keyNum == 72 ) {
		if ( map[char_POS.y - 1][char_POS.x / 2] == 0 ) {
			if ( map[char_POS.y][char_POS.x / 2] == 4 )
			{
				char_POS.y -= 1;
				setCursor ( char_POS.x, char_POS.y + 1 );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( monsterAppear_probability[rand ( ) % 10] == 1 )
				{
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					miniGame ( Legends_charNum );
				}
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 0 )
			{
				char_POS.y -= 1;
				setCursor ( char_POS.x, char_POS.y + 1 );
				printf ( "  " );		//잔상 제거.
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 13 )
			{
				char_POS.y -= 1;
				setCursor ( char_POS.x, char_POS.y + 1 );
				printf ( "  " );
			}
		}
		else if ( map[char_POS.y - 1][char_POS.x / 2] == 23 )
		{
			setCursor ( char_POS.x, char_POS.y );
			printf ( "  " );
			main_Teleport ( 23, map );
		}
		else if ( map[char_POS.y - 1][char_POS.x / 2] == 4 ) {
			if ( map[char_POS.y][char_POS.x / 2] == 4 )
			{
				char_POS.y -= 1;
				setCursor ( char_POS.x, char_POS.y + 1 );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( monsterAppear_probability[rand ( ) % 10] == 1 )
				{
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					miniGame ( Legends_charNum );
				}
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 0 )
			{
				char_POS.y -= 1;
				setCursor ( char_POS.x, char_POS.y + 1 );
				printf ( "  " );
			}
		}
		else if ( map[char_POS.y - 1][char_POS.x / 2] == 13 ) {
			while ( map[char_POS.y - 1][char_POS.x / 2] == 13 ) {
				char_POS.y -= 1;
				setCursor ( char_POS.x, char_POS.y + 1 );
				printf ( "  " );
				Sleep ( 20 );
			}
		}
		else if ( map[char_POS.y - 1][char_POS.x / 2] == 8 )
		{
			char_POS.y -= 1;
			setCursor ( char_POS.x, char_POS.y + 1 );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 1, map );
		}
		else if ( map[char_POS.y - 1][char_POS.x / 2] == 9 )
		{
			char_POS.y -= 1;
			setCursor ( char_POS.x, char_POS.y + 1 );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 2, map );
		}
		else if ( map[char_POS.y - 1][char_POS.x / 2] == 10 )
		{
			char_POS.y -= 1;
			setCursor ( char_POS.x, char_POS.y + 1 );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 3, map );
		}
		else if ( map[char_POS.y - 1][char_POS.x / 2] == 11 )
		{
			if ( map[char_POS.y][char_POS.x / 2] == 0 ) {
				char_POS.y -= 1;
				setCursor ( char_POS.x, char_POS.y + 1 );
				printf ( " " );
				if ( Legends_charNum == 1 ) {
					King.hasItem++;
				}
				else if ( Legends_charNum == 2 ) {
					Knight.hasItem++;
				}
				else if ( Legends_charNum == 3 ) {
					Magician.hasItem++;
				}
				else if ( Legends_charNum == 4 ) {
					Rogue.hasItem++;
				}
				map[char_POS.y][char_POS.x / 2] = 0;
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 4 ) {
				char_POS.y -= 1;
				setCursor ( char_POS.x, char_POS.y + 1 );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( Legends_charNum == 1 ) {
					King.hasItem++;
				}
				else if ( Legends_charNum == 2 ) {
					Knight.hasItem++;
				}
				else if ( Legends_charNum == 3 ) {
					Magician.hasItem++;
				}
				else if ( Legends_charNum == 4 ) {
					Rogue.hasItem++;
				}
				map[char_POS.y][char_POS.x / 2] = 0;
			}
		}
	}

	//아래로 이동
	else if ( keyNum == 80 ) {
		if ( map[char_POS.y + 1][char_POS.x / 2] == 0 ) {
			if ( map[char_POS.y][char_POS.x / 2] == 4 )
			{
				char_POS.y += 1;
				setCursor ( char_POS.x, char_POS.y - 1 );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( monsterAppear_probability[rand ( ) % 10] == 1 )
				{
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					miniGame ( Legends_charNum );
				}
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 0 )
			{
				char_POS.y += 1;
				setCursor ( char_POS.x, char_POS.y - 1 );
				printf ( "  " );		//잔상 제거.
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 13 )
			{
				char_POS.y += 1;
				setCursor ( char_POS.x, char_POS.y - 1 );
				printf ( "  " );
			}
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 21 )
		{
			setCursor ( char_POS.x, char_POS.y );
			printf ( "  " );
			main_Teleport ( 21, map );
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 24 )
		{
			setCursor ( char_POS.x, char_POS.y );
			printf ( "  " );
			main_Teleport ( 24, map );
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 4 ) {
			if ( map[char_POS.y][char_POS.x / 2] == 4 )
			{
				char_POS.y += 1;
				setCursor ( char_POS.x, char_POS.y - 1 );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( monsterAppear_probability[rand ( ) % 10] == 1 )
				{
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					mini_MapWhite ( );
					mini_MapBlack ( );
					miniGame ( Legends_charNum );
				}
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 0 )
			{
				char_POS.y += 1;
				setCursor ( char_POS.x, char_POS.y - 1 );
				printf ( "  " );		//잔상 제거.
			}
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 13 ) {
			while ( map[char_POS.y + 1][char_POS.x / 2] == 13 ) {
				char_POS.y += 1;
				setCursor ( char_POS.x, char_POS.y - 1 );
				printf ( "  " );
				Sleep ( 20 );
			}
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 3 ) {
			char_POS.y += 2;
			setCursor ( char_POS.x, char_POS.y - 2 );
			printf ( "  " );
			Sleep ( 20 );
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 8 )
		{
			char_POS.y += 1;
			setCursor ( char_POS.x, char_POS.y - 1 );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 1, map );
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 9 )
		{
			char_POS.y += 1;
			setCursor ( char_POS.x, char_POS.y - 1 );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 2, map );
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 10 )
		{
			char_POS.y += 1;
			setCursor ( char_POS.x, char_POS.y - 1 );
			printf ( "  " );		//잔상 제거.
			SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_MapWhite ( );
			mini_MapBlack ( );
			mini_BossMapViewer ( Legends_charNum, 3, map );
		}
		else if ( map[char_POS.y + 1][char_POS.x / 2] == 11 )
		{
			if ( map[char_POS.y][char_POS.x / 2] == 0 ) {
				char_POS.y += 1;
				setCursor ( char_POS.x, char_POS.y - 1 );
				printf ( " " );
				if ( Legends_charNum == 1 ) {
					King.hasItem++;
				}
				else if ( Legends_charNum == 2 ) {
					Knight.hasItem++;
				}
				else if ( Legends_charNum == 3 ) {
					Magician.hasItem++;
				}
				else if ( Legends_charNum == 4 ) {
					Rogue.hasItem++;
				}
				map[char_POS.y][char_POS.x / 2] = 0;
			}
			else if ( map[char_POS.y][char_POS.x / 2] == 4 ) {
				char_POS.y += 1;
				setCursor ( char_POS.x, char_POS.y - 1 );
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
				printf ( "⊥" );
				if ( Legends_charNum == 1 ) {
					King.hasItem++;
				}
				else if ( Legends_charNum == 2 ) {
					Knight.hasItem++;
				}
				else if ( Legends_charNum == 3 ) {
					Magician.hasItem++;
				}
				else if ( Legends_charNum == 4 ) {
					Rogue.hasItem++;
				}
				map[char_POS.y][char_POS.x / 2] = 0;
			}
		}
	}
	current_State ( Legends_charNum );
	setCursor ( char_POS.x, char_POS.y );		//좌표 수정 반영한 커서 위치 재설정
}



void main_Teleport ( int num, char ( *map )[54] )
{
	if ( num == 21 ) {
		char_POS.x = 18;
		char_POS.y = 11;
	}
	else if ( num == 22 ) {
		char_POS.x = 6;
		char_POS.y = 2;
	}
	else if ( num == 23 ) {
		char_POS.x = 4;
		char_POS.y = 36;
	}
	else if ( num == 24 ) {
		char_POS.x = 8;
		char_POS.y = 14;
	}
}

void main_clearWall ( char ( *map )[54] )							//보스 클리어할 경우 벽 무력화.
{
	if ( count == 1 ) {
		map[32][32] = 0;
		map[32][33] = 0;
		map[32][34] = 0;
		map[32][35] = 0;
		map[31][35] = 0;
		map[30][35] = 0;
		map[29][35] = 0;
		map[28][35] = 0;
		map[28][36] = 0;
		map[28][37] = 0;
		map[28][38] = 0;
		map[28][39] = 0;

		map[35][42] = 0;
		map[35][43] = 0;
		map[35][44] = 0;
		map[35][45] = 0;
		map[34][44] = 0;
		map[34][45] = 0;
		map[34][43] = 0;
		map[34][42] = 0;
		map[33][44] = 0;
		map[33][45] = 0;
		map[32][44] = 0;
		map[32][45] = 0;
		map[31][44] = 0;
		map[31][45] = 0;

		map[32][49] = 0;
		map[32][50] = 0;
		map[31][49] = 0;
		map[31][50] = 0;

		map[19][46] = 0;
		map[19][47] = 0;
		map[18][46] = 0;
		map[18][47] = 0;
		map[17][46] = 0;
		map[17][47] = 0;
		map[16][46] = 0;
		map[16][47] = 0;
	}
	else if ( count == 2 ) {
		map[2][40] = 0;

		map[20][20] = 0;
		map[20][21] = 0;
		map[21][20] = 0;
		map[21][21] = 0;

		map[37][14] = 0;
		map[37][15] = 0;
		map[37][16] = 0;
		map[37][17] = 0;
		map[37][18] = 0;
		map[37][19] = 0;
		map[37][20] = 0;
		map[36][20] = 0;
		map[35][20] = 0;
		map[34][20] = 0;
		map[33][20] = 0;
		map[32][20] = 0;
		map[33][19] = 0;
		map[33][18] = 0;
		map[33][17] = 0;
		map[33][16] = 0;
		map[33][15] = 0;
		map[34][15] = 0;
		map[35][15] = 0;
		map[35][16] = 0;
		map[35][17] = 0;
	}
	else if ( count == 3 ) {
		map[20][17] = 0;
		map[20][18] = 0;
		map[20][19] = 0;
		map[21][17] = 0;
		map[21][18] = 0;
		map[21][19] = 0;

		map[15][14] = 0;

		map[18][30] = 0;
		map[18][31] = 0;
		map[19][30] = 0;
		map[19][31] = 0;
	}
}




void monsterInit ( int monsterNum )		//몬스터 정보 초기화
{
	int weight = 1.4;
	int level = 0;

	if ( charLevel < 4 ) {
		MonsterNum[monsterNum].level = 1 + rand ( ) % (charLevel + 1);
		//	level = MonsterNum[monsterNum].level;
	}
	else {
		MonsterNum[monsterNum].level = 1 + rand ( ) % 4 + (charLevel - 3);
		//	level = MonsterNum[monsterNum].level;
	}
	level = MonsterNum[monsterNum].level;
	MonsterNum[monsterNum].damageMin = 2 + (level * weight);
	MonsterNum[monsterNum].damageMax = 4 + (level * weight);
	MonsterNum[monsterNum].life = 15 + ((level + 2) * (weight + 0.4));
	MonsterNum[monsterNum].giveEX = 5 + ((level + 2) * weight);
	MonsterNum[0].name = "이상해씨";
	MonsterNum[1].name = "꼬부기";
	MonsterNum[2].name = "고스트";
}

void BossInit ( )
{
	BossNum1.level = 3;
	BossNum1.damageMin = 6;
	BossNum1.damageMax = 9;
	BossNum1.life = 40;
	BossNum1.giveEX = 50;

	BossNum2.level = 6;
	BossNum2.damageMin = 10;
	BossNum2.damageMax = 15;
	BossNum2.life = 65;
	BossNum2.giveEX = 100;

	BossNum3.level = 10;
	BossNum3.damageMin = 17;
	BossNum3.damageMax = 20;
	BossNum3.life = 105;
	BossNum3.giveEX = 150;
}



void miniGame ( int charNum )					//미니게임 관련 함수
{
	mini_Map_Viewer ( charNum );
}

void mini_Map_Viewer ( int charNum )			//미니 게임 맵1 그리기
{
	int menu;
	int result;
	monsterNum = rand ( ) % 3;

	if ( monsterNum == 0 ) {
		//이상해씨
		//0:  , 1:■, 2:─, 3:│, 4:┬, 5:■(노랑), 6: ■(캐릭터(주인공)), 7: ■(몬스터), 8:■(초록), 9: ■(빨강), 10: ■(회색), 11: ■(보라), 12: ■(노랑)
		char mini_Map[39][54] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,8,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,0,8,0,8,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,0,0,0,0,0,8,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,8,0,7,7,0,0,0,0,0,0,0,0,8,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,7,7,7,0,7,0,0,0,0,0,0,0,8,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,8,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,7,7,0,0,7,0,0,0,0,0,0,0,8,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,5,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,0,7,0,0,0,0,0,0,0,8,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,0,7,0,0,0,0,0,0,0,8,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,7,7,0,0,7,7,0,0,0,0,0,0,8,8,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,5,5,6,6,6,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,8,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,0,0,0,7,7,0,0,0,0,7,0,0,8,8,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,7,7,7,0,0,0,0,0,0,0,7,8,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,7,0,7,0,0,7,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,6,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,0,0,7,7,0,0,0,7,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,6,6,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,7,7,0,0,0,7,7,0,0,0,7,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,0,7,0,7,0,0,0,7,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,6,0,0,6,0,0,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,0,7,7,7,7,7,0,0,0,7,7,7,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,0,0,6,6,0,0,0,6,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,6,6,0,6,0,0,0,6,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
		};

		for ( int i = 0; i<39; i++ )
		{
			for ( int j = 0; j<54; j++ )
			{
				if ( mini_Map[i][j] == 0 )
					printf ( "  " );
				else if ( mini_Map[i][j] == 1 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 2 )
					printf ( "─" );
				else if ( mini_Map[i][j] == 3 )
					printf ( "│" );
				else if ( mini_Map[i][j] == 4 )
					printf ( "┬" );
				else if ( mini_Map[i][j] == 5 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 6 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 7 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 8 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 9 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0c );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 10 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x07 );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 11 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0d );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 12 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
				}
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			}
			printf ( "\n" );
		}
	}
	else if ( monsterNum == 1 ) {
		//꼬부기
		//0:  , 1:■, 2:─, 3:│, 4:┬, 5:■(노랑), 6: ■(캐릭터(주인공)), 7: ■(몬스터), 8:■(초록), 9: ■(파랑), 10: ■(회색), 11: ■(보라), 12: ■(노랑), 13: ■(빨강)
		char mini_Map[39][54] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,7,7,0,0,0,7,7,0,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,7,7,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,5,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,9,0,12,12,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,9,7,9,9,9,9,9,9,9,0,12,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,7,0,0,0,0,9,0,0,9,0,0,12,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,5,5,6,6,6,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,7,0,0,0,9,0,0,9,7,0,0,12,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,0,7,0,0,9,0,0,9,0,7,0,0,12,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,7,0,0,0,9,9,0,0,7,0,12,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,0,12,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,6,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,7,0,0,0,0,7,9,12,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,6,6,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,7,7,7,7,0,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,9,0,0,0,0,9,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,6,0,0,6,0,0,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,0,0,9,9,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,0,0,6,6,0,0,0,6,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,6,6,0,6,0,0,0,6,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
		};

		for ( int i = 0; i<39; i++ )
		{
			for ( int j = 0; j<54; j++ )
			{
				if ( mini_Map[i][j] == 0 )
					printf ( "  " );
				else if ( mini_Map[i][j] == 1 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 2 )
					printf ( "─" );
				else if ( mini_Map[i][j] == 3 )
					printf ( "│" );
				else if ( mini_Map[i][j] == 4 )
					printf ( "┬" );
				else if ( mini_Map[i][j] == 5 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 6 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 7 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 8 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 9 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0b );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 10 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x07 );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 11 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0d );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 12 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 13 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0c );
					printf ( "■" );
				}
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			}
			printf ( "\n" );
		}
	}
	else if ( monsterNum == 2 ) {
		//고스트
		//0:  , 1:■, 2:─, 3:│, 4:┬, 5:■(노랑), 6: ■(캐릭터(주인공)), 7: ■(몬스터), 8:■(빨강), 9: ■(보라)
		char mini_Map[39][54] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,0,9,0,9,0,9,9,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,9,0,9,0,9,0,0,9,9,9,9,9,9,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,5,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,8,0,0,0,0,0,0,0,8,0,0,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,8,0,8,0,0,0,0,0,8,0,8,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,5,5,6,6,6,5,5,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,8,0,7,8,0,0,0,8,7,0,8,9,9,9,9,9,9,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,0,8,0,0,0,8,0,8,0,0,0,8,0,0,0,0,9,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,8,8,8,0,0,0,8,8,8,0,0,0,9,9,9,9,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,7,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,6,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,7,0,0,0,0,0,0,0,7,0,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,6,6,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,7,7,7,7,7,7,7,0,0,9,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,9,9,9,0,0,0,9,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,6,0,0,6,0,0,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,0,0,0,9,0,0,9,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,0,0,6,6,0,0,0,6,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,0,0,9,0,0,9,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,6,6,0,6,0,0,0,6,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
		};

		for ( int i = 0; i<39; i++ )
		{
			for ( int j = 0; j<54; j++ )
			{
				if ( mini_Map[i][j] == 0 )
					printf ( "  " );
				else if ( mini_Map[i][j] == 1 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 2 )
					printf ( "─" );
				else if ( mini_Map[i][j] == 3 )
					printf ( "│" );
				else if ( mini_Map[i][j] == 4 )
					printf ( "┬" );
				else if ( mini_Map[i][j] == 5 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 6 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 7 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 8 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0c );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 9 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0d );
					printf ( "■" );
				}
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			}
			printf ( "\n" );
		}
	}
	monsterInit ( monsterNum );

	showCharState ( charNum, 0 );

more:
	menu = mini_Control ( charNum );
	if ( menu == 1 )		//공격
	{
		result = mini_Fight ( charNum, 1, monsterNum );
		showCharState ( charNum, 0 );
	}
	else if ( menu == 2 )		//스킬
	{
		result = mini_Fight ( charNum, 2, monsterNum );
		showCharState ( charNum, 0 );
	}
	else if ( menu == 3 )		//아이템
	{
		result = mini_Fight ( charNum, 3, monsterNum );
		showCharState ( charNum, 0 );
	}
	else if ( menu == 4 )	//도망
	{
		result = mini_Fight ( charNum, 4, monsterNum );
		showCharState ( charNum, 0 );
	}


	if ( result == 1 )
	{
		setCursor ( 48, 32 );
		if ( charNum == 1 ) {
			printf ( "King이 싸움에서 졌습니다." );
			Sleep ( 700 );
			goto charDie;
		}
		else if ( charNum == 2 ) {
			printf ( "Knight가 싸움에서 졌습니다." );
			Sleep ( 700 );
			goto charDie;
		}
		else if ( charNum == 3 ) {
			printf ( "Magician이 싸움에서 졌습니다." );
			Sleep ( 700 );
			goto charDie;
		}
		else if ( charNum == 4 ) {
			printf ( "Rogue가 싸움에서 졌습니다." );
			Sleep ( 700 );
			goto charDie;
		}
		Sleep ( 2500 );
		goto skip;
	}
	else if ( result == 2 )
	{
		goto skip;
	}

	if ( MonsterNum[monsterNum].life > 0 )
	{
		goto more;
	}

	setCursor ( 48, 32 );

	if ( monsterNum == 0 ) {
		printf ( "이상해씨를 해치웠습니다!" );
		setCursor ( 48, 34 );
		printf ( "경험치 %d를 획득했습니다!", MonsterNum[monsterNum].giveEX );
		Sleep ( 800 );
	}
	else if ( monsterNum == 1 ) {
		printf ( "꼬부기를 해치웠습니다!" );
		setCursor ( 48, 34 );
		printf ( "경험치 %d를 획득했습니다!", MonsterNum[monsterNum].giveEX );
		Sleep ( 800 );
	}
	else if ( monsterNum == 2 ) {
		printf ( "고스트를 해치웠습니다!" );
		setCursor ( 48, 34 );
		printf ( "경험치 %d를 획득했습니다!", MonsterNum[monsterNum].giveEX );
		Sleep ( 800 );
	}
	if ( charNum == 1 ) {
		King.currentEX += MonsterNum[monsterNum].giveEX;

		if ( King.currentEX >= King.experiencePoint )
			charLevelUp ( Legends_charNum, King.currentEX - King.experiencePoint );
		showCharState ( charNum, 0 );
	}
	else if ( charNum == 2 ) {
		Knight.currentEX += MonsterNum[monsterNum].giveEX;

		if ( Knight.currentEX >= Knight.experiencePoint )
			charLevelUp ( Legends_charNum, Knight.currentEX - Knight.experiencePoint );
		showCharState ( charNum, 0 );
	}
	else if ( charNum == 3 ) {
		Magician.currentEX += MonsterNum[monsterNum].giveEX;

		if ( Magician.currentEX >= Magician.experiencePoint )
			charLevelUp ( Legends_charNum, Magician.currentEX - Magician.experiencePoint );
		showCharState ( charNum, 0 );
	}
	else if ( charNum == 4 ) {
		Rogue.currentEX += MonsterNum[monsterNum].giveEX;

		if ( Rogue.currentEX >= Rogue.experiencePoint )
			charLevelUp ( Legends_charNum, Rogue.currentEX - Rogue.experiencePoint );
		showCharState ( charNum, 0 );
	}
	Sleep ( 1500 );

	if ( charNum == 1 ) {
		if ( King.life + 3 > King.MaxLife )
			King.life = King.MaxLife;
		else
			King.life += 3;
	}
	else if ( charNum == 2 ) {
		if ( Knight.life + 3 > Knight.MaxLife )
			Knight.life = Knight.MaxLife;
		else
			Knight.life += 3;
	}
	else if ( charNum == 3 ) {
		if ( Magician.life + 3 > Magician.MaxLife )
			Magician.life = Magician.MaxLife;
		else
			Magician.life += 3;
	}
	else if ( charNum == 4 ) {
		if ( Rogue.life + 3 > Rogue.MaxLife )
			Rogue.life = Rogue.MaxLife;
		else
			Rogue.life += 3;
	}

skip:
	//monsterInit();	//몬스터 값 다시 초기화

	system ( "cls" );	//화면 비우기
	main_MapViewer ( );	//메인 맵 다시 불러오기

charDie:
	if ( King.life <= 0 || Knight.life <= 0 || Magician.life <= 0 || Rogue.life <= 0 ) {
		char_POS.x = 46;
		char_POS.y = 36;
		//charInit();
		if ( charNum == 1 ) {
			King.experiencePoint = 0;
			King.life = King.MaxLife;
			King.hasItem = 0;
		}
		else if ( charNum == 2 ) {
			Knight.experiencePoint = 0;
			Knight.life = Knight.MaxLife;
			Knight.hasItem = 0;
		}
		else if ( charNum == 3 ) {
			Magician.experiencePoint = 0;
			Magician.life = Magician.MaxLife;
			Magician.hasItem = 0;
		}
		else if ( charNum == 4 ) {
			Rogue.experiencePoint = 0;
			Rogue.life = Rogue.MaxLife;
			Rogue.hasItem = 0;
		}

		system ( "cls" );
		main_MapViewer ( );
	}
}

void mini_MapWhite ( )	//몬스터 출현!! 맵을 하얗게 만들자!
{
	char mini_MapWhite[39][54] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};

	for ( int i = 0; i<39; i++ ) {
		for ( int j = 0; j<54; j++ ) {
			if ( mini_MapWhite[i][j] == 0 )
				printf ( "  " );
			else if ( mini_MapWhite[i][j] == 1 )
				printf ( "■" );
		}
		printf ( "\n" );
	}
	Sleep ( 100 );
}

void mini_MapBlack ( )	//몬스터 출현!! 맵을 까맣게 만들자!
{
	system ( "cls" );
	Sleep ( 100 );
}

int mini_Control ( int charNum )
{
	mini_stateTableClear ( );

	int keyNum;

	setCursor ( 8, 28 );
	printf ( "1. 공격" );
	setCursor ( 8, 30 );
	printf ( "2. 스킬 공격" );
	setCursor ( 8, 32 );
	printf ( "3. 아이템" );
	setCursor ( 8, 34 );
	printf ( "4. 도망" );
	setCursor ( 48, 28 );
	printf ( "하실 행동을 선택하세요" );

	char_POS_mini.x = 4;
	char_POS_mini.y = 28;
	while ( 1 )
	{
		setCursor ( char_POS_mini.x, char_POS_mini.y );
		printf ( "▶" );	//메뉴 선택 아이콘 출력
		keyNum = getch ( );		//방향키 입력받은 정보 저장.

		if ( keyNum == 72 ) {	//위로 이동
			if ( char_POS_mini.y == 34 ) {	//맨 위가 아닐 경우
				char_POS_mini.y -= 2;
				setCursor ( char_POS_mini.x, char_POS_mini.y + 2 );
				printf ( "  " );		//잔상 제거
			}
			else if ( char_POS_mini.y == 32 ) {
				char_POS_mini.y -= 2;
				setCursor ( char_POS_mini.x, char_POS_mini.y + 2 );
				printf ( "  " );		//잔상 제거
			}
			else if ( char_POS_mini.y == 30 ) {
				char_POS_mini.y -= 2;
				setCursor ( char_POS_mini.x, char_POS_mini.y + 2 );
				printf ( "  " );		//잔상 제거
			}
			else if ( char_POS_mini.y == 28 ) {	//맨 위일 경우
				char_POS_mini.y += 6;
				setCursor ( char_POS_mini.x, char_POS_mini.y - 6 );
				printf ( "  " );
			}
		}
		else if ( keyNum == 80 ) {	//아래로 이동
			if ( char_POS_mini.y == 32 ) {	//맨 아래가 아닐 경우
				char_POS_mini.y += 2;
				setCursor ( char_POS_mini.x, char_POS_mini.y - 2 );
				printf ( "  " );		//잔상 제거
			}
			else if ( char_POS_mini.y == 30 ) {
				char_POS_mini.y += 2;
				setCursor ( char_POS_mini.x, char_POS_mini.y - 2 );
				printf ( "  " );		//잔상 제거
			}
			else if ( char_POS_mini.y == 28 ) {
				char_POS_mini.y += 2;
				setCursor ( char_POS_mini.x, char_POS_mini.y - 2 );
				printf ( "  " );		//잔상 제거
			}
			else if ( char_POS_mini.y == 34 ) {	//맨 아래일 경우
				char_POS_mini.y -= 6;
				setCursor ( char_POS_mini.x, char_POS_mini.y + 6 );
				printf ( "  " );
			}
		}
		setCursor ( char_POS_mini.x, char_POS_mini.y );		//좌표 수정 반영한 커서 위치 재설정

		if ( keyNum == 13 )		//메뉴 선택했을 경우
		{
			if ( char_POS_mini.y == 28 ) {
				setCursor ( 48, 28 );
				printf ( "							" );
				return 1;
			}
			else if ( char_POS_mini.y == 30 ) {
				setCursor ( 48, 28 );
				printf ( "							" );
				return 2;
			}
			else if ( char_POS_mini.y == 32 ) {
				setCursor ( 48, 28 );
				printf ( "							" );
				return 3;
			}
			else if ( char_POS_mini.y == 34 ) {
				setCursor ( 48, 28 );
				printf ( "							" );
				return 4;
			}
		}
	}
	setCursor ( 4, 30 ); printf ( "  " );
	setCursor ( 4, 32 ); printf ( "  " );
	setCursor ( 4, 34 ); printf ( "  " );
}

void mini_monsterControl ( int charNum, int menuSelect, int monsterNum )
{
	mini_stateTableClear ( );
	int damage;

	if ( menuSelect == 1 )		//공격 선택시
	{
		if ( charNum == 1 )		//영웅 : King 일 경우
		{
			setCursor ( 48, 28 );
			printf ( "%s의 공격!", MonsterNum[monsterNum].name );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			damage = rand ( ) % ((int)MonsterNum[monsterNum].damageMax - (int)MonsterNum[monsterNum].damageMin + 1) + (int)MonsterNum[monsterNum].damageMin;
			printf ( "King이 %d 피해를 받았습니다.", damage );
			King.life -= damage;

			if ( King.life < 0 )
				King.life = 0;
			if ( King.currentEX < 0 )
				King.currentEX = 0;

			showCharState ( charNum, 0 );
			Sleep ( 1100 );
		}
		else if ( charNum == 2 )
		{
			setCursor ( 48, 28 );
			printf ( "%s의 공격!", MonsterNum[monsterNum].name );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			damage = rand ( ) % ((int)MonsterNum[monsterNum].damageMax - (int)MonsterNum[monsterNum].damageMin + 1) + (int)MonsterNum[monsterNum].damageMin;
			printf ( "Knight이 %d 피해를 받았습니다.", damage );
			Knight.life -= damage;

			if ( Knight.life < 0 )
				Knight.life = 0;
			if ( Knight.currentEX < 0 )
				Knight.currentEX = 0;

			showCharState ( charNum, 0 );
			Sleep ( 1100 );
		}
		else if ( charNum == 3 )
		{
			setCursor ( 48, 28 );
			printf ( "%s의 공격!", MonsterNum[monsterNum].name );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			damage = rand ( ) % ((int)MonsterNum[monsterNum].damageMax - (int)MonsterNum[monsterNum].damageMin + 1) + (int)MonsterNum[monsterNum].damageMin;
			printf ( "Magician이 %d 피해를 받았습니다.", damage );
			Magician.life -= damage;

			if ( Magician.life < 0 )
				Magician.life = 0;
			if ( Magician.currentEX < 0 )
				Magician.currentEX = 0;

			showCharState ( charNum, 0 );
			Sleep ( 1100 );
		}
		else if ( charNum == 4 )
		{
			setCursor ( 48, 28 );
			printf ( "%s의 공격!", MonsterNum[monsterNum].name );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			damage = rand ( ) % ((int)MonsterNum[monsterNum].damageMax - (int)MonsterNum[monsterNum].damageMin + 1) + (int)MonsterNum[monsterNum].damageMin;
			printf ( "Rogue이 %d 피해를 받았습니다.", damage );
			Rogue.life -= damage;

			if ( Rogue.life < 0 )
				Rogue.life = 0;
			if ( Rogue.currentEX < 0 )
				Rogue.currentEX = 0;

			showCharState ( charNum, 0 );
			Sleep ( 1100 );
		}
	}
	else if ( menuSelect == 4 )
	{
		//empty
	}
}

void showCharState ( int charNum, int BossNum )
{
	setCursor ( 20, 7 );
	if ( charNum == 1 ) {
		printf ( "King" );
		setCursor ( 30, 7 );
		printf ( "Lv : %d  ", King.level );
		setCursor ( 20, 8 );
		printf ( "HP : %d / %d ", King.life, King.MaxLife );
		setCursor ( 20, 9 );
		printf ( "EX : %d / %d  ", King.currentEX, King.experiencePoint );
	}
	else if ( charNum == 2 ) {
		printf ( "Knight" );
		setCursor ( 30, 7 );
		printf ( "Lv : %d  ", Knight.level );
		setCursor ( 20, 8 );
		printf ( "HP : %d / %d ", Knight.life, Knight.MaxLife );
		setCursor ( 20, 9 );
		printf ( "EX : %d / %d  ", Knight.currentEX, Knight.experiencePoint );
	}
	else if ( charNum == 3 ) {
		printf ( "Magician" );
		setCursor ( 30, 7 );
		printf ( "Lv : %d  ", Magician.level );
		setCursor ( 20, 8 );
		printf ( "HP : %d / %d ", Magician.life, Magician.MaxLife );
		setCursor ( 20, 9 );
		printf ( "EX : %d / %d  ", Magician.currentEX, Magician.experiencePoint );
	}
	else if ( charNum == 4 ) {
		printf ( "Rogue" );
		setCursor ( 30, 7 );
		printf ( "Lv : %d  ", Rogue.level );
		setCursor ( 20, 8 );
		printf ( "HP : %d / %d ", Rogue.life, Rogue.MaxLife );
		setCursor ( 20, 9 );
		printf ( "EX : %d / %d  ", Rogue.currentEX, Rogue.experiencePoint );
	}

	if ( BossNum == 0 )
	{
		if ( monsterNum == 0 ) {
			setCursor ( 60, 4 );
			printf ( "이상해씨" );
			setCursor ( 70, 4 );
			printf ( "Lv : %d  ", MonsterNum[monsterNum].level );
			setCursor ( 60, 5 );
			printf ( "HP : %d  ", MonsterNum[monsterNum].life );
		}
		else if ( monsterNum == 1 ) {
			setCursor ( 60, 4 );
			printf ( "꼬부기" );
			setCursor ( 70, 4 );
			printf ( "Lv : %d  ", MonsterNum[monsterNum].level );
			setCursor ( 60, 5 );
			printf ( "HP : %d  ", MonsterNum[monsterNum].life );
		}
		else if ( monsterNum == 2 ) {
			setCursor ( 60, 4 );
			printf ( "고스트" );
			setCursor ( 70, 4 );
			printf ( "Lv : %d  ", MonsterNum[monsterNum].level );
			setCursor ( 60, 5 );
			printf ( "HP : %d  ", MonsterNum[monsterNum].life );
		}
	}
	else if ( BossNum == 1 )
	{
		setCursor ( 54, 4 );
		printf ( "대장 이상해씨" );
		setCursor ( 70, 4 );
		printf ( "Lv : %d  ", BossNum1.level );
		setCursor ( 60, 5 );
		printf ( "HP : %d  ", BossNum1.life );
	}
	else if ( BossNum == 2 )
	{
		setCursor ( 56, 4 );
		printf ( "대장 꼬부기" );
		setCursor ( 70, 4 );
		printf ( "Lv : %d  ", BossNum2.level );
		setCursor ( 60, 5 );
		printf ( "HP : %d  ", BossNum2.life );
	}
	else if ( BossNum == 3 )
	{
		setCursor ( 56, 4 );
		printf ( "대장 고스트" );
		setCursor ( 70, 4 );
		printf ( "Lv : %d  ", BossNum3.level );
		setCursor ( 60, 5 );
		printf ( "HP : %d  ", BossNum3.life );
	}
}

void mini_stateTableClear ( )
{
	setCursor ( 48, 28 );
	printf ( "　　　　　　　　　　　　　　　　　　　　　　　　　　　　" );
	setCursor ( 48, 30 );
	printf ( "　　　　　　　　　　　　　　　　　　　　　　　　　　　　" );
	setCursor ( 48, 32 );
	printf ( "　　　　　　　　　　　　　　　　　　　　　　　　　　　　" );
	setCursor ( 48, 34 );
	printf ( "　　　　　　　　　　　　　　　　　　　　　　　　　　　　" );
	setCursor ( 4, 28 );
	printf ( "　" );
	setCursor ( 4, 30 );
	printf ( "　" );
	setCursor ( 4, 32 );
	printf ( "　" );
	setCursor ( 4, 34 );
	printf ( "　" );
}

void charLevelUp ( int charNum, int surplusEX )
{
	if ( charNum == 1 ) {
		King.currentEX = surplusEX;
		King.level++;
		charLevel = King.level;
		King.experiencePoint = King.level * 10 + (King.level - 1) * 5;
		King.damageMin = 2 + (King.level - 1) * King.levelUpBonus;
		King.damageMax = 4 + (King.level - 1) * King.levelUpBonus;
		King.life = 25 + King.level * 7 * King.levelUpBonus;
		King.MaxLife = King.life;

		SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0b );

		setCursor ( 34, 13 );
		printf ( "■■■■■■■■■■■■■■■■■■■■■" );
		setCursor ( 34, 14 );
		printf ( "■                                      ■" );
		setCursor ( 34, 15 );
		printf ( "■                                      ■" );
		setCursor ( 34, 16 );
		printf ( "■                                      ■" );
		setCursor ( 34, 17 );
		printf ( "■                                      ■" );
		setCursor ( 34, 18 );
		printf ( "■                                      ■" );
		setCursor ( 34, 19 );
		printf ( "■                                      ■" );
		setCursor ( 34, 20 );
		printf ( "■■■■■■■■■■■■■■■■■■■■■" );

		SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );

		setCursor ( 38, 15 );
		printf ( "Level UP!!!" );
		setCursor ( 38, 16 );
		printf ( "King State" );
		setCursor ( 38, 17 );
		printf ( "LV : %d		공격력 : %d ~ %d  ", King.level, (int)King.damageMin, (int)King.damageMax );
		setCursor ( 38, 18 );
		printf ( "HP : %d  ", King.life );
	}
	else if ( charNum == 2 ) {
		Knight.currentEX = surplusEX;
		Knight.level++;
		charLevel = Knight.level;
		Knight.experiencePoint = Knight.level * 10 + (Knight.level - 1) * 5;
		Knight.damageMin = 2 + (Knight.level - 1) * Knight.levelUpBonus;
		Knight.damageMax = 4 + (Knight.level - 1) * Knight.levelUpBonus;
		Knight.life = 30 + Knight.level * 7 * Knight.levelUpBonus;
		Knight.MaxLife = Knight.life;

		SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0b );

		setCursor ( 34, 13 );
		printf ( "■■■■■■■■■■■■■■■■■■■■■" );
		setCursor ( 34, 14 );
		printf ( "■                                      ■" );
		setCursor ( 34, 15 );
		printf ( "■                                      ■" );
		setCursor ( 34, 16 );
		printf ( "■                                      ■" );
		setCursor ( 34, 17 );
		printf ( "■                                      ■" );
		setCursor ( 34, 18 );
		printf ( "■                                      ■" );
		setCursor ( 34, 19 );
		printf ( "■                                      ■" );
		setCursor ( 34, 20 );
		printf ( "■■■■■■■■■■■■■■■■■■■■■" );

		SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );

		setCursor ( 38, 15 );
		printf ( "Level UP!!!" );
		setCursor ( 38, 16 );
		printf ( "Knight State" );
		setCursor ( 38, 17 );
		printf ( "LV : %d		공격력 : %d ~ %d  ", Knight.level, (int)Knight.damageMin, (int)Knight.damageMax );
		setCursor ( 38, 18 );
		printf ( "HP : %d  ", Knight.life );
	}
	else if ( charNum == 3 ) {
		Magician.currentEX = surplusEX;
		Magician.level++;
		charLevel = Magician.level;
		Magician.experiencePoint = Magician.level * 10 + (Magician.level - 1) * 5;
		Magician.damageMin = 2 + (Magician.level - 1) * Magician.levelUpBonus;
		Magician.damageMax = 4 + (Magician.level - 1) * Magician.levelUpBonus;
		Magician.life = 18 + Magician.level * 7 * Magician.levelUpBonus;
		Magician.MaxLife = Magician.life;

		SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0b );

		setCursor ( 34, 13 );
		printf ( "■■■■■■■■■■■■■■■■■■■■■" );
		setCursor ( 34, 14 );
		printf ( "■                                      ■" );
		setCursor ( 34, 15 );
		printf ( "■                                      ■" );
		setCursor ( 34, 16 );
		printf ( "■                                      ■" );
		setCursor ( 34, 17 );
		printf ( "■                                      ■" );
		setCursor ( 34, 18 );
		printf ( "■                                      ■" );
		setCursor ( 34, 19 );
		printf ( "■                                      ■" );
		setCursor ( 34, 20 );
		printf ( "■■■■■■■■■■■■■■■■■■■■■" );

		SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );

		setCursor ( 38, 15 );
		printf ( "Level UP!!!" );
		setCursor ( 38, 16 );
		printf ( "Magician State" );
		setCursor ( 38, 17 );
		printf ( "LV : %d		공격력 : %d ~ %d  ", Magician.level, (int)Magician.damageMin, (int)Magician.damageMax );
		setCursor ( 38, 18 );
		printf ( "HP : %d  ", Magician.life );
	}
	else if ( charNum == 4 ) {
		Rogue.currentEX = surplusEX;
		Rogue.level++;
		charLevel = Rogue.level;
		Rogue.experiencePoint = Rogue.level * 10 + (Rogue.level - 1) * 5;
		Rogue.damageMin = 2 + (Rogue.level - 1) * Rogue.levelUpBonus;
		Rogue.damageMax = 4 + (Rogue.level - 1) * Rogue.levelUpBonus;
		Rogue.life = 20 + Rogue.level * 7 * Rogue.levelUpBonus;
		Rogue.MaxLife = Rogue.life;

		SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0b );

		setCursor ( 34, 13 );
		printf ( "■■■■■■■■■■■■■■■■■■■■■" );
		setCursor ( 34, 14 );
		printf ( "■                                      ■" );
		setCursor ( 34, 15 );
		printf ( "■                                      ■" );
		setCursor ( 34, 16 );
		printf ( "■                                      ■" );
		setCursor ( 34, 17 );
		printf ( "■                                      ■" );
		setCursor ( 34, 18 );
		printf ( "■                                      ■" );
		setCursor ( 34, 19 );
		printf ( "■                                      ■" );
		setCursor ( 34, 20 );
		printf ( "■■■■■■■■■■■■■■■■■■■■■" );

		SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );

		setCursor ( 38, 15 );
		printf ( "Level UP!!!" );
		setCursor ( 38, 16 );
		printf ( "Rogue State" );
		setCursor ( 38, 17 );
		printf ( "LV : %d		공격력 : %d ~ %d  ", Rogue.level, (int)Rogue.damageMin, (int)Rogue.damageMax );
		setCursor ( 38, 18 );
		printf ( "HP : %d  ", Rogue.life );
	}
}

int mini_Fight ( int charNum, int menuSelect, int monsterNum )
{
	mini_stateTableClear ( );

	int damage = 0;

	if ( charNum == 1 )		//영웅 : King 일 경우
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			setCursor ( 48, 28 );
			printf ( "King의 공격!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			damage = rand ( ) % ((int)King.damageMax - (int)King.damageMin + 1) + (int)King.damageMin;
			printf ( "%s에게 %d 피해를 입혔습니다.", MonsterNum[monsterNum].name, damage );
			MonsterNum[monsterNum].life -= damage;

			if ( MonsterNum[monsterNum].life < 0 )
				MonsterNum[monsterNum].life = 0;

			showCharState ( charNum, 0 );
			Sleep ( 1300 );

			if ( MonsterNum[monsterNum].life <= 0 )
				return 0;

			mini_monsterControl ( charNum, 1, monsterNum );
			if ( King.life>0 )
				return 0;
			else
				return 1;
		}
		else if ( menuSelect == 2 )
		{
			charSkill ( charNum, monsterNum );

			if ( MonsterNum[monsterNum].life == 0 )
				return 0;

			mini_monsterControl ( charNum, 1, monsterNum );

			if ( King.life>0 )
				return 0;
			else
				return 1;
		}
		else if ( menuSelect == 3 )
		{
			if ( King.hasItem > 0 ) {
				King.hasItem = King.hasItem - 1;
				setCursor ( 48, 28 );
				printf ( "King이 아이템을 사용했습니다!" );
				Sleep ( 300 );
				setCursor ( 48, 30 );
				printf ( "체력을 20 회복했습니다" );
				if ( King.MaxLife - King.life < 20 )
					King.life = King.MaxLife;
				else
					King.life += 20;
				Sleep ( 600 );
			}
			else {
				setCursor ( 48, 28 );
				printf ( "현재 아이템이 없습니다" );
				Sleep ( 600 );
			}
		}
		else if ( menuSelect == 4 )
		{
			setCursor ( 48, 28 );
			printf ( "도망!!!!!!!!!!!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			int flee[2] = { 1,0 };
			if ( flee[rand ( ) % 2] == 1 ) {
				printf ( "도망 성공!" );
				Sleep ( 1100 );
				return 2;
			}
			else {
				printf ( "도망 실패 ㅠㅠ" );
				Sleep ( 1100 );
				mini_monsterControl ( charNum, 1, monsterNum );
				return 0;
			}
		}
	}
	else if ( charNum == 2 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			setCursor ( 48, 28 );
			printf ( "Knight의 공격!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			damage = rand ( ) % ((int)Knight.damageMax - (int)Knight.damageMin + 1) + (int)Knight.damageMin;
			printf ( "%s에게 %d 피해를 입혔습니다.", MonsterNum[monsterNum].name, damage );
			MonsterNum[monsterNum].life -= damage;

			if ( MonsterNum[monsterNum].life < 0 )
				MonsterNum[monsterNum].life = 0;

			showCharState ( charNum, 0 );
			Sleep ( 1300 );

			if ( MonsterNum[monsterNum].life <= 0 )
				return 0;

			mini_monsterControl ( charNum, 1, monsterNum );
			if ( Knight.life>0 )
				return 0;
			else
				return 1;
		}
		else if ( menuSelect == 2 )
		{
			charSkill ( charNum, monsterNum );

			if ( MonsterNum[monsterNum].life == 0 )
				return 0;

			mini_monsterControl ( charNum, 1, monsterNum );

			if ( Knight.life>0 )
				return 0;
			else
				return 1;
		}
		else if ( menuSelect == 3 )
		{
			if ( Knight.hasItem > 0 ) {
				setCursor ( 48, 28 );
				printf ( "Knight가 아이템을 사용했습니다!" );
				Knight.hasItem = Knight.hasItem - 1;
				Sleep ( 300 );
				setCursor ( 48, 30 );
				printf ( "체력을 20 회복했습니다" );
				if ( Knight.MaxLife - Knight.life < 20 )
					Knight.life = Knight.MaxLife;
				else
					Knight.life += 20;
				Sleep ( 600 );
			}
			else {
				setCursor ( 48, 28 );
				printf ( "현재 아이템이 없습니다" );
				Sleep ( 600 );
			}
		}
		else if ( menuSelect == 4 )
		{
			setCursor ( 48, 28 );
			printf ( "도망!!!!!!!!!!!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			int flee[2] = { 1,0 };
			if ( flee[rand ( ) % 2] == 1 ) {
				printf ( "도망 성공!" );
				Sleep ( 1100 );
				return 2;
			}
			else {
				printf ( "도망 실패 ㅠㅠ" );
				Sleep ( 1100 );
				mini_monsterControl ( charNum, 1, monsterNum );
				return 0;
			}
		}
	}
	else if ( charNum == 3 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			setCursor ( 48, 28 );
			printf ( "Magician의 공격!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			damage = rand ( ) % ((int)Magician.damageMax - (int)Magician.damageMin + 1) + (int)Magician.damageMin;
			printf ( "%s에게 %d 피해를 입혔습니다.", MonsterNum[monsterNum].name, damage );
			MonsterNum[monsterNum].life -= damage;

			if ( MonsterNum[monsterNum].life < 0 )
				MonsterNum[monsterNum].life = 0;

			showCharState ( charNum, 0 );
			Sleep ( 1300 );

			if ( MonsterNum[monsterNum].life <= 0 )
				return 0;

			mini_monsterControl ( charNum, 1, monsterNum );
			if ( Magician.life>0 )
				return 0;
			else
				return 1;
		}
		else if ( menuSelect == 2 )
		{
			charSkill ( charNum, monsterNum );

			if ( MonsterNum[monsterNum].life == 0 )
				return 0;

			mini_monsterControl ( charNum, 1, monsterNum );

			if ( Magician.life>0 )
				return 0;
			else
				return 1;
		}
		else if ( menuSelect == 3 )
		{
			if ( Magician.hasItem > 0 ) {
				setCursor ( 48, 28 );
				printf ( "Magician이 아이템을 사용했습니다!" );
				Magician.hasItem = Magician.hasItem - 1;
				Sleep ( 300 );
				setCursor ( 48, 30 );
				printf ( "체력을 20 회복했습니다" );
				if ( Magician.MaxLife - Magician.life < 20 )
					Magician.life = Magician.MaxLife;
				else
					Magician.life += 20;
				Sleep ( 600 );
			}
			else {
				setCursor ( 48, 28 );
				printf ( "현재 아이템이 없습니다" );
				Sleep ( 600 );
			}
		}
		else if ( menuSelect == 4 )
		{
			setCursor ( 48, 28 );
			printf ( "도망!!!!!!!!!!!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			int flee[2] = { 1,0 };
			if ( flee[rand ( ) % 2] == 1 ) {
				printf ( "도망 성공!" );
				Sleep ( 1100 );
				return 2;
			}
			else {
				printf ( "도망 실패 ㅠㅠ" );
				Sleep ( 1100 );
				mini_monsterControl ( charNum, 1, monsterNum );
				return 0;
			}
		}
	}
	else if ( charNum == 4 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			setCursor ( 48, 28 );
			printf ( "Rogue의 공격!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			damage = rand ( ) % ((int)Rogue.damageMax - (int)Rogue.damageMin + 1) + (int)Rogue.damageMin;
			printf ( "%s에게 %d 피해를 입혔습니다.", MonsterNum[monsterNum].name, damage );
			MonsterNum[monsterNum].life -= damage;

			if ( MonsterNum[monsterNum].life < 0 )
				MonsterNum[monsterNum].life = 0;

			showCharState ( charNum, 0 );
			Sleep ( 1300 );

			if ( MonsterNum[monsterNum].life <= 0 )
				return 0;

			mini_monsterControl ( charNum, 1, monsterNum );
			if ( Rogue.life>0 )
				return 0;
			else
				return 1;
		}
		else if ( menuSelect == 2 )
		{
			charSkill ( charNum, monsterNum );

			if ( MonsterNum[monsterNum].life == 0 )
				return 0;

			mini_monsterControl ( charNum, 1, monsterNum );

			if ( Rogue.life>0 )
				return 0;
			else
				return 1;
		}
		else if ( menuSelect == 3 )
		{
			if ( Rogue.hasItem > 0 ) {
				setCursor ( 48, 28 );
				printf ( "Rogue가 아이템을 사용했습니다!" );
				Rogue.hasItem = Rogue.hasItem - 1;
				Sleep ( 300 );
				setCursor ( 48, 30 );
				printf ( "체력을 20 회복했습니다" );
				if ( Rogue.MaxLife - Rogue.life < 20 )
					Rogue.life = Rogue.MaxLife;
				else
					Rogue.life += 20;
				Sleep ( 600 );
			}
			else {
				setCursor ( 48, 28 );
				printf ( "현재 아이템이 없습니다" );
				Sleep ( 600 );
			}
		}
		else if ( menuSelect == 4 )
		{
			setCursor ( 48, 28 );
			printf ( "도망!!!!!!!!!!!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			int flee[2] = { 1,0 };
			if ( flee[rand ( ) % 2] == 1 ) {
				printf ( "도망 성공!" );
				Sleep ( 1100 );
				return 2;
			}
			else {
				printf ( "도망 실패 ㅠㅠ" );
				Sleep ( 1100 );
				mini_monsterControl ( charNum, 1, monsterNum );
				return 0;
			}
		}
	}
}

void mini_BossMapViewer ( int charNum, int BossNum, char ( *map )[54] )		//보스 출현 맵
{
	int menu;
	int result;

	//0:  , 1:■, 2:─, 3:│, 4:┬, 5:■(노랑), 6: ■(캐릭터(주인공)), 7: ■(몬스터), 8:■(초록)
	if ( BossNum == 1 ) {
		char mini_Map[39][54] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,8,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,0,8,0,8,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,0,0,0,0,0,8,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,8,0,7,7,0,0,0,0,0,0,0,0,8,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,7,7,7,0,7,0,0,0,0,0,0,0,8,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,8,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,7,7,0,0,7,0,0,0,0,0,0,0,8,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,5,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,0,7,0,0,0,0,0,0,0,8,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,0,7,0,0,0,0,0,0,0,8,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,7,7,0,0,7,7,0,0,0,0,0,0,8,8,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,5,5,6,6,6,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,8,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,0,0,0,7,7,0,0,0,0,7,0,0,8,8,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,7,7,7,0,0,0,0,0,0,0,7,8,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,7,0,7,0,0,7,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,6,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,0,0,7,7,0,0,0,7,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,6,6,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,7,7,0,0,0,7,7,0,0,0,7,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,0,7,0,7,0,0,0,7,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,6,0,0,6,0,0,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,0,7,7,7,7,7,0,0,0,7,7,7,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,0,0,6,6,0,0,0,6,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,6,6,0,6,0,0,0,6,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
		};

		for ( int i = 0; i<39; i++ )
		{
			for ( int j = 0; j<54; j++ )
			{
				if ( mini_Map[i][j] == 0 )
					printf ( "  " );
				else if ( mini_Map[i][j] == 1 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 2 )
					printf ( "─" );
				else if ( mini_Map[i][j] == 3 )
					printf ( "│" );
				else if ( mini_Map[i][j] == 4 )
					printf ( "┬" );
				else if ( mini_Map[i][j] == 5 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 6 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 7 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 8 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
					printf ( "■" );
				}
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			}
			printf ( "\n" );
		}
	}
	else if ( BossNum == 2 ) {
		char mini_Map[39][54] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,7,7,0,0,0,7,7,0,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,0,0,0,7,7,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,5,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,9,0,12,12,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,9,7,9,9,9,9,9,9,9,0,12,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,7,0,0,0,0,9,0,0,9,0,0,12,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,5,5,6,6,6,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,7,0,0,0,9,0,0,9,7,0,0,12,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,0,7,0,0,9,0,0,9,0,7,0,0,12,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,7,0,0,0,9,9,0,0,7,0,12,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,0,12,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,6,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,7,0,0,0,0,7,9,12,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,6,6,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,7,7,7,7,0,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,9,0,0,0,0,9,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,6,0,0,6,0,0,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,0,0,9,9,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,0,0,6,6,0,0,0,6,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,6,6,0,6,0,0,0,6,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
		};

		for ( int i = 0; i<39; i++ )
		{
			for ( int j = 0; j<54; j++ )
			{
				if ( mini_Map[i][j] == 0 )
					printf ( "  " );
				else if ( mini_Map[i][j] == 1 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 2 )
					printf ( "─" );
				else if ( mini_Map[i][j] == 3 )
					printf ( "│" );
				else if ( mini_Map[i][j] == 4 )
					printf ( "┬" );
				else if ( mini_Map[i][j] == 5 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 6 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 7 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 8 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x02 );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 9 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0b );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 10 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x07 );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 11 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0d );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 12 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 13 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0c );
					printf ( "■" );
				}
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			}
			printf ( "\n" );
		}
	}
	else if ( BossNum == 3 ) {
		char mini_Map[39][54] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,0,9,0,9,0,9,9,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,9,0,9,0,9,0,0,9,9,9,9,9,9,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,5,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,8,0,0,0,0,0,0,0,8,0,0,0,9,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,8,0,8,0,0,0,0,0,8,0,8,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,5,5,6,6,6,5,5,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,8,0,7,8,0,0,0,8,7,0,8,9,9,9,9,9,9,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,0,8,0,0,0,8,0,8,0,0,0,8,0,0,0,0,9,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,8,8,8,0,0,0,8,8,8,0,0,0,9,9,9,9,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,7,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,6,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,7,0,0,0,0,0,0,0,7,0,0,9,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,6,6,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,7,7,7,7,7,7,7,0,0,9,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,9,9,9,0,0,0,9,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,6,0,0,6,0,0,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,0,0,0,9,0,0,9,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,0,0,6,6,0,0,0,6,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,0,0,9,0,0,9,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,6,6,6,0,6,0,0,0,6,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
		};

		for ( int i = 0; i<39; i++ )
		{
			for ( int j = 0; j<54; j++ )
			{
				if ( mini_Map[i][j] == 0 )
					printf ( "  " );
				else if ( mini_Map[i][j] == 1 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 2 )
					printf ( "─" );
				else if ( mini_Map[i][j] == 3 )
					printf ( "│" );
				else if ( mini_Map[i][j] == 4 )
					printf ( "┬" );
				else if ( mini_Map[i][j] == 5 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0e );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 6 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 7 )
					printf ( "■" );
				else if ( mini_Map[i][j] == 8 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0c );
					printf ( "■" );
				}
				else if ( mini_Map[i][j] == 9 ) {
					SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0d );
					printf ( "■" );
				}
				SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), 0x0f );
			}
			printf ( "\n" );
		}
	}
	showCharState ( charNum, BossNum );

more:
	menu = mini_Control ( charNum );
	if ( menu == 1 )		//공격
	{
		result = mini_Fight_Boss ( charNum, 1, BossNum );
		showCharState ( charNum, BossNum );
	}
	else if ( menu == 2 )		//스킬
	{
		result = mini_Fight_Boss ( charNum, 2, BossNum );
		showCharState ( charNum, BossNum );
	}
	else if ( menu == 3 )		//아이템
	{
		result = mini_Fight_Boss ( charNum, 3, BossNum );
		showCharState ( charNum, BossNum );
	}
	else if ( menu == 4 )	//도망
	{
		result = mini_Fight_Boss ( charNum, 4, BossNum );
		showCharState ( charNum, BossNum );
	}

	if ( result == 1 )
	{
		setCursor ( 48, 32 );
		if ( charNum == 1 ) {
			printf ( "King이 싸움에서 졌습니다." );
			Sleep ( 700 );
			goto charDie;
		}
		else if ( charNum == 2 ) {
			printf ( "Knight가 싸움에서 졌습니다." );
			Sleep ( 700 );
			goto charDie;
		}
		else if ( charNum == 3 ) {
			printf ( "Magician이 싸움에서 졌습니다." );
			Sleep ( 700 );
			goto charDie;
		}
		else if ( charNum == 4 ) {
			printf ( "Rogue가 싸움에서 졌습니다." );
			Sleep ( 700 );
			goto charDie;
		}
		Sleep ( 2500 );
		goto skip;
	}
	else if ( result == 2 )
	{
		goto skip;
	}

	if ( BossNum1.life > 0 && count == 0 )
	{
		goto more;
	}
	else if ( BossNum2.life > 0 && count == 1 )
	{
		goto more;
	}
	else if ( BossNum3.life > 0 && count == 2 )
	{
		goto more;
	}

	setCursor ( 48, 32 );
	printf ( "Boss를 해치웠습니다!" );
	setCursor ( 48, 34 );
	if ( BossNum == 1 ) {
		printf ( "경험치 %d를 획득했습니다!", BossNum1.giveEX );
		count++;
		map[37][52] = 0;
	}
	else if ( BossNum == 2 ) {
		printf ( "경험치 %d를 획득했습니다!", BossNum2.giveEX );
		count++;
		map[4][52] = 0;
	}
	else if ( BossNum == 3 ) {
		printf ( "경험치 %d를 획득했습니다!", BossNum3.giveEX );
		count++;
		map[12][13] = 0;
	}

	Sleep ( 800 );

	if ( charNum == 1 ) {
		if ( BossNum == 1 ) {
			King.currentEX += BossNum1.giveEX;

			while ( King.currentEX >= King.experiencePoint )
				charLevelUp ( charNum, King.currentEX - King.experiencePoint );
			showCharState ( charNum, BossNum );
		}
		else if ( BossNum == 2 ) {
			King.currentEX += BossNum2.giveEX;

			while ( King.currentEX >= King.experiencePoint )
				charLevelUp ( charNum, King.currentEX - King.experiencePoint );
			showCharState ( charNum, BossNum );
		}
		else if ( BossNum == 3 ) {
			King.currentEX += BossNum3.giveEX;

			while ( King.currentEX >= King.experiencePoint )
				charLevelUp ( charNum, King.currentEX - King.experiencePoint );
			showCharState ( charNum, BossNum );
		}
	}
	else if ( charNum == 2 ) {
		if ( BossNum == 1 ) {
			Knight.currentEX += BossNum1.giveEX;

			while ( Knight.currentEX >= Knight.experiencePoint )
				charLevelUp ( charNum, Knight.currentEX - Knight.experiencePoint );
			showCharState ( charNum, BossNum );
		}
		else if ( BossNum == 2 ) {
			Knight.currentEX += BossNum2.giveEX;

			while ( Knight.currentEX >= Knight.experiencePoint )
				charLevelUp ( charNum, Knight.currentEX - Knight.experiencePoint );
			showCharState ( charNum, BossNum );
		}
		else if ( BossNum == 3 ) {
			Knight.currentEX += BossNum3.giveEX;

			while ( Knight.currentEX >= Knight.experiencePoint )
				charLevelUp ( charNum, Knight.currentEX - Knight.experiencePoint );
			showCharState ( charNum, BossNum );
		}
	}
	else if ( charNum == 3 ) {
		if ( BossNum == 1 ) {
			Magician.currentEX += BossNum1.giveEX;

			while ( Magician.currentEX >= Magician.experiencePoint )
				charLevelUp ( charNum, Magician.currentEX - Magician.experiencePoint );
			showCharState ( charNum, BossNum );
		}
		else if ( BossNum == 2 ) {
			Magician.currentEX += BossNum2.giveEX;

			while ( Magician.currentEX >= Magician.experiencePoint )
				charLevelUp ( charNum, Magician.currentEX - Magician.experiencePoint );
			showCharState ( charNum, BossNum );
		}
		else if ( BossNum == 3 ) {
			Magician.currentEX += BossNum3.giveEX;

			while ( Magician.currentEX >= Magician.experiencePoint )
				charLevelUp ( charNum, Magician.currentEX - Magician.experiencePoint );
			showCharState ( charNum, BossNum );
		}
	}
	else if ( charNum == 4 ) {
		if ( BossNum == 1 ) {
			Rogue.currentEX += BossNum1.giveEX;

			while ( Rogue.currentEX >= Rogue.experiencePoint )
				charLevelUp ( charNum, Rogue.currentEX - Rogue.experiencePoint );
			showCharState ( charNum, BossNum );
		}
		else if ( BossNum == 2 ) {
			Rogue.currentEX += BossNum2.giveEX;

			while ( Rogue.currentEX >= Rogue.experiencePoint )
				charLevelUp ( charNum, Rogue.currentEX - Rogue.experiencePoint );
			showCharState ( charNum, BossNum );
		}
		else if ( BossNum == 3 ) {
			Rogue.currentEX += BossNum3.giveEX;

			while ( Rogue.currentEX >= Rogue.experiencePoint )
				charLevelUp ( charNum, Rogue.currentEX - Rogue.experiencePoint );
			showCharState ( charNum, BossNum );
		}
	}
	Sleep ( 1500 );

	if ( charNum == 1 ) {
		if ( King.life + 3 > King.MaxLife )
			King.life = King.MaxLife;
		else
			King.life += 3;
	}
	else if ( charNum == 2 ) {
		if ( Knight.life + 3 > Knight.MaxLife )
			Knight.life = Knight.MaxLife;
		else
			Knight.life += 3;
	}
	else if ( charNum == 3 ) {
		if ( Magician.life + 3 > Magician.MaxLife )
			Magician.life = Magician.MaxLife;
		else
			Magician.life += 3;
	}
	else if ( charNum == 4 ) {
		if ( Rogue.life + 3 > Rogue.MaxLife )
			Rogue.life = Rogue.MaxLife;
		else
			Rogue.life += 3;
	}

skip:
	system ( "cls" );	//화면 비우기
	main_MapViewer ( );	//메인 맵 다시 불러오기

charDie:
	if ( King.life <= 0 || Knight.life <= 0 || Magician.life <= 0 || Rogue.life <= 0 ) {
		char_POS.x = 48;
		char_POS.y = 37;
		//charInit();
		if ( charNum == 1 ) {
			King.hasItem = 0;
			King.life = King.MaxLife;
			King.experiencePoint = 0;
		}
		else if ( charNum == 2 ) {
			Knight.hasItem = 0;
			Knight.life = Knight.MaxLife;
			Knight.experiencePoint = 0;
		}
		else if ( charNum == 3 ) {
			Magician.hasItem = 0;
			Magician.life = Magician.MaxLife;
			Magician.experiencePoint = 0;
		}
		else if ( charNum == 4 ) {
			Rogue.hasItem = 0;
			Rogue.life = Rogue.MaxLife;
			Rogue.experiencePoint = 0;
		}

		system ( "cls" );
		main_MapViewer ( );
	}
}

int mini_Fight_Boss ( int charNum, int menuSelect, int BossNum )
{
	mini_stateTableClear ( );

	int damage = 0;

	if ( BossNum == 1 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			if ( charNum == 1 )		//영웅 : King 일 경우
			{
				setCursor ( 48, 28 );
				printf ( "King의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)King.damageMax - (int)King.damageMin + 1) + (int)King.damageMin;
				printf ( "대장 이상해씨에게 %d 피해를 입혔습니다.", damage );
				BossNum1.life -= damage;

				if ( BossNum1.life < 0 )
					BossNum1.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum1.life == 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( King.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 2 )
			{
				setCursor ( 48, 28 );
				printf ( "Knight의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Knight.damageMax - (int)Knight.damageMin + 1) + (int)Knight.damageMin;
				printf ( "대장 이상해씨에게 %d 피해를 입혔습니다.", damage );
				BossNum1.life -= damage;

				if ( BossNum1.life < 0 )
					BossNum1.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum1.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Knight.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 3 )
			{
				setCursor ( 48, 28 );
				printf ( "Magician의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Magician.damageMax - (int)Magician.damageMin + 1) + (int)Magician.damageMin;
				printf ( "대장 이상해씨에게 %d 피해를 입혔습니다.", damage );
				BossNum1.life -= damage;

				if ( BossNum1.life < 0 )
					BossNum1.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum1.life == 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Magician.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 4 )
			{
				setCursor ( 48, 28 );
				printf ( "Rogue의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Rogue.damageMax - (int)Rogue.damageMin + 1) + (int)Rogue.damageMin;
				printf ( "대장 이상해씨에게 %d 피해를 입혔습니다.", damage );
				BossNum1.life -= damage;

				if ( BossNum1.life < 0 )
					BossNum1.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum1.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Rogue.life>0 )
					return 0;
				else
					return 1;
			}
		}
		else if ( menuSelect == 2 )
		{
			charSkill_ForBoss ( charNum, BossNum );

			if ( BossNum1.life == 0 )
				return 0;

			mini_BossControl ( charNum, 1, BossNum );

			if ( charNum == 1 ) {
				if ( King.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 2 )
			{
				if ( Knight.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 3 )
			{
				if ( Magician.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 4 )
			{
				if ( Rogue.life>0 )
					return 0;
				else
					return 1;
			}
		}
		else if ( menuSelect == 3 )
		{
			if ( charNum == 1 )
			{
				if ( King.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "King이 아이템을 사용했습니다!" );
					King.hasItem = King.hasItem - 1;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( King.MaxLife - King.life < 20 )
						King.life = King.MaxLife;
					else
						King.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 2 )
			{
				if ( Knight.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Knight이 아이템을 사용했습니다!" );
					Knight.hasItem = Knight.hasItem - 1;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Knight.MaxLife - Knight.life < 20 )
						Knight.life = Knight.MaxLife;
					else
						Knight.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 3 )
			{
				if ( Magician.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Magician이 아이템을 사용했습니다!" );
					Magician.hasItem = Magician.hasItem - 1;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Magician.MaxLife - Magician.life < 20 )
						Magician.life = Magician.MaxLife;
					else
						Magician.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 4 )
			{
				if ( Rogue.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Rogue이 아이템을 사용했습니다!" );
					Rogue.hasItem = Rogue.hasItem - 1;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Rogue.MaxLife - Rogue.life < 20 )
						Rogue.life = Rogue.MaxLife;
					else
						Rogue.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
		}
		else if ( menuSelect == 4 )
		{
			setCursor ( 48, 28 );
			printf ( "도망!!!!!!!!!!!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			int flee[2] = { 1,0 };
			if ( flee[rand ( ) % 2] == 1 ) {
				printf ( "도망 성공!" );
				Sleep ( 1100 );
				return 2;
			}
			else {
				printf ( "도망 실패 ㅠㅠ" );
				Sleep ( 1100 );
				mini_monsterControl ( charNum, 1, monsterNum );
				return 0;
			}
		}
	}
	else if ( BossNum == 2 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			if ( charNum == 1 )		//영웅 : King 일 경우
			{
				setCursor ( 48, 28 );
				printf ( "King의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)King.damageMax - (int)King.damageMin + 1) + (int)King.damageMin;
				printf ( "대장 꼬부기에게 %d 피해를 입혔습니다.", damage );
				BossNum2.life -= damage;

				if ( BossNum2.life < 0 )
					BossNum2.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum1.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( King.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 2 )
			{
				setCursor ( 48, 28 );
				printf ( "Knight의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Knight.damageMax - (int)Knight.damageMin + 1) + (int)Knight.damageMin;
				printf ( "대장 꼬부기에게 %d 피해를 입혔습니다.", damage );
				BossNum2.life -= damage;

				if ( BossNum2.life < 0 )
					BossNum2.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum2.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Knight.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 3 )
			{
				setCursor ( 48, 28 );
				printf ( "Magician의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Magician.damageMax - (int)Magician.damageMin + 1) + (int)Magician.damageMin;
				printf ( "대장 꼬부기에게 %d 피해를 입혔습니다.", damage );
				BossNum2.life -= damage;

				if ( BossNum2.life < 0 )
					BossNum2.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum2.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Magician.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 4 )
			{
				setCursor ( 48, 28 );
				printf ( "Rogue의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Rogue.damageMax - (int)Rogue.damageMin + 1) + (int)Rogue.damageMin;
				printf ( "대장 꼬부기에게 %d 피해를 입혔습니다.", damage );
				BossNum2.life -= damage;

				if ( BossNum2.life < 0 )
					BossNum2.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum2.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Rogue.life>0 )
					return 0;
				else
					return 1;
			}
		}
		else if ( menuSelect == 2 )
		{
			charSkill_ForBoss ( charNum, BossNum );

			if ( BossNum2.life == 0 )
				return 0;

			mini_BossControl ( charNum, 1, BossNum );

			if ( charNum == 1 ) {
				if ( King.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 2 )
			{
				if ( Knight.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 3 )
			{
				if ( Magician.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 4 )
			{
				if ( Rogue.life>0 )
					return 0;
				else
					return 1;
			}
		}
		else if ( menuSelect == 3 )
		{
			if ( charNum == 1 )
			{
				if ( King.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "King이 아이템을 사용했습니다!" );
					King.hasItem--;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( King.MaxLife - King.life < 20 )
						King.life = King.MaxLife;
					else
						King.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 2 )
			{
				if ( Knight.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Knight이 아이템을 사용했습니다!" );
					Knight.hasItem--;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Knight.MaxLife - Knight.life < 20 )
						Knight.life = Knight.MaxLife;
					else
						Knight.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 3 )
			{
				if ( Magician.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Magician이 아이템을 사용했습니다!" );
					Magician.hasItem--;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Magician.MaxLife - Magician.life < 20 )
						Magician.life = Magician.MaxLife;
					else
						Magician.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 4 )
			{
				if ( Rogue.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Rogue이 아이템을 사용했습니다!" );
					Rogue.hasItem--;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Rogue.MaxLife - Rogue.life < 20 )
						Rogue.life = Rogue.MaxLife;
					else
						Rogue.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
		}
		else if ( menuSelect == 4 )
		{
			setCursor ( 48, 28 );
			printf ( "도망!!!!!!!!!!!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			int flee[2] = { 1,0 };
			if ( flee[rand ( ) % 2] == 1 ) {
				printf ( "도망 성공!" );
				Sleep ( 1100 );
				return 2;
			}
			else {
				printf ( "도망 실패 ㅠㅠ" );
				Sleep ( 1100 );
				mini_BossControl ( charNum, 1, BossNum );
				return 0;
			}
		}
	}
	else if ( BossNum == 3 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			if ( charNum == 1 )		//영웅 : King 일 경우
			{
				setCursor ( 48, 28 );
				printf ( "King의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)King.damageMax - (int)King.damageMin + 1) + (int)King.damageMin;
				printf ( "대장 고스트에게 %d 피해를 입혔습니다.", damage );
				BossNum3.life -= damage;

				if ( BossNum3.life < 0 )
					BossNum3.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum3.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( King.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 2 )
			{
				setCursor ( 48, 28 );
				printf ( "Knight의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Knight.damageMax - (int)Knight.damageMin + 1) + (int)Knight.damageMin;
				printf ( "대장 고스트에게 %d 피해를 입혔습니다.", damage );
				BossNum3.life -= damage;

				if ( BossNum3.life < 0 )
					BossNum3.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum3.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Knight.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 3 )
			{
				setCursor ( 48, 28 );
				printf ( "Magician의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Magician.damageMax - (int)Magician.damageMin + 1) + (int)Magician.damageMin;
				printf ( "대장 고스트에게 %d 피해를 입혔습니다.", damage );
				BossNum3.life -= damage;

				if ( BossNum3.life < 0 )
					BossNum3.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum3.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Magician.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 4 )
			{
				setCursor ( 48, 28 );
				printf ( "Rogue의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)Rogue.damageMax - (int)Rogue.damageMin + 1) + (int)Rogue.damageMin;
				printf ( "대장 고스트에게 %d 피해를 입혔습니다.", damage );
				BossNum3.life -= damage;

				if ( BossNum3.life < 0 )
					BossNum3.life = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1300 );

				if ( BossNum3.life <= 0 )
					return 0;

				mini_BossControl ( charNum, 1, BossNum );

				if ( Rogue.life>0 )
					return 0;
				else
					return 1;
			}
		}
		else if ( menuSelect == 2 )
		{
			charSkill_ForBoss ( charNum, BossNum );

			if ( BossNum3.life == 0 )
				return 0;

			mini_BossControl ( charNum, 1, BossNum );

			if ( charNum == 1 ) {
				if ( King.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 2 )
			{
				if ( Knight.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 3 )
			{
				if ( Magician.life>0 )
					return 0;
				else
					return 1;
			}
			else if ( charNum == 4 )
			{
				if ( Rogue.life>0 )
					return 0;
				else
					return 1;
			}
		}
		else if ( menuSelect == 3 )
		{
			if ( charNum == 1 )
			{
				if ( King.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "King이 아이템을 사용했습니다!" );
					King.hasItem--;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( King.MaxLife - King.life < 20 )
						King.life = King.MaxLife;
					else
						King.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 2 )
			{
				if ( Knight.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Knight이 아이템을 사용했습니다!" );
					Knight.hasItem--;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Knight.MaxLife - Knight.life < 20 )
						Knight.life = Knight.MaxLife;
					else
						Knight.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 3 )
			{
				if ( Magician.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Magician이 아이템을 사용했습니다!" );
					Magician.hasItem--;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Magician.MaxLife - Magician.life < 20 )
						Magician.life = Magician.MaxLife;
					else
						Magician.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
			else if ( charNum == 4 )
			{
				if ( Rogue.hasItem > 0 ) {
					setCursor ( 48, 28 );
					printf ( "Rogue이 아이템을 사용했습니다!" );
					Rogue.hasItem--;
					Sleep ( 600 );
					setCursor ( 48, 30 );
					printf ( "체력을 20 회복했습니다" );
					if ( Rogue.MaxLife - Rogue.life < 20 )
						Rogue.life = Rogue.MaxLife;
					else
						Rogue.life += 20;
					Sleep ( 1100 );
				}
				else {
					setCursor ( 48, 28 );
					printf ( "현재 아이템이 없습니다" );
					Sleep ( 1100 );
				}
			}
		}
		else if ( menuSelect == 4 )
		{
			setCursor ( 48, 28 );
			printf ( "도망!!!!!!!!!!!" );
			Sleep ( 600 );
			setCursor ( 48, 30 );
			int flee[2] = { 1,0 };
			if ( flee[rand ( ) % 2] == 1 ) {
				printf ( "도망 성공!" );
				Sleep ( 1100 );
				return 2;
			}
			else {
				printf ( "도망 실패 ㅠㅠ" );
				Sleep ( 1100 );
				mini_BossControl ( charNum, 1, BossNum );
				return 0;
			}
		}
	}
}

void mini_BossControl ( int charNum, int menuSelect, int BossNum )
{
	mini_stateTableClear ( );
	int damage;

	if ( BossNum == 1 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			if ( charNum == 1 )		//영웅 : King 일 경우
			{
				setCursor ( 48, 28 );
				printf ( "대장 이상해씨의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum1.damageMax - (int)BossNum1.damageMin + 1) + (int)BossNum1.damageMin;
				printf ( "King이 %d 피해를 받았습니다.", damage );
				King.life -= damage;

				if ( King.life < 0 )
					King.life = 0;
				if ( King.currentEX < 0 )
					King.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
			else if ( charNum == 2 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 이상해씨의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum1.damageMax - (int)BossNum1.damageMin + 1) + (int)BossNum1.damageMin;
				printf ( "King이 %d 피해를 받았습니다.", damage );
				Knight.life -= damage;

				if ( Knight.life < 0 )
					Knight.life = 0;
				if ( Knight.currentEX < 0 )
					Knight.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
			else if ( charNum == 3 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 이상해씨의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum1.damageMax - (int)BossNum1.damageMin + 1) + (int)BossNum1.damageMin;
				printf ( "Magician이 %d 피해를 받았습니다.", damage );
				Magician.life -= damage;

				if ( Magician.life < 0 )
					Magician.life = 0;
				if ( Magician.currentEX < 0 )
					Magician.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
			else if ( charNum == 4 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 이상해씨의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum1.damageMax - (int)BossNum1.damageMin + 1) + (int)BossNum1.damageMin;
				printf ( "Rogue이 %d 피해를 받았습니다.", damage );
				Rogue.life -= damage;

				if ( Rogue.life < 0 )
					Rogue.life = 0;
				if ( Rogue.currentEX < 0 )
					Rogue.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
		}
		else if ( menuSelect == 4 )
		{
			//empty
		}
	}
	else if ( BossNum == 2 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			if ( charNum == 1 )		//영웅 : King 일 경우
			{
				setCursor ( 48, 28 );
				printf ( "대장 꼬부기의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum2.damageMax - (int)BossNum2.damageMin + 1) + (int)BossNum2.damageMin;
				printf ( "King이 %d 피해를 받았습니다.", damage );
				King.life -= damage;

				if ( King.life < 0 )
					King.life = 0;
				if ( King.currentEX < 0 )
					King.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
				//			char_POS_mini.y = 28;
			}
			else if ( charNum == 2 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 꼬부기의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum2.damageMax - (int)BossNum2.damageMin + 1) + (int)BossNum2.damageMin;
				printf ( "King이 %d 피해를 받았습니다.", damage );
				Knight.life -= damage;

				if ( Knight.life < 0 )
					Knight.life = 0;
				if ( Knight.currentEX < 0 )
					Knight.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
			else if ( charNum == 3 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 꼬부기의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum2.damageMax - (int)BossNum2.damageMin + 1) + (int)BossNum2.damageMin;
				printf ( "Magician이 %d 피해를 받았습니다.", damage );
				Magician.life -= damage;

				if ( Magician.life < 0 )
					Magician.life = 0;
				if ( Magician.currentEX < 0 )
					Magician.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
			else if ( charNum == 4 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 꼬부기의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum2.damageMax - (int)BossNum2.damageMin + 1) + (int)BossNum2.damageMin;
				printf ( "Rogue이 %d 피해를 받았습니다.", damage );
				Rogue.life -= damage;

				if ( Rogue.life < 0 )
					Rogue.life = 0;
				if ( Rogue.currentEX < 0 )
					Rogue.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
		}
		else if ( menuSelect == 4 )
		{
			//empty
		}
	}
	else if ( BossNum == 3 )
	{
		if ( menuSelect == 1 )		//공격 선택시
		{
			if ( charNum == 1 )		//영웅 : King 일 경우
			{
				setCursor ( 48, 28 );
				printf ( "대장 고스트의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum3.damageMax - (int)BossNum3.damageMin + 1) + (int)BossNum3.damageMin;
				printf ( "King이 %d 피해를 받았습니다.", damage );
				King.life -= damage;

				if ( King.life < 0 )
					King.life = 0;
				if ( King.currentEX < 0 )
					King.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
			else if ( charNum == 2 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 고스트의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum3.damageMax - (int)BossNum3.damageMin + 1) + (int)BossNum3.damageMin;
				printf ( "King이 %d 피해를 받았습니다.", damage );
				Knight.life -= damage;

				if ( Knight.life < 0 )
					Knight.life = 0;
				if ( Knight.currentEX < 0 )
					Knight.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
			else if ( charNum == 3 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 고스트의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum3.damageMax - (int)BossNum3.damageMin + 1) + (int)BossNum3.damageMin;
				printf ( "Magician이 %d 피해를 받았습니다.", damage );
				Magician.life -= damage;

				if ( Magician.life < 0 )
					Magician.life = 0;
				if ( Magician.currentEX < 0 )
					Magician.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
			else if ( charNum == 4 )
			{
				setCursor ( 48, 28 );
				printf ( "대장 고스트의 공격!" );
				Sleep ( 600 );
				setCursor ( 48, 30 );
				damage = rand ( ) % ((int)BossNum3.damageMax - (int)BossNum3.damageMin + 1) + (int)BossNum3.damageMin;
				printf ( "Rogue이 %d 피해를 받았습니다.", damage );
				Rogue.life -= damage;

				if ( Rogue.life < 0 )
					Rogue.life = 0;
				if ( Rogue.currentEX < 0 )
					Rogue.currentEX = 0;

				showCharState ( charNum, BossNum );
				Sleep ( 1100 );
			}
		}
		else if ( menuSelect == 4 )
		{
			//empty
		}
	}
}



void charSkill_ForBoss ( int charNum, int BossNum )
{
	int damage;

	if ( BossNum == 1 )
	{
		if ( charNum == 1 ) {
			setCursor ( 48, 28 );
			printf ( "King의 스킬 'Taking' 발동!!" );
			damage = rand ( ) % ((int)King.damageMax - (int)King.damageMin + 1) * 2 + (int)King.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 이상해씨에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum1.life -= damage;

			if ( BossNum1.life < 0 )
				BossNum1.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 2 ) {
			setCursor ( 48, 28 );
			printf ( "Knight의 스킬 'Demasia' 발동!!" );
			damage = rand ( ) % ((int)Knight.damageMax - (int)Knight.damageMin + 1) * 2 + (int)Knight.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 이상해씨에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum1.life -= damage;

			if ( BossNum1.life < 0 )
				BossNum1.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 3 ) {
			setCursor ( 48, 28 );
			printf ( "Magician의 스킬 'Piber!' 발동!!" );
			damage = rand ( ) % ((int)Magician.damageMax - (int)Magician.damageMin + 1) * 2 + (int)Magician.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 이상해씨에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum1.life -= damage;

			if ( BossNum1.life < 0 )
				BossNum1.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 4 ) {
			setCursor ( 48, 28 );
			printf ( "Rogue의 스킬 'Death Lotus' 발동!!" );
			damage = rand ( ) % ((int)Rogue.damageMax - (int)Rogue.damageMin + 1) * 2 + (int)Rogue.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 이상해씨에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum1.life -= damage;

			if ( BossNum1.life < 0 )
				BossNum1.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
	}
	else if ( BossNum == 2 )
	{
		if ( charNum == 1 ) {
			setCursor ( 48, 28 );
			printf ( "King의 스킬 'Taking' 발동!!" );
			damage = rand ( ) % ((int)King.damageMax - (int)King.damageMin + 1) * 2 + (int)King.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 꼬부기에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum2.life -= damage;

			if ( BossNum2.life < 0 )
				BossNum2.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 2 ) {
			setCursor ( 48, 28 );
			printf ( "Knight의 스킬 'Demasia' 발동!!" );
			damage = rand ( ) % ((int)Knight.damageMax - (int)Knight.damageMin + 1) * 2 + (int)Knight.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 꼬부기에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum2.life -= damage;

			if ( BossNum2.life < 0 )
				BossNum2.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 3 ) {
			setCursor ( 48, 28 );
			printf ( "Magician의 스킬 'Piber!' 발동!!" );
			damage = rand ( ) % ((int)Magician.damageMax - (int)Magician.damageMin + 1) * 2 + (int)Magician.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 꼬부기에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum2.life -= damage;

			if ( BossNum2.life < 0 )
				BossNum2.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 4 ) {
			setCursor ( 48, 28 );
			printf ( "Rogue의 스킬 'Death Lotus' 발동!!" );
			damage = rand ( ) % ((int)Rogue.damageMax - (int)Rogue.damageMin + 1) * 2 + (int)Rogue.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 꼬부기에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum2.life -= damage;

			if ( BossNum2.life < 0 )
				BossNum2.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
	}
	else if ( BossNum == 3 )
	{
		if ( charNum == 1 ) {
			setCursor ( 48, 28 );
			printf ( "King의 스킬 'Taking' 발동!!" );
			damage = rand ( ) % ((int)King.damageMax - (int)King.damageMin + 1) * 2 + (int)King.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 고스트에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum3.life -= damage;

			if ( BossNum3.life < 0 )
				BossNum3.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 2 ) {
			setCursor ( 48, 28 );
			printf ( "Knight의 스킬 'Demasia' 발동!!" );
			damage = rand ( ) % ((int)Knight.damageMax - (int)Knight.damageMin + 1) * 2 + (int)Knight.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 고스트에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum3.life -= damage;

			if ( BossNum3.life < 0 )
				BossNum3.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 3 ) {
			setCursor ( 48, 28 );
			printf ( "Magician의 스킬 'Piber!' 발동!!" );
			damage = rand ( ) % ((int)Magician.damageMax - (int)Magician.damageMin + 1) * 2 + (int)Magician.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 고스트에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum3.life -= damage;

			if ( BossNum3.life < 0 )
				BossNum3.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
		else if ( charNum == 4 ) {
			setCursor ( 48, 28 );
			printf ( "Rogue의 스킬 'Death Lotus' 발동!!" );
			damage = rand ( ) % ((int)Rogue.damageMax - (int)Rogue.damageMin + 1) * 2 + (int)Rogue.damageMin * 2;
			Sleep ( 600 );

			setCursor ( 48, 30 );
			printf ( "대장 고스트에게 %d만큼의 피해를 입혔습니다.", damage );
			BossNum3.life -= damage;

			if ( BossNum3.life < 0 )
				BossNum3.life = 0;

			showCharState ( charNum, BossNum );
			Sleep ( 1300 );
		}
	}
}

void charSkill ( int charNum, int monsterNum )			//영웅 스킬 함수
{
	int damage;

	if ( charNum == 1 ) {
		setCursor ( 48, 28 );
		printf ( "King의 스킬 'Taking' 발동!!" );
		damage = rand ( ) % ((int)King.damageMax - (int)King.damageMin + 1) * 2 + (int)King.damageMin * 2;
		Sleep ( 600 );

		setCursor ( 48, 30 );
		printf ( "%s에게 %d만큼의 피해를 입혔습니다.", MonsterNum[monsterNum].name, damage );
		MonsterNum[monsterNum].life -= damage;

		if ( MonsterNum[monsterNum].life < 0 )
			MonsterNum[monsterNum].life = 0;

		showCharState ( charNum, 0 );
		Sleep ( 1300 );
	}
	else if ( charNum == 2 ) {
		setCursor ( 48, 28 );
		printf ( "Knight의 스킬 'Demasia' 발동!!" );
		damage = rand ( ) % ((int)Knight.damageMax - (int)Knight.damageMin + 1) * 2 + (int)Knight.damageMin * 2;
		Sleep ( 600 );

		setCursor ( 48, 30 );
		printf ( "%s에게 %d만큼의 피해를 입혔습니다.", MonsterNum[monsterNum].name, damage );
		MonsterNum[monsterNum].life -= damage;

		if ( MonsterNum[monsterNum].life < 0 )
			MonsterNum[monsterNum].life = 0;

		showCharState ( charNum, 0 );
		Sleep ( 1300 );
	}
	else if ( charNum == 3 ) {
		setCursor ( 48, 28 );
		printf ( "Magician의 스킬 'Piber!' 발동!!" );
		damage = rand ( ) % ((int)Magician.damageMax - (int)Magician.damageMin + 1) * 2 + (int)Magician.damageMin * 2;
		Sleep ( 600 );

		setCursor ( 48, 30 );
		printf ( "%s에게 %d만큼의 피해를 입혔습니다.", MonsterNum[monsterNum].name, damage );
		MonsterNum[monsterNum].life -= damage;

		if ( MonsterNum[monsterNum].life < 0 )
			MonsterNum[monsterNum].life = 0;

		showCharState ( charNum, 0 );
		Sleep ( 1300 );
	}
	else if ( charNum == 4 ) {
		setCursor ( 48, 28 );
		printf ( "Rogue의 스킬 'Death Lotus' 발동!!" );
		damage = rand ( ) % ((int)Rogue.damageMax - (int)Rogue.damageMin + 1) * 2 + (int)Rogue.damageMin * 2;
		Sleep ( 600 );

		setCursor ( 48, 30 );
		printf ( "%s에게 %d만큼의 피해를 입혔습니다.", MonsterNum[monsterNum].name, damage );
		MonsterNum[monsterNum].life -= damage;

		if ( MonsterNum[monsterNum].life < 0 )
			MonsterNum[monsterNum].life = 0;

		showCharState ( charNum, 0 );
		Sleep ( 1300 );
	}
}



void gameClear ( )
{
	setCursor ( 40, 10 );
	printf ( "게임 클리어!!!" );
	setCursor ( 40, 12 );
	printf ( "게임을 종료하시려면 아무 키나 누르십시오" );
}