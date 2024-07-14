add_executable(myjqlserver myjqlserver.c)
target_link_libraries(myjqlserver myjql)
if(UNIX)
    target_link_libraries(myjqlserver pthread)
endif()

add_executable(myjqlshell myjqlshell.cpp)
target_link_libraries(myjqlshell myjqlclient)