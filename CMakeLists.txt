set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(find-goto NoGoto.cpp)

target_link_libraries(find-goto
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )
