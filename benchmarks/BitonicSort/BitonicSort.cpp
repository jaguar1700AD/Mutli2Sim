/* ============================================================

Copyright (c) 2009 Advanced Micro Devices, Inc.  All rights reserved.
 
Redistribution and use of this material is permitted under the following 
conditions:
 
Redistributions must retain the above copyright notice and all terms of this 
license.
 
In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION 
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERA TION, OR THAT IT IS FREE 
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER 
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED 
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT. 
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY 
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES, 
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS 
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS 
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND 
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES, 
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE 
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE 
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR 
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE 
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL 
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR 
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH 
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS 
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS, 
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is 
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to 
computer software and technical data, respectively. Use, duplication, 
distribution or disclosure by the U.S. Government and/or DOD agencies is 
subject to the full extent of restrictions in all applicable regulations, 
including those found at FAR52.227 and DFARS252.227 et seq. and any successor 
regulations thereof. Use of this material by the U.S. Government and/or DOD 
agencies is acknowledgment of the proprietary rights of any copyright holders 
and contributors, including those of Advanced Micro Devices, Inc., as well as 
the provisions of FAR52.227-14 through 23 regarding privately developed and/or 
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be 
modified or waived, and no breach of this license can be excused, unless done 
so in a writing signed by all affected parties. Each term of this license is 
separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.

============================================================ */


#include "BitonicSort.hpp"

int BitonicSort::setupBitonicSort()
{
    cl_uint inputSizeBytes;

    /* allocate and init memory used by host */
    inputSizeBytes = length * sizeof(cl_uint);
    input = (cl_uint *) malloc(inputSizeBytes);
    if(input == NULL)    
    { 
        sampleCommon->error("Failed to allocate host memory. (input)");
        return SDK_FAILURE;
    }

    /* random initialisation of input */
    sampleCommon->fillRandom<cl_uint>(input, length, 1, 0, 255);

    if(verify)
    {
        verificationInput = (cl_uint *) malloc(length*sizeof(cl_int));
        if(verificationInput == NULL)
        { 
            sampleCommon->error("Failed to allocate host memory. (verificationInput)");
                return SDK_FAILURE;
        }
        memcpy(verificationInput, input, length*sizeof(cl_int));
    }

    /* 
     * Unless quiet mode has been enabled, print the INPUT array.
     * No more than 256 values are printed because it clutters the screen
     * and it is not practical to manually compare a large set of numbers
     */
    if(!quiet) 
    {
        sampleCommon->printArray<cl_uint>(
            "Unsorted Input", 
            input, 
            length, 
            1);
    }

    return SDK_SUCCESS;
}


int 
BitonicSort::genBinaryImage()
{
    cl_int status = CL_SUCCESS;

    /*
     * Have a look at the available platforms and pick either
     * the AMD one if available or a reasonable default.
     */
    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetPlatformIDs failed."))
    {
        return SDK_FAILURE;
    }
    if (0 < numPlatforms) 
    {
        cl_platform_id* platforms = new cl_platform_id[numPlatforms];
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clGetPlatformIDs failed."))
        {
            return SDK_FAILURE;
        }

        char platformName[100];
        for (unsigned i = 0; i < numPlatforms; ++i) 
        {
            status = clGetPlatformInfo(platforms[i],
                                       CL_PLATFORM_VENDOR,
                                       sizeof(platformName),
                                       platformName,
                                       NULL);

            if(!sampleCommon->checkVal(status,
                                       CL_SUCCESS,
                                       "clGetPlatformInfo failed."))
            {
                return SDK_FAILURE;
            }

            platform = platforms[i];
            if (!strcmp(platformName, "Advanced Micro Devices, Inc.")) 
            {
                break;
            }
        }
        std::cout << "Platform found : " << platformName << "\n";
        delete[] platforms;
    }

    if(NULL == platform)
    {
        sampleCommon->error("NULL platform found so Exiting Application.");
        return SDK_FAILURE;
    }

    /*
     * If we could find our platform, use it. Otherwise use just available platform.
     */
    cl_context_properties cps[5] = 
    {
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)platform, 
        CL_CONTEXT_OFFLINE_DEVICES_AMD,
        (cl_context_properties)1,
        0
    };

    context = clCreateContextFromType(cps,
                                      CL_DEVICE_TYPE_ALL,
                                      NULL,
                                      NULL,
                                      &status);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateContextFromType failed."))
    {
        return SDK_FAILURE;
    }

    /* create a CL program using the kernel source */
    streamsdk::SDKFile kernelFile;
    std::string kernelPath = sampleCommon->getPath();
    kernelPath.append("BitonicSort_Kernels.cl");
    if(!kernelFile.open(kernelPath.c_str()))
    {
        std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
        return SDK_FAILURE;
    }
    const char * source = kernelFile.source().c_str();
    size_t sourceSize[] = {strlen(source)};
    program = clCreateProgramWithSource(context,
                                        1,
                                        &source,
                                        sourceSize,
                                        &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateProgramWithSource failed."))
    {
        return SDK_FAILURE;
    }
    
    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(program,
                            0,
                            NULL,
                            NULL,
                            NULL,
                            NULL);

    size_t numDevices;
    status = clGetProgramInfo(program, 
                           CL_PROGRAM_NUM_DEVICES,
                           sizeof(numDevices),
                           &numDevices,
                           NULL );
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetProgramInfo(CL_PROGRAM_NUM_DEVICES) failed."))
    {
        return SDK_FAILURE;
    }

    std::cout << "Number of devices found : " << numDevices << "\n\n";
    devices = (cl_device_id *)malloc( sizeof(cl_device_id) * numDevices );
    if(devices == NULL)
    {
        sampleCommon->error("Failed to allocate host memory.(devices)");
        return SDK_FAILURE;
    }
    /* grab the handles to all of the devices in the program. */
    status = clGetProgramInfo(program, 
                              CL_PROGRAM_DEVICES, 
                              sizeof(cl_device_id) * numDevices,
                              devices,
                              NULL );
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetProgramInfo(CL_PROGRAM_DEVICES) failed."))
    {
        return SDK_FAILURE;
    }


    /* figure out the sizes of each of the binaries. */
    size_t *binarySizes = (size_t*)malloc( sizeof(size_t) * numDevices );
    if(devices == NULL)
    {
        sampleCommon->error("Failed to allocate host memory.(binarySizes)");
        return SDK_FAILURE;
    }
    
    status = clGetProgramInfo(program, 
                              CL_PROGRAM_BINARY_SIZES,
                              sizeof(size_t) * numDevices, 
                              binarySizes, NULL);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetProgramInfo(CL_PROGRAM_BINARY_SIZES) failed."))
    {
        return SDK_FAILURE;
    }

    size_t i = 0;
    /* copy over all of the generated binaries. */
    char **binaries = (char **)malloc( sizeof(char *) * numDevices );
    if(binaries == NULL)
    {
        sampleCommon->error("Failed to allocate host memory.(binaries)");
        return SDK_FAILURE;
    }

    for(i = 0; i < numDevices; i++)
    {
        if(binarySizes[i] != 0)
        {
            binaries[i] = (char *)malloc( sizeof(char) * binarySizes[i]);
            if(binaries[i] == NULL)
            {
                sampleCommon->error("Failed to allocate host memory.(binaries[i])");
                return SDK_FAILURE;
            }
        }
        else
        {
            binaries[i] = NULL;
        }
    }
    status = clGetProgramInfo(program, 
                              CL_PROGRAM_BINARIES,
                              sizeof(char *) * numDevices, 
                              binaries, 
                              NULL);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetProgramInfo(CL_PROGRAM_BINARIES) failed."))
    {
        return SDK_FAILURE;
    }

    /* dump out each binary into its own separate file. */
    for(i = 0; i < numDevices; i++)
    {
        char fileName[100];
        sprintf(fileName, "%s.%d", dumpBinary.c_str(), (int)i);
        if(binarySizes[i] != 0)
        {
            char deviceName[1024];
            status = clGetDeviceInfo(devices[i], 
                                     CL_DEVICE_NAME, 
                                     sizeof(deviceName),
                                     deviceName, 
                                     NULL);
            if(!sampleCommon->checkVal(status,
                                       CL_SUCCESS,
                                       "clGetDeviceInfo(CL_DEVICE_NAME) failed."))
            {
                return SDK_FAILURE;
            }

            printf( "%s binary kernel: %s\n", deviceName, fileName);
            streamsdk::SDKFile BinaryFile;
            if(!BinaryFile.writeBinaryToFile(fileName, 
                                             binaries[i], 
                                             binarySizes[i]))
            {
                std::cout << "Failed to load kernel file : " << fileName << std::endl;
                return SDK_FAILURE;
            }
        }
        else
        {
            printf("Skipping %s since there is no binary data to write!\n",
                    fileName);
        }
    }

    // Release all resouces and memory
    for(i = 0; i < numDevices; i++)
    {
        if(binaries[i] != NULL)
        {
            free(binaries[i]);
            binaries[i] = NULL;
        }
    }

    if(binaries != NULL)
    {
        free(binaries);
        binaries = NULL;
    }

    if(binarySizes != NULL)
    {
        free(binarySizes);
        binarySizes = NULL;
    }

    if(devices != NULL)
    {
        free(devices);
        devices = NULL;
    }

    status = clReleaseProgram(program);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseProgram failed."))
    {
        return SDK_FAILURE;
    }

    status = clReleaseContext(context);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseContext failed."))
    {
        return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}

int
BitonicSort::setupCL(void)
{
    cl_int status = 0;
    size_t deviceListSize;

    cl_device_type dType;
    
    if(deviceType.compare("cpu") == 0)
    {
        dType = CL_DEVICE_TYPE_CPU;
    }
    else //deviceType = "gpu" 
    {
        dType = CL_DEVICE_TYPE_GPU;
    }

    /*
     * Have a look at the available platforms and pick either
     * the AMD one if available or a reasonable default.
     */

    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetPlatformIDs failed."))
    {
        return SDK_FAILURE;
    }
    if (0 < numPlatforms) 
    {
        cl_platform_id* platforms = new cl_platform_id[numPlatforms];
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clGetPlatformIDs failed."))
        {
            return SDK_FAILURE;
        }
        for (unsigned i = 0; i < numPlatforms; ++i) 
        {
            char pbuf[100];
            status = clGetPlatformInfo(platforms[i],
                                       CL_PLATFORM_VENDOR,
                                       sizeof(pbuf),
                                       pbuf,
                                       NULL);

            if(!sampleCommon->checkVal(status,
                                       CL_SUCCESS,
                                       "clGetPlatformInfo failed."))
            {
                return SDK_FAILURE;
            }

            platform = platforms[i];
            if (!strcmp(pbuf, "Advanced Micro Devices, Inc.")) 
            {
                break;
            }
        }
        delete[] platforms;
    }

    if(NULL == platform)
    {
        sampleCommon->error("NULL platform found so Exiting Application.");
        return SDK_FAILURE;
    }

    // Display available devices.
    if(!sampleCommon->displayDevices(platform, dType))
    {
        sampleCommon->error("sampleCommon::displayDevices() failed");
        return SDK_FAILURE;
    }

    /*
     * If we could find our platform, use it. Otherwise use just available platform.
     */

    cl_context_properties cps[3] = 
    {
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)platform, 
        0
    };
    
    context = clCreateContextFromType(
                  cps,
                  dType,
                  NULL,
                  NULL,
                  &status);

    if(!sampleCommon->checkVal(status, 
                  CL_SUCCESS,
                  "clCreateContextFromType failed."))
        return SDK_FAILURE;

    /* First, get the size of device list data */
    status = clGetContextInfo(
                 context, 
                 CL_CONTEXT_DEVICES, 
                 0, 
                 NULL, 
                 &deviceListSize);
    if(!sampleCommon->checkVal(
            status, 
            CL_SUCCESS,
            "clGetContextInfo failed."))
        return SDK_FAILURE;

    int deviceCount = (int)(deviceListSize / sizeof(cl_device_id));
    if(!sampleCommon->validateDeviceId(deviceId, deviceCount))
    {
        sampleCommon->error("sampleCommon::validateDeviceId() failed");
        return SDK_FAILURE;
    }

    /* Now allocate memory for device list based on the size we got earlier */
    devices = (cl_device_id *)malloc(deviceListSize);
    if(devices == NULL)
    {
        sampleCommon->error("Failed to allocate memory (devices).");
        return SDK_FAILURE;
    }

    /* Now, get the device list data */
    status = clGetContextInfo(
                 context, 
                 CL_CONTEXT_DEVICES, 
                 deviceListSize, 
                 devices, 
                 NULL);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS, 
            "clGetGetContextInfo failed."))
        return SDK_FAILURE;

    {
        /* The block is to move the declaration of prop closer to its use */
        cl_command_queue_properties prop = 0;
        if(timing)
            prop |= CL_QUEUE_PROFILING_ENABLE;

        commandQueue = clCreateCommandQueue(
                           context, 
                           devices[deviceId], 
                           prop, 
                           &status);
        if(!sampleCommon->checkVal(
                status,
                0,
                "clCreateCommandQueue failed."))
            return SDK_FAILURE;
    }

    inputBuffer = clCreateBuffer(
                      context, 
                      CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                      sizeof(cl_uint) * length,
                      input, 
                      &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateBuffer failed. (inputBuffer)"))
        return SDK_FAILURE;

    /* create a CL program using the kernel source */
    streamsdk::SDKFile kernelFile;

    /* Get path of current executable */
    std::string kernelPath = sampleCommon->getPath();
    if(isLoadBinaryEnabled())
    {
        kernelPath.append(loadBinary.c_str());
        if(!kernelFile.readBinaryFromFile(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }

        const char * binary = kernelFile.source().c_str();
        size_t binarySize = kernelFile.source().size();
        program = clCreateProgramWithBinary(context,
                                            1,
                                            &devices[deviceId], 
                                            (const size_t*)&binarySize,
                                            (const unsigned char**)&binary,
                                            NULL,
                                            &status);
    }
    else
    {
        kernelPath.append("BitonicSort_Kernels.cl");
        if(!kernelFile.open(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }
        const char * source = kernelFile.source().c_str();
        size_t sourceSize[] = { strlen(source) };
        program = clCreateProgramWithSource(context,
                                            1,
                                            &source,
                                            sourceSize,
                                            &status);
    }
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateProgramWithSource failed."))
        return SDK_FAILURE;

    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(program, 1, &devices[deviceId], NULL, NULL, NULL);
    if(status != CL_SUCCESS)
    {
        if(status == CL_BUILD_PROGRAM_FAILURE)
        {
            cl_int logStatus;
            char * buildLog = NULL;
            size_t buildLogSize = 0;
            logStatus = clGetProgramBuildInfo(program,
                                              devices[deviceId],
                                              CL_PROGRAM_BUILD_LOG,
                                              buildLogSize,
                                              buildLog,
                                              &buildLogSize);
            if(!sampleCommon->checkVal(logStatus,
                                       CL_SUCCESS,
                                       "clGetProgramBuildInfo failed."))
            {
                  return SDK_FAILURE;
            }
            
            buildLog = (char*)malloc(buildLogSize);
            if(buildLog == NULL)
            {
                sampleCommon->error("Failed to allocate host memory. (buildLog)");
                return SDK_FAILURE;
            }
            memset(buildLog, 0, buildLogSize);

            logStatus = clGetProgramBuildInfo(program, 
                                              devices[deviceId], 
                                              CL_PROGRAM_BUILD_LOG, 
                                              buildLogSize, 
                                              buildLog, 
                                              NULL);
            if(!sampleCommon->checkVal(logStatus,
                                       CL_SUCCESS,
                                       "clGetProgramBuildInfo failed."))
            {
                  free(buildLog);
                  return SDK_FAILURE;
            }

            std::cout << " \n\t\t\tBUILD LOG\n";
            std::cout << " ************************************************\n";
            std::cout << buildLog << std::endl;
            std::cout << " ************************************************\n";
            free(buildLog);
        }

          if(!sampleCommon->checkVal(status,
                                     CL_SUCCESS,
                                     "clBuildProgram failed."))
          {
                return SDK_FAILURE;
          }
    }

    /* get a kernel object handle for a kernel with the given name */
    kernel = clCreateKernel(program, "bitonicSort", &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateKernel failed."))
        return SDK_FAILURE;

    return SDK_SUCCESS;
}


int 
BitonicSort::runCLKernels(void)
{
    cl_int   status;
    cl_event events[2];

    cl_uint numStages = 0;
    cl_uint temp;

    cl_uint stage;
    cl_uint passOfStage;

    size_t globalThreads[1] = {length / 2};
    size_t localThreads[1] = {GROUP_SIZE};


    /* Check group size against kernelWorkGroupSize */
    status = clGetKernelWorkGroupInfo(kernel,
                                      devices[deviceId],
                                      CL_KERNEL_WORK_GROUP_SIZE,
                                      sizeof(size_t),
                                      &kernelWorkGroupSize,
                                      0);
    if(!sampleCommon->checkVal(
                        status,
                        CL_SUCCESS, 
                        "clGetKernelWorkGroupInfo failed."))
    {
        return SDK_FAILURE;
    }

    if((cl_uint)(localThreads[0]) > kernelWorkGroupSize)
    {
		if(!quiet)
		{
			std::cout << "Out of Resources!" << std::endl;
			std::cout << "Group Size specified : " << localThreads[0] << std::endl;
			std::cout << "Max Group Size supported on the kernel : " 
					  << kernelWorkGroupSize << std::endl;
			std::cout<<"Changing the group size to " << kernelWorkGroupSize << "\n";
		}
		localThreads[0] = kernelWorkGroupSize;
    }

    /*
     * This algorithm is run as NS stages. Each stage has NP passes.
     * so the total number of times the kernel call is enqueued is NS * NP.
     *
     * For every stage S, we have S + 1 passes.
     * eg: For stage S = 0, we have 1 pass.
     *     For stage S = 1, we have 2 passes.
     *
     * if length is 2^N, then the number of stages (numStages) is N.
     * Do keep in mind the fact that the algorithm only works for
     * arrays whose size is a power of 2.
     * 
     * here, numStages is N.
     *
     * For an explanation of how the algorithm works, please go through
     * the documentation of this sample.
     */

    /*
     * 2^numStages should be equal to length.
     * i.e the number of times you halve length to get 1 should be numStages 
     */
    for(temp = length; temp > 1; temp >>= 1)
        ++numStages;

    /*** Set appropriate arguments to the kernel ***/
    /* the input array - also acts as output for this pass (input for next) */
    status = clSetKernelArg(
                    kernel, 
                    0, 
                    sizeof(cl_mem), 
                    (void *)&inputBuffer);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clSetKernelArg failed. (inputBuffer)"))
        return SDK_FAILURE;

    /* length - i.e number of elements in the array */
    status = clSetKernelArg(
                    kernel, 
                    3, 
                    sizeof(cl_uint), 
                    (void *)&length);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clSetKernelArg failed. (length)"))
        return SDK_FAILURE;

    /* whether sort is to be in increasing order. CL_TRUE implies increasing */
    status = clSetKernelArg(
                 kernel, 
                 4, 
                 sizeof(cl_uint), 
                 (void *)&sortDescending); 
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clSetKernelArg failed. (increasing)"))
        return SDK_FAILURE;

    for(stage = 0; stage < numStages; ++stage) {
        /* stage of the algorithm */
        status = clSetKernelArg(
                        kernel, 
                        1, 
                        sizeof(cl_uint), 
                        (void *)&stage);
        if(!sampleCommon->checkVal(
                status,
                CL_SUCCESS,
                "clSetKernelArg failed. (stage)"))
            return SDK_FAILURE;
        
        /* Every stage has stage+1 passes. */
        for(passOfStage = 0; passOfStage < stage + 1; ++passOfStage) {
            /* pass of the current stage */
            status = clSetKernelArg(
                         kernel,
                         2, 
                         sizeof(cl_uint), 
                         (void *)&passOfStage);
            if(!sampleCommon->checkVal(
                    status,
                    CL_SUCCESS,
                    "clSetKernelArg failed. (passOfStage)"))
                return SDK_FAILURE;

            /* 
             * Enqueue a kernel run call.
             * For simplicity, the groupsize used is 1.
             * 
             * Each thread writes a sorted pair.
             * So, the number of  threads (global) is half the length.
             */
            status = clEnqueueNDRangeKernel(
                         commandQueue,
                         kernel,
                         1,
                         NULL,
                         globalThreads,
                         localThreads,
                         0,
                         NULL,
                         &events[0]);
            
            if(!sampleCommon->checkVal(
                    status,
                    CL_SUCCESS,
                    "clEnqueueNDRangeKernel failed."))
                return SDK_FAILURE;


            /* wait for the kernel call to finish execution */
            status = clWaitForEvents(1, &events[0]);
            if(!sampleCommon->checkVal(
                    status,
                    CL_SUCCESS,
                    "clWaitForEvents failed."))
                return SDK_FAILURE;

        }
   }

    /* Enqueue readBuffer*/
    status = clEnqueueReadBuffer(
                commandQueue,
                inputBuffer,
                CL_TRUE,
                0,
                length * sizeof(cl_uint),
                input,
                0,
                NULL,
                &events[1]);
    if(!sampleCommon->checkVal(
    		status,
    		CL_SUCCESS,
    		"clEnqueueReadBuffer failed."))
    	return SDK_FAILURE;
    
    /* Wait for the read buffer to finish execution */
    status = clWaitForEvents(1, &events[1]);
    if(!sampleCommon->checkVal(
    		status,
    		CL_SUCCESS,
    		"clWaitForEvents failed."))
    	return SDK_FAILURE;

    clReleaseEvent(events[1]);
    return SDK_SUCCESS;
}

void 
BitonicSort::swapIfFirstIsGreater(cl_uint *a, cl_uint *b)
{
    if(*a > *b) 
    {
        cl_uint temp = *a;
        *a = *b;
        *b = temp;
    }
}


/*
 * sorts the input array (in place) using the bitonic sort algorithm
 * sorts in increasing order if sortIncreasing is CL_TRUE
 * else sorts in decreasing order
 * length specifies the length of the array
 */
void 
BitonicSort::bitonicSortCPUReference(
    cl_uint * input, 
    const cl_uint length, 
    const cl_bool sortIncreasing) 
{
    const cl_uint halfLength = length/2;

    cl_uint i;
    for(i = 2; i <= length; i *= 2) 
    {
        cl_uint j;
        for(j = i; j > 1; j /= 2) 
        {
            cl_bool increasing = sortIncreasing;
            const cl_uint half_j = j/2;

            cl_uint k;
            for(k = 0; k < length; k += j) 
            {
                const cl_uint k_plus_half_j = k + half_j;
                cl_uint l;

                if(i < length) 
                {
                    if((k == i) || ((k % i) == 0) && (k != halfLength)) 
                        increasing = !increasing;
                }

                for(l = k; l < k_plus_half_j; ++l) 
                {
                    if(increasing)
                        swapIfFirstIsGreater(&input[l], &input[l + half_j]);
                    else
                        swapIfFirstIsGreater(&input[l + half_j], &input[l]);
                }
            }
        }
    }
}

int BitonicSort::initialize()
{
    // Call base class Initialize to get default configuration
    if(!this->SDKSample::initialize())
        return SDK_FAILURE;

    // Now add customized options
    streamsdk::Option* array_length = new streamsdk::Option;
    if(!array_length)
    {
        sampleCommon->error("Memory allocation error.\n");
        return SDK_FAILURE;
    }
    
    array_length->_sVersion = "x";
    array_length->_lVersion = "length";
    array_length->_description = "Length of the array to be sorted";
    array_length->_type = streamsdk::CA_ARG_INT;
    array_length->_value = &length;
    sampleArgs->AddOption(array_length);

    delete array_length;
   
    streamsdk::Option* sort_order = new streamsdk::Option;
    if(!sort_order)
    {
        sampleCommon->error("Memory allocation error.\n");
        return SDK_FAILURE;
    }
    sortDescending = 0;
    sort_order->_sVersion = "de";
    sort_order->_lVersion = "desc";
    sort_order->_description = "Sort in descending order";
    sort_order->_type = streamsdk::CA_ARG_INT;
    sort_order->_value = &sortDescending;
    sampleArgs->AddOption(sort_order);

    delete sort_order;

    streamsdk::Option* num_iterations = new streamsdk::Option;
    if(!num_iterations)
    {
        sampleCommon->error("Memory allocation error.\n");
        return SDK_FAILURE;
    }

    num_iterations->_sVersion = "i";
    num_iterations->_lVersion = "iterations";
    num_iterations->_description = "Number of iterations for kernel execution";
    num_iterations->_type = streamsdk::CA_ARG_INT;
    num_iterations->_value = &iterations;

    sampleArgs->AddOption(num_iterations);

    delete num_iterations;

    return SDK_SUCCESS;
}

int BitonicSort::setup()
{
    if(!sampleCommon->isPowerOf2(length))
        length = sampleCommon->roundToPowerOf2(length);
    if(setupBitonicSort()!=SDK_SUCCESS)
        return SDK_FAILURE;

    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);

    if(setupCL()!=SDK_SUCCESS)
        return SDK_FAILURE;

    sampleCommon->stopTimer(timer);

    setupTime = (cl_double)sampleCommon->readTimer(timer);

    return SDK_SUCCESS;
}


int BitonicSort::run()
{
    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);   

    std::cout << "Executing kernel for " << iterations << 
        " iterations" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for(int i = 0; i < iterations; i++)
    {
        /* Arguments are set and execution call is enqueued on command buffer */
        if(runCLKernels()!=SDK_SUCCESS)
            return SDK_FAILURE;
    }
    sampleCommon->stopTimer(timer);
    totalKernelTime = (double)(sampleCommon->readTimer(timer)) / iterations;


    if(!quiet) {
        sampleCommon->printArray<cl_uint>("Output", input, length, 1);
    }

    return SDK_SUCCESS;
}

int BitonicSort::verifyResults()
{
    if(verify)
    {

        /* reference implementation
         * it overwrites the input array with the output
         */
        int refTimer = sampleCommon->createTimer();
        sampleCommon->resetTimer(refTimer);
        sampleCommon->startTimer(refTimer);
        bitonicSortCPUReference(verificationInput, length, sortDescending);
        sampleCommon->stopTimer(refTimer);
        referenceKernelTime = sampleCommon->readTimer(refTimer);

        /* compare the results and see if they match */
        if(memcmp(input, verificationInput, length*sizeof(cl_uint)) == 0)
        {
            std::cout<<"Passed!\n";
            return SDK_SUCCESS;
        }
        else
        {
            std::cout<<"Failed\n";
            return SDK_FAILURE;
        }
    }

    return SDK_SUCCESS;
}

void BitonicSort::printStats()
{
    std::string strArray[3] = {"Length", "Time(sec)", "kernelTime(sec)"};
    std::string stats[3];

    totalTime = setupTime + totalKernelTime;
    
    stats[0]  = sampleCommon->toString(length, std::dec);
    stats[1]  = sampleCommon->toString(totalTime, std::dec);
	stats[2]  = sampleCommon->toString(totalKernelTime, std::dec);
    
    this->SDKSample::printStats(strArray, stats, 3);
}

int BitonicSort::cleanup()
{
    /* Releases OpenCL resources (Context, Memory etc.) */
    cl_int status;

    status = clReleaseKernel(kernel);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseKernel failed."))
        return SDK_FAILURE;

    status = clReleaseProgram(program);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseProgram failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(inputBuffer);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
        return SDK_FAILURE;

   status = clReleaseCommandQueue(commandQueue);
     if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseCommandQueue failed."))
        return SDK_FAILURE;

    status = clReleaseContext(context);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clReleaseContext failed."))
        return SDK_FAILURE;

    /* release program resources (input memory etc.) */
    if(input) 
        free(input);

    if(verificationInput) 
        free(verificationInput);

    if(devices)
        free(devices);

    return SDK_SUCCESS;
}



int 
main(int argc, char * argv[])
{
    BitonicSort clBitonicSort("OpenCL Bitonic Sort");

    clBitonicSort.initialize();
    if(!clBitonicSort.parseCommandLine(argc, argv))
        return SDK_FAILURE;
    if(clBitonicSort.isDumpBinaryEnabled())
    {
        return clBitonicSort.genBinaryImage();
    }
    else
    {
        if(clBitonicSort.setup()!=SDK_SUCCESS)
		    return SDK_FAILURE;
        if(clBitonicSort.run()!=SDK_SUCCESS)
		    return SDK_FAILURE;
        if(clBitonicSort.verifyResults()!=SDK_SUCCESS)
		    return SDK_FAILURE;
        if(clBitonicSort.cleanup()!=SDK_SUCCESS)
		    return SDK_FAILURE;
        clBitonicSort.printStats();
    }
    

    return SDK_SUCCESS;
}
