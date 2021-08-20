/************************TRICK HEADER*************************
PURPOSE: Mass-Spring-Damper class header file .
PROGRAMMERS:
    (((Scott P. Fennell) (CACI International Inc.) (January 2018) (Trick Learning Project)))
*************************************************************/

#ifndef MSD_H
#define MSD_H

class MSD {
public:

    double a;           /* m/s2 acceleration  */
    double v;           /* m/s velocity */
    double x;           /* m position */
    double m;           /* kg mass */;

    double v_0;         /* *i m/s initial velocity of the mass (at t = 0) */
    double x_0;         /* *i m initial position of the mass (at t = 0) */
    
    double b;           /* N.s/m damping constant */
    double k;           /* N/m spring constant */
    double F;           /* N force constant */

    MSD(double _m, double _k, double _b, double _F,
        double _v_0, double _x_0);
    
    static int default_data(MSD &);
    int init();
    int shutdown();
    int state_deriv(void);
    int state_integ(void);


    MSD(void); /* not used */

 };



#endif /* MSD_H */