#include "common_lib.h"

#include <FL/Fl.H>

#include "config.h"
#include "cartography.h"
#include "public_transp.h"

using namespace std;
using namespace jsoncons;

void usage(const char* progname)
{
  string pn(progname);
  cerr << "Usage: " << pn.substr(pn.find_last_of("/\\") + 1) << " path/to/json/config" << endl;
  cerr << R"(JSON CONFIG SAMPLE
{
  "city_tag"        : "Bologna",
  ///// cartography setup
  "file_pro"        : "../input/CART/Bologna.pro",                  // poly properties input file
  "file_pnt"        : "../input/CART/Bologna.pnt",                  // poly geometry input file
  "file_data"       : [ "../../bolobm/input/DATI/originali/04a.csv"
                       ,"../../bolobm/input/DATI/originali/04b.csv"      
                      ], // name of data file
  
  ///// time restrictions
  "start_time"      : "2017-04-01 00:00:00",        // date-time analysis start   
  "end_time"        : "2017-04-30 00:00:00",        // date-time analysis end  
  
  ///// space restricions
  "lat_min" 		: 44.4521634,
  "lat_max" 		: 44.5339614,
  "lon_min" 		: 11.2448502,
  "lon_max" 		: 11.4213181,

  ///// distances and resolutions
  "map_resolution"    : "60.0",                       // meters for map resolution
  "grid_resolution"   : "150.0",                      // meters for grid resolution
  "l_gauss"           : "10.0",                       // meters for GPS data precision
  "min_data_distance" : "49.0",                       // minimum distance between two consecutive data
  "min_node_distance" : "20.0",                       // minimum distance datum-node
  "min_poly_distance" : "50.0",                       // minimum distance datum-poly
}
)";
}

Config config_;
Cartography cart;

int main(int argc, char **argv){

  string conf;
  if (argc == 2)
  {
    conf = argv[1];
  }
  else
  {
    usage(argv[0]);
    exit(1);
  }

  json jconf;

  try
  {
    jconf = json::parse_file(conf);
    config_.set_config(jconf);

    cart.InitCart();

    PublicTransp pub_transp;
    pub_transp.InitPublicTransp();

  }
  catch (exception &e)
  {
    cerr << "EXC: " << e.what() << endl;
    exit(1);
  }
}