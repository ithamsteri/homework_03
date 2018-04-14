# make executable file for application
add_executable(allocator
        main.cpp
        ChunkPool.h
        ForwardList.h
        Malloc.h
)

set_target_properties(allocator PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

install(TARGETS allocator RUNTIME DESTINATION bin)
