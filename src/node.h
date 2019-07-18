#pragma once
#include "common_lib.h"

//----------------------------------------------------------------------------  
struct Node {
  Node(void);
  ~Node(void);
  long long int cid;
  int lid;
  double lon, lat;
  std::vector <int> lid_neighbor_node, lid_neighbor_poly;
  void reset(void);
  void set(long long int cid_, int lid_);
  void add_link(int id_neig_node_, int id_neig_poly_);
  void remove_link(void);
  double distance(double lon_, double lat_);
};
//---------------------------------------------------------------------
struct NeighborNode{
  int lid;
  double distance;
};
//---------------------------------------------------------------------
bool CompNeighborNode(const NeighborNode& a, const NeighborNode& b);




