add_executable(yaml_test yaml_test.cpp)
target_link_libraries(yaml_test ${PROJECT_NAME} ${OpenCV_LIBS})