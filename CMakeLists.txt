cmake_minimum_required(VERSION 3.9)

set(prjname "my-prj")

# Set the project name and language
project( ${prjname}
         VERSION 0.1.0
         DESCRIPTION "Project Template"
         LANGUAGES CXX C
)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in   ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in.1)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in.1 ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

# Disable build for libure examples
set(URE_BUILD_EXAMPLES     OFF)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" ) 
set_property(CACHE CMAKE_BUILD_TYPE       PROPERTY STRINGS Release Debug)

set(URE_WINDOWS_MANAGER  "glfw"    CACHE STRING "Windows, Context and Event Manager" )
set_property(CACHE URE_WINDOWS_MANAGER    PROPERTY STRINGS glfw glut sdl)

set(URE_BACKEND_RENDER   "gles"    CACHE STRING "Rendering system" )
set_property(CACHE URE_BACKEND_RENDER     PROPERTY STRINGS gles opengl2 opengl3 vulkan wgpu)

if ( URE_WINDOWS_MANAGER STREQUAL "" )
  message(FATAL_ERROR "URE_WINDOWS_MANAGER must be set to a valid value")
endif()



find_package(Git)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Update submodules with remote" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote -f
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive --remote -f : failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

add_subdirectory(submodules/ure)

message( "PARENT_DEFINITIONS: ${PARENT_DEFINITIONS}" )
message( "PARENT_LIBS       : ${PARENT_LIBS}"        )
add_definitions( ${PARENT_DEFINITIONS} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include                                     )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/ure/include                      )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/ure/3rd-party/glad/include       )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/ure/submodules/lock-free/include )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/ure/submodules/glm               )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/submodules/ure/submodules/libsigc++         )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/submodules/ure/submodules/libsigc++         ) # necessary for sigc++config.h


# uncomment to set a default CXX standard for the external tools like clang-tidy and cppcheck
# and the targets that do not specify a standard.
# If not set, the latest supported standard for your compiler is used
# You can later set fine-grained standards for each target using `target_compile_features`
# Note: linking together projects compiled with different C++ standards may work, but
# it is not recommended because of possible issues with ABI
set(CMAKE_CXX_STANDARD 20)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS         ON)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_C_STANDARD_REQUIRED    ON)

set(  LIB_BINARY_DIR 
      ${CMAKE_CURRENT_BINARY_DIR}
   )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions(
                  -ggdb3
                  -D_DEBUG
  )
endif()

if(MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(-D_CRT_SECURE_NO_WARNINGS /W4 /WX)
else()
  # lots of warning
  add_compile_options(-Wall -Wextra -pedantic)
  
  set( EXT_LIBRARIES
      m
      pthread
     )
endif()

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )

else()

endif()


file( GLOB 
      LIB_INCLUDE
      ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    )

file( GLOB 
      LIB_SRC
      ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    )

message("${prjname}: CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
message("${prjname}: URE_LINK_FLAGS : ${URE_LINK_FLAGS}"  )

add_executable       ( ${prjname}        ${LIB_SRC}         )

target_link_libraries( ${prjname}        "${PARENT_LIBS}"   )

if(ENABLE_WASM)
option(JS_ONLY            "Build with WASM=0"          OFF)
endif()

if(ENABLE_WASM)
if(JS_ONLY)
	message(STATUS "${prjname}: Setting compilation target to native JavaScript")
	set(CMAKE_EXECUTABLE_SUFFIX ".js")
	set_target_properties( ${prjname} PROPERTIES LINK_FLAGS "${URE_LINK_FLAGS} -s WASM=0 -s USE_WEBGL2=1 -s FULL_ES3=1 --preload-file ../resources@resources -s EXPORTED_FUNCTIONS='[_main]'")
else(JS_ONLY)
	message(STATUS "${prjname}: Setting compilation target to WASM")
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
	set_target_properties( ${prjname} PROPERTIES LINK_FLAGS "${URE_LINK_FLAGS} -s WASM=1 -s USE_WEBGL2=1 -s FULL_ES3=1 
                                                        -s EXPORTED_RUNTIME_METHODS=['UTF8ToString'] 
                                                        --preload-file ../resources@resources -s EXPORTED_FUNCTIONS='[_main]'")
endif(JS_ONLY)
else(ENABLE_WASM)
  target_link_libraries( ${prjname}            ${EXT_LIBRARIES} )
  target_link_libraries( ${prjname}            ${CMAKE_DL_LIBS} )
endif(ENABLE_WASM)
