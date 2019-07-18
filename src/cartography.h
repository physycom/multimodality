#include "common_lib.h"
#include "node.h"
#include "poly.h"

struct Cartography {
  double dlat, dlon, lat0, lon0;
  std::vector<Poly> poly;
  std::vector<Node> node;
  std::map<long long int, int> node_cid2lid;
  int node_size;
  int poly_size;
  Cartography() {};
  void MakePoly();
  void MakeNode();
  void InitCart();
};