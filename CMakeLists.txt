# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.

project(PathlineEvaluation)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_FLAGS "-g -fopenmp -Ofast")
set(CMAKE_CXX_STANDARD 14)

SET(VTKm_DIR /home/users/ssane/Dev/VIS20_research/Lagrangian_grid_interpolation/VTKM/install/lib/cmake/vtkm-1.5)
find_package(VTKm REQUIRED QUIET)

add_executable(PathlineEvaluation main.cxx)
add_executable(SW4_Evaluation sw4_main.cxx)

################################

  target_include_directories(PathlineEvaluation PRIVATE ${VTKm_INCLUDE_DIRS})
  target_link_libraries(PathlineEvaluation PRIVATE vtkm_cont ${VTKm_LIBRARIES} ${VTK_LIBRARIES} ${MPI_LIBRARIES} ${MPI_CXX_LIBRARIES} -fopenmp)
  target_compile_options(PathlineEvaluation PRIVATE ${VTKm_COMPILE_OPTIONS})


################################

  target_include_directories(SW4_Evaluation PRIVATE ${VTKm_INCLUDE_DIRS})
  target_link_libraries(SW4_Evaluation PRIVATE vtkm_cont ${VTKm_LIBRARIES} ${VTK_LIBRARIES} ${MPI_LIBRARIES} ${MPI_CXX_LIBRARIES} -fopenmp)
  target_compile_options(SW4_Evaluation PRIVATE ${VTKm_COMPILE_OPTIONS})

