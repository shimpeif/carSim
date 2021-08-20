/*
PURPOSE:
	(Generate a LEFT_HANDED quaternion using an Euler PITCH_ROLL_YAW sequence
         OR generate an Euler  PITCH_ROLL_YAW sequence using a quaternion.)

PROGRAMMERS:
	(((Hung Nguyen) (CACI) (03/23/2016)))
*/

#include "trick/trick_math.h"

int euler213_quat(
    double angle[3],	/* Inout: r  Method=1, 0=PITCH, 1=ROLL, 2=YAW. */
    double quat[4],	/* Inout: r  Method=0, quaternion matrix. */
    int method, 	/*    In: -- 0 = Make left handed quaternion from angles,
    			             1 = Make angles from quaternion
    			             2 = Make angles from quaternion but use previous
    				     values to prevent singularities. */
    double *prev)	/*   In: r  Previous values of euler angles. */
{

   double half_angle[3];
   double mat00, mat01, mat10, mat11, mat20, mat21, mat22;
   double s1;
   double c1;
   double s2;
   double c2;
   double s3;
   double c3;
   double tmp;
   int ret = 0;
   static unsigned short error_flag[5] = {0, 0, 0, 0, 0}; /* Send errors only once */

   if (method == 0){
       /* Compute sines and cosines of 0.5*eulers */
       V_SCALE(half_angle, angle, 0.5);
       s1 = sin(half_angle[0]);
       c1 = cos(half_angle[0]);
       s2 = sin(half_angle[1]);
       c2 = cos(half_angle[1]);
       s3 = sin(half_angle[2]);
       c3 = cos(half_angle[2]);

       quat[0] =  c1*c2*c3 + s1*s2*s3;
       quat[1] = -c1*s2*c3 - s1*c2*s3;
       quat[2] = -s1*c2*c3 + c1*s2*s3;
       quat[3] = -c1*c2*s3 + s1*s2*c3;


   }
   else if (method == 1){
#define TOLERANCE 1.0e-15
       /* Within normal range for asin function */
       mat21 = 2. * (quat[2] * quat[3] + quat[0] * quat[1]);

       if (-1.0 <= -mat21 && -mat21 <= 1.0) {
               angle[1] = asin(-mat21);
               if (M_ABS(angle[1] - M_PI_2) < 1.0e-6) {
                       mat10 = 2. * (quat[1] * quat[2] + quat[0] * quat[3]);
                       mat00 = 1. - 2. * (quat[2] * quat[2] + quat[3] * quat[3]);
        	       angle[0] = atan2(mat10, mat00);
        	       angle[1] = M_PI_2;
        	       angle[2] = 0.0;
        	       ret = TM_SING_213_P;
        	       if ( error_flag[0] == 0 ) {
        		   tm_print_error(ret);
        		   error_flag[0]=1;
        	       }
               } else if (M_ABS(angle[1] + M_PI_2) < 1.0e-6) {
                       mat10 = 2. * (quat[1] * quat[2] + quat[0] * quat[3]);
                       mat00 = 1. - 2. * (quat[2] * quat[2] + quat[3] * quat[3]);
        	       angle[0] = atan2(-mat10, mat00);
        	       angle[1] = -M_PI_2;
        	       angle[2] = 0.0;
        	       ret = TM_SING_213_N;
        	       if ( error_flag[1] == 0 ) {
        		   tm_print_error(ret);
        		   error_flag[1]=1;
        	       }
               } else {
                       mat01 = 2. * (quat[1] * quat[2] - quat[0] * quat[3]);
                       mat11 = 1. - 2. * (quat[3] * quat[3] + quat[1] * quat[1]);
                       mat20 = 2. * (quat[1] * quat[3] - quat[0] * quat[2]);
                       mat22 = 1. - 2. * (quat[1] * quat[1] + quat[2] * quat[2]);
        	       angle[0] = atan2(mat20, mat22);
        	       angle[2] = atan2(mat01, mat11);
               }
       }

       /* Out of normal range for asin func, but within tolerance */
        else if (1.0 < -mat21 && -mat21 <= (1.0 + TOLERANCE)) {
                mat10 = 2. * (quat[1] * quat[2] + quat[0] * quat[3]);
                mat00 = 1. - 2. * (quat[2] * quat[2] + quat[3] * quat[3]);
                angle[0] = atan2(mat10, mat00);
                angle[1] = M_PI_2;
                angle[2] = 0.0;
                ret = TM_SING_213_P;
                if ( error_flag[2] == 0 ) {
                    tm_print_error(ret);
                    error_flag[2]=1;
                }
        } else if ((-1.0 - TOLERANCE) <= -mat21 && -mat21 < -1.0) {
                mat10 = 2. * (quat[1] * quat[2] + quat[0] * quat[3]);
                mat00 = 1. - 2. * (quat[2] * quat[2] + quat[3] * quat[3]);
                angle[0] = atan2(-mat10, mat00);
                angle[1] = -M_PI_2;
                angle[2] = 0.0;
                ret = TM_SING_213_N;
                if ( error_flag[3] == 0 ) {
                    tm_print_error(ret);
                    error_flag[3]=1;
                }
        }
        /* Error: Out of normal range & beyond tolerance
           for asin func */
        else {
                ret = TM_ANG_NAN;
                if ( error_flag[4] == 0 ) {
                    tm_print_error(ret);
                    error_flag[4]=1;
                }
                angle[0] = angle[1] = angle[2] = NAN;
        }
#undef TOLERANCE
   } else if (method == 2) {
#define TOLERANCE 0.0314159265358979    /* 1.8 degree tolerance */

        mat21 = 2. * (quat[2] * quat[3] + quat[0] * quat[1]);

        /* Compute euler angles from tranformation */
        if (M_ABS(mat21 + 1.0) < 1.0e-6) {
                mat10 = 2. * (quat[1] * quat[2] + quat[0] * quat[3]);
                mat00 = 1. - 2. * (quat[2] * quat[2] + quat[3] * quat[3]);
                angle[0] = atan2(mat10, mat00) + prev[2];
                angle[1] = M_PI_2;
                angle[2] = prev[2];
                tmp = angle[0] - prev[0];
                if (M_ABS(tmp - 2.0 * M_PI) < TOLERANCE)
                        angle[0] -= 2.0 * M_PI;
                else if (M_ABS(tmp + 2.0 * M_PI) < TOLERANCE)
                        angle[0] += 2.0 * M_PI;

        } else if (M_ABS(mat21 - 1.0) < 1.0e-6) {
                mat10 = 2. * (quat[1] * quat[2] + quat[0] * quat[3]);
                mat00 = 1. - 2. * (quat[2] * quat[2] + quat[3] * quat[3]);
                angle[0] = atan2(-mat10, mat00) - prev[2];
                angle[1] = -M_PI_2;
                angle[2] = prev[2];
                tmp = angle[0] - prev[0];
                if (M_ABS(tmp - 2.0 * M_PI) < TOLERANCE)
                        angle[0] -= 2.0 * M_PI;
                else if (M_ABS(tmp + 2.0 * M_PI) < TOLERANCE)
                        angle[0] += 2.0 * M_PI;
        } else {
                mat01 = 2. * (quat[1] * quat[2] - quat[0] * quat[3]);
                mat11 = 1. - 2. * (quat[3] * quat[3] + quat[1] * quat[1]);
                mat20 = 2. * (quat[1] * quat[3] - quat[0] * quat[2]);
                mat21 = 2. * (quat[2] * quat[3] + quat[0] * quat[1]);
                mat22 = 1. - 2. * (quat[1] * quat[1] + quat[2] * quat[2]);
                angle[0] = atan2(mat20, mat22);
                angle[1] = asin(-mat21);
                angle[2] = atan2(mat01, mat11);
        }
#undef TOLERANCE
   }

   return(ret);
}
