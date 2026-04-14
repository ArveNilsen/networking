add_executable(main)

target_sources(main
    PRIVATE
        main.cpp
        NetworkAdapterInfo.cpp

        # Platform specific sources
        $<$<PLATFORM_ID:Windows>:InitWindows.cpp>
        $<$<PLATFORM_ID:Linux>:InitOthers.cpp>
        $<$<PLATFORM_ID:Darwin>:InitOthers.cpp>

        $<$<PLATFORM_ID:Darwin>:SocketPosixPolicy.cpp>
        $<$<PLATFORM_ID:Darwin>:SocketWindowsPolicy.cpp>

    
    PUBLIC 
    FILE_SET HEADERS
    BASE_DIRS ../include
    FILES
        ../include/networking/ListIter.hpp
        ../include/networking/IfAddrs.hpp
        ../include/networking/ListIter.hpp
        ../include/networking/NetworkAdapterInfo.hpp
        ../include/networking/Init.hpp
        ../include/networking/SocketDefaultPolicy.hpp
        ../include/networking/Socket.hpp

        # Platform specific headers
        $<$<PLATFORM_ID:Darwin>:../include/networking/SocketPosixPolicy.hpp>
        $<$<PLATFORM_ID:Darwin>:../include/networking/SocketWindowsPolicy.hpp>
)

target_link_libraries(main
    PRIVATE
        project_warnings
        project_options

        # Platform specific linking
        $<$<PLATFORM_ID:Windows>:ws2_32.lib>
)

#[=[ CMake logic to copy headers at build time. TODO: Test this!
if (WIN32)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/platform/windows_default_policy.hpp
                 ${CMAKE_CURRENT_BINARY_DIR}/include/platform/default_policy.hpp COPYONLY)
else()
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/platform/posix_default_policy.hpp
                 ${CMAKE_CURRENT_BINARY_DIR}/include/platform/default_policy.hpp COPYONLY)
endif()
#]=]

