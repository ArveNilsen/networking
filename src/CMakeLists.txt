add_executable(main)

target_sources(main
    PRIVATE
        main.cpp
        NetworkAdapterInfo.cpp
)

target_link_libraries(main
    PRIVATE
        project_warnings
        project_options
)
