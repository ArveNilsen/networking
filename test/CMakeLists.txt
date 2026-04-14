add_executable(test_main)

target_sources(test_main
    PRIVATE
        test_ListIter_constexpr.cpp
    
    PUBLIC 
    FILE_SET HEADERS
    BASE_DIRS ../include
    FILES
        ../include/networking/ListIter.hpp
)

target_link_libraries(test_main
    PRIVATE
        project_warnings
        project_options
)
