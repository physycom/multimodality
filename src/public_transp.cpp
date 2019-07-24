#include "common_lib.h"
#include "config.h"
#include "cartography.h"
#include "public_transp.h"

using namespace std;

extern Config config_;
extern Cartography cart;

BusStop::BusStop(int busstop_lid_, string name_, string location_, double lat_, double lon_, int code_area_) {
  this->busstop_lid = busstop_lid_;
  this->name = name_;
  this->location = location_;
  this->lat = lat_;
  this->lon = lon_;
}

void BusArc::Clear() {
  this->busarc_lid = 0;
  this->path.clear();
}

void PublicTransp::MakeBusStop() {
  // Read Bus stop
  ifstream file_bus_stop(config_.file_bus_stop.c_str());
  vector<string> tokens;
  string line;

  if (!file_bus_stop) {
    cout << "ERROR: unable to read input file " << config_.file_bus_stop << endl;
    exit(6);
  }
  getline(file_bus_stop, line); // skip header
  line.clear(); tokens.clear();
  while (getline(file_bus_stop, line))
  {
    physycom::split(tokens, line, string(";"), physycom::token_compress_off);
    BusStop bw(stoi(tokens[0]), tokens[1], tokens[2], stod(tokens[6]), stod(tokens[7]), stoi(tokens[8]));

    if (cart.NearestNode(bw.lat, bw.lon, bw.dist_node, bw.node_lid) && cart.NearestPoly(bw.lat, bw.lon, bw.dist_poly, bw.poly_lid))
      this->busstop.push_back(bw);

    line.clear(); tokens.clear();
  }
  file_bus_stop.close();

  this->busstop_size = int(this->busstop.size());
  cout << " **** Read Bus Stop: " << busstop_size << "   ****" << endl;

  //lid2pos bus stop
  int n = 0;
  for (auto &i : busstop) {
    this->busstop_proxy[i.busstop_lid] = n++;
  }
}

void PublicTransp::MakeBusArc() {

  // Read Bus Arc
  ifstream file_bus_arc(config_.file_bus_arc.c_str());
  vector<string> tokens;
  string line;

  if (!file_bus_arc) {
    cout << "ERROR: unable to read input file " << config_.file_bus_arc << endl;
    exit(6);
  }
  getline(file_bus_arc, line); // skip header
  line.clear(); tokens.clear();
  map<int, BusArc> baw_map;
  while (getline(file_bus_arc, line))
  {
    physycom::split(tokens, line, string(";"), physycom::token_compress_off);
    double lat = stod(tokens[4]);
    double lon = stod(tokens[5]);
    if (lat<config_.lat_max && lat > config_.lat_min && lon > config_.lon_min && lon < config_.lon_max && stoi(tokens[6])!=0){
    Point pw;
    baw_map[stoi(tokens[0])].busarc_lid = stoi(tokens[0]);
    pw.set(stod(tokens[5]), stod(tokens[4]));
    baw_map[stoi(tokens[0])].path.push_back(pw);
    }
    line.clear(); tokens.clear();
  }
  file_bus_arc.close();

  int n = 0;
  for (auto &i : baw_map) {
    this->busarc.push_back(i.second);
    this->busarc_proxy[i.second.busarc_lid] = n++;
  }

  this->busarc_size = int(busarc.size());
  cout << " **** Read Bus Arc:  " << busarc_size << "   ****" << endl;

  for (auto &a:busarc) {
    for (int i = 0; i < a.path.size() - 1; ++i) {
      list<int> bpath;
      bpath = cart.BestPath(a.path[i].lat, a.path[i].lon, a.path[i+1].lat, a.path[i+1].lon);
      a.poly_path.merge(bpath);
    }
    a.poly_path.unique();
  }
}


void PublicTransp::MakeBusLine() {

  // Read Bus line
  ifstream file_bus_line(config_.file_bus_line.c_str());
  vector<string> tokens;
  string line;

  if (!file_bus_line) {
    cout << "ERROR: unable to read input file " << config_.file_bus_line << endl;
    exit(6);
  }
  getline(file_bus_line, line); // skip header
  line.clear(); tokens.clear();

  map<string,map<string, map<int, BusLine>>> blw_map;
  while (getline(file_bus_line, line))
  {
    physycom::split(tokens, line, string(";"), physycom::token_compress_off);
    //cout << line << endl;
    string busline_lid = tokens[0];
    string oneway = tokens[1];
    int path_lid = stoi(tokens[2]);
    int arc_lid = stoi(tokens[4]);
    if (this->busarc_proxy.find(arc_lid) != this->busarc_proxy.end()) {
      blw_map[busline_lid][oneway][path_lid].busarc.push_back(this->busarc_proxy[arc_lid]);
      blw_map[busline_lid][oneway][path_lid].busstop.push_back(stoi(tokens[6]));
      blw_map[busline_lid][oneway][path_lid].busstop.push_back(stoi(tokens[7]));
    }
    line.clear(); tokens.clear();
  }
  file_bus_line.close();

  for (auto &i : blw_map) 
    for (auto &h :i.second)
      for (auto &j : h.second){
      BusLine blw;
      blw.busline_lid = i.first;
      blw.path = j.first;
      blw.oneway = h.first;
      j.second.busstop.erase(unique(j.second.busstop.begin(), j.second.busstop.end()), j.second.busstop.end());
      blw.busstop = j.second.busstop;
      this->busline.push_back(blw);
      }
  
  for (auto &l : busline) {
    for (auto &s : l.busstop) {
      busstop[busstop_proxy[s]].busline.push_back(l.busline_lid);
    }
  }


  this->busline_size = int(this->busline.size());
  cout << " **** Read Bus Line: " << busline_size << "   ****" << endl;

}



void PublicTransp::InitPublicTransp() {

  cout << "****************************************************"  << endl;
  MakeBusStop();
  MakeBusArc();
  MakeBusLine();


}