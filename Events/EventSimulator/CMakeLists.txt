################################################################################
# Package: prometheus
################################################################################



include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE HEADERS EventSimulator/*)

ROOT_GENERATE_DICTIONARY(EventSimulatorDict ${HEADERS}
                                        LINKDEF
                                        ${CMAKE_CURRENT_SOURCE_DIR}/Root/LinkDef.h
                                        MODULE
                                        EventSimulator)
                                      
                                      add_library(EventSimulator  OBJECT EventSimulatorDict.cxx)
install(FILES ${HEADERS}  DESTINATION EventSimulator)

gaugi_install_python_modules( ${CMAKE_CURRENT_SOURCE_DIR}/python EventSimulator)




