FILE(GLOB SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/*)
FOREACH(dir ${SUBDIRS})
    IF(IS_DIRECTORY ${dir})
        
        FILE(GLOB sources 
            ${dir}/CMakeLists.txt
        )

        IF(sources)
            add_subdirectory(${dir})
        ENDIF()
    ENDIF()
ENDFOREACH()
