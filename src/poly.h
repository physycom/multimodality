#pragma once
#include "common_lib.h"
#include "global_info.h"

using namespace std;

//----------------------------------------------------------------------------  
struct Point {
  double lon, lat;
  void set(double lon_, double lat_);
};
//----------------------------------------------------------------------------  
//----------------------------------------------------------------------------  
struct Segment_pro {

  // a= affinity; s= distance from beginning of poly; d= distance from (lat lon) point
  Segment_pro();
  double s, d, a;
  void set(double s, double d, double a) { this->s = s; this->d = d; this->a = a; }
  void add(Segment_pro spro);
};
//----------------------------------------------------------------------------
struct Segment {
  int id_poly;
  double dx, dy, length, s0;
  Point a, b;
  void set(int id_poly, double s0, Point a, Point b);
  double distance(double x, double y);
  bool affinity(double lon, double lat, Segment_pro &spro);
};


//----------------------------------------------------------------------------  
struct Poly {
  string           name;
  double           length;
  int              nodo_F, nodo_T;
  int              lid;
  long long int    cid, cid_nodeF, cid_nodeT;
  vector <Point> points;
  Poly(void);
  void set(long long int cid_, int lid_, vector <Point> points_);
  void set(long long int cid_nodeF_, long long int cid_nodeT_);
  void invert();
  void clear(void);
  void measure_length(void);
};
//----------------------------------------------------------------------------  
