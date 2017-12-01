// FiveStoneProcess.h: interface for the FiveStoneProcess class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#define	 X_MAX_GRID			20
#define	 Y_MAX_GRID			20




enum COLOR_STONE { DARKSTONE=1,WHITESTONE=2 };

class CGoBangGame  
{
public:
	void merge1(POINT target[],int &tn,POINT append[],int an,int roundcolor);
	BOOL is_maketwolevelalive(int x, int y, int color);
	BOOL has_twolevel_qianmodel_defent_low(int color,int &counter,POINT tempresult[]);
	void getclose_twolevelqianmodel(int color, int &counter, POINT tempresult[]);
	void cut_dangerpoints(int color,POINT tempresult[],int &n);
	BOOL has_chongtwoandmakeqianmodel(int color,int &counter,POINT tempresult[]);
	BOOL is_makealive2(int x, int y, int color);
	void selectchongtwo(int color, int &x, int &y, POINT tempresult[],int n);
	BOOL is_makealiveandtwolevel(int color,int x,int y);
	void setthinkpos(int pos);
	int getthinkpos();
	void getclose_twolevel(int color, int &counter, POINT tempresult[]);
	void getclose_twolevel_four(int color, int &counter, POINT tempresult[]);	
	void getclose_twolevelqianmodel_qiang(int color, int &counter, POINT tempresult[]);	
	void getclose_qianmodeladdone(int color, int &counter, POINT tempresult[],BOOL qiang = false);
	void getclose_qianmodel(int color,int &counter,POINT tempresult[],BOOL qiang = false);
	BOOL has_chongtwo(int color, int &counter, POINT tempresult[]);
	BOOL is_chongtwo(int color,int x,int y);
	BOOL has_beatpoints_low(int color);
	BOOL has_twolevel_beat_low(int color, int &counter, POINT tempresult[]);
	BOOL has_beatpoints_low(int color, int &counter, POINT tempresult[]);
	BOOL has_defent_point(int color,int &counter,POINT tempresult[]);
	BOOL is_defent_point(int color,int x,int y);
	void design_depart(int x, int y, int color);
	BOOL make_beatpoints(int color, int &counter, POINT tempresult[]);
	BOOL is_beatpoint(int color,int x,int y);
	int countpoints();
	BOOL has_twolevelqianmodel_qiang_defent(int color,int &counter,POINT tempresult[]);
	BOOL has_twolevelqianmodel_qiang_defent_low(int color,int &counter,POINT tempresult[]);
	BOOL has_threelevelqianmodel_qiang_defent(int color,int &counter,POINT tempresult[]);
	BOOL has_threelevelqianmodel_qiang_defent_low(int color,int &counter,POINT tempresult[]);
	BOOL has_qiang_beat(int color,int &counter,POINT tempresult[]);
	BOOL is_qianmodel(int color,int x,int y);
	BOOL is_maketwoalive(int x,int y,int color);
	BOOL has_threelevelqianmodel_qiang(int color);
	BOOL has_threelevelqianmodel_qiang(int color,int &counter,POINT tempresult[]);
	BOOL has_twolevelqianmodel_qiang(int color);
	BOOL has_twolevelqianmodel_qiang(int color,int &counter,POINT tempresult[]);
	BOOL isempty();
	BOOL has_twolevel_beat(int color, int &counter, POINT tempresult[]);
	BOOL make_twoalive(int color,int &counter,POINT tempresult[]);
	BOOL has_alive_defent(int color);
	BOOL has_alive_defent_low(int color);
	BOOL has_canfour(int color);
	BOOL has_twolevel_four_defent(int color);
	BOOL has_twolevel_four_defent_low(int color);
	BOOL has_twolevel_defent(int color);
	BOOL has_twolevel_defent_low(int color);
	void is_danger(int color);
	BOOL has_beatpoints(int color,int &counter,POINT tempresult[]);
	BOOL has_beatpoints(int color);
	void setGrade(int grade);
	BOOL is_qian(int color,int x,int y);
	BOOL is_qiannofour(int color, int x, int y);
	BOOL can_maketwolevel_alive(int color, int x, int y);
	BOOL can_makeqianmodel(int color,int x,int y);
	BOOL can_makeqianmodel3(int color, int x, int y);
	BOOL can_maketwolevelqianmodel(int color,int x,int y);
	BOOL can_makethreelevelqianmodel(int color, int x, int y);
	int level;//用来控制递归深度，主要用于has_canfour方法中
	BOOL has_twoqianmodel(int color,int &counter,POINT tempresult[]);
	BOOL is_twoqianmodel(int color,int x, int y,POINT tempresult[],int n);
	BOOL has_tie();
	void selectqianmodel(int color,int &x,int &y,POINT tempresult[],int n);
	void selectqianmodel3(int color,int &x,int &y,POINT tempresult[],int n);
	void selectqianmodel5(int color,int &x,int &y,POINT tempresult[],int n,BOOL defent = false);
	void selectless_twomodel(int color,int &x,int &y,POINT tempresult[],int n);
	void getclose_subqian(int color,int &counter,POINT tempresult[]);
	void getclose_small_subqian(int color,int &counter, POINT tempresult[]);
	void sub(POINT target[],int &tn,POINT subpoints[],int sn);
	CString passtime(int &cur_time);
	BOOL defent_success;
	BOOL is_makealive(int x,int y,int color);
	BOOL has_twolevel_qianmodel(int color, int &counter,POINT tempresult[]);
	BOOL has_twolevel_qianmodel(int color);
	BOOL has_threelevel_qianmodel(int color, int &counter,POINT tempresult[]);
	BOOL has_threelevel_qianmodel(int color);
	BOOL has_twolevel_qianmodel_defent(int color, int &counter,POINT tempresult[]);
	BOOL twolevel_four(int x,int y,int color);
	BOOL make_qianmodel(int color,int &counter,POINT tempresult[]);
	BOOL make_twolevel_qianmodel(int color, int &counter, POINT tempresult[]);
	BOOL make_threelevel_qianmodel(int color, int &counter, POINT tempresult[]);
	void common_info(CString s,BOOL debug,int &cur_time);
	BOOL has_qianmodel_addone(int color,int &counter,POINT tempresult[],BOOL qiang = false);
	BOOL has_qianmodel_addone(int color,BOOL qiang = false);
	BOOL has_qianmodel_addone_defent(int color, int &counter, POINT tempresult[],BOOL qiang = false);
	BOOL has_qianmodel_addone_defent_low(int color, int &counter, POINT tempresult[],BOOL qiang = false);
	BOOL can_qianmodel(int color,int x,int y,POINT tempresult[],int n,BOOL qiang = false);
	BOOL can_qianmodel(int color, int x, int y,POINT tempresult[],int n,int &counter,POINT modelresult[],BOOL qiang = false);
	int getcolor(int color,int x,int y,int i,int move,POINT tempresult[],int n);
	BOOL has_qianmodel(int color,int &counter,POINT tempresult[],BOOL qiang = false);
	BOOL has_qianmodel(int color,BOOL qiang = false);
	BOOL has_qianmodel_defent(int color,int &counter,POINT tempresult[],BOOL qiang = false);
	BOOL has_qianmodel_defent_low(int color,int &counter,POINT tempresult[],BOOL qiang = false);
	BOOL is_five(int color,int x,int y);
	void ShowInfo(CString s,BOOL debug = false);
	CEdit *m_outinfo;
	BOOL is_canfour(int color,int x,int y);
	BOOL has_twolevel_defent(int color,int &counter,POINT tempresult[]);
	BOOL has_twolevel_defent_low(int color,int &counter,POINT tempresult[]);
	BOOL has_twolevel_four_defent(int color,int &counter,POINT tempresult[]);
	BOOL has_twolevel_four_defent_low(int color,int &counter,POINT tempresult[]);
	BOOL has_alive_defent(int color,int &counter,POINT tempresult[]);
	BOOL has_alive_defent_low(int color,int &counter,POINT tempresult[]);
	BOOL has_roundcolor(int x,int y,int color);
	void merge(POINT target[],int &tn,POINT append[],int an);
	void getclose(int color,int &counter,POINT tempresult[]);
	void getclose_small(int &counter,POINT tempresult[]);
	BOOL has_qian(int color,int &counter,POINT tempresult[]);
	BOOL has_qian_nocanfour(int color,int &counter,POINT tempresult[]);
	BOOL has_canfour(int color,int &counter,POINT tempresult[]);
	BOOL has_canfive(int color,int &counter,POINT tempresult[]);
	BOOL has_canfour_old(int color, int &counter, POINT tempresult[]);
	void clearvalue();
	void design(int x,int y,int color);
	void design_old(int x, int y, int color);
	void selectmax(int color,int &x,int &y,POINT tempresult[],int n,BOOL defent = false,int m = 2);
	int getmax(int &counter,POINT tempresult[]);
	int getcolor(int x,int i,int move,int y);

	//int value[16][16];//用于赋值算法保存所赋值

	void design(int color);
	BOOL make_alive(int color,int &counter,POINT tempresult[]);
	BOOL make_twolevel(int color,int &counter,POINT tempresult[]);
	void clearhint();
	void clear();
	void getclosest(int color,int n,POINT closepoints[]);
	int random(int x,int y);
	BOOL has_twolevel(int color,int &counter,POINT tempresult[]);	
	BOOL has_twolevel(int color);
	BOOL has_twolevel_pure(int color,int &counter,POINT tempresult[]);	
	BOOL has_twolevel_pure(int color);
	BOOL has_twolevel_four(int color,int &counter,POINT tempresult[]);
	BOOL has_twolevel_four(int color);
	void getroundpoints(int x,int y,int color,POINT roundpoints[]);
	BOOL twolevel_alive(int x,int y,int color);
	BOOL twolevel_alive_pure(int x, int y, int color);
	BOOL twolevel_alive_old(int x,int y,int color);

	void GetNextStepPoint( int &x,int &y,int color ,BOOL debug );
	void getpoint( int &x,int &y,int color ,BOOL debug );

	void getpoint1(int &x,int &y,int color,BOOL debug);
	void getpoint2(int &x,int &y,int color,BOOL debug);
	void getpoint3(int &x,int &y,int color,BOOL debug);
	void getpoint4(int &x,int &y,int color,BOOL debug);
	void getpoint5(int &x,int &y,int color,BOOL debug);


	BOOL has_alive(int color,int &counter,POINT tempresult[]);
	BOOL has_alive(int color);
	BOOL can_five(int color);
	BOOL can_five(int color,int &x,int &y);
	BOOL is_alive(int x,int y,int color);
	BOOL is_alive_old(int x, int y,int color);

	//int  points[16][16];//所有的子，1代表黑色棋，2代表白色棋，0代表没有子
	
	int		m_ptvalue[X_MAX_GRID][Y_MAX_GRID];//用于赋值算法保存所赋值
	int		ChessPoints[X_MAX_GRID][Y_MAX_GRID];	//所有的子，1代表黑色棋，2代表白色棋，0代表没有子
	void	SetGridPointCount( int xGrid,int yGrid );


	BOOL has_five(int color);
	CGoBangGame();
	virtual ~CGoBangGame();


protected:
	int m_xGrid;
	int m_yGrid;


private:
	int thinkpos;//指明电脑思考的位置，最小值为0，最大值为100
	BOOL timeout;
	BOOL is_low;
	BOOL notshowinfo;
	BOOL isdanger;//用于判断当前是否危险
	int m_grade;
};























