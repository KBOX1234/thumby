cmake_minimum_required(VERSION 3.16)
project(thumby)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------
# Collect all source files
# -------------------------
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# -------------------------
# Create executable
# -------------------------
add_executable(thumby ${SRC_FILES})

# -------------------------
# Include headers
# -------------------------
target_include_directories(thumby PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# -------------------------
# wxWidgets detection
# -------------------------

# Try modern config first
find_package(wxWidgets CONFIG QUIET)

if(wxWidgets_FOUND)
    message(STATUS "Using wxWidgets via CONFIG")
    target_link_libraries(thumby PRIVATE wx::core wx::base)
else()
    # Fallback
    find_package(wxWidgets REQUIRED COMPONENTS core base)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(thumby ${wxWidgets_LIBRARIES})
endif()
