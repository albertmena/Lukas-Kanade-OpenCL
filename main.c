	/* Lucas Kanade  *
argv[1] =path_movie== /home/alberto/Dropbox/Master_E/TFM/LK-C_OpenACC/divergingtree_150x150_40frs.bin
argv[2] =image size x= 150
argv[3] =image size y= 150
argv[4] =nframes= 40
argv[5] =filtro solo permitido 3,5,7,9= 3
argv[6] =block size= 10


*Ejemplo geforce
 gcc -g LK_1_7.c -Wall -std=c99 -I /usr/local/cuda-7.5/include -L /opt/intel/opencl-1.2-4.5.0.8/lib64 -lOpenCL
*Ejemplo @bujaruelo
 gcc -g LK_1_7.c -Wall -std=c99 -I /opt/intel/opencl-1.2-3.2.1.16712/include -L /opt/intel/opencl-1.2-3.2.1.16712/lib64 -lOpenCL
*Ejemplo compilacion Inspiron PC:
gcc -g LK_1_7.c -Wall -std=c99 -I/home/alberto/AMDAPPSDK-3.0/include -L/home/alberto/AMDAPPSDK-3.0/lib/x86_64 -lOpenCL	
gcc -g LK_1_7.c -Wall -std=c99 -I/opt/AMDAPPSDK-3.0/include -L/opt/AMDAPPSDK-3.0/lib/x86_64 -lOpenCL	
*Ejemplo compilacion TENTE:
gcc -g LK_1_7.c -Wall -std=c99 -I/home/tente/AMDAPPSDK-2.9-1/include -L/home/tente/AMDAPPSDK-2.9-1/lib/x86 -lOpenCL	

*Ejemplo ejecucion devarm:
./a.out /home/albermena/Documentos/opencl/LK/estimulos/divergingtree_150x150_40frs.bin  150 150 2 5 9
*Ejemplo ejecucion INSPIRON:
./a.out /home/alberto/Documentos/LK-03-06-2015/estimulos/divergingtree_150x150_40frs.bin  150 150 40 5 3
*Ejemplo ejecucion TENTE:
./a.out /home/tente/Documentos/LK/Estimulos/divergingtree_150x150_40frs.bin  150 150 40 5 3
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <sys/resource.h>
#include <CL/cl.h>
#include <sys/time.h> //para el gettimeofday()
#include <inttypes.h>// para definicion de algunos tipos de integers
//—————————————————————————————————————————————————
//DEFINICION ENTORNO OPENCL-CONTROL ERRORES
///----------DEVICE INFO-------------------
 
int output_device_info(cl_device_id device_id, int max_WG)
{
 
    int err;                            // error code returned from OpenCL calls
 
    cl_device_type device_type;         // Parameter defining the type of the compute device
    cl_uint comp_units;                 // the max number of compute units on a device
    cl_uint num_max_args;                 // the max number ofarguments in a device
    cl_char vendor_name[1024] = {0};    // string to hold vendor name for compute device
    cl_char device_version[1024] = {0};    // string to hold device version compute device
    cl_char device_name[1024] = {0};    // string to hold name of compute device
    cl_uint          max_work_itm_dims;
    size_t           max_wrkgrp_size;
    size_t          *max_loc_size;
 

    err = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), &device_name, NULL);
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to access device name!\n");
        return EXIT_FAILURE;
		}
    printf(" \n //Device is %s \n",device_name);
 
 
    err = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to access device type information!\n");
        return EXIT_FAILURE;
		}
    if(device_type  == CL_DEVICE_TYPE_GPU)
       printf(" //GPU from ");
    else if (device_type == CL_DEVICE_TYPE_CPU)
       printf(" //CPU from ");
    else
       printf("\n non  CPU or GPU processor from ");
 
      err = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(vendor_name), &vendor_name, NULL);
 
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to access device vendor name!\n");
        return EXIT_FAILURE;
		}
    printf("vendor %s ",vendor_name);
    
     err = clGetDeviceInfo(device_id, CL_DEVICE_VERSION, sizeof(device_version), &device_version, NULL);
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to access device version name!\n");
        return EXIT_FAILURE;
		}
    printf("device version  %s \n",device_version);
 
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &comp_units, NULL);
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to access device number of compute units !\n");
        return EXIT_FAILURE;
		}
    printf(" //With a max of %d compute units ",comp_units);
 
 
    err = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint),&max_work_itm_dims, NULL);
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS)!\n");
        return EXIT_FAILURE;
		}
 
    max_loc_size = (size_t*)malloc(max_work_itm_dims * sizeof(size_t));
    if(max_loc_size == NULL){
       printf(" malloc failed\n");
       return EXIT_FAILURE;
		}
    
    err = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, max_work_itm_dims* sizeof(size_t), max_loc_size, NULL);
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_ITEM_SIZES)!\n");
        return EXIT_FAILURE;
		}
		
    err = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_wrkgrp_size, NULL);
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_GROUP_SIZE)!\n");
        return EXIT_FAILURE;
		}
		
	printf(" max loc dim ");
   for(int i=0; i< max_work_itm_dims; i++)
		printf(" %d ",(int)(*(max_loc_size+i)));

   printf("\n");
   printf(" //Maximum number of work-items in a work-group = %d",(int)max_wrkgrp_size);
	max_WG=max_wrkgrp_size;
 
   err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(num_max_args), &num_max_args, NULL);
    if (err != CL_SUCCESS)
		{
        printf("Error: Failed to access max_constant_args!\n");
        return EXIT_FAILURE;
		} 
    return CL_SUCCESS;
}
 
// define the image size
int NR; int NC; int z; int work_size; int max_WG;
// define the block size
int BX; int BY;
FILE *fileU;
FILE *fileV;//files where de LK will be post
double t0, t1; //para medida de tiempos completos
static struct timeval time;

double get_time()
{
	double t;
	gettimeofday(&time, (struct timezone*)0);
	//obtain the current time, expressed as seconds and microseconds since the Epoch,
	//http://rabbit.eng.miami.edu/info/functions/time.html
	t = ((time.tv_usec) + (time.tv_sec)*1000000);
	return (t);
}

//—————————————————————————————————————————————————
//READ MOVIE
 
void read_movie(float*R, char *s, int nFrames)
{
    int i;
    FILE *file;//puntero al archivo
    float *Rf = (float *)malloc(NR*NC*nFrames*sizeof(float));
    //puntero a reserva memoria. fopen() devuelve puntero al archivo
    if ((file = fopen(s, "rb")) == NULL) { //rb= read binary
        printf("Error opening the file or the file does not exist\n");
        printf("string de entrada: %s \n", s);
        exit(0);
    } else {
        fread(Rf, NR*NC*nFrames*sizeof(float), 1, file);
        //fread(*ptr,size_of_elements, number_of_elements, *a_file);
        fclose (file);
        for (i=0; i<NR*NC*nFrames; i++)
            R[i] = (float)(Rf[i]);
        //guardamos datos fichero en R[i]
        free(Rf);

    }
}
 
 void uv2raw(float *M, char s[])
{
    FILE *file;
 
    if ((file = fopen(s, "wb")) == NULL)
        printf("Error opening the file or the file does not exist\n");
    else {
        fwrite(M, NR*NC*sizeof(float), 1, file);
        fclose(file);
    }
} 
 

  
//—————————————————————————————————————————————————
// This function reads in a text file and stores it as a char pointer
char* readSource(char* kernelPath) {
 
   cl_int status;
   FILE *fp;
   char *source;
   long int size;
 
   printf("\n //Program file is: %s\n", kernelPath);
 
   fp = fopen(kernelPath, "rb");
   if(!fp) {
      printf("Could not open kernel file\n");
      exit(-1);
   }
   status = fseek(fp, 0, SEEK_END);
   if(status != 0) {
      printf("Error seeking to end of file\n");
      exit(-1);
   }
   size = ftell(fp);
   if(size < 0) {
      printf("Error getting file position\n");
      exit(-1);
   }
 
   rewind(fp);
 
   source = (char *)malloc(size + 1);
 
   int i;
   for (i = 0; i < size+1; i++) {
      source[i]='\0';
   }
 
   if(source == NULL) {
      printf("Error allocating space for the kernel source\n");
      exit(-1);
   }
 
   fread(source, 1, size, fp);
   source[size] = '\0';
 
   return source;
}
 
void chk(cl_int status, const char* cmd) {
 
   if(status != CL_SUCCESS) {
      printf("%s failed (%d)\n", cmd, status);
      exit(-1);
   }
}

void timeCalculation(cl_ulong t_subs,cl_ulong t_filterX,cl_ulong t_filterY,cl_ulong t_multIxIy,cl_ulong t_mascIxIy, int nFrames){
	printf("\nTiempos medios:\n");
	printf("Media tiempo kernel filterX: %" PRIu64" us \n",(t_filterX/(nFrames-1))/ 1000);
	printf("Media tiempo kernel filterY: %" PRIu64" us \n",(t_filterY/(nFrames-1))/ 1000);
	printf("Media tiempo kernel subs: %" PRIu64" us \n", (t_subs/(nFrames-1))/1000);
	printf("Media tiempo kernel multIxIy: %" PRIu64" us \n", (t_multIxIy/(nFrames-1))/1000);
	printf("Media tiempo kernel mascIxIy: %" PRIu64" us \n", t_mascIxIy/(nFrames-1)/ 1000);
 }

 
//ELECTION WORK_SIZE
int select_work_size(int NC, int FS, int max_WG){//mejor buscar que sea potencia de dos y si eso tambien multiplo de 16

	if(16 <= 2*FS){	work_size= 32;//se divide en 4WG
	}
	else{	work_size=16;	
	}//no compruebo si 16WG son válidos
	return work_size;
}
 
int lucas_kanade(char *pathMovie, int nFrames, int FS, int BX)
{
	//reservamos la memoria y fijamos el filtro
    // the unary operator sizeof is used to calculate the size of any
    // datatype, measured in the number of bytes required to represent the type
    int dataSize = NR*NC*sizeof(float);
	int movieSize = NR*NC*nFrames*sizeof(float);
	
    // read images
    float *movie = (float *)malloc(movieSize);
    read_movie(movie, pathMovie, nFrames);
  
    float *Hx = (float *)malloc(FS*sizeof(float));//los bytes que ocupan un float por FS
    float *Hy = (float *)malloc(FS*sizeof(float));
    float *U = (float*)malloc(NR*NC*sizeof(float));
    float *V = (float*)malloc(NR*NC*sizeof(float));
    
    if (Hx == NULL ||  Hy == NULL  )
        {printf("Error en malloc"); return 0;}
	printf("Memoria reservada\n");fflush(NULL);
  
    switch(FS) {
        case 3:
            Hx[0] = Hy[2] = -1;
            Hx[1] = Hy[1] = -0;
            Hx[2] = Hy[0] = 1;
            break;
        case 5:
            Hx[0] = Hy[4] = 1/12.0;
            Hx[1] = Hy[3] = -2/3.0;
            Hx[2] = Hy[2] = 0;
            Hx[3] = Hy[1] = 2/3.0;
            Hx[4] = Hy[0] = -1/12.0;
            break;
        case 7:
            Hx[0] = Hy[6] = -1/60.0;
            Hx[1] = Hy[5] = 3/20.0;
            Hx[2] = Hy[4] = -3/4.0;
            Hx[3] = Hy[3] = 0;
            Hx[4] = Hy[2] = 3/4.0;
            Hx[5] = Hy[1] = -3/20.0;
            Hx[6] = Hy[0] = 1/60.0;
            break;
        case 9:
            Hx[0] = Hy[8] = 1/280.0;
            Hx[1] = Hy[7] = -4/105.0;
            Hx[2] = Hy[6] = 1/5.0;
            Hx[3] = Hy[5] = -4/5.0;
            Hx[4] = Hy[4] = 0;
            Hx[5] = Hy[3] = 4/5.0;
            Hx[6] = Hy[2] = -1/5.0;
            Hx[7] = Hy[1] = 4/105.0;
            Hx[8] = Hy[0] = -1/280.0;
            break;
	}

 
///--------Set up the OpenCL environment--------------
 
   cl_int status;
   //Discover and initialize the platforms
   cl_uint num_platforms;// Num of platforms finds
   cl_platform_id platform;
   status = clGetPlatformIDs(2, &platform, &num_platforms);  chk(status, "clGetPlatformIDs");
   printf(" //Num. platforms = %d ",num_platforms);


 // Discover and initialize the devices
   cl_device_id device;
   clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
   //clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);//---si queremos usar la CPU
   chk(status, "clGetDeviceIDs");
 
   // Create a context
   cl_context_properties props[3] = {CL_CONTEXT_PLATFORM,
   (cl_context_properties)(platform), 0};
   cl_context context;
   context = clCreateContext(props, 1, &device, NULL, NULL, &status);
   chk(status, "clCreateContext");
    
   // Create a command queue
   cl_command_queue queue;
   queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
   chk(status, "clCreateCommandQueue");clFinish(queue);
    
   output_device_info(device, max_WG);
	work_size = select_work_size(NC, FS, max_WG);
	printf("\n //Work_size: %d. Number of Work_Groups by dimension: %d", work_size, NC/work_size); 

   //Create device buffers
   //http://stackoverflow.com/questions/22057692/whats-the-difference-between-clenqueuemapbuffer-and-clenqueuewritebuffer
   cl_mem *bufferMovie = clCreateBuffer(context, CL_MEM_READ_ONLY,movieSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *filter_inputx = clCreateBuffer(context, CL_MEM_READ_ONLY, FS*sizeof(float), NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *filter_inputy = clCreateBuffer(context, CL_MEM_READ_ONLY, FS*sizeof(float), NULL, &status);chk(status, "clCreateBuffer");
  
   cl_mem *bufferx = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *buffery = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *buffert = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   
   cl_mem *ixix = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *ixiy = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *iyiy = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer"); 
   cl_mem *ixit = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *iyit = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status); chk(status, "clCreateBuffer");
   cl_mem *mixix = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBufferñññ");
   cl_mem *mixiy = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *miyiy = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer"); 
   cl_mem *mixit = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *miyit = clCreateBuffer(context, CL_MEM_READ_WRITE, dataSize, NULL, &status);chk(status, "clCreateBuffer");

   cl_mem *det1 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *det2 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *det = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *one_det = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *C00 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *C01 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *C11 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *U1 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *U2 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *V1 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *V2 = clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");

   cl_mem *bu_U = clCreateBuffer(context, CL_MEM_WRITE_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");
   cl_mem *bu_V = clCreateBuffer(context, CL_MEM_WRITE_ONLY, dataSize, NULL, &status);chk(status, "clCreateBuffer");

	//CREATE EVENTS
	cl_event eventTimes[3], eventTimesMu[5], eventTimesMa[5], eventWriteMovie, eventWriteFilter, eventTimesLK[4];
	cl_event event_det, event_one_det, eventTimesC[3], event_C01, eventTimesUV[4], eventU, eventV;

   //times calculations
    cl_ulong time_start, time_end,total_time, t_subs, t_filterX, t_filterY, t_multIxIy, t_mascIxIy;

   //Create and compile the programs
   const char* source = readSource("source_2.0.cl");
   cl_program program;
   program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);
   chk(status, "clCreateProgramWithSource");


   // Write host data to device buffers
   status = clEnqueueWriteBuffer(queue, bufferMovie, CL_FALSE, 0, movieSize, movie, 0, NULL, &eventWriteMovie);
   chk(status, "clEnqueueWriteBuffer1a"); 		
 /*  status = clWaitForEvents(1, &eventWriteMovie); chk(status, "Kernel run wait for event eventWriteMovie");		
		clGetEventProfilingInfo(eventWriteMovie, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
		clGetEventProfilingInfo(eventWriteMovie, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
		total_time = (time_end - time_start); //info in nanoseconds
		printf("\nExecution time write Movie= %"  PRIu64 " us\n", (total_time / 1000) );
*/
   status = clEnqueueWriteBuffer(queue, filter_inputx, CL_FALSE, 0, FS*sizeof(float), Hx, 0, NULL, &eventWriteFilter);
   chk(status, "clEnqueueWriteBuffer1a");
  /* status = clWaitForEvents(1, &eventWriteFilter); chk(status, "Kernel run wait fro event eventWriteFilter");		
		clGetEventProfilingInfo(eventWriteFilter, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
		clGetEventProfilingInfo(eventWriteFilter, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
		total_time = (time_end - time_start); //info in nanoseconds
		printf("Execution time write Filter X = %"  PRIu64 " us\n", (total_time / 1000) );
   		
	*/		
   status = clEnqueueWriteBuffer(queue, filter_inputy, CL_FALSE, 0,FS*sizeof(float), Hy, 0, NULL, NULL);
   chk(status, "clEnqueueWriteBuffer1a");
 

	if (clBuildProgram(program, 1, &device, NULL, NULL, NULL) != CL_SUCCESS) {
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		// Allocate memory for the log
		char *log = (char *) malloc(log_size);
		// Get the log
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("CL Compilation failed:%s\n", log);
		}
		
	//CREATE KERNELS
	cl_kernel k_subst = clCreateKernel(program, "subst_cl", &status); chk(status, "clCreateKernelS");
	cl_kernel k_imfilterX = clCreateKernel(program, "imfilterX_cl", &status); chk(status, "clCreateKernelIx");
	cl_kernel k_imfilterY = clCreateKernel(program, "imfilterY_cl", &status); chk(status, "clCreateKernelIy");
	cl_kernel k_mult = clCreateKernel(program, "mult_cl", &status);  chk(status, "clCreateKernelM");
	cl_kernel k_imfilterS = clCreateKernel(program, "imfilterS_cl", &status); chk(status, "clCreateKernelIS");

	cl_kernel multEsc_cl = clCreateKernel(program, "multEsc_cl", &status); chk(status, "clCreateKernelIS");
	cl_kernel divEsc_cl = clCreateKernel(program, "divEsc_cl", &status); chk(status, "clCreateKernelIS");
	cl_kernel sum_cl = clCreateKernel(program, "sum_cl", &status); chk(status, "clCreateKernelIS");
	cl_kernel sub2_cl = clCreateKernel(program, "sub2_cl", &status); chk(status, "clCreateKernelIS");

	t0 = get_time();

	////LUCAS KANADE----------------------------------------------------
	for (int z=0; z<nFrames-1; z++){
	//for (z=0; z<5; z++){
	//printf("Frame: %i\r",z);fflush(stdout);
	//printf("Frame: %i\n",z);
	// https://www.khronos.org/registry/cl/specs/opencl-1.x-latest.pdf#page=103
	
	//Global-Local Work Size
       size_t globalWorkSize[2];//2 dims
       globalWorkSize[0] = NR;//Numero de workItems en una dimension
       globalWorkSize[1] = NC; 
       size_t local_work_size[2];
       local_work_size[0]= work_size; local_work_size[1]=  work_size;//describe the number of work-items that make up a work-group
   	   //const size_t *local_work_size=?; Num. de WI en una dim de un WGroup
       //si es 2, tendremos (num.WI)/2=numero de workgroups  

    ///----subst-------------------------------------------
           status = clSetKernelArg(k_subst, 0, sizeof(cl_mem), &bufferMovie);//A0
           chk(status, "clSetKernelArgSubsMovie"); 
           status |= clSetKernelArg(k_subst, 1, sizeof(cl_mem), &buffert);//It
           status |= clSetKernelArg(k_subst, 2, sizeof(int), &NR);
           status |= clSetKernelArg(k_subst, 3, sizeof(int), &NC);
           chk(status, "clSetKernelArgSubs"); 
           status = clSetKernelArg(k_subst, 4, sizeof(int), &z); chk(status, "clSetKernelArgSubsz"); 
    
           status = clEnqueueNDRangeKernel(queue, k_subst, 2, NULL, globalWorkSize, NULL, 0, NULL, &eventTimes[0]);
           chk(status, "clEnqueueNDRange3b");
	// clWaitForEvents(1 , &eventTimes[0]);
	   // status = clWaitForEvents(1, &eventTimes[0]); chk(status, "Kernel run wait fro event eventTimes[0]");		
		//Time Measures
		clGetEventProfilingInfo(eventTimes[0], CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
		clGetEventProfilingInfo(eventTimes[0], CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
		total_time = time_end - time_start;
		//printf("Execution time kernel subs = %0.3f ms\n", (total_time / 1000000.0) );
		t_subs+=total_time;
		//printf("t_subs : %" PRIu64 " us\n", time_end/1000 - time_start/1000);

    ///----imfilterX-------------------------------------------  
	if (	clSetKernelArg(k_imfilterX, 0, sizeof(cl_mem), &bufferMovie)        			||
		clSetKernelArg(k_imfilterX, 1, sizeof(cl_mem), &bufferx)   				||
		clSetKernelArg(k_imfilterX, 2, sizeof(cl_mem), &filter_inputx)      			||	
		clSetKernelArg(k_imfilterX, 3, sizeof(float) * (work_size+(FS-1))*(work_size), NULL) 	||	
		clSetKernelArg(k_imfilterX, 4, sizeof(int), &FS)     					||
		clSetKernelArg(k_imfilterX, 5, sizeof(int), &NC)					||
		clSetKernelArg(k_imfilterX, 6, sizeof(int), &NR)					||
		clSetKernelArg(k_imfilterX, 7, sizeof(int), &z) != CL_SUCCESS)	{
		printf("Unable to set kernel_NR arguments\n");		
	exit(1);	}
		
            //printf("\nglobal:%" PRIu64 " local: %" PRIu64 "\n", globalWorkSize[1], local_work_size[1]);

            status = clEnqueueNDRangeKernel(queue,k_imfilterX, 2, NULL, globalWorkSize, NULL,  0, NULL, &eventTimes[1]);
            chk(status, "clEnqueueNDRange2x");

	  /* //status = clWaitForEvents(1, &eventTimes[1]); chk(status, "Kernel run wait fro event eventTimes[1]");		
           	//Time Measures
			clGetEventProfilingInfo(eventTimes[1], CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
			clGetEventProfilingInfo(eventTimes[1], CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
			total_time = time_end - time_start;
			//printf("Execution time kernel filter X = %0.3f ms\n", (total_time / 1000000.0) );
			t_filterX+=total_time;
*/
			
    ///----imfilterY-------------------------------------------  
	if (	clSetKernelArg(k_imfilterY, 0, sizeof(cl_mem), &bufferMovie)        			||
		clSetKernelArg(k_imfilterY, 1, sizeof(cl_mem), &buffery)   				||
		clSetKernelArg(k_imfilterY, 2, sizeof(cl_mem), &filter_inputy)      			||	
		clSetKernelArg(k_imfilterY, 3, sizeof(float) * (work_size+(FS-1))*(work_size), NULL) 	||	
		clSetKernelArg(k_imfilterY, 4, sizeof(int), &FS)     					||
		clSetKernelArg(k_imfilterY, 5, sizeof(int), &NC)					||
		clSetKernelArg(k_imfilterY, 6, sizeof(int), &NR)					||
		clSetKernelArg(k_imfilterY, 7, sizeof(int), &z) != CL_SUCCESS)	{
		printf("Unable to set kernel_NR arguments of imfilterY\n");		
		exit(1);	}
       status = clEnqueueNDRangeKernel(queue,k_imfilterY, 2, NULL, globalWorkSize, NULL,  0, NULL, &eventTimes[2]);
       chk(status, "clEnqueueNDRange2y");
       /*status = clWaitForEvents(1, &eventTimes[2]); chk(status, "Kernel run wait fro event eventTimes[2]");		
			//Time Measures
			clGetEventProfilingInfo(eventTimes[2], CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
			clGetEventProfilingInfo(eventTimes[2], CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
			total_time = time_end - time_start;
			//printf("Execution time kernel filter Y = %0.3f ms\n", (total_time / 1000000.0) );
			t_filterY +=total_time;
		*/

    //MULTIPLICATIONS-----------------------
    ///IxIY-------------------------------------------  
		   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &bufferx);
		   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &buffery);
		   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &ixiy);
		   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
		   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);
		   chk(status, "clSetKernelArg3");
		   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 3, eventTimes, &eventTimesMu[0]);
		   chk(status, "clEnqueueNDRangeIxIy");
	  /*      //Time Measures
			clGetEventProfilingInfo(eventTimesMu[0], CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
			clGetEventProfilingInfo(eventTimesMu[0], CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
			total_time = time_end - time_start;
			t_multIxIy +=total_time;
            */
      ///Ix2----------------------------------------------  
		   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &bufferx);
		   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &bufferx);
		   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &ixix);
		   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
		   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);
		   chk(status, "clSetKernelArg3");
		   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL,  3, eventTimes, &eventTimesMu[1]);
		   chk(status, "clEnqueueNDRangeIx2");
        
      ///Iy2----------------------------------------------  
		   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &buffery);
		   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &buffery);
		   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &iyiy);
		   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
		   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);
		   chk(status, "clSetKernelArg3");
		   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 3, eventTimes, &eventTimesMu[2]);
		   chk(status, "clEnqueueNDRangeIy2");
 	        
      ///IxIt---------------------------------------------  
		   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &bufferx);
		   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &buffert);
		   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &ixit);
		   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
		   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);
		   chk(status, "clSetKernelArg3");
		   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 3, eventTimes, &eventTimesMu[3]);
		   chk(status, "clEnqueueNDRangeIxIt");

      ///IyIt-----------------------------------------------  
		   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &buffery);
		   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &buffert);
		   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &ixit);
		   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
		   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);
		   chk(status, "clSetKernelArg3");
		   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 3, eventTimes, &eventTimesMu[4]);
		   chk(status, "clEnqueueNDRangeIyIt");

	//Mascaras
	       ///mIxIy--------------------------------  
		   status  = clSetKernelArg(k_imfilterS, 0, sizeof(cl_mem), &ixiy);
		   status |= clSetKernelArg(k_imfilterS, 1, sizeof(cl_mem), &mixiy);
		   status |= clSetKernelArg(k_imfilterS, 2, sizeof(int), &NR);
		   status |= clSetKernelArg(k_imfilterS, 3, sizeof(int), &NC);
		   status |= clSetKernelArg(k_imfilterS, 4, sizeof(int), &BX);
		   chk(status, "clSetKernelArg4");
		   status = clEnqueueNDRangeKernel(queue,k_imfilterS, 1, NULL, globalWorkSize, NULL, 5, eventTimesMu, &eventTimesMa[0]);
		   chk(status, "clEnqueueNDRange6");

     	/*		//Time Measures
				clGetEventProfilingInfo(eventTimesMa[0], CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
				clGetEventProfilingInfo(eventTimesMa[0], CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
				total_time = time_end - time_start;
				t_mascIxIy +=total_time;
*/
	      ///mIxIx------------------------------
		   status  = clSetKernelArg(k_imfilterS, 0, sizeof(cl_mem), &ixix);
		   status |= clSetKernelArg(k_imfilterS, 1, sizeof(cl_mem), &mixix);
		   status |= clSetKernelArg(k_imfilterS, 2, sizeof(int), &NR);
		   status |= clSetKernelArg(k_imfilterS, 3, sizeof(int), &NC);
		   status |= clSetKernelArg(k_imfilterS, 4, sizeof(int), &BX);
		   chk(status, "clSetKernelArg4");
		   status = clEnqueueNDRangeKernel(queue,k_imfilterS, 1, NULL, globalWorkSize, NULL, 5,eventTimesMu, &eventTimesMa[1]);
		   chk(status, "clEnqueueNDRange6");
	      ///mIyIy--------------- -----------------
		   status  = clSetKernelArg(k_imfilterS, 0, sizeof(cl_mem), &iyiy);
		   status |= clSetKernelArg(k_imfilterS, 1, sizeof(cl_mem), &miyiy);
		   status |= clSetKernelArg(k_imfilterS, 2, sizeof(int), &NR);
		   status |= clSetKernelArg(k_imfilterS, 3, sizeof(int), &NC);
		   status |= clSetKernelArg(k_imfilterS, 4, sizeof(int), &BX);
		   chk(status, "clSetKernelArg4");
		   status = clEnqueueNDRangeKernel(queue,k_imfilterS, 1, NULL, globalWorkSize, NULL,5,eventTimesMu, &eventTimesMa[2]);
		   chk(status, "clEnqueueNDRange6");
	      ///mIxIt------------- --------------------
		   status  = clSetKernelArg(k_imfilterS, 0, sizeof(cl_mem), &ixit);
		   status |= clSetKernelArg(k_imfilterS, 1, sizeof(cl_mem), &mixit);
		   status |= clSetKernelArg(k_imfilterS, 2, sizeof(int), &NR);
		   status |= clSetKernelArg(k_imfilterS, 3, sizeof(int), &NC);
		   status |= clSetKernelArg(k_imfilterS, 4, sizeof(int), &BX);
		   chk(status, "clSetKernelArg4");
		   status = clEnqueueNDRangeKernel(queue,k_imfilterS, 1, NULL, globalWorkSize, NULL, 5,eventTimesMu, &eventTimesMa[3]);
		   chk(status, "clEnqueueNDRange6");
	      ///mIyIt-------------------------------  
		   status  = clSetKernelArg(k_imfilterS, 0, sizeof(cl_mem), &iyit);
		   status |= clSetKernelArg(k_imfilterS, 1, sizeof(cl_mem), &miyit);
		   status |= clSetKernelArg(k_imfilterS, 2, sizeof(int), &NR);
		   status |= clSetKernelArg(k_imfilterS, 3, sizeof(int), &NC);
		   status |= clSetKernelArg(k_imfilterS, 4, sizeof(int), &BX);
		   chk(status, "clSetKernelArg4");
		   status = clEnqueueNDRangeKernel(queue,k_imfilterS, 1, NULL, globalWorkSize, NULL,5,eventTimesMu, &eventTimesMa[4]);
		   chk(status, "clEnqueueNDRange6");
/*
	   status = clWaitForEvents(1, &eventTimesMa[0]); chk(status, "Kernel run wait fro event eventTimesMa[0]");		
	   status = clWaitForEvents(1, &eventTimesMa[1]); chk(status, "Kernel run wait fro event eventTimesMa[1]");		
	   status = clWaitForEvents(1, &eventTimesMa[2]); chk(status, "Kernel run wait fro event eventTimesMa[2]");		
	   status = clWaitForEvents(1, &eventTimesMa[3]); chk(status, "Kernel run wait fro event eventTimesMa[3]");		
	   status = clWaitForEvents(1, &eventTimesMa[4]); chk(status, "Kernel run wait fro event eventTimesMa[4]");	
*/	
//—————————————————————————————————————————————————
//FUNCIONES RELATIVAS A LUCAS KANADE                               
//Resuelvo matrices LK para obtener U, V finales
//mIx2*mIy2
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &mixix);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &miyiy);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &det1);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgdet1");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 5, eventTimesMa, &eventTimesLK[0]);
	   chk(status, "clEnqueueNDRange1");
//mIxIy*mIxIy
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &mixiy);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &mixiy);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &det2);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgdet2");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 5, eventTimesMa, &eventTimesLK[1]);
	   chk(status, "clEnqueueNDRange2");
//(-1)*mIyIt
	float E=-1;
	   status  = clSetKernelArg(multEsc_cl, 0, sizeof(cl_mem), &miyit);
	   status |= clSetKernelArg(multEsc_cl, 1, sizeof(float), &E);
	   status |= clSetKernelArg(multEsc_cl, 2, sizeof(cl_mem), &miyit);
	   status |= clSetKernelArg(multEsc_cl, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(multEsc_cl, 4, sizeof(int), &NC);chk(status, "clSetKernelArgmIyIt");
	   status = clEnqueueNDRangeKernel(queue,multEsc_cl, 2, NULL, globalWorkSize, NULL, 5, eventTimesMa, &eventTimesLK[2]);
	   chk(status, "clEnqueueNDRange3");
//(-1)*mIxIt
	   status  = clSetKernelArg(multEsc_cl, 0, sizeof(cl_mem), &mixit);
	   status |= clSetKernelArg(multEsc_cl, 1, sizeof(float), &E);
	   status |= clSetKernelArg(multEsc_cl, 2, sizeof(cl_mem), &mixit);
	   status |= clSetKernelArg(multEsc_cl, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(multEsc_cl, 4, sizeof(int), &NC);chk(status, "clSetKernelArgmIxIt");
	   status = clEnqueueNDRangeKernel(queue, multEsc_cl, 2, NULL, globalWorkSize, NULL, 5, eventTimesMa, &eventTimesLK[3]);
	   chk(status, "clEnqueueNDRange4");
//
//
//det=et1-det2
           status = clSetKernelArg(sub2_cl, 0, sizeof(cl_mem), &det1);
           status |= clSetKernelArg(sub2_cl, 1, sizeof(cl_mem), &det2);
           status |= clSetKernelArg(sub2_cl, 2, sizeof(cl_mem), &det);
           status |= clSetKernelArg(sub2_cl, 3, sizeof(int), &NR);
           status |= clSetKernelArg(sub2_cl, 4, sizeof(int), &NC); chk(status, "clSetKernelArgdet1-det2"); 
           status = clEnqueueNDRangeKernel(queue, sub2_cl, 2, NULL, globalWorkSize, NULL, 4, eventTimesLK, &event_det);
           chk(status, "clEnqueueNDRange5"); 
	  // status = clWaitForEvents(1, &event_det); chk(status, "Kernel run wait fro event event_det");
//
//
//1/det
	E=1;
           status = clSetKernelArg(divEsc_cl, 0, sizeof(cl_mem), &det1);
           status |= clSetKernelArg(divEsc_cl, 1, sizeof(float), &E);
           status |= clSetKernelArg(divEsc_cl, 2, sizeof(cl_mem), &one_det);
           status |= clSetKernelArg(divEsc_cl, 3, sizeof(int), &NR);
           status |= clSetKernelArg(divEsc_cl, 4, sizeof(int), &NC); chk(status, "clSetKernelArg1/det"); 
           status = clEnqueueNDRangeKernel(queue, divEsc_cl, 2, NULL, globalWorkSize, NULL, 0, NULL, &event_one_det);
           chk(status, "clEnqueueNDRange6"); 
	  // status = clWaitForEvents(1, &event_one_det); chk(status, "Kernel run wait fro event event_one_det");
//
//
//C00=mIx2*one_det
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &mixix);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &one_det);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &C00);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgC00");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 0, NULL, &eventTimesC[0]);
	   chk(status, "clEnqueueNDRange7");
//C01=mIxIy*one_det
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &mixiy);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &one_det);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &C01);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgC01");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 0, NULL, &eventTimesC[1]);
	   chk(status, "clEnqueueNDRange8");  
//C11=mIx2*one_det
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &miyiy);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &one_det);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &C11);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgC01");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 0, NULL, &eventTimesC[2]);
	   chk(status, "clEnqueueNDRange9");  
//
//
//(-1)*C01
	E=-1;
	   status  = clSetKernelArg(multEsc_cl, 0, sizeof(cl_mem), &C01);
	   status |= clSetKernelArg(multEsc_cl, 1, sizeof(float), &E);
	   status |= clSetKernelArg(multEsc_cl, 2, sizeof(cl_mem), &C01);
	   status |= clSetKernelArg(multEsc_cl, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(multEsc_cl, 4, sizeof(int), &NC);chk(status, "clSetKernelArgmC01_2");
	   status = clEnqueueNDRangeKernel(queue, multEsc_cl, 2, NULL, globalWorkSize, NULL, 3, eventTimesC, &event_C01);
	   chk(status, "clEnqueueNDRange10");
	 //  status = clWaitForEvents(1, &event_C01); chk(status, "Kernel run wait fro event event_C01");
//
//
//U1=C00*mIxIt
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &C00);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &mixit);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &U1);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgU1");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 0, NULL, &eventTimesUV[0]);
	   chk(status, "clEnqueueNDRange11");  
//U2=C01*mIyIt
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &C01);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &miyit);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &U2);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgU2");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 0, NULL, &eventTimesUV[1]);
	   chk(status, "clEnqueueNDRange12"); 
//V1=C00*mIxIt
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &C01);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &mixit);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &V1);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgV1");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 0, NULL, &eventTimesUV[2]);
	   chk(status, "clEnqueueNDRange13");  
//V2=C01*mIyIt
	   status  = clSetKernelArg(k_mult, 0, sizeof(cl_mem), &C11);
	   status |= clSetKernelArg(k_mult, 1, sizeof(cl_mem), &miyit);
	   status |= clSetKernelArg(k_mult, 2, sizeof(cl_mem), &V2);
	   status |= clSetKernelArg(k_mult, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(k_mult, 4, sizeof(int), &NC);chk(status, "clSetKernelArgV2");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 0, NULL, &eventTimesUV[3]);
	   chk(status, "clEnqueueNDRange14"); 
//
//
//U=U1+U2
	   status  = clSetKernelArg(sum_cl, 0, sizeof(cl_mem), &U1);
	   status |= clSetKernelArg(sum_cl, 1, sizeof(cl_mem), &U2);
	   status |= clSetKernelArg(sum_cl, 2, sizeof(cl_mem), &bu_U);
	   status |= clSetKernelArg(sum_cl, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(sum_cl, 4, sizeof(int), &NC);chk(status, "clSetKernelArgU");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 4, eventTimesUV, &eventU);
	   chk(status, "clEnqueueNDRange15");
//V=V1+V2
	   status  = clSetKernelArg(sum_cl, 0, sizeof(cl_mem), &V1);
	   status |= clSetKernelArg(sum_cl, 1, sizeof(cl_mem), &V2);
	   status |= clSetKernelArg(sum_cl, 2, sizeof(cl_mem), &bu_V);
	   status |= clSetKernelArg(sum_cl, 3, sizeof(int), &NR);
	   status |= clSetKernelArg(sum_cl, 4, sizeof(int), &NC);chk(status, "clSetKernelArgV");
	   status = clEnqueueNDRangeKernel(queue, k_mult, 2, NULL, globalWorkSize, NULL, 4, eventTimesUV, &eventV);
	   chk(status, "clEnqueueNDRange16");

	  // status = clWaitForEvents(1, &eventU); chk(status, "Kernel run wait fro event eventU");
	  // status = clWaitForEvents(1, &eventV); chk(status, "Kernel run wait fro event eventV");
           status = clEnqueueReadBuffer(queue, bu_U, CL_TRUE, 0, dataSize, U, 0, NULL, NULL);
           status = clEnqueueReadBuffer(queue, bu_V, CL_TRUE, 0, dataSize, V, 0, NULL, NULL);
          

   
    uv2raw(U, "u.raw");
    uv2raw(V, "v.raw");
    
    }  //fin de las iteraciones para barrer los frames.

    //timeCalculation(t_subs, t_filterX, t_filterY, t_multIxIy, t_mascIxIy, nFrames);
    
    //printf("\n");
    //printf("Sale del bucle lucas Kanade\n");

	t1 = get_time();
	printf("\nComplete algorithm time %f us\n", (t1-t0)*1.0); 


    // Free memory
    //free(movie);
    free(U);
    free(V);
    free(Hx);
    free(Hy);
    return 0;
}
 
//—————————————————————————————————————————————————
//MAIN
//—————————————————————————————————————————————————
 
int main(int argc, char **argv) {

printf("\n------ Lucas Kanade_2_0 ------\n");
     
        int count=0;
        printf ("This program was called with \"%s\".\n",argv[0]);
    if (argc > 1)
        {
		  for (count = 1; count < argc; count++){
			printf("argv[%d] = %s\n", count, argv[count]);
			}
        }
     
    if (argc < 7) {
        printf("\nUsage: %s : [1]path_movie [2]numRows [3]numCols [4]numFrames  [5]filterSize [6]blockSize  \n", argv[0]);
        exit(-1);
		} 
    else {
        //Asignaciones de valores
        char *path_movie = argv[1];
        NR = atoi(argv[2]);
        NC = atoi(argv[3]);
        int nFrames = atoi(argv[4]);
        int FS = atoi(argv[5]);//tiene que ser impar para que funcione el kernel
 
        if (FS!=3 && FS!=5 && FS!=7 && FS!=9) {
            printf("\nFilter sizes allowed: 3 - 5 - 7 - 9\n");
       	printf("\nUsage: %s : [1]path_movie [2]numRows [3]numCols [4]numFrames  [5]filterSize [6]blockSize  \n", argv[0]);
            exit(-1);
        }
 
        BX = BY = atoi(argv[6]);//poner condicion para que no ocupe demasiado?
								//y para que sea par su raiz
		
		if(BX<0 || BX > NC/2){
			printf("Block Size allowed: odd, >0 and <NC/2 \n"); 
       printf("\nUsage: %s : [1]path_movie [2]numRows [3]numCols [4]numFrames  [5]filterSize [6]blockSize  \n", argv[0]);
            exit(-1);						
			}				
			
		printf("Antes de entrar en lucas_kanade\n");
		
       		lucas_kanade(path_movie, nFrames, FS, BX);
     
		}

return 0;
}

