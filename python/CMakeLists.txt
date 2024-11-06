file(GLOB_RECURSE SRC_FILES 
  ${CMAKE_CURRENT_LIST_DIR}/*.cc
)

pybind11_add_module(pybinder ${SRC_FILES})
target_link_libraries(pybinder PRIVATE ${PROJECT_NAME})
