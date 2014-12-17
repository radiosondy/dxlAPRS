/*
 * dxlAPRS toolchain
 *
 * Copyright (C) Christian Rabler <oe5dxl@oevsv.at>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
/* "@(#)gpspos.c Jun  7 15:48:17 2014" */


#define X2C_int32
#define X2C_index32
#ifndef gpspos_H_
#include "gpspos.h"
#endif
#define gpspos_C_
#ifndef osi_H_
#include "osi.h"
#endif
#include <math.h>
#ifndef InOut_H_
#include "InOut.h"
#endif
#include "sem.h"
#include "yuma.h"
#include "rinex.h"
#include "gps.h"
#include "navigation.h"
#include "geodesy.h"
#ifndef TimeConv_H_
#include "TimeConv.h"
#endif



/* decode weather sonde gps data to position by oe5dxl */
/*FROM Lib IMPORT NextArg; */
/*SEM_structAlmanac,*/
/*YUMA_structAlmanac,*/
/*GPS_structEphemeris,*/
/*FROM gnss_types IMPORT GNSS_structKlobuchar; */
/*FROM ionosphere IMPORT IONOSPHERE_GetL1KlobucharCorrection; */
/*FROM time_conversion IMPORT TIMECONV_GetGPSTimeFromRinexTime; */
/*FROM troposphere IMPORT TROPOSPHERE_DetermineZenithDelayValues_WAAS_Model,
                */
/*                        TROPOSPHERE_GetDryAndWetDelay_UsingThe_UNBabc_MappingFunction;
                 */
#define gpspos_LIGHT 2.99792458E+8

#define gpspos_FREQ 1.57542E+9

#define gpspos_PI 3.1415926535898

#define gpspos_RAD 1.7453292519943E-2

#define gpspos_PRM 2.8618384385692E-1

#define gpspos_TESTDIL 50.0
/* shift sat to test influence to position */

#define gpspos_WAVLEN 1.9029367279836E-1

#define gpspos_MAXSPEED 500

#define gpspos_SECONDSINWEEK 604800

struct SATPOS;


struct SATPOS {
   char ok0;
   double x;
   double y;
   double z;
   double clk;
   double azimuth0;
   double elevation;
   double ionocorr;
   double range;
   double doppler;
   double hdil;
   double vdil;
};

typedef struct SATPOS SATPOSES[32];

struct RESULTS;


struct RESULTS {
   double lat;
   double long0;
   double heig;
   double hd[4]; /* h and v dilatation per sat */
   double vd[4];
   double dlat[4];
   double dlong[4];
   double dheig[4];
   double qsumv;
   double qsumh;
   unsigned short satset;
   unsigned short res;
};

struct COMMONALMANACH;


struct COMMONALMANACH {
   unsigned long toa; /* almanac time of applicability (reference time [s]*/
   unsigned short week; /* 10 bit gps week 0-1023 (user must account for week rollover) [week]*/
   unsigned short prn; /* GPS prn number */
   unsigned short reserved; /* reserved */
   unsigned short svn; /* Satellite vehicle number */
   unsigned char ura; /* User Range Accuracy lookup code, [0-15], see p. 83 GPSICD200C, 0 is*/
   unsigned char health; /* 0=healthy, unhealthy otherwise  [], subframe 4 and 5, page 25 six-b*/
   unsigned char config_code; /* configuration code   [], if >=9 Anti-Spoofing is on */
   /* this inicator is not part of the SEM standard but is added by the user if known */
   char is_af0_af1_high_precision;
                /* indicates precision of af0 and af1 [1=high precision,
                0=lo*/
   double ecc; /* eccentricity */
   double i0; /* orbital inclination at reference time                        [rad] */
   double omegadot; /* rate of right ascension                                      [rad/s] */
   double sqrta; /* square root of the semi-major axis                           [m^(1/2)]*/
   double omega0; /* longitude of ascending node of orbit plane at weekly epoch   [rad] */
   double w; /* argument of perigee                                          [rad] */
   double m0; /* mean anomaly at reference time                               [rad] */
   double af0; /* polynomial clock correction coefficient (clock bias)         [s],   Note: pa*/
   double af1;
   double af2; /* polynomial clock correction coefficient (clock drift)        [s/s], Note: pa*/
   unsigned long toe; /* reference time ephemeris (0-604800) */
   unsigned long toc; /* reference time (clock)   (0-604800) */
   unsigned short iodc; /* 10 bit issue of data (clock) */
   char iode; /* 8 bit  issue of data (ephemeris) */
   char alert_flag; /* 1 = URA may be worse than indicated */
   char anti_spoof; /* anti-spoof flag from 0=off, 1=on */
   char code_on_L2; /* 0=reserved, 1=P code on L2, 2=C/A on L2 */
   char L2_P_data_flag; /* flag indicating if P is on L2 1=true */
   char fit_interval_flag; /* fit interval flag (four hour interval or longer) 0=4 fours */
   unsigned short age_of_data_offset; /* age of data offset */
   unsigned short tow_week; /* The week corresponding to tow (0-1024+). Can be one week l */
   unsigned long tow; /* The time of week derived formt the Z-count in the Hand Ove */
   /* clock parameters */
   double tgd; /* group delay */
   /* ephemeris parameters */
   double delta_n; /* mean motion difference from computed value */
   double idot; /* rate of inclination angle */
   double cuc; /* amplitude of the cosine harmonic correction term to the argument o */
   double cus; /* amplitude of the sine harmonic correction term to the argument of */
   double crc; /* amplitude of the cosine harmonic correction term to the orbit radi */
   double crs; /* amplitude of the sine harmonic correction term to the orbit radius */
   double cic; /* amplitude of the cosine harmonic correction term to the angle of i  */
   double cis; /* amplitude of the sine harmonic correction term to the angle of inc */
};

struct structEphemeris;


struct structEphemeris {
   unsigned long toe; /* reference time ephemeris (0-604800) */
   unsigned long toc; /* reference time (clock)   (0-604800) */
   unsigned short prn; /* GPS PRN number */
   unsigned short week; /* 10 bit gps week 0-1023 (user must account for week rollove */
   unsigned short iodc; /* 10 bit issue of data (clock) */
   unsigned short reserved1; /* reserved bytes */
   char iode; /* 8 bit  issue of data (ephemeris) */
   char health; /* 6 bit health parameter, 0 if healthy, unhealth othersize */
   char alert_flag; /* 1 = URA may be worse than indicated */
   char anti_spoof; /* anti-spoof flag from 0=off, 1=on */
   char code_on_L2; /* 0=reserved, 1=P code on L2, 2=C/A on L2 */
   char L2_P_data_flag; /* flag indicating if P is on L2 1=true */
   char fit_interval_flag; /* fit interval flag (four hour interval or longer) 0=4 fours */
   char ura; /* User Range Accuracy lookup code, 0 is excellent, 15 is use */
   unsigned short age_of_data_offset; /* age of data offset */
   unsigned short tow_week; /* The week corresponding to tow (0-1024+). Can be one week l */
   unsigned long tow; /* The time of week derived formt the Z-count in the Hand Ove */
   /* clock parameters */
   double tgd; /* group delay */
   double af2; /* polynomial clock correction coefficient (rate of clock drift) */
   double af1; /* polynomial clock correction coefficient (clock drift) */
   double af0; /* polynomial clock correction coefficient (clock bias) */
   /* ephemeris parameters */
   double m0; /* mean anomaly at reference time */
   double delta_n; /* mean motion difference from computed value */
   double ecc; /* eccentricity */
   double sqrta; /* square root of the semi-major axis */
   double omega0; /* longitude of ascending node of orbit plane at weekly epoch */
   double i0; /* inclination angle at reference time */
   double w; /* argument of perigee */
   double omegadot; /* rate of right ascension */
   double idot; /* rate of inclination angle */
   double cuc; /* amplitude of the cosine harmonic correction term to the argument o */
   double cus; /* amplitude of the sine harmonic correction term to the argument of */
   double crc; /* amplitude of the cosine harmonic correction term to the orbit radi */
   double crs; /* amplitude of the sine harmonic correction term to the orbit radius */
   double cic; /* amplitude of the cosine harmonic correction term to the angle of i  */
   double cis; /* amplitude of the sine harmonic correction term to the angle of inc */
};

struct structKlobuchar;


struct structKlobuchar {
   unsigned short isValid; /* Is this structure valid for use 1=YES, 0=NO.*/
   unsigned short week; /* The GPS week corresponding to the correction parameters [weeks]. */
   unsigned long tow; /* The GPS time of week corresponding to the correction parameters [s]. */
   double alpha0; /* coefficients of a cubic equation representing the amplitude of the ve */
   double alpha1; /* coefficients of a cubic equation representing the amplitude of the ve */
   double alpha2; /* coefficients of a cubic equation representing the amplitude of the ve */
   double alpha3; /* coefficients of a cubic equation representing the amplitude of the ve */
   double beta0; /* coefficients of a cubic equation representing the period of the model */
   double beta1; /* coefficients of a cubic equation representing the period of the model */
   double beta2; /* coefficients of a cubic equation representing the period of the model */
   double beta3; /* coefficients of a cubic equation representing the period of the model */
};

static struct structKlobuchar rinexklobuchar;

static char semok;

static char yumaok;

static char rinexok;

static struct RESULTS stats[500];

static struct COMMONALMANACH calm[32];


static double sqr(double x)
{
   return x*x;
} /* end sqr() */

#define gpspos_Z 48


static void degtostr(float d, char lat, char form, char s[],
                unsigned long s_len)
{
   unsigned long i;
   unsigned long n;
   if (s_len-1<11UL) {
      s[0UL] = 0;
      return;
   }
   if (form=='2') i = 7UL;
   else if (form=='3') i = 8UL;
   else i = 9UL;
   if (d<0.0f) {
      d = -d;
      if (lat) s[i] = 'S';
      else s[i+1UL] = 'W';
   }
   else if (lat) s[i] = 'N';
   else s[i+1UL] = 'E';
   if (form=='2') {
      /* DDMM.MMNDDMM.MME */
      n = (unsigned long)X2C_TRUNCC(d*3.4377467707849E+5f+0.5f,0UL,
                X2C_max_longcard);
      s[0UL] = (char)((n/600000UL)%10UL+48UL);
      i = (unsigned long)!lat;
      s[i] = (char)((n/60000UL)%10UL+48UL);
      ++i;
      s[i] = (char)((n/6000UL)%10UL+48UL);
      ++i;
      s[i] = (char)((n/1000UL)%6UL+48UL);
      ++i;
      s[i] = (char)((n/100UL)%10UL+48UL);
      ++i;
      s[i] = '.';
      ++i;
      s[i] = (char)((n/10UL)%10UL+48UL);
      ++i;
      s[i] = (char)(n%10UL+48UL);
      ++i;
   }
   else if (form=='3') {
      /* DDMM.MMMNDDMM.MMME */
      n = (unsigned long)X2C_TRUNCC(d*3.4377467707849E+6f+0.5f,0UL,
                X2C_max_longcard);
      s[0UL] = (char)((n/6000000UL)%10UL+48UL);
      i = (unsigned long)!lat;
      s[i] = (char)((n/600000UL)%10UL+48UL);
      ++i;
      s[i] = (char)((n/60000UL)%10UL+48UL);
      ++i;
      s[i] = (char)((n/10000UL)%6UL+48UL);
      ++i;
      s[i] = (char)((n/1000UL)%10UL+48UL);
      ++i;
      s[i] = '.';
      ++i;
      s[i] = (char)((n/100UL)%10UL+48UL);
      ++i;
      s[i] = (char)((n/10UL)%10UL+48UL);
      ++i;
      s[i] = (char)(n%10UL+48UL);
      ++i;
   }
   else {
      /* DDMMSS */
      n = (unsigned long)X2C_TRUNCC(d*2.062648062471E+5f+0.5f,0UL,
                X2C_max_longcard);
      s[0UL] = (char)((n/360000UL)%10UL+48UL);
      i = (unsigned long)!lat;
      s[i] = (char)((n/36000UL)%10UL+48UL);
      ++i;
      s[i] = (char)((n/3600UL)%10UL+48UL);
      ++i;
      s[i] = 'd';
      ++i;
      s[i] = (char)((n/600UL)%6UL+48UL);
      ++i;
      s[i] = (char)((n/60UL)%10UL+48UL);
      ++i;
      s[i] = '\'';
      ++i;
      s[i] = (char)((n/10UL)%6UL+48UL);
      ++i;
      s[i] = (char)(n%10UL+48UL);
      ++i;
      s[i] = '\"';
      ++i;
   }
   ++i;
   s[i] = 0;
} /* end degtostr() */

#define gpspos_A 6378137
/*      B=6356752; */

/*      F=(A-B)/A; */
/*      E=2*F-F*F; */
#define gpspos_E 8.1819190842522E-2


static void wgs84(double lat, double long0, double heig, double * x,
                double * y, double * z)
/* wgs84 ecef */
{
   double h;
   double sl;
   double n;
   sl = sin(lat);
   n = X2C_DIVL(6.378137E+6,sqrt(1.0-6.694379990125E-3*sl*sl));
   h = heig+n;
   *z = (n*9.9330562000988E-1+heig)*sl;
   *y = h*sin(long0)*cos(lat);
   *x = h*cos(long0)*cos(lat);
} /* end wgs84() */

#define gpspos_K 0.0


static long get4sats(const SATPOSES sats, const unsigned long satnum[],
                unsigned long satnum_len, unsigned long dil, double dilm,
                double * lat, double * long0, double * heig)
{
   unsigned long i;
   double rx_clock_bias; /*, clock_drift, satVx,satVy,satVz,azimuth,elevation,doppler*/
   long ret;
   double dils[4];
   struct X2C_XHandler_STR anonym;
   long get4sats_ret;
   /*WrStrLn(" satnums"); */
   /* ;WrInt(satnum[i], 3);*/
   /*+K*sats[satnum[0]].drydelay*/
   /*+K*sats[satnum[1]].drydelay*/
   /*+K*sats[satnum[2]].drydelay*/
   /*+K*sats[satnum[3]].drydelay*/
   /*WrFixed(rx_clock_bias, 2,12); WrStrLn("=bias"); */
   if (X2C_XTRY(&anonym)) {
      for (i = 0UL; i<=3UL; i++) {
         dils[i] = 0.0;
      } /* end for */
      if (dil>0UL) dils[dil-1UL] = dilm;
      ret = NAVIGATION_PerformClosedFormPositionSolution_FromPseuodrangeMeasurements(sats[satnum[0UL]
                ].range+dils[0U], sats[satnum[1UL]].range+dils[1U],
                sats[satnum[2UL]].range+dils[2U],
                sats[satnum[3UL]].range+dils[3U], sats[satnum[0UL]].clk,
                sats[satnum[1UL]].clk, sats[satnum[2UL]].clk,
                sats[satnum[3UL]].clk, sats[satnum[0UL]].x,
                sats[satnum[1UL]].x, sats[satnum[2UL]].x,
                sats[satnum[3UL]].x, sats[satnum[0UL]].y,
                sats[satnum[1UL]].y, sats[satnum[2UL]].y,
                sats[satnum[3UL]].y, sats[satnum[0UL]].z,
                sats[satnum[1UL]].z, sats[satnum[2UL]].z,
                sats[satnum[3UL]].z, lat, long0, heig, &rx_clock_bias);
      if (ret>500L) ret = 500L;
      if ((long)X2C_TRUNCI(*heig,X2C_min_longint,X2C_max_longint)<-10000L) {
         ret = 0L;
      }
      get4sats_ret = ret;
      X2C_XOFF();
   }
   else get4sats_ret = -1L;
   X2C_XREMOVE();
   return get4sats_ret;
} /* end get4sats() */


static double qdist(double dlat, double dlong)
/* horizontal distance square rad*/
{
   double c;
   c = cos(dlat)*dlong;
   return dlat*dlat+c*c;
} /* end qdist() */


static void satposit(struct SATPOS * sat, unsigned long satnum, double myx,
                double myy, double myz, unsigned long userweek,
                unsigned long weekms)
{
   double satVz;
   double satVy;
   double satVx;
   double clock_drift;
   struct COMMONALMANACH * anonym;
   sat->ok0 = 0;
   if (calm[satnum].prn>0U) {
      { /* with */
         struct COMMONALMANACH * anonym = &calm[satnum];
         if (rinexok) {
            GPS_ComputeSatellitePositionVelocityAzimuthElevationDoppler_BasedOnEphmerisData(myx,
                 myy, myz, (unsigned short)userweek, (double)weekms*0.001,
                anonym->tow_week&1023U, anonym->toe, anonym->toc,
                anonym->af0, anonym->af1, anonym->af2, anonym->tgd,
                anonym->m0, anonym->delta_n, anonym->ecc, anonym->sqrta,
                anonym->omega0, anonym->i0, anonym->w, anonym->omegadot,
                anonym->idot, anonym->cuc, anonym->cus, anonym->crc,
                anonym->crs, anonym->cic, anonym->cis, &sat->clk,
                &clock_drift, &sat->x, &sat->y, &sat->z, &satVx, &satVy,
                &satVz, &sat->azimuth0, &sat->elevation, &sat->doppler);
         }
         else {
            /*WrInt(ORD(fit_interval_flag), 4); WrInt(tow, 10);
                WrInt(weekms DIV 1000, 10); WrStrLn("=fit tow tsat"); */
            GPS_ComputeSatellitePositionVelocityAzimuthElevationDoppler_BasedOnAlmanacData(myx,
                 myy, myz, (unsigned short)userweek, (double)weekms*0.001,
                (double)(float)anonym->toa, anonym->week, anonym->prn,
                anonym->ecc, anonym->i0, anonym->omegadot, anonym->sqrta,
                anonym->omega0, anonym->w, anonym->m0, anonym->af0,
                anonym->af1, &sat->clk, &clock_drift, &sat->x, &sat->y,
                &sat->z, &satVx, &satVy, &satVz, &sat->azimuth0,
                &sat->elevation, &sat->doppler);
         }
      }
      /*WrInt(toe, 9); WrInt((time()-7200+3600*24*3) MOD 604800, 9);
                WrInt(weekms DIV 1000, 9); WrStrLn("=toe"); */
      /*
      WrInt(prn, 2);
      WrFixed(ecc, 5, 9);
      WrFixed(m0, 5, 9);
      WrFixed(sqrta, 5, 9);
      WrFixed(omega0, 5, 9);
      WrFixed(i0, 5, 9);
      WrFixed(w, 5, 9);
      WrFixed(omegadot, 5, 9);
      
      --WrInt(toe, 5);
      --WrInt(toc, 5);
      WrInt(tow_week, 5);
      WrFixed(sat.azimuth/RAD, 1, 7);
      WrFixed(sat.elevation/RAD, 1, 6);
      WrStrLn("=p a e"); 
      */
      sat->ok0 = 1;
   }
/*WrInt(alm[satnum].prn, 3); WrFixed(clock_drift, 7, 10);
                WrStrLn("=clock_drift"); */
} /* end satposit() */


static double lowele(double e)
{
   if (e<0.01) e = 0.01;
   return X2C_DIVL(1.0,sin(e));
} /* end lowele() */


static char stat4sats(SATPOSES sats, const unsigned long satnums[],
                unsigned long satnums_len, unsigned long try0)
{
   unsigned long i;
   double azi;
   double ele;
   double dil;
   double bz;
   double by;
   double bx;
   struct RESULTS * anonym;
   { /* with */
      struct RESULTS * anonym = &stats[try0];
      if (get4sats(sats, satnums, satnums_len, 0UL, 0.0, &anonym->lat,
                &anonym->long0, &anonym->heig)>0L) {
         wgs84(anonym->lat, anonym->long0, anonym->heig, &bx, &by, &bz);
                /* for elevation */
         for (i = 0UL; i<=3UL; i++) {
            dil = 50.0;
            if (GEODESY_ComputeAzimuthAndElevationAnglesBetweenToPointsInTheEarthFixedFrame(GEODESY_REFERENCE_ELLIPSE_WGS84,
                 bx, by, bz, sats[satnums[i]].x, sats[satnums[i]].y,
                sats[satnums[i]].z, &ele, &azi)) {
               dil = 50.0+lowele(ele)*5.0;
                /* make bad hdop on low elevation */
            }
            if (get4sats(sats, satnums, satnums_len, i+1UL, dil,
                &anonym->dlat[i], &anonym->dlong[i], &anonym->dheig[i])>0L) {
               anonym->hd[i] = sqrt(qdist(anonym->lat-anonym->dlat[i],
                anonym->long0-anonym->dlong[i]))*6.3661977236758E+6;
               anonym->vd[i] = fabs(anonym->heig-anonym->dheig[i]);
               sats[satnums[i]].hdil = sats[satnums[i]].hdil+anonym->hd[i];
               sats[satnums[i]].vdil = sats[satnums[i]].vdil+anonym->vd[i];
            }
            else return 0;
         } /* end for */
         return 1;
      }
   }
   return 0;
} /* end stat4sats() */


static void satposits(SATPOSES satsp, double myx, double myy, double myz,
                unsigned long userweek, unsigned long weekms)
{
   unsigned long i;
   memset((char *)satsp,(char)0,sizeof(SATPOSES));
   for (i = 0UL; i<=31UL; i++) {
      satposit(&satsp[i], i, myx, myy, myz, userweek, weekms);
   } /* end for */
} /* end satposits() */

/*
PROCEDURE tropomodel(VAR satpos:SATPOS; userlat, userhigh:LONGREAL;
                time:CARDINAL);
VAR    zenith_dry_delay, zenith_wet_delay :LONGREAL;
       dayofyear:CARDINAL;
BEGIN
  WITH satpos DO
    IF elevation>0.0 THEN
      dayofyear:=time DIV (3600*24 DIV 4) MOD (365*4+1) DIV 4;
      TROPOSPHERE_DetermineZenithDelayValues_WAAS_Model(userlat, userhigh,
                dayofyear,
      zenith_dry_delay, zenith_wet_delay);

      TROPOSPHERE_GetDryAndWetDelay_UsingThe_UNBabc_MappingFunction(
      zenith_dry_delay, zenith_wet_delay, elevation, userlat, userhigh,
      drydelay, wetdelay);

--WrFixed(elevation/RAD, 1,6); WrFixed(wetdelay, 5,12);
                WrFixed(drydelay, 5,12);
--WrFixed(zenith_dry_delay, 5,12); WrFixed(zenith_wet_delay, 5,12);
                WrStrLn("");
    END;
  END;

END tropomodel;
*/
#define gpspos_MAXHMUL 2.0

#define gpspos_MAXVMUL 4.0


static void killexo(struct RESULTS stats0[], unsigned long stats_len,
                unsigned long probs, float * devhsum, float * devvsum)
{
   unsigned long i;
   unsigned long im;
   unsigned long c;
   double dy;
   double dx;
   float cosy;
   float max0;
   float dev;
   char ok0;
   unsigned long tmp;
   do {
      ok0 = 1;
      c = 0UL;
      dx = 0.0;
      dy = 0.0;
      tmp = probs-1UL;
      i = 0UL;
      if (i<=tmp) for (;; i++) {
         /* median */
         if ((stats0[i].res&0x9U)==0U) {
            dx = dx+stats0[i].long0;
            dy = dy+stats0[i].lat;
            ++c;
         }
         if (i==tmp) break;
      } /* end for */
      *devhsum = 0.0f;
      if (c>1UL) {
         /* filter min 3 measurements only */
         dx = X2C_DIVL(dx,(double)c);
         dy = X2C_DIVL(dy,(double)c);
         cosy = (float)cos((double)(float)dy);
         max0 = (-1.0f);
         tmp = probs-1UL;
         i = 0UL;
         if (i<=tmp) for (;; i++) {
            /* deviation and find badest */
            if ((stats0[i].res&0x9U)==0U) {
               dev = (float)(sqr((double)((float)(stats0[i].long0-dx)*cosy))
                +sqr((double)(float)(stats0[i].lat-dy)));
                /* error distance ^2 */
               /*WrFixed(sqrt(dev)/RAD/360.0*40000000.0, 1,6);
                WrStrLn("=dev"); */
               *devhsum = *devhsum+dev;
               if (dev>max0) {
                  max0 = dev;
                  im = i;
               }
            }
            if (i==tmp) break;
         } /* end for */
         if (c>2UL) stats0[im].res |= 0x1U;
         *devhsum = X2C_DIVR(*devhsum,(float)c); /* median deviation rad^2*/
         if (max0> *devhsum*2.0f) ok0 = 0;
      }
   } while (!ok0);
   if (*devhsum>0.0f) {
      *devhsum = (float)(sqrt((double)*devhsum)*6.3661977236758E+6);
                /* meter */
   }
   do {
      ok0 = 1;
      c = 0UL;
      dx = 0.0;
      tmp = probs-1UL;
      i = 0UL;
      if (i<=tmp) for (;; i++) {
         if ((stats0[i].res&0x14U)==0U) {
            dx = dx+stats0[i].heig;
            ++c;
         }
         if (i==tmp) break;
      } /* end for */
      *devvsum = 0.0f;
      if (c>1UL) {
         dx = X2C_DIVL(dx,(double)c);
         max0 = (-1.0f);
         tmp = probs-1UL;
         i = 0UL;
         if (i<=tmp) for (;; i++) {
            if ((stats0[i].res&0x14U)==0U) {
               dev = (float)sqr((double)(float)(stats0[i].heig-dx));
               *devvsum = *devvsum+dev;
               if (dev>max0) {
                  max0 = dev;
                  im = i;
               }
            }
            if (i==tmp) break;
         } /* end for */
         if (c>2UL) stats0[im].res |= 0x4U;
         *devvsum = X2C_DIVR(*devvsum,(float)c);
                /* median alt deviation m^2 */
         if (max0> *devvsum*4.0f) ok0 = 0;
      }
   } while (!ok0);
   if (*devvsum>0.0f) *devvsum = (float)sqrt((double)*devvsum);
} /* end killexo() */


static void killdop(struct RESULTS stats0[], unsigned long stats_len,
                unsigned long probs)
{
   unsigned long i;
   unsigned long tmp;
   tmp = probs-1UL;
   i = 0UL;
   if (i<=tmp) for (;; i++) {
      if (stats0[i].qsumh>1.E+5) stats0[i].res |= 0x8U;
      if (stats0[i].qsumv>1.E+5) stats0[i].res |= 0x10U;
      if (i==tmp) break;
   } /* end for */
} /* end killdop() */


static void median(const struct RESULTS stats0[], unsigned long stats_len,
                unsigned long tries, double * lat, double * long0,
                double * heig, unsigned long * cnt)
{
   unsigned long cz;
   unsigned long i;
   unsigned long tmp;
   *lat = 0.0;
   *long0 = 0.0;
   *heig = 0.0;
   *cnt = 0UL;
   cz = 0UL;
   tmp = tries-1UL;
   i = 0UL;
   if (i<=tmp) for (;; i++) {
      if ((stats0[i].res&0x9U)==0U) {
         *long0 = *long0+stats0[i].long0;
         *lat = *lat+stats0[i].lat;
         ++*cnt;
      }
      if ((stats0[i].res&0x14U)==0U) {
         *heig = *heig+stats0[i].heig;
         ++cz;
      }
      if (i==tmp) break;
   } /* end for */
   if (*cnt>0UL) {
      *lat = X2C_DIVL(*lat,(double)*cnt);
      *long0 = X2C_DIVL(*long0,(double)*cnt);
   }
   else {
      *lat = 0.0;
      *long0 = 0.0;
   }
   if (cz>0UL) *heig = X2C_DIVL(*heig,(double)cz);
   else *heig = 0.0;
} /* end median() */


static void showstats(const struct RESULTS stats0[], unsigned long stats_len,
                 unsigned long probs, float errh, float errv,
                unsigned long restcnt, char full)
{
   unsigned long p;
   unsigned long i;
   char h[31];
   unsigned long tmp;
   if (full) {
      InOut_WriteString(" probs:", 8ul);
      InOut_WriteInt((long)probs, 1UL);
      osi_WrStrLn("", 1ul);
      tmp = probs-1UL;
      p = 0UL;
      if (p<=tmp) for (;; p++) {
         for (i = 0UL; i<=11UL; i++) {
            InOut_WriteInt((long)(unsigned long)X2C_IN(i,16,
                stats0[p].satset), 1UL);
         } /* end for */
         InOut_WriteString(" ", 2ul);
         degtostr((float)stats0[p].lat, 1, '2', h, 31ul);
         InOut_WriteString(h, 31ul);
         InOut_WriteString(" ", 2ul);
         degtostr((float)stats0[p].long0, 0, '2', h, 31ul);
         InOut_WriteString(h, 31ul);
         InOut_WriteInt((long)X2C_TRUNCI(stats0[p].heig,X2C_min_longint,
                X2C_max_longint), 7UL);
         for (i = 0UL; i<=3UL; i++) {
            osi_WrFixed((float)stats0[p].hd[i], 0L, 5UL);
            osi_WrFixed((float)stats0[p].vd[i], 0L, 5UL);
         } /* end for */
         osi_WrFixed((float)stats0[p].qsumh, 0L, 8UL);
         osi_WrFixed((float)stats0[p].qsumv, 0L, 8UL);
         InOut_WriteString(" ", 2ul);
         /*    WrFixed(stats[p].minele/RAD, 0, 3); WrStr(" "); */
         for (i = 0UL; i<=4UL; i++) {
            InOut_WriteInt((long)(unsigned long)X2C_IN(i,16,stats0[p].res),
                1UL);
         } /* end for */
         osi_WrStrLn("", 1ul);
         if (p==tmp) break;
      } /* end for */
   }
   /*
     la:=0.0;
     lo:=0.0;
     i:=0;
     FOR p:=0 TO probs-1 DO la:=la+stats[p].lat; lo:=lo+stats[p].long;
                INC(i) END;
     la:=la/LFLOAT(i);
     lo:=lo/LFLOAT(i);
     las:=0.0;
     los:=0.0;
     FOR p:=0 TO probs-1 DO las:=las+ABS(stats[p].lat-la);
                los:=los+ABS(stats[p].long-lo) END;
     WrStr("s=");WrFixed(las/LFLOAT(i)*100000.0, 2, 8); WrStr(" ");
                WrFixed(los/LFLOAT(i)*100000.0, 2, 8); WrStrLn("");
   */
   InOut_WriteString("n=", 3ul);
   InOut_WriteInt((long)restcnt, 3UL);
   InOut_WriteString(" s=", 4ul);
   osi_WrFixed(errh, 1L, 0UL);
   osi_WrFixed(errv, 1L, 6UL);
   osi_WrStrLn("", 1ul);
} /* end showstats() */


static double median0(gpspos_SATS sats, unsigned long satcnt,
                unsigned long bad)
/* median except from bad and badspeeds */
{
   unsigned long n;
   unsigned long i;
   double m;
   unsigned long tmp;
   m = 0.0;
   n = 0UL;
   tmp = satcnt-1UL;
   i = 0UL;
   if (i<=tmp) for (;; i++) {
      if (i!=bad && !sats[i].badspeed) {
         m = m+sats[i].userspeed;
         ++n;
      }
      if (i==tmp) break;
   } /* end for */
   if (n>0UL) m = X2C_DIVL(m,(double)n);
   return m;
} /* end median() */


static unsigned long peak(gpspos_SATS sats, unsigned long satcnt, double med,
                 unsigned long bad)
/* max deviation except from bad and badspeeds */
{
   unsigned long b;
   unsigned long i;
   double a;
   double m;
   unsigned long tmp;
   m = 0.0;
   b = X2C_max_longcard;
   tmp = satcnt-1UL;
   i = 0UL;
   if (i<=tmp) for (;; i++) {
      if (i!=bad && !sats[i].badspeed) {
         a = fabs(sats[i].userspeed-med);
         if (a>=m) {
            m = a;
            b = i;
         }
      }
      if (i==tmp) break;
   } /* end for */
   return b;
} /* end peak() */

#define gpspos_MINSATS 4

#define gpspos_MAXSPEED0 200


static void speedrange(gpspos_SATS sats, unsigned long satcnt,
                SATPOSES satspos)
/* shift ranges with speed */
{
   unsigned long goodsats;
   unsigned long bad1;
   unsigned long bad;
   unsigned long i;
   double med1;
   double med;
   struct gpspos_SAT * anonym;
   unsigned long tmp;
   if (satcnt>=4UL) {
      tmp = satcnt-1UL;
      i = 0UL;
      if (i<=tmp) for (;; i++) {
         { /* with */
            struct gpspos_SAT * anonym = &sats[i];
            anonym->userspeed = (double)
                anonym->rang1*7.4333465936861E-4-satspos[anonym->almidx]
                .doppler; /* doppler speed sat - user */
            anonym->badspeed = 0;
         }
         if (i==tmp) break;
      } /* end for */
      goodsats = satcnt;
      for (;;) {
         /*WrStr("speeds"); */
         med = median0(sats, satcnt, X2C_max_longcard);
                /* find sat oszillator deviation */
         bad = peak(sats, satcnt, med, X2C_max_longcard);
                /* find bad synchronized doppler for a sat */
         if (bad>=satcnt) break;
         /*WrInt(bad, 3); */
         med1 = median0(sats, satcnt, bad); /* median without bad sat */
         if (fabs(sats[bad].userspeed-med1)<=200.0) {
            bad1 = peak(sats, satcnt, med1, bad); /* second badest sat */
            /*WrInt(bad1, 3); */
            if (bad1>=satcnt || fabs(sats[bad].userspeed-med1)
                *0.25<fabs(sats[bad1].userspeed-med1)) {
               break; /* if badest sat deviates less than k* second badest */
            }
         }
         sats[bad].badspeed = 1;
         med = med1;
         /*WrStrLn(""); */
         tmp = satcnt-1UL;
         i = 0UL;
         if (i<=tmp) for (;; i++) {
            /*WrFixed(sats[i].userspeed, 1, 7); */
            sats[i].userspeed = sats[i].userspeed-med;
            if (i==tmp) break;
         } /* end for */
         /*WrFixed(sats[i].userspeed, 1, 7); */
         /*WrInt(ORD(sats[i].badspeed), 2); WrStrLn(""); */
         --goodsats;
         if (goodsats<=4UL) break;
      }
   }
} /* end speedrange() */


static double neardist(double lat1, double long1, double lat2, double long2)
{
   double y;
   double x;
   x = (long1-long2)*cos((lat1+lat2)*0.5);
   y = lat1-lat2;
   return 6.37E+6*sqrt(x*x+y*y);
} /* end neardist() */


static double azimuth(double lat1, double long1, double lat2, double long2)
/* degrees */
{
   double ldiff;
   double h;
   struct X2C_XHandler_STR anonym;
   double azimuth_ret;
   if (X2C_XTRY(&anonym)) {
      ldiff = long2-long1;
      if ((ldiff==0.0 || cos(lat1)==0.0) || cos(lat2)==0.0) {
         if (lat1<=lat2) azimuth_ret = 0.0;
         else azimuth_ret = 180.0;
      }
      else {
         h = X2C_DIVL(atan(cos(lat1)*(X2C_DIVL(tan(lat1)*cos(ldiff)-tan(lat2)
                ,sin(ldiff)))),1.7453292519943E-2);
         if (ldiff<0.0) azimuth_ret = h+270.0;
         else azimuth_ret = h+90.0;
      }
      X2C_XOFF();
   }
   else azimuth_ret = 0.0;
   X2C_XREMOVE();
   return azimuth_ret;
} /* end azimuth() */


static void dirmed(double * dirsum, double dir, unsigned long cnt)
/* angle median in degrees */
{
   double d;
   /*WrFixed(dir, 0,0); WrFixed(dirsum/(LFLOAT(cnt)+0.0001), 0,0); */
   if (cnt==0UL) *dirsum = dir;
   else {
      *dirsum = *dirsum+dir;
      /*WrFixed(dir-dirsum/LFLOAT(cnt+1), 0,0); */
      if (fabs(dir-X2C_DIVL(*dirsum,(double)(cnt+1UL)))>180.0) {
         *dirsum = *dirsum+360.0;
      }
      d = X2C_DIVL(*dirsum,(double)(cnt+1UL));
      if (d>=360.0) {
         d = d-360.0;
         *dirsum = d*(double)(cnt+1UL);
      }
   }
/*WrFixed(dirsum/(LFLOAT(cnt+1)+0.0001), 0,0); WrStrLn("=dir"); */
} /* end dirmed() */

/*
PROCEDURE ionomodel(VAR sat:SATPOS; klobuchar:GNSS_structKlobuchar;
          mylat, mylong, myhigh:LONGREAL; tow:CARDINAL);
BEGIN
  WITH klobuchar DO
    IF NOT IONOSPHERE_GetL1KlobucharCorrection
       (alpha0, alpha1, alpha2, alpha3, beta0,  beta1,  beta2,  beta3,
       mylat, mylong, sat.elevation, sat.azimuth, LFLOAT(tow), sat.ionocorr)
    THEN sat.ionocorr:=0.0 END;
--WrFixed(sat.ionocorr, 1, 6); WrFixed(mylat/RAD, 3, 7);
                WrFixed(mylong/RAD, 3, 7);
--WrFixed(sat.azimuth/RAD, 1, 6);WrFixed(sat.elevation/RAD, 1, 6);
                WrStrLn("=iono"); 


  END;
END ionomodel;
*/
#define gpspos_ZEROTIME 315964800

#define gpspos_WEEK 604800

#define gpspos_HALFWEEK 302400


extern long gpspos_getposit(unsigned long weekms, unsigned long * systime,
                gpspos_SATS sats, double mylat, double mylong, double myhigh,
                 double * lat, double * long0, double * heig, double * speed,
                 double * dir, double * climb, float * hrms, float * vrms)
{
   double aa;
   double vheig;
   double vlong;
   double vlat;
   double min0;
   double malt;
   double mlong;
   double mlat;
   double bz;
   double by;
   double bx;
   double myz;
   double myy;
   double myx;
   unsigned long satnums[4];
   long ret;
   unsigned long restcnt;
   unsigned long speeds;
   unsigned long tries;
   unsigned long satcnt;
   unsigned long bit;
   unsigned long j;
   unsigned long i;
   unsigned long cnt;
   unsigned long weeks;
   unsigned long userweek;
   unsigned long gpstime;
   unsigned short comb;
   SATPOSES satspos;
   gpspos_SATS tmp;
   unsigned long tmp0;
   unsigned long tmp1;
   sats = (struct gpspos_SAT *)memcpy(tmp,sats,sizeof(gpspos_SATS));
   weeks = (weekms/1000UL)%604800UL; /* second in week from gps */
   gpstime = *systime-315964800UL; /* absolute gps time from system clock */
   i = ((weeks+604800UL)-gpstime%604800UL)%604800UL; /* gps is later */
   if (i<302400UL) i += 604800UL;
   gpstime = (gpstime+i)-604800UL; /* system time corrected to gps time */
   *systime = gpstime+315964800UL; /* return corrected systime */
   userweek = gpstime/604800UL&1023UL;
   /*WrInt(calm[0].week MOD 1024, 6); WrInt(userweek, 6); WrInt(weeks, 16);
                WrInt(gpstime MOD WEEK, 16);WrInt(i-WEEK, 16);
                WrStrLn("=weeks"); */
   wgs84(mylat, mylong, myhigh, &myx, &myy, &myz);
   memset((char *)stats,(char)0,sizeof(struct RESULTS [500]));
   memset((char *)satspos,(char)0,sizeof(SATPOSES));
   satposits(satspos, myx, myy, myz, userweek, weekms);
   /*corrrang(sats); */
   satcnt = 0UL;
   for (i = 0UL; i<=11UL; i++) {
      if (sats[i].prn && sats[i].rang<X2C_max_longint) {
         j = 0UL;
         for (;;) {
            if ((unsigned long)calm[j].prn==sats[i].prn) {
               sats[satcnt] = sats[i];
               sats[satcnt].almidx = j;
               /*
                         IF rinexok THEN
                           ionomodel(satspos[j], rinexklobuchar, mylat,
                mylong, myhigh, weeks);
                         END;
               */
               /*        tropomodel(satspos[j], mylat, myhigh, systime); */
               ++satcnt;
               break;
            }
            ++j;
            if (j>31UL) break;
         }
      }
   } /* end for */
   tries = 0UL;
   *hrms = 0.0f;
   ret = -1L;
   mlat = 0.0;
   mlong = 0.0;
   malt = 0.0;
   if (satcnt>=4UL) {
      comb = 0U;
      do {
         cnt = 0UL;
         tmp0 = satcnt-1UL;
         bit = 0UL;
         if (bit<=tmp0) for (;; bit++) {
            if (X2C_IN(bit,16,(unsigned short)comb)) ++cnt;
            if (bit==tmp0) break;
         } /* end for */
         if (cnt==4UL) {
            cnt = 0UL;
            tmp0 = satcnt-1UL;
            bit = 0UL;
            if (bit<=tmp0) for (;; bit++) {
               if (X2C_IN(bit,16,(unsigned short)comb)) {
                  satspos[sats[bit].almidx].range = (double)(unsigned long)
                (X2C_max_longint-sats[bit].rang)*2.8618384385692E-1;
                  /*- (LFLOAT(ORD(ODD(weeks))*2)-1.0)
                *satspos[sats[bit].almidx].ionocorr; */
                  satnums[cnt] = sats[bit].almidx;
                  ++cnt;
               }
               if (bit==tmp0) break;
            } /* end for */
            if (stat4sats(satspos, satnums, 4ul, tries)) {
               stats[tries].qsumh = 0.0;
               stats[tries].qsumv = 0.0;
               for (i = 0UL; i<=3UL; i++) {
                  stats[tries].qsumh = stats[tries].qsumh+sqr(stats[tries]
                .hd[i]);
                  stats[tries].qsumv = stats[tries].qsumv+sqr(stats[tries]
                .vd[i]);
                  if (stats[tries].hd[i]==0.0 || stats[tries].vd[i]==0.0) {
                     stats[tries].qsumh = 0.0;
                     stats[tries].qsumv = 0.0;
                  }
               } /* end for */
               stats[tries].satset = (unsigned short)comb;
               ++tries;
            }
         }
         ++comb;
      } while (!X2C_IN(satcnt,16,(unsigned short)comb));
   }
   InOut_WriteInt((long)satcnt, 2UL);
   InOut_WriteString("=sats", 6ul);
   InOut_WriteInt((long)tries, 5UL);
   osi_WrStrLn("=tries", 7ul);
   if (tries>0UL) {
      killdop(stats, 500ul, tries);
      killexo(stats, 500ul, tries, hrms, vrms);
      median(stats, 500ul, tries, lat, long0, heig, &restcnt);
                /* final position */
      showstats(stats, 500ul, tries, *hrms, *vrms, restcnt, 0);
      ret = 0L;
   }
   if (ret>=0L) {
      /*---speed */
      if (*lat==0.0 && *long0==0.0) {
         wgs84(mylat, mylong, myhigh, &bx, &by, &bz); /* show sat table */
      }
      else wgs84(*lat, *long0, *heig, &bx, &by, &bz);
      satposits(satspos, bx, by, bz, userweek, weekms);
      /*    dgps(sats, satcnt, satspos, bx, by, bz, mylat, mylong, myhigh,
                rangcor); */
      speedrange(sats, satcnt, satspos);
      *speed = 0.0;
      *dir = 0.0;
      *climb = 0.0;
      speeds = 0UL;
      tmp0 = tries-1UL;
      i = 0UL;
      if (i<=tmp0) for (;; i++) {
         if ((stats[i].res&0x18U)==0U) {
            cnt = 0UL;
            tmp1 = satcnt-1UL;
            bit = 0UL;
            if (bit<=tmp1) for (;; bit++) {
               if (X2C_IN(bit,16,stats[i].satset) && !sats[bit].badspeed) {
                  satspos[sats[bit].almidx].range = (double)(unsigned long)
                (X2C_max_longint-sats[bit].rang)
                *2.8618384385692E-1+sats[bit].userspeed;
                  satnums[cnt] = sats[bit].almidx;
                  ++cnt;
               }
               if (bit==tmp1) break;
            } /* end for */
            if (cnt==4UL && get4sats(satspos, satnums, 4ul, 0UL, 0.0, &vlat,
                &vlong, &vheig)>0L) {
               *speed = *speed+neardist(stats[i].lat, stats[i].long0, vlat,
                vlong);
               dirmed(dir, azimuth(vlat, vlong, stats[i].lat,
                stats[i].long0), speeds);
               *climb = (*climb+stats[i].heig)-vheig;
               ++speeds;
            }
         }
         if (i==tmp0) break;
      } /* end for */
      /*WrInt(i,2); WrFixed((stats[i].lat-vlat)/RAD*111111.0, 1,6); */
      /*WrFixed((stats[i].long-vlong)/RAD*111111.0, 1,6); */
      /*WrFixed(stats[i].heig-vheig, 1,9);  WrStrLn(""); */
      if (speeds>0UL) {
         *speed = X2C_DIVL(*speed,(double)speeds);
         *dir = X2C_DIVL(*dir,(double)speeds);
         *climb = X2C_DIVL(*climb,(double)speeds);
      }
      /*
      WrFixed(speed, 1,6);
      WrFixed(dir, 1,6);
      WrStrLn("=speed/dir");
      */
      /*---speed */
      min0 = 0.0;
      osi_WrStrLn("prn az    ele       clc      dopp   freq0", 42ul);
      tmp0 = satcnt;
      i = 1UL;
      if (i<=tmp0) for (;; i++) {
         j = sats[i-1UL].almidx;
         if (j<=31UL) {
            /*& (satspos[j].elevation>-10.0)*/
            InOut_WriteInt((long)sats[i-1UL].prn, 2UL);
            osi_WrFixed((float)(X2C_DIVL(satspos[j].azimuth0,
                1.7453292519943E-2)), 1L, 7UL);
            osi_WrFixed((float)(X2C_DIVL(satspos[j].elevation,
                1.7453292519943E-2)), 1L, 6UL);
            osi_WrFixed((float)satspos[j].clk, 0L, 10UL);
            osi_WrFixed((float)satspos[j].doppler, 1L, 10UL);
            InOut_WriteInt(sats[i-1UL].rang1, 8UL);
            /*        WrFixed(LFLOAT(sats[i-1].freq0)*(WAVLEN/256.0), 3, 10);
                 */
            aa = (double)sats[i-1UL].rang1*7.4333465936861E-4-satspos[j].doppler;
            osi_WrFixed((float)aa, 3L, 10UL);
            /*      WrFixed(ABS(aa-min), 3, 10); */
            min0 = aa;
            if (sats[i-1UL].badspeed) InOut_WriteString(" v", 3ul);
            osi_WrStrLn("", 1ul);
         }
         if (i==tmp0) break;
      } /* end for */
   }
   return ret;
} /* end getposit() */

struct SEM_structAlmanac;


struct SEM_structAlmanac {
   unsigned long toa; /* almanac time of applicability (reference time [s]*/
   unsigned short week; /* 10 bit gps week 0-1023 (user must account for week rollover) [week] */
   unsigned short prn; /* GPS prn number */
   unsigned short reserved; /* reserved */
   unsigned short svn; /* Satellite vehicle number */
   unsigned char ura; /* User Range Accuracy lookup code, [0-15], see p. 83 GPSICD200C, 0 is excellent, 15 is use at own risk */
   unsigned char health; /* 0=healthy, unhealthy otherwise  [], subframe 4 and 5, page 25 six-bit health code */
   unsigned char config_code; /* configuration code   [], if >=9 Anti-Spoofing is on */
   /* this inicator is not part of the SEM standard but is added by the user if known */
   char is_af0_af1_high_precision;
                /* indicates precision of af0 and af1 [1=high precision,
                0=low precision] (22&16 bits,
                ephemeris source) vs (11&11 bits, almanac source),
                0 is typical for most SEM sources */
   double ecc; /* eccentricity */
   double i0; /* orbital inclination at reference time                        [rad] */
   double omegadot; /* rate of right ascension                                      [rad/s] */
   double sqrta; /* square root of the semi-major axis                           [m^(1/2)]*/
   double omega0; /* longitude of ascending node of orbit plane at weekly epoch   [rad] */
   double w; /* argument of perigee                                          [rad] */
   double m0; /* mean anomaly at reference time                               [rad] */
   double af0; /* polynomial clock correction coefficient (clock bias)         [s],   Note: parameters from ephemeris preferred vs almanac (22 vs 11 bits) */
   double af1; /* polynomial clock correction coefficient (clock drift)        [s/s], Note: parameters from ephemeris preferred vs almanac (16 vs 11 bits) */
};

struct YUMA_structAlmanac;


struct YUMA_structAlmanac {
   unsigned short reserved1;
   unsigned short week; /* 10 bit gps week 0-1023 (user must account for week rollover) [week] */
   unsigned short prn; /* GPS prn number */
   unsigned char health; /* 0=healthy, unhealthy otherwise  [], subframe 4 and 5, page 25 six-bit health code */
   double ecc; /* eccentricity */
   double toa; /* time of applicability */
   double i0; /* orbital inclination at reference time                        [rad] */
   double omegadot; /* rate of right ascension                                      [rad/s] */
   double sqrta; /* square root of the semi-major axis                           [m^(1/2)]*/
   double omega0; /* longitude of ascending node of orbit plane at weekly epoch   [rad] */
   double w; /* argument of perigee                                          [rad] */
   double m0; /* mean anomaly at reference time                               [rad] */
   double af0; /* polynomial clock correction coefficient (clock bias)         [s],   Note: parameters from ephemeris preferred vs almanac (22 vs 11 bits) */
   double af1; /* polynomial clock correction coefficient (clock drift)        [s/s], Note: parameters from ephemeris preferred vs almanac (16 vs 11 bits) */
};


extern char gpspos_readalmanach(char fnsem[], unsigned long fnsem_len,
                char fnyuma[], unsigned long fnyuma_len, char fnrinex[],
                unsigned long fnrinex_len, unsigned long secondinweek)
{
   unsigned char cnt;
   unsigned long ti;
   unsigned long ri;
   unsigned long j;
   unsigned long i;
   struct SEM_structAlmanac alm[32];
   struct YUMA_structAlmanac yumaalm[32];
   struct structEphemeris rinexalm[3072];
   unsigned long min0[32];
   unsigned long tmp;
   char gpspos_readalmanach_ret;
   X2C_PCOPY((void **)&fnsem,fnsem_len);
   X2C_PCOPY((void **)&fnyuma,fnyuma_len);
   X2C_PCOPY((void **)&fnrinex,fnrinex_len);
   memset((char *)alm,(char)0,sizeof(struct SEM_structAlmanac [32]));
   memset((char *)calm,(char)0,sizeof(struct COMMONALMANACH [32]));
   memset((char *)yumaalm,(char)0,sizeof(struct YUMA_structAlmanac [32]));
   memset((char *)rinexalm,(char)0,sizeof(struct structEphemeris [3072]));
   semok = fnsem[0UL] && SEM_ReadAlmanacDataFromFile(fnsem, (char *)alm, 32U,
                 &cnt);
   if (semok) {
      for (i = 0UL; i<=31UL; i++) {
         alm[i].i0 = alm[i].i0+9.4247779607694E-1;
      } /* end for */
   }
   yumaok = fnyuma[0UL] && YUMA_ReadAlmanacDataFromFile(fnyuma,
                (char *)yumaalm, 32U, &cnt);
   rinexok = fnrinex[0UL] && RINEX_DecodeGPSNavigationFile(fnrinex,
                (char *) &rinexklobuchar, (char *)rinexalm, 3071UL, &ri);
   if (rinexok && ri>0UL) {
      InOut_WriteInt((long)ri, 1UL);
      osi_WrStrLn("=rec", 5ul);
      for (i = 0UL; i<=31UL; i++) {
         min0[i] = 0UL;
      } /* end for */
      ti = ((TimeConv_time()-7200UL)+345600UL)%604800UL;
      InOut_WriteInt((long)ti, 12UL);
      InOut_WriteInt((long)secondinweek, 12UL);
      osi_WrStrLn("=ti secondinweek", 17ul);
      tmp = ri-1UL;
      j = 0UL;
      if (j<=tmp) for (;; j++) {
         /*WrInt(VAL(CARDINAL,rinexalm[j].toe) + VAL(CARDINAL,
                rinexalm[j].week)*604800, 18); WrStrLn("=toe"); */
         /*WrInt(VAL(CARDINAL, rinexalm[j].week), 18);
                WrInt(time() DIV 604800, 18);  WrStrLn("=week"); */
         /*WrInt(VAL(CARDINAL, rinexalm[j].tow), 18); WrStrLn("=tow"); */
         i = (unsigned long)rinexalm[j].prn;
         /*IF i>0 THEN WrInt(rinexalm[j].toe, 8); WrStrLn("=toe"); END; */
         /*      IF (i>0) & (i<=HIGH(min)) & (rinexalm[j].toe>=ti)
                & (min[i]>rinexalm[j].toe) THEN */
         /*      IF (i>0) & (i<=HIGH(min)+1) & (rinexalm[j].toe<ti)
                & (min[i-1]<rinexalm[j].toe) THEN */
         ti = ((rinexalm[j].tow+604800UL)-secondinweek)%604800UL;
         /*      IF (i>0) & (i<=HIGH(min)+1) & (rinexalm[j].tow<secondinweek)
                 & (min[i-1]<rinexalm[j].tow) THEN */
         if (((i>0UL && i<=32UL) && ti>302400UL) && min0[i-1UL]<ti) {
            --i;
            /*        min[i]:=rinexalm[j].tow; */
            min0[i] = ti;
            /*WrInt(min[i], 8); WrStrLn("=min"); */
            calm[i].week = rinexalm[j].week;
            calm[i].prn = rinexalm[j].prn;
            calm[i].health = rinexalm[j].health;
            calm[i].ecc = rinexalm[j].ecc;
            calm[i].i0 = rinexalm[j].i0;
            calm[i].omegadot = rinexalm[j].omegadot;
            calm[i].sqrta = rinexalm[j].sqrta;
            calm[i].omega0 = rinexalm[j].omega0;
            calm[i].w = rinexalm[j].w;
            calm[i].m0 = rinexalm[j].m0;
            calm[i].af0 = rinexalm[j].af0;
            calm[i].af1 = rinexalm[j].af1;
            calm[i].af2 = rinexalm[j].af2;
            calm[i].toe = rinexalm[j].toe;
            calm[i].toc = rinexalm[j].toc;
            calm[i].tow_week = rinexalm[j].tow_week;
            calm[i].tow = rinexalm[j].tow;
            calm[i].tgd = rinexalm[j].tgd;
            calm[i].delta_n = rinexalm[j].delta_n;
            calm[i].idot = rinexalm[j].idot;
            calm[i].cuc = rinexalm[j].cuc;
            calm[i].cus = rinexalm[j].cus;
            calm[i].crc = rinexalm[j].crc;
            calm[i].crs = rinexalm[j].crs;
            calm[i].cic = rinexalm[j].cic;
            calm[i].cis = rinexalm[j].cis;
         }
         if (j==tmp) break;
      } /* end for */
      for (i = 0UL; i<=31UL; i++) {
         InOut_WriteInt((long)calm[i].tow, 8UL);
      } /* end for */
      osi_WrStrLn("=used tow", 10ul);
   }
   else if (semok) {
      for (i = 0UL; i<=31UL; i++) {
         calm[i].toa = alm[i].toa;
         calm[i].week = alm[i].week;
         calm[i].prn = alm[i].prn;
         calm[i].health = alm[i].health;
         calm[i].ecc = alm[i].ecc;
         calm[i].i0 = alm[i].i0;
         calm[i].omegadot = alm[i].omegadot;
         calm[i].sqrta = alm[i].sqrta;
         calm[i].omega0 = alm[i].omega0;
         calm[i].w = alm[i].w;
         calm[i].m0 = alm[i].m0;
         calm[i].af0 = alm[i].af0;
         calm[i].af1 = alm[i].af1;
      } /* end for */
   }
   else if (yumaok) {
      /*    IF verb THEN */
      /*
            FOR i:=0 TO HIGH(calm) DO
              j:=0;
              WHILE (j<=HIGH(yumaalm)) & (alm[i].prn<>yumaalm[j].prn)
                DO INC(j) END;
              IF j<=HIGH(yumaalm) THEN
                WrInt(alm[i].prn, 3); WrStrLn("");
                WrInt(alm[i].toa, 25); WrInt(TRUNC(yumaalm[j].toa), 25);
                WrStrLn("");
                WrInt(alm[i].week, 25); WrInt(yumaalm[j].week, 25);
                WrStrLn("");
                WrInt(alm[i].health, 25); WrInt(yumaalm[j].health, 25);
                WrStrLn("");
                WrFixed(alm[i].ecc, 12, 25); WrFixed(yumaalm[j].ecc, 12, 25);
                 WrStrLn("");
                WrFixed(alm[i].i0, 12, 25);  WrFixed(yumaalm[j].i0, 12, 25);
                WrStrLn("");
                WrFixed(alm[i].omegadot, 12, 25);
                WrFixed(yumaalm[j].omegadot, 12, 25); WrStrLn("");
                WrFixed(alm[i].sqrta, 12, 25);
                WrFixed(yumaalm[j].sqrta, 12, 25); WrStrLn("");
                WrFixed(alm[i].omega0, 12, 25);
                WrFixed(yumaalm[j].omega0, 12, 25); WrStrLn("");
                WrFixed(alm[i].w, 12, 25); WrFixed(yumaalm[j].w, 12, 25);
                WrStrLn("");
                WrFixed(alm[i].m0, 12, 25); WrFixed(yumaalm[j].m0, 12, 25);
                WrStrLn("");
                WrFixed(alm[i].af0, 12, 25); WrFixed(yumaalm[j].af0, 12, 25);
                 WrStrLn("");
                WrFixed(alm[i].af1, 12, 25); WrFixed(yumaalm[j].af1, 12, 25);
                 WrStrLn("");
              END;
      */
      /*      END; */
      for (i = 0UL; i<=31UL; i++) {
         calm[i].toa = (unsigned long)X2C_TRUNCC(yumaalm[i].toa,0UL,
                X2C_max_longcard);
         calm[i].week = yumaalm[i].week;
         calm[i].prn = yumaalm[i].prn;
         calm[i].health = yumaalm[i].health;
         calm[i].ecc = yumaalm[i].ecc;
         calm[i].i0 = yumaalm[i].i0;
         calm[i].omegadot = yumaalm[i].omegadot;
         calm[i].sqrta = yumaalm[i].sqrta;
         calm[i].omega0 = yumaalm[i].omega0;
         calm[i].w = yumaalm[i].w;
         calm[i].m0 = yumaalm[i].m0;
         calm[i].af0 = yumaalm[i].af0;
         calm[i].af1 = yumaalm[i].af1;
      } /* end for */
   }
   gpspos_readalmanach_ret = (semok || yumaok) || rinexok;
   X2C_PFREE(fnsem);
   X2C_PFREE(fnyuma);
   X2C_PFREE(fnrinex);
   return gpspos_readalmanach_ret;
} /* end readalmanach() */


extern void gpspos_BEGIN(void)
{
   static int gpspos_init = 0;
   if (gpspos_init) return;
   gpspos_init = 1;
   TimeConv_BEGIN();
   osi_BEGIN();
}

