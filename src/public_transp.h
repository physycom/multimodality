#pragma once
#include "common_lib.h"

class BusStop {
public:
  int busstop_lid;
  double lat, lon;
  double dist_node;
  int node_lid, poly_lid;
  int code_area;
  std:: string name;
  std::string location;
  
  std::vector<int> busline;

  BusStop() {};
  BusStop(int busstop_lid_, std::string name_, std::string location_, double lat_, double lon_, int code_area_);

};

class BusLine {
public:
  int busline_lid, busline_cid;

  std::vector<int> busstop;
};

struct PublicTransp {

  std::vector<BusStop> busstop;
  std::vector<BusLine> busline;

  int busstop_size;
  int busline_size;

  PublicTransp() {};
  void InitPublicTransp();
};