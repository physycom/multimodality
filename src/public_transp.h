#pragma once
#include "common_lib.h"

class BusStop {
public:
  int busstop_lid;
  double lat, lon;
  double dist_node;
  double dist_poly;
  int node_lid, poly_lid;
  int code_area;
  std:: string name;
  std::string location;
  
  std::vector<string> busline;

  BusStop() {};
  BusStop(int busstop_lid_, std::string name_, std::string location_, double lat_, double lon_, int code_area_);

};

class BusArc {
public:
  int busarc_lid;
  int stop_start;
  int stop_end;
  std::vector<Point> path;
  std::list<int> poly_path;

  void Clear();
};

class BusLine {
public:
  string busline_lid;
  int path;
  string oneway;

  std::vector<int> busstop;
  std::vector<int> busarc;
  BusLine() {};
};

struct PublicTransp {

  std::vector<BusStop> busstop;
  std::vector<BusLine> busline;
  std::vector<BusArc>  busarc;
  std::map<int, int> busstop_proxy;
  std::map<int, int> busarc_proxy;

  int busstop_size;
  int busarc_size;
  int busline_size;

  PublicTransp() {};
  void InitPublicTransp();
  void MakeBusStop();
  void MakeBusArc();
  void MakeBusLine();
};