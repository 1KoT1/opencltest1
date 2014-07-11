#include <QDebug>
#include <CL/opencl.h>

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

int main(int argc, char *argv[])
{
	int err;
	auto device = getDeviceByType(CL_DEVICE_TYPE_GPU, &err);
	if(err != CL_SUCCESS)
		return -1;

	auto ctx = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

	err = clRetainContext(ctx);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;

return 0;
}
