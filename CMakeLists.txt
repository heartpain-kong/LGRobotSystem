#此文件从模板自动生成! 请勿更改!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 4.3)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(LGRobotSystem C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

# ★ 手加:STM32H723 (Cortex-M7,双精度 FPU) 必须用硬浮点 + fpv5-d16
#   这不仅是为了性能 —— CMSIS-DSP 预编译库 libarm_cortexM7lfsp_math.a 就是
#   硬浮点 ABI(readelf: Tag_ABI_VFP_args = VFP registers / Tag_FP_arch = FPv5/FP-D16),
#   工程若按软浮点编译,链接期会报 "uses VFP register arguments, ... does not"。
add_compile_options(-mcpu=cortex-m7 -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv5-d16)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(Core/Inc USB_DEVICE/App USB_DEVICE/Target Drivers/STM32H7xx_HAL_Driver/Inc Drivers/STM32H7xx_HAL_Driver/Inc/Legacy Middlewares/Third_Party/FreeRTOS/Source/include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 Drivers/CMSIS/RTOS2/Include Middlewares/ST/STM32_USB_Device_Library/Core/Inc Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc Drivers/CMSIS/Device/ST/STM32H7xx/Include Drivers/CMSIS/Include Middlewares/ST/ARM/DSP/Inc Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1)

# ★ 手加:自己的代码目录(User 段)。
#   CLion 的 STM32CubeMX 插件认这两个 BEGIN/END 标记,会保留这一块;
#   新加一个目录时在这里补一行,并记得 Reset Cache and Reload Project
#   (file(GLOB) 是配置期展开的,不 reload 看不到新文件)。
# User include_directories BEGIN
include_directories(
        User/Bsp
        User/Bsp/fdcan
        User/Bsp/adc
        User/Bsp/dwt
        User/Bsp/flash
        User/Bsp/gpio
        User/Bsp/power
        User/Bsp/pwm
        User/Bsp/timestamp
        User/Bsp/spi
        User/Device/buzzer
        User/Device/ws2812
        User/Bsp/uart
        User/Device
        User/Device/bmi088
        User/Device/log
        User/Device/electric
        User/Device/referee
        User/Device/minipc
        User/Device/motor
        User/Device/motor/motor_DJ
        User/Device/motor/motor_DM
        User/Device/motor/motor_LZ
        User/Device/motor/motor_YS
        User/App
        User/App/motor_task
        User/Bsp/fault
        User/App/debug
        User/Bsp/rc
        User/Device/remote
        User/App/robot
        User/App/error_task
        User/App/msg_center
        User/App/cmd
        User/App/imu
        User/App/chassis
        User/App/gimbal
        User/App/highRise
        User/Bsp/ospi
        User/Device/w25q64
        User/algorithm
        User/algorithm/PID
        User/algorithm/Slope
        User/algorithm/math_support
        User/algorithm/crc
        User/algorithm/chassis_control
        User/algorithm/Kalman
        User/algorithm/Mahony
        # FreeRTOS 移植层:★ 这里的路径必须与磁盘上真实存在的目录一致
        #   (共两处:上面那条 include_directories + 这里)。
        #   本工程用 **M7 专用移植层** GCC/ARM_CM7/r0p1(上游 FreeRTOS V10.6.2,已按 blob SHA 校验字节)。
        #   ⚠️ CubeMX 重新导出会把 portable/ 整个换成 CM4F 并删掉 CM7 ⇒
        #      导出后跑 tools/cubemx_post_export.ps1:它会重装 CM7 移植层、把 CM4F 移出编译树
        #      (两个 port.c 同时在树里会因 file(GLOB) 双双参与编译 ⇒ 符号重复)。
        Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
)

# ★★★ 自愈 1/2:CubeMX 每次导出都会把它生成的那条 include 行里的 FreeRTOS 移植层写成
#     ARM_CM4F,而磁盘上是 ARM_CM7/r0p1。指向不存在的目录编译器会忽略,所以能编过,
#     但会误导排查(曾经造成"内核与移植层版本不匹配"的假象)。
#     ⚠️ 判定**只看路径名字**,不要用 IS_DIRECTORY 去探测(Windows 下分隔符/转义会误判,
#        实测会把有效的 ARM_CM7 路径一起删掉,导致 portmacro.h 找不到)。
#     规则:凡是指向 FreeRTOS/Source/portable/GCC/ 但**不是** ARM_CM7/r0p1 的 include 路径,一律剔除;
#           再把真实存在的那一个显式加回来。
get_property(_freertos_include_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
set(_freertos_include_dirs_fixed "")
foreach(_include_dir IN LISTS _freertos_include_dirs)
    string(REPLACE "\\" "/" _include_dir_slash "${_include_dir}")
    if (_include_dir_slash MATCHES "FreeRTOS/Source/portable/GCC/" AND NOT _include_dir_slash MATCHES "ARM_CM7/r0p1")
        message(STATUS "[自愈] 剔除非 CM7 的 FreeRTOS 移植层 include 路径: ${_include_dir}")
    else()
        list(APPEND _freertos_include_dirs_fixed "${_include_dir}")
    endif()
endforeach()
set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES ${_freertos_include_dirs_fixed})
include_directories(Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1)# User include_directories END

add_definitions(-DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx)

# ★ 手加:"User/*.*" —— 否则整个 User/ 树都不参与编译
file(GLOB_RECURSE SOURCES "Core/*.*" "Middlewares/*.*" "Drivers/*.*" "USB_DEVICE/*.*"
        # User file BEGIN
        "User/*.*"
        
# ★★★ 自愈 2/2:导出若把别的移植层(如 ARM_CM4F)塞进树里,源码收集会把它一起编进来,
#     于是**两个 port.c 同时进固件**(重复符号 / 内核与移植层不匹配)—— 这是本工程踩过的坑。
#     这里把"非 ARM_CM7/r0p1 的 FreeRTOS 移植层源文件"从编译列表里排除掉。
#     (tools/cubemx_post_export.ps1 还会把那个目录整个移出树,这是第二道保险)
if (DEFINED SOURCES)
    foreach(_src IN LISTS SOURCES)
        if (_src MATCHES "FreeRTOS/Source/portable/GCC/" AND NOT _src MATCHES "ARM_CM7/r0p1")
            message(STATUS "[自愈] 排除非 CM7 的 FreeRTOS 移植层源文件: ${_src}")
            list(REMOVE_ITEM SOURCES "${_src}")
        endif()
    endforeach()
endif()
# User file END
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H723VGTX_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m7 -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv5-d16)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

# ★ 手加:CMSIS-DSP 预编译数学库(arm_math.h 里非 inline 的那些函数都在里面)
#   ABI 必须与上面一致(hard-float + fpv5-d16);再补 -lm,因为该库内部会调 libm。
target_link_libraries(${PROJECT_NAME}.elf PRIVATE
        ${CMAKE_SOURCE_DIR}/Middlewares/ST/ARM/DSP/Lib/libarm_cortexM7lfsp_math.a
        m)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
