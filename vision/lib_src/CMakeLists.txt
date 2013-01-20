add_library(${PROJECT_NAME}
  wmb_ctor.cpp
  wmb.cpp
  wmb_pre.cpp
  wmb_params.cpp
  wmb_wheels.cpp
  wmb_fork.cpp
  wmb_hist.cpp
  
  bike_persistance.cpp
  viz.cpp
  result.cpp
  bike_features.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
)
