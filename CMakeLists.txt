cmake_minimum_required(VERSION 3.16)
project(Final_Project_RBT LANGUAGES CXX)

# C++ standard + compile_commands.json for IntelliSense
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pick the RBT source file 
set(RBT_FILE "")
if(EXISTS "${CMAKE_SOURCE_DIR}/code/RBT.cpp")
  set(RBT_FILE "code/RBT.cpp")
else()
  message(FATAL_ERROR "Could not find 'code/RBT.cpp'.")
endif()

# Core library (BST + RBT + Leaderboard)
add_library(bst_rbt
  "code/BST.cpp"
  "${RBT_FILE}"
  "code/Leaderboard.cpp"     
)
target_include_directories(bst_rbt PUBLIC code)

# App (prompts for name + score, then shows rank & details)
add_executable(app "app/main.cpp")
target_link_libraries(app PRIVATE bst_rbt)

# ---- Tests 
include(CTest)

if(EXISTS "${CMAKE_SOURCE_DIR}/tests/test_rank.cpp")
  add_executable(test_rank "tests/test_rank.cpp")
  target_link_libraries(test_rank PRIVATE bst_rbt)
  add_test(NAME rank_suite COMMAND test_rank)
  set_target_properties(test_rank PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/runtests")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/tests/testlb.cpp")
  add_executable(testlb "tests/testlb.cpp")
  target_link_libraries(testlb PRIVATE bst_rbt)
  add_test(NAME testlb_suite COMMAND testlb)
  set_target_properties(testlb PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMEAKE_BINARY_DIR}/runtests")  # typo fixed below
endif()