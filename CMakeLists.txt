# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.

project(PathlineEvaluation)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_FLAGS "-g -fopenmp -Ofast")
set(CMAKE_CXX_STANDARD 14)

SET(VTKm_DIR /home/users/ssane/Repositories/VTKM/install/lib/cmake/vtkm-1.5)
find_package(VTKm REQUIRED QUIET)

add_executable(PathlineEvaluation main.cxx)

  target_include_directories(PathlineEvaluation PRIVATE ${VTKm_INCLUDE_DIRS})
  target_link_libraries(PathlineEvaluation PRIVATE vtkm_cont vtkm_io ${VTKm_LIBRARIES} ${VTK_LIBRARIES} ${MPI_LIBRARIES} ${MPI_CXX_LIBRARIES} -fopenmp)
#  target_compile_definitions(Eulerian_Reconstruct PRIVATE "VTKM_DEVICE_ADAPTER=VTKM_DEVICE_ADAPTER_SERIAL")
  target_compile_options(PathlineEvaluation PRIVATE ${VTKm_COMPILE_OPTIONS})
