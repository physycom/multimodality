#pragma once
#include "common_lib.h"

using namespace std;
//---------------------------------------------------------------------
struct SeedPro {   
  double distance;
  int node_lid, neig_node, neig_poly;
  void set(int node_lid_, int neig_node_, int neig_poly_, double distance_);
};
//---------------------------------------------------------------------
struct Seed {
  double dd;  // dd= distance + d_eu
  int cnt;
  bool operator<(const Seed &s) const { return dd > s.dd; }
};
//---------------------------------------------------------------------
struct CompSeed {
  bool operator()(const Seed& a, const Seed& b) { return (a.dd > b.dd); }
};
//---------------------------------------------------------------------
