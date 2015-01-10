/* Upload with "Upload Using Programmer", Shift + Ctl + U
 * 
 * 
 */


/* SunTime class
 * has a single function:
 * riseset( float year, float month, float day, 
 *	       bool sunrise, float *stime);
 * 
 * Provide year, month, day on which you want a sunrise or sunset
 * calculation. Set sunrise to true to get sunrise, otherwise it will
 * provide the time in the position pointed to by stime.
 * 
 * Function returns 0 if it finished without error. If there is no
 * sunrise or sunset--arctic summer, for example, then the function
 * returns 1.
 */
#include "suntime.h"
#include <math.h>

int SunTime::riseset( float year, float month, float day, 
		      bool sunrise, float *stime){
  float N1, N2, N3, N, t, M, L, lngHour, RA, Lquadrant, 
    RAquadrant, sinDec, cosDec, cosH, H, T;
  const float zenith=90.;
  const float d2r = M_PI/180;
  const float r2d = 180/M_PI;

  // 1. first calculate the day of the year
  N1 = floor(275 * month / 9);
  N2 = floor((month + 9) / 12);
  N3 = (1 + floor((year - 4 * floor(year / 4) + 2) / 3));
  N = N1 - (N2 * N3) + day - 30;

  // 2. convert the longitude to hour value and calculate an approximate time
  lngHour = lon_deg / 15;
  
  // if rising time is desired:
  if( sunrise){
    t = N + ((6 - lngHour) / 24);
  }
  else{ // if setting time is desired:
    t = N + ((18 - lngHour) / 24);
  }
  
  // 3. calculate the Sun's mean anomaly
  M = (0.9856 * t) - 3.289;

  // 4. calculate the Sun's true longitude
  L = M + (1.916 * sin(M *d2r)) + (0.020 * sin(2 * M * d2r)) + 282.634;
  L = fmod( L, 360);

  // 5a. calculate the Sun's right ascension	
  RA = atan(0.91764 * tan(L * d2r))*r2d; //
  // adjust into the range [0,360) by adding/subtracting 360
  RA = fmod( RA, 360);

  // 5b. right ascension value needs to be in the same quadrant as L
  Lquadrant  = (floor( L/90)) * 90;
  RAquadrant = (floor(RA/90)) * 90;
  RA = RA + (Lquadrant - RAquadrant);

  // 5c. right ascension value needs to be converted into hours
  RA = RA / 15;

  // 6. calculate the Sun's declination
  sinDec = 0.39782 * sin(L * d2r);
  cosDec = cos(asin(sinDec*d2r)*r2d);

  // 7a. calculate the Sun's local hour angle	
  cosH = ((cos(zenith*d2r) - (sinDec * sin(lat_deg*d2r))) / 
	  (cosDec * cos(lat_deg*d2r)));

  // the sun never rises here (on the specified date)--first condition
  // the sun never sets here (on the specified date)--second condition
  if ( (cosH > 1) ||(cosH < -1 ) ) {
    return 1;
  }

  // 7b. finish calculating H and convert into hours
  if( sunrise){
    H = 360 - acos(cosH)*r2d;
  } else {// setting time is desired:
    H = acos(cosH)*r2d;
  }
  H /=  15;

  // 8. calculate local mean time of rising/setting
  T = H + RA - (0.06571 * t) - 6.622;
  while( T < 0) T+=24;
  T = fmod( T, 24.);

  /*9. adjust back to UTC, UT = T - lngHour
    10. convert UT value to local time zone of latitude/longitude
    localT = UT + localOffset
  */

  *stime = T;

  return 0;
}
