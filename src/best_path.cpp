#include "best_path.h"
#include "cartography.h"

extern Cartography cart;

void SeedPro::set(int node_lid_, int neig_node_, int neig_poly_, double distance_) {
  this->node_lid = node_lid_;
  this->neig_node = neig_node_;
  this->neig_poly = neig_poly_;
  this->distance = distance_;
}

list<int> Cartography::BestPath(double lat_start, double lon_start, double lat_end, double lon_end) {
  list<int> bpath;
  int node_start, node_end;
  double dist_start, dist_end;
  if (!cart.NearestNode(lat_start, lon_start, dist_start, node_start) || !cart.NearestNode(lat_end, lon_end, dist_end, node_end))
    return bpath;
  if (node_start == node_end) return bpath; 
  static int *index;
  Seed node_visited;           //  (.cnt .dd = dist+d_eu)   d_eu= 0.9*euclid_dist
  SeedPro node_visited_pro;     // (.distance .node_lid .neig_poly .neig_node)
  vector <SeedPro> list_node_pro; vector <int> visited;

  int nw, inw, nw_neig, pw_neig, iv, iter = 0;
  double x1, y1, x2, y2, dx, dy, dist, d_eu, a_eu = 0.90;
  double distance, best_dist;
  static bool first_time = true;
  if (first_time) {
    first_time = false;
    index = new int[cart.node_size]; for (int n = 0; n < node_size; n++) index[n] = 0;
  }

  bool reached_goal = false;
  priority_queue<Seed, vector <Seed>, CompSeed> heap_B;

  x2 = node[node_end].lon; y2 = node[node_end].lat;           // end
  x1 = node[node_start].lon; y1 = node[node_start].lat;       // start

  dx = cart.dlon* (x1 - x2); dy = cart.dlat * (y1 - y2); 
  d_eu = a_eu * sqrt(dx*dx + dy * dy);
  
  iv = 0; node_visited_pro.set(0, 0, 0, 0.0); list_node_pro.push_back(node_visited_pro);   
  iv = 1;   node_visited_pro.set(node_start, 0, 0, 0.0); list_node_pro.push_back(node_visited_pro);
  node_visited.cnt = iv; node_visited.dd = d_eu; heap_B.push(node_visited);

  while (!heap_B.empty()) {
    node_visited = heap_B.top(); heap_B.pop(); iter++; 
    nw = list_node_pro[node_visited.cnt].node_lid;  
    distance = list_node_pro[node_visited.cnt].distance; 
    inw = index[nw]; 
    if (inw > 0 && (list_node_pro[inw].distance < distance)) continue; 
    if (inw == 0) visited.push_back(nw); 
    index[nw] = node_visited.cnt;
    if (reached_goal && distance > best_dist) break;  
    if (nw == node_end) {
      best_dist = distance; reached_goal= true;
      continue;
    }
    for (int n = 0; n < node[nw].lid_neighbor_node.size(); n++) {
      nw_neig = node[nw].lid_neighbor_node[n];
      pw_neig = node[nw].lid_neighbor_poly[n];  
      if (pw_neig > 0) dist = list_node_pro[index[nw]].distance + poly[abs(pw_neig)].length;
      else          dist = list_node_pro[index[nw]].distance + poly[abs(pw_neig)].length;
      x1 = node[nw_neig].lon; y1 = node[nw_neig].lat;
      dx = cart.dlon * (x1 - x2); dy = cart.dlat * (y1 - y2); 
      d_eu = a_eu * sqrt(dx*dx + dy * dy);
      iv++; node_visited_pro.set(nw_neig, nw, pw_neig, dist); list_node_pro.push_back(node_visited_pro);
      node_visited.cnt = iv; node_visited.dd = dist + d_eu; heap_B.push(node_visited);
    }
  } 

  int n; n = node_end; pw_neig = list_node_pro[index[n]].neig_poly;
  while (pw_neig != 0) {
    bpath.push_front(pw_neig);
    n = list_node_pro[index[n]].neig_node;
    pw_neig = list_node_pro[index[n]].neig_poly;
  }

  for (auto &i:visited) index[i] = 0;
  visited.clear(); list_node_pro.clear();

  return bpath;
}