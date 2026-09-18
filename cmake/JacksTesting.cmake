# Included only when BUILD_TESTING is enabled.
include(FetchContent)
# Official GoogleTest v1.14.0, pinned to the accepted immutable commit.
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571)
set(gtest_force_shared_crt ON CACHE BOOL "Match MSVC runtime" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "Only GoogleTest needed" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "No global installation" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Each caller chooses its existing target name, source paths and tested libraries.
function(jacks_add_tests target)
  cmake_parse_arguments(test "" "" "SOURCES;LIBRARIES" ${ARGN})
  add_executable(${target} ${test_SOURCES})
  target_link_libraries(${target} PRIVATE ${test_LIBRARIES} GTest::gtest_main)
  jacks_enable_warnings(${target})
  gtest_discover_tests(${target} DISCOVERY_MODE PRE_TEST)
endfunction()
