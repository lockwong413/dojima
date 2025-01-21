# -----------------------------------------------------------------------------

set(TARGET nif2gltf)
add_executable(${TARGET} main.cc)

# (wip)
add_executable(wots3 wots3.cc)

list(APPEND Targets
  ${TARGET}
  wots3
)

# -----------------------------------------------------------------------------

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
