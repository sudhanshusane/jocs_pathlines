#include <vtkm/cont/DataSet.h>
#include <vtkm/io/reader/VTKDataSetReader.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

int main(int argc, char* argv[])
{
// Setup
std::string input_path(argv[1]);
int start = atoi(argv[2]);
int end = atoi(argv[3]);
int interval = atoi(argv[4]);
int num_mpi = atoi(argv[5]);
std::string output_path(argv[6]);

std::cout << "input: " << argv[1] << std::endl;
std::cout << "start: " << argv[2] << std::endl;
std::cout << "end: " << argv[3] << std::endl;
std::cout << "interval: " << argv[4] << std::endl;
std::cout << "num_mpi: " << argv[5] << std::endl;
std::cout << "output: " << argv[6] << std::endl;

for(int cycle = start; cycle <= end; cycle += interval)
{
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << input_path << n << "_" << cycle << ".vtk";
    std::cout << filename.str().c_str() << std::endl;
  }
}

}
