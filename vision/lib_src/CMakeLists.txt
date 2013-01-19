add_library(${PROJECT_NAME}
  wmb.cpp
  bike_persistance.cpp
  viz.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
)
