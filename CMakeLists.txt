cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED PATCH_VERSION)
    set(PATCH_VERSION "1" )
endif()

set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(allocator VERSION ${PROJECT_VESRION})

################################################################
#### Setting targets ###########################################
################################################################

add_executable(ExecutableTarget main.cpp)

add_executable(TestTarget test.cpp)

set_target_properties(ExecutableTarget PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
) 
set_target_properties(TestTarget PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
) 
target_compile_definitions(ExecutableTarget PUBLIC -DPATCH_VERSION=${PATCH_VERSION})
if (MSVC)
    target_compile_options(ExecutableTarget PRIVATE
        /W4 
    )
    target_compile_options(ExecutableTarget PRIVATE
        /W4 
    )
else ()
    target_compile_options(ExecutableTarget PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(ExecutableTarget PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

################################################################
#### Dependencies ##############################################
################################################################

find_package(GTest REQUIRED)

target_link_libraries(TestTarget PRIVATE GTest::GTest GTest::Main)

################################################################
#### Testing ###################################################
################################################################

enable_testing()

add_test(TestBucketAllocator TestTarget)

################################################################
#### Packaging #################################################
################################################################

install(TARGETS ExecutableTarget RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
