################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Config.cpp \
../src/DynamicGETResource.cpp \
../src/DynamicPOSTResource.cpp \
../src/DynamicResource.cpp \
../src/Exceptions.cpp \
../src/Helpers.cpp \
../src/HttpProcessor.cpp \
../src/HttpRequest.cpp \
../src/HttpResponse.cpp \
../src/HttpValidator.cpp \
../src/Main.cpp \
../src/MimeTypes.cpp \
../src/OptionParser.cpp \
../src/ProcessOperations.cpp \
../src/Resource.cpp \
../src/SockInterface.cpp \
../src/StringOperations.cpp \
../src/TCPConnection.cpp 

OBJS += \
./src/Config.o \
./src/DynamicGETResource.o \
./src/DynamicPOSTResource.o \
./src/DynamicResource.o \
./src/Exceptions.o \
./src/Helpers.o \
./src/HttpProcessor.o \
./src/HttpRequest.o \
./src/HttpResponse.o \
./src/HttpValidator.o \
./src/Main.o \
./src/MimeTypes.o \
./src/OptionParser.o \
./src/ProcessOperations.o \
./src/Resource.o \
./src/SockInterface.o \
./src/StringOperations.o \
./src/TCPConnection.o 

CPP_DEPS += \
./src/Config.d \
./src/DynamicGETResource.d \
./src/DynamicPOSTResource.d \
./src/DynamicResource.d \
./src/Exceptions.d \
./src/Helpers.d \
./src/HttpProcessor.d \
./src/HttpRequest.d \
./src/HttpResponse.d \
./src/HttpValidator.d \
./src/Main.d \
./src/MimeTypes.d \
./src/OptionParser.d \
./src/ProcessOperations.d \
./src/Resource.d \
./src/SockInterface.d \
./src/StringOperations.d \
./src/TCPConnection.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


