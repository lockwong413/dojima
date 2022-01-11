# -----------------------------------------------------------------------------
# Sources.
# -----------------------------------------------------------------------------

# Framework (library).
list(APPEND FrameworkSources
  api/dummy.cc
)

# Sample (executable).
list(APPEND SampleSources
  main.cc
)

# -----------------------------------------------------------------------------
# Target properties and build parameters.
# -----------------------------------------------------------------------------

# Framework Target (library).
# set(FrameworkTarget ${CMAKE_PROJECT_NAME})
# add_library(${FrameworkTarget} ${PROJECT_LIB_TYPE} ${FrameworkSources})

# Sample Target (executable).
set(AppTarget nif2gltf)
add_executable(${AppTarget} ${SampleSources})
target_link_libraries(${AppTarget} ${FrameworkTarget})

# -----------------------------------------------------------------------------

list(APPEND Targets ${FrameworkTarget} ${AppTarget})

# Set generic target properties.
foreach(target ${Targets})
  target_compile_options(
    ${target}
    PRIVATE
      "${CXX_FLAGS}"
      "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
      "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>"
      "$<$<CONFIG:DebugWithRelInfo>:${CXX_FLAGS_RELWITHDEBINFO}>"
  )
  target_compile_definitions(${target} PRIVATE ${CustomDefinitions})
  target_include_directories(${target} PRIVATE ${CustomIncludeDirs}
    #    INTERFACE
    #      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    #      $<INSTALL_INTERFACE:include>
  )
  target_link_libraries(${target} ${CustomLibs})

  # [prefers LINK_OPTION with a list on Cmake 3.13+]
  set_target_properties(${target} PROPERTIES LINK_FLAGS ${LinkFlagsString})

  # Force Debug / Release output in the same directory (for MSVC).
  helpers_setTargetOutputDirectory(${target} ${PROJECT_BINARY_DIR})
endforeach()

# -----------------------------------------------------------------------------
