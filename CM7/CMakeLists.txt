cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Compiler options
set(STM32_MCU_FLAGS  "-mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard " )

#Linker options
set(STM32_LINKER_SCRIPT stm32h747xx_flash_CM7.ld)
set(STM32_LINKER_OPTION  )

# Set the project name
set(CMAKE_PROJECT_NAME Open_deck_CM7)
project(${CMAKE_PROJECT_NAME})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Include mx-generated
include("mx-generated.cmake")


# In order to use CMake for cross-compiling
set(CMAKE_CXX_COMPILER_FORCED "true")

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Src/myapp.c
    # Add user sources here
)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined libraries
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Validate that STM32CubeMX code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()

# ============================================================
# TINYUSB v0.20.0 + DWC2 (STM32H7) - FINAL FIX
# ============================================================

set(TINYUSB_ROOT "E:/stm32/library/tinyusb/src")
add_subdirectory(${TINYUSB_ROOT} ${CMAKE_BINARY_DIR}/tinyusb_build)

tinyusb_target_add(${CMAKE_PROJECT_NAME})

#  ŚCIEŻKA DO FOLDERU DWC2
set(DWC2_DIR "${TINYUSB_ROOT}/../portable/synopsys/dwc2")
if(NOT EXISTS "${DWC2_DIR}")
    set(DWC2_DIR "${TINYUSB_ROOT}/portable/synopsys/dwc2")
endif()

if(NOT EXISTS "${DWC2_DIR}")
    message(FATAL_ERROR "Folder DWC2 nie istnieje! Sprawdź ścieżkę do TinyUSB.")
endif()

# Dodaj WSZYSTKIE pliki .c z folderu DWC2 (dcd_dwc2.c + dwc2_core.c)
file(GLOB DWC2_SOURCES "${DWC2_DIR}/*.c")
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${DWC2_SOURCES})

# Twoje deskryptory
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/usb_descriptors.c
)

# Include'y
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc   # tusb_config.h + usb_descriptors.h
    ${DWC2_DIR}                            # nagłówki DWC2
)

# Definicje
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    CFG_TUSB_MCU=OPT_MCU_STM32H7
    "CFG_TUSB_RHPORT0_MODE=(OPT_MODE_DEVICE | OPT_MODE_HIGH_SPEED)"
    CFG_TUD_CDC=1
)







