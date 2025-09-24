cmake_minimum_required(VERSION 3.16...3.27)
project(MaterialStudy)

find_package(Geant4 REQUIRED ui_all vis_all)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)   
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)  

add_executable(si_cyl main.cc ${sources} ${headers})
target_include_directories(si_cyl PRIVATE include)
target_link_libraries(si_cyl PRIVATE ${Geant4_LIBRARIES})


set(EXAMPLEB4A_SCRIPTS
  macros/init_vis.mac
  )

foreach(_script ${EXAMPLEB4A_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

