#include "common_lib.h"
#include "cartography.h"
#include "config.h"

extern config config_;
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
  //poly_w.set(0, 0, point_vec_w);
  //this->poly.push_back(poly_w);

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

  int n = 0;
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
  cout << " **** Read Poly: " << this->poly_size << " ****" << endl;
}

//----------------------------------------------------------------------------------------------
void Cartography::MakeNode() {
  int count = 1;
  for (auto &p : poly)
  {
    p.lid = count++;
    this->node_cid2lid[p.cid_nodeF] = 0;
    this->node_cid2lid[p.cid_nodeT] = 0;
  }


  int index = 1; 
  for (auto &i : node_cid2lid) i.second = index++;
  node_size = int(node_cid2lid.size()); //perchè c'era + 1?


  node.resize(node_size+1);

  
  for (auto &p : poly)
  {
    int lid_nodeF = node_cid2lid[p.cid_nodeF];
    int lid_nodeT = node_cid2lid[p.cid_nodeT];
    //cout << "analizzo la " << p.lid << " con: " << p.cid_nodeF << " " << p.cid_nodeT << endl;
    //cout << "analizzo la " << p.lid <<" con: "<<lid_nodeF << " " << lid_nodeT << endl;

    node[lid_nodeF-1].add_link(lid_nodeT, p.lid);
    node[lid_nodeF-1].lid = lid_nodeF;
    node[lid_nodeF-1].cid = p.cid_nodeF;
    node[lid_nodeF-1].lat = p.points.front().lat;
    node[lid_nodeF-1].lon = p.points.front().lon;

    node[lid_nodeT-1].add_link(lid_nodeF, -p.lid);
    node[lid_nodeT-1].lid = lid_nodeT;
    node[lid_nodeT-1].cid = p.cid_nodeT;
    node[lid_nodeT-1].lat = p.points.back().lat;
    node[lid_nodeT-1].lon = p.points.back().lon;

  }

  cout << " **** Read Node: " << this->node_size << " ****" << endl;


  for (int i = 0; i < poly_size; i++) {
    poly[i].node_F = node_cid2lid[poly[i].cid_nodeF];
    poly[i].node_T = node_cid2lid[poly[i].cid_nodeT];
  }

  // find polys with same pair of nodes
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
}

