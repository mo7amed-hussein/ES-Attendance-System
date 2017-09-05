################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../dualultrasonic \
../wifi \
../ultrasonic \
../uart


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../dualultrasonic/dualultrasonic.c \
../uart/softuart.c \
../uart/usart0.c \
../ultrasonic/ultrasonic.c \
../wifi/wifi.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
dualultrasonic/dualultrasonic.o \
uart/softuart.o \
uart/usart0.o \
ultrasonic/ultrasonic.o \
wifi/wifi.o

OBJS_AS_ARGS +=  \
dualultrasonic/dualultrasonic.o \
uart/softuart.o \
uart/usart0.o \
ultrasonic/ultrasonic.o \
wifi/wifi.o

C_DEPS +=  \
dualultrasonic/dualultrasonic.d \
uart/softuart.d \
uart/usart0.d \
ultrasonic/ultrasonic.d \
wifi/wifi.d

C_DEPS_AS_ARGS +=  \
dualultrasonic/dualultrasonic.d \
uart/softuart.d \
uart/usart0.d \
ultrasonic/ultrasonic.d \
wifi/wifi.d

OUTPUT_FILE_PATH +=libdrivers.a

OUTPUT_FILE_PATH_AS_ARGS +=libdrivers.a

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler











dualultrasonic/%.o: ../dualultrasonic/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)E:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE)  -x c -O0 -ffunction-sections -fdata-sections -mmcu=atmega328p -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

uart/%.o: ../uart/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)E:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE)  -x c -O0 -ffunction-sections -fdata-sections -mmcu=atmega328p -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

ultrasonic/%.o: ../ultrasonic/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)E:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE)  -x c -O0 -ffunction-sections -fdata-sections -mmcu=atmega328p -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

wifi/%.o: ../wifi/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)E:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE)  -x c -O0 -ffunction-sections -fdata-sections -mmcu=atmega328p -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)


$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Archiver : 4.3.3
	$(QUOTE)E:\WinAVR-20100110\bin\avr-ar.exe$(QUOTE) -r  -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) 
	@echo Finished building target: $@
	
	




# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "libdrivers.elf" "libdrivers.a" "libdrivers.hex" "libdrivers.lss" "libdrivers.eep" "libdrivers.map" "libdrivers.srec" "libdrivers.usersignatures"
	