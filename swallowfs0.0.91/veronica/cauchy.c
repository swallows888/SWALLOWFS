#include "cauchy.h"
//cols是k源码块
//rows是m编码块
int cauchy_inital()//用于初始化系统
{
	if(cbest_init==FALSE)//将
	{
		cbest_all[0] = cbest_0;
		cbest_all[1] = cbest_1; 
		cbest_all[2] = cbest_2; 
		cbest_all[3] = cbest_3; 
		cbest_all[4] = cbest_4; 
		cbest_all[5] = cbest_5;
		cbest_all[6] = cbest_6; 
		cbest_all[7] = cbest_7;
		cbest_all[8] = cbest_8;
		cbest_all[9] = cbest_9; 
		cbest_all[10] = cbest_10;
		cbest_all[11] = cbest_11;
		cbest_all[12] = cbest_12;
		cbest_all[13] = cbest_13;
		cbest_all[14] = cbest_14;
		cbest_all[15] = cbest_15;
		cbest_all[16] = cbest_16;
		cbest_all[17] = cbest_17;
		cbest_all[18] = cbest_18;
		cbest_all[19] = cbest_19;
		cbest_all[20] = cbest_20;
		cbest_all[21] = cbest_21;
		cbest_all[22] = cbest_22;
		cbest_all[23] = cbest_23;
		cbest_all[24] = cbest_24;
		cbest_all[25] = cbest_25;
		cbest_all[26] = cbest_26;
		cbest_all[27] = cbest_27;
		cbest_all[28] = cbest_28;
		cbest_all[29] = cbest_29;
		cbest_all[30] = cbest_30;
		cbest_all[31] = cbest_31;
		cbest_all[32] = (int *) cbest_32;
	}
	cbest_init=TRUE;
	return TRUE;
}
int cauchy_n_ones(int n,int w)//返回n生成的bit子矩阵有几个1
{
	int i,j,no,cno,nones,highbit;
	//生成最高位
	highbit=(1<<(w-1));

	if(PPs[w]==-1)
	{
		nones=0;
		PPs[w]=galois_single_multiply(highbit,2,w);//生成生成元
		for(i=0;i<w;i++)
		{
			if(PPs[w]&(1<<i))
			{
				ONEs[w][nones]=(1<<i);
				nones++;
			}
			NOs[w]=nones;
		}
	}
	no=0;
	for(i=0;i<w;i++)//扫描第一行
	{
		if(n&(1<<i))
		{
			no++;
		}
	}
	cno=no;
	for(i=1;i<w;i++)
	{
		if(n&highbit)
		{
			n^=highbit;
			n<<=1;
			n^=PPs[w];
			cno--;
			for(j=0;j<NOs[w];j++)
			{
				cno+=(n&ONEs[w][j])?1:-1;
			}
		}
		else
		{
			n<<=1;
		}
		no+=cno;
	}
	return no;
}
int cauchy_orgininal_coding_matrix(INTMATRIX *matrix)//生成初始的柯西矩阵
{
	int i,j,rows,cols,w,index;
	UINT32 *mat;
	//读取数据
	mat=matrix->matrix;
	rows=matrix->xm;
	cols=matrix->ym;
	w=matrix->w;
	//生成随机cauchy数据
	//先分配空间
	/*if(mat!=NULL)
	{
		free(mat);
	}*/
	if(mat==NULL)
	{
		mat=talloc(UINT32,rows*cols);
		if(mat==NULL)
		{
			show_error("cauchy","coding_matrix","no memory");
			return FALSE;
		}
		matrix->matrix=mat;
	}
	
	index=0;
	//开始计算
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			mat[index]=galois_inverse(i^(rows+j),w);
			index++;
		}
	}
	matrix->matrix=mat;
	return TRUE;
}

int cauchy_orgininal_coding_bitmatrix(BITMATRIX *matrix)//生成相应的bitmatrix
{
	int resTF;
	INTMATRIX *intmatrixptr;
	UINT32 *intmat;
	int rows,cols,w;
	int introws,intcols;

	rows=matrix->xm;
	cols=matrix->ym;
	w=matrix->w;

	introws=rows/w;
	intcols=cols/w;
	intmat=talloc(UINT32,introws*intcols);
	if(intmat==NULL)
	{
		show_error("cauchy","orgininal_coding_bitmatrix","No memory");
		return FALSE;
	}
	intmatrixptr=talloc(INTMATRIX,1);
	if(intmatrixptr==NULL)
	{
		show_error("cauchy","orgininal_coding_bitmatrix","No memory");
		return FALSE;
	}
	intmatrixptr->matrix=intmat;
	intmatrixptr->xm=introws;
	intmatrixptr->ym=intcols;
	intmatrixptr->w=w;
	resTF=cauchy_orgininal_coding_matrix(intmatrixptr);
	if(resTF==FALSE)
	{
		show_error("cauchy","orgininal_coding_bitmatrix","orgininal coding matrix can't calculate");
		return FALSE;
	}
	resTF=matrix_matrix_to_bitmatrix(intmatrixptr,matrix);
	if(resTF==FALSE)
	{
		show_error("cauchy","orgininal_coding_bitmatrix","bitmatrix can't convert");
		return FALSE;
	}
	//matrix_matrix_to_bitmatrix(intmatrixptr,matrix);
	free(intmatrixptr);
	free(intmat);
	return TRUE;
}

int cauchy_xy_coding_matrix(INTMATRIX *matrix, INTARRAY *arrx,INTARRAY *arry)//根据给定x,y生成
{
	//读取参数
	int i,j,index,rows,cols,w,xsize,ysize;
	UINT32 *mat,*xitem,*yitem;
	mat=matrix->matrix;
	rows=matrix->xm;
	cols=matrix->ym;
	w=matrix->w;

	xitem=arrx->items;
	xsize=arrx->size;

	yitem=arry->items;
	ysize=arry->size;

	//合法性检测
	if(rows!=xsize)
	{
		show_error("cauchy","coding_matrix","rows!=xsize");
		return FALSE;
	}
	if(cols!=ysize)
	{
		show_error("cauchy","coding_matrix","cols!=ysize");
		return FALSE;
	}
	//先分配空间
	if(mat!=NULL)
	{
		free(mat);
	}
	mat=talloc(UINT32,rows*cols);
	if(mat=NULL)
	{
		show_error("cauchy","coding_matrix","no memory");
		return FALSE;
	}
	matrix->matrix=mat;
	index=0;
	//开始生成
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			mat[index]=galois_inverse(xitem[i]^yitem[j],w);
			index++;
		}
	}
	matrix->matrix=mat;
	return TRUE;
}
int cauchy_improve_coding_matrix(INTMATRIX *matrix)//改进cauchy矩阵
{
	int rows,cols,w,index,i,j,x,tmp,bno,tno,bno_index;
	UINT32 *mat;

	mat=matrix->matrix;
	rows=matrix->xm;
	cols=matrix->ym;
	w=matrix->w;
	
	bno_index=-1;

	if(mat==NULL)
	{
		show_error("cauchy","improve_coding_matrix","matrix==NULL");
		return FALSE;
	}
	for(j=0;j<cols;j++)//编码块
	{
		if(mat[j]!=1)
		{
			tmp=galois_inverse(mat[j],w);
			index=j;
			for(i=0;i<rows;i++)
			{
				mat[index]=galois_single_multiply(mat[index],tmp,w);
				index+=cols;
			}
		}
	}
	for(i=1;i<rows;i++)
	{
		bno=0;
		index=i*cols;
		for(j=0;j<cols;j++)
		{
			bno+=cauchy_n_ones(mat[index+j],w);
		}
		bno_index=-1;
		for(j=0;j<cols;j++)
		{
			tmp=galois_inverse(mat[index+j],w);
			tno=0;
			for(x=0;x<cols;x++)
			{
				tno+=cauchy_n_ones(galois_single_multiply(mat[index+x],tmp,w),w);
			}
			if(tno<bno)
			{
				bno=tno;
				bno_index=j;
			}
		}
	}
	if(bno_index!=-1)
	{
		tmp=galois_inverse(mat[index+bno_index],w);
		for(j=0;j<cols;j++)
		{
			mat[index+j]=galois_single_multiply(mat[index+j],tmp,w);
		}
	}
	return TRUE;
}
int cauchy_good_general_coding_matrix(INTMATRIX *matrix)//生成Plank的Cauchy矩阵
{
	int i,rows,cols,w,resTF;
	UINT32 *mat;

	mat=matrix->matrix;
	rows=matrix->xm;
	cols=matrix->ym;
	w=matrix->w;
	if(mat==NULL)
	{
		mat=talloc(UINT32,rows*cols);
		if(mat==NULL)
		{
			show_error("cauchy","good_general_coding","no memory");
			return FALSE;
		}
		matrix->matrix=mat;
	}
	
	//如何没有初始化
	if(rows==2&&cols<cbest_max_k[w])//主要为RAID-6准备
	{
		//生成空间
		/*if(mat!=NULL)
		{
			free(mat);
		}
		mat=talloc(UINT32,rows*cols);
		if(mat==NULL)
		{
			show_error("cauchy","good_general_coding","no memory");
			return FALSE;
		}
		matrix->matrix=mat;*/
		if(cbest_init==FALSE)
		{
			cauchy_inital();
		}
		for(i=0;i<cols;i++)
		{
			mat[i]=1;
			mat[i+cols]=cbest_all[w][i];
		}	
	}
	else
	{
		//使用改进版矩阵
		resTF=cauchy_orgininal_coding_matrix(matrix);
		if(resTF==FALSE)
		{
			show_error("cauchy","good_general_coding","fails to create org matrix");
			return FALSE;
		}
		resTF=cauchy_improve_coding_matrix(matrix);
		if(resTF==FALSE)
		{
			show_error("cauchy","good_general_coding","fails to create org matrix");
			return FALSE;
		}
	}
	//最后加固矩阵
	matrix->matrix=mat;
	return TRUE;
}
//生成相应的bitmatrix
int cauchy_good_general_coding_bitmatrix(BITMATRIX *matrix)
{
	int resTF;
	INTMATRIX *intmatrixptr;
	UINT32 *intmat;
	int rows,cols,w;
	int introws,intcols;

	rows=matrix->xm;
	cols=matrix->ym;
	w=matrix->w;

	introws=rows/w;
	intcols=cols/w;
	intmat=talloc(UINT32,introws*intcols);
	if(intmat==NULL)
	{
		show_error("cauchy","orgininal_coding_bitmatrix","No memory");
		return FALSE;
	}
	intmatrixptr=talloc(INTMATRIX,1);
	if(intmatrixptr==NULL)
	{
		show_error("cauchy","orgininal_coding_bitmatrix","No memory");
		return FALSE;
	}
	intmatrixptr->matrix=intmat;
	intmatrixptr->xm=introws;
	intmatrixptr->ym=intcols;
	intmatrixptr->w=w;
	resTF=cauchy_good_general_coding_matrix(intmatrixptr);
	if(resTF==FALSE)
	{
		show_error("cauchy","orgininal_coding_bitmatrix","good general coding matrix can't calculate");
		return FALSE;
	}
	resTF=matrix_matrix_to_bitmatrix(intmatrixptr,matrix);
	if(resTF==FALSE)
	{
		show_error("cauchy","orgininal_coding_bitmatrix","bitmatrix can't convert");
		return FALSE;
	}
	//matrix_matrix_to_bitmatrix(intmatrixptr,matrix);
	free(intmatrixptr);
	free(intmat);
	return TRUE;
}
int cauchy_API_RAID6(STRIPSECTBLOCKGROUP *strsetblockgroupptr,STRIPSECT *pparityptr,STRIPSECT *qparityptr)
{
	BITMATRIX *bitmatrixptr;

	REGION *regionptr;
	CODEPARA *codeparaptr;
	STRIPSECT *strsetptr,*curstrsetptr;
	UINT32 *mat;
	int w,datanum,codenum,rows,cols,resTF;
	//计算数据
	bitmatrixptr=talloc(BITMATRIX,1);
	if(bitmatrixptr==NULL)
	{
		show_error("cauchy","API_RAID6","No memory");
		return FALSE;
	}

	regionptr=pparityptr->regionptr;//获取regionptr

	codeparaptr=regionptr->codepara;

	datanum=codeparaptr->datanum;
	codenum=codeparaptr->codenum;
	w=codeparaptr->w;

	rows=codenum*w;
	cols=datanum*w;
	mat=talloc(UINT32,rows*cols);
	if(mat==NULL)
	{
		show_error("cauchy","API_RAID6","No memory");
		return FALSE;
	}
	bitmatrixptr->matrix=mat;
	bitmatrixptr->xm=rows;
	bitmatrixptr->ym=cols;
	bitmatrixptr->w=w;

	resTF=cauchy_good_general_coding_bitmatrix(bitmatrixptr);
	if(resTF==FALSE)
	{
		show_error("cauchy","API_RAID6","bit matrix can't create");
		return FALSE;
	}
	resTF=matrix_bitmatrix_encode_stripsect_RAID6(strsetblockgroupptr,pparityptr,qparityptr,bitmatrixptr);
	if(resTF==FALSE)
	{
		show_error("cauchy","API_RAID6","");
		return FALSE;
	}
	return TRUE;
}
int cauchy_API(STRIP *stripptr)
{
	//
	BITMATRIX *bitmatrixptr;

	REGION *regionptr;
	CODEPARA *codeparaptr;
	STRIPSECT *strsetptr,*curstrsetptr;
	UINT32 *mat;
	int w,datanum,codenum,rows,cols,resTF;
	//计算数据
	bitmatrixptr=talloc(BITMATRIX,1);
	if(bitmatrixptr==NULL)
	{
		show_error("cauchy","API_RAID6","No memory");
		return FALSE;
	}

	regionptr=stripptr->regionptr;//获取regionptr

	codeparaptr=regionptr->codepara;

	datanum=codeparaptr->datanum;
	codenum=codeparaptr->codenum;
	w=codeparaptr->w;

	rows=codenum*w;
	cols=datanum*w;
	mat=talloc(UINT32,rows*cols);
	if(mat==NULL)
	{
		show_error("cauchy","API_RAID6","No memory");
		return FALSE;
	}
	bitmatrixptr->matrix=mat;
	bitmatrixptr->xm=rows;
	bitmatrixptr->ym=cols;
	bitmatrixptr->w=w;

	resTF=cauchy_good_general_coding_bitmatrix(bitmatrixptr);
	if(resTF==FALSE)
	{
		show_error("cauchy","API_RAID6","bit matrix can't create");
		return FALSE;
	}

	return TRUE;
}

int cauchy_degrade_API_RAID6(CHUNK *chunkptr,ERRORPATTERN *errormatrix)
{
	return TRUE;
}
int cauchy_degrade_API(CHUNK *chunkptr,ERRORPATTERN *errormatrix)
{
	return TRUE;
}


int cauchy_update_API_RAID6(CHUNKPOSITIONBLOCKGROUP *chunkpositionblockgroupptr,CHUNKBLOCKGROUP *parityblockgroupptr,STRIP *stripptr)
{
	//int resTF;
	//根据当前的矩阵数据计算更更新
	BITMATRIX *matrix;

	REGION *regionptr;
	CODEPARA *codeparaptr;


	STRIPSECT *pparityptr,*qparityptr;

	STRIPSECTBLOCKGROUP *strsetblockgroupptr;
	STRIPSECTBLOCK *strsetblockptr,*curstrsetblockptr;
	int i,isparity;
	int strsetnum;

	STRIPSECTGROUP *strsetgroupptr;
	STRIPSECT *strsetptr,*curstrsetptr;

	UINT32 *mat;
	int w,datanum,codenum,rows,cols,resTF;

	regionptr=stripptr->regionptr;
	codeparaptr=regionptr->codepara;

	datanum=codeparaptr->datanum;
	codenum=codeparaptr->codenum;
	w=codeparaptr->w;

	//计算行列标号
	rows=codenum*w;
	cols=datanum*w;
	mat=talloc(UINT32,rows*cols);
	if(mat==NULL)
	{
		show_error("cauchy","API_RAID6","No memory");
		return FALSE;
	}
	matrix=talloc(BITMATRIX,1);
	if(matrix==NULL)
	{
		show_error("cauchy","API_RAID6","No memory");
		return FALSE;
	}
	matrix->matrix=mat;
	matrix->xm=rows;
	matrix->ym=cols;
	matrix->w=w;
	strsetnum=datanum;

	strsetblockptr=talloc(STRIPSECTBLOCK,strsetnum);
	if(strsetblockptr==NULL)
	{
		show_error("cauchy","API_RAID6","No memory");
		return FALSE;
	}
	strsetblockgroupptr=talloc(STRIPSECTBLOCKGROUP,1);
	if(strsetblockgroupptr==NULL)
	{
		show_error("cauchy","API_RAID6","No memory");
		return FALSE;
	}
	strsetblockgroupptr->stripsectblockptr=strsetblockptr;
	strsetblockgroupptr->groupsize=strsetnum;

	strsetgroupptr=stripptr->stripsectlist;

	strsetptr=strsetgroupptr->stripsectgroup;
	strsetnum=strsetgroupptr->groupsize;

	curstrsetptr=strsetptr;
	curstrsetblockptr=strsetblockptr;
	for(i=0;i<strsetnum;i++)
	{
		isparity=curstrsetptr->isparity;
		if(isparity==FALSE)
		{
			curstrsetblockptr->stripsectptr=curstrsetptr;
			curstrsetblockptr->stripsectid=curstrsetptr->StripsectID;
			curstrsetblockptr++;
		}
		if(isparity==PPARITY)
		{
			pparityptr=curstrsetptr;
		}
		if(isparity==QPARITY)
		{
			qparityptr=curstrsetptr;
		}
		curstrsetptr++;
	}

	resTF=cauchy_good_general_coding_bitmatrix(matrix);
	if(resTF==FALSE)
	{
		show_error("cauchy","API_RAID6","bit matrix can't create");
		return FALSE;
	}
	resTF=UPDATE_API_bitmatrix_update_RAID6(chunkpositionblockgroupptr,strsetblockgroupptr,pparityptr,qparityptr,
											parityblockgroupptr,matrix);
	if(resTF==FALSE)
	{
		show_error("cauchy","API_RAID6","update can't execute");
		return FALSE;
	}
	free(strsetblockgroupptr);
	free(strsetblockptr);
	free(matrix);
	free(mat);
	return TRUE;
}
int cauchy_update_API(CHUNKPOSITIONBLOCKGROUP *chunkpositionblockgroupptr,CHUNKBLOCKGROUP *parityblockgroupptr,STRIP *stripptr)
{

	return TRUE;
}

//经典RS解码接口
int cauchy_decoder_API_strip(STRIP *stripptr,ERRORPATTERN *errormatrix)
{
	return TRUE;
}
//经典RS解码接口
int cauchy_decoder_API_chunk(CHUNK *chunkptr,ERRORPATTERN *errormatrix)
{
	return TRUE;
}
//随机请求大小接口
int cauchy_API_Context(AONTSHAREGROUP *sharegroup,CODEPARA *codeptr,BYTE *context,int strlen)
{
	//计算相应的数据
	BITMATRIX *matrix;
	AONTSHARE *shareptr,*curshareptr;
	UINT32 *mat; 
	int resTF;
	int i=0,sharesize;
	int totnum,datanum,codenum,w,cols,rows;
	BYTE *conptr,*cursouptr;
	totnum=codeptr->totnum;
	datanum=codeptr->datanum;
	codenum=codeptr->codenum;
	sharesize=strlen/datanum;
	w=codeptr->w;
	
	
	sharegroup->groupsize=totnum;//根据总数来决定
	matrix=talloc(BITMATRIX,1);
	if(matrix==NULL)
	{
		show_error("cauchy","API_Context","No memory");
		return FALSE;
	}
	matrix->w=w;
	rows=w*codenum;
	cols=w*datanum;
	mat=talloc(UINT32,rows*cols);
	matrix->xm=rows;
	matrix->ym=cols;

	if(mat==NULL)
	{
		show_error("cauchy","API_Context","No memory");
		return FALSE;
	}
	matrix->matrix=mat;
	resTF=cauchy_good_general_coding_bitmatrix(matrix);
	if(resTF==FALSE)
	{
		show_error("cauchy","API_RAID6","bit matrix can't create");
		return FALSE;
	}
	//切片
	cursouptr=context;
	shareptr=sharegroup->sharegroupptr;
	curshareptr=shareptr;
	for(i=0;i<datanum;i++)
	{
		//直接内存copy
		conptr=curshareptr->content;
		mcpy(conptr,cursouptr,char,sharesize);
		curshareptr->codeptr=codeptr;
		curshareptr++;
		cursouptr=cursouptr+sharesize;
	}
	//开始进行编解码操作
	resTF=matrix_matrix_calculate_share(matrix,codeptr,sharegroup,sharesize);
	if(resTF==FALSE)
	{
		show_error("cauchy","API_RAID6","RS code cannot calculate");
		return FALSE;
	}
	return TRUE;
}


static int cbest_2[3] = { 1, 2, 3 };
static int cbest_3[7] = { 1, 2, 5, 4, 7, 3, 6 };

static int cbest_4[15] = { 1, 2, 9, 4, 8, 13, 3, 6, 12, 5, 11, 15, 10, 14, 7 };

static int cbest_5[31] = { 1, 2, 18, 4, 9, 8, 22, 16, 3, 11, 19, 5, 10, 6, 20, 27, 13, 23, 26, 12,
    17, 25, 24, 31, 30, 7, 15, 21, 29, 14, 28 };

static int cbest_6[63] = { 1, 2, 33, 4, 8, 49, 16, 32, 57, 3, 6, 12, 24, 48, 5, 35, 9, 37, 10, 17,
    41, 51, 56, 61, 18, 28, 53, 14, 20, 34, 7, 13, 25, 36, 59, 26, 39, 40, 45, 50, 60, 52, 63,
    11, 30, 55, 19, 22, 29, 43, 58, 15, 21, 38, 44, 47, 62, 27, 54, 42, 31, 23, 46 };

static int cbest_7[127] = { 1, 2, 68, 4, 34, 8, 17, 16, 76, 32, 38, 3, 64, 69, 5, 19, 35, 70, 6, 9,
    18, 102, 10, 36, 85, 12, 21, 42, 51, 72, 77, 84, 20, 25, 33, 50, 78, 98, 24, 39, 49, 100, 110
   , 48, 65, 93, 40, 66, 71, 92, 7, 46, 55, 87, 96, 103, 106, 11, 23, 37, 54, 81, 86, 108, 13,
    22, 27, 43, 53, 73, 80, 14, 26, 52, 74, 79, 99, 119, 44, 95, 101, 104, 111, 118, 29, 59, 89,
    94, 117, 28, 41, 58, 67, 88, 115, 116, 47, 57, 83, 97, 107, 114, 127, 56, 82, 109, 113, 126,
    112, 125, 15, 63, 75, 123, 124, 31, 45, 62, 91, 105, 122, 30, 61, 90, 121, 60, 120 };

static int cbest_8[255] = { 1, 2, 142, 4, 71, 8, 70, 173, 3, 35, 143, 16, 17, 67, 134, 140, 172, 6, 34
   , 69, 201, 216, 5, 33, 86, 12, 65, 138, 158, 159, 175, 10, 32, 43, 66, 108, 130, 193, 234, 9,
    24, 25, 50, 68, 79, 100, 132, 174, 200, 217, 20, 21, 42, 48, 87, 169, 41, 54, 64, 84, 96, 117
   , 154, 155, 165, 226, 77, 82, 135, 136, 141, 168, 192, 218, 238, 7, 18, 19, 39, 40, 78, 113,
    116, 128, 164, 180, 195, 205, 220, 232, 14, 26, 27, 58, 109, 156, 157, 203, 235, 13, 28, 29, 38
   , 51, 56, 75, 85, 90, 101, 110, 112, 139, 171, 11, 37, 49, 52, 76, 83, 102, 119, 131, 150, 151
   , 167, 182, 184, 188, 197, 219, 224, 45, 55, 80, 94, 97, 133, 170, 194, 204, 221, 227, 236, 36,
    47, 73, 92, 98, 104, 118, 152, 153, 166, 202, 207, 239, 251, 22, 23, 44, 74, 91, 148, 149, 161
   , 181, 190, 233, 46, 59, 88, 137, 146, 147, 163, 196, 208, 212, 222, 250, 57, 81, 95, 106, 111,
    129, 160, 176, 199, 243, 249, 15, 53, 72, 93, 103, 115, 125, 162, 183, 185, 189, 206, 225, 255,
    186, 210, 230, 237, 242, 248, 30, 31, 62, 89, 99, 105, 114, 121, 124, 178, 209, 213, 223, 228,
    241, 254, 60, 191, 198, 247, 120, 240, 107, 127, 144, 145, 177, 211, 214, 246, 245, 123, 126,
    187, 231, 253, 63, 179, 229, 244, 61, 122, 215, 252 };

static int cbest_9[511] = { 1, 2, 264, 4, 132, 8, 66, 16, 33, 32, 280, 64, 140, 128, 3, 70, 265, 5,
    133, 256, 266, 6, 9, 35, 67, 134, 268, 396, 10, 17, 34, 330, 12, 18, 68, 198, 297, 20, 37, 74
   , 136, 148, 165, 281, 296, 24, 36, 41, 65, 82, 99, 164, 272, 282, 388, 40, 49, 98, 141, 194,
    284, 328, 412, 48, 97, 129, 142, 196, 346, 71, 72, 96, 130, 313, 392, 80, 206, 257, 267, 312,
    334, 7, 135, 156, 173, 192, 258, 269, 397, 404, 11, 78, 144, 161, 172, 260, 270, 299, 331, 344,
    398, 13, 19, 39, 69, 86, 103, 160, 167, 199, 202, 298, 322, 384, 14, 21, 38, 43, 75, 102, 137,
    149, 166, 204, 289, 332, 408, 462, 22, 25, 42, 51, 83, 101, 138, 150, 273, 283, 288, 301, 350,
    389, 429, 26, 50, 76, 100, 195, 274, 285, 300, 329, 363, 390, 413, 428, 28, 45, 84, 143, 197,
    200, 214, 231, 276, 286, 315, 320, 347, 362, 414, 458, 44, 53, 73, 90, 107, 131, 152, 169, 181,
    230, 314, 338, 361, 393, 400, 454, 460, 52, 57, 81, 106, 115, 168, 175, 180, 207, 229, 305, 335
   , 348, 360, 394, 421, 478, 56, 105, 114, 157, 163, 174, 193, 210, 227, 228, 259, 304, 317, 326,
    405, 420, 445, 79, 104, 113, 145, 158, 162, 212, 226, 261, 271, 316, 345, 379, 399, 406, 444,
    450, 456, 87, 88, 112, 146, 203, 225, 262, 291, 323, 336, 378, 385, 425, 452, 474, 15, 205, 222
   , 224, 239, 290, 303, 333, 367, 377, 386, 409, 424, 431, 463, 470, 476, 23, 139, 151, 189, 208,
    238, 302, 324, 351, 366, 376, 410, 430, 437, 27, 47, 77, 94, 111, 177, 188, 237, 275, 293, 342,
    365, 391, 436, 448, 29, 46, 55, 85, 110, 119, 171, 176, 183, 201, 215, 218, 235, 236, 277, 287,
    292, 321, 355, 364, 415, 417, 459, 466, 472, 30, 54, 59, 91, 109, 118, 153, 170, 182, 220, 234,
    278, 307, 339, 354, 401, 416, 423, 441, 455, 461, 468, 495, 58, 108, 117, 154, 233, 306, 319,
    349, 353, 383, 395, 402, 422, 440, 447, 479, 494, 92, 116, 211, 232, 318, 327, 340, 352, 382,
    446, 493, 61, 159, 213, 216, 247, 309, 381, 407, 427, 451, 457, 464, 491, 492, 60, 89, 123, 147
   , 185, 246, 263, 308, 337, 371, 380, 426, 433, 453, 475, 487, 490, 122, 184, 191, 223, 245, 370,
    387, 432, 439, 471, 477, 486, 489, 511, 121, 179, 190, 209, 243, 244, 295, 325, 359, 369, 411,
    438, 485, 488, 510, 95, 120, 178, 242, 294, 343, 358, 368, 419, 449, 483, 484, 509, 219, 241,
    357, 418, 443, 467, 473, 482, 507, 508, 31, 221, 240, 255, 279, 356, 442, 469, 481, 503, 506,
    155, 254, 403, 480, 502, 505, 63, 93, 127, 253, 311, 341, 375, 501, 504, 62, 126, 187, 217, 251
   , 252, 310, 374, 435, 465, 499, 500, 125, 186, 250, 373, 434, 498, 124, 249, 372, 497, 248, 496
    };

static int cbest_10[1023] = { 1, 2, 516, 4, 258, 8, 129, 16, 32, 580, 64, 128, 290, 145, 256, 3, 512,
    517, 5, 259, 518, 588, 6, 9, 18, 36, 72, 144, 774, 10, 17, 131, 262, 288, 524, 645, 12, 33,
    133, 266, 294, 387, 532, 576, 581, 20, 34, 65, 137, 274, 548, 582, 24, 66, 291, 838, 40, 68,
    130, 147, 161, 322, 644, 709, 806, 48, 132, 193, 257, 386, 596, 80, 136, 298, 419, 612, 661, 772
   , 96, 149, 260, 272, 306, 403, 513, 146, 153, 160, 264, 292, 385, 514, 519, 544, 584, 589, 708,
    870, 7, 19, 37, 73, 192, 354, 590, 770, 775, 11, 38, 74, 177, 263, 289, 418, 520, 525, 534, 641
   , 660, 725, 802, 836, 846, 13, 22, 76, 148, 209, 267, 295, 320, 330, 402, 526, 528, 533, 577,
    647, 717, 804, 14, 21, 26, 35, 44, 135, 152, 165, 201, 275, 304, 384, 401, 435, 549, 578, 583,
    604, 608, 782, 903, 25, 52, 67, 88, 139, 270, 296, 391, 417, 550, 620, 653, 790, 834, 839, 41,
    50, 69, 104, 141, 176, 278, 302, 323, 395, 423, 540, 598, 640, 705, 724, 807, 866, 28, 42, 49,
    70, 82, 100, 163, 208, 282, 310, 556, 592, 597, 646, 663, 677, 711, 716, 868, 878, 81, 134, 151
   , 164, 195, 200, 299, 326, 352, 362, 400, 434, 564, 613, 657, 768, 773, 902, 967, 97, 138, 155,
    169, 197, 261, 273, 307, 358, 390, 416, 433, 451, 614, 652, 733, 800, 814, 844, 854, 935, 56, 84
   , 98, 140, 181, 217, 265, 293, 328, 338, 394, 422, 515, 545, 585, 704, 788, 822, 871, 919, 162,
    179, 276, 355, 407, 427, 546, 586, 591, 616, 662, 669, 676, 710, 727, 741, 771, 780, 901, 39, 75
   , 150, 157, 194, 211, 225, 268, 280, 308, 314, 389, 411, 439, 521, 530, 535, 628, 656, 721, 803,
    832, 837, 842, 847, 966, 23, 77, 112, 154, 168, 196, 300, 321, 331, 393, 421, 432, 450, 522, 527
   , 529, 552, 606, 643, 673, 693, 713, 732, 805, 864, 874, 934, 999, 15, 27, 45, 54, 78, 90, 108,
    180, 216, 305, 483, 560, 579, 600, 605, 609, 719, 778, 783, 852, 876, 886, 899, 918, 983, 46, 53
   , 89, 167, 178, 185, 203, 213, 271, 297, 324, 334, 336, 360, 370, 406, 426, 467, 542, 551, 610,
    621, 649, 668, 726, 740, 786, 791, 810, 820, 835, 900, 917, 931, 951, 965, 975, 30, 51, 105, 156
   , 205, 210, 224, 279, 303, 356, 366, 388, 405, 410, 438, 449, 459, 536, 541, 594, 599, 622, 655,
    720, 812, 818, 862, 867, 933, 29, 43, 71, 83, 92, 101, 106, 143, 173, 283, 311, 312, 346, 392,
    409, 420, 437, 443, 557, 566, 593, 642, 659, 672, 692, 707, 712, 737, 757, 869, 879, 911, 998,
    60, 102, 241, 327, 353, 363, 399, 425, 482, 558, 565, 624, 679, 718, 735, 749, 769, 798, 898,
    963, 982, 58, 86, 166, 183, 184, 202, 212, 219, 233, 286, 359, 431, 466, 615, 636, 648, 689, 729
   , 801, 815, 840, 845, 850, 855, 884, 916, 930, 950, 964, 974, 981, 995, 1015, 57, 85, 99, 120,
    171, 199, 204, 229, 318, 329, 339, 368, 404, 448, 458, 465, 499, 654, 671, 685, 784, 789, 823,
    872, 882, 915, 932, 949, 997, 1007, 116, 142, 159, 172, 277, 408, 436, 442, 455, 481, 491, 547,
    572, 587, 617, 630, 658, 665, 706, 723, 736, 756, 776, 781, 816, 860, 894, 897, 910, 947, 991,
    114, 221, 240, 269, 281, 309, 315, 332, 342, 344, 378, 398, 424, 441, 475, 487, 531, 618, 629,
    678, 695, 734, 743, 748, 808, 833, 843, 929, 943, 962, 973, 113, 182, 189, 218, 227, 232, 301,
    364, 374, 430, 457, 523, 553, 562, 602, 607, 688, 728, 753, 796, 830, 865, 875, 927, 980, 994,
    1014, 55, 79, 91, 109, 170, 187, 198, 215, 228, 284, 415, 464, 498, 554, 561, 601, 670, 675, 684
   , 715, 745, 765, 779, 848, 853, 877, 887, 909, 914, 948, 979, 996, 1006, 1013, 47, 110, 158, 249
   , 316, 325, 335, 337, 361, 371, 397, 447, 454, 480, 490, 497, 538, 543, 611, 632, 664, 722, 787,
    811, 821, 880, 896, 913, 946, 961, 971, 990, 1011, 31, 94, 220, 245, 357, 367, 429, 440, 474,
    486, 537, 595, 623, 651, 681, 694, 701, 742, 759, 813, 819, 858, 863, 892, 928, 942, 945, 972,
    989, 993, 1003, 1023, 62, 93, 107, 188, 207, 226, 237, 243, 313, 340, 347, 376, 456, 471, 473,
    507, 567, 568, 626, 752, 890, 907, 926, 1005, 61, 103, 124, 175, 186, 214, 372, 414, 453, 463,
    489, 503, 559, 625, 638, 674, 691, 714, 731, 739, 744, 764, 794, 799, 828, 908, 925, 939, 959,
    978, 1012, 59, 87, 122, 248, 287, 350, 396, 413, 446, 485, 495, 496, 637, 751, 826, 841, 851,
    885, 912, 941, 960, 970, 977, 1010, 118, 121, 235, 244, 319, 369, 382, 428, 445, 574, 650, 667,
    680, 700, 758, 761, 785, 873, 883, 944, 988, 992, 1002, 1009, 1022, 117, 206, 223, 231, 236, 242
   , 470, 472, 506, 573, 631, 687, 777, 817, 856, 861, 895, 906, 987, 1004, 1021, 115, 174, 191, 333
   , 343, 345, 379, 452, 462, 469, 488, 502, 505, 619, 690, 697, 730, 738, 755, 809, 888, 924, 938,
    958, 969, 1019, 253, 365, 375, 412, 484, 494, 501, 563, 603, 750, 767, 792, 797, 831, 923, 940,
    957, 976, 1001, 234, 251, 285, 348, 444, 479, 555, 634, 666, 760, 824, 849, 905, 955, 1008, 111,
    222, 230, 247, 317, 380, 461, 511, 539, 633, 686, 703, 747, 881, 937, 986, 1020, 95, 190, 468,
    493, 504, 570, 696, 754, 859, 893, 968, 985, 1018, 63, 126, 252, 341, 377, 500, 569, 627, 683,
    766, 891, 922, 956, 1000, 1017, 125, 239, 250, 373, 478, 639, 795, 829, 904, 921, 954, 123, 246,
    351, 460, 477, 510, 702, 746, 763, 827, 936, 953, 119, 383, 492, 509, 575, 984, 682, 699, 857,
    1016, 238, 255, 889, 920, 476, 762, 793, 952, 349, 508, 635, 825, 381, 698, 254, 571, 127 };

static int cbest_11[1023] = { 1,
    2, 1026, 4, 513, 8, 16, 1282, 32, 64, 641, 128, 256, 512, 1346, 1024, 3, 673, 1027, 5, 10, 20, 40, 80, 160, 320,
    640, 6, 9, 515, 1030, 1280, 1539, 17, 517, 1034, 1283, 12, 18, 33, 521, 1042, 1362, 34, 65, 529, 1058, 1286, 1795,
    24, 36, 66, 129, 545, 643, 1090, 1290, 1667, 68, 130, 257, 577, 645, 672, 1154, 1298, 1344, 48, 72, 132, 258, 336,
    649, 681, 1314, 1347, 136, 168, 260, 514, 657, 769, 1538, 1923, 84, 96, 144, 264, 516, 1025, 1350, 1410, 1859, 42,
    272, 520, 705, 1032, 1354, 11, 21, 41, 81, 161, 192, 288, 321, 528, 675, 1028, 1537, 1699, 1794, 7, 22, 82, 162,
    322, 544, 642, 677, 897, 1031, 1046, 1066, 1106, 1186, 1281, 1366, 1378, 1666, 14, 44, 164, 324, 384, 523, 533,
    553, 576, 593, 644, 833, 1035, 1040, 1288, 1360, 1987, 13, 19, 28, 88, 328, 519, 648, 680, 689, 1043, 1056, 1284,
    1363, 1474, 1543, 1793, 1955, 26, 35, 56, 176, 656, 768, 1038, 1059, 1088, 1287, 1302, 1322, 1442, 1547, 1665,
    1922, 25, 37, 52, 67, 112, 340, 352, 525, 531, 737, 1091, 1152, 1291, 1296, 1555, 1858, 1875, 38, 69, 74, 104, 131,
    224, 547, 651, 661, 683, 704, 721, 961, 1050, 1062, 1155, 1299, 1312, 1345, 1370, 1571, 1799, 49, 70, 73, 133, 138,
    148, 170, 208, 259, 337, 448, 537, 549, 579, 647, 674, 929, 1094, 1294, 1315, 1352, 1536, 1603, 1671, 1698, 1803,
    1921, 50, 134, 137, 169, 261, 266, 276, 296, 338, 416, 581, 676, 896, 1074, 1098, 1158, 1348, 1394, 1408, 1675,
    1707, 1811, 1857, 2019, 76, 85, 97, 145, 262, 265, 522, 532, 552, 561, 585, 592, 653, 659, 685, 771, 832, 849,
    1064, 1162, 1194, 1306, 1318, 1351, 1386, 1411, 1506, 1683, 1827, 1986, 2003, 43, 86, 98, 140, 146, 172, 273, 344,
    518, 688, 773, 1033, 1110, 1122, 1170, 1355, 1490, 1542, 1697, 1792, 1927, 1954, 100, 193, 268, 274, 289, 597, 609,
    665, 697, 707, 777, 1029, 1044, 1104, 1184, 1330, 1364, 1376, 1414, 1546, 1664, 1731, 1863, 1931, 1963, 23, 46, 83,
    92, 152, 163, 184, 194, 290, 323, 368, 524, 530, 555, 693, 709, 736, 753, 785, 993, 1036, 1047, 1067, 1107, 1187,
    1218, 1320, 1358, 1367, 1379, 1418, 1450, 1545, 1554, 1867, 1874, 1939, 1985, 15, 30, 45, 60, 90, 120, 165, 180,
    196, 240, 280, 292, 325, 330, 360, 385, 480, 546, 650, 660, 679, 682, 713, 720, 745, 801, 899, 960, 977, 1041,
    1289, 1361, 1426, 1472, 1541, 1570, 1703, 1798, 1953, 29, 58, 89, 116, 166, 200, 232, 326, 329, 386, 464, 535, 536,
    548, 578, 595, 646, 835, 901, 928, 1048, 1057, 1070, 1190, 1285, 1300, 1368, 1382, 1440, 1475, 1559, 1579, 1602,
    1619, 1670, 1802, 1879, 1891, 1920, 27, 57, 177, 304, 388, 527, 557, 580, 691, 725, 837, 905, 937, 1039, 1054,
    1089, 1114, 1292, 1303, 1323, 1374, 1443, 1553, 1674, 1706, 1715, 1801, 1810, 1856, 1873, 1991, 2018, 2035, 53,
    106, 113, 178, 212, 332, 341, 353, 392, 424, 541, 560, 584, 601, 652, 658, 684, 770, 841, 848, 913, 1060, 1082,
    1096, 1153, 1202, 1297, 1402, 1478, 1522, 1569, 1673, 1682, 1705, 1797, 1826, 1959, 1995, 2002, 2027, 39, 54, 75,
    105, 114, 225, 342, 354, 400, 539, 569, 739, 772, 1051, 1063, 1078, 1092, 1138, 1160, 1192, 1304, 1313, 1326, 1371,
    1384, 1398, 1446, 1482, 1514, 1551, 1601, 1669, 1696, 1763, 1815, 1835, 1926, 71, 139, 149, 171, 209, 226, 298,
    356, 449, 565, 596, 608, 625, 663, 664, 696, 706, 723, 741, 776, 853, 865, 963, 1072, 1095, 1130, 1156, 1250, 1295,
    1310, 1353, 1392, 1687, 1730, 1747, 1809, 1862, 1930, 1962, 1971, 2007, 2017, 51, 78, 108, 135, 150, 210, 228, 267,
    277, 297, 339, 348, 417, 450, 551, 554, 587, 617, 655, 687, 692, 708, 752, 784, 931, 965, 992, 1009, 1075, 1099,
    1159, 1174, 1234, 1316, 1338, 1349, 1395, 1409, 1458, 1494, 1504, 1544, 1563, 1575, 1681, 1825, 1866, 1883, 1929,
    1938, 1961, 1984, 2001, 77, 142, 174, 263, 278, 346, 376, 418, 452, 496, 583, 669, 678, 701, 712, 729, 744, 761,
    800, 898, 933, 969, 976, 1001, 1065, 1108, 1120, 1163, 1168, 1195, 1307, 1319, 1334, 1356, 1387, 1416, 1448, 1488,
    1507, 1540, 1607, 1702, 1807, 1865, 1925, 1952, 87, 99, 141, 147, 156, 173, 188, 216, 248, 270, 300, 345, 372, 420,
    456, 488, 534, 563, 594, 667, 699, 757, 779, 789, 809, 834, 851, 900, 1102, 1111, 1123, 1171, 1328, 1412, 1491,
    1558, 1578, 1587, 1611, 1618, 1679, 1711, 1729, 1861, 1878, 1890, 1907, 1943, 2023, 94, 101, 124, 154, 186, 244,
    269, 275, 284, 526, 556, 589, 690, 724, 775, 836, 904, 936, 945, 981, 1045, 1068, 1105, 1166, 1185, 1198, 1216,
    1331, 1365, 1377, 1390, 1415, 1430, 1510, 1552, 1577, 1714, 1800, 1819, 1831, 1872, 1899, 1937, 1990, 2034, 47, 62,
    93, 102, 122, 153, 185, 195, 282, 291, 312, 362, 369, 432, 468, 540, 599, 600, 611, 715, 747, 840, 857, 912, 1037,
    1052, 1112, 1126, 1219, 1321, 1359, 1372, 1419, 1424, 1451, 1568, 1623, 1635, 1672, 1691, 1701, 1704, 1723, 1796,
    1958, 1994, 2011, 2026, 2043, 31, 61, 91, 121, 181, 197, 202, 234, 241, 281, 293, 308, 331, 361, 370, 481, 538,
    568, 613, 695, 711, 738, 755, 781, 787, 995, 1080, 1118, 1178, 1188, 1210, 1380, 1400, 1427, 1473, 1498, 1530,
    1550, 1557, 1600, 1617, 1668, 1719, 1735, 1762, 1779, 1814, 1834, 1843, 1877, 1889, 1935, 1967, 1993, 2025, 2039,
    59, 117, 167, 182, 198, 201, 233, 242, 294, 327, 387, 465, 482, 559, 564, 605, 624, 662, 722, 740, 803, 852, 864,
    881, 907, 917, 939, 962, 979, 997, 1049, 1071, 1086, 1146, 1191, 1206, 1222, 1266, 1301, 1324, 1369, 1383, 1406,
    1422, 1441, 1454, 1480, 1512, 1526, 1549, 1686, 1713, 1739, 1746, 1771, 1808, 1833, 1871, 1970, 1989, 2006, 2016,
    2033, 118, 305, 334, 364, 389, 394, 404, 426, 466, 484, 543, 550, 573, 586, 603, 616, 633, 654, 686, 717, 749, 793,
    805, 843, 873, 903, 930, 964, 1008, 1055, 1115, 1128, 1142, 1200, 1226, 1258, 1293, 1308, 1375, 1476, 1520, 1562,
    1574, 1680, 1824 };