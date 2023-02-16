cmake_minimum_required(VERSION 3.8)
project(Demo2_1)

# set(ParaView_DIR D:\\Project\\pv\\build\\lib\\cmake\\paraview-5.11)
find_package(ParaView REQUIRED)

if (NOT PARAVIEW_USE_QT)
  message(STATUS
    "Skipping example: ${PROJECT_NAME} example requires PARAVIEW_USE_QT "
    "to be enabled. Please rebuild ParaView (or point to a different build of "
    "ParaView) with PARAVIEW_USE_QT set to TRUE")
  return ()
endif()

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

find_package(Qt5 REQUIRED COMPONENTS Widgets Xml )

set(CMAKE_AUTOMOC 1)
set(CMAKE_AUTOUIC 1)

# List source files & resources
FILE(GLOB Sources *.cpp *.cxx *.md)
FILE(GLOB Headers *.h)
FILE(GLOB UIs *.ui)

SOURCE_GROUP("Sources" FILES ${Sources})
SOURCE_GROUP("Header" FILES ${Headers})
SOURCE_GROUP("UIs" FILES ${UIs})

add_executable(DemoApp2 DemoApp2.cxx ${Sources} ${Headers} ${UIs})

add_subdirectory(Entities)
add_subdirectory(Functions)
include_directories(./Entities)
include_directories(./Functions)

target_link_libraries(DemoApp2
  PRIVATE
    ParaView::pqApplicationComponents
    Qt5::Widgets
    Qt5::Xml
    Entities
    Functions)

configure_file(ParaViewFilters.xml ${CMAKE_BINARY_DIR} COPYONLY)
