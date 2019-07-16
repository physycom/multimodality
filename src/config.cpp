#include <physycom/time.hpp>
#include "config.h"
//#include "global_info.h"


void config::set_config(jsoncons::json jconf)
{
  this->city_tag = jconf.has_member("city_tag") ? jconf["city_tag"].as<std::string>() : "city_name";

  this->file_pro = jconf.has_member("file_pro") ? jconf["file_pro"].as<std::string>() : "roads.pro";
  this->file_pnt = jconf.has_member("file_pnt") ? jconf["file_pnt"].as<std::string>() : "roads.pnt";
  if (jconf.has_member("file_data"))
    this->file_data = jconf["file_data"].as<std::vector<std::string>>();
  else
    this->file_data = { "data.csv" };

  this->start_datetime = jconf.has_member("start_time") ? jconf["start_time"].as<std::string>() : "2017-07-13 10:00:00";
  this->end_datetime = jconf.has_member("end_time") ? jconf["end_time"].as<std::string>() : "2017-07-13 10:00:00";
  this->start_time = physycom::date_to_unix(this->start_datetime);
  this->end_time = physycom::date_to_unix(this->end_datetime);

  this->lat_max = jconf.has_member("lat_max") ? jconf["lat_max"].as<double>() : 45.451410;
  this->lat_min = jconf.has_member("lat_min") ? jconf["lat_min"].as<double>() : 45.451410;
  this->lon_max = jconf.has_member("lon_max") ? jconf["lon_max"].as<double>() : 12.372170;
  this->lon_min = jconf.has_member("lon_min") ? jconf["lon_min"].as<double>() : 12.372170;
  this->dslat = 111053.8;
  //this->dslon = this->dslat*cos((this->lat_max + this->lat_min) / 2 * PI_180);
  this->dslon = this->dslat*cos((this->lat_max + this->lat_min) / 2 );

  this->map_resolution = jconf.has_member("map_resolution") ? jconf["map_resolution"].as<double>() : 60.0;
  this->grid_resolution = jconf.has_member("grid_resolution") ? jconf["grid_resolution"].as<double>() : 150.0;
  this->l_gauss = jconf.has_member("l_gauss") ? jconf["l_gauss"].as<double>() : 10.0;
  this->min_data_distance = jconf.has_member("min_data_distance") ? jconf["min_data_distance"].as<double>() : 49.0;
  this->min_node_distance = jconf.has_member("min_node_distance") ? jconf["min_node_distance"].as<double>() : 20.0;
  this->min_poly_distance = jconf.has_member("min_poly_distance") ? jconf["min_poly_distance"].as<double>() : 50.0;

  info();
}
void config::info()
{
  std::cout << "******** DATA INFO ********" << std::endl;
  for (const auto &i : file_data)
    std::cout << "* Data file      : " << i << std::endl;
  std::cout << "* Time range : " << start_datetime << " " << end_datetime << std::endl;
  std::cout << "* LAT range : " << lat_min << " " << lat_max << std::endl;
  std::cout << "* LON range : " << lon_min << " " << lon_max << std::endl;
  std::cout << "**********************************" << std::endl;
}
