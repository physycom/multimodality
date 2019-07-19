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


void PublicTransp::InitPublicTransp() {

  cout << "****************************************************" << endl << endl;
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
    
    if(cart.NearestNode(bw.lat, bw.lon, bw.dist_node, bw.node_lid))
      this->busstop.push_back(bw);
    
    line.clear(); tokens.clear();
  }
  file_bus_stop.close();

  this->busstop_size = int(this->busstop.size());
  cout << " **** Read Bus Stop: " << busstop_size << "   ****" << endl;


}