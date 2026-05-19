add_library(debugplus_core STATIC
  debug_session.cpp
  parser/parser.cpp
)

target_include_directories(debugplus_core
  PUBLIC  ${CMAKE_SOURCE_DIR}/src
  PUBLIC  ${LLVM_INCLUDE_DIRS}
)

# Propagate dependencies + standard
target_link_libraries(debugplus_core PUBLIC LLDB::LLDB)
target_link_libraries(debugplus_core PRIVATE project_warnings)
target_compile_features(debugplus_core PUBLIC cxx_std_20)
target_compile_definitions(debugplus_core PUBLIC ${LLVM_DEFINITIONS_LIST})
