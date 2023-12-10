cmake_minimum_required(VERSION 3.13.4)


project(GameTemplate)


find_package(OpenGL REQUIRED)


FILE(GLOB MAIN
    src/main.c
    src/main.cpp
)


add_executable(${PROJECT_NAME} ${MAIN})


FILE(GLOB DEP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/*/include)


target_include_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE ${DEP_INCLUDES}
    PRIVATE ${OPENGL_INCLUDE_DIRS}
)




add_subdirectory(dependencies)




set(DEPLIBRARIES "")


FILE(GLOB SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/*)
FOREACH(dir ${SUBDIRS})
    IF(IS_DIRECTORY ${dir})
        
        get_filename_component(tmp ${dir} NAME)

        FILE(GLOB_RECURSE sources 
            ${dir}/CMakeLists.txt
        )

        IF(sources)
            string(APPEND DEPLIBRARIES ";${tmp}")
        ENDIF()
    ENDIF()
ENDFOREACH()




set(LIBRARIES "")


FILE(GLOB SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
FOREACH(dir ${SUBDIRS})
    IF(IS_DIRECTORY ${dir})
        
        FILE(GLOB_RECURSE sources 
            ${dir}/*.c
            ${dir}/*.cpp
        )

        IF(sources)
            get_filename_component(tmp ${dir} NAME)
            string(REPLACE " " "_" tmp ${tmp})

            add_library(${tmp} OBJECT ${sources})
            
            target_include_directories(${tmp}
                PRIVATE src
                PRIVATE ${DEP_INCLUDES}
                PRIVATE ${OPENGL_INCLUDE_DIRS}
            )

            string(APPEND LIBRARIES ";${tmp}")
        ENDIF()
    ENDIF()
ENDFOREACH()


target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${OPENGL_LIBRARIES} ${DEPLIBRARIES})
