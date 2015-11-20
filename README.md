# Lukas-Kanade-OpenCL
Lucas-Kanade Algoritm not ended developed for the TFM (Trabajo Fin de Máster) "Máster en nuevas tecnologías electrónicas y fotónicas" UCM


INDICE:
1/Compilación
2/Ejecución
3/Salida
4/Pasos hasta terminar
5/Control errores OpenCL

1/COMPILACION
Para compilar hay que tener instalado el paquete de AMD de Intel o el de NVIDIA que tambien incluye el OpenCL
hay que saber en que directorio esta el CL.h y la libreria libOpenCL.so. Se compila con gcc
gcc nombrearchivo.c -std=c99 -I/opt/direcciondelincludeCL.h -L/opt/direcciondelalibrerialibOpen  -lOpenCL
Ejemplo compilación:
gcc LK_1.c -Wall -std=c99 -I/opt_speedy/AMDAPP/include -L/opt_speedy/AMDAPP/lib/x86_64 -lOpenCL

2/EJECUCION
Para la ejecución hace falta un fichero de video binario conocer las dimensiones de cada frame el número de frames e indicar el
tamaño del bloque y del filtro.
Ejecutar a.out:
 ./a.out ./estimulos/divergingtree_150x150_40frs.bin  150 150 40 5 10

3/SALIDA
El código genera dos ficheros U.raw V.raw con las velocidades de cada pixel para el frame. Visualizar con el código showUV.m desde Matlab

4/PASOS HASTA TERMINAR
Falta generar dos ficheros de velocidades pero con todos los frames
Ajustar el kernel para que no de erores en el tratamiento de los bordes

5/LISTA ERRORES OPENCLL:
http://www.techdarting.com/2014/01/opencl-errors.html

CL_SUCCESS 	0 	Everything is good.
CL_DEVICE_NOT_FOUND 	-1 	No OpenCL devices that matched given device type were found
CL_DEVICE_NOT_AVAILABLE 	-2 	No OpenCL compatible device was found
CL_COMPILER_NOT_AVAILABLE 	-3 	OpenCL Compiler perhaps failed to configure itself, or check your OpenCL installation
CL_MEM_OBJECT_ALLOCATION_FAILURE 	-4 	Failed to allocate memory for buffer object
CL_OUT_OF_RESOURCES 	-5 	failure to allocate resources required by the OpenCL implementation on the device
CL_OUT_OF_HOST_MEMORY 	-6 	failure to allocate resources required by the OpenCL implementation on the host
CL_PROFILING_INFO_NOT_AVAILABLE 	-7 	returned by clGetEventProfilingInfo, if the CL_QUEUE_PROFILING_ENABLE flag is not set for the command-queue and if the profiling information is currently not available
CL_MEM_COPY_OVERLAP 	-8 	if source and destination buffers are the same buffer object and the source and destination regions overlap
CL_IMAGE_FORMAT_MISMATCH 	-9 	src and dst image do not use the same image format<>
CL_IMAGE_FORMAT_NOT_SUPPORTED 	-10 	the image format is not supported.
CL_BUILD_PROGRAM_FAILURE 	-11 	program build error for given device, Use clGetProgramBuildInfo API call to get the build log of the kernel compilation.
CL_MAP_FAILURE 	-12 	failed to map the requested region into the host address space. This error does not occur for buffer objects created with CL_MEM_USE_HOST_PTR or CL_MEM_ALLOC_HOST_PTR
CL_MISALIGNED_SUB_BUFFER_OFFSET 	-13 	no devices in given context associated with buffer for which the origin value is aligned to the CL_DEVICE_MEM_BASE_ADDR_ALIGN value
CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST 	-14 	returned by clWaitForEvents(), execution status of any of the events in event list is a negative integer value i.e., error
CL_COMPILE_PROGRAM_FAILURE 	-15 	failed to compile the program source. Error occurs if clCompileProgram does not return until the compile has completed
CL_LINKER_NOT_AVAILABLE 	-16 	Linker unavailable
CL_LINK_PROGRAM_FAILURE 	-17 	failed to link the compiled binaries and perhaps libraries
CL_DEVICE_PARTITION_FAILED 	-18 	given partition name is supported by the implementation but input device couldn't be partitioned further
CL_KERNEL_ARG_INFO_NOT_AVAILABLE 	-19 	argument information is not available for the given kernel
CL_INVALID_VALUE 	-30 	values passed in the flags parameter is not valid
CL_INVALID_DEVICE_TYPE 	-31 	device type specified is not valid, its returned by clCreateContextFromType / clGetDeviceIDs
CL_INVALID_PLATFORM 	-32 	the specified platform is not a valid platform, its returned by clGetPlatformInfo /clGetDeviceIDs / clCreateContext / clCreateContextFromType
CL_INVALID_DEVICE 	-33 	device/s specified are not valid
CL_INVALID_CONTEXT 	-34 	the given context is invalid OpenCL context, or the context associated with certain parameters are not the same
CL_INVALID_QUEUE_PROPERTIES 	-35 	specified properties are valid but are not supported by the device, its returned by clCreateCommandQueue / clSetCommandQueueProperty
CL_INVALID_COMMAND_QUEUE 	-36 	the specified command-queue is not a valid command-queue
CL_INVALID_HOST_PTR 	-37 	host pointer is NULL and CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags. returned by clCreateBuffer / clCreateImage2D / clCreateImage3D
CL_INVALID_MEM_OBJECT 	-38 	the passed parameter is not a valid memory, image, or buffer object
CL_INVALID_IMAGE_FORMAT_DESCRIPTOR 	-39 	image format specified is not valid or is NULL, clCreateImage2D /clCreateImage3D returns this.
INVALID_IMAGE_SIZE 	-40 	Its returned by create Image functions 2D/3D, if specified image width or height are outbound or 0
CL_INVALID_SAMPLER 	-41 	specified sampler is an invalid sampler object
CL_INVALID_BINARY 	-42 	program binary is not a valid binary for the specified device, returned by clBuildProgram / clCreateProgramWithBinary
CL_INVALID_BUILD_OPTIONS 	-43 	the given build options are not valid
CL_INVALID_PROGRAM 	-44 	the given program is an invalid program object, returned by clRetainProgram / clReleaseProgram / clBuildProgram / clGetProgramInfo / clGetProgramBuildInfo / clCreateKernel / clCreateKernelsInProgram
CL_INVALID_PROGRAM_EXECUTABLE 	-45 	if there is no successfully built executable for program returned by clCreateKernel, there is no device in program then returned by clCreateKernelsInProgram, if no successfully built program executable present for device associated with command queue then returned by clEnqueueNDRangeKernel / clEnqueueTask
CL_INVALID_KERNEL_NAME 	-46 	mentioned kernel name is not found in program
CL_INVALID_KERNEL_DEFINITION 	-47 	arguments mismatch for the __kernel function definition and the passed ones, returned by clCreateKernel
CL_INVALID_KERNEL 	-48 	specified kernel is an invalid kernel object
CL_INVALID_ARG_INDEX 	-49 	clSetKernelArg if an invalid argument index is specified
CL_INVALID_ARG_VALUE 	-50 	the argument value specified is NULL, returned by clSetKernelArg
CL_INVALID_ARG_SIZE 	-51 	the given argument size (arg_size) do not match size of the data type for an argument, returned by clSetKernelArg
CL_INVALID_KERNEL_ARGS 	-52 	the kernel argument values have not been specified, returned by clEnqueueNDRangeKernel / clEnqueueTask
CL_INVALID_WORK_DIMENSION 	-53 	given work dimension is an invalid value, returned by clEnqueueNDRangeKernel
CL_INVALID_WORK_GROUP_SIZE 	-54 	the specified local workgroup size and number of workitems specified by global workgroup size is not evenly divisible by local workgroup size
CL_INVALID_WORK_ITEM_SIZE 	-55 	no. of workitems specified in any of local work group sizes is greater than the corresponding values specified by CL_DEVICE_MAX_WORK_ITEM_SIZES in that particular dimension
CL_INVALID_GLOBAL_OFFSET 	-56 	global_work_offset is not NULL. Must currently be a NULL value. In a future revision of OpenCL, global_work_offset can be used but not until OCL 1.2
CL_INVALID_EVENT_WAIT_LIST 	-57 	event wait list is NULL and (no. of events in wait list > 0), or event wait list is not NULL and no. of events in wait list is 0, or specified event objects are not valid events
CL_INVALID_EVENT 	-58 	invalid event objects specified
CL_INVALID_OPERATION 	-59 	
CL_INVALID_GL_OBJECT 	-60 	not a valid GL buffer object
CL_INVALID_BUFFER_SIZE 	-61 	the value of the parameter size is 0 or exceeds CL_DEVICE_MAX_MEM_ALLOC_SIZE for all devices specified in the parameter context, returned by clCreateBuffer
CL_INVALID_MIP_LEVEL 	-62 	
CL_INVALID_GLOBAL_WORK_SIZE 	-63 	specified global work size is NULL, or any of the values specified in global work dimensions are 0 or exceeds the range given by the sizeof(size_t) for the device on which the kernel will be enqueued, returned by clEnqueueNDRangeKernel
CL_INVALID_PROPERTY 	-64 	context property name in properties is not a supported property name, returned by clCreateContext
CL_INVALID_IMAGE_DESCRIPTOR 	-65 	values specified in image description are invalid
CL_INVALID_COMPILER_OPTIONS 	-66 	compiler options specified by options are invalid, returned by clCompileProgram
CL_INVALID_LINKER_OPTIONS 	-67 	linker options specified by options are invalid, returned by clLinkProgram
CL_INVALID_DEVICE_PARTITION_COUNT 	-68 	

