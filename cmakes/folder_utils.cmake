macro(add_to_external_folder TARGET_NAME SOURCES)
    add_to_folder(${TARGET_NAME} "thirdparty" ${SOURCES})
endmacro()

macro(add_to_apps_folder TARGET_NAME SOURCES)
    add_to_folder(${TARGET_NAME} "apps" ${SOURCES})
endmacro()

macro(add_to_folder TARGET_NAME FOLDER_NAME SOURCES)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
    source_group("_dev" FILES CMakeLists.txt)
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER ${FOLDER_NAME})
endmacro()