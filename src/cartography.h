#include "common_lib.h"
#include "node.h"
#include "poly.h"

struct Cell {
  int index_i;
  int index_j;
  std::vector<int> node_lid;
  std::vector<int> segment_lid;
};

struct Cartography {
  double dlat, dlon, lat0, lon0;
  double map_dslat, map_dslon;
  std::vector<Poly> poly;
  std::vector<Node> node;
  std::vector<Segment> segment;
  std::map<long long int, int> node_cid2lid;
  std::map<int, std::map<int, Cell>> cartomap;
  int node_size;
  int poly_size;
  int segment_size;
  Cartography() {};
  void MakePoly();
  void MakeNode();
  void MakeSegment();
  void MakeMap();
  void InitCart();

  bool NearestNode(double lat, double lon, double &distance, int &node_lid);
  bool NearestPoly(double lat, double lon, double &distance, int &poly_lid);
};

