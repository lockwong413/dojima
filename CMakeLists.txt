# -----------------------------------------------------------------------------
# CMake configuration file (2022 - unlicense.org)
# 
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10.2) 

# -----------------------------------------------------------------------------
# CMake policies.
# -----------------------------------------------------------------------------

# Enable the use of "VERSION" for project setting in older cmake version.
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

# Force OpenGL_GL_PREFERENCE to be set to GLVND (GL3+) instead of LEGACY (GL2).
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

# Use <PackageName>_ROOT variables.
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# Do not place MSVC warning flags in the default cache entries.
if(POLICY CMP0092)
  cmake_policy(SET CMP0092 NEW)
endif()

# -----------------------------------------------------------------------------
# Project.
# -----------------------------------------------------------------------------

project(dojima VERSION 0.1 LANGUAGES CXX C)

# -----------------------------------------------------------------------------
# Options.
# -----------------------------------------------------------------------------

# Enable clang static code analyzer.
option(OPT_ENABLE_CLANG_TIDY        "Use clang-tidy ?"                      OFF)

# Enable RTTI.
option(OPT_ENABLE_RTTI              "Use RTTI ?"                            OFF)

# Pragma concurrency might be used on some loops.
option(OPT_USE_OPENMP               "Use OpenMP for pragma concurrency ?"   OFF)

# Choose how to compile the libraries.
option(OPT_BUILD_SHARED_LIBS        "Compile libraries as shared ?"         OFF)

if(OPT_BUILD_SHARED_LIBS)
  set(PROJECT_LIB_TYPE SHARED)
else()
  set(PROJECT_LIB_TYPE STATIC)
endif()

# -----------------------------------------------------------------------------
# Paths.
# -----------------------------------------------------------------------------

set(PROJECT_ROOT_PATH               ${CMAKE_CURRENT_SOURCE_DIR})

# Input directories.
set(PROJECT_RESOURCES_DIR           ${PROJECT_ROOT_PATH}/resources)
set(PROJECT_SOURCE_DIR              ${PROJECT_ROOT_PATH}/src)
set(PROJECT_THIRD_PARTY_DIR         ${PROJECT_ROOT_PATH}/third_party)
set(PROJECT_TOOLS_DIR               ${PROJECT_ROOT_PATH}/tools)

# Output directories.
set(PROJECT_BINARY_DIR              ${PROJECT_ROOT_PATH}/bin)

# -----------------------------------------------------------------------------
# CMake includes.
# -----------------------------------------------------------------------------

# CMake module path.
list(APPEND CMAKE_MODULE_PATH       ${PROJECT_TOOLS_DIR}/cmake)

# Custom CMake helpers functions.
include(Helpers)

# -----------------------------------------------------------------------------
# Determine the host's total number of processors.
# -----------------------------------------------------------------------------

include(ProcessorCount)
ProcessorCount(PROJECT_NPROC_MAX)

# -----------------------------------------------------------------------------
# CMake settings.
# -----------------------------------------------------------------------------

set(CMAKE_VERBOSE_MAKEFILE          OFF)
set(CMAKE_COLOR_MAKEFILE            ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD   ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)
set(CMAKE_DISABLE_SOURCE_CHANGES    OFF)

# Default standards with no extensions.
set(CMAKE_C_STANDARD                11)
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)

# Remove (and disallow) sneaky default flags for targets.
set(CMAKE_CXX_FLAGS                 "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_MINSIZEREL      "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "" CACHE INTERNAL "")

# Specify a postfix on debug targets.
#set(CMAKE_DEBUG_POSTFIX             d)

# Force a specific output directory. Used to bypass MSVC settings.
helpers_setGlobalOutputDirectory(${PROJECT_BINARY_DIR})

# Force debug mode by default when no target is specified
# (for generators working at cache generation time like Makefile / Ninja).
helpers_setDefaultBuildType("Debug")

# -----------------------------------------------------------------------------
# Internal configuration attributes.
# -----------------------------------------------------------------------------

# Determines if the build is flagged for full debug mode.
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(DEBUG TRUE)
  message(STATUS "DEBUG build.")
else()
  set(DEBUG FALSE)
endif()

# Determines the build platform.
if(UNIX AND NOT APPLE AND NOT ANDROID)
  set(OS_NAME "Linux")
  set(OS_LINUX TRUE)
elseif(WIN32)
  set(OS_NAME "Windows")
  set(OS_WIN32 TRUE)
else()
  message(WARNING "The \"${CMAKE_SYSTEM_NAME}\" platform is not officially supported.")
endif()

# Determines the current compiler.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(USE_GCC TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
  set(USE_CLANG TRUE)
elseif(MSVC)
  set(USE_MSVC TRUE)
else()
  message(WARNING "The \"${CMAKE_CXX_COMPILER_ID}\" compiler is not officially supported.")
endif()

# -----------------------------------------------------------------------------
# Setup code analyzer.
# -----------------------------------------------------------------------------

# Setup clang-tidy parameters (if enabled and found).
if(OPT_ENABLE_CLANG_TIDY)  
  # [FIXME] Checks options must be on the same line to work.
  helpers_setClangTidy(
    #--checks=
    clang-analyzer-*,google-*,-google-build-using-namespace#,cppcoreguidelines-*,-cppcoreguidelines-avoid-magic-numbers
    #--header-filter=
    ${PROJECT_SOURCE_DIR}
  )
endif()

# -----------------------------------------------------------------------------
# Compiler settings.
# -----------------------------------------------------------------------------

# Check the compiler version.
if(USE_GCC)
  # https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2017
  helpers_checkCompilerVersion(${CMAKE_CXX_COMPILER_ID} 7.1)
elseif(USE_CLANG)
  helpers_checkCompilerVersion(${CMAKE_CXX_COMPILER_ID} 11.0)
elseif(USE_MSVC)
  helpers_checkCompilerVersion(${CMAKE_CXX_COMPILER_ID} 14.0) # 15.7 is ideal
endif()

# Specify generic compiler and linker flags.
if(USE_GCC OR USE_CLANG)
  # This is a mix of warning options not available in all GCC / Clang version.
  list(APPEND CXX_FLAGS 
    -Wall
    -Wextra -Wpedantic
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wdouble-promotion
    -Wuseless-cast
    -Wcast-align
    -Wlogical-op
    -Wnull-dereference
    # -Wshadow
    -Wmisleading-indentation
    -Wformat=2 -Wformat-truncation -Wformat-overflow
    -Wduplicated-cond
    -Wno-unknown-pragmas 
    -Wno-unused-function 
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    # -Wzero-as-null-pointer-constant
    -Wdisabled-optimization
    -Wvector-operation-performance
    # -Wold-style-cast
    # -Wduplicated-branches 
    # -Wconversion
  )

  # To avoid warnings about the previous mismatched GCC / Clang options.
  if(USE_CLANG)
    list(APPEND CXX_FLAGS -Wno-unknown-warning-option)
  endif()

  # Features.
  list(APPEND CXX_FLAGS
    -fno-strict-aliasing 
    -fno-builtin-memcmp
    -fno-common
    -ffunction-sections
    -fdata-sections
    # -fno-exceptions
    # -ftree-loop-distribution
    # -funroll-loops
  )
  
  # Force ANSI colored outputs.
  if(USE_GCC)
    add_compile_options(-fdiagnostics-color=always)
  elseif (USE_CLANG)
    add_compile_options(-fcolor-diagnostics)
  endif()

  if(NOT OPT_ENABLE_RTTI)
    list(APPEND CXX_FLAGS -fno-rtti)
  endif()

  # Compiler flags.
  list(APPEND CXX_FLAGS_DEBUG
    -O0 
    -g3 
    -Wundef
    #-Wconversion
  )
  list(APPEND CXX_FLAGS_RELEASE
    -O2
    -msse4.1 
  )
  list(APPEND CXX_FLAGS_RELWITHDEBINFO 
    ${CXX_FLAGS_RELEASE} 
    -g1
  )
  
  # Linker flags.
  list(APPEND CustomLinkFlags 
    -Wl,--gc-sections
  )

  # [improve] Used only for static library compilation.
  if(NOT OPT_BUILD_SHARED_LIBS)
    list(APPEND CXX_FLAGS         -fvisibility=hidden)
    if(NOT EMSCRIPTEN)
      list(APPEND CustomLinkFlags   -Wl,--no-undefined)
    endif()
  endif()

  # Definitions.
  if(NOT DEBUG)
    list(APPEND CustomDefinitions 
      -DNDEBUG=1
    )
  endif()

  # For C++17 filesystem support, might became depecrated.
  # set(STD_CXX_FILESYSTEM_LIB stdc++fs)

elseif(USE_MSVC)
  
  list(APPEND CXX_FLAGS
    /W4 
    /w14640   # enable warning on thread un-safe static member initialization.
    /w14928   # illegal copy-initialization.
    /w14265   # class has virtual functions, but destructor is not virtual.
    /w14555   # expression with no effect or side-effect.
    /wd4100   # keep unused parameters.
    /MP       # build with multiple process.
    #/EHsc-    # no exception.
    /permissive-
  )

  if(NOT OPT_ENABLE_RTTI)
    list(APPEND CXX_FLAGS /GR-)
  endif()

  list(APPEND CXX_FLAGS_DEBUG           /Zi)
  list(APPEND CXX_FLAGS_RELEASE         /Ox /INCREMENTAL:NO /DEBUG:NONE /DNDEBUG)
  list(APPEND CXX_FLAGS_RELWITHDEBINFO  ${CXX_FLAGS_RELEASE} /Ox /Zi)
 
  list(APPEND CustomDefinitions 
   -DNOMINMAX
   -DWIN32
   -D_WINDOWS
   -D_CRT_SECURE_NO_WARNINGS
   -D_HAS_EXCEPTIONS=0
   #-DVC_EXTRALEAN
   #-DWIN32_LEAN_AND_MEAN
   -D_USE_MATH_DEFINES
  )

  list(APPEND CustomLinkFlags 
    ""
  )
endif()

# -----------------------------------------------------------------------------
# Third parties.
# -----------------------------------------------------------------------------

# Defines LIBM_LIBRARIES for mathematics when needed.
# helpers_findLibM()
# list(APPEND CustomLibs ${LIBM_LIBRARIES})

# OpenMP.
if(OPT_USE_OPENMP)
  find_package(OpenMP)
  if(OPENMP_FOUND)
    list(APPEND CXX_FLAGS   "${OpenMP_CXX_FLAGS}")
    list(APPEND CustomLibs  "${OpenMP_CXX_LIBRARIES}")
  endif()
endif()

if(TRUE)
  set(NIFLIB_ROOT ${PROJECT_THIRD_PARTY_DIR}/niflib)
  
  set(NIFLIB_LIBRARY nif)
  set(NIFLIB_INCLUDE_DIR ${NIFLIB_ROOT}/include)

  list(APPEND CustomLibs        ${NIFLIB_LIBRARY})
  list(APPEND CustomIncludeDirs ${NIFLIB_INCLUDE_DIR})

  set(NIFLIB_BUILD_SHARED_LIBS OPT_BUILD_SHARED_LIBS CACHE BOOL "Build NIFLIB as shared library.")
  set(NIFLIB_BUILD_TESTS    OFF CACHE BOOL "Do not run NIFLIB tests.")
  set(NIFLIB_BUILD_DOCS     OFF CACHE BOOL "Do not build NIFLIB documentation.")
  set(NIFLIB_INSTALL        OFF CACHE BOOL "Do not generate install targets.")
  add_subdirectory(${NIFLIB_ROOT}) # niflib EXCLUDE_FROM_ALL)
endif()

# TinyGLTF
set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_USE_CPP14 ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${PROJECT_THIRD_PARTY_DIR}/tinygltf)

# -----------------------------------------------------------------------------
# Generic dependencies settings.
# -----------------------------------------------------------------------------

# Include directories.
list(APPEND CustomIncludeDirs
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_THIRD_PARTY_DIR}
)

# Definitions.
list(APPEND CustomDefinitions 
  -DPROJECT_NPROC_MAX=${PROJECT_NPROC_MAX}
)

# Transform the linker flags from a list to a string to be accepted by set_target_properties LINK_FLAG.
# On CMake 3.13+, we could use LINK_OPTIONS instead.
set(LinkFlagsString "")
foreach(flag ${CustomLinkFlags})
  set(LinkFlagsString "${LinkFlagsString} ${flag}")
endforeach()

# -----------------------------------------------------------------------------
# System dependencies.
# -----------------------------------------------------------------------------

if(OS_LINUX)
  # include(FindPkgConfig)
endif()

# -----------------------------------------------------------------------------
# Sources.
# -----------------------------------------------------------------------------

add_subdirectory(${PROJECT_SOURCE_DIR})

# -----------------------------------------------------------------------------
