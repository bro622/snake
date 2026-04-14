cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject 
    VERSION 1.0.0
    DESCRIPTION "Snake game using SFML"
    LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录配置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ==================== 编译选项配置 ====================
# 选项：是否构建为共享库，共享库设为 ON；静态库设为 OFF
option(BUILD_SHARED_LIBS "Build libraries as shared" OFF)

# 选项：启用测试
option(ENABLE_TESTS "Enable testing" ON)

# 选项：开启详细编译输出
option(VERBOSE_BUILD "Enable verbose build output" OFF)
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# ==================== SFML 依赖配置 ====================
# 方式 1：使用本地安装的 SFML（推荐）
set(CMAKE_PREFIX_PATH "C:/SFML-3.0.2" ${CMAKE_PREFIX_PATH})
find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

# 方式 2：使用 FetchContent 自动下载并构建 SFML（如果本地没有安装）
if(NOT SFML_FOUND)
    message(STATUS "Local SFML not found, will fetch from GitHub...")
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
    message(STATUS "Fetching SFML from GitHub...")
    FetchContent_MakeAvailable(SFML)
    message(STATUS "SFML fetched successfully!")
else()
    message(STATUS "Found local SFML at ${SFML_DIR}")
endif()

# ==================== 可执行文件配置 ====================
# 收集所有源文件
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Snake.cpp
    src/Food.cpp
    src/Renderer.cpp
    src/SaveSystem.cpp
    src/systems/InputSystem.cpp
    src/systems/TimeSystem.cpp
    src/core/StateMachine.cpp
)

if(WIN32)
    # Windows GUI 程序，隐藏控制台窗口
    add_executable(main WIN32 ${SOURCES})
else()
    # Linux/macOS 或控制台程序
    add_executable(main ${SOURCES})
endif()

# 设置目标属性
set_target_properties(main PROPERTIES
    OUTPUT_NAME "snake_game"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 链接 SFML 库
target_link_libraries(main PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio
)

# 包含头文件目录
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 设置 C++ 标准
target_compile_features(main PRIVATE cxx_std_17)

# 添加编译定义
target_compile_definitions(main PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE>
)

# Windows 特定配置
if(WIN32)
    # MinGW-w64 静态链接配置
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Configuring static linking for MinGW/Clang on Windows")
        target_link_options(main PRIVATE
            -static-libgcc
            -static-libstdc++
        )
    endif()
    
    # 可选：完全隐藏控制台窗口（需要时取消注释）
    # target_link_options(main PRIVATE "-mwindows")
endif()

# ==================== 资源文件配置 ====================
# 复制 data 文件夹到可执行文件输出目录
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
    COMMENT "Copying data directory to output directory..."
)

# 复制 data 文件夹到 Debug 目录（如果存在）
if(CMAKE_CONFIGURATION_TYPES)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug/data
        COMMENT "Copying data directory to debug output directory..."
    )
endif()

# Windows: 复制 SFML DLL 到可执行文件目录
if(WIN32 AND SFML_FOUND)
    # 从 SFML 安装目录复制 DLL
    set(SFML_BIN_DIR "C:/SFML-3.0.2/bin")
    
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_BIN_DIR}/sfml-system-3.dll
            ${SFML_BIN_DIR}/sfml-audio-3.dll
            ${SFML_BIN_DIR}/sfml-graphics-3.dll
            ${SFML_BIN_DIR}/sfml-window-3.dll
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
        COMMENT "Copying SFML DLLs to output directory..."
    )
endif()

# ==================== 测试配置 ====================
if(ENABLE_TESTS)
    enable_testing()
    add_test(NAME SmokeTest
             COMMAND main
             WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
    message(STATUS "Tests enabled")
endif()


# ==================== 安装配置 ====================
install(TARGETS main 
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION Applications
)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/data/"
        DESTINATION "data"
        FILES_MATCHING PATTERN "*"
)

# ==================== CPack 打包配置 ====================
set(CPACK_PACKAGE_NAME "Snake")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VENDOR "HDU")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CMake-SFML-Snake Game")
set(CPACK_PACKAGE_CONTACT "contact@example.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")

# Windows 特定打包配置
if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_MODIFY_PATH ON)
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
endif()

include(CPack)
message(STATUS "CPack configured. Run 'cpack' to generate packages.")