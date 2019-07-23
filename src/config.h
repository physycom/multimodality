#pragma once
#include "common_lib.h"


struct Config {

  std::string city_tag;
  std::string file_pro;
  std::string file_pnt;
  std::string file_bus_stop;
  std::string file_bus_arc;
  std::string file_bus_line;
  std::vector<std::string> file_data;

  std::string start_datetime;
  std::string end_datetime;
  size_t start_time;
  size_t end_time;

  double lat_max;
  double lat_min;
  double lon_max;
  double lon_min;
  double dslat;
  double dslon;

  double map_resolution;
  double grid_resolution;
  double l_gauss;
  double min_data_distance;
  double min_node_distance;
  double min_poly_distance;

  Config() {};
  void set_config(jsoncons::json _jconf);

  void info();
}; 



