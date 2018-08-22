if (BUILD_TESTING)
    find_package (Boost COMPONENTS unit_test_framework REQUIRED)

    add_executable(allocator_test
        ChunkAllocator.test.cpp
    )
    add_executable(forwardlist_test
        ForwardList.test.cpp
    )
    set_target_properties(allocator_test forwardlist_test PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS};${SOURCE_PATH}"
        LINK_LIBRARIES ${Boost_LIBRARIES}
    )

    add_test(AllocatorTest allocator_test)
    add_test(ForwardListTest forwardlist_test)
endif()