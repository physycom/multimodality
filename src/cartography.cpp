#include "common_lib.h"
#include "cartography.h"
#include "config.h"

extern Config config_;
//----------------------------------------------------------------------------------------------
//--------------------------------- Cartography ------------------------------------------------
void Cartography::MakePoly() {
  // Read pnt file
  ifstream pnt_file(config_.file_pnt.c_str());
  string line;
  vector<string> tokens;

  if (!pnt_file) {
    cout << "ERROR: unable to read input file " << config_.file_pnt << endl;
    exit(5);
  }

  //initialize first empty poly 0
  Poly poly_w;
  vector<Point> point_vec_w;
  poly_w.set(0, 0, point_vec_w);
  this->poly.push_back(poly_w);

  while (getline(pnt_file, line)) {

    poly_w.clear();
    physycom::split(tokens, line, string(" \t"), physycom::token_compress_off);

    poly_w.cid = stoll(tokens[0]);
    poly_w.lid = stoi(tokens[1]);
    poly_w.n_points = stoi(tokens[2]);
    for (int i = 0; i < poly_w.n_points; ++i)
    {
      tokens.clear(); line.clear();
      Point pw;
      getline(pnt_file, line);
      physycom::split(tokens, line, string(" \t"), physycom::token_compress_off);
      pw.lat = stod(tokens[0]) * 1e-6;
      pw.lon = stod(tokens[1]) * 1e-6;
      poly_w.points.push_back(pw);
    }
    this->poly.push_back(poly_w);
    line.clear(); tokens.clear();
  }
  pnt_file.close();

  // Read pro file
  ifstream pro_file(config_.file_pro.c_str());

  if (!pro_file) {
    cout << "ERROR: unable to read input file " << config_.file_pro << endl;
    exit(6);
  }
  getline(pro_file, line); // skip header

  int n = 1;
  line.clear(); tokens.clear();
  while (getline(pro_file, line))
  {
    physycom::split(tokens, line, string(" \t"), physycom::token_compress_on);
    if (this->poly[n].cid == stoll(tokens[0]))
    {
      this->poly[n].cid_nodeF = stoll(tokens[1]);
      this->poly[n].cid_nodeT = stoll(tokens[2]);
      this->poly[n].name = tokens[10];
      poly[n].measure_length();
      ++n;
    }
    else
      cout << "indexing problem in poly[" << n << "], cid = " << stol(tokens[0]) << endl;
    line.clear(); tokens.clear();
  }
  pro_file.close();

  this->poly_size = int(this->poly.size());
  cout << " **** Read Poly: " << this->poly_size << "      ****" << endl;
}

//----------------------------------------------------------------------------------------------
void Cartography::MakeNode() {

  for (int p=1; p < poly.size(); ++p)
  {
    poly[p].lid = p;
    this->node_cid2lid[poly[p].cid_nodeF] = 0;
    this->node_cid2lid[poly[p].cid_nodeT] = 0;
  }

  int index = 1; 
  for (auto &i : node_cid2lid) i.second = index++;
  node_size = int(node_cid2lid.size()+1); //perchè c'era + 1?


  node.resize(node_size);

  
  for (int p = 1; p < poly.size(); ++p)
  {
    int lid_nodeF = node_cid2lid[poly[p].cid_nodeF];
    int lid_nodeT = node_cid2lid[poly[p].cid_nodeT];
    //cout << "analizzo la " << p.lid << " con: " << p.cid_nodeF << " " << p.cid_nodeT << endl;
    //cout << "analizzo la " << p.lid <<" con: "<<lid_nodeF << " " << lid_nodeT << endl;

    node[lid_nodeF].add_link(lid_nodeT, poly[p].lid);
    node[lid_nodeF].lid = lid_nodeF;
    node[lid_nodeF].cid = poly[p].cid_nodeF;
    node[lid_nodeF].lat = poly[p].points.front().lat;
    node[lid_nodeF].lon = poly[p].points.front().lon;

    node[lid_nodeT].add_link(lid_nodeF, -poly[p].lid);
    node[lid_nodeT].lid = lid_nodeT;
    node[lid_nodeT].cid = poly[p].cid_nodeT;
    node[lid_nodeT].lat = poly[p].points.back().lat;
    node[lid_nodeT].lon = poly[p].points.back().lon;

  }
  cout << " **** Read Node: " << this->node_size << "      ****" << endl;


  for (int i = 1; i < poly_size; i++) {
    poly[i].node_F = node_cid2lid[poly[i].cid_nodeF];
    poly[i].node_T = node_cid2lid[poly[i].cid_nodeT];
  }

  //  find polys with same pair of nodes
  /*
  vector<int> to_delate;
  for (auto &n : node) 
    for (int i = 0; i < n.lid_neighbor_node.size() - 1; ++i)
      for (int j = i + 1; j < n.lid_neighbor_node.size(); ++j)
      {
        if (n.lid_neighbor_node[i] == n.lid_neighbor_node[j]) 
        {
          if (poly[abs(n.lid_neighbor_poly[i]) - 1].length > poly[abs(n.lid_neighbor_poly[j]) - 1].length) to_delate.push_back(abs(n.lid_neighbor_poly[i]));
          else to_delate.push_back(abs(n.lid_neighbor_poly[j]));
        }
      }
  sort(to_delate.begin(), to_delate.end());
  to_delate.erase(unique(to_delate.begin(), to_delate.end()), to_delate.end());
  // If this is different from zero, cityedit doesn't work.
  cout << " **** Delated Poly: " << to_delate.size() << " ****" << endl;
  */
  
}
//----------------------------------------------------------------------------------------------
void Cartography::MakeSegment() {
  Segment sw; 
  segment.clear(); 
  double s0;
  for (int p = 1; p < poly.size(); ++p) {
    s0 = 0.0;
    for (int k = 0; k < poly[p].points.size() - 1; ++k) {
      sw.set(poly[p].lid, s0, poly[p].points[k], poly[p].points[k + 1]);
      segment.push_back(sw);
      s0 += sw.length;
    }
  }
  this->segment_size = int(segment.size());
  cout << " **** Read Segment: " << this->segment_size << "   ****" << endl;
}

//----------------------------------------------------------------------------------------------
void Cartography::MakeMap() {
  
  this->map_dslat = config_.map_resolution / config_.dslat;
  this->map_dslon = config_.map_resolution / config_.dslon;
  
  double c_ris1 = 1.72*config_.map_resolution; 
  double c_ris2 = c_ris1 * c_ris1;

  int index_jmax = int(1 + (config_.lat_max - config_.lat_min) / map_dslat);
  int index_imax = int(1 + (config_.lon_max - config_.lon_min) / map_dslon);

  cout << " **** "<<config_.city_tag <<" Map [" << index_imax << ","<< index_jmax << "] ****" << endl;
  cout << " **** LX[km]= " << (config_.lat_max - config_.lat_min)*config_.dslat / 1000 << "       ****" << endl;
  cout << " **** LY[km]= " << (config_.lon_max - config_.lon_min)*config_.dslon / 1000 << "       ****" << endl;
  
  // mapping of segment
  int n_mapseg = 0;
  for (auto &is : segment) {
    int tw = 0;
    int ia = int((is.a.lon - config_.lon_min) / map_dslat);
    int ja = int((is.a.lat - config_.lat_min) / map_dslat);
    int ib = int((is.b.lon - config_.lon_min) / map_dslon);
    int jb = int((is.b.lat - config_.lat_min) / map_dslon);
    if (ib < ia) { tw = ia; ia = ib; ib = tw; }         
    if (jb < ja) { tw = ja; ja = jb; jb = tw; }
    int i0 = (ia - 1 > 0 ? ia - 1 : 0);  int i1 = (ib + 2 < index_imax ? ib + 2 : index_imax);
    int j0 = (ja - 1 > 0 ? ja - 1 : 0);  int j1 = (jb + 2 < index_jmax ? jb + 2 : index_jmax);

    for (int j = j0; j < j1; j++) {
      double lat_c = config_.lat_min + (j + 0.5)*map_dslat;
      for (int i = i0; i < i1; i++) {
        double lon_c = config_.lon_min + (i + 0.5)*map_dslon;
        if (is.distance(lon_c, lat_c) < c_ris1) {
          this->cartomap[j][i].segment_lid.push_back(n_mapseg);
        }
      }
    }
    n_mapseg++;
  }
  // mapping of node
  int n_mapnode = 0;
  for (auto &in : node) {
    
    int ia = int((in.lon - config_.lon_min) / map_dslon);
    int ja = int((in.lat - config_.lat_min) / map_dslat);
    int i0 = (ia - 1 > 0 ? ia - 1 : 0);  int i1 = (ia + 2 < index_imax ? ia + 2 : index_imax);
    int j0 = (ja - 1 > 0 ? ja - 1 : 0);  int j1 = (ja + 2 < index_jmax ? ja + 2 : index_jmax);

    for (int j = j0; j < j1; j++) {
      double lat_c = config_.lat_min + (j + 0.5)*map_dslat;
      double dya = config_.dslat*(in.lat - lat_c);
      for (int i = i0; i < i1; i++) {
        double lon_c = config_.lon_min + (i + 0.5)*map_dslon;
        double dxa = config_.dslon*(in.lon - lon_c);
        if (dxa*dxa + dya * dya < c_ris2){
          cartomap[j][i].node_lid.push_back(in.lid);
        }
      }
    }
    n_mapnode++;
  }
}
//----------------------------------------------------------------------------------------------
void Cartography::InitCart() {

  this->dlat = 0.5*(config_.lat_max - config_.lat_min) / ZOOM_START;
  this->lat0 = 0.5*(config_.lat_min + config_.lat_max);
  this->dlon = 0.5*(config_.lon_max - config_.lon_min) / ZOOM_START;
  this->lon0 = 0.5*(config_.lon_min + config_.lon_max);

  if (config_.dslat*this->dlat > config_.dslon*this->dlon)
    this->dlon = (config_.dslat / config_.dslon)*this->dlat*double(SCREEN_WIDTH) / SCREEN_HEIGHT;
  else
    this->dlat = (config_.dslon / config_.dslat)*this->dlon*double(SCREEN_HEIGHT) / SCREEN_WIDTH;

  MakePoly();
  MakeNode();
  MakeSegment();
  MakeMap();
}

//----------------------------------------------------------------------------------------------
bool Cartography::NearestNode(double lat, double lon, double &distance, int &node_lid) {

  distance = 1.0e8; 
  node_lid = 0;
  
  list <NeighborNode> neighbornode;
  int i = int((lon - config_.lon_min) / this->map_dslon); 
  int j = int((lat - config_.lat_min) / this->map_dslat);
  int n_neighbors= int(cartomap[j][i].node_lid.size());
  if (n_neighbors == 0) return false;
  for (auto &n : cartomap[j][i].node_lid) {
    double node_dist = node[n-1].distance(lon, lat);
    if (distance > node_dist) { node_lid = n; distance = node_dist; }
  }
  if (node_lid == 0) 
    return false; 
  else 
    return true;
}
//----------------------------------------------------------------------------------------------
bool Cartography::NearestPoly(double lat, double lon, double &distance, int &poly_lid) {
  distance = 1.0e10;
  poly_lid = 0;
  int i = int((lon - config_.lon_min) / this->map_dslon); 
  int j = int((lat - config_.lat_min) / this->map_dslat);
 
  int n_neighbors = int(cartomap[j][i].segment_lid.size());
  if (n_neighbors == 0) return false;

  for (auto &s : cartomap[j][i].segment_lid) {
    double seg_dist = segment[s].distance(lon, lat);
    if (seg_dist < distance) { 
      distance = seg_dist; 
      poly_lid = segment[s].poly_lid;
    }
  }
  if (poly_lid == 0)
    return false;
  else
    return true;
  
}