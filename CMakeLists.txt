cmake_minimum_required(VERSION 2.6)

project(ImageValidationISLES)

include (${CMAKE_ROOT}/Modules/FindITK.cmake)
if (USE_ITK_FILE)
  include(${USE_ITK_FILE})
endif (USE_ITK_FILE)

add_executable(validateLabelImages-ISLES validateLabelImages-ISLES.cxx)
target_link_libraries(validateLabelImages-ISLES ${ITK_LIBRARIES} ${VTK_LIBRARIES})