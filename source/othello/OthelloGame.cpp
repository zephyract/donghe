
/***************************************************************************
       你好!
       很高兴你对这些代码有兴趣,你可以随意复制和使用些代码,唯一有一点小小的
   愿望:如果这些代码对你有用的话,请发一封email告诉我。
	   如果你有什么好的意见也请Mail给我。
       wangfei@hanwang.com.cn
	   或
	   wangfei@engineer.com.cn
                                                                 2001年2月
****************************************************************************/

/*
设计思路:
   从棋盘的当前状态构造博奕树，到达终局状态计算状态分，并回归到当前状态，以求出最
有利的一步(部分回溯值最大的一个子结点)。

 术语:  
     棋手   计算机方
	 对手   计算机的对手
   
 要点:

    一.终局状态
	   1.棋局已结束 
	   2.当前颜色已无处可下子 
	   3.博奕树的深度已到指定的深度

    二.博奕树的构造方法
	   采用深度优先的方法节省构造过程中的内存使用,构造过程使用堆栈空间存储子节点，
	   已完成构造的分枝可抛弃，以达到节省内存(使用递归程序)。

      算法简单描述如下：
	    
       如果当前棋局为终局状态，则返回状态分
	   从当前棋局的状态出发,找出一个可走的步数,试走此部,新状态扩展为当前棋局的
	   一个子结点
	   此子结点做为新的当前状态递归调用  (此过程中可加入α-β裁减)
       
        思考：如果采用循环代替递归，保存所有已构造的节点，在下一次构造博奕树时本次构造
		      的节点可能重复利用，节省构造时间。

    三.构造过程的α-β裁减
	   1.α裁减
	      在考虑轮到棋手下棋的一个亲节点及轮到对手下棋的一个子节点时，
		  如果该子节点的数值已经小于或等于其亲节点的回溯值，
		  那么就不需要对该节点或者其后续节点做更多的处理了。
		  计算的过程可以直接返回到亲节点上。

	   2.β裁减
	      在考虑轮到对手下棋的一个亲节点及轮到棋手下棋的一个子节点时，
		  如果该子节点的部分回溯值已经大于或等于其亲节点的部分回溯值，
		  那么就不需要对该子节点或者其后裔节点做更多的处理了。
		  计算过程可以直接返回到亲节点上。

    四.棋局的状态估值函数
	   此函数量化的方法描述棋局某一状态下某一方棋子的形式,对棋局形式的正确分析直接关系到
	   计算机棋力的高低
	   考虑黑白棋的规则,棋局结速束时,棋盘上哪一方的棋子多哪一方则获胜,简单的状态估值方法是
	   对某一状态下棋子的个数做为状态分,但棋盘上很多棋子都可能被对方吃掉,所以比较好的方法是
	   计算棋盘上所有不可能被对方吃掉的棋子做为状态分.

	   棋盘上任一个棋子是否能被对方吃掉由它四个方向其它棋格的状态决定,
	   四个方向为:
		   左上到右下的斜线(BD_AX)
		   右上到左下的斜线(FD_AX)
		   水平方向(H_AX)
		   垂直方向(V_AX)

       一个棋子如果在这四个方向上都受保护,我们则认为此棋子不可能被对方吃掉,
	   一个棋子是否在某一个方向上受保护,可由下面的方法判断：

          当出现下面的两种情况之一时，我们可以认为此棋子在此方向上受保护
          1.此方向上与此棋子相联的已方棋子已到达边界 
		  2.此方向上与此棋子相联的已方棋子两边没有空格子(对方棋子，或边界)
          
		 由此知,棋盘空时,四个角上的棋子一定是不可被吃的棋子
         状态分的计分方法:
	     1.不可被吃掉的棋子计四分
		 2.三个方向受保护的棋子计三分
		 3.两个方向上受保护的棋子计两分
		 4.一个方向上受保护的棋子计一分

         另由于角的重要性下面的情况出现时被视为危险状态,将会被减分
          角边上有已方棋子,但在到达角的方向上没有受到保护,则减分 
   
数据结构: 

每一个棋格用一个字节记,低两位记录棋子颜色,高四位记录此子在四个方向上是否受保护的情况
如果一个棋格为空,则对应字节为0x0,为避免边界检查，在棋盘四周加一边框，边框对应值为0xFF
*/

#include "stdafx.h"
#include "OthelloGame.h"

UINT8   computer_side = CHESS_BLACK;
UINT8   max_depth = 4;

UINT8   cur_depth = 0;


UINT8	 cur_step =0;
UINT16   step_array[128];
UINT8	 Step_Color[128];



//UINT8   g_iGameLevel = GMLEVEL_MID; //游戏难度等级
UINT8   g_iGameLevel = GMLEVEL_EASY;


const  UINT8 depth1[]={6, 7, 8,9,10};
const  UINT8 depth2[]={5, 6, 7,8,9 };

/*找出所有在水平方向受保护的obcolor方的棋子，并累计分数*/
INT16 scan_horiz_aixes(board_type *board_ptr, UINT8 obcolor)
{
   /*扫描8个水平方向*/
	INT16 score=0;
	UINT8 *cur_ptr, *stop_ptr;
	UINT8  piece[4][2];
	UINT8 count=0, tmpscore;
	UINT8 bFull;
	for(UINT8 row=1; row<9; row++)
	{
        tmpscore = (row == 1 || row == 8) ? 10:2;
		cur_ptr = &board_ptr->board[row][1];
		stop_ptr= &board_ptr->board[row][9];
		bFull = TRUE;
		count=0;
		while(cur_ptr < stop_ptr)
		{
		   if(*cur_ptr == obcolor)
		   {
			 piece[count][0]  = cur_ptr - &board_ptr->board[row][0];
			 while(*cur_ptr == obcolor)
				 cur_ptr ++;
			 piece[count++][1] = cur_ptr - &board_ptr->board[row][0];
		   }
		   if(!*cur_ptr)
			   bFull = FALSE; 
		   cur_ptr++;
		}
		while(count--)
		{
			UINT8 nums = (piece[count][1]-piece[count][0]);
			if(bFull || piece[count][0]==1 || piece[count][1] == 9)
				score += nums;
			if(piece[count][0]==1 || piece[count][1] == 9)
				score += tmpscore;
			else if(!bFull && (piece[count][0] == 2 || piece[count][1] == 8) && (row == 1 || row == 8))
				score -= tmpscore;
		}
	}
	return score;
}

//airthmetic
/*找出所有在垂直方向受保护的obcolor方的棋子，并累计分数*/
INT16 scan_vertical_aixes(board_type *board_ptr, UINT8 obcolor)
{
	INT16 score=0;
	UINT8 *cur_ptr, *stop_ptr;
	UINT8  piece[4][2];
	UINT8 count=0, tmpscore;
	UINT8 bFull;
	for(UINT8 col=1; col<9; col++)
	{
        tmpscore = (col == 1 || col == 8) ? 10:2;
		cur_ptr = &board_ptr->board[1][col];
		stop_ptr= &board_ptr->board[9][col];
		bFull = TRUE;
		count=0;
		while(cur_ptr < stop_ptr)
		{
		   if(*cur_ptr == obcolor)
		   {
			 piece[count][0]  = (cur_ptr - &board_ptr->board[0][col])/10;
			 while(*cur_ptr == obcolor)
				 cur_ptr += 10;
			 piece[count++][1] = (cur_ptr - &board_ptr->board[0][col])/10;
		   }
		   if(!*cur_ptr)
			   bFull = FALSE;
		   cur_ptr += 10;
		}
		while(count--)
		{
			UINT8 nums = (piece[count][1]-piece[count][0]);
			if(bFull || piece[count][0]==1 || piece[count][1] == 9)
				score += nums;
			if(piece[count][0]==1 || piece[count][1] == 9)
				score += tmpscore;
			else if(!bFull && (piece[count][0] == 2 || piece[count][1] == 8) && (col == 1 || col == 8))
				score -= (tmpscore<<1);
		}
	}
   	return score;
}

/*找出所有在右上到左下方向受保护的obcolor方的棋子，并累计分数*/
INT16 scan_fd_aixes(board_type *board_ptr, UINT8 obcolor)
{
	INT16 score =0;
    UINT8 *cur_ptr, *stop_ptr, *base_ptr;
	UINT8  piece[4][2];
	UINT8 count=0, tmpscore;
	UINT8 bFull;
	for(INT8 aixes = -5; aixes <= 5; aixes++)
	{
        tmpscore = (aixes == 0) ? 10:2;
		if(aixes <=0)
		{
			base_ptr = cur_ptr = &board_ptr->board[1][8+aixes];
			stop_ptr = &board_ptr->board[9+aixes][0];
		}
		else
		{
			base_ptr = cur_ptr = &board_ptr->board[aixes+1][8];
			stop_ptr= &board_ptr->board[9][aixes];
		}
		bFull = TRUE;
		count=0;
		while(cur_ptr < stop_ptr)
		{
		   if(*cur_ptr == obcolor)
		   {
				piece[count][0]  = cur_ptr - board_ptr->board[0];
				while(*cur_ptr == obcolor)
					cur_ptr += 9;
				piece[count++][1] = cur_ptr- board_ptr->board[0];
		   }
		   if(!*cur_ptr)
			   bFull = FALSE;
		   cur_ptr += 9;
		}
		while(count--)
		{
			UINT8 nums = (piece[count][1]-piece[count][0])/9;
			BOOL  toborder = (piece[count][0] == base_ptr - board_ptr->board[0] || 
				                  piece[count][1] == stop_ptr - board_ptr->board[0]);
			if(bFull || toborder)
				score += nums;

			if((aixes == 1 || aixes == -1) && toborder)
			    score -= tmpscore;
			/*如果是这块棋到达边界*/
			else if(toborder)
				score += tmpscore;
			/*如果有棋在角边上，则扣分*/
			else if(!bFull && (piece[count][0] == 27 ||
				                   piece[count][1] == 81))
				score -= (tmpscore<<1);
		}
	}

	/*如果角边有棋子，扣分*/
	if(board_ptr->board[1][1] == obcolor)
			score += 10;
	else 
	{
		if(board_ptr->board[1][2] == obcolor)
			 score -=2;
		if(board_ptr->board[2][1] == obcolor)
			 score -=2;
		if(board_ptr->board[2][2]== obcolor)
		 score -=2;
	}
     
	if(board_ptr->board[8][8] == obcolor)
		 score +=10;
	else
	{
		if(board_ptr->board[7][8]  == obcolor)
			 score -=2;
		if(board_ptr->board[8][7]== obcolor)
			 score -=2;
		if(board_ptr->board[7][7]== obcolor)
			score -= 2;
	}    
    return score;
}

/*找出所有在左上到右下方向受保护的obcolor方的棋子，并累计分数*/
INT16 scan_bd_aixes(board_type *board_ptr,  UINT8 obcolor)
{
	
    INT16 score =0;
    UINT8 *cur_ptr, *stop_ptr, *base_ptr;
	UINT8  piece[4][2];
	UINT8 count=0, tmpscore;
	UINT8 bFull;
	for(INT8 aixes = -5; aixes <= 5; aixes++)
	{
        tmpscore = (aixes == 0) ? 10:2;
		if(aixes <=0)
		{
			base_ptr = cur_ptr = &board_ptr->board[1-aixes][1];
			stop_ptr = &board_ptr->board[9][9+aixes];
		}
		else
		{
			base_ptr = cur_ptr = &board_ptr->board[1][aixes+1];
			stop_ptr= &board_ptr->board[9-aixes][9];
		}
		bFull = TRUE;
		count=0;
		while(cur_ptr < stop_ptr)
		{
		   if(*cur_ptr == obcolor)
		   {
				piece[count][0]  = cur_ptr - board_ptr->board[0];
				while(*cur_ptr == obcolor)
					cur_ptr += 11;
				piece[count++][1] = cur_ptr- board_ptr->board[0];
		   }
		   if(!*cur_ptr)
			   bFull = FALSE;
		   cur_ptr += 11;
		} 
		while(count--)
		{
			UINT8 nums = (piece[count][1]-piece[count][0])/11;
			BOOL  toborder = (piece[count][0] == base_ptr - board_ptr->board[0] || 
				                  piece[count][1] == stop_ptr - board_ptr->board[0]);

			if(bFull || toborder)
				score += nums;
			/*如果角边有棋子，扣分*/
			if((aixes == 1 || aixes == -1) && toborder)
			    score -= tmpscore;
			/*如果是这块棋到达边界*/
			else if(toborder)
				score += tmpscore;
			/*如果有棋在角边上，则扣分, 主对角线方向*/
			else if(!bFull && (piece[count][0] == 22 || piece[count][1] == 88) )
				score -= (tmpscore<<1);
		}
	}

	/*如果角边有棋子，扣分*/
	if(board_ptr->board[1][8] == obcolor)
			score += 10;
	else 
	{
		if(board_ptr->board[1][7] == obcolor)
			 score -=2;
		if(board_ptr->board[2][8] == obcolor)
			 score -=2;
		if(board_ptr->board[2][7]== obcolor)
		 score -=2;
	}
     
	if(board_ptr->board[8][1] == obcolor)
		 score +=10;
	else
	{
		if(board_ptr->board[7][1]  == obcolor)
			 score -=2;
		if(board_ptr->board[8][2]== obcolor)
			 score -=2;
		if(board_ptr->board[7][2]== obcolor)
			score -= 2;
	} 
    return score;
}

INT16 sample_calc_board_status(board_type *board_ptr, UINT8 obcolor)
{
	INT16 score=0;
	UINT8 *ptr = &board_ptr->board[1][1];
	UINT8 *stop = &board_ptr->board[8][9]; 
	UINT8 tmpcol = ~obcolor &0x03;
	while(ptr<stop)
	{
		if(*ptr == obcolor)
			score++;
		else if(*ptr == tmpcol)
			score--;
		ptr++;
	}
	return score;
}

/*计算棋局board_ptr的状态分*/
INT16 calc_board_status(board_type *board_ptr, UINT8 obcolor)
{
    INT16 score=0;
    score += scan_horiz_aixes(board_ptr,  obcolor);
    score += scan_vertical_aixes(board_ptr, obcolor);
	score += scan_bd_aixes(board_ptr, obcolor);
	score += scan_fd_aixes(board_ptr, obcolor);
	UINT8 tmpcol = ~obcolor & 0x03 ;
	if(board_ptr->board[1][1] == tmpcol)
		score -= 44;
	if(board_ptr->board[8][8] == tmpcol)
		score -= 44;
	if(board_ptr->board[1][8] == tmpcol)
        score -= 44;
	if(board_ptr->board[8][1] == tmpcol)
		score -= 44;
	return score;
}


/*从start_pos出发找到一个可下子的点，返回受影响的子的个数，
affected_list存放受影响的棋格的指针,第一个指针为落子的点*/

const INT16 delta_array[8] = {-11, 11, -9, 9, -1, 1, -10, 10};
INT16 find_move( board_type *board_ptr, INT16 start_pos, UINT8 obcolor, INT16 *affected_list)
{
	UINT8 *cel_ptr = board_ptr->board[0] + start_pos;
	UINT8 *stop_ptr = &board_ptr->board[8][9], *p;
	INT16 *aff_ptr = affected_list+1, *hold_aff;
	UINT8 aixes;
	UINT8 thithercolor = THITHER_COLOR(obcolor);
	while(1)
	{
		/*找到一个空格子*/
		while(*cel_ptr) 
		  if(++cel_ptr>=stop_ptr)
			  return 0;

		/*检查在8个方向上是否能吃掉对方的棋子，并记录被吃掉棋子的下标*/
		for(aixes =0;aixes<8; aixes++)
		{
			hold_aff = aff_ptr;
			p = cel_ptr + delta_array[aixes];
			while(*p == thithercolor)
			{
				*aff_ptr++ = p - board_ptr->board[0];
				p+= delta_array[aixes];
			}
			if(*p != obcolor)
				aff_ptr = hold_aff;
		}
		/*如果cel_ptr对应的点可以吃掉对方的子*/
		if(aff_ptr - affected_list > 1) 
		{
			*affected_list = cel_ptr - board_ptr->board[0];
			return (aff_ptr - affected_list);
		}
		cel_ptr++;
	}
}
  
void init_board(board_type *board_ptr)
{
	memset(board_ptr, 0, sizeof(board_type));
	/*init boarder*/
	memset(board_ptr->board[0], 0xff, 10);
	memset(board_ptr->board[9], 0xff, 10);
	for(int i=0; i<9; i++)
	{
		board_ptr->board[i][0] = board_ptr->board[i][9] =0xff;
	}

	/*init chess*/
	board_ptr->board[4][4] = board_ptr->board[5][5] = CHESS_WHITE;
	board_ptr->board[4][5] = board_ptr->board[5][4] = CHESS_BLACK;
	cur_step = 0;
	computer_side = CHESS_WHITE;
}   

/*从棋盘的一个状态出发，扩展此结点，并返回此结点的部分回溯值*/
void extend_node_one(tree_node_type *node_ptr, tree_node_type *parent_ptr,UINT8 obcolor)
{
   tree_node_type childnode;
   INT16 affected_list[MAX_AFFECTED_PIECES];
   INT16 start_pos = 11, num;
   num = find_move(&node_ptr->board, start_pos, obcolor, affected_list);
   /*如果是终局状态，则返回状态估值函数的值*/
   if(++cur_depth == max_depth || num==0 )
   {
	   /*如果已方PASS但没到棋局结束,要扣分*/
	   node_ptr->value = calc_board_status(&node_ptr->board, computer_side);
	   if(!num)
	   {
		   /*如果双方都没棋下*/
		   if(!find_move(&node_ptr->board, 11, ~obcolor&0x03, affected_list)) 
              return;

           if(obcolor == computer_side)
		   {
			   node_ptr->value -= 15;
			   return ;
		   }
		   node_ptr->value += 15;
	   }
      return;		   
   }
      /*初始化回溯值*/ 
   node_ptr->value = (obcolor == computer_side)? -INITIAL_VALUE : INITIAL_VALUE;
   memcpy(&childnode.board, &node_ptr->board, sizeof(board_type));
   while(num)
   {
		while(num--)
			childnode.board.board[0][affected_list[num]] = obcolor;

		/*递归计算部分回溯值*/
		UINT8 depth = cur_depth;
		extend_node_one(&childnode, node_ptr, (~obcolor)&0x03);
		cur_depth = depth;
		/*如果此结点是棋手一方，则部分回溯值是子结点中最大的一个*/
		if(obcolor == computer_side)
		{
			if(childnode.value > node_ptr->value)
			{
			   node_ptr->value = childnode.value; 
			   node_ptr->movepos = affected_list[0];
			}
		}
		/*如果是对手一方，部分回溯值是子结点中最小的一个*/ 
		else
		{
			if(childnode.value < node_ptr->value)
			{
				node_ptr->value = childnode.value;
				node_ptr->movepos = affected_list[0];
			}
		}
		/* α-β裁减的判断   在考虑轮到棋手下棋的一个亲节点及轮到对手下棋的一个子节点时，
		  如果该子节点的数值已经小于或等于其亲节点的回溯值，
		  那么就不需要对该节点或者其后续节点做更多的处理了。
		  计算的过程可以直接返回到亲节点上。
		*/
		/*在考虑轮到对手下棋的一个亲节点及轮到棋手下棋的一个子节点时，
			  如果该子节点的部分回溯值已经大于或等于其亲节点的部分回溯值，
			  那么就不需要对该子节点或者其后裔节点做更多的处理了。
			  计算过程可以直接返回到亲节点上。*/
		if(parent_ptr)
		{
			if(obcolor != computer_side)
			{
				/*α裁减*/
				if(node_ptr->value <= parent_ptr->value)
					return;
			}
			else
			{
				/*β裁减*/
				if(node_ptr->value >= parent_ptr->value)
					return;
			}
		}
		/*找到下一个可落子的点*/
		start_pos = affected_list[0]+1;
		memcpy(&childnode.board, &node_ptr->board, sizeof(board_type));
		num = find_move(&childnode.board, start_pos, obcolor, affected_list);
   }
   return;
}


void extend_node_two(tree_node_type *node_ptr, tree_node_type *parent_ptr,UINT8 obcolor)
{
   tree_node_type childnode;
   INT16 affected_list[MAX_AFFECTED_PIECES];
   INT16 start_pos = 11, num;
   num = find_move(&node_ptr->board, start_pos, obcolor, affected_list);
   /*如果是终局状态，则返回状态估值函数的值*/
   if(!num)
   {
	   /*如果已方PASS但没到棋局结束,要扣分*/
	   node_ptr->value = sample_calc_board_status(&node_ptr->board, computer_side);
	   /*如果双方都没棋下*/
	    if(!find_move(&node_ptr->board, 11, ~obcolor&0x03, affected_list)) 
             return;

        if(obcolor == computer_side)
		{
			node_ptr->value -= 10;
	        return;
		}
    	node_ptr->value += 10;
	    return;
   }
      /*初始化回溯值*/ 
   node_ptr->value = (obcolor == computer_side)? -INITIAL_VALUE : INITIAL_VALUE;
   memcpy(&childnode.board, &node_ptr->board, sizeof(board_type));
   while(num)
   {
     while(num--)
		 childnode.board.board[0][affected_list[num]] = obcolor;
	 /*递归计算部分回溯值*/
	 UINT8 depth = cur_depth;
     extend_node_two(&childnode, node_ptr, (~obcolor)&0x03);
	 cur_depth = depth;
	 /*如果此结点是棋手一方，则部分回溯值是子结点中最大的一个*/
	 if(obcolor == computer_side)
	 {
	   if(childnode.value > node_ptr->value)
	   {
		   node_ptr->value = childnode.value; 
		   node_ptr->movepos = affected_list[0];
	   }
	 }
	 /*如果是对手一方，部分回溯值是子结点中最小的一个*/ 
	 else
	 {
		if(childnode.value < node_ptr->value)
		{
			node_ptr->value = childnode.value;
			node_ptr->movepos = affected_list[0];
		}
   	 }
	 /* α-β裁减的判断   在考虑轮到棋手下棋的一个亲节点及轮到对手下棋的一个子节点时，
		  如果该子节点的数值已经小于或等于其亲节点的回溯值，
		  那么就不需要对该节点或者其后续节点做更多的处理了。
		  计算的过程可以直接返回到亲节点上。
	 */
	   /*在考虑轮到对手下棋的一个亲节点及轮到棋手下棋的一个子节点时，
			  如果该子节点的部分回溯值已经大于或等于其亲节点的部分回溯值，
			  那么就不需要对该子节点或者其后裔节点做更多的处理了。
			  计算过程可以直接返回到亲节点上。*/
	 if(parent_ptr)
	 {
		 if(obcolor != computer_side)
		 {
			 /*α裁减*/
			 if(node_ptr->value <= parent_ptr->value)
			  return;
		 }
		 else
		 {
			 /*β裁减*/
			 if(node_ptr->value >= parent_ptr->value)
				return ;
		 }
	 }
   	 /*找到下一个可落子的点*/
	 start_pos = affected_list[0]+1;
	 memcpy(&childnode.board, &node_ptr->board, sizeof(board_type));
	 num = find_move(&childnode.board, start_pos, obcolor, affected_list);
   }
   return;
}

void get_chess_score(board_type *board_ptr, UINT16 &iWscore, UINT16 &iBscore)
{
	iWscore =0; iBscore =0;
	for(INT16 i=1; i<=BOARD_ROWS; i++)
		for(INT16 j=1; j<=BOARD_COLS; j++)
		{
			if(board_ptr->board[i][j]  == CHESS_BLACK)
				iBscore++;
			else  if(board_ptr->board[i][j] == CHESS_WHITE)
				iWscore++;
		}
}

void game_over(board_type *board_ptr, HWND hwnd)
{
		if ( hwnd!=NULL )
		{
			SendMessage(hwnd,WM_GAME_OVER,0,0);
			return;
		}

		UINT16 wscore, bscore;
		TCHAR strcomwin[]=TEXT("虽然你很历害,但我还是赢了你!");
		TCHAR struserwin[]=TEXT("让你一次,下次你可没这么走运了!");
		TCHAR strdogfall[]=TEXT("我没好好下,你才有机会平局!");
		TCHAR *text;
		get_chess_score(board_ptr, wscore, bscore);
		if(computer_side == CHESS_WHITE)
		{
			if(wscore > bscore)
				text = strcomwin;
			else if(wscore <bscore)
				text = struserwin;
			else text = strdogfall;
		}
		else
		{
			if(wscore > bscore)
				text = struserwin;			
			else if(wscore <bscore)
				text = strcomwin;
			else text = strdogfall;
		}

		MessageBox(hwnd,  text, L"黑白棋", MB_OK|MB_ICONINFORMATION);
}
 
void computer_play(board_type *board_ptr, HWND hwnd)
{
   cur_depth =0;
   tree_node_type node;
   INT16 affected_list[MAX_AFFECTED_PIECES];
start:
   memcpy(&node.board, board_ptr, sizeof(board_type));
   node.movepos =0;
   if(cur_step>= STEP_MONMENT2)
   {
      extend_node_two(&node, NULL, computer_side);
   }
   else if(cur_step > STEP_MONMENT1)
   {
	   max_depth = depth2[g_iGameLevel];
	   extend_node_one(&node, NULL, computer_side);
   }
   else 
   {
	   max_depth = depth1[g_iGameLevel];
	   extend_node_one(&node, NULL, computer_side);
   }

   if(!do_move_chess(board_ptr, node.movepos, computer_side, hwnd))
   {
		if(!find_move(board_ptr, 11, (~computer_side)&0x03, affected_list))
		{
			game_over(board_ptr, hwnd);
			return;
		}
		else
		{
			//MessageBox(hwnd,L"我没棋下了,你再走一步!", L"黑白棋", MB_OK|MB_ICONINFORMATION);
			return;
		}
   }
   else
   { 
	   if(!find_move(board_ptr, 11, (~computer_side)&0x03, affected_list))
	   {
   		   if(!find_move(board_ptr, 11, computer_side, affected_list))
		   {
			   game_over(board_ptr, hwnd); 
			   return;
		   }
		   else
		   {
				//MessageBox(hwnd ,L"你没棋下了,我再走一步!", L"黑白棋", MB_OK|MB_ICONINFORMATION);         
				goto start;
		   }
	   }
   }
} 

UINT8 do_move_chess(board_type *board_ptr, UINT16 movepos, UINT8 obcolor, HWND hwnd)
{
	INT16 affected_list[MAX_AFFECTED_PIECES];
	memset( affected_list,0,sizeof(affected_list) );
	INT16 num = find_move(board_ptr, movepos, obcolor, affected_list);
	if(!num || affected_list[0] != movepos)
		return 0; 

	if( hwnd!=NULL )
	{
		::SendMessage(hwnd, WM_CHESSMOVE, WPARAM(affected_list[0]), obcolor );
	}

	for(int i=0; i<num; i++)
	{
		board_ptr->board[0][affected_list[i]] = obcolor;
		if( hwnd!=NULL )
			::SendMessage(hwnd, WM_TRANCHESS, WPARAM(affected_list[i]),LPARAM(i<<16|obcolor));
	}
	step_array[cur_step] = movepos;
	Step_Color[cur_step] = obcolor;
	++cur_step;
	return 1;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*******************************************************************
* Function Name ： 
* Remark        ： 
* Update Record ： 
*               ： 
* Params        ： 
* Return Value  ： 
* Author        ： Zhxj
* Emaile        ： shenzhenit@163.com
* Date Created  ： 2009年10月27日 星期二
*******************************************************************/

int Check( const board_type *board_ptr, int x, int y)
{
	if(x<1 || x>BOARD_ROWS || y<1 || y>BOARD_COLS )
		return 0;//越界为0
	if( board_ptr->board[x][y]==0 )
		return 0;//无子为0
	return 1;
}


void	GetMovPtArray(const board_type *board_ptr, PTVECAY &wptay,PTVECAY &bptay )
{
	int wn=0,bn=0; //wn：代表白色可放棋子的个数，bn：代表黑色可放棋子的个数（本函数的功能1）
	int btp_x=-7,btp_y=-7,wtp_x=-7,wtp_y=-7;
	int temp,temp2;
	int x,y;
	POINT ptAdd;

	wptay.clear();
	bptay.clear();


	for(int i=1; i<=BOARD_ROWS; i++)
		for(int j=1; j<=BOARD_COLS; j++)
		{
			if( board_ptr->board[i][j]==0)
			{
				//水平向左
				if(Check(board_ptr,i,j-1) && Check(board_ptr,i,j-2))//没有越界且此位置有棋子
				{
					x=i,y=j;
					temp=board_ptr->board[x][y-1];
					for(y-=2;y>=1;y--)
					{
						temp2=board_ptr->board[x][y];
						if(temp2==0)
							break;
						if(temp2!=temp)
						{
							if(temp2==CHESS_WHITE)
							{
								if(wtp_x!=i || wtp_y!=j)
								{
									wtp_x=i,wtp_y=j;
									wn++;
									ptAdd.x =i;ptAdd.y=j ;
									wptay.push_back( ptAdd );//白色可放子的位置
								}
							}
							else
							{
								if(btp_x!=i || btp_y!=j)
								{
									btp_x=i,btp_y=j;
									bn++;
									ptAdd.x =i;ptAdd.y=j ;
									bptay.push_back( ptAdd );
									//bptay.push_back( ptAdd );//黑色可放子的位置
								}
							}
							break;
						}
					}
				}
				//水平向右
				if(Check(board_ptr,i,j+1) && Check(board_ptr,i,j+2))
				{
					x=i,y=j;
					temp=board_ptr->board[x][y+1];
					for(y+=2;y<=BOARD_COLS;y++)
					{
						temp2=board_ptr->board[x][y];
						if(temp2==0)
							break;
						if(temp2!=temp)
						{
							if(temp2==CHESS_WHITE)
							{
								if(wtp_x!=i || wtp_y!=j)
								{
									wtp_x=i,wtp_y=j;
									wn++;
									ptAdd.x =i;ptAdd.y=j ;
									wptay.push_back( ptAdd );//白色可放子的位置
								}
							}
							else
							{
								if(btp_x!=i || btp_y!=j)
								{
									btp_x=i,btp_y=j;
									bn++;
									ptAdd.x =i;ptAdd.y=j ;
									bptay.push_back( ptAdd );
								}
							}
							break;
						}
					}
				}
				//垂直向上
				if(Check(board_ptr,i-1,j) && Check(board_ptr,i-2,j))
				{
					x=i,y=j;
					temp=board_ptr->board[x-1][y];
					for(x-=2;x>=1;x--)
					{
						temp2=board_ptr->board[x][y];
						if(temp2==0)
							break;
						if(temp2!=temp)
						{
							if(temp2==CHESS_WHITE)
							{
								if(wtp_x!=i || wtp_y!=j)
								{
									wtp_x=i,wtp_y=j;
									wn++;
									ptAdd.x =i;ptAdd.y=j ;
									wptay.push_back( ptAdd );//白色可放子的位置
								}
							}
							else
							{
								if(btp_x!=i || btp_y!=j)
								{
									btp_x=i,btp_y=j;
									bn++;
									ptAdd.x =i;ptAdd.y=j ;
									bptay.push_back( ptAdd );
								}
							}
							break;
						}
					}
				}

				//垂直向下
				if(Check(board_ptr,i+1,j) && Check(board_ptr,i+2,j))
				{
					x=i,y=j;
					temp=board_ptr->board[x+1][y];
					for(x+=2;x<=BOARD_ROWS;x++)
					{
						temp2=board_ptr->board[x][y];
						if(temp2==0)
							break;
						if(temp2!=temp)
						{
							if(temp2==CHESS_WHITE)
							{
								if(wtp_x!=i || wtp_y!=j)
								{
									wtp_x=i,wtp_y=j;
									wn++;
									ptAdd.x =i;ptAdd.y=j ;
									wptay.push_back( ptAdd );//白色可放子的位置
								}
							}
							else
							{
								if(btp_x!=i || btp_y!=j)
								{
									btp_x=i,btp_y=j;
									bn++;
									ptAdd.x =i;ptAdd.y=j ;
									bptay.push_back( ptAdd );
								}
							}
							break;
						}
					}
				}

				//斜向左上
				if(Check(board_ptr,i-1,j-1) && Check(board_ptr,i-2,j-2))
				{
					x=i,y=j;
					temp=board_ptr->board[x-1][y-1];
					for(x-=2,y-=2;(x>=1) && (y>=1);x--,y--)
					{
						temp2=board_ptr->board[x][y];
						if(temp2==0)
							break;
						if(temp2!=temp)
						{
							if(temp2==CHESS_WHITE)
							{
								if(wtp_x!=i || wtp_y!=j)
								{
									wtp_x=i,wtp_y=j;
									wn++;
									ptAdd.x =i;ptAdd.y=j ;
									wptay.push_back( ptAdd );//白色可放子的位置
								}
							}
							else
							{
								if(btp_x!=i || btp_y!=j)
								{
									btp_x=i,btp_y=j;
									bn++;
									ptAdd.x =i;ptAdd.y=j ;
									bptay.push_back( ptAdd );
								}
							}
							break;
						}
					}
				}

				//斜向右上
				if(Check(board_ptr,i-1,j+1) && Check(board_ptr,i-2,j+2))
				{
					x=i,y=j;
					temp=board_ptr->board[x-1][y+1];
					for(x-=2,y+=2;(x>=1) && (y<=BOARD_COLS);x--,y++)
					{
						temp2=board_ptr->board[x][y];
						if(temp2==0)
							break;
						if(temp2!=temp)
						{
							if(temp2==CHESS_WHITE)
							{
								if(wtp_x!=i || wtp_y!=j)
								{
									wtp_x=i,wtp_y=j;
									wn++;
									ptAdd.x =i;ptAdd.y=j ;	
									wptay.push_back( ptAdd );//白色可放子的位置
								}
							}
							else
							{
								if(btp_x!=i || btp_y!=j)
								{
									btp_x=i,btp_y=j;
									bn++;
									ptAdd.x =i;ptAdd.y=j ;	
									bptay.push_back( ptAdd );
								}
							}
							break;
						}
					}
				}

				//斜向左下
				if(Check(board_ptr,i+1,j-1) && Check(board_ptr,i+2,j-2))
				{
					x=i,y=j;
					temp=board_ptr->board[x+1][y-1];
					for(x+=2,y-=2;(x<=BOARD_ROWS) && (y>=1);x++,y--)
					{
						temp2=board_ptr->board[x][y];
						if(temp2==0)
							break;
						if(temp2!=temp)
						{
							if(temp2==CHESS_WHITE)
							{
								if(wtp_x!=i || wtp_y!=j)
								{
									wtp_x=i,wtp_y=j;
									wn++;
									ptAdd.x =i;ptAdd.y=j ;	
									wptay.push_back( ptAdd );//白色可放子的位置
								}
							}
							else
							{
								if(btp_x!=i || btp_y!=j)
								{
									btp_x=i,btp_y=j;
									bn++;
									ptAdd.x =i;ptAdd.y=j ;	
									bptay.push_back( ptAdd );
								}
							}
							break;
						}
					}
				}

				//斜向右下
				if(Check(board_ptr,i+1,j+1) && Check(board_ptr,i+2,j+2))
				{
					x=i,y=j;
					temp=board_ptr->board[x+1][y+1];
					for(x+=2,y+=2;(x<=BOARD_ROWS) && (y<=BOARD_COLS);x++,y++)
					{
						temp2=board_ptr->board[x][y];
						if(temp2==0)
							break;
						if(temp2!=temp)
						{
							if(temp2==CHESS_WHITE)
							{
								if(wtp_x!=i || wtp_y!=j)
								{
									wtp_x=i,wtp_y=j;
									wn++;
									ptAdd.x =i;ptAdd.y=j ;	
									wptay.push_back( ptAdd );//白色可放子的位置
								}
							}
							else
							{
								if(btp_x!=i || btp_y!=j)
								{
									btp_x=i,btp_y=j;
									bn++;
									ptAdd.x =i;ptAdd.y=j ;	
									bptay.push_back( ptAdd );
								}
							}
							break;
						}
					}
				}

			}//end if
		}//end second for

}