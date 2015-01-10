
#ifndef SUNTIME_INCLUDE
#define SUNTIME_INCLUDE

class SunTime{
  const static float lon_deg = 106.61;
  const static float lat_deg = 35.1107;
public:
  int riseset( float year, float month, float day, 
	       bool sunrise, float *stime);
};


#endif
