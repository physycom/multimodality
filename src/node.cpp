#include "common_lib.h"
#include "node.h"
#include "config.h"

extern config config_;

//--------------------------------------------------------------------
//---------------------------------- Nodo ----------------------------
Node::Node(void) { this->lid = 0; }
//--------------------------------------------------------------------
Node::~Node(void)
{ 
  this->lid_neighbor_node.clear(); 
  this->lid_neighbor_poly.clear(); 
}
//--------------------------------------------------------------------
void Node::reset(void) 
{ 
  this->lid = 0; 
  this->lid_neighbor_node.clear();
  this->lid_neighbor_poly.clear();
}
//--------------------------------------------------------------------
void Node::set(long long int cid_, int lid_) {
  this->cid = cid_;
  this->lid = lid_;
}
//--------------------------------------------------------------------
void Node::add_link(int id_neig_node_, int id_neig_poly_) {
  this->lid_neighbor_node.push_back(id_neig_node_);
  this->lid_neighbor_poly.push_back(id_neig_poly_);
}
//--------------------------------------------------------------------
void Node::remove_link(void) {
  this->lid_neighbor_node.pop_back();
  this->lid_neighbor_poly.pop_back();
}
//--------------------------------------------------------------------
double Node::distance(double lon_, double lat_) {
  double dx = config_.dslon*(this->lon - lon_);
  double dy = config_.dslat*(this->lat - lat_);
  double ds2 = dx * dx + dy * dy;
  if (ds2 > 0) 
    return sqrt(ds2);
  else
    return 0.0;
}
//----------------------------------------------------------------------------  
bool CompNeighborNode(const NeighborNode& a, const NeighborNode& b) { return (a.distance < b.distance); }

//----------------------------------------------------------------------------  


