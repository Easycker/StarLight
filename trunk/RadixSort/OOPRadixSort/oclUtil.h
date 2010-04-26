#pragma once

void oclCheckError(cl_int errorCode, const char * msg = NULL);

void oclPrintBuildLog(cl_device_id device, cl_program program);

void oclPrintDeviceDescription ( cl_device_id device );

void oclPrintPlatformInfo (cl_platform_id platform, cl_platform_info paramName, char* paramNameStr );

void oclPrintPlatformDescription( cl_platform_id platform );

