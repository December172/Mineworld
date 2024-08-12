aux_source_directory(. src_client_SOURCES)
list(APPEND SOURCES ${src_client_SOURCES})

include_directories(${PROJECT_SOURCE_CODE_DIR}/client)

add_subdirectory(${PROJECT_SOURCE_CODE_DIR}/client/renderer)
add_subdirectory(${PROJECT_SOURCE_CODE_DIR}/client/resource)