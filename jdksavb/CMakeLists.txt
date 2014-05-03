cmake_minimum_required (VERSION 2.8)
project (jdksavb)
enable_testing()

option(TODO "Enable TODO items that are in progress" OFF)
option(TESTS "Enable building of extended test code in library" ON)
option(EXAMPLES "Enable building of example programs" ON)
option(TOOLS "Enable building of tools" ON)

if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions("/W2")
else()
    add_definitions("-Wall -Wextra")
endif()

if(TODO MATCHES "ON")
   add_definitions("-DTODO=1")
   message(STATUS "TODO items that are in progress are enabled for compiling")
endif()


set(LIBS ${LIBS} ${CHECK_LIBRARIES} jdksavb)

include_directories( include )
include_directories( ../jdksavdecc-c/include )

file( GLOB JDKSAVDECC_C_INCLUDES "../jdksavdecc-c/include/*.h")
file( GLOB JDKSAVDECC_C_SRC "../jdksavdecc-c/src/*.c")

if(TESTS MATCHES "ON")
   message(STATUS "Extended test code functions are enabled")
   file(GLOB JDKSAVB_INCLUDES "include/*.h" "include/tests/*.h" )
   file(GLOB JDKSAVB_SRC "src/*.c" "src/test/*.c")
else()
   file(GLOB JDKSAVB_INCLUDES "include/*.h" )
   file(GLOB JDKSAVB_SRC "src/*.c")
endif()

add_library (jdksavb ${JDKSAVB_SRC} ${JDKSAVB_INCLUDES} ${JDKSAVDECC_C_SRC} ${JDKSAVDECC_C_INCLUDES})

INSTALL(TARGETS jdksavb DESTINATION "lib")

INSTALL(FILES
   ${JDKSAVB_INCLUDES}
   ${JDKSAVDECC_C_INCLUDES}
   DESTINATION include
)

link_directories( ${JDKSAVB_BINARY_DIR} )


if(EXAMPLES MATCHES "ON")
    file(GLOB JDKSAVB_EXAMPLES "examples/*.c")
    foreach(item ${JDKSAVB_EXAMPLES})
      GET_FILENAME_COMPONENT(examplename ${item} NAME_WE )
      add_executable(${examplename} ${item})
      target_link_libraries(${examplename} ${LIBS} )
    endforeach(item)
endif()

if(TOOLS MATCHES "ON")
    file(GLOB JDKSAVB_TOOLS_DEV "tools-dev/*.c")
    foreach(item ${JDKSAVB_TOOLS_DEV})
      GET_FILENAME_COMPONENT(toolname ${item} NAME_WE )
      add_executable(${toolname} ${item})
      target_link_libraries(${toolname} ${LIBS} )
    endforeach(item)
endif()

if(TESTS MATCHES "ON")
   file(GLOB JDKSAVB_TESTS "tests/*.c")
   foreach(item ${JDKSAVB_TESTS})
      GET_FILENAME_COMPONENT(testname ${item} NAME_WE )
      add_executable(${testname} ${item})
      target_link_libraries(${testname} ${LIBS} )
      add_test(NAME ${testname} COMMAND ${testname} )
   endforeach(item)
endif()

