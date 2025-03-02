cmake_minimum_required(VERSION 3.31)

project(Frogger)

# Recursively collect source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Frogger/src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Frogger/src
)

# Link directories
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lib)

# Set up Debug/Release specific flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(${PROJECT_NAME} sggd)  # Debug version of the library
    target_compile_options(${PROJECT_NAME} PRIVATE "/MDd")  # Use Debug runtime library
else()
    target_link_libraries(${PROJECT_NAME} sgg)   # Release version of the library
    target_compile_options(${PROJECT_NAME} PRIVATE "/MD")  # Use Release runtime library
endif()

# Disable iterator debugging (only for MSVC)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE "/D_ITERATOR_DEBUG_LEVEL=0")  # Disable iterator debugging
endif()

# Collect DLL files
file(GLOB DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/bin/*.dll")

# Add a custom command to copy DLLs and assets at build time
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILES} "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/bin/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying DLLs and assets to output directory"
)