#include <vtkm/cont/DataSet.h>
#include <vtkm/cont/DataSetBuilderExplicit.h>
#include <vtkm/cont/DataSetFieldAdd.h>
#include <vtkm/io/writer/VTKDataSetWriter.h>
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

vtkm::cont::DataSet input_data;
vtkm::cont::DataSet output_data;

int connectivity_index = 0;
std::vector<vtkm::Id> connectivity;
std::vector<vtkm::Vec<vtkm::Float64, 3>> pointCoordinates;
std::vector<vtkm::UInt8> shapes;
std::vector<vtkm::IdComponent> numIndices;
std::vector<vtkm::Id> pathlineId;
std::vector<vtkm::Int32> particleTime;

for(int cycle = start; cycle <= end; cycle += interval)
{
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << input_path << n << "_" << cycle << ".vtk";
    vtkm::io::reader::VTKDataSetReader reader(filename.str().c_str());
    input_data = reader.ReadDataSet();
    
    vtkm::cont::ArrayHandleVirtualCoordinates coordinatesystem = input_data.GetCoordinateSystem().GetData();
    int num_points_in_rank = coordinatesystem.GetNumberOfValues();
    int num_pathline_segments = num_points_in_rank/2;
    if(num_pathline_segments == 0)
    {
      continue;
    } 
  
    auto coords_portal = input_data.GetCoordinateSystem().GetData().GetPortalControl();
    auto id_vah = input_data.GetField("ID").GetData();
    vtkm::cont::ArrayHandle<vtkm::Int32> id_arrayhandle;
    id_vah.CopyTo(id_arrayhandle);
    auto id_portal = id_arrayhandle.GetPortalControl(); 
  
    // Add to the info to go into output
    for(int i = 0; i < num_pathline_segments; i++)
    {
      auto pt1 = coords_portal.Get(i*2 + 0);
      auto pt2 = coords_portal.Get(i*2 + 1);

      connectivity.push_back(connectivity_index);
      connectivity.push_back(connectivity_index + 1);
      connectivity_index += 2;
      pointCoordinates.push_back(
           vtkm::Vec<vtkm::Float64, 3>(pt1[0], pt1[1], pt1[2]));
      pointCoordinates.push_back(
           vtkm::Vec<vtkm::Float64, 3>(pt2[0], pt2[1], pt2[2]));
      shapes.push_back(vtkm::CELL_SHAPE_LINE);
      numIndices.push_back(2);
      pathlineId.push_back(i);
      particleTime.push_back(cycle-interval);
      particleTime.push_back(cycle);
    }
  }
}

vtkm::cont::DataSetBuilderExplicit DSB_Explicit;
vtkm::cont::DataSet pathlines = DSB_Explicit.Create(pointCoordinates, shapes, numIndices, connectivity);
vtkm::cont::DataSetFieldAdd dsfa;
dsfa.AddCellField(pathlines, "ID", pathlineId);
dsfa.AddPointField(pathlines, "Time", particleTime);

vtkm::io::writer::VTKDataSetWriter writer(output_path.c_str());
writer.WriteDataSet(pathlines);

}
