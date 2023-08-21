cmake_minimum_required(VERSION 3.5.0)
# project(MyOpenGL VERSION 0.1.0)

# # 使用 C++ 17 标准
# set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required (VERSION 3.5.0)
# cmake_policy(VERSION 3.0)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
# find_package(SOIL REQUIRED)
# message(STATUS "Found SOIL in ${SOIL_INCLUDE_DIR}")
# find_package(GLEW REQUIRED)
# message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")
if(APPLE)
  find_package(Freetype REQUIRED)
  message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

# 设置代码文件
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

# 添加头文件
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include/)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/)
include_directories(${HEADER_DIR} ${LIB_DIR})

# 添加目标链接
set(GLFW_LINK ${LIB_DIR}libglfw.3.dylib)
link_libraries(${GLFW_LINK})

INCLUDE_DIRECTORIES(/System/Library/Frameworks)
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
FIND_LIBRARY(IOKit_LIBRARY IOKit)
FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
set(LIBS ${LIBS} ${APPLE_LIBS})

# 配置文件
configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)


add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

# 章节编号
set(CHAPTERS
    01_getting_started
)

# 第一章节
set(01_getting_started
    CH1_01_S2_2Triggle
    CH1_01_Triggle
    CH1_01_Triggle_EBO
    CH1_02_Triggle_Uniform
    CH1_02_Triggle_Uniform_Shader
)

# add_library(GLAD "src/tools/glad.c")
# set(LIBS ${LIBS} GLAD)


function(create_project_from_sources chapter demo)
    file(GLOB SOURCE
        "src/${chapter}/${demo}/*.h"
        "src/${chapter}/${demo}/*.cpp"
        "src/${chapter}/${demo}/*.vs"
        "src/${chapter}/${demo}/*.fs"
        "src/${chapter}/${demo}/*.tcs"
        "src/${chapter}/${demo}/*.tes"
        "src/${chapter}/${demo}/*.gs"
        "src/${chapter}/${demo}/*.cs"
    )

    if (demo STREQUAL "") # if demo == ""
        SET(replaced "")
        string(REPLACE "/" "_" replaced ${chapter})
        set(NAME ${replaced})
    else()
        set(NAME "${chapter}__${demo}")
    endif()

    # set(SOURCE2 ${SRC_DIR}tools/glad.c ${SOURCE})
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})


    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${chapter}")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${chapter}")


    file(GLOB SHADERS
        "src/${chapter}/${demo}/*.vs"
        # "src/${chapter}/${demo}/*.frag"
        "src/${chapter}/${demo}/*.fs"
        "src/${chapter}/${demo}/*.tcs"
        "src/${chapter}/${demo}/*.tes"
        "src/${chapter}/${demo}/*.gs"
        "src/${chapter}/${demo}/*.cs"
    )

  	# copy dlls
	# file(GLOB DLLS "dlls/*.dll")
    # foreach(SHADER ${SHADERS})
    #     # if(WIN32)
    #     #     # configure_file(${SHADER} "test")
    #     #     add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
	# 	# 	add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
    #     # elseif(UNIX AND NOT APPLE)
    #     #     file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${chapter})
    #     # elseif(APPLE)
    #         # create symbolic link for *.vs *.fs *.gs
    #         get_filename_component(SHADERNAME ${SHADER} NAME)
    #         # makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${chapter}/${SHADERNAME} ${NAME})
    #     # endif(WIN32)
    # endforeach(SHADER)

endfunction()


foreach(CHAPTER ${CHAPTERS})
    foreach(DEMO ${${CHAPTER}})
		create_project_from_sources(${CHAPTER} ${DEMO})
    endforeach(DEMO)
endforeach(CHAPTER)

# 链接系统的 OpenGL 框架
# if (APPLE)
#     target_link_libraries(MyOpenGL "-framework OpenGL")
# endif()






# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)





# 第一次执行
# mkdir build
# cd build
# cmake ..


# 在build 目录下执行  make


#  然后执行
#  ./MyOpenGL

