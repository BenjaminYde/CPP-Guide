# tells the linker to use the GUI subsystem (no terminal window)
set_target_properties(${TARGET_NAME} PROPERTIES WIN32_EXECUTABLE ON)

# post build command: get windows dependencies
cmake_path(GET CMAKE_BINARY_DIR FILENAME DEPLOY_DIR_NAME)
set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/../${DEPLOY_DIR_NAME}_full")
#set(QT_TOOLCHAIN_PATH "/opt/qt/${QT_VERSION}/mingw_64")

add_custom_command(TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND bash "${CMAKE_SOURCE_DIR}/scripts/create_windows_build_artifact.sh" 
            "$<TARGET_FILE:${TARGET_NAME}>" 
            "${DEPLOY_DIR}"
            #"${QT_TOOLCHAIN_PATH}"
            "${QT_VERSION}"
    COMMENT "Running Windows Deployment Script with Qt path: ${QT_TOOLCHAIN_PATH}"
    VERBATIM
)