cmake_minimum_required(VERSION 3.2)

# check environment variable TRAVIS_BUILD_NUMBER
if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(allocator VERSION 0.0.${VERSION_BUILD})

# configure directories
set(SOURCE_PATH "${PROJECT_SOURCE_DIR}/src")

# make executable file for application
add_executable(allocator
        ${SOURCE_PATH}/main.cpp
        ${SOURCE_PATH}/ChunkAllocator.h
        ${SOURCE_PATH}/ForwardList.h
        ${SOURCE_PATH}/Malloc.h
)

set_target_properties(allocator PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# Need for YCM autocomplete in Vim editor
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT alex.aspirine@gmail.com)

include(CPack)

enable_testing()

add_subdirectory(tests)

