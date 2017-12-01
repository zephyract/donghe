//define.h 
#ifndef define_h_
#define define_h_

#define BLACKCHESS 1//�ڷ�
#define REDCHESS   2//�췽

//��ȷ�ʽDepthSort
#define DS_DEFAULTSET 1
#define DS_USERDEFINE 2

#define CS_PCCHESS   1//�˻�����
#define CS_PPCHESS	 2//���˶���
#define CS_CCCHESS   3//��������
#define CS_HASHCHESS 4//���Ӷ���

//--------����--------
#define NOCHESS    0 //û������

#define B_KING	   8 //��˧
#define B_CAR	   9 //�ڳ�
#define B_HORSE	   10 //����
#define B_CANON	   11 //����
#define B_BISHOP   12 //��ʿ
#define B_ELEPHANT 13 //����
#define B_PAWN     14 //����
#define B_BEGIN    B_KING
#define B_END      B_PAWN

#define R_KING	   1 //�콫
#define R_CAR      2 //�쳵
#define R_HORSE    3//����
#define R_CANON    4//����
#define R_BISHOP   5//��ʿ
#define R_ELEPHANT 6//����
#define R_PAWN     7//���
#define R_BEGIN    R_KING
#define R_END      R_PAWN
//--------------------

#define IsBlack(x) (x>=B_BEGIN && x<=B_END)//�ж�ĳ�������ǲ��Ǻ�ɫ
#define IsRed(x)   (x>=R_BEGIN && x<=R_END)//�ж�ĳ�������ǲ��Ǻ�ɫ

//�ж����������ǲ���ͬɫ
#define IsSameSide(x,y) ((IsBlack(x) && IsBlack(y)) || (IsRed(x) && IsRed(y)))

//����λ��
typedef struct
{
	BYTE x;
	BYTE y;
}CHESSMANPOS;

//�����߷�
typedef struct
{
	short nChessID;  //������ʲô����
	CHESSMANPOS From;//��ʼλ��
	CHESSMANPOS To;  //�ߵ�ʲôλ��
	int Score;       //�߷��ķ���
}CHESSMOVE;

//����ʱ��Ҫ�����ݽṹ
typedef struct
{
	CHESSMOVE cmChessMove;
	short nChessID;//���Ե�������
}UNDOMOVE;

#endif
