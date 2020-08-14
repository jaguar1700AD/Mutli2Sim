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


#include "SimpleImage.hpp"
#include <cmath>


int
SimpleImage::readInputImage(std::string inputImageName)
{

    /* load input bitmap image */
    inputBitmap.load(inputImageName.c_str());

    /* error if image did not load */
    if(!inputBitmap.isLoaded())
    {
        sampleCommon->error("Failed to load input image!");
        return SDK_FAILURE;
    }

    /* get width and height of input image */
    height = inputBitmap.getHeight();
    width = inputBitmap.getWidth();

    /* allocate memory for input & output image data  */
    inputImageData  = (cl_uchar4*)malloc(width * height * sizeof(cl_uchar4));


    /* error check */
    if(inputImageData == NULL)
    {
        sampleCommon->error("Failed to allocate memory! (inputImageData)");
        return SDK_FAILURE;
    }

    /* allocate memory for 2D-copy output image data */
    outputImageData2D = (cl_uchar4*)malloc(width * height * sizeof(cl_uchar4));
    /* error check */
    if(outputImageData2D == NULL)
    {
        sampleCommon->error("Failed to allocate memory! (outputImageData)");
        return SDK_FAILURE;
    }

    /* allocate memory for 3D-copy output image data */
    outputImageData3D = (cl_uchar4*)malloc(width * height * sizeof(cl_uchar4));
    /* error check */
    if(outputImageData3D == NULL)
    {
        sampleCommon->error("Failed to allocate memory! (outputImageData)");
        return SDK_FAILURE;
    }

    /* initializa the Image data to NULL */
    memset(outputImageData2D, 0, width * height * pixelSize);
    memset(outputImageData3D, 0, width * height * pixelSize);

    /* get the pointer to pixel data */
    pixelData = inputBitmap.getPixels();

    /* error check */
    if(pixelData == NULL)
    {
        sampleCommon->error("Failed to read pixel Data!");
        return SDK_FAILURE;
    }


    /* Copy pixel data into inputImageData */
    memcpy(inputImageData, pixelData, width * height * pixelSize);

    /* allocate memory for verification output */
    verificationOutput = (cl_uchar*)malloc(width * height * pixelSize);
    /* error check */
    if(verificationOutput == NULL)
    {
        sampleCommon->error("verificationOutput heap allocation failed!");
        return SDK_FAILURE;
    }
    /* initialize the data to NULL */
    //memset(verificationOutput, 0, width * height * pixelSize);
    memcpy(verificationOutput, inputImageData, width * height * pixelSize);

    return SDK_SUCCESS;

}


int
SimpleImage::writeOutputImage(std::string outputImageName)
{
    /* copy output image data back to original pixel data */
    memcpy(pixelData, outputImageData2D, width * height * pixelSize);

    /* write the output bmp file */
    if(!inputBitmap.write(outputImageName.c_str()))
    {
        sampleCommon->error("Failed to write output image!");
        return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}

int 
SimpleImage::genBinaryImage()
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
    kernelPath.append("SimpleImage_Kernels.cl");
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
    
    std::string flagsStr = std::string("");

    // Get additional options
    if(isComplierFlagsSpecified())
    {
        streamsdk::SDKFile flagsFile;
        std::string flagsPath = sampleCommon->getPath();
        flagsPath.append(flags.c_str());
        if(!flagsFile.open(flagsPath.c_str()))
        {
            std::cout << "Failed to load flags file: " << flagsPath << std::endl;
            return SDK_FAILURE;
        }
        flagsFile.replaceNewlineWithSpaces();
        const char * flags = flagsFile.source().c_str();
        flagsStr.append(flags);
    }

    if(flagsStr.size() != 0)
        std::cout << "Build Options are : " << flagsStr.c_str() << std::endl;


    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(program,
                            0,
                            NULL,
                            flagsStr.c_str(),
                            NULL,
                            NULL);

    sampleCommon->checkVal(status,
                        CL_SUCCESS,
                        "clBuildProgram failed.");

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
SimpleImage::setupCL()
{
    cl_int status = CL_SUCCESS;
    cl_device_type dType;

    if(deviceType.compare("cpu") == 0)
    {
        dType = CL_DEVICE_TYPE_CPU;
    }
    else //deviceType = "gpu" 
    {
        dType = CL_DEVICE_TYPE_GPU;
        if(isThereGPU() == false)
        {
            std::cout << "GPU not found. Falling back to CPU device" << std::endl;
            dType = CL_DEVICE_TYPE_CPU;
        }
    }

    size_t deviceListSize;

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
        if(isPlatformEnabled())
        {
            platform = platforms[platformId];
        }
        else
        {
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
    {
        return SDK_FAILURE;
    }

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
    devices = (cl_device_id*)malloc(deviceListSize);
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
        "clGetContextInfo failed."))
        return SDK_FAILURE;

    /* Check for image support */
    status = clGetDeviceInfo(devices[deviceId],
                             CL_DEVICE_IMAGE_SUPPORT,
                             sizeof(cl_bool),
                             &imageSupport,
                             0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetDeviceInfo failed."))
        return SDK_FAILURE;

    if(!imageSupport)
    {
        sampleCommon->expectedError("Images are not supported on this device!");
        return SDK_EXPECTED_FAILURE;
    }
    /* Create command queue */

    cl_command_queue_properties prop = 0;
    commandQueue = clCreateCommandQueue(
        context,
        devices[deviceId],
        prop,
        &status);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateCommandQueue failed."))
    {
        return SDK_FAILURE;
    }

    /*
    * Create and initialize image objects
    */
    /* Create 2D input image */
    inputImage2D = clCreateImage2D(context,
                                   CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                                   &imageFormat,
                                   width,
                                   height,
                                   0,
                                   inputImageData,
                                   &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (inputImageBuffer)"))
    {
        return SDK_FAILURE;
    }

    /* Create 2D output image */
    outputImage2D = clCreateImage2D(context,
                                   CL_MEM_WRITE_ONLY,
                                   &imageFormat,
                                   width,
                                   height,
                                   0,
                                   0,
                                   &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (inputImageBuffer)"))
    {
        return SDK_FAILURE;
    }

    /* Create 3D input image */
    inputImage3D = clCreateImage3D(context,
                                   CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                                   &imageFormat,
                                   width,
                                   height / 2,  
                                   2,           //2 slices
                                   0,
                                   0,
                                   inputImageData,
                                   &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (inputImageBuffer)"))
    {
        return SDK_FAILURE;
    }

    /* Writes to 3D images not allowed in spec currently */
    outputImage3D = clCreateImage2D(context,
                                   CL_MEM_WRITE_ONLY,
                                   &imageFormat,
                                   width,
                                   height,
                                   0,
                                   0,
                                   &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (inputImageBuffer)"))
    {
        return SDK_FAILURE;
    }

    if(!sampleCommon->checkVal(status,
        CL_SUCCESS,
        "clCreateBuffer failed. (outputImageBuffer)"))
    {
        return SDK_FAILURE;
    }

    /* create a CL program using the kernel source */
    streamsdk::SDKFile kernelFile;
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
                                            (const size_t *)&binarySize,
                                            (const unsigned char**)&binary,
                                            NULL,
                                            &status);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clCreateProgramWithBinary failed."))
        {
            return SDK_FAILURE;
        }

    }
    else
    {
        kernelPath.append("SimpleImage_Kernels.cl");
        if(!kernelFile.open(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : "<< kernelPath << std::endl;
            return SDK_FAILURE;
        }
        const char *source = kernelFile.source().c_str();
        size_t sourceSize[] = {strlen(source)};
        program = clCreateProgramWithSource(context,
            1,
            &source,
            sourceSize,
            &status);
        if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateProgramWithSource failed."))
            return SDK_FAILURE;
    }

    std::string flagsStr = std::string("");

    // Get additional options
    if(isComplierFlagsSpecified())
    {
        streamsdk::SDKFile flagsFile;
        std::string flagsPath = sampleCommon->getPath();
        flagsPath.append(flags.c_str());
        if(!flagsFile.open(flagsPath.c_str()))
        {
            std::cout << "Failed to load flags file: " << flagsPath << std::endl;
            return SDK_FAILURE;
        }
        flagsFile.replaceNewlineWithSpaces();
        const char * flags = flagsFile.source().c_str();
        flagsStr.append(flags);
    }

    if(flagsStr.size() != 0)
        std::cout << "Build Options are : " << flagsStr.c_str() << std::endl;

    

    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(program, 
                            1, 
                            &devices[deviceId], 
                            flagsStr.c_str(), 
                            NULL, 
                            NULL);
    if(status != CL_SUCCESS)
    {
        if(status == CL_BUILD_PROGRAM_FAILURE)
        {
            cl_int logStatus;
            char *buildLog = NULL;
            size_t buildLogSize = 0;
            logStatus = clGetProgramBuildInfo (program, 
                devices[deviceId], 
                CL_PROGRAM_BUILD_LOG, 
                buildLogSize, 
                buildLog, 
                &buildLogSize);
            if(!sampleCommon->checkVal(
                logStatus,
                CL_SUCCESS,
                "clGetProgramBuildInfo failed."))
                return SDK_FAILURE;

            buildLog = (char*)malloc(buildLogSize);
            if(buildLog == NULL)
            {
                sampleCommon->error("Failed to allocate host memory. (buildLog)");
                return SDK_FAILURE;
            }
            memset(buildLog, 0, buildLogSize);

            logStatus = clGetProgramBuildInfo (program, 
                devices[deviceId], 
                CL_PROGRAM_BUILD_LOG, 
                buildLogSize, 
                buildLog, 
                NULL);
            if(!sampleCommon->checkVal(
                logStatus,
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

        if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clBuildProgram failed."))
            return SDK_FAILURE;
    }

    /* get a kernel object handle for a kernel with the given name */
    kernel2D = clCreateKernel(program, "image2dCopy", &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateKernel failed."))
    {
        return SDK_FAILURE;
    }

    kernel3D = clCreateKernel(program, "image3dCopy", &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateKernel failed."))
    {
        return SDK_FAILURE;
    }

    /* Check group size against group size returned by kernel */
    status = clGetKernelWorkGroupInfo(kernel2D,
        devices[deviceId],
        CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(size_t),
        &kernel2DWorkGroupSize,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetKernelWorkGroupInfo  failed."))
    {
        return SDK_FAILURE;
    }

    /* Check group size against group size returned by kernel */
    status = clGetKernelWorkGroupInfo(kernel3D,
        devices[deviceId],
        CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(size_t),
        &kernel3DWorkGroupSize,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetKernelWorkGroupInfo  failed."))
    {
        return SDK_FAILURE;
    }

    cl_uint temp = (cl_uint)min(kernel2DWorkGroupSize, kernel3DWorkGroupSize);

    if((blockSizeX * blockSizeY) > temp)
    {
        if(!quiet)
        {
            std::cout << "Out of Resources!" << std::endl;
            std::cout << "Group Size specified : "
                      << blockSizeX * blockSizeY << std::endl;
            std::cout << "Max Group Size supported on the kernel(s) : " 
                      << temp << std::endl;
            std::cout << "Falling back to " << temp << std::endl;
        }

        if(blockSizeX > temp)
        {
            blockSizeX = temp;
            blockSizeY = 1;
        }

    }

    return SDK_SUCCESS;
}

int 
SimpleImage::runCLKernels()
{
    cl_int status;

    /* Set appropriate arguments to the kernel2D */

    /* input buffer image */
    status = clSetKernelArg(
        kernel2D,
        0,
        sizeof(cl_mem),
        &inputImage2D);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (inputImage2D)"))
    {
        return SDK_FAILURE;
    }

    /* outBuffer image */
    status = clSetKernelArg(
        kernel2D,
        1,
        sizeof(cl_mem),
        &outputImage2D);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (outputImage2D)"))
    {
        return SDK_FAILURE;
    }

    /* Set appropriate arguments to the kernel3D */

    /* input buffer image */
    status = clSetKernelArg(
        kernel3D,
        0,
        sizeof(cl_mem),
        &inputImage3D);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (inputImage3D)"))
    {
        return SDK_FAILURE;
    }

    /* outBuffer image */
    status = clSetKernelArg(
        kernel3D,
        1,
        sizeof(cl_mem),
        &outputImage3D);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clSetKernelArg failed. (outputImage3D)"))
    {
        return SDK_FAILURE;
    }

    /* 
    * Enqueue a kernel run call.
    */
    size_t globalThreads[] = {width, height};
    size_t localThreads[] = {blockSizeX, blockSizeY};


    status = clEnqueueNDRangeKernel(
        commandQueue,
        kernel2D,
        2,
        NULL,
        globalThreads,
        localThreads,
        0,
        NULL,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueNDRangeKernel failed."))
    {
        return SDK_FAILURE;
    }

    status = clEnqueueNDRangeKernel(
        commandQueue,
        kernel3D,
        2,
        NULL,
        globalThreads,
        localThreads,
        0,
        NULL,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueNDRangeKernel failed."))
    {
        return SDK_FAILURE;
    }

    status = clFinish(commandQueue);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clFinish failed."))
        return SDK_FAILURE;

    /* Enqueue Read Image */
    size_t origin[] = {0, 0, 0};
    size_t region[] = {width, height, 1};

    /* Read output of 2D copy */
    status = clEnqueueReadImage(commandQueue,
                                outputImage2D,
                                1,
                                origin,
                                region,
                                0,
                                0,
                                outputImageData2D,
                                0, 0, 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueReadImage failed."))
        return SDK_FAILURE;

    /* Read output of 3D copy */
    status = clEnqueueReadImage(commandQueue,
                                outputImage3D,
                                1,
                                origin,
                                region,
                                0,
                                0,
                                outputImageData3D,
                                0, 0, 0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clEnqueueReadImage failed."))
        return SDK_FAILURE;


    /* Wait for the read buffer to finish execution */
    status = clFinish(commandQueue);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clFinish failed."))
        return SDK_FAILURE;


    return SDK_SUCCESS;
}



int 
SimpleImage::initialize()
{
    // Call base class Initialize to get default configuration
    if(!this->SDKSample::initialize())
        return SDK_FAILURE;

    streamsdk::Option* iteration_option = new streamsdk::Option;
    if(!iteration_option)
    {
        sampleCommon->error("Memory Allocation error.\n");
        return SDK_FAILURE;
    }
    iteration_option->_sVersion = "i";
    iteration_option->_lVersion = "iterations";
    iteration_option->_description = "Number of iterations to execute kernel";
    iteration_option->_type = streamsdk::CA_ARG_INT;
    iteration_option->_value = &iterations;

    sampleArgs->AddOption(iteration_option);
    
    delete iteration_option;

    return SDK_SUCCESS;
}

int 
SimpleImage::setup()
{
    /* Allocate host memoryF and read input image */
    std::string filePath = sampleCommon->getPath() + std::string(INPUT_IMAGE);
    if(readInputImage(filePath) != SDK_SUCCESS)
        return SDK_FAILURE;

    /* create and initialize timers */
    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);

    int status = setupCL();
    if(status != SDK_SUCCESS)
        return status;

    sampleCommon->stopTimer(timer);
    /* Compute setup time */
    setupTime = (double)(sampleCommon->readTimer(timer));

    return SDK_SUCCESS;

}

int 
SimpleImage::run()
{
    if(!byteRWSupport)
        return SDK_SUCCESS;

    /* create and initialize timers */
    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);

    std::cout << "Executing kernel for " << iterations << 
        " iterations" <<std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for(int i = 0; i < iterations; i++)
    {
        /* Set kernel arguments and run kernel */
        if(runCLKernels() != SDK_SUCCESS)
            return SDK_FAILURE;
    }

    sampleCommon->stopTimer(timer);    
    /* Compute kernel time */
    kernelTime = (double)(sampleCommon->readTimer(timer)) / iterations;

    /* write the output image to bitmap file */
    if(writeOutputImage(OUTPUT_IMAGE) != SDK_SUCCESS)
    {
        return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}

int 
SimpleImage::cleanup()
{
    if(!byteRWSupport)
        return SDK_SUCCESS;

    /* Releases OpenCL resources (Context, Memory etc.) */
    cl_int status;

    status = clReleaseKernel(kernel2D);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseKernel failed."))
        return SDK_FAILURE;

    status = clReleaseKernel(kernel3D);
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

    status = clReleaseMemObject(inputImage2D);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(outputImage2D);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(inputImage3D);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(outputImage3D);
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
    if(inputImageData) 
        free(inputImageData);

    if(outputImageData2D)
        free(outputImageData2D);

    if(outputImageData3D)
        free(outputImageData3D);

    if(verificationOutput) 
        free(verificationOutput);

    if(devices)
        free(devices);

    return SDK_SUCCESS;
}


void 
SimpleImage::simpleImageCPUReference()
{

}


int 
SimpleImage::verifyResults()
{
    if(verify)
    {
        std::cout << "Verifying 2D copy result - ";
        /* compare the results and see if they match */
        if(!memcmp(inputImageData, outputImageData2D, width * height * 4))
        {
            std::cout << "Passed!\n" << std::endl;
        }
        else
        {
            std::cout << "Failed\n" << std::endl;
            return SDK_FAILURE;
        }

        std::cout << "Verifying 3D copy result - ";
        /* compare the results and see if they match */
        if(!memcmp(inputImageData, outputImageData3D, width * height * 4))
        {
            std::cout << "Passed!\n" << std::endl;
            return SDK_SUCCESS;
        }
        else
        {
            std::cout << "Failed\n" << std::endl;
            return SDK_FAILURE;
        }
    }

    return SDK_SUCCESS;
}

void 
SimpleImage::printStats()
{
    std::string strArray[4] = 
    {
        "Width", 
        "Height", 
        "Time(sec)", 
        "kernelTime(sec)"
    };
    std::string stats[4];

    totalTime = setupTime + kernelTime;

    stats[0] = sampleCommon->toString(width, std::dec);
    stats[1] = sampleCommon->toString(height, std::dec);
    stats[2] = sampleCommon->toString(totalTime, std::dec);
    stats[3] = sampleCommon->toString(kernelTime, std::dec);

    this->SDKSample::printStats(strArray, stats, 4);
}


int 
main(int argc, char * argv[])
{
    SimpleImage clSimpleImage("OpenCL SimpleImage");

    if(clSimpleImage.initialize() !=  SDK_SUCCESS)
        return SDK_FAILURE;

    if(!clSimpleImage.parseCommandLine(argc, argv))
        return SDK_FAILURE;


    if(clSimpleImage.isDumpBinaryEnabled())
    {
        return clSimpleImage.genBinaryImage();
    }
    else
    {
        int status = clSimpleImage.setup();
        if(status !=  SDK_SUCCESS)
        {
            if(status == SDK_FAILURE)
                return SDK_FAILURE;
            else
                return SDK_SUCCESS;
        }

        if(clSimpleImage.run() !=  SDK_SUCCESS)
            return SDK_FAILURE;

        if(clSimpleImage.verifyResults() !=  SDK_SUCCESS)
            return SDK_FAILURE;
        if(clSimpleImage.cleanup() !=  SDK_SUCCESS)
            return SDK_FAILURE;

        clSimpleImage.printStats();
    }

    return SDK_SUCCESS;
}



