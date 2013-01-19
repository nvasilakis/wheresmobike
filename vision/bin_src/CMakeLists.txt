add_executable(yaml_test yaml_test.cpp)
target_link_libraries(yaml_test ${PROJECT_NAME} ${OpenCV_LIBS})

add_executable(trainer trainer.cpp)
target_link_libraries(trainer ${PROJECT_NAME} ${OpenCV_LIBS})

add_executable(magic magic.cpp)
target_link_libraries(magic ${PROJECT_NAME} ${OpenCV_LIBS})