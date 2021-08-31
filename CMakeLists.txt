cmake_minimum_required(VERSION 3.0)

# Project name
project(RTFV)

# This setting is useful for providing JSON file used by CodeLite for code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CONFIGURATION_NAME "Debug")

set(CL_WORKSPACE_DIRECTORY ./build)
# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${CL_WORKSPACE_DIRECTORY}/cmake-build-${CONFIGURATION_NAME}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

# Projects


# Top project
# Define some variables
set(PROJECT_RTFV_PATH "${CMAKE_CURRENT_LIST_DIR}/RTFV/")
set(WORKSPACE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")



#{{{{ User Code 1
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS core base gl)
include(${wxWidgets_USE_FILE})
#}}}}

include_directories(
    ./RTFV/
)


# Compiler options
execute_process(COMMAND
    wx-config --cxxflags --unicode=yes
    OUTPUT_VARIABLE
    CL_TMP_VAR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
string(STRIP ${CL_TMP_VAR} CL_VAR_1)
execute_process(COMMAND
    wx-config --cxxflags --unicode=yes
    OUTPUT_VARIABLE
    CL_TMP_VAR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
string(STRIP ${CL_TMP_VAR} CL_VAR_2)
add_definitions(${CL_VAR_1})
add_definitions(${CL_VAR_2})
add_definitions(-g)
add_definitions(-O0)
add_definitions(-Wall)
add_definitions(
    -D_FILE_OFFSET_BITS=64
    -DWXUSINGDLL
    -D__WXGTK__
)


# Linker options
execute_process(COMMAND
    wx-config   --libs --unicode=yes
    OUTPUT_VARIABLE
    CL_TMP_VAR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
string(STRIP ${CL_TMP_VAR} CL_VAR_3)
set(LINK_OPTIONS ${CL_VAR_3})

# Library path
link_directories(
    .
)

# Define the CXX sources
set ( CXX_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/RTFV/messagehandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RTFV/mainwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RTFV/commserial.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RTFV/anonymousglcanvas.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RTFV/glpanel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RTFV/serialpanel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RTFV/anonymousglcontext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RTFV/main.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS
    " -g -O0 -Wall -pthread")


#{{{{ User Code 2
# Place your code here
#}}}}

add_executable(RTFV ${RC_SRCS} ${CXX_SRCS} ${C_SRCS})
target_link_libraries(RTFV ${LINK_OPTIONS})



#{{{{ User Code 3
target_link_libraries(RTFV ${wxWidgets_LIBRARIES})
target_link_libraries(RTFV GL)

add_custom_command(TARGET RTFV POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:RTFV>)
#}}}}



