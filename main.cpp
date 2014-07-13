#include <QDebug>
#include <CL/opencl.h>
#include <iostream>
#include <fstream>

using namespace std;

QString errToStr(int err){
	switch (err) {
		case CL_SUCCESS:
			return "CL_SUCCESS";
		case CL_DEVICE_NOT_FOUND:
			return "CL_DEVICE_NOT_FOUND";
		case CL_DEVICE_NOT_AVAILABLE:
			return "CL_DEVICE_NOT_AVAILABLE";
		case CL_OUT_OF_RESOURCES:
			return "CL_OUT_OF_RESOURCES";
		case CL_OUT_OF_HOST_MEMORY:
			return "CL_OUT_OF_HOST_MEMORY";
		case CL_INVALID_VALUE:
			return "CL_INVALID_VALUE";
		case CL_INVALID_DEVICE_TYPE:
			return "CL_INVALID_DEVICE_TYPE";
		case CL_INVALID_PLATFORM:
			return "CL_INVALID_PLATFORM";
		case CL_INVALID_DEVICE:
			return "CL_INVALID_DEVICE";
		case CL_INVALID_PROPERTY:
			return "CL_INVALID_PROPERTY";
		default:
			return "qq";
	}

}

QString deviceTypeToStr(cl_device_type deviceType){
	switch (deviceType) {
		case CL_DEVICE_TYPE_CPU:
			return "CL_DEVICE_TYPE_CPU";
		case CL_DEVICE_TYPE_GPU:
			return "CL_DEVICE_TYPE_GPU";
		case CL_DEVICE_TYPE_ACCELERATOR:
			return "CL_DEVICE_TYPE_ACCELERATOR";
		case CL_DEVICE_TYPE_CUSTOM:
			return "CL_DEVICE_TYPE_CUSTOM";
		case CL_DEVICE_TYPE_DEFAULT:
			return "CL_DEVICE_TYPE_DEFAULT";
		case CL_DEVICE_TYPE_ALL:
			return "CL_DEVICE_TYPE_ALL";
		default:
			return "qq";
	}
}

cl_device_id getDeviceByType(cl_device_type deviceType, int *err){
	uint num_platforms;
	*err = clGetPlatformIDs(0, nullptr, &num_platforms);
	if(*err != CL_SUCCESS)
		return nullptr;
	cl_platform_id platform_ids[num_platforms];
	*err = clGetPlatformIDs(num_platforms, platform_ids, nullptr);
	if(*err != CL_SUCCESS)
		return nullptr;
	for (cl_platform_id* i = platform_ids; i < platform_ids + num_platforms; ++i) {
		uint numDevices;
		*err = clGetDeviceIDs(*i, CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
		if(*err != CL_SUCCESS)
			continue;
		cl_device_id deviceIds[numDevices];
		*err = clGetDeviceIDs(*i, CL_DEVICE_TYPE_ALL, numDevices, deviceIds, nullptr);
		if(*err != CL_SUCCESS)
			continue;

		for (cl_device_id* d = deviceIds; d < deviceIds + numDevices; ++d) {
			cl_device_type newDeviceType;
			*err = clGetDeviceInfo(*d, CL_DEVICE_TYPE, sizeof(cl_device_type), &newDeviceType, nullptr);
			if(deviceType == newDeviceType){
				*err = CL_SUCCESS;
				return *d;
			}
		}
	}
	*err = CL_DEVICE_NOT_FOUND;
	return nullptr;
}

string loadProgrammSource(string path){
	string rez;
	ifstream f(path);
	if(f.fail())
		return "";
	string line;
	while (!f.eof()){
		getline(f, line);
		if(f.fail())
			return "";
		rez.append(line).push_back('\n');
	}
	return rez;
}

int main(int argc, char *argv[]){
	int err;
	auto device = getDeviceByType(CL_DEVICE_TYPE_GPU, &err);
	if(err != CL_SUCCESS)
		return -1;

	auto ctx = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	auto commandQue = clCreateCommandQueue(ctx, device, 0, &err);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	size_t datacount = 5;
	auto cmDevSrcA = clCreateBuffer(ctx, CL_MEM_READ_ONLY, sizeof(cl_float) * datacount, nullptr, &err);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;
	auto cmDevSrcB = clCreateBuffer(ctx, CL_MEM_READ_ONLY, sizeof(cl_float) * datacount, nullptr, &err);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;
	auto cmDevDst = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY, sizeof(cl_float) * datacount, nullptr, &err);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	auto source = loadProgrammSource("./vectorAdd.cl");
	qDebug()<<source.empty();
	if(source.empty())
		return -1;
	cout<<source;

	int numProgramm = 1;
	char const *sources[numProgramm];
	sources[0] = source.c_str();
	size_t sourceslengs[numProgramm];
	sourceslengs[0] = source.size();
	cl_program prog = clCreateProgramWithSource(ctx, 1,sources, sourceslengs, &err);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	err = clBuildProgram(prog, 0, nullptr, nullptr, nullptr, nullptr);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	cl_kernel kernel = clCreateKernel(prog, "VectorAdd", &err);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	int argumentIndex = 0;
	clSetKernelArg(kernel, argumentIndex++, sizeof(cl_mem), (void*)&cmDevSrcA);
	clSetKernelArg(kernel, argumentIndex++, sizeof(cl_mem), (void*)&cmDevSrcB);
	clSetKernelArg(kernel, argumentIndex++, sizeof(cl_mem), (void*)&cmDevDst);
	clSetKernelArg(kernel, argumentIndex++, sizeof(cl_int), (void*)&datacount);

	float srcA[] = {4, 4, 6, 9, 2};
	float srcB[] = {9, 5, 9, 7, 6};

	err = clEnqueueWriteBuffer(commandQue, cmDevSrcA, CL_FALSE, 0, sizeof(cl_float) * datacount, srcA, 0, nullptr, nullptr);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;
	err = clEnqueueWriteBuffer(commandQue, cmDevSrcB, CL_FALSE, 0, sizeof(cl_float) * datacount, srcB, 0, nullptr, nullptr);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	err = clEnqueueNDRangeKernel(commandQue, kernel, 1, nullptr, &datacount, nullptr, 0, nullptr, nullptr);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	float dst[datacount];
	err = clEnqueueReadBuffer(commandQue, cmDevDst, CL_TRUE, 0, sizeof(cl_float) * datacount, dst, 0, nullptr, nullptr);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;
	for(auto i: dst){
		qDebug()<<i;
	}

	err = clRetainKernel(kernel);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	err = clReleaseProgram(prog);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	err = clRetainContext(ctx);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	return 0;
}
