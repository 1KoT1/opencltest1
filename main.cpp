#include <QDebug>
#include <CL/opencl.h>

using namespace std;

QString errToStr(int err){
	switch (err) {
		case CL_SUCCESS:
			return "CL_SUCCESS";
		case CL_DEVICE_NOT_FOUND:
			return "CL_DEVICE_NOT_FOUND";
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

int main(int argc, char *argv[])
{

	qDebug()<<"qqq";

	uint num_platforms;
	auto err = clGetPlatformIDs(0, nullptr, &num_platforms);
	qDebug()<<errToStr(err)<<num_platforms;
	if(err != CL_SUCCESS)
		return -1;
	cl_platform_id platform_ids[num_platforms];
	err = clGetPlatformIDs(num_platforms, platform_ids, nullptr);
	qDebug()<<errToStr(err);
	if(err != CL_SUCCESS)
		return -1;
	for (cl_platform_id* i = platform_ids; i < platform_ids + num_platforms; ++i) {
		size_t platformProfileSize = 16;
		char platformProfile[platformProfileSize];
		err = clGetPlatformInfo(*i, CL_PLATFORM_PROFILE, platformProfileSize, platformProfile, nullptr);
		qDebug()<<errToStr(err)<<platformProfile;

		size_t platformVersionSize = 100;
		char platformVersion[platformVersionSize];
		err = clGetPlatformInfo(*i, CL_PLATFORM_VERSION, platformVersionSize, platformVersion, nullptr);
		qDebug()<<errToStr(err)<<platformVersion;

		size_t platformNameSize = 100;
		char platformName[platformNameSize];
		err = clGetPlatformInfo(*i, CL_PLATFORM_NAME, platformNameSize, platformName, nullptr);
		qDebug()<<errToStr(err)<<platformName;

		qDebug()<<QObject::trUtf8("Устройства");
		uint numDevices;
		err = clGetDeviceIDs(*i, CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
		qDebug()<<errToStr(err)<<numDevices;
		if(err != CL_SUCCESS)
			continue;
		cl_device_id deviceIds[numDevices];
		err = clGetDeviceIDs(*i, CL_DEVICE_TYPE_ALL, numDevices, deviceIds, nullptr);
		qDebug()<<errToStr(err);
		if(err != CL_SUCCESS)
			continue;

		for (cl_device_id* d = deviceIds; d < deviceIds + numDevices; ++d) {
			cl_device_type deviceType;
			err = clGetDeviceInfo(*d, CL_DEVICE_TYPE, sizeof(deviceType), &deviceType, nullptr);
			qDebug()<<errToStr(err)<<deviceTypeToStr(deviceType);
		}
	}
}
