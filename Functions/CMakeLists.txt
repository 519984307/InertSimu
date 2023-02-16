cmake_minimum_required(VERSION 3.8)
project(Functions)

find_package(Qt5 COMPONENTS Core Xml REQUIRED)

FILE(GLOB Sources *.cpp)
FILE(GLOB Headers *.h)
FILE(GLOB UIs *.ui)

SOURCE_GROUP("Sources" FILES ${Sources})
SOURCE_GROUP("Header" FILES ${Headers})
SOURCE_GROUP("UIs" FILES ${UIs})

include_directories(../Entities)

set(QRC_SOURCE_FILES SPHResource.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

add_library(Functions ${Sources} ${Headers} ${QRC_FILES})

target_link_libraries(Functions
  PRIVATE
    Qt5::Core
    Qt5::Xml)

