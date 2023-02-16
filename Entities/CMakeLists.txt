cmake_minimum_required(VERSION 3.8)
project(Entities)

find_package(Qt5 COMPONENTS Core Xml REQUIRED)

FILE(GLOB Sources *.cpp)
FILE(GLOB Headers *.h)
FILE(GLOB UIs *.ui)

SOURCE_GROUP("Sources" FILES ${Sources})
SOURCE_GROUP("Header" FILES ${Headers})
SOURCE_GROUP("UIs" FILES ${UIs})

add_library(Entities ${Sources} ${Headers})

target_link_libraries(Entities
  PRIVATE
    Qt5::Core
    Qt5::Xml)
