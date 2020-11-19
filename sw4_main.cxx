#include <iostream>
#include <sstream>
#include <fstream>
#include <vtkm/io/reader/VTKDataSetReader.h>
#include <vtkm/cont/DataSet.h>
#include <vtkm/cont/DataSetBuilderExplicit.h>
#include <vtkm/cont/DataSetFieldAdd.h>
#include <vtkm/io/writer/VTKDataSetWriter.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char* argv[])
{
  std::string seed_path(argv[1]);
  int num_seeds = atoi(argv[2]);
  int num_mpi = atoi(argv[3]);
  int interval = atoi(argv[4]);
  std::string gt_path(argv[5]);
  std::string eul250_path(argv[6]);
  std::string eul500_path(argv[6]);
  std::string lag1_path(argv[8]);
  std::string lag8_path(argv[9]);
  std::string lag27_path(argv[10]);
  std::string lag64_path(argv[11]);
  std::string output_path(argv[12]);

  float *seeds = (float*)malloc(sizeof(float)*num_seeds*3);
  float x1, y1, z1;  
  std::ifstream seed_stream(seed_path);
  int counter=0;
  while(seed_stream >> x1)
  {
    seed_stream >> y1;
    seed_stream >> z1;
    seeds[counter*3+0] = x1;
    seeds[counter*3+1] = y1;
    seeds[counter*3+2] = z1;
    counter++;
  }

  std::cout << "Displacement, Test" << std::endl;
  vtkm::cont::DataSet input_data;

  float *test = (float*)malloc(sizeof(float)*num_seeds*3);
  int *check = (int*)malloc(sizeof(int)*num_seeds);  
    
  for(int i = 0; i < num_seeds; i++)
  {
    check[i] = 0;
  }

  int connectivity_index = 0;
  std::vector<vtkm::Id> connectivity;
  std::vector<vtkm::Vec<vtkm::Float64, 3>> pointCoordinates;
  std::vector<vtkm::UInt8> shapes;
  std::vector<vtkm::IdComponent> numIndices;
  std::vector<vtkm::Id> pathlineId;
  std::vector<vtkm::Float32> gt;
  std::vector<vtkm::Float32> eul_1;
  std::vector<vtkm::Float32> eul_2;
  std::vector<vtkm::Float32> lag_1;
  std::vector<vtkm::Float32> lag_2;
  std::vector<vtkm::Float32> lag_3;
  std::vector<vtkm::Float32> lag_4;

  for(int i = 0; i < num_seeds; i++)
  { 
     connectivity.push_back(connectivity_index);
     connectivity_index += 1;
     pointCoordinates.push_back(
          vtkm::Vec<vtkm::Float32, 3>(seeds[i*3+0], seeds[i*3+1], seeds[i*3+2]));
     shapes.push_back(vtkm::CELL_SHAPE_VERTEX);
     numIndices.push_back(1);
     pathlineId.push_back(i); 
  }


  // GT 
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << gt_path << n << "_" << interval << ".vtk";
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

    for(int i = 0; i < num_pathline_segments; i++)
    {
      auto pt = coords_portal.Get(i*2 + 1);
      int index = id_portal.Get(i);
      test[index*3+0] = pt[0];
      test[index*3+1] = pt[1];
      test[index*3+2] = pt[2];
      check[index] = 1;
    }
  }

  for(int i = 0; i < num_seeds; i++)
  {
    if(check[i] == 1)
    {
      float dist = sqrt(pow((test[i*3+0]-seeds[i*3+0]),2.0) + pow((test[i*3+1]-seeds[i*3+1]),2.0) + pow((test[i*3+2]-seeds[i*3+2]),2.0));
      std::cout << dist << ", " << "GT" << std::endl;
      gt.push_back(dist);
    }
    else
    {
      gt.push_back(-1.0);
    }
  }
  
  for(int i = 0; i < num_seeds; i++)
  {
    check[i] = 0;
  }
  
  // Eul 250 
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << eul250_path << n << "_" << interval << ".vtk";
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

    for(int i = 0; i < num_pathline_segments; i++)
    {
      auto pt = coords_portal.Get(i*2 + 1);
      int index = id_portal.Get(i);
      test[index*3+0] = pt[0];
      test[index*3+1] = pt[1];
      test[index*3+2] = pt[2];
      check[index] = 1;
    }
  }

  for(int i = 0; i < num_seeds; i++)
  {
    if(check[i] == 1)
    {
      float dist = sqrt(pow((test[i*3+0]-seeds[i*3+0]),2.0) + pow((test[i*3+1]-seeds[i*3+1]),2.0) + pow((test[i*3+2]-seeds[i*3+2]),2.0));
      std::cout << dist << ", " << "Eul_250" << std::endl;
        eul_1.push_back(dist);
    }
    else
    {
      eul_1.push_back(-1.0);
    }
  }
  
  for(int i = 0; i < num_seeds; i++)
  {
    check[i] = 0;
  }
  
  // Eul 500 
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << eul500_path << n << "_" << interval << ".vtk";
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

    for(int i = 0; i < num_pathline_segments; i++)
    {
      auto pt = coords_portal.Get(i*2 + 1);
      int index = id_portal.Get(i);
      test[index*3+0] = pt[0];
      test[index*3+1] = pt[1];
      test[index*3+2] = pt[2];
      check[index] = 1;
    }
  }

  for(int i = 0; i < num_seeds; i++)
  {
    if(check[i] == 1)
    {
      float dist = sqrt(pow((test[i*3+0]-seeds[i*3+0]),2.0) + pow((test[i*3+1]-seeds[i*3+1]),2.0) + pow((test[i*3+2]-seeds[i*3+2]),2.0));
      std::cout << dist << ", " << "Eul_500" << std::endl;
      eul_2.push_back(dist);
    }
    else
    {
      eul_2.push_back(-1.0);
    }
  }
  
  for(int i = 0; i < num_seeds; i++)
  {
    check[i] = 0;
  }

  // Lag 1:1 
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << lag1_path << n << "_" << interval << ".vtk";
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

    for(int i = 0; i < num_pathline_segments; i++)
    {
      auto pt = coords_portal.Get(i*2 + 1);
      int index = id_portal.Get(i);
      test[index*3+0] = pt[0];
      test[index*3+1] = pt[1];
      test[index*3+2] = pt[2];
      check[index] = 1;
    }
  }

  for(int i = 0; i < num_seeds; i++)
  {
    if(check[i] == 1)
    {
      float dist = sqrt(pow((test[i*3+0]-seeds[i*3+0]),2.0) + pow((test[i*3+1]-seeds[i*3+1]),2.0) + pow((test[i*3+2]-seeds[i*3+2]),2.0));
      std::cout << dist << ", " << "Lag_1:1" << std::endl;
      lag_1.push_back(dist);
    }
    else
    {
      lag_1.push_back(-1.0);
    }
  }
  
  for(int i = 0; i < num_seeds; i++)
  {
    check[i] = 0;
  }

  // Lag 1:8 
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << lag8_path << n << "_" << interval << ".vtk";
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

    for(int i = 0; i < num_pathline_segments; i++)
    {
      auto pt = coords_portal.Get(i*2 + 1);
      int index = id_portal.Get(i);
      test[index*3+0] = pt[0];
      test[index*3+1] = pt[1];
      test[index*3+2] = pt[2];
      check[index] = 1;
    }
  }

  for(int i = 0; i < num_seeds; i++)
  {
    if(check[i] == 1)
    {
      float dist = sqrt(pow((test[i*3+0]-seeds[i*3+0]),2.0) + pow((test[i*3+1]-seeds[i*3+1]),2.0) + pow((test[i*3+2]-seeds[i*3+2]),2.0));
      std::cout << dist << ", " << "Lag_1:8" << std::endl;
      lag_2.push_back(dist);
    }
    else
    {
      lag_2.push_back(-1.0);
    }
  }
  
  for(int i = 0; i < num_seeds; i++)
  {
    check[i] = 0;
  }
  
  // Lag 1:27
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << lag27_path << n << "_" << interval << ".vtk";
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

    for(int i = 0; i < num_pathline_segments; i++)
    {
      auto pt = coords_portal.Get(i*2 + 1);
      int index = id_portal.Get(i);
      test[index*3+0] = pt[0];
      test[index*3+1] = pt[1];
      test[index*3+2] = pt[2];
      check[index] = 1;
    }
  }

  for(int i = 0; i < num_seeds; i++)
  {
    if(check[i] == 1)
    {
      float dist = sqrt(pow((test[i*3+0]-seeds[i*3+0]),2.0) + pow((test[i*3+1]-seeds[i*3+1]),2.0) + pow((test[i*3+2]-seeds[i*3+2]),2.0));
      std::cout << dist << ", " << "Lag_1:27" << std::endl;
      lag_3.push_back(dist);
    }
    else
    {
      lag_3.push_back(-1.0);
    }
  }
  
  for(int i = 0; i < num_seeds; i++)
  {
    check[i] = 0;
  }

  // Lag 1:64
  for(int n = 0; n < num_mpi; n++)
  {
    std::stringstream filename;
    filename << lag27_path << n << "_" << interval << ".vtk";
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

    for(int i = 0; i < num_pathline_segments; i++)
    {
      auto pt = coords_portal.Get(i*2 + 1);
      int index = id_portal.Get(i);
      test[index*3+0] = pt[0];
      test[index*3+1] = pt[1];
      test[index*3+2] = pt[2];
      check[index] = 1;
    }
  }

  for(int i = 0; i < num_seeds; i++)
  {
    if(check[i] == 1)
    {
      float dist = sqrt(pow((test[i*3+0]-seeds[i*3+0]),2.0) + pow((test[i*3+1]-seeds[i*3+1]),2.0) + pow((test[i*3+2]-seeds[i*3+2]),2.0));
      std::cout << dist << ", " << "Lag_1:64" << std::endl;
      lag_4.push_back(dist);
    }
    else
    {
      lag_4.push_back(-1.0);
    }
  }
  
  for(int i = 0; i < num_seeds; i++)
  {
    check[i] = 0;
  }


vtkm::cont::DataSetBuilderExplicit DSB_Explicit;
vtkm::cont::DataSet pathlines = DSB_Explicit.Create(pointCoordinates, shapes, numIndices, connectivity);
vtkm::cont::DataSetFieldAdd dsfa;
dsfa.AddCellField(pathlines, "ID", pathlineId);
dsfa.AddPointField(pathlines, "GT", gt);
dsfa.AddPointField(pathlines, "Eul250", eul_1);
dsfa.AddPointField(pathlines, "Eul500", eul_2);
dsfa.AddPointField(pathlines, "Lag1", lag_1);
dsfa.AddPointField(pathlines, "Lag8", lag_2);
dsfa.AddPointField(pathlines, "Lag27", lag_3);
dsfa.AddPointField(pathlines, "Lag64", lag_4);

vtkm::io::writer::VTKDataSetWriter writer(output_path.c_str());
writer.WriteDataSet(pathlines);


}
