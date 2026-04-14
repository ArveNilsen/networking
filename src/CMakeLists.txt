add_executable(main)

target_sources(main
    PRIVATE
        main.cpp
        NetworkAdapterInfo.cpp

        # Platform specific sources
        $<$<PLATFORM_ID:Windows>:InitWindows.cpp>
        $<$<PLATFORM_ID:Linux>:InitOthers.cpp>
        $<$<PLATFORM_ID:Darwin>:InitOthers.cpp>
    
    PUBLIC 
    FILE_SET HEADERS
    BASE_DIRS ../include
    FILES
        ../include/networking/ListIter.hpp
        ../include/networking/IfAddrs.hpp
        ../include/networking/ListIter.hpp
        ../include/networking/NetworkAdapterInfo.hpp
        ../include/networking/Init.hpp
)

target_link_libraries(main
    PRIVATE
        project_warnings
        project_options

        # Platform specific linking
        $<$<PLATFORM_ID:Windows>:ws2_32.lib>
)
