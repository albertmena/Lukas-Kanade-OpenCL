//--------------------------------------
//IMFILTER X
__kernel void imfilterX_cl(
	__global float * im,
	__global float * im_out,
	__constant float * filt,
	__local float  * tile,
	const uint CONV_SIZE, 
	const uint width,
	const uint height,
	int z)
{
    int ii, jj;
	int j  = get_global_id(0);
	int i  = get_global_id(1);
	int tx = get_local_id(0);
	int ty = get_local_id(1);
	
	int work_size =  get_local_size(0);//Returns the number of local work-items.
	int CONV_SIZE2 = (CONV_SIZE-1)/2;
	int tile_dim  = work_size+CONV_SIZE2*2;
	/* filter copy */
	//if (ty==0 && tx<CONV_SIZE)
	//	filt_loc[tx] = filt[tx];

	tile[ty*tile_dim+tx+CONV_SIZE2] = im[i*width+j];//copio la parte central
	
	if(tx>=0 && tx<CONV_SIZE2){//margen izquierdo
		if (j>CONV_SIZE2)
			tile[ty*tile_dim+tx] = im[i*width+j-CONV_SIZE2];
		else
			tile[ty*tile_dim+tx] = 0.0;
	}
	if (tx>=work_size-CONV_SIZE2 && tx<work_size){//margen derecho
		if (j<width-CONV_SIZE2)
			tile[ty*tile_dim+CONV_SIZE2+tx] = im[i*width+j+CONV_SIZE2];
		else
			tile[ty*tile_dim+CONV_SIZE2+tx] = 0.0;
	}
	barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
	//esperamos a que se copien todos
	
	if(i<height && j>=CONV_SIZE2 && j<width-CONV_SIZE2)
	{
		im_out[i*width+j] = 0.0;// iniciamos a cero la salida

		for (jj=-CONV_SIZE2;jj<=CONV_SIZE2; jj++)
			im_out[i*width+j] += filt/*_loc*/[jj+CONV_SIZE2]*tile[ty*tile_dim + (jj+tx)];//+ (jj+tx+CONV_SIZE2)
	}
}			
			
//--------------------------------------
//IMFILTER Y
__kernel void  imfilterY_cl(
	__global float * im,
	__global float * im_out,
	__constant float * filt,
	__local float  * tile,
	const uint CONV_SIZE, 
	const uint width,
	const uint height,
	int z)
{
   	int ii, jj;
	int j  = get_global_id(0);
	int i  = get_global_id(1);
	int tx = get_local_id(0);
	int ty = get_local_id(1);

	int work_size =  get_local_size(0);
	int CONV_SIZE2 = (CONV_SIZE-1)/2;
	int tile_dim  = work_size+CONV_SIZE2*2;

	/* filter copy */
	//if (ty==0 && tx<CONV_SIZE)
	//	filt_loc[tx] = filt[tx];

	tile[(ty+CONV_SIZE2)*tile_dim+tx] = im[i*width+j];//copio la parte central

	if(ty>=0 && ty<CONV_SIZE2){//margen superior
		if (i>CONV_SIZE2)
			tile[ty*tile_dim+tx] = im[(i-CONV_SIZE2)*width+j];
		else
			tile[ty*tile_dim+tx] = 0.0;
	}
	if (ty>=work_size-CONV_SIZE2 && ty<work_size){//margen inferior
		if (i < height-CONV_SIZE2)
			tile[ty*tile_dim+CONV_SIZE2+tx] = im[(i+CONV_SIZE2)*width+j];
		else
			tile[ty*tile_dim+CONV_SIZE2+tx] = 0.0;
	}
	barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
	//esperamos a que se copien todos

	if(j<width && i>=CONV_SIZE2 && i<height-CONV_SIZE2)
	{
		im_out[i*width+j] = 0.0;// iniciamos a cero la salida

		for (jj=-CONV_SIZE2;jj<=CONV_SIZE2; jj++)
			im_out[i*width+j] += filt/*_loc*/[jj+CONV_SIZE2]*tile[(ty+jj)*tile_dim + tx];
	}
}	
//--------------------------------------
//SUBST
__kernel void subst_cl(
	__global float *M1,
	__global float *It,
	int NR,
	int NC,
	int z)				
{
	int i, j;
	i = get_global_id(0); 
	j = get_global_id(1);
	if (i<NR && j<NC)			
		It[i*NC+j] = M1[NR*NC*z+i*NC+j] - M1[NR*NC*(z+1)+i*NC+j];
}
//--------------------------------------
//SUBST2
__kernel void sub2_cl(
	__global float *M1,
	__global float *M2,
	__global float *R,
	int NR,
	int NC)				
{
	int i, j;
	i = get_global_id(0); 
	j = get_global_id(1);
	if (i<NR && j<NC)			
		R[i*NC+j] = M1[i*NC+j] - M2[i*NC+j];
}

//--------------------------------------
//SUM
__kernel void sum_cl(
	__global float *M1,
	__global float *M2,
	__global float *S,
	int NR,
	int NC)				
{
	int i, j;
	i = get_global_id(0); 
	j = get_global_id(1);
	if (i<NR && j<NC)			
		S[i*NC+j] = M1[i*NC+j] + M1[i*NC+j];
}

//--------------------------------------
//MULT
__kernel void mult_cl(
	__global float *M1,
	__global float *M2,
	__global float *R,
	int NR,
	int NC)			
{
	int i, j;
	i = get_global_id(0); 
	j = get_global_id(1);
	if (i<NR && j<NC){
		R[i*NC+j] = M1[i*NC+j] * M2[i*NC+j];

	}
}

//--------------------------------------
//MULTESCALAR
__kernel void multEsc_cl(
	__global float *M1,
	float E,
	__global float *R,
	int NR,
	int NC)			
{
	int i, j;
	i = get_global_id(0); 
	j = get_global_id(1);
	if (i<NR && j<NC){
		R[i*NC+j] = M1[i*NC+j] * E;
	}
}

//--------------------------------------
//DIVESCALAR con control de division por cero
__kernel void divEsc_cl(
	__global float *M1,
	float E,
	__global float *R,
	int NR,
	int NC)			
{
	int i, j;
	i = get_global_id(0); 
	j = get_global_id(1);
	if (i<NR && j<NC){
		if(M1[i*NC+j]!=0){
		R[i*NC+j] = E / M1[i*NC+j];
		}
		else{
		R[i*NC+j]=0;
		}
	}
}

//--------------------------------------
//IMFILTERS
/*__kernel void imfilterS_cl(
__global float *A, //frame input
__global float *R,//frame output
int NR,
int NC,
int BX)

	{
	int i, j, k, l, difx, dify;
	difx = BX/2;
	dify = BX/2;

		for (i=0; i<NR; i++)
		{
			for (j=0; j<NC; j++)
			{
				float tmp = 0.0;
				for (k=-dify; k<=dify; k++)
				{
					for (l=-difx; l<=difx; l++)
					{
						if (i+k>=0 && j+l>=0 && i+k<NR && j+l<NC)
						tmp += A[(i+k)*NC+(j+l)];
						}
				}
				R[i*NC+j] = tmp;
			}
		}
	}
*/
//--------------------------------------
//IMFILTERS
__kernel void imfilterS_cl(
__global float *A, //frame input
__global float *R,//frame output
int NR,
int NC,
int BX)//must be odd:impar
	{
	int i; int j; int k, l, difx, dify;
	i = get_global_id(0); 
	j = get_global_id(1);
	difx = (BX-1)/2;
	dify = (BX-1)/2;

		float tmp = 0.0;
		for (k=-dify; k<=dify; k++)
		{
			for (l=-difx; l<=difx; l++)
			{
				if (i+l>=0 && j+k>=0 && i+l<NR && j+k<NC)
				tmp += A[(j+k)*NC+(i+l)];
			}
		}
		R[j*NC+i] = tmp;
	}

