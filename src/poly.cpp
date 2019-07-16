#include "common_lib.h"

#include "global_info.h"
#include "polyline.h"
#include "config.h"

using namespace std;

extern config config_;

//----------------------------------------------------------------------------------------
//--------------------------------- Point ------------------------------------------------
void Point::set(double lon_, double lat_) {
  this->lon = lon_;
  this->lat = lat_;
}

//----------------------------------------------------------------------------------------
//--------------------------------- Segment_pro ------------------------------------------
Segment_pro::Segment_pro() {
  this->set(0.0, config_.map_resolution, 0.0);
}
//----------------------------------------------------------------------------------------
void Segment_pro::add(Segment_pro spro) {
  this->a += spro.a;
  if (this->d > spro.d) { this->d = spro.d; this->s = spro.s; }
}

//----------------------------------------------------------------------------------------
//--------------------------------- Segment ----------------------------------------------
void Segment::set(int id_poly, double s0, Point a, Point b) {
  this->id_poly = id_poly;
  this->s0 = s0;
  this->a = a;
  this->b = b;
  dx = config_.dslon*(b.lon - a.lon);   
  dy = config_.dslat*(b.lat - a.lat);
  length = sqrt(dx*dx + dy * dy);
  if (length > 0.01) { dx /= length; dy /= length; }
  else {
    cout << " id_poly= " << id_poly << " arc length= " << length << " meters " << endl;
    cout << fixed << setprecision(6);
    cout << a.lat << "   " << a.lon << endl;
    cout << b.lat << "   " << b.lon << endl;
    system("pause");
  }
}
//----------------------------------------------------------------------------------------
double Segment::distance(double lon, double lat) {  // in metri

  double dxa = config_.dslon*(lon - a.lon); double dya = config_.dslat*(lat - a.lat);
  double ds = dxa * dx + dya * dy; double distanza;

  if (ds < 0) distanza = sqrt(dxa*dxa + dya * dya);
  else if (ds > length) {
    double dxb = config_.dslon*(lon - b.lon); double dyb = config_.dslat*(lat - b.lat); distanza = sqrt(dxb*dxb + dyb * dyb);
  }
  else distanza = fabs(dxa*dy - dya * dx);

  return distanza;
}
//----------------------------------------------------------------------------------------
bool Segment::affinity(double lon, double lat, Segment_pro &spro) {
  // s = distance from nearest point from the biginning of poly
  // d = distance from nearest point of segment a( lat,lon)
  // a = affinity
  spro.set(s0, config_.map_resolution, 0.0);  // (s,d,a)
  double lon_c = (a.lon + b.lon) / 2; 
  double lat_c = (a.lat + b.lat) / 2;
  double xc = config_.dslon*(lon - lon_c);   
  double yc = config_.dslat*(lat - lat_c);

  double x, y;  // coordinates in the segment system
  y = abs(xc*dy - yc * dx);
  if (y > config_.map_resolution)
    return false;
  x = (xc*dx + yc * dy); 
  
  double l = 0.5*this->length; 

  if (x < -l) 
  { 
    sap.s = s0 + EPSILON;
    sap.d = sqrt(y*y + (x + l)*(x + l)); 
  }
  else if (x > l) 
  {
    sap.s = s0 + l + l - EPSILON;
    sap.d = sqrt(y*y + (x - l)*(x - l)); 
  }
  else 
  { 
    sap.s = s0 + l + x;
    sap.d = y;
  }

  if (sap.d > config_.map_resolution) 
    return false;
  x /= config_.l_gauss;
  y /= config_.l_gauss;
  l /= config_.l_gauss;
  sap.a = exp(-y * y)*(erf(l - x) + erf(l + x)) / 2;
  // the affinity of a point on a infinite length segment is 1

  return true;
}


//----------------------------------------------------------------------------------------
//--------------------------------- Poly -------------------------------------------------
Poly::Poly(void) {
  clear();
}
//----------------------------------------------------------------------------------------
void Poly::clear(void) {
  points.clear(); 
  name = "__";
}
//----------------------------------------------------------------------------------------
void Poly::set(long long int cid_, int lid_, vector <Point> points_) {
  this->lid = lid_;
  this->cid= cid_;
  this->points = points_;
}
//----------------------------------------------------------------------------------------
void Poly::set(long long int cid_nodeF_, long long int cid_nodeT_) {
  this->cid_nodeF = cid_nodeF_;
  this->cid_nodeT = cid_nodeT_;
}
//----------------------------------------------------------------------------------------
void Poly::measure_length(void) {
  double sum = 0.0;
  for (int k = 0; k < int(points.size())-1; ++k) {
    double dx = config_.dslon*(points[k + 1].lon - points[k].lon);
    double dy = config_.dslat*(points[k + 1].lat - points[k].lat);
    double ds = sqrt(dx*dx + dy * dy);
    sum += ds;
  }
  this->length = sum;
}
//----------------------------------------------------------------------------
void Poly::invert() {
  vector <Point> pw;
  for (vector<Point>::reverse_iterator rit = points.rbegin(); rit != points.rend(); ++rit) pw.push_back(*rit);
  points = pw;
}
//----------------------------------------------------------------------------




