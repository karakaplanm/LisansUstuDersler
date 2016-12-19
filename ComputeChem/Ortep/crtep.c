/**
 * CAIROTEP source.
 * uses CAIRO for ORTEP-III plotting backend.
 *
 * Y.Lin (transi@fzu.edu.cn / aka.yun.lin@gmail.com)
 * Sep. 2007.
 * Instrumentation Analysis and Measurement Center, Fuzhou University.
 * Fuzhou, Fujian, P.R.China.
 *
 * GNU Lesser General Public License (LGPL)
 */

/* ortep.f -- translated by f2c (version 20060506).
   You must link the resulting object file with libf2c:
    on Microsoft Windows system, link with libf2c.lib;
    on Linux or Unix systems, link with .../path/to/libf2c.a -lm
    or, if you install libf2c.a in a standard place, with -lf2c -lm
    -- in that order, at the end of the command line, as in
        cc *.o -lf2c -lm
    Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

        http://www.netlib.org/f2c/libf2c.zip
*/

// :-
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CRTEP_DEBUG_TRACE
#include <time.h>
#endif
// --
// :- Additional header for cairotep:
#include <cairo.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>
// --

#include "f2c.h"

/* Common Block Declarations */

union {
    struct {
    integer ng;
    real a[9], aa[9]    /* was [3][3] */, aarev[9]  /* was [3][3] 
        */, aawrk[9]    /* was [3][3] */, aid[9]    /* was [3][3] 
        */;
    doublereal ain[140], atomid[2000];
    real atoms[6000]    /* was [3][2000] */, bb[9]  /* was [3][3] 
        */, brdr, cd[4000]  /* was [8][500] */, cont[5], d__[390]   
        /* was [3][130] */, da[9]   /* was [3][3] */, dp[260]   
        /* was [2][130] */, disp, edge, fore, fs[864]   /* was [3][3][
        96] */;
    integer in, itilt, kd[2500] /* was [5][500] */, latm, natom, ncd, 
        nj, nj2, nout, nsr, nsym;
    real orgn[3], pac[15]   /* was [3][5] */, pat[9]    /* was [3][3] 
        */, q[9]    /* was [3][3] */, refv[9]   /* was [3][3] 
        */, res[4], rms[5], scal1, scal2, scl, symb[9]  /* was [3][3] 
        */, taper, theta;
    char title[72], title2[72];
    real ts[288]    /* was [3][96] */, view, vt[12] /* was [3][4] */, v1[
        4], v2[3], v3[3], v4[3], v5[3], v6[3], wrkv[9]  /* was [3][3] 
        */, xlng[3], xo[3], xt[3];
    } _1;
    struct {
    integer ng;
    } _2;
} _BLNK__;

#define _BLNK__1 (_BLNK__._1)
#define _BLNK__2 (_BLNK__._2)

struct {
    char chem[4040];
    real ev[1515]   /* was [3][505] */, p[1515] /* was [3][505] */, 
        pa[4545]    /* was [3][3][505] */;
    shortint ident[1010]    /* was [2][505] */;
    integer maxatm;
} parms_;

#define parms_1 parms_

struct {
    integer ned, nque, next, nback;
    char inq[73], que[365000], hque[365000];
} queue_;

#define queue_1 queue_

struct {
    integer npf, ndraw, norient, nvar;
} ns_;

#define ns_1 ns_

struct {
    real xtrans, ytrans;
} trfac_;

#define trfac_1 trfac_

struct {
    real conic[17500]   /* was [7][2500] */, cover[120] /* was [6][20] */;
    integer kc[20], kq[30], nconic, ncover, nqover, nquad;
    real ovmrgn, qover[360] /* was [3][4][30] */, quad[27000]   /* 
        was [9][3000] */, segm[100] /* was [50][2] */;
} olap_;

#define olap_1 olap_

struct {
    char infile__[60];
    integer idraw, iorient, iout;
    char ext[4], atomfi[60];
    real fpaplen;
} dfl_;

#define dfl_1 dfl_

struct {
    integer ixmin, ixmax, iymin, iymax, ixt, iyt;
} ps_;

#define ps_1 ps_

/* Table of constant values */

static integer c__1 = 1;
static doublereal c_b113 = 0.;
static integer c_n3 = -3;
static integer c__104 = 104;
static real c_b235 = 1.f;
static integer c__2 = 2;
static integer c__800 = 800;
static real c_b317 = 0.f;
static integer c__3 = 3;
static integer c_n1 = -1;
static integer c_n2 = -2;
static integer c__65 = 65;
static real c_b469 = 10.f;
static integer c__9 = 9;
static integer c__401 = 401;
static integer c__501 = 501;
static integer c__506 = 506;
static integer c__0 = 0;
static integer c__602 = 602;
static integer c__600 = 600;
static integer c__700 = 700;
static integer c__6 = 6;
static integer c__129 = 129;
static integer c__2999 = 2999;
static integer c__4 = 4;
static real c_b1321 = 1e3f;
static integer c__822 = 822;
static real c_b1817 = 2.f;
static real c_b1928 = 100.f;
static integer c__400 = 400;

// :- Extra vars defined by cairotep:

#define CRTEP_VER "CairoTEP 1.0.2, Based On ORTEP-III 1.0.3"

integer crtep_nmode = 0; // 0: interactive, 1: args.

cairo_surface_t* crtep_surface = NULL;
cairo_t* crtep_cr = NULL;
integer crtep_ntype = 0; // cairo target number, 0: PNG, 1: SVG, 2: PDF, 3: PS, 4: EPS. C (long int).
const char *crtep_ntype_ext[] = {".png", ".svg", ".pdf", ".ps", ".eps"};
char* crtep_infname = NULL; // input file name.
char* crtep_outfname = NULL; // output file name.

real crtep_cw = -1; // canvas width, in inch.
real crtep_ch = -1; // canvas height, in inch.
integer crtep_dpi = 72; // screen resolution.
integer crtep_frag = 1; // 1: fragment path every pen move, 0: lazy.
integer crtep_scale = 0; // v <= 0: no scale, else: v/100.
integer crtep_fg = -1; // foreground.
integer crtep_bg = 0xFFFFFF/*-1*/; // background.
real crtep_pw = -1; // pen width.

#ifdef CRTEP_DEBUG_TRACE
clock_t crtep_ct; // for processing time measurement.
#endif
/*
struct {
    cairo_surface_t* surface;
    cairo_t* cr;
} cairotep;
*/

// --

// :- parse command-line args, params:

// print help information:
void crtep_print_help() {
    printf(CRTEP_VER "\n");
    printf("Contact: Y.Lin (transi@fzu.edu.cn)\n");
    printf("Usage:\n");
    printf("crtep [ins-file [parameters [out-file]]]\n");
    printf("For help: crtep -h\n");
    printf("For version info: crtep -v\n");
    printf("\n");
    printf("Parameters: Colon-separated, positional or keyword arguments.\n");
    printf("   0 back:    plotting backend, [cairo:ps:hpgl], default: cairo\n");
    printf("cairo:\n");
    printf("   1 type:    surface/image type for cairo, [png|svg|pdf|ps|eps], default: [png]\n");
    printf("   2 dpi:     resolution (dpi), default: [72]\n");
    printf("   3 cw:      canvas width (inch)\n");
    printf("   4 ch:      canvas height (inch)\n");
    printf("   5 frag:    fragment path, [yes|no], default: [yes]\n");
    printf("   6 scale:   canvas scale (%%)\n");
    printf("   7 fg:      initial pen color, decimal or hexadecimal, default: [0]\n");
    printf("   8 bg:      canvas background, decimal or hexadecimal, [-1] for transparent, default: [0xFFFFFF]\n");
    printf("   9 pw:      initial pen width (pixel), default: [1]\n");
    printf("ps/hpgl:\n");
    printf("   1 orient:  orient for ps/hpgl document, [portrait|landscape], default: [portrait]\n");
    printf("\n");
    printf("e.g. \"cairo:svg:::no:pw=1.5:fg=#999999\"\n");
    printf("\n");
    printf("Check [http://cairotep.sourceforge.net] for more information.\n");
}
// print version information:
void crtep_print_version() {
    printf(CRTEP_VER "\n");
    printf("Contact: Y.Lin (transi@fzu.edu.cn)\n");
}

// parse parameter:
int crtep_parse_param(char *s) {
    int r = 0; // return 0 if continue.
    static int positional = 1;
    static int iparam = 0;
    //if(!*s) printf("*\n");
    char *eqt = strchr(s, '=');
    if(eqt != NULL) { // it's a keyword parameter.
        positional = 0; // end positional parsing state.
        char *k = s; // set keyword k.
        *eqt = 0; // terminate k.
        s = eqt+1; // set value s.
        // find position:
        if(!strcmp(k, "back")) {
            iparam = 0;
        } else if(!strcmp(k, "type")) {
            iparam = 1;
        } else if(!strcmp(k, "orient")) {
            iparam = 1;
        } else if(!strcmp(k, "dpi")) {
            iparam = 2;
        } else if(!strcmp(k, "cw")) {
            iparam = 3;
        } else if(!strcmp(k, "ch")) {
            iparam = 4;
        } else if(!strcmp(k, "frag")) {
            iparam = 5;
        } else if(!strcmp(k, "scale")) {
            iparam = 6;
        } else if(!strcmp(k, "fg")) {
            iparam = 7;
        } else if(!strcmp(k, "bg")) {
            iparam = 8;
        } else if(!strcmp(k, "pw")) {
            iparam = 9;
        } else {
            printf("Bad Parameter Keyword [%s].\n", k);
            iparam = -1;
        }
    } else {
        if(!positional) {
            printf("Positional Param After Keyword.\n");
            iparam = -1;
            r = 1;
        }
    }
    switch(iparam) {
        case 0: // output backend.
            if(strlen(s) && strcmp(s, "cairo")) {
                if(!strcmp(s, "ps")) {
                    ns_1.ndraw = 2;
                } else if(!strcmp(s, "hpgl")) {
                    ns_1.ndraw = 3;
                } else {
                    printf("Bad Output Backend [%s], Use Default [cairo].\n", s);
                    ns_1.ndraw = 0;
                }
            } else { // value is "cairo" or "".
                ns_1.ndraw = 1;
            }
            break;
        case 1:
            if(ns_1.ndraw == 1) { // cairo surface/image type.
                if(!strlen(s) || !strcmp(s, "png")) {
                    crtep_ntype = 0;
                } else if(!strcmp(s, "svg")) {
                    crtep_ntype = 1;
                } else if(!strcmp(s, "pdf")) {
                    crtep_ntype = 2;
                } else if(!strcmp(s, "ps")) {
                    crtep_ntype = 3;
                } else if(!strcmp(s, "eps")) {
                    crtep_ntype = 4;
                } else {
                    crtep_ntype = 0;
                    printf("Bad Cairo Surface/Image Type [%s], Use Default [png].\n", s);
                }
            } else { // orient for ps and hpgl.
                r = 1; // no further parse.
                if(!strlen(s) || !strcmp(s, "portrait")) {
                    ns_1.norient = 1;
                } else if(!strcmp(s, "landscape")) {
                    ns_1.norient = 2;
                } else {
                    ns_1.norient = 1;
                    printf("Bad Orient Type [%s], Use Default [portrait].\n", s);
                }
            }
            break;
        case 2: // dpi.
            if(strlen(s)) crtep_dpi = atoi(s);
            if(crtep_dpi <= 0) crtep_dpi = 72;
            break;
        case 3: // canvas width.
            if(strlen(s)) crtep_cw = atof(s);
            if(crtep_cw < 0) crtep_cw = 0;
            break;
        case 4: // canvas height.
            if(strlen(s)) crtep_ch = atof(s);
            if(crtep_ch < 0) crtep_ch = 0;
            break;
        case 5: // fragment.
            if(strlen(s)) {
                if(!strcmp(s, "yes") || !strcmp(s, "true")) {
                    crtep_frag = 1;
                } else if(!strcmp(s, "no") || !strcmp(s, "false")) {
                    crtep_frag = 0;
                } else {
                    printf("Bad Frag Value [%s], Use Default [yes].\n", s);
                    crtep_frag = 1;
                }
            }
            break;
        case 6: // scale.
            if(strlen(s)) crtep_scale = atoi(s);
            if(crtep_scale <= 0) crtep_scale = 100;
            break;
        case 7: // foreground.
            if(strlen(s)) {
                if(*s=='#' && strlen(s)>2) {
                    crtep_fg = strtol(s+1, NULL, 16);
                } else {
                    crtep_fg = strtol(s, NULL, 0);
                }
                if(crtep_fg < 0) crtep_fg = -1;
            }
            break;
        case 8: // background.
            if(strlen(s)) {
                if(*s=='#' && strlen(s)>2) { // for bg=#XXXXXX
                    crtep_bg = strtol(s+1, NULL, 16);
                } else { // for bg=d... or bg=0xXXXXXX
                    crtep_bg = strtol(s, NULL, 0);
                }
                if(crtep_bg < 0) crtep_bg = -1;
            }
            break;
        case 9: // pen width.
            if(strlen(s)) {
                crtep_pw = atof(s);
                if(crtep_pw < 0) crtep_pw = -1;
            }
            break;
        default:
            printf("Bad CairoTep Parameter [%d].\n", iparam);
            break;
    }

    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: Parameter %d = %s\n", iparam, s);
    #endif
    if(positional) iparam++;

    return r;
}
// parse command-line arguments:
void crtep_parse_args(int argc, char **argv) {
    if(argc >= 2) {
        if(!strcmp(argv[1], "-h")) {
            crtep_print_help();
            exit(0);
        }
        if(!strcmp(argv[1], "-v")) {
            crtep_print_version();
            exit(0);
        }
        crtep_nmode = 1;
        //int i = 0;
        //while(++i<argc) {
        //  printf("%s\n", argv[i]);
        //  char* arg = argv[i];
        //}
        crtep_infname = argv[1];
        if(argc >= 3) {
            char *params = malloc(strlen(argv[2])+1);
            if(params != NULL) {
                params = strcpy(params, argv[2]);
                char *s, *p; // parameter s and parsing pointer p.
                int r = 0; // store return code by crtep_parse_param.
                for(s=p=params; *p; p++) {
                    if(*p == ':') { // parameters is separated by columns.
                        *p = 0; // overwrited by null, to terminate this parameter.
                        r = crtep_parse_param(s); // parse single parameter, either by position or keyword.
                        s = p+1; // set s the next parameter.
                        if(r) break; // if r > 0, don't continue parsing.
                    }
                }
                // parse the final parameter:
                if(!r) crtep_parse_param(s);
            } else {
                printf("Malloc Failed.\n");
                exit(1);
            }
            if(argc >= 4) {
                crtep_outfname = argv[3];
            }
        }
    } else {
        crtep_nmode = 0;
    }
}
// --

/* ***************************************************************** */

/*     ORTEP-III: Oak Ridge Thermal Ellipsoid Plot Program */
/*     Carroll K. Johnson and Michael N. Burnett */
/*     Oak Ridge National Laboratory */
/*     Version 1.0.3 January 31, 2000 */

/*     Send comments, questions, problems, suggestions, etc. to */
/*     ortep@ornl.gov */

/* ***************************************************************** */
/* Disclaimer of Liability */
/*     This software was prepared as an account of work sponsored by an */
/*     agency of the U.S. Government. Neither the U.S. Government nor */
/*     any agency thereof, or any of their employees, makes any */
/*     warranty, express or implied, or assumes any legal liability or */
/*     responsibility for the accuracy, completeness, or usefulness of */
/*     any information, apparatus, product, or process disclosed, or */
/*     represents that its use would not infringe privately owned rights. */
/* ****************** */
/*     const: */
/*     DM_A=2000(ATOMID,ATOMS) DM_D=500(CD,KD) */
/*     DM_C=2500(COMIC) DM_Q=3000(QUAD) */
/*     DMQU=5000(QUE,HQUE) */
/* ****************** */
/* Main program */ int MAIN__(void)
{
    /* Format strings */
    // :- static char fmt_1[] = "(\002 ORTEP-III Version 1.0.3 Jan. 25, 2000\002)";
    static char fmt_1[] = "(\002 " CRTEP_VER "\002)";
    // --
    static char fmt_4[] = "(18a4)";
    static char fmt_6[] = "(\0021\002,10x,18a4)";
    static char fmt_5[] = "(\0020\002,10x,18a4)";
    static char fmt_2012[] = "(a72)";
    static char fmt_11[] = "(\0020\002,4x,\002((((( INSTRUCTION\002,i5,\002 "
        ")))))\002)";
    static char fmt_12[] = "(i3,i6,7f9.0)";
    static char fmt_33[] = "(i3,6x,5i3,8f6.0)";
    static char fmt_35[] = "(\002 \002,11x,5i3,8f11.5)";
    static char fmt_129[] = "(\0020\002,10x,\002ATOM\002,3x,\002RMS DISPLACE"
        "MENT\002,3x,\002ROW VECTORS, BASED ON REFERENCE\002,17x,\002PROB"
        "ABILITY COVARIANCE MATRIX\002)";
    static char fmt_149[] = "(\0020\002,10x,a6,f10.6,6x,3f12.7,10x,3f12.7)";
    static char fmt_159[] = "(\002 \002,16x,f10.6,6x,3f12.7,10x,3f12.7)";
    static char fmt_389[] = "(\0020\002,10x,\002PLOT LIMITS\002,f6.2,\002 B"
        "Y\002,f6.2,\002 IN.  INCLUDING\002,f6.2,\002  IN. MARGIN\002)";
    static char fmt_399[] = "(\002 \002,10x,\002VIEW DISTANCE\002,f7.3,\002 "
        "INCHES\002)";
    static char fmt_319[] = "(\0020\002,10x,\002REGULAR TITLE AND SYMBOL ROT"
        "ATION IN DEGREES\002,f8.2)";
    static char fmt_313[] = "(\0020\002,10x,\002RETRACE DISPLACEMENT =\002,f"
        "7.4,\002 INCH\002)";
    static char fmt_499[] = "(\0020\002,10x,\002CONTENTS OF ATOMS ARRAY\002/"
        "(15x,10f10.0))";
    static char fmt_503[] = "(\0020\002,10x,\002ORIGIN FOR PROJECTION AXIS I"
        "N CRYSTAL COORD.\002,3f15.6)";
    static char fmt_529[] = "(\0020\002,10x,\002ORTHONORMAL REFERENCE VECTOR"
        "S BASED ON CRYSTAL AXES\002,16x,\002POST-FACTOR TRANSFORMATION M"
        "ATRIX\002/16x,\002X VECTOR\002,8x,\002Y VECTOR\002,8x,\002Z VECT"
        "OR\002)";
    static char fmt_519[] = "(\002 \002,10x,3e16.7,8x,3e16.7)";
    static char fmt_509[] = "(\0020\002,10x,\002ORTHONORMAL WORKING VECTORS "
        "BASED ON CRYSTAL AXES\002,18x,\002POST-FACTOR TRANSFORMATION MAT"
        "RIX\002/16x,\002X VECTOR\002,8x,\002Y VECTOR\002,8x,\002Z VECTO"
        "R\002)";
    static char fmt_609[] = "(\0020\002,10x,\002ORIGIN POINT IN PLOTTER COOR"
        "D.(\002,f6.2,\002 ,\002,f6.2,\002 ) IN.  \002/11x,\002OVERALL SC"
        "ALE =\002,f6.3,\002 INCH/ANGSTROM ELLIPSOID SCALE =\002,f6.3)";

    /* System generated locals */
    integer i__1;
    olist o__1;
    cllist cl__1;
    alist al__1, al__2;
    inlist ioin__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), f_inqu(inlist *), f_clos(cllist *)
        , f_open(olist *), s_rsfe(cilist *), do_fio(integer *, char *, 
        ftnlen), e_rsfe(void);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    integer s_cmp(char *, char *, ftnlen, ftnlen), f_back(alist *), s_rsfi(
        icilist *), e_rsfi(void), s_wsue(cilist *), do_uio(integer *, 
        char *, ftnlen), e_wsue(void), s_rsue(cilist *), e_rsue(void);
    double cos(doublereal), sin(doublereal);
    integer f_rew(alist *);

    /* Local variables */
    static integer i__, j, k, n1, n2;
    static real t1;
    static integer nf;
    static doublereal td;
    extern /* Subroutine */ int f200_(void), f400_(void), f500_(void), f600_(
        void), f700_(void), f800_(void), f900_(void);
    static integer nf1, nj3;
    extern /* Subroutine */ int f1000_(void);
    static integer iic;
    extern /* Subroutine */ int searc_(void);
    static integer isave;
    extern /* Subroutine */ int spare_(integer *), prime_(void), paxes_(
        doublereal *, integer *);
    static real costh, sinth;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *), prelim_(void)
        , exitng_(integer *);
    static logical tmpopn;
    extern /* Subroutine */ int uinput_(integer *, integer *), go2edtr_(void);

    /* Fortran I/O blocks */
    static cilist io___1 = { 0, 6, 0, fmt_1, 0 };
    static cilist io___3 = { 0, 0, 0, fmt_4, 0 };
    static cilist io___5 = { 0, 0, 0, fmt_4, 0 };
    static cilist io___6 = { 0, 0, 0, fmt_6, 0 };
    static cilist io___7 = { 0, 0, 0, fmt_6, 0 };
    static cilist io___8 = { 1, 0, 1, fmt_2012, 0 };
    static icilist io___13 = { 0, queue_1.inq, 0, fmt_12, 73, 1 };
    static cilist io___16 = { 0, 0, 0, 0, 0 };
    static cilist io___17 = { 0, 0, 0, 0, 0 };
    static cilist io___18 = { 0, 0, 0, fmt_11, 0 };
    static icilist io___20 = { 0, queue_1.inq, 0, fmt_33, 73, 1 };
    static cilist io___21 = { 0, 0, 0, 0, 0 };
    static cilist io___22 = { 0, 0, 0, 0, 0 };
    static cilist io___23 = { 0, 0, 0, fmt_35, 0 };
    static cilist io___24 = { 0, 0, 0, 0, 0 };
    static icilist io___25 = { 0, queue_1.inq, 0, fmt_4, 73, 1 };
    static cilist io___26 = { 0, 0, 0, fmt_5, 0 };
    static cilist io___27 = { 0, 0, 0, 0, 0 };
    static cilist io___29 = { 0, 0, 0, fmt_6, 0 };
    static cilist io___30 = { 0, 0, 0, fmt_129, 0 };
    static cilist io___32 = { 0, 0, 0, fmt_149, 0 };
    static cilist io___33 = { 0, 0, 0, fmt_159, 0 };
    static cilist io___35 = { 0, 0, 0, fmt_389, 0 };
    static cilist io___36 = { 0, 0, 0, fmt_399, 0 };
    static cilist io___40 = { 0, 0, 0, fmt_319, 0 };
    static cilist io___41 = { 0, 0, 0, fmt_313, 0 };
    static cilist io___42 = { 0, 0, 0, fmt_499, 0 };
    static cilist io___43 = { 0, 0, 0, fmt_503, 0 };
    static cilist io___44 = { 0, 0, 0, fmt_529, 0 };
    static cilist io___45 = { 0, 0, 0, fmt_519, 0 };
    static cilist io___46 = { 0, 0, 0, fmt_509, 0 };
    static cilist io___47 = { 0, 0, 0, fmt_519, 0 };
    static cilist io___48 = { 0, 0, 0, fmt_609, 0 };


/* *** Drawing Output Options */
/* *** ndraw=0: no drawing output */
/* *** ndraw=1: screen output */
/* *** ndraw=2: Postscript file output */
/* *** ndraw=3: HPGL file output */
/* *** ndraw=9: reserved for future use */
/* *** Logical Unit Numbers *** */
/* *** 15,16 are used in subroutine EDITR */
/* *** 18 (variable iu) is used in subroutine PRELIM */
/* *** NOUT is set in subroutine UINPUT */
    _BLNK__1.in = 3;
    queue_1.ned = 7;
    _BLNK__1.nsr = 8;
    ns_1.npf = 10;

// :- parse args:
    // default:
    ns_1.ndraw = 1;

    extern int xargc;
    extern char** xargv;
    crtep_parse_args(xargc, xargv);

    if(crtep_nmode == 0) {
        s_wsfe(&io___1); // ::
        e_wsfe(); // ::
        uinput_(&_BLNK__1.in, &_BLNK__1.nout); // ::
    } else { // crtep_nmode == 1
        dflts_();
        olist ol;
        ol.oerr = 1;
        ol.ounit = _BLNK__1.in;
        ol.ofnmlen = strlen(crtep_infname);
        ol.ofnm = crtep_infname;
        ol.orl = 0;
        ol.osta = "old";
        ol.oacc = 0;
        ol.ofm = 0;
        ol.oblnk = 0;
        integer i = f_open(&ol);
        if(i != 0) {
            printf("Failed To Open Input File.\n");
            exit(1);
        }
        _BLNK__1.nout = -4;
        if(!crtep_outfname || strlen(crtep_outfname) == 0) {
            crtep_outfname = malloc(strlen(crtep_infname)+5);
            strcpy(crtep_outfname, crtep_infname);
            char* p = crtep_outfname + strlen(crtep_infname) - 1;
            while(p >= crtep_outfname) {
                if(*p == '.') {
                    *p = 0; // terminate.
                    break;
                }
                if(*p == '\\' || *p == '/') break;
                p--;
            }
            if(ns_1.ndraw == 1) {
                strcat(crtep_outfname, crtep_ntype_ext[crtep_ntype]);
            } else if(ns_1.ndraw == 2) {
                strcat(crtep_outfname, ".ps");
            } else if(ns_1.ndraw == 3) {
                strcat(crtep_outfname, ".hpg");
            }
        }
    }
// --

L2:
    prime_();
/* *** open ORTEP scratch file *** */
/* *** if already open, close it first *** */
    ioin__1.inerr = 0;
    ioin__1.inunit = _BLNK__1.nsr;
    ioin__1.infile = 0;
    ioin__1.inex = 0;
    ioin__1.inopen = &tmpopn;
    ioin__1.innum = 0;
    ioin__1.innamed = 0;
    ioin__1.inname = 0;
    ioin__1.inacc = 0;
    ioin__1.inseq = 0;
    ioin__1.indir = 0;
    ioin__1.infmt = 0;
    ioin__1.inform = 0;
    ioin__1.inunf = 0;
    ioin__1.inrecl = 0;
    ioin__1.innrec = 0;
    ioin__1.inblank = 0;
    f_inqu(&ioin__1);
    if (tmpopn) {
    cl__1.cerr = 0;
    cl__1.cunit = _BLNK__1.nsr;
    cl__1.csta = 0;
    f_clos(&cl__1);
    }
    o__1.oerr = 0;
    o__1.ounit = _BLNK__1.nsr;
    o__1.ofnm = 0;
    o__1.orl = 0;
    o__1.osta = "scratch";
    o__1.oacc = 0;
    o__1.ofm = "unformatted";
    o__1.oblnk = 0;
    f_open(&o__1);
/* *** open a temporary file - needed by the editor *** */
/* *** if already open, close it first *** */
    ioin__1.inerr = 0;
    ioin__1.inunit = queue_1.ned;
    ioin__1.infile = 0;
    ioin__1.inex = 0;
    ioin__1.inopen = &tmpopn;
    ioin__1.innum = 0;
    ioin__1.innamed = 0;
    ioin__1.inname = 0;
    ioin__1.inacc = 0;
    ioin__1.inseq = 0;
    ioin__1.indir = 0;
    ioin__1.infmt = 0;
    ioin__1.inform = 0;
    ioin__1.inunf = 0;
    ioin__1.inrecl = 0;
    ioin__1.innrec = 0;
    ioin__1.inblank = 0;
    f_inqu(&ioin__1);
    if (tmpopn) {
    cl__1.cerr = 0;
    cl__1.cunit = queue_1.ned;
    cl__1.csta = 0;
    f_clos(&cl__1);
    }
    o__1.oerr = 0;
    o__1.ounit = queue_1.ned;
    o__1.ofnm = 0;
    o__1.orl = 0;
    o__1.osta = "scratch";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
/*     ***** READ JOB TITLE CARD ***** */
    io___3.ciunit = _BLNK__1.in;
    s_rsfe(&io___3);
    for (i__ = 1; i__ <= 18; ++i__) {
    do_fio(&c__1, _BLNK__1.title + (i__ - 1 << 2), (ftnlen)4);
    }
    e_rsfe();
    io___5.ciunit = queue_1.ned;
    s_wsfe(&io___5);
    for (i__ = 1; i__ <= 18; ++i__) {
    do_fio(&c__1, _BLNK__1.title + (i__ - 1 << 2), (ftnlen)4);
    }
    e_wsfe();
    if (_BLNK__1.nout >= 0) {
    io___6.ciunit = _BLNK__1.nout;
    s_wsfe(&io___6);
    for (i__ = 1; i__ <= 18; ++i__) {
        do_fio(&c__1, _BLNK__1.title + (i__ - 1 << 2), (ftnlen)4);
    }
    e_wsfe();
    }
/* L5: */
    prelim_();
    if (_BLNK__1.nout >= 0) {
    io___7.ciunit = _BLNK__1.nout;
    s_wsfe(&io___7);
    for (i__ = 1; i__ <= 18; ++i__) {
        do_fio(&c__1, _BLNK__1.title + (i__ - 1 << 2), (ftnlen)4);
    }
    e_wsfe();
    }
/*     ***** LOAD INSTRUCTION QUE ***** */
    queue_1.nque = 0;
L2005:
    ++queue_1.nque;
L2010:
    io___8.ciunit = _BLNK__1.in;
    i__1 = s_rsfe(&io___8);
    if (i__1 != 0) {
    goto L100001;
    }
    i__1 = do_fio(&c__1, queue_1.que + (queue_1.nque - 1) * 73, (ftnlen)73);
    if (i__1 != 0) {
    goto L100001;
    }
    i__1 = e_rsfe();
L100001:
    if (i__1 < 0) {
    goto L2015;
    }
    if (i__1 > 0) {
    goto L3000;
    }
    if (*(unsigned char *)&queue_1.que[(queue_1.nque - 1) * 73] == '#') {
    goto L2010;
    }
    s_copy(queue_1.hque + (queue_1.nque - 1) * 73, queue_1.que + (
        queue_1.nque - 1) * 73, (ftnlen)73, (ftnlen)73);
    if (s_cmp(queue_1.que + ((queue_1.nque - 1) * 73 + 3), "    -2", (ftnlen)
        6, (ftnlen)6) == 0) {
    goto L2020;
    }
    if ((real) queue_1.nque < 5e3f) {
    goto L2005;
    }
    goto L2020;
L2015:
    --queue_1.nque;
/*     ***** REPOSITION TO POINT BEFORE EOF ***** */
    al__1.aerr = 0;
    al__1.aunit = _BLNK__1.in;
    f_back(&al__1);
L2020:
    queue_1.nback = queue_1.nque;
    queue_1.next = 1;
    isave = 0;
    goto L507;
L7:
    isave = 0;
/*     ***** ZERO AIN ARRAY ***** */
L8:
    for (j = 1; j <= 140; ++j) {
/* L10: */
    _BLNK__1.ain[j - 1] = 0.f;
    }
/* L11: */
/* L12: */
/* L13: */
/* L14: */
/*     ***** READ NEW INSTRUCTION CARD ***** */
    _BLNK__1.ncd = 0;
    n1 = -6;
L16:
    n1 += 7;
    n2 = n1 + 6;
    if (isave >= 0) {
    goto L18;
    } else {
    goto L22;
    }
L18:
    s_copy(queue_1.inq, queue_1.que + (queue_1.next - 1) * 73, (ftnlen)73, (
        ftnlen)73);
    ++queue_1.next;
    s_rsfi(&io___13);
    do_fio(&c__1, (char *)&iic, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&nf, (ftnlen)sizeof(integer));
    i__1 = n2;
    for (i__ = n1; i__ <= i__1; ++i__) {
    do_fio(&c__1, (char *)&_BLNK__1.ain[i__ - 1], (ftnlen)sizeof(
        doublereal));
    }
    e_rsfi();
    if (isave <= 0) {
    goto L24;
    } else {
    goto L20;
    }
L20:
    io___16.ciunit = _BLNK__1.nsr;
    s_wsue(&io___16);
    do_uio(&c__1, (char *)&iic, (ftnlen)sizeof(integer));
    do_uio(&c__1, (char *)&nf, (ftnlen)sizeof(integer));
    i__1 = n2;
    for (i__ = n1; i__ <= i__1; ++i__) {
    do_uio(&c__1, (char *)&_BLNK__1.ain[i__ - 1], (ftnlen)sizeof(
        doublereal));
    }
    e_wsue();
    goto L24;
L22:
    io___17.ciunit = _BLNK__1.nsr;
    s_rsue(&io___17);
    do_uio(&c__1, (char *)&iic, (ftnlen)sizeof(integer));
    do_uio(&c__1, (char *)&nf, (ftnlen)sizeof(integer));
    i__1 = n2;
    for (i__ = n1; i__ <= i__1; ++i__) {
    do_uio(&c__1, (char *)&_BLNK__1.ain[i__ - 1], (ftnlen)sizeof(
        doublereal));
    }
    e_rsue();
    if (iic >= 0) {
    goto L24;
    } else {
    goto L7;
    }
L24:
    if (n1 - 1 <= 0) {
    goto L26;
    } else {
    goto L30;
    }
L26:
    if (_BLNK__1.nout >= 0) {
    io___18.ciunit = _BLNK__1.nout;
    s_wsfe(&io___18);
    do_fio(&c__1, (char *)&nf, (ftnlen)sizeof(integer));
    e_wsfe();
    }
    nf1 = nf;
    if (nf1 < 0) {
    goto L28;
    } else if (nf1 == 0) {
    goto L8;
    } else {
    goto L30;
    }
/* *** run editor? */
L28:
    // :- go2edtr_();
    if(crtep_nmode == 0) {
        go2edtr_();
    } else {
        #ifdef CRTEP_DEBUG_TRACE
        printf("Debug: O.K.\n");
        #endif
    }
    // --
    if (queue_1.next < queue_1.nque) {
    goto L8;
    }
    if (nf1 + 2 <= 0) {
    goto L2;
    } else {
    goto L3000;
    }
L30:
/* CC   IF (NOUT.GE.0) */
/* CC  &WRITE (NOUT,14)(AIN(I),I=N1,N2) */
L32:
    ++iic;
    switch (iic) {
    case 1:  goto L90;
    case 2:  goto L16;
    case 3:  goto L38;
    case 4:  goto L50;
    }
/* L33: */
/* L34: */
/* L35: */
/*     ***** READ FORMAT 2 TRAILER CARDS ***** */
L38:
    ++_BLNK__1.ncd;
    if (isave >= 0) {
    goto L40;
    } else {
    goto L44;
    }
L40:
    s_copy(queue_1.inq, queue_1.que + (queue_1.next - 1) * 73, (ftnlen)73, (
        ftnlen)73);
    ++queue_1.next;
    s_rsfi(&io___20);
    do_fio(&c__1, (char *)&iic, (ftnlen)sizeof(integer));
    for (i__ = 1; i__ <= 5; ++i__) {
    do_fio(&c__1, (char *)&_BLNK__1.kd[i__ + _BLNK__1.ncd * 5 - 6], (
        ftnlen)sizeof(integer));
    }
    for (i__ = 1; i__ <= 8; ++i__) {
    do_fio(&c__1, (char *)&_BLNK__1.cd[i__ + (_BLNK__1.ncd << 3) - 9], (
        ftnlen)sizeof(real));
    }
    e_rsfi();
    if (isave <= 0) {
    goto L46;
    } else {
    goto L42;
    }
L42:
    io___21.ciunit = _BLNK__1.nsr;
    s_wsue(&io___21);
    do_uio(&c__1, (char *)&iic, (ftnlen)sizeof(integer));
    for (i__ = 1; i__ <= 5; ++i__) {
    do_uio(&c__1, (char *)&_BLNK__1.kd[i__ + _BLNK__1.ncd * 5 - 6], (
        ftnlen)sizeof(integer));
    }
    for (i__ = 1; i__ <= 8; ++i__) {
    do_uio(&c__1, (char *)&_BLNK__1.cd[i__ + (_BLNK__1.ncd << 3) - 9], (
        ftnlen)sizeof(real));
    }
    e_wsue();
    goto L46;
L44:
    io___22.ciunit = _BLNK__1.nsr;
    s_rsue(&io___22);
    do_uio(&c__1, (char *)&iic, (ftnlen)sizeof(integer));
    for (i__ = 1; i__ <= 5; ++i__) {
    do_uio(&c__1, (char *)&_BLNK__1.kd[i__ + _BLNK__1.ncd * 5 - 6], (
        ftnlen)sizeof(integer));
    }
    for (i__ = 1; i__ <= 8; ++i__) {
    do_uio(&c__1, (char *)&_BLNK__1.cd[i__ + (_BLNK__1.ncd << 3) - 9], (
        ftnlen)sizeof(real));
    }
    e_rsue();
L46:
    if (_BLNK__1.nout >= 0) {
    io___23.ciunit = _BLNK__1.nout;
    s_wsfe(&io___23);
    for (i__ = 1; i__ <= 5; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.kd[i__ + _BLNK__1.ncd * 5 - 6], (
            ftnlen)sizeof(integer));
    }
    for (i__ = 1; i__ <= 8; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.cd[i__ + (_BLNK__1.ncd << 3) - 9],
             (ftnlen)sizeof(real));
    }
    e_wsfe();
    }
    goto L32;
/*     ***** READ FORMAT 3 TRAILER CARD ***** */
L50:
    if (isave >= 0) {
    goto L54;
    } else {
    goto L52;
    }
L52:
    io___24.ciunit = _BLNK__1.nsr;
    s_rsue(&io___24);
    for (i__ = 1; i__ <= 18; ++i__) {
    do_uio(&c__1, _BLNK__1.title2 + (i__ - 1 << 2), (ftnlen)4);
    }
    e_rsue();
    goto L55;
L54:
    s_copy(queue_1.inq, queue_1.que + (queue_1.next - 1) * 73, (ftnlen)73, (
        ftnlen)73);
    ++queue_1.next;
    s_rsfi(&io___25);
    for (i__ = 1; i__ <= 18; ++i__) {
    do_fio(&c__1, _BLNK__1.title2 + (i__ - 1 << 2), (ftnlen)4);
    }
    e_rsfi();
L55:
    if (_BLNK__1.nout >= 0) {
    io___26.ciunit = _BLNK__1.nout;
    s_wsfe(&io___26);
    for (i__ = 1; i__ <= 18; ++i__) {
        do_fio(&c__1, _BLNK__1.title2 + (i__ - 1 << 2), (ftnlen)4);
    }
    e_wsfe();
    }
    if (isave <= 0) {
    goto L90;
    } else {
    goto L56;
    }
L56:
    io___27.ciunit = _BLNK__1.nsr;
    s_wsue(&io___27);
    for (i__ = 1; i__ <= 18; ++i__) {
    do_uio(&c__1, _BLNK__1.title2 + (i__ - 1 << 2), (ftnlen)4);
    }
    e_wsue();
/*     ***** EXECUTE INSTRUCTION ***** */
L90:
    _BLNK__1.nj = nf1 / 100;
    _BLNK__1.nj2 = nf1 - _BLNK__1.nj * 100;
    nj3 = _BLNK__1.nj2 % 10;
    if (_BLNK__1.nj - 12 >= 0) {
    goto L92;
    } else {
    goto L98;
    }
L92:
    spare_(&nf1);
    if (_BLNK__1.ng != 0) {
    goto L94;
    } else {
    goto L8;
    }
L94:
    erpnt_(&c_b113, &nf1);
    goto L8;
/*     ******BRANCH TABLE FOR FUNCTION TYPES****** */
L98:
    switch (_BLNK__1.nj) {
    case 1:  goto L100;
    case 2:  goto L200;
    case 3:  goto L300;
    case 4:  goto L400;
    case 5:  goto L500;
    case 6:  goto L600;
    case 7:  goto L700;
    case 8:  goto L800;
    case 9:  goto L900;
    case 10:  goto L1000;
    case 11:  goto L1100;
    }
/*     *******100 INSTRUCTIONS-STRUCTURE ANALYSIS FUNCTIONS******* */
L100:
    switch (_BLNK__1.nj2) {
    case 1:  goto L101;
    case 2:  goto L101;
    case 3:  goto L104;
    case 4:  goto L104;
    case 5:  goto L101;
    case 6:  goto L101;
    case 7:  goto L94;
    }
L101:
    searc_();
    goto L8;
/*     ***** ANISOTROPIC TEMP FACTOR OUTPUT ***** */
L104:
    i__1 = _BLNK__1.natom;
    for (i__ = 1; i__ <= i__1; ++i__) {
    if (i__ % 14 - 1 != 0) {
        goto L134;
    } else {
        goto L114;
    }
L114:
    if (_BLNK__1.nout >= 0) {
        io___29.ciunit = _BLNK__1.nout;
        s_wsfe(&io___29);
        for (j = 1; j <= 18; ++j) {
        do_fio(&c__1, _BLNK__1.title + (j - 1 << 2), (ftnlen)4);
        }
        e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
        io___30.ciunit = _BLNK__1.nout;
        s_wsfe(&io___30);
        e_wsfe();
    }
L134:
    td = (real) i__ * 1e5f + 55501.;
    paxes_(&td, &c_n3);
    if (_BLNK__1.ng != 0) {
        goto L144;
    } else {
        goto L154;
    }
L144:
    erpnt_(&td, &c__104);
/* L149: */
L154:
    if (_BLNK__1.nout >= 0) {
        io___32.ciunit = _BLNK__1.nout;
        s_wsfe(&io___32);
        do_fio(&c__1, parms_1.chem + (i__ - 1 << 3), (ftnlen)8);
        do_fio(&c__1, (char *)&_BLNK__1.rms[0], (ftnlen)sizeof(real));
        for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.pac[j - 1], (ftnlen)sizeof(
            real));
        }
        for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.q[j - 1], (ftnlen)sizeof(real)
            );
        }
        e_wsfe();
    }
/* L164: */
    if (_BLNK__1.nout >= 0) {
        io___33.ciunit = _BLNK__1.nout;
        s_wsfe(&io___33);
        for (k = 2; k <= 3; ++k) {
        do_fio(&c__1, (char *)&_BLNK__1.rms[k - 1], (ftnlen)sizeof(
            real));
        for (j = 1; j <= 3; ++j) {
            do_fio(&c__1, (char *)&_BLNK__1.pac[j + k * 3 - 4], (
                ftnlen)sizeof(real));
        }
        for (j = 1; j <= 3; ++j) {
            do_fio(&c__1, (char *)&_BLNK__1.q[j + k * 3 - 4], (ftnlen)
                sizeof(real));
        }
        }
        e_wsfe();
    }
    }
    goto L8;
/*     *******200 INSTRUCTIONS-PLOTTER CONTROL FUNCTIONS******* */
L200:
    f200_();
    goto L8;
/*     *******300 INSTRUCTIONS-DRAWING CONTROL FUNCTIONS******* */
L300:
    switch (_BLNK__1.nj2) {
    case 1:  goto L301;
    case 2:  goto L302;
    case 3:  goto L303;
    case 4:  goto L304;
    case 5:  goto L94;
    }
/*     *******PLOT DIMENSIONS******* */
L301:
    if (_BLNK__1.ain[0] <= 0.) {
    goto L321;
    } else {
    goto L311;
    }
L311:
    _BLNK__1.xlng[0] = _BLNK__1.ain[0];
L321:
    if (_BLNK__1.ain[1] <= 0.) {
    goto L341;
    } else {
    goto L331;
    }
L331:
    _BLNK__1.xlng[1] = _BLNK__1.ain[1];
L341:
    if (_BLNK__1.ain[2] >= 0.) {
    goto L351;
    } else {
    goto L361;
    }
L351:
    _BLNK__1.view = _BLNK__1.ain[2];
L361:
    if (_BLNK__1.ain[3] <= 0.) {
    goto L381;
    } else {
    goto L371;
    }
L371:
    _BLNK__1.brdr = _BLNK__1.ain[3];
L381:
    if (_BLNK__1.nout >= 0) {
    io___35.ciunit = _BLNK__1.nout;
    s_wsfe(&io___35);
    do_fio(&c__1, (char *)&_BLNK__1.xlng[0], (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&_BLNK__1.xlng[1], (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&_BLNK__1.brdr, (ftnlen)sizeof(real));
    e_wsfe();
    }
L391:
    if (_BLNK__1.nout >= 0) {
    io___36.ciunit = _BLNK__1.nout;
    s_wsfe(&io___36);
    do_fio(&c__1, (char *)&_BLNK__1.view, (ftnlen)sizeof(real));
    e_wsfe();
    }
    goto L8;
/*     *******LEGEND ROTATION******* */
L302:
    _BLNK__1.theta = _BLNK__1.ain[0];
    t1 = _BLNK__1.theta * .01745329252f;
    costh = cos(t1);
    sinth = sin(t1);
    for (j = 1; j <= 9; ++j) {
/* L312: */
    _BLNK__1.symb[j - 1] = 0.f;
    }
    _BLNK__1.symb[0] = costh;
    _BLNK__1.symb[4] = costh;
    _BLNK__1.symb[8] = 1.f;
    _BLNK__1.symb[1] = sinth;
    _BLNK__1.symb[3] = -sinth;
    if (_BLNK__1.nout >= 0) {
    io___40.ciunit = _BLNK__1.nout;
    s_wsfe(&io___40);
    do_fio(&c__1, (char *)&_BLNK__1.theta, (ftnlen)sizeof(real));
    e_wsfe();
    }
    goto L8;
/*     ***** RETRACE DISPLACEMENT ***** */
L303:
    _BLNK__1.disp = _BLNK__1.ain[0];
    if (_BLNK__1.nout >= 0) {
    io___41.ciunit = _BLNK__1.nout;
    s_wsfe(&io___41);
    do_fio(&c__1, (char *)&_BLNK__1.disp, (ftnlen)sizeof(real));
    e_wsfe();
    }
    goto L8;
/*     ***** change resolution (smoothness) of ellipses ***** */
L304:
    _BLNK__1.res[0] = _BLNK__1.ain[0] * .75f;
    _BLNK__1.res[1] = _BLNK__1.res[0] * .5f;
    _BLNK__1.res[2] = _BLNK__1.res[1] * .25f;
    goto L8;
/*     *******400 INSTRUCTIONS-ATOM LIST FUNCTIONS******* */
L400:
    switch (_BLNK__1.nj2) {
    case 1:  goto L401;
    case 2:  goto L401;
    case 3:  goto L401;
    case 4:  goto L401;
    case 5:  goto L401;
    case 6:  goto L401;
    case 7:  goto L401;
    case 8:  goto L490;
    case 9:  goto L94;
    case 10:  goto L410;
    case 11:  goto L401;
    case 12:  goto L401;
    case 13:  goto L401;
    case 14:  goto L401;
    case 15:  goto L401;
    case 16:  goto L401;
    case 17:  goto L401;
    case 18:  goto L94;
    }
L401:
    f400_();
    goto L490;
L410:
    _BLNK__1.latm = 0;
    for (i__ = 1; i__ <= 2000; ++i__) {
    _BLNK__1.atomid[i__ - 1] = 0.f;
    for (j = 1; j <= 3; ++j) {
/* L420: */
        _BLNK__1.atoms[j + i__ * 3 - 4] = 0.f;
    }
    }
L490:
    if (_BLNK__1.latm <= 0) {
    goto L8;
    } else {
    goto L491;
    }
L491:
    if (_BLNK__1.nout >= 0) {
    io___42.ciunit = _BLNK__1.nout;
    s_wsfe(&io___42);
    i__1 = _BLNK__1.latm;
    for (i__ = 1; i__ <= i__1; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.atomid[i__ - 1], (ftnlen)sizeof(
            doublereal));
    }
    e_wsfe();
    }
    goto L8;
/*     *******500 INSTRUCTIONS-CARTESIAN COORDINATE SYSTEM FUNCTIONS******* */
L500:
    f500_();
    if (_BLNK__1.nout >= 0) {
    io___43.ciunit = _BLNK__1.nout;
    s_wsfe(&io___43);
    for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.orgn[j - 1], (ftnlen)sizeof(real))
            ;
    }
    e_wsfe();
    }
    if ((i__1 = nj3 - 3) < 0) {
    goto L507;
    } else if (i__1 == 0) {
    goto L539;
    } else {
    goto L504;
    }
L504:
    if (nj3 - 6 != 0) {
    goto L601;
    } else {
    goto L507;
    }
L507:
    if (_BLNK__1.nout >= 0) {
    io___44.ciunit = _BLNK__1.nout;
    s_wsfe(&io___44);
    e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
    io___45.ciunit = _BLNK__1.nout;
    s_wsfe(&io___45);
    for (j = 1; j <= 3; ++j) {
        for (i__ = 1; i__ <= 3; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.refv[j + i__ * 3 - 4], (
            ftnlen)sizeof(real));
        }
        for (i__ = 1; i__ <= 3; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.aarev[j + i__ * 3 - 4], (
            ftnlen)sizeof(real));
        }
    }
    e_wsfe();
    }
    goto L8;
/* L509: */
L539:
    if (_BLNK__1.nout >= 0) {
    io___46.ciunit = _BLNK__1.nout;
    s_wsfe(&io___46);
    e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
    io___47.ciunit = _BLNK__1.nout;
    s_wsfe(&io___47);
    for (j = 1; j <= 3; ++j) {
        for (i__ = 1; i__ <= 3; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.wrkv[j + i__ * 3 - 4], (
            ftnlen)sizeof(real));
        }
        for (i__ = 1; i__ <= 3; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.aawrk[j + i__ * 3 - 4], (
            ftnlen)sizeof(real));
        }
    }
    e_wsfe();
    }
    goto L8;
/*     *******600 INSTRUCTIONS-PLOT CENTERING FUNCTIONS******* */
L600:
    f600_();
L601:
    if (_BLNK__1.nout >= 0) {
    io___48.ciunit = _BLNK__1.nout;
    s_wsfe(&io___48);
    do_fio(&c__1, (char *)&_BLNK__1.xo[0], (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&_BLNK__1.xo[1], (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&_BLNK__1.scal1, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&_BLNK__1.scal2, (ftnlen)sizeof(real));
    e_wsfe();
    }
    goto L391;
/*     *******700 INSTRUCTIONS-ELLIPSOID AND SYMBOL PLOT FUNCTIONS******* */
/*     ********FILL OUT DETAILS FOR SPECIAL MODELS******** */
L700:
    switch (nj3) {
    case 1:  goto L701;
    case 2:  goto L702;
    case 3:  goto L704;
    case 4:  goto L705;
    case 5:  goto L709;
    case 6:  goto L7006;
    case 7:  goto L94;
    }
L7006:
    _BLNK__1.ain[2] = 1.f;
    goto L703;
L701:
    _BLNK__1.ain[2] = 8.f;
    goto L703;
L702:
    _BLNK__1.ain[2] = 0.f;
L703:
    _BLNK__1.ain[0] = 4.f;
    _BLNK__1.ain[1] = 0.f;
    _BLNK__1.ain[3] = 0.f;
    goto L709;
L704:
    _BLNK__1.ain[0] = 3.f;
    _BLNK__1.ain[1] = -5.f;
    goto L706;
L705:
    _BLNK__1.ain[0] = 1.f;
    _BLNK__1.ain[1] = 0.f;
L706:
    _BLNK__1.ain[2] = 1.f;
    _BLNK__1.ain[3] = 5.f;
L709:
    f700_();
    goto L8;
/*     *******800 INSTRUCTIONS-BOND FUNCTIONS******* */
L800:
    f800_();
    goto L8;
/*     *******900 INSTRUCTIONS-TITLE FUNCTIONS******* */
L900:
    f900_();
    goto L8;
/*     *******1000 INSTRUCTIONS-OVERLAP FUNCTIONS******* */
L1000:
    f1000_();
    goto L8;
/*     *******1100 INSTRUCTIONS-SAVE SEQUENCE FUNCTIONS******* */
L1100:
    if ((i__1 = _BLNK__1.nj2 - 2) < 0) {
    goto L1101;
    } else if (i__1 == 0) {
    goto L1102;
    } else {
    goto L1103;
    }
L1101:
    isave = 1;
    goto L1104;
L1102:
    isave = 0;
    j = -1;
/* CC   END FILE NSR */
    goto L1104;
L1103:
    isave = -1;
L1104:
    al__2.aerr = 0;
    al__2.aunit = _BLNK__1.nsr;
    f_rew(&al__2);
    goto L8;
L3000:
    exitng_(&_BLNK__1.ng);
    return 0;
} /* MAIN__ */

doublereal arccos_(real *x)
{
    /* System generated locals */
    real ret_val;

    /* Builtin functions */
    double r_sign(real *, real *), sqrt(doublereal), atan(doublereal);

/*     ARCCOS(X) IN DEGREES */
    if (1.f - dabs(*x) >= 0.f) {
    goto L2;
    } else {
    goto L1;
    }
L1:
    *x = r_sign(&c_b235, x);
L2:
    if (*x < 0.f) {
    goto L3;
    } else if (*x == 0) {
    goto L4;
    } else {
    goto L5;
    }
L3:
    ret_val = atan(sqrt(1.f - *x * *x) / *x) * 57.29577951f + 180.f;
    goto L6;
L4:
    ret_val = 90.f;
    goto L6;
L5:
    ret_val = atan(sqrt(1.f - *x * *x) / *x) * 57.29577951f;
L6:
    return ret_val;
} /* arccos_ */

/* Subroutine */ int atom_(doublereal *qa, real *z__)
{
    /* Builtin functions */
    double d_mod(doublereal *, doublereal *);

    /* Local variables */
    static integer j, k;
    static real x[3];
    static integer k1, k2, k3;
    static doublereal ta;
    static integer ks, kt;
    static doublereal d100k;
    static integer ksym;

/*     ATOM COORDINATE SUBROUTINE */
    /* Parameter adjustments */
    --z__;

    /* Function Body */
    d100k = 1e5f;
    k = (integer) (*qa / d100k);
    if (k <= 0) {
    goto L109;
    } else {
    goto L117;
    }
L109:
    x[0] = 0.f;
    x[1] = 0.f;
    x[2] = 0.f;
    goto L125;
L117:
    if (k - _BLNK__1.natom <= 0) {
    goto L119;
    } else {
    goto L503;
    }
L503:
    _BLNK__1.ng = 5;
    goto L325;
L119:
    for (j = 1; j <= 3; ++j) {
/* L123: */
    x[j - 1] = parms_1.p[j + k * 3 - 4];
    }
L125:
    ta = abs(*qa);
    ksym = (integer) d_mod(&ta, &d100k);
    kt = ksym / 100;
    ks = ksym - kt * 100;
    if (ks - _BLNK__1.nsym <= 0) {
    goto L203;
    } else {
    goto L403;
    }
L403:
    _BLNK__1.ng = 4;
    goto L325;
L203:
    if (ks < 0) {
    goto L403;
    } else if (ks == 0) {
    goto L205;
    } else {
    goto L213;
    }
L205:
    z__[1] = x[0];
    z__[2] = x[1];
    z__[3] = x[2];
    goto L311;
L213:
    for (k = 1; k <= 3; ++k) {
    z__[k] = _BLNK__1.ts[k + ks * 3 - 4];
    for (j = 1; j <= 3; ++j) {
/* L223: */
        z__[k] += _BLNK__1.fs[j + (k + ks * 3) * 3 - 13] * x[j - 1];
    }
    }
L311:
    if (kt < 0) {
    goto L403;
    } else if (kt == 0) {
    goto L325;
    } else {
    goto L313;
    }
L313:
    if (kt - 555 != 0) {
    goto L317;
    } else {
    goto L315;
    }
L315:
    ksym = ks;
    goto L325;
L317:
    k1 = kt / 100;
    k = kt - k1 * 100;
    k2 = k / 10;
    k3 = k - k2 * 10;
    z__[1] += (real) (k1 - 5);
    z__[2] += (real) (k2 - 5);
    z__[3] += (real) (k3 - 5);
L325:
    return 0;
} /* atom_ */

/* Subroutine */ int axeqb_(real *a1, real *x, real *b1, integer *jjj)
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2, r__3;

    /* Local variables */
    static real a[9]    /* was [3][3] */, b[9]  /* was [3][3] */;
    static integer i__, j, k, l, m, n;
    static real r__, s;
    static integer nv;
    static real tem;
    static integer ipo;

/*     ***** SOLUTION OF MATRIX EQUATION AX=B FOR X ***** */
/*     ***** USES METHOD OF TRIANGULAR ELIMINATION ***** */
/*     ***** B AND X HAVE DIMENSIONS (3,JJJ),A IS ALWAYS (3,3) */
/*     ***** TO INVERT A MAKE B 3 BY 3 IDENITY MATRIX ***** */
    /* Parameter adjustments */
    b1 -= 4;
    x -= 4;
    a1 -= 4;

    /* Function Body */
    nv = *jjj;
/*     ***** TRANSFER DATA ***** */
    for (i__ = 1; i__ <= 3; ++i__) {
    for (j = 1; j <= 3; ++j) {
        if (nv - j >= 0) {
        goto L1;
        } else {
        goto L2;
        }
L1:
        b[i__ + j * 3 - 4] = b1[i__ + j * 3];
L2:
        a[i__ + j * 3 - 4] = a1[i__ + j * 3];
    }
    }
/*     ***** TRIANGULARIZE MATRIX A ***** */
    for (i__ = 1; i__ <= 2; ++i__) {
    s = 0.f;
    for (j = i__; j <= 3; ++j) {
        r__ = (r__1 = a[j + i__ * 3 - 4], dabs(r__1));
        if (r__ - s >= 0.f) {
        goto L3;
        } else {
        goto L4;
        }
L3:
        s = r__;
        l = j;
L4:
        ;
    }
    if (l - i__ != 0) {
        goto L5;
    } else {
        goto L10;
    }
L5:
    for (j = i__; j <= 3; ++j) {
        s = a[i__ + j * 3 - 4];
        a[i__ + j * 3 - 4] = a[l + j * 3 - 4];
/* L6: */
        a[l + j * 3 - 4] = s;
    }
    i__1 = nv;
    for (j = 1; j <= i__1; ++j) {
        s = b[i__ + j * 3 - 4];
        b[i__ + j * 3 - 4] = b[l + j * 3 - 4];
/* L8: */
        b[l + j * 3 - 4] = s;
    }
L10:
    tem = a[i__ + i__ * 3 - 4];
    if (tem != 0.f) {
        goto L11;
    } else {
        goto L17;
    }
L11:
    ipo = i__ + 1;
    for (j = ipo; j <= 3; ++j) {
        if (a[j + i__ * 3 - 4] != 0.f) {
        goto L12;
        } else {
        goto L16;
        }
L12:
        s = a[j + i__ * 3 - 4] / tem;
        a[j + i__ * 3 - 4] = 0.f;
        for (k = ipo; k <= 3; ++k) {
/* L13: */
        a[j + k * 3 - 4] -= a[i__ + k * 3 - 4] * s;
        }
        i__1 = nv;
        for (k = 1; k <= i__1; ++k) {
/* L15: */
        b[j + k * 3 - 4] -= b[i__ + k * 3 - 4] * s;
        }
L16:
        ;
    }
L17:
    ;
    }
/*     ***** MODIFY SINGULAR MATRIX ***** */
    for (i__ = 1; i__ <= 3; ++i__) {
    if (a[i__ + i__ * 3 - 4] != 0.f) {
        goto L20;
    } else {
        goto L19;
    }
L19:
/* Computing MAX */
/* Computing MAX */
    r__3 = max(a[0],a[4]);
    r__1 = 1e-25f, r__2 = dmax(r__3,a[8]) * 1e-15f;
    a[i__ + i__ * 3 - 4] = dmax(r__1,r__2);
L20:
    ;
    }
    i__1 = nv;
    for (k = 1; k <= i__1; ++k) {
    for (i__ = 1; i__ <= 3; ++i__) {
        n = 4 - i__;
        m = n + 1;
        tem = b[n + k * 3 - 4];
        if (3 - m >= 0) {
        goto L21;
        } else {
        goto L23;
        }
L21:
        for (j = m; j <= 3; ++j) {
/* L22: */
        tem -= a[n + j * 3 - 4] * b[j + k * 3 - 4];
        }
L23:
        b[n + k * 3 - 4] = tem / a[n + n * 3 - 4];
/* L24: */
        x[n + k * 3] = b[n + k * 3 - 4];
    }
    }
    return 0;
} /* axeqb_ */

/* Subroutine */ int axes_(real *u, real *v, real *x, integer *itype)
{
    static integer i__, j;
    static real w[9]    /* was [3][3] */;
    static integer ic, it;
    extern /* Subroutine */ int norm_(real *, real *, real *, integer *), 
        unity_(real *, real *, integer *);

/*     ***** STORE THREE ORTHOGONAL VECTORS EACH 1 ANGSTROM LONG ***** */
/*     ***** ITYPE .GT.0 FOR CARTESIAN,.LE.0 FOR TRICLINIC ***** */
/*     *****IABS(ITYPE)=1 W(1)=U,W(2)=(UXV),W(3)=UX(UXV) ***** */
/*     *****IABS(ITYPE)=2 W(1)=U,W(2)=(UXV)XU,W(3)=(UXV) ***** */
/*     ***** ITYPE=0 W(1)=A,W(2)=(AXB)XA,W(3)=(AXB), ABC=CELL VECTORS *** */
    /* Parameter adjustments */
    x -= 4;
    --v;
    --u;

    /* Function Body */
    it = *itype;
    if (it != 0) {
    goto L115;
    } else {
    goto L105;
    }
L105:
    u[1] = 1.f;
    u[2] = 0.f;
    u[3] = 0.f;
    v[1] = 0.f;
    v[2] = 1.f;
    v[3] = 0.f;
L115:
    for (j = 1; j <= 3; ++j) {
/* L125: */
    w[j - 1] = u[j];
    }
    if (abs(it) - 1 != 0) {
    goto L145;
    } else {
    goto L135;
    }
L135:
    norm_(&u[1], &v[1], &w[3], &it);
    norm_(&u[1], &w[3], &w[6], &it);
    goto L155;
L145:
    norm_(&u[1], &v[1], &w[6], &it);
    norm_(&w[6], &u[1], &w[3], &it);
L155:
    for (i__ = 1; i__ <= 3; ++i__) {
    if (it <= 0) {
        goto L165;
    } else {
        goto L175;
    }
L165:
    ic = -1;
    goto L195;
L175:
    ic = 1;
L195:
    unity_(&w[i__ * 3 - 3], &x[i__ * 3 + 1], &ic);
    }
    return 0;
} /* axes_ */

/* Subroutine */ int bond_(doublereal *z1, doublereal *z2, integer *nb, 
    integer *na1, integer *na2)
{
    /* Initialized data */

    static real resb[2] = { .2f,.08f };

    /* Format strings */
    static char fmt_136[] = "(\002 \002,10x,a6,\002  (\002,i3,\002,\002,i3,i"
        "2,\002)   \002,2f8.2,5x,3f8.3,13x,3f8.4)";
    static char fmt_571[] = "(\002 \002,59x,\002DISTANCE =\002,f8.3/\002 "
        "\002)";

    /* System generated locals */
    integer i__1, i__2;
    real r__1;
    doublereal d__1;

    /* Builtin functions */
    double d_mod(doublereal *, doublereal *);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    double sqrt(doublereal);
    integer pow_ii(integer *, integer *);
    double pow_ri(real *, integer *), r_int(real *);

    /* Local variables */
    static real e[9]    /* was [3][3] */;
    static integer i__, j, k, l, m, n;
    static real r__[3], s[9]    /* was [3][3] */, u[9]  /* was [3][3] */, w[
        26] /* was [13][2] */, z__[3], b1[9]    /* was [3][3] */;
    static integer j1, j2, l1, k1, k2, i9;
    static real t1, t2, v7[3], t6, t5, t3, t4, x1, y1, x2, y2, t9;
    static integer ii, kk;
    static doublereal td, wd[2];
    static integer kl;
    extern /* Subroutine */ int mv_(real *, real *, real *), vm_(real *, real 
        *, real *);
    extern doublereal vv_(real *, real *);
    static doublereal d100;
    static integer ng1;
    static real vj1, vj2;
    static doublereal d1000;
    static real off;
    static integer icq;
    static real ddx, hgt, ddy, vue[3];
    extern doublereal vmv_(real *, real *, real *);
    static doublereal d100k;
    extern /* Subroutine */ int xyz_(doublereal *, real *, integer *);
    static integer ibnd, nbnd, kode;
    extern /* Subroutine */ int difv_(real *, real *, real *), draw_(real *, 
        real *, real *, integer *), axes_(real *, real *, real *, integer 
        *);
    static integer nbis;
    static real dist;
    extern /* Subroutine */ int proj_(real *, real *, real *, real *, real *, 
        integer *, integer *, integer *), norm_(real *, real *, real *, 
        integer *);
    static integer kstp;
    extern /* Subroutine */ int lap800_(integer *, integer *, integer *);
    static integer nres1, kdash;
    static real fract, sdash, denom;
    extern /* Subroutine */ int paxes_(doublereal *, integer *);
    static integer npart;
    static real distr;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *), unity_(real *
        , real *, integer *), pltxy_(real *, real *), radial_(integer *), 
        simbol_(real *, real *, real *, char *, real *, integer *, ftnlen)
        ;
    static real radius;
    static integer nresol;
    extern /* Subroutine */ int numbur_(real *, real *, real *, real *, real *
        , integer *);

    /* Fortran I/O blocks */
    static cilist io___90 = { 0, 0, 0, fmt_136, 0 };
    static cilist io___91 = { 0, 0, 0, fmt_136, 0 };
    static cilist io___144 = { 0, 0, 0, fmt_571, 0 };


/*     ***** OBTAIN POSITIONAL PARAMETERS ***** */
    d100 = 100.f;
    d1000 = 1e3f;
    d100k = 1e5f;
    ng1 = 0;
    for (j = 1; j <= 26; ++j) {
/* L105: */
    w[j - 1] = 0.f;
    }
    wd[0] = *z1;
    wd[1] = *z2;
    for (i__ = 1; i__ <= 2; ++i__) {
    xyz_(&wd[i__ - 1], &w[i__ * 13 - 10], &c__2);
    if (_BLNK__1.ng != 0) {
        goto L125;
    } else {
        goto L110;
    }
L110:
    for (j = 1; j <= 3; ++j) {
/* L115: */
        w[j + 6 + i__ * 13 - 14] = _BLNK__1.xt[j - 1];
    }
    k = (integer) (wd[i__ - 1] / d100k);
    d__1 = wd[i__ - 1] / d100;
    l = (integer) d_mod(&d__1, &d1000);
    l1 = (integer) d_mod(&wd[i__ - 1], &d100);
    pltxy_(&w[i__ * 13 - 10], &w[i__ * 13 - 12]);
    if (_BLNK__1.edge - _BLNK__1.brdr * .25f >= 0.f) {
        goto L128;
    } else {
        goto L120;
    }
L120:
    _BLNK__1.ng = 10;
L125:
    ng1 = 1;
    if (_BLNK__1.nout >= 0) {
        io___90.ciunit = _BLNK__1.nout;
        s_wsfe(&io___90);
        do_fio(&c__1, parms_1.chem + (k - 1 << 3), (ftnlen)8);
        do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&l, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&l1, (ftnlen)sizeof(integer));
        for (j = 2; j <= 9; ++j) {
        do_fio(&c__1, (char *)&w[j + i__ * 13 - 14], (ftnlen)sizeof(
            real));
        }
        e_wsfe();
    }
    erpnt_(&wd[i__ - 1], &c__800);
    goto L134;
L128:
    if (_BLNK__1.nj2 - 10 >= 0) {
        goto L134;
    } else {
        goto L130;
    }
L130:
    if (_BLNK__1.nout >= 0) {
        io___91.ciunit = _BLNK__1.nout;
        s_wsfe(&io___91);
        do_fio(&c__1, parms_1.chem + (k - 1 << 3), (ftnlen)8);
        do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&l, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&l1, (ftnlen)sizeof(integer));
        for (j = 2; j <= 9; ++j) {
        do_fio(&c__1, (char *)&w[j + i__ * 13 - 14], (ftnlen)sizeof(
            real));
        }
        e_wsfe();
    }
L134:
/* L135: */
    ;
    }
    if (ng1 != 0) {
    goto L999;
    } else {
    goto L137;
    }
L137:
    difv_(&w[6], &w[19], v7);
    dist = sqrt(vmv_(v7, _BLNK__1.aa, v7));
    if (_BLNK__1.nj2 % 2 == 0) {
    goto L143;
    }
    if (_BLNK__1.nj2 % 10 == 1) {
    goto L143;
    }
/* *** Line bonds with NO symbol on atom position (803,813) */
    if ((i__1 = _BLNK__1.kd[*nb * 5 - 1], abs(i__1)) >= 1) {
    draw_(&w[1], &c_b317, &c_b317, &c__3);
    draw_(&w[14], &c_b317, &c_b317, &c__2);
    goto L570;
    }
/* *** LINE BONDS AND CENTERED SYMBOLS (803,813) */
    hgt = _BLNK__1.scl * .12f;
    simbol_(&w[1], &w[2], &hgt, " ", &c_b317, &c_n1, (ftnlen)1);
    simbol_(&w[14], &w[15], &hgt, " ", &c_b317, &c_n2, (ftnlen)1);
    goto L570;
/*     ***** STICK BONDS FOR 801,802,811,812 ***** */
L143:
    kode = _BLNK__1.kd[*nb * 5 - 1];
/* *** check for dashed bonds */
    kdash = 0;
    if (abs(kode) >= 10) {
    kdash = abs(kode);
    if (kode < 0) {
        kode = -1;
    } else {
        kode = 1;
    }
    }
    if (kode < 0) {
    goto L145;
    } else if (kode == 0) {
    goto L144;
    } else {
    goto L146;
    }
L144:
    nbnd = 0;
    goto L148;
L145:
    kode = -kode;
L146:
    nbnd = 128 / pow_ii(&c__2, &kode);
/*     ***** FIND UPPERMOST ATOM PUT IN POSITION ONE ***** */
L148:
    if (_BLNK__1.view != 0.f) {
    goto L152;
    } else {
    goto L150;
    }
L150:
    w[11] = 1.f;
    w[24] = 1.f;
    if (w[5] - w[18] >= 0.f) {
    goto L175;
    } else {
    goto L165;
    }
/*     *****VECTOR FROM ATOM TO VIEWPOINT ***** */
L152:
    for (i__ = 1; i__ <= 2; ++i__) {
    for (j = 10; j <= 12; ++j) {
/* L155: */
        w[j + i__ * 13 - 14] = -w[j - 6 + i__ * 13 - 14];
    }
    w[i__ * 13 - 2] += _BLNK__1.view;
/*     ***** DISTANCE SQUARED TO VIEWPOINT ***** */
/* L160: */
    w[i__ * 13 - 1] = vv_(&w[i__ * 13 - 4], &w[i__ * 13 - 4]);
    }
    if (w[25] - w[12] >= 0.f) {
    goto L175;
    } else {
    goto L165;
    }
/*     ***** SWITCH ATOMS ***** */
L165:
    for (j = 1; j <= 13; ++j) {
    t1 = w[j - 1];
    w[j - 1] = w[j + 12];
/* L170: */
    w[j + 12] = t1;
    }
    td = wd[0];
    wd[0] = wd[1];
    wd[1] = td;
/*     ***** FORM IDEMFACTOR MATRIX ***** */
L175:
    for (j = 1; j <= 3; ++j) {
    e[j + j * 3 - 4] = 1.f;
    e[j] = 0.f;
/* L180: */
    e[j + 4] = 0.f;
    }
/*     ***** FORM VECTOR SET RADIAL TO BOND ***** */
    difv_(&w[16], &w[3], &_BLNK__1.da[6]);
    unity_(&_BLNK__1.da[6], _BLNK__1.v3, &c__1);
/*     ***** UNIT VECTOR FROM BOND MIDPOINT TO REFERENCE VIEWPOINT ***** */
    for (i__ = 1; i__ <= 3; ++i__) {
    _BLNK__1.v2[i__ - 1] = 0.f;
    for (j = 1; j <= 3; ++j) {
/* L181: */
        _BLNK__1.v2[i__ - 1] += _BLNK__1.aarev[j + 5] * _BLNK__1.wrkv[j + 
            i__ * 3 - 4];
    }
    if (_BLNK__1.view <= 0.f) {
        goto L183;
    } else {
        goto L182;
    }
L182:
    _BLNK__1.v2[i__ - 1] = _BLNK__1.v2[i__ - 1] * _BLNK__1.view - (w[i__ 
        + 2] + w[i__ + 15]) * .5f;
L183:
    ;
    }
    unity_(_BLNK__1.v2, _BLNK__1.v2, &c__1);
    t6 = (r__1 = vv_(_BLNK__1.v3, _BLNK__1.v2), dabs(r__1));
    if (.9994f - t6 <= 0.f) {
    goto L185;
    } else {
    goto L187;
    }
/*     ***** ALTERNATE CALC IF BOND IS ALONG REFERENCE VIEW DIRECTION *** */
L185:
    for (j = 1; j <= 3; ++j) {
/* L186: */
    _BLNK__1.v2[j - 1] = w[j + 8] + w[j + 21];
    }
    unity_(_BLNK__1.v2, _BLNK__1.v2, &c__1);
    t6 = (r__1 = vv_(_BLNK__1.v3, _BLNK__1.v2), dabs(r__1));
    if (.9994f - t6 <= 0.f) {
    goto L390;
    } else {
    goto L187;
    }
L187:
    axes_(_BLNK__1.v3, _BLNK__1.v2, b1, &c__1);
/* L188: */
    t1 = _BLNK__1.cd[(*nb << 3) - 6] / _BLNK__1.scal2;
    for (j = 1; j <= 3; ++j) {
    _BLNK__1.da[j - 1] = -b1[j + 2] * t1;
/* L190: */
    _BLNK__1.da[j + 2] = -b1[j + 5] * t1;
    }
    if (nbnd <= 0) {
    goto L500;
    } else {
    goto L195;
    }
/*     ***** SET PLOTTING RESOLUTION FOR BOND ***** */
L195:
    t1 = _BLNK__1.cd[(*nb << 3) - 6] * _BLNK__1.scl;
    nresol = 4;
    nbis = 3;
    for (j = 1; j <= 2; ++j) {
    if (t1 >= resb[j - 1]) {
        goto L202;
    }
    if (nbnd <= nresol) {
        goto L202;
    }
    --nbis;
/* L200: */
    nresol <<= 1;
    }
L202:
    nres1 = nresol + 1;
    radial_(&nbis);
/*     ***** DERIVE QUADRICS FOR EACH ATOM ***** */
    for (ii = 1; ii <= 2; ++ii) {
    paxes_(&wd[ii - 1], &c__2);
    if (_BLNK__1.ng != 0) {
        goto L205;
    } else {
        goto L210;
    }
L205:
    erpnt_(&wd[ii - 1], &c__800);
    goto L999;
/*     ***** DOES BOND GO TO ELLIPSOID OR TO ENVELOPE ***** */
L210:
    t1 = (real) (3 - (ii << 1));
    for (j = 1; j <= 3; ++j) {
        _BLNK__1.v3[j - 1] *= t1;
/* L212: */
        vue[j - 1] = 0.f;
    }
    if (_BLNK__1.kd[*nb * 5 - 1] <= 0) {
        goto L260;
    } else {
        goto L215;
    }
L215:
    if (vmv_(_BLNK__1.v3, _BLNK__1.q, &w[ii * 13 - 4]) >= 0.f) {
        goto L260;
    } else {
        goto L220;
    }
L220:
    ibnd = 0;
    if (_BLNK__1.view <= 0.f) {
        goto L240;
    } else {
        goto L225;
    }
/*     ***** DERIVE TANGENT CONE DIRECTLY WITHOUT ROTATING COORDINATES ** */
L225:
/* Computing 2nd power */
    r__1 = _BLNK__1.scal2 * _BLNK__1.rms[0] * _BLNK__1.rms[1] * 
        _BLNK__1.rms[2];
    t2 = -(r__1 * r__1);
    for (j = 1; j <= 3; ++j) {
        _BLNK__1.v1[j - 1] = -w[j + 9 + ii * 13 - 14] / _BLNK__1.scal1;
        vue[j - 1] = _BLNK__1.v1[j - 1] / _BLNK__1.scal2;
/*     ***** INVERT ELLIPSOID MATRIX ***** */
        for (k = j; k <= 3; ++k) {
        t1 = 0.f;
        for (i__ = 1; i__ <= 3; ++i__) {
/* L228: */
/* Computing 2nd power */
            r__1 = _BLNK__1.rms[i__ - 1];
            t1 += _BLNK__1.pac[j + i__ * 3 - 4] * _BLNK__1.pac[k + 
                i__ * 3 - 4] * (r__1 * r__1);
        }
        u[j + k * 3 - 4] = t1;
/* L230: */
        u[k + j * 3 - 4] = t1;
        }
    }
/*     *****  ADD POLARIZED COFACTOR MATRIX TO ELLIPSOID MATRIX ***** */
    for (j = 1; j <= 3; ++j) {
        j1 = j % 3 + 1;
        vj1 = _BLNK__1.v1[j1 - 1];
        j2 = (j + 1) % 3 + 1;
        vj2 = _BLNK__1.v1[j2 - 1];
        for (k = j; k <= 3; ++k) {
        k1 = k % 3 + 1;
        k2 = (k + 1) % 3 + 1;
        s[j + k * 3 - 4] = t2 * _BLNK__1.q[j + k * 3 - 4] + (vj2 * (u[
            j1 + k1 * 3 - 4] * _BLNK__1.v1[k2 - 1] - u[j1 + k2 * 
            3 - 4] * _BLNK__1.v1[k1 - 1]) + vj1 * (u[j2 + k2 * 3 
            - 4] * _BLNK__1.v1[k1 - 1] - u[j2 + k1 * 3 - 4] * 
            _BLNK__1.v1[k2 - 1]));
/* L235: */
        s[k + j * 3 - 4] = s[j + k * 3 - 4];
        }
    }
    t5 = 0.f;
    goto L300;
/*     ***** DERIVE TANGENT CYLINDER WITH AXIS ALONG Z ***** */
L240:
    t1 = -1.f / _BLNK__1.q[8];
    for (j = 1; j <= 2; ++j) {
        for (k = 1; k <= 2; ++k) {
/* L245: */
        s[k + j * 3 - 4] = _BLNK__1.q[k + j * 3 - 4] + _BLNK__1.q[k + 
            5] * _BLNK__1.q[j + 5] * t1;
        }
        s[j * 3 - 1] = 0.f;
/* L250: */
        s[j + 5] = 0.f;
    }
    s[8] = 0.f;
    goto L270;
/*     ***** TRANSFER ELLIPSOID ***** */
L260:
    for (j = 1; j <= 9; ++j) {
/* L265: */
        s[j - 1] = _BLNK__1.q[j - 1];
    }
    ibnd = ii;
L270:
    t5 = 1.f;
/*     ***** CHECK FOR BOND TAPER ***** */
L300:
    if (ii - 2 >= 0) {
        goto L310;
    } else {
        goto L305;
    }
L305:
    radius = t6 * _BLNK__1.taper + 1.f;
    goto L320;
L310:
    radius = 1.f - t6 * _BLNK__1.taper;
L320:
    mv_(s, _BLNK__1.v3, _BLNK__1.v4);
    t2 = vv_(_BLNK__1.v3, _BLNK__1.v4);
/*     ***** COMPUTE BOND INTERSECTION ***** */
    kl = 5 - ii - ii;
    kstp = nresol;
    if (_BLNK__1.nj2 - 21 >= 0) {
        goto L322;
    } else {
        goto L324;
    }
L322:
    kstp = 32;
L324:
    i__1 = kstp;
    for (k = 1; i__1 < 0 ? k >= 65 : k <= 65; k += i__1) {
        for (j = 1; j <= 3; ++j) {
        _BLNK__1.v6[j - 1] = _BLNK__1.d__[j + k * 3 - 4] * radius;
/* L325: */
        _BLNK__1.v5[j - 1] = _BLNK__1.v6[j - 1] + vue[j - 1];
        }
        t3 = vv_(_BLNK__1.v5, _BLNK__1.v4);
        t4 = t3 * t3 - t2 * (vmv_(_BLNK__1.v5, s, _BLNK__1.v5) - t5);
        if (t4 >= 0.f) {
        goto L330;
        } else {
        goto L345;
        }
L330:
        t4 = sqrt(t4);
        t1 = (t4 - t3) / t2;
        t3 = (-t4 - t3) / t2;
        l = k + kl - 1;
        for (j = 1; j <= 3; ++j) {
        _BLNK__1.d__[j + l * 3 - 4] = (_BLNK__1.v6[j - 1] + t1 * 
            _BLNK__1.v3[j - 1]) * _BLNK__1.scl;
/* L335: */
        _BLNK__1.d__[j + (l + 1) * 3 - 4] = (-_BLNK__1.v6[j - 1] - t3 
            * _BLNK__1.v3[j - 1]) * _BLNK__1.scl;
        }
    }
    if (ibnd + 21 - _BLNK__1.nj2 != 0) {
        goto L360;
    } else {
        goto L338;
    }
L338:
    if (_BLNK__1.kd[*nb * 5 - 1] <= 0) {
        goto L360;
    } else {
        goto L340;
    }
/*     ***** FOR LOCAL OVERLAP, MAKE BOND QUADRANGLE TANGENT TO ENVELOPING CONE */
L340:
    t3 = vv_(vue, _BLNK__1.v4);
/* Computing 2nd power */
    r__1 = t3;
    t4 = r__1 * r__1 - t2 * (vmv_(vue, s, vue) - t5);
    if (t4 >= 0.f) {
        goto L350;
    } else {
        goto L345;
    }
L345:
    _BLNK__1.ng = 13;
    erpnt_(&wd[ii - 1], &c__800);
    goto L999;
L350:
    t1 = (sqrt(t4) - t3) / t2;
    for (j = 1; j <= 3; ++j) {
        t4 = (t1 * _BLNK__1.v3[j - 1] * _BLNK__1.scl - (_BLNK__1.d__[j + 
            kl * 3 - 4] + _BLNK__1.d__[j + (kl + 64) * 3 - 4]) * .5f) 
            * 1.001f;
        _BLNK__1.d__[j + kl * 3 - 4] += t4;
/* L355: */
        _BLNK__1.d__[j + (kl + 64) * 3 - 4] += t4;
    }
L360:
    proj_(&_BLNK__1.d__[kl * 3 - 3], &_BLNK__1.dp[(ii << 1) - 2], &w[ii * 
        13 - 10], _BLNK__1.xo, &_BLNK__1.view, &c__1, &c__65, &kstp);
    if (ibnd - 1 != 0) {
        goto L370;
    } else {
        goto L365;
    }
L365:
    i__1 = 65 - kstp;
    proj_(&_BLNK__1.d__[(kl + kstp + 1) * 3 - 3], &_BLNK__1.dp[(ii + 64 + 
        kstp << 1) - 2], &w[ii * 13 - 10], _BLNK__1.xo, &
        _BLNK__1.view, &c__1, &i__1, &kstp);
    goto L380;
/*     ***** RETRACE TOP HALF ***** */
L370:
    kk = 64 - (ii - 1) * kstp;
    i__1 = kk;
    i__2 = kstp;
    for (k = kstp; i__2 < 0 ? k >= i__1 : k <= i__1; k += i__2) {
        l = k + ii;
        m = l + 64;
        n = 66 - l;
        _BLNK__1.dp[(m << 1) - 2] = _BLNK__1.dp[(n << 1) - 2];
/* L375: */
        _BLNK__1.dp[(m << 1) - 1] = _BLNK__1.dp[(n << 1) - 1];
    }
L380:
    ;
    }
/*     ***** CHECK FOR LOCAL OVERLAP OR HIDDEN BOND ***** */
    for (k = 1; k <= 65; k += 32) {
    t1 = 0.f;
    t2 = 0.f;
    for (j = 1; j <= 2; ++j) {
/* Computing 2nd power */
        r__1 = _BLNK__1.dp[j + (k << 1) - 3] - w[j];
        t1 += r__1 * r__1;
/* L385: */
/* Computing 2nd power */
        r__1 = _BLNK__1.dp[j + (k + 1 << 1) - 3] - w[j];
        t2 += r__1 * r__1;
    }
    if (t2 - t1 <= 0.f) {
        goto L390;
    } else {
        goto L395;
    }
L395:
    ;
    }
/*     ***** CALL GLOBAL OVERLAP ROUTINE ***** */
    icq = 0;
    lap800_(na1, na2, &icq);
    if (_BLNK__1.nj2 - 21 >= 0) {
    goto L999;
    } else {
    goto L400;
    }
L400:
    if (icq >= 0) {
    goto L405;
    } else {
    goto L390;
    }
L405:
/* *** draw dashed stick bonds */
    if (kdash != 0) {
/*        draw bond ends */
    draw_(_BLNK__1.dp, &c_b317, &c_b317, &c__3);
    i__2 = nresol;
    for (k = nres1; i__2 < 0 ? k >= 129 : k <= 129; k += i__2) {
/* L406: */
        draw_(&_BLNK__1.dp[(k << 1) - 2], &c_b317, &c_b317, &c__2);
    }
    draw_(&_BLNK__1.dp[2], &c_b317, &c_b317, &c__3);
    i__2 = nresol;
    for (k = 2; i__2 < 0 ? k >= 66 : k <= 66; k += i__2) {
/* L408: */
        draw_(&_BLNK__1.dp[(k << 1) - 2], &c_b317, &c_b317, &c__2);
    }
/*        draw dashed parts */
    r__[2] = 0.f;
    sdash = (real) (kdash / 10);
    fract = (real) (kdash % 10);
    if (fract == 0.f) {
        fract = 5.f;
    }
    for (k = 1; k <= 65; k += 64) {
        x1 = _BLNK__1.dp[(k + 1 << 1) - 2];
        y1 = _BLNK__1.dp[(k + 1 << 1) - 1];
        x2 = _BLNK__1.dp[(k << 1) - 2];
        y2 = _BLNK__1.dp[(k << 1) - 1];
        denom = sdash * fract + (sdash - 1.f) * (10.f - fract);
        ddx = (x2 - x1) / denom;
        ddy = (y2 - y1) / denom;
        draw_(&_BLNK__1.dp[(k + 1 << 1) - 2], &c_b317, &c_b317, &c__3);
        npart = sdash * 2.f - 1.f;
        i__2 = npart;
        for (j = 1; j <= i__2; ++j) {
        if (j % 2 == 1) {
            r__[0] = x1 + ddx * fract;
            r__[1] = y1 + ddy * fract;
            draw_(r__, &c_b317, &c_b317, &c__2);
        } else {
            r__[0] = x1 + ddx * (10.f - fract);
            r__[1] = y1 + ddy * (10.f - fract);
            draw_(r__, &c_b317, &c_b317, &c__3);
        }
        x1 = r__[0];
        y1 = r__[1];
/* L410: */
        }
    }
    goto L500;
    }
/* *** draw non-dashed stick bonds */
/*     ***** DRAW BOND OUTLINE ***** */
    draw_(_BLNK__1.dp, &c_b317, &c_b317, &c__3);
    i__2 = nresol;
    for (k = nres1; i__2 < 0 ? k >= 129 : k <= 129; k += i__2) {
/* L415: */
    draw_(&_BLNK__1.dp[(k << 1) - 2], &c_b317, &c_b317, &c__2);
    }
    i__2 = nresol;
    for (k = 2; i__2 < 0 ? k >= 66 : k <= 66; k += i__2) {
/* L420: */
    draw_(&_BLNK__1.dp[(k << 1) - 2], &c_b317, &c_b317, &c__2);
    }
    draw_(&_BLNK__1.dp[128], &c_b317, &c_b317, &c__2);
/*     ***** DRAW BOND DETAIL ***** */
/* L425: */
    k = 65;
L430:
    k -= nbnd;
    if (k - 1 <= 0) {
    goto L500;
    } else {
    goto L435;
    }
L435:
    draw_(&_BLNK__1.dp[(k << 1) - 2], &c_b317, &c_b317, &c__3);
    draw_(&_BLNK__1.dp[(k + 1 << 1) - 2], &c_b317, &c_b317, &c__2);
    k -= nbnd;
    if (k - 1 <= 0) {
    goto L500;
    } else {
    goto L440;
    }
L440:
    draw_(&_BLNK__1.dp[(k + 1 << 1) - 2], &c_b317, &c_b317, &c__3);
    draw_(&_BLNK__1.dp[(k << 1) - 2], &c_b317, &c_b317, &c__2);
    goto L430;
L500:
    hgt = _BLNK__1.cd[(*nb << 3) - 5];
    off = _BLNK__1.cd[(*nb << 3) - 4];
    if (hgt <= 0.f) {
    goto L570;
    } else {
    goto L510;
    }
/*     ***** PERSPECTIVE BOND LABEL ROUTINE ***** */
/*     ***** BASE DECISIONS ON REFERENCE SYSTEM ***** */
L510:
    k = 0;
    difv_(&w[19], &w[6], v7);
    vm_(v7, _BLNK__1.aarev, _BLNK__1.v1);
    axes_(_BLNK__1.v1, &e[6], u, &c__1);
    for (i__ = 1; i__ <= 3; ++i__) {
    t1 = 1.f;
    if (i__ - 2 <= 0) {
        goto L515;
    } else {
        goto L520;
    }
L515:
    if (vv_(&u[i__ * 3 - 3], &_BLNK__1.symb[i__ * 3 - 3]) >= 0.f) {
        goto L530;
    } else {
        goto L525;
    }
L520:
    if (k % 2 != 0) {
        goto L530;
    } else {
        goto L525;
    }
L525:
    t1 = -1.f;
    ++k;
L530:
    for (j = 1; j <= 3; ++j) {
        u[j + i__ * 3 - 4] *= t1;
/* L535: */
        _BLNK__1.vt[j + i__ * 3 - 4] = b1[j + i__ * 3 - 4] * t1;
    }
    }
    for (j = 1; j <= 3; ++j) {
/* L540: */
    _BLNK__1.vt[j + 8] = (w[j + 2] + w[j + 15]) * .5f;
    }
/*     ***** CHECK FOR EXCESS FORESHORTENING ***** */
    if (_BLNK__1.fore - dabs(u[2]) >= 0.f) {
    goto L550;
    } else {
    goto L545;
    }
L545:
    norm_(&u[3], &_BLNK__1.symb[6], _BLNK__1.vt, &c__1);
    _BLNK__1.vt[6] = _BLNK__1.symb[6];
    _BLNK__1.vt[7] = _BLNK__1.symb[7];
    _BLNK__1.vt[8] = _BLNK__1.symb[8];
    hgt = _BLNK__1.cd[(*nb << 3) - 3];
    off = _BLNK__1.cd[(*nb << 3) - 2];
    if (hgt != 0.f) {
    goto L550;
    } else {
    goto L999;
    }
L550:
    t1 = _BLNK__1.cd[(*nb << 3) - 1];
    z__[0] = _BLNK__1.vt[9] - hgt * (t1 * 3.f + 11.f) / 7.f;
    z__[1] = _BLNK__1.vt[10] + off - hgt * .5f;
    z__[2] = _BLNK__1.vt[11];
    _BLNK__1.xo[2] = z__[2];
    _BLNK__1.itilt = 1;
    i9 = t1 + 2.f;
    t9 = pow_ri(&c_b469, &i9);
    r__1 = dist * t9 + .5f;
    distr = r_int(&r__1) / t9 + 1e-4f;
    numbur_(z__, &z__[1], &hgt, &distr, &c_b317, &i9);
L570:
    _BLNK__1.itilt = 0;
    if (_BLNK__1.nj2 - 10 >= 0) {
    goto L999;
    } else {
    goto L580;
    }
L580:
    if (_BLNK__1.nout >= 0) {
    io___144.ciunit = _BLNK__1.nout;
    s_wsfe(&io___144);
    do_fio(&c__1, (char *)&dist, (ftnlen)sizeof(real));
    e_wsfe();
    }
    goto L999;
L390:
    _BLNK__1.ng = 14;
    erpnt_(&wd[1], &c__800);
L999:
    return 0;
} /* bond_ */

/* Subroutine */ int difv_(real *x, real *y, real *z__)
{
/*     VECTOR - VECTOR */
/*     Z(3)=X(3)-Y(3) */
    /* Parameter adjustments */
    --z__;
    --y;
    --x;

    /* Function Body */
    z__[1] = x[1] - y[1];
    z__[2] = x[2] - y[2];
    z__[3] = x[3] - y[3];
    return 0;
} /* difv_ */

/* Subroutine */ int draw_(real *w, real *dx, real *dy, integer *npen)
{
    static integer i__, j;
    static real x[3], y[3], z__[3];
    static integer ncq;
    extern /* Subroutine */ int pltxy_(real *, real *), scribe_(real *, 
        integer *), lapdrw_(real *, integer *, integer *);

    /* Parameter adjustments */
    --w;

    /* Function Body */
    y[0] = w[1] + *dx;
    y[1] = w[2] + *dy;
    if (_BLNK__1.itilt != 0) {
    goto L115;
    } else {
    goto L140;
    }
/*     ***** ROTATE FOR PERSPECTIVE TITLE ***** */
L115:
    y[2] = _BLNK__1.xo[2];
    for (i__ = 1; i__ <= 3; ++i__) {
/* L120: */
    z__[i__ - 1] = y[i__ - 1] - _BLNK__1.vt[i__ + 8];
    }
    for (i__ = 1; i__ <= 3; ++i__) {
/* L130: */
    x[i__ - 1] = _BLNK__1.vt[i__ - 1] * z__[0] + _BLNK__1.vt[i__ + 2] * 
        z__[1] + _BLNK__1.vt[i__ + 5] * z__[2] + _BLNK__1.vt[i__ + 8];
    }
    pltxy_(x, y);
/*     ***** CHECK BOUNDRY ***** */
L140:
    for (j = 1; j <= 2; ++j) {
    if (y[j - 1] - _BLNK__1.xlng[j - 1] + .1f <= 0.f) {
        goto L150;
    } else {
        goto L145;
    }
L145:
    y[j - 1] = _BLNK__1.xlng[j - 1] - .1f;
L150:
    if (y[j - 1] - .1f >= 0.f) {
        goto L160;
    } else {
        goto L155;
    }
L155:
    y[j - 1] = .1f;
L160:
    ;
    }
/*     ***** CHECK FOR OVERLAP ***** */
    ncq = 0;
    lapdrw_(y, npen, &ncq);
    if (ncq <= 0) {
    goto L165;
    } else {
    goto L170;
    }
/*     ***** CALL PLOTTING ROUTINE IF NO OVERLAPPING ELEMENTS ARE STORED */
L165:
    scribe_(y, npen);
L170:
    return 0;
} /* draw_ */

/* Subroutine */ int editr_(void)
{
    /* Format strings */
    static char fmt_10[] = "(6x,\002         1111111111222222222233333333334"
        "444444444555\002,\00255555556666666666777\002,/,1x,\002LINE\002,"
        "1x,\0021234567890123456789012345\002,\00267890123456789012345678"
        "901234567890123456789012\002)";
    static char fmt_103[] = "(1x)";
    static char fmt_111[] = "(1x,i3,2x,a72)";
    // :-
    /*
    static char fmt_121[] = "(/,\002 C=Change line #                        "
        " D=Delete line(s) # [#]\002,/,\002 I=Insert line before #       "
        "           T=Type line(s) [#] [#]\002,/,\002 S=Save modified ins"
        "truction set         O=Restore original instruction set\002,/"
        ",\002 P=Save drawing as Postscript            H=Save drawing as "
        "HPGL\002,/,\002 R=Redraw structure on screen            Q=Qui"
        "t\002,/,\002 >-> \002,$)";
    */
    static char fmt_121[] = "(/,\002 C=Change line #                        "
        " D=Delete line(s) # [#]\002,/,\002 I=Insert line before #       "
        "           T=Type line(s) [#] [#]\002,/,\002 S=Save modified ins"
        "truction set         O=Restore original instruction set\002,/"
        ",\002 P=Save drawing as Postscript            H=Save drawing as "
        "HPGL\002,/,\002 R=Save drawing as PNG                   Q=Qui"
        "t\002,/,\002 >-> \002,$)";
    // --
    static char fmt_131[] = "(a)";
    static char fmt_134[] = "(/,\002***INVALID INPUT! Enter 1 letter and 0, "
        "1, or 2 integers separated by spaces.***\002)";
    static char fmt_136[] = "(\002'\002,a1,\002'\002,1x,a)";
    static char fmt_137[] = "(/,\002*** value out of range ***\002)";
    static char fmt_271[] = "(\002 *** NOTE: Type @ to substitute a space in"
        " the original line. ***\002,/)";
    static char fmt_276[] = "(1x,i3,\002  \002,$)";
    static char fmt_281[] = "(a72)";
    static char fmt_283[] = "(/,\002      Line now reads:\002)";
    static char fmt_284[] = "(6x,a)";
    static char fmt_285[] = "(/,\002 Hit ENTER or RETURN key \002,$)";
    static char fmt_422[] = "(\002 Enter file name:  \002,$)";
    static char fmt_450[] = "(a73)";

    /* System generated locals */
    address a__1[2];
    integer i__1, i__2, i__3[2];
    olist o__1;
    cllist cl__1;
    alist al__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen),
         s_rsfe(cilist *), e_rsfe(void);
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);
    integer f_open(olist *), f_rew(alist *), s_rsle(cilist *), do_lio(integer 
        *, integer *, char *, ftnlen), e_rsle(void), f_clos(cllist *), 
        s_wsle(cilist *), e_wsle(void);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);

    /* Local variables */
    static integer i__, j, n;
    static char ch[1];
    static integer nn, ich, num, num1, num2, num4;
    static char card[80];
    extern integer iend_(char *, ftnlen);
    static integer last, numz;
    static char tline[73], answer[80];
    extern /* Subroutine */ int recycle_(void);

    /* Fortran I/O blocks */
    static cilist io___153 = { 0, 6, 0, fmt_103, 0 };
    static cilist io___154 = { 0, 6, 0, fmt_111, 0 };
    static cilist io___156 = { 0, 6, 0, fmt_121, 0 };
    static cilist io___157 = { 0, 5, 0, fmt_131, 0 };
    static cilist io___163 = { 0, 6, 0, fmt_134, 0 };
    static cilist io___164 = { 0, 15, 0, fmt_136, 0 };
    static cilist io___165 = { 0, 15, 0, 0, 0 };
    static cilist io___168 = { 0, 6, 0, fmt_137, 0 };
    static cilist io___169 = { 0, 6, 0, 0, 0 };
    static cilist io___170 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___171 = { 0, 6, 0, fmt_111, 0 };
    static cilist io___172 = { 0, 6, 0, 0, 0 };
    static cilist io___173 = { 0, 6, 0, 0, 0 };
    static cilist io___174 = { 0, 6, 0, fmt_271, 0 };
    static cilist io___175 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___176 = { 0, 6, 0, fmt_111, 0 };
    static cilist io___177 = { 0, 6, 0, fmt_276, 0 };
    static cilist io___178 = { 0, 5, 0, fmt_281, 0 };
    static cilist io___180 = { 0, 6, 0, fmt_283, 0 };
    static cilist io___181 = { 0, 6, 0, fmt_284, 0 };
    static cilist io___182 = { 0, 6, 0, fmt_285, 0 };
    static cilist io___183 = { 0, 5, 0, fmt_131, 0 };
    static cilist io___184 = { 0, 6, 0, 0, 0 };
    static cilist io___188 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___189 = { 0, 6, 0, fmt_111, 0 };
    static cilist io___190 = { 0, 6, 0, fmt_276, 0 };
    static cilist io___191 = { 0, 5, 0, fmt_281, 0 };
    static cilist io___192 = { 0, 6, 0, fmt_422, 0 };
    static cilist io___193 = { 0, 5, 0, fmt_131, 0 };
    static cilist io___194 = { 0, 0, 1, fmt_131, 0 };
    static cilist io___196 = { 0, 16, 0, fmt_131, 0 };
    static cilist io___197 = { 0, 16, 0, fmt_450, 0 };
    static cilist io___198 = { 0, 6, 0, 0, 0 };
    static cilist io___199 = { 0, 0, 1, fmt_131, 0 };
    static cilist io___200 = { 0, 16, 0, fmt_131, 0 };
    static cilist io___201 = { 0, 16, 0, fmt_450, 0 };


/* L10: */
    num = 1;
/* *** PRINT PART OR ALL OF COMMAND QUE *** */
/* Computing MAX */
    i__1 = 1, i__2 = num - 2;
    num1 = max(i__1,i__2);
L100:
    s_wsfe(&io___153);
    e_wsfe();
    s_wsfe(&io___154);
    i__1 = queue_1.nque;
    for (j = num1; j <= i__1; ++j) {
    do_fio(&c__1, (char *)&j, (ftnlen)sizeof(integer));
    do_fio(&c__1, queue_1.que + (j - 1) * 73, (ftnlen)73);
    }
    e_wsfe();
/* *** DISPLAY PROMPT *** */
L115:
    s_wsfe(&io___156);
    e_wsfe();
/* *** READ COMMAND CHARACTER AND LINE NUMBER(S) *** */
    s_rsfe(&io___157);
    do_fio(&c__1, answer, (ftnlen)80);
    e_rsfe();
    if (*(unsigned char *)answer == ' ') {
    goto L133;
    }
    if (*(unsigned char *)answer >= '1' && *(unsigned char *)answer <= '9') {
    goto L133;
    }
    last = iend_(answer, (ftnlen)80);
    i__1 = last;
    for (i__ = 2; i__ <= i__1; ++i__) {
    *(unsigned char *)ch = *(unsigned char *)&answer[i__ - 1];
    if (*(unsigned char *)ch >= 'a' && *(unsigned char *)ch <= 'z' || *(
        unsigned char *)ch >= 'A' && *(unsigned char *)ch <= 'Z') {
        goto L133;
    }
    ich = *(unsigned char *)ch - 48;
    if (*(unsigned char *)ch != ' ' && (ich < 0 || ich > 9)) {
        goto L133;
    }
/* L132: */
    }
    goto L135;
L133:
    s_wsfe(&io___163);
    e_wsfe();
    goto L115;
L135:
/* Writing concatenation */
    i__3[0] = last, a__1[0] = answer;
    i__3[1] = 4, a__1[1] = " 0 0";
    s_cat(answer, a__1, i__3, &c__2, (ftnlen)80);
/*     read(answer,*)ch,num,num2 */
    o__1.oerr = 0;
    o__1.ounit = 15;
    o__1.ofnm = 0;
    o__1.orl = 0;
    o__1.osta = "scratch";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    s_wsfe(&io___164);
    do_fio(&c__1, answer, (ftnlen)1);
    do_fio(&c__1, answer + 1, (ftnlen)74);
    e_wsfe();
    al__1.aerr = 0;
    al__1.aunit = 15;
    f_rew(&al__1);
    s_rsle(&io___165);
    do_lio(&c__9, &c__1, ch, (ftnlen)1);
    do_lio(&c__3, &c__1, (char *)&num, (ftnlen)sizeof(integer));
    do_lio(&c__3, &c__1, (char *)&num2, (ftnlen)sizeof(integer));
    e_rsle();
    cl__1.cerr = 0;
    cl__1.cunit = 15;
    cl__1.csta = 0;
    f_clos(&cl__1);
    numz = num;
    num = max(1,num);
    if (num2 > queue_1.nque) {
    s_wsfe(&io___168);
    e_wsfe();
    goto L115;
    }
    if (num > num2) {
    num2 = num;
    }
    s_wsle(&io___169);
    do_lio(&c__9, &c__1, " ", (ftnlen)1);
    e_wsle();
    if (*(unsigned char *)ch == 'T' || *(unsigned char *)ch == 't') {
    goto L210;
    }
    if (*(unsigned char *)ch == 'D' || *(unsigned char *)ch == 'd') {
    goto L240;
    }
    if (*(unsigned char *)ch == 'C' || *(unsigned char *)ch == 'c') {
    goto L270;
    }
    if (*(unsigned char *)ch == 'I' || *(unsigned char *)ch == 'i') {
    goto L310;
    }
    if (*(unsigned char *)ch == 'O' || *(unsigned char *)ch == 'o') {
    goto L410;
    }
    if (*(unsigned char *)ch == 'S' || *(unsigned char *)ch == 's') {
    goto L420;
    }
    if (*(unsigned char *)ch == 'R' || *(unsigned char *)ch == 'r') {
    goto L540;
    }
    if (*(unsigned char *)ch == 'Q' || *(unsigned char *)ch == 'q') {
    goto L590;
    }
    if (*(unsigned char *)ch == 'P' || *(unsigned char *)ch == 'p') {
    goto L510;
    }
    if (*(unsigned char *)ch == 'H' || *(unsigned char *)ch == 'h') {
    goto L520;
    }
    goto L115;
/* *** TYPE LINES *** */
L210:
    if (numz == 0) {
    num2 = queue_1.nque;
    }
    s_wsfe(&io___170);
    e_wsfe();
    s_wsfe(&io___171);
    i__1 = num2;
    for (j = num; j <= i__1; ++j) {
    do_fio(&c__1, (char *)&j, (ftnlen)sizeof(integer));
    do_fio(&c__1, queue_1.que + (j - 1) * 73, (ftnlen)73);
    }
    e_wsfe();
    goto L115;
/* *** DELETE LINES *** */
L240:
    if (numz == 0) {
    s_wsle(&io___172);
    do_lio(&c__9, &c__1, " *** Supply line number(s) with command ***", (
        ftnlen)43);
    e_wsle();
    goto L115;
    }
    i__1 = num2;
    for (i__ = num; i__ <= i__1; ++i__) {
    --queue_1.nque;
    --queue_1.next;
    i__2 = queue_1.nque;
    for (j = num; j <= i__2; ++j) {
/* L250: */
        s_copy(queue_1.que + (j - 1) * 73, queue_1.que + j * 73, (ftnlen)
            73, (ftnlen)73);
    }
/* L260: */
    s_copy(queue_1.que + queue_1.nque * 73, "                        ", (
        ftnlen)73, (ftnlen)24);
    }
    goto L100;
/* *** CHANGE AN OLD LINE *** */
L270:
    if (numz == 0) {
    s_wsle(&io___173);
    do_lio(&c__9, &c__1, " *** Supply line number with command ***", (
        ftnlen)40);
    e_wsle();
    goto L115;
    }
/*     NUM3=MAX0(1,NUM-3) */
/*     WRITE(*,111)(J,QUE(J),J=NUM3,NUM) */
    s_wsfe(&io___174);
    e_wsfe();
    s_wsfe(&io___175);
    e_wsfe();
    s_wsfe(&io___176);
    do_fio(&c__1, (char *)&num, (ftnlen)sizeof(integer));
    do_fio(&c__1, queue_1.que + (num - 1) * 73, (ftnlen)73);
    e_wsfe();
    s_wsfe(&io___177);
    do_fio(&c__1, (char *)&num, (ftnlen)sizeof(integer));
    e_wsfe();
    s_rsfe(&io___178);
    do_fio(&c__1, tline, (ftnlen)73);
    e_rsfe();
    for (i__ = 1; i__ <= 72; ++i__) {
    if (*(unsigned char *)&tline[i__ - 1] != ' ') {
        if (*(unsigned char *)&tline[i__ - 1] == '@') {
        *(unsigned char *)&queue_1.que[(num - 1) * 73 + (i__ - 1)] = 
            ' ';
        } else {
        *(unsigned char *)&queue_1.que[(num - 1) * 73 + (i__ - 1)] = *
            (unsigned char *)&tline[i__ - 1];
        }
    }
/* L282: */
    }
    s_wsfe(&io___180);
    e_wsfe();
    s_wsfe(&io___181);
    do_fio(&c__1, queue_1.que + (num - 1) * 73, (ftnlen)73);
    e_wsfe();
    s_wsfe(&io___182);
    e_wsfe();
    s_rsfe(&io___183);
    do_fio(&c__1, ch, (ftnlen)1);
    e_rsfe();
    goto L100;
/* *** INSERT A NEW LINE *** */
L310:
    if (numz == 0) {
    s_wsle(&io___184);
    do_lio(&c__9, &c__1, " *** Supply line number with command ***", (
        ftnlen)40);
    e_wsle();
    goto L115;
    }
    if ((real) (queue_1.nque + 1) > 5e3f) {
    goto L115;
    }
    ++queue_1.nque;
    nn = queue_1.nque;
    n = queue_1.nque - num;
    i__1 = n;
    for (j = 1; j <= i__1; ++j) {
    s_copy(queue_1.que + (nn - 1) * 73, queue_1.que + (nn - 2) * 73, (
        ftnlen)73, (ftnlen)73);
/* L320: */
    --nn;
    }
/* Computing MAX */
    i__1 = 1, i__2 = num - 4;
    num4 = max(i__1,i__2);
/* Computing MAX */
    i__1 = 1, i__2 = num - 1;
    num1 = max(i__1,i__2);
    s_wsfe(&io___188);
    e_wsfe();
    s_wsfe(&io___189);
    i__1 = num1;
    for (j = num4; j <= i__1; ++j) {
    do_fio(&c__1, (char *)&j, (ftnlen)sizeof(integer));
    do_fio(&c__1, queue_1.que + (j - 1) * 73, (ftnlen)73);
    }
    e_wsfe();
    s_wsfe(&io___190);
    do_fio(&c__1, (char *)&num, (ftnlen)sizeof(integer));
    e_wsfe();
    s_rsfe(&io___191);
    do_fio(&c__1, queue_1.que + (num - 1) * 73, (ftnlen)73);
    e_rsfe();
    ++queue_1.next;
    goto L100;
/* *** RETRIEVE OLD SET OF INSTRUCTIONS *** */
L410:
    i__1 = queue_1.nback;
    for (j = 1; j <= i__1; ++j) {
/* L415: */
    s_copy(queue_1.que + (j - 1) * 73, queue_1.hque + (j - 1) * 73, (
        ftnlen)73, (ftnlen)73);
    }
    num = 1;
    num1 = 1;
    queue_1.nque = queue_1.nback;
    num2 = queue_1.nque;
    goto L100;
/* *** SAVE CURRENT SET OF INSTRUCTIONS *** */
L420:
L421:
    s_wsfe(&io___192);
    e_wsfe();
    s_rsfe(&io___193);
    do_fio(&c__1, answer, (ftnlen)80);
    e_rsfe();
    o__1.oerr = 1;
    o__1.ounit = 16;
    o__1.ofnmlen = 80;
    o__1.ofnm = answer;
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
    goto L460;
    }
    al__1.aerr = 0;
    al__1.aunit = queue_1.ned;
    f_rew(&al__1);
L430:
    io___194.ciunit = queue_1.ned;
    i__1 = s_rsfe(&io___194);
    if (i__1 != 0) {
    goto L440;
    }
    i__1 = do_fio(&c__1, card, (ftnlen)80);
    if (i__1 != 0) {
    goto L440;
    }
    i__1 = e_rsfe();
    if (i__1 != 0) {
    goto L440;
    }
    s_wsfe(&io___196);
    do_fio(&c__1, card, iend_(card, (ftnlen)80));
    e_wsfe();
    goto L430;
L440:
    s_wsfe(&io___197);
    i__1 = queue_1.nque;
    for (i__ = 1; i__ <= i__1; ++i__) {
    do_fio(&c__1, queue_1.que + (i__ - 1) * 73, (ftnlen)73);
    }
    e_wsfe();
    cl__1.cerr = 0;
    cl__1.cunit = 16;
    cl__1.csta = 0;
    f_clos(&cl__1);
    goto L115;
L460:
    s_wsle(&io___198);
    do_lio(&c__9, &c__1, "File already exists.  Choose a different name.", (
        ftnlen)46);
    e_wsle();
    goto L421;
/* *** SAVE PICTURE AS POSTSCRIPT*** */
L510:
    ns_1.ndraw = 2;
    goto L541;
/* *** SAVE PICTURE AS HPGL*** */
L520:
    ns_1.ndraw = 3;
    goto L541;
/* *** REDRAW PICTURE *** */
L540:
    ns_1.ndraw = 1;
/* *** default instruction save */
L541:
    o__1.oerr = 0;
    o__1.ounit = 16;
    o__1.ofnmlen = 7;
    o__1.ofnm = "TEP.NEW";
    o__1.orl = 0;
    o__1.osta = "unknown";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    al__1.aerr = 0;
    al__1.aunit = queue_1.ned;
    f_rew(&al__1);
L542:
    io___199.ciunit = queue_1.ned;
    i__1 = s_rsfe(&io___199);
    if (i__1 != 0) {
    goto L543;
    }
    i__1 = do_fio(&c__1, card, (ftnlen)80);
    if (i__1 != 0) {
    goto L543;
    }
    i__1 = e_rsfe();
    if (i__1 != 0) {
    goto L543;
    }
    s_wsfe(&io___200);
    do_fio(&c__1, card, iend_(card, (ftnlen)80));
    e_wsfe();
    goto L542;
L543:
    s_wsfe(&io___201);
    i__1 = queue_1.nque;
    for (i__ = 1; i__ <= i__1; ++i__) {
    do_fio(&c__1, queue_1.que + (i__ - 1) * 73, (ftnlen)73);
    }
    e_wsfe();
    cl__1.cerr = 0;
    cl__1.cunit = 16;
    cl__1.csta = 0;
    f_clos(&cl__1);
    recycle_();
L590:
    return 0;
} /* editr_ */

/* Subroutine */ int eigen_(real *w, real *valu, real *vect)
{
    /* System generated locals */
    real r__1, r__2, r__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static real a[9]    /* was [3][3] */, b[9]  /* was [3][3] */;
    static integer i__, j, l;
    static real u[3], v[3], x, y, b0, b1, b2, c1, c0;
    static integer i1;
    static real t1;
    static integer ii, iii, lll;
    static real tem;
    extern doublereal vmv_(real *, real *, real *);
    static integer imax;
    static real vold, smax, vnew, sigma;
    extern /* Subroutine */ int axeqb_(real *, real *, real *, integer *);
    static real errnd;
    extern /* Subroutine */ int unity_(real *, real *, integer *);

/*     ***** EIGENVALUES AND EIGENVECTORS OF 3X3 MATRIX ***** */
/*     ***** STATEMENT FUNCTION ***** */
/*     ***** START OF PROGRAM ***** */
    /* Parameter adjustments */
    vect -= 4;
    --valu;
    w -= 4;

    /* Function Body */
    errnd = 5e-7f;
    sigma = 0.f;
    for (j = 1; j <= 3; ++j) {
    for (i__ = 1; i__ <= 3; ++i__) {
        tem = w[i__ + j * 3];
        a[i__ + j * 3 - 4] = tem;
/* L115: */
        sigma += tem * tem;
    }
    }
/*     ***** CHECK FOR NULL MATRIX ***** */
    if (sigma <= 0.f) {
    goto L230;
    } else {
    goto L120;
    }
L120:
    sigma = sqrt(sigma);
/*     ***** FORM CHARACTERISTIC EQUATION ***** */
    b2 = a[0] + a[4] + a[8];
    b1 = -a[0] * a[4] - a[0] * a[8] - a[4] * a[8] + a[6] * a[2] + a[7] * a[5] 
        + a[3] * a[1];
    b0 = a[0] * a[4] * a[8] + a[3] * a[7] * a[2] + a[6] * a[5] * a[1] - a[6] *
         a[2] * a[4] - a[0] * a[7] * a[5] - a[3] * a[1] * a[8];
/*     ***** FIRST ROOT BY BISECTION ***** */
    x = 0.f;
    y = sigma;
    tem = ((b2 - sigma) * sigma + b1) * sigma + b0;
    vnew = 0.f;
    if (b0 < 0.f) {
    goto L135;
    } else if (b0 == 0) {
    goto L250;
    } else {
    goto L145;
    }
L135:
    if (tem <= 0.f) {
    goto L140;
    } else {
    goto L165;
    }
L140:
    y = -y;
    goto L165;
L145:
    y = 0.f;
    x = sigma;
    if (tem <= 0.f) {
    goto L165;
    } else {
    goto L150;
    }
L150:
    x = -x;
/*     ***** NOW PHIF(X).LT.0.AND.PHIF(Y).GT.0. ***** */
L165:
    vnew = (x + y) * .5f;
    for (i__ = 1; i__ <= 40; ++i__) {
/* L175: */
    if ((r__1 = ((b2 - vnew) * vnew + b1) * vnew + b0) < 0.f) {
        goto L180;
    } else if (r__1 == 0) {
        goto L250;
    } else {
        goto L185;
    }
L180:
    x = vnew;
    goto L200;
L185:
    y = vnew;
L200:
    vold = vnew;
    vnew = (x + y) * .5f;
    tem = (r__1 = vold - vnew, dabs(r__1));
    if (tem - errnd <= 0.f) {
        goto L250;
    } else {
        goto L205;
    }
L205:
    if (vold != 0.f) {
        goto L210;
    } else {
        goto L225;
    }
L210:
    if ((r__1 = tem / vold, dabs(r__1)) - errnd <= 0.f) {
        goto L250;
    } else {
        goto L225;
    }
L225:
    ;
    }
/*     ***** DID NOT CONVERGE, SET ERROR INDICATOR ***** */
L230:
    _BLNK__2.ng = 6;
    goto L400;
/*     ***** STORE FIRST ROOT ***** */
L250:
    u[2] = vnew;
/*     ***** DEFLATE ***** */
    c1 = b2 - vnew;
    c0 = b1 + c1 * vnew;
/*     ***** SOLVE QUADRATIC ***** */
    tem = c1 * c1 + c0 * 4.f;
    if (tem < 0.f) {
    goto L255;
    } else if (tem == 0) {
    goto L265;
    } else {
    goto L260;
    }
/*     ***** IGNORE IMAGINARY COMPONENT OF COMPLEX ROOT ***** */
L255:
    tem = 0.f;
    goto L265;
L260:
    tem = sqrt(tem);
L265:
    u[0] = (c1 - tem) * .5f;
    u[1] = (c1 + tem) * .5f;
/*     ***** SORT ROOTS ***** */
    for (j = 1; j <= 2; ++j) {
    if (u[j - 1] - u[2] <= 0.f) {
        goto L275;
    } else {
        goto L270;
    }
L270:
    tem = u[j - 1];
    u[j - 1] = u[2];
    u[2] = tem;
L275:
    ;
    }
    lll = -2;
    for (iii = 1; iii <= 2; ++iii) {
/*     ***** CHECK FOR MULTIPLE ROOTS ***** */
    tem = errnd * 100.f;
    _BLNK__2.ng = 0;
    l = 1;
    for (i__ = 1; i__ <= 2; ++i__) {
        if (u[i__] - u[i__ - 1] - tem <= 0.f) {
        goto L300;
        } else {
        goto L290;
        }
L290:
        if (u[i__ - 1] != 0.f) {
        goto L295;
        } else {
        goto L305;
        }
L295:
        if ((r__1 = (u[i__] - u[i__ - 1]) / u[i__ - 1], dabs(r__1)) - tem 
            <= 0.f) {
        goto L300;
        } else {
        goto L305;
        }
L300:
        --l;
        _BLNK__2.ng -= i__ << 1;
L305:
        ;
    }
    if (lll - l >= 0) {
        goto L400;
    } else {
        goto L308;
    }
L308:
    lll = l;
/*     ***** EIGENVECTOR ROUTINE ***** */
    for (ii = 1; ii <= 3; ++ii) {
        t1 = u[ii - 1];
        if (l < 0) {
        goto L315;
        } else if (l == 0) {
        goto L310;
        } else {
        goto L322;
        }
/*     ***** TWO VECTORS NULL FOR DOUBLE ROOT ***** */
L310:
        if (_BLNK__2.ng + 5 - ii != 0) {
        goto L315;
        } else {
        goto L322;
        }
/*     ***** ALL VECTORS NULL FOR TRIPLE ROOT ***** */
L315:
        for (j = 1; j <= 3; ++j) {
/* L320: */
        vect[j + ii * 3] = 0.f;
        }
        goto L375;
L322:
        for (j = 1; j <= 3; ++j) {
/* L325: */
        a[j + j * 3 - 4] = w[j + j * 3] - t1;
        }
        smax = 0.f;
        for (i__ = 1; i__ <= 3; ++i__) {
        i1 = 1;
        if (i__ - 2 <= 0) {
            goto L335;
        } else {
            goto L340;
        }
L335:
        i1 = i__ + 1;
L340:
        b[i__ - 1] = a[i__ + 2] * a[i1 + 5] - a[i__ + 5] * a[i1 + 2];
        b[i__ + 2] = a[i__ + 5] * a[i1 - 1] - a[i__ - 1] * a[i1 + 5];
        b[i__ + 5] = a[i__ - 1] * a[i1 + 2] - a[i__ + 2] * a[i1 - 1];
/* Computing 2nd power */
        r__1 = b[i__ - 1];
/* Computing 2nd power */
        r__2 = b[i__ + 2];
/* Computing 2nd power */
        r__3 = b[i__ + 5];
        tem = r__1 * r__1 + r__2 * r__2 + r__3 * r__3;
        if (tem - smax <= 0.f) {
            goto L355;
        } else {
            goto L350;
        }
L350:
        smax = tem;
        imax = i__;
L355:
        ;
        }
        if (smax <= 0.f) {
        goto L353;
        } else {
        goto L360;
        }
L353:
        _BLNK__2.ng = 7;
        goto L375;
L360:
        smax = sqrt(smax);
        for (j = 1; j <= 3; ++j) {
/* L365: */
        v[j - 1] = b[imax + j * 3 - 4] / smax;
        }
/*     ***** REFINE EIGENVECTOR ***** */
        axeqb_(a, v, v, &c__1);
/* Computing MAX */
        r__1 = dabs(v[0]), r__2 = dabs(v[1]), r__1 = max(r__1,r__2), r__2 
            = dabs(v[2]);
        tem = dmax(r__1,r__2);
        for (j = 1; j <= 3; ++j) {
/* L370: */
        v[j - 1] /= tem;
        }
        unity_(v, &vect[ii * 3 + 1], &c__1);
/*     ***** REFINE EIGENVALUE ***** */
        t1 = vmv_(&vect[ii * 3 + 1], &w[4], &vect[ii * 3 + 1]);
        u[ii - 1] = t1;
L375:
        valu[ii] = t1;
    }
    }
L400:
    return 0;
} /* eigen_ */

/* Subroutine */ int erpnt_(doublereal *td, integer *n)
{
    /* Initialized data */

    static char msg[63*16] = "No sentinel found after reading 96 symmetry ca"
        "rds              " "No sentinel found after reading parameter ca"
        "rds for 100 atoms  " "Aniso temp factor coefs form non-positive "
        "definite matrix      " "Symmetry operation no. is higher than no"
        ". of input operations  " "Atom number is higher than the number "
        "of input atoms           " "Null temp factor matrix or failure i"
        "n bisection routine        " "Eigenvector routine failure due to"
        " null vector                 " "Error initializing screen driver"
        "                               " "Unidentified instruction numbe"
        "r                                " "Atom out of bounds          "
        "                                   " "No vector search codes    "
        "                                     " "Insufficient number of a"
        "toms in ATOMS list                     " "Imaginary bond interse"
        "ction (i.e., bond larger than atom)      " "Hidden (end-on) bond"
        "                                           " "Null vector as bas"
        "e line                                       " "ATOMS list is fu"
        "ll                                             ";

    /* Format strings */
    static char fmt_115[] = "(\002 \002,10x,\002FAULT NG =\002,i3,f10.0,i6)";
    static char fmt_116[] = "(\002 \002,10x,a,/)";

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Fortran I/O blocks */
    static cilist io___229 = { 0, 0, 0, fmt_115, 0 };
    static cilist io___230 = { 0, 0, 0, fmt_116, 0 };


    if (_BLNK__1.nout >= 0) {
    io___229.ciunit = _BLNK__1.nout;
    s_wsfe(&io___229);
    do_fio(&c__1, (char *)&_BLNK__1.ng, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*td), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
    e_wsfe();
    io___230.ciunit = _BLNK__1.nout;
    s_wsfe(&io___230);
    do_fio(&c__1, msg + (_BLNK__1.ng - 1) * 63, (ftnlen)63);
    e_wsfe();
    }
    _BLNK__1.ng = 0;
    return 0;
} /* erpnt_ */

/* Subroutine */ int exitng_(integer *ing)
{
    /* Initialized data */

    static char routin[12*16] = "PRELIM      " "PRELIM      " "PRELIM      " 
        "ATOM, PAXES " "ATOM, PAXES " "EIGEN       " "EIGEN       " "INI"
        "Txx      " "MAIN, SPARE " "BOND, F700  " "F800        " "F600, S"
        "EARCH" "BOND        " "BOND        " "F900        " "STORE       "
        ;
    static char msg[63*16] = "No sentinel found after reading 96 symmetry ca"
        "rds              " "No sentinel found after reading parameter ca"
        "rds for 100 atoms  " "Aniso temp factor coefs form non-positive "
        "definite matrix      " "Symmetry operation no. is higher than no"
        ". of input operations  " "Atom number is higher than the number "
        "of input atoms           " "Null temp factor matrix or failure i"
        "n bisection routine        " "Eigenvector routine failure due to"
        " null vector                 " "Error initializing screen driver"
        "                               " "Unidentified instruction numbe"
        "r                                " "Atom out of bounds          "
        "                                   " "No vector search codes    "
        "                                     " "Insufficient number of a"
        "toms in ATOMS list                     " "Imaginary bond interse"
        "ction (i.e., bond larger than atom)      " "Hidden (end-on) bond"
        "                                           " "Null vector as bas"
        "e line                                       " "ATOMS list is fu"
        "ll                                             ";

    /* Format strings */
    static char fmt_101[] = "(\002Fault Indicator:  \002,i2)";
    static char fmt_102[] = "(\002Subroutine(s) Involved:  \002,a)";
    static char fmt_103[] = "(\002Fault:  \002,a)";

    /* System generated locals */
    cllist cl__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void),
         f_clos(cllist *);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Fortran I/O blocks */
    static cilist io___233 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___234 = { 0, 0, 0, fmt_102, 0 };
    static cilist io___235 = { 0, 0, 0, fmt_103, 0 };
    static cilist io___236 = { 0, 6, 0, fmt_101, 0 };
    static cilist io___237 = { 0, 6, 0, fmt_102, 0 };
    static cilist io___238 = { 0, 6, 0, fmt_103, 0 };


    if (_BLNK__1.ng > 0) {
    if (_BLNK__1.nout > 0) {
        io___233.ciunit = _BLNK__1.nout;
        s_wsfe(&io___233);
        do_fio(&c__1, (char *)&(*ing), (ftnlen)sizeof(integer));
        e_wsfe();
        io___234.ciunit = _BLNK__1.nout;
        s_wsfe(&io___234);
        do_fio(&c__1, routin + (*ing - 1) * 12, (ftnlen)12);
        e_wsfe();
        io___235.ciunit = _BLNK__1.nout;
        s_wsfe(&io___235);
        do_fio(&c__1, msg + (*ing - 1) * 63, (ftnlen)63);
        e_wsfe();
    }
    s_wsfe(&io___236);
    do_fio(&c__1, (char *)&(*ing), (ftnlen)sizeof(integer));
    e_wsfe();
    s_wsfe(&io___237);
    do_fio(&c__1, routin + (*ing - 1) * 12, (ftnlen)12);
    e_wsfe();
    s_wsfe(&io___238);
    do_fio(&c__1, msg + (*ing - 1) * 63, (ftnlen)63);
    e_wsfe();
    }
    if (_BLNK__1.nout > 0) {
    cl__1.cerr = 0;
    cl__1.cunit = _BLNK__1.nout;
    cl__1.csta = "KEEP";
    f_clos(&cl__1);
    }
    s_stop("", (ftnlen)0);
    return 0;
} /* exitng_ */

/* Subroutine */ int f200_(void)
{
    /* Format strings */
    static char fmt_203[] = "(\002TRN\002,2(1x,f10.6))";

    /* System generated locals */
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    integer f_open(olist *), f_clos(cllist *), s_wsfe(cilist *), do_fio(
        integer *, char *, ftnlen), e_wsfe(void);

    /* Local variables */
    static real penw;
    extern /* Subroutine */ int endsc_(void), endhp_(void), endps_(void), 
        colrsc_(integer *);
    static integer icolor;
    extern /* Subroutine */ int colrhp_(integer *), initsc_(void), inithp_(
        void), penwsc_(real *), penwhp_(real *), colrps_(integer *), 
        initps_(void), penwps_(real *);

    /* Fortran I/O blocks */
    static cilist io___239 = { 0, 0, 0, fmt_203, 0 };


/* *** NO drawing */
    if (ns_1.ndraw == 0) {
    return 0;
    }
    switch (_BLNK__1.nj2) {
    case 1:  goto L201;
    case 2:  goto L202;
    case 3:  goto L201;
    case 4:  goto L204;
    case 5:  goto L205;
    }
/* *** initialize plotting (201 or 203 inst) *** */
L201:
    trfac_1.xtrans = 0.f;
    trfac_1.ytrans = 0.f;
    if (ns_1.ndraw == 1) {
    initsc_();
    }
    if (ns_1.ndraw == 2) {
    initps_();
    }
    if (ns_1.ndraw == 3) {
    inithp_();
    }
    if (ns_1.ndraw == 9) {
    o__1.oerr = 0;
    o__1.ounit = ns_1.npf;
    o__1.ofnmlen = 7;
    o__1.ofnm = "TEP.EDT";
    o__1.orl = 0;
    o__1.osta = "unknown";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    ns_1.nvar = 1;
    }
    return 0;
/* *** change origin of plotting area or terminate (202 inst) *** */
L202:
    if (_BLNK__1.ain[0] == 0.f && _BLNK__1.ain[1] == 0.f) {
    if (ns_1.ndraw == 2) {
        endps_();
    }
    if (ns_1.ndraw == 3) {
        endhp_();
    }
    if (ns_1.ndraw == 1) {
        endsc_();
    }
    if (ns_1.ndraw == 9) {
        cl__1.cerr = 0;
        cl__1.cunit = ns_1.npf;
        cl__1.csta = 0;
        f_clos(&cl__1);
    }
    } else {
    trfac_1.xtrans = _BLNK__1.ain[0];
    trfac_1.ytrans = _BLNK__1.ain[1];
    if (ns_1.ndraw == 9) {
        io___239.ciunit = ns_1.npf;
        s_wsfe(&io___239);
        do_fio(&c__1, (char *)&trfac_1.xtrans, (ftnlen)sizeof(real));
        do_fio(&c__1, (char *)&trfac_1.ytrans, (ftnlen)sizeof(real));
        e_wsfe();
    }
    }
    return 0;
/* *** change plot color (204 inst) *** */
L204:
    icolor = (integer) _BLNK__1.ain[0];
    if (ns_1.ndraw == 1) {
    colrsc_(&icolor);
    }
    if (ns_1.ndraw == 2) {
    colrps_(&icolor);
    }
    if (ns_1.ndraw == 3) {
    colrhp_(&icolor);
    }
    if (ns_1.ndraw == 9) {
    colrsc_(&icolor);
    }
    return 0;
/* *** change pen width (205 inst) *** */
/* *** parameter units are thousandths of an inch (default=5) */
L205:
    penw = _BLNK__1.ain[0];
    if (ns_1.ndraw == 1) {
    penwsc_(&penw);
    }
    if (ns_1.ndraw == 2) {
    penwps_(&penw);
    }
    if (ns_1.ndraw == 3) {
    penwhp_(&penw);
    }
    if (ns_1.ndraw == 9) {
    penwsc_(&penw);
    }
    return 0;
} /* f200_ */

/* Subroutine */ int f400_(void)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5, i__6;
    doublereal d__1;

    /* Builtin functions */
    double d_mod(doublereal *, doublereal *);

    /* Local variables */
    static integer i__, j, l1, m1, m2, m3, m4, m5, n1, n2, n3, n4, n5, l5, l4,
         l3, l2, ii, jj;
    static doublereal td, d100;
    static integer id1, id2;
    static doublereal td1, td2, d1000, d100k;
    extern /* Subroutine */ int atom_(doublereal *, real *), stor_(doublereal 
        *), searc_(void), erpnt_(doublereal *, integer *);

/*     ***** ATOM LIST FUNCTIONS ***** */
    d100 = 100.f;
    d1000 = 1e3f;
    d100k = 1e5f;
    _BLNK__1.ng = 0;
    if (_BLNK__1.latm <= 0) {
    goto L402;
    } else {
    goto L400;
    }
L400:
    i__1 = _BLNK__1.latm;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L401: */
    atom_(&_BLNK__1.atomid[i__ - 1], &_BLNK__1.atoms[i__ * 3 - 3]);
    }
L402:
    if ((i__1 = _BLNK__1.nj2 % 10 - 1) < 0) {
    goto L499;
    } else if (i__1 == 0) {
    goto L404;
    } else {
    goto L403;
    }
L403:
    if ((i__1 = _BLNK__1.nj2 % 10 - 7) < 0) {
    goto L406;
    } else if (i__1 == 0) {
    goto L404;
    } else {
    goto L499;
    }
L406:
    searc_();
    goto L499;
/*     ***** STORES (401) OR REMOVES (411) RUNS OF ATOMS ***** */
/*     ***** RUN HIERARCHY = ATOM NO./SYM/ A/B/C TRANS. ***** */
L404:
    ii = 1;
/*     ***** FIND RUNS IN AIN ARRAY ***** */
L405:
    td1 = _BLNK__1.ain[ii - 1];
    if (td1 <= 0.) {
    goto L410;
    } else {
    goto L420;
    }
L410:
    ++ii;
    if (140 - ii >= 0) {
    goto L405;
    } else {
    goto L499;
    }
L420:
    jj = ii;
/*     ***** SET INITIAL RUN VALUES ***** */
    m1 = (integer) (td1 / d100k);
    m2 = (integer) d_mod(&td1, &d100);
    d__1 = td1 / d100;
    m5 = (integer) d_mod(&d__1, &d1000);
    if (m5 <= 0) {
    goto L422;
    } else {
    goto L423;
    }
L422:
    m5 = 555;
L423:
    m3 = m5 / 100;
    m4 = m5 / 10 % 10;
    m5 %= 10;
L425:
    ++jj;
    if (140 - jj >= 0) {
    goto L430;
    } else {
    goto L435;
    }
L430:
    td2 = -_BLNK__1.ain[jj - 1];
    if (td2 < 0.) {
    goto L435;
    } else if (td2 == 0) {
    goto L425;
    } else {
    goto L440;
    }
L435:
    ii = jj - 1;
/*     ***** SET TERMINAL VALUES FOR DEGENERATE RUN ***** */
    n1 = m1;
    n2 = m2;
    n3 = m3;
    n4 = m4;
    n5 = m5;
    goto L450;
L440:
    ii = jj;
/*     ***** SET TERMINAL RUN VALUES ***** */
    n1 = (integer) (td2 / d100k);
    n2 = (integer) d_mod(&td2, &d100);
    d__1 = td2 / d100;
    n5 = (integer) d_mod(&d__1, &d1000);
    if (n5 <= 0) {
    goto L445;
    } else {
    goto L446;
    }
L445:
    n5 = 555;
L446:
    n3 = n5 / 100;
    n4 = n5 / 10 % 10;
    n5 %= 10;
/*     ***** LOOP THROUGH ALL RUNS ***** */
L450:
    i__1 = n5;
    for (l5 = m5; l5 <= i__1; ++l5) {
    i__2 = n4;
    for (l4 = m4; l4 <= i__2; ++l4) {
        i__3 = n3;
        for (l3 = m3; l3 <= i__3; ++l3) {
        i__4 = n2;
        for (l2 = m2; l2 <= i__4; ++l2) {
            i__5 = n1;
            for (l1 = m1; l1 <= i__5; ++l1) {
            td = (doublereal) l1 * d100k + (doublereal) (l3 * 
                10000 + l4 * 1000 + l5 * 100 + l2);
            atom_(&td, _BLNK__1.v1);
            if (_BLNK__1.ng != 0) {
                goto L455;
            } else {
                goto L458;
            }
L455:
            erpnt_(&td, &c__401);
            goto L490;
/*     ***** CHECK IDENT CODE IF 407/417 INSTRUCTION ***** */
L458:
            if ((i__6 = _BLNK__1.nj2 % 10 - 7) < 0) {
                goto L475;
            } else if (i__6 == 0) {
                goto L460;
            } else {
                goto L490;
            }
L460:
            id1 = parms_1.ident[(l1 << 1) - 2];
            id2 = parms_1.ident[(l1 << 1) - 1];
            if (_BLNK__1.ncd <= 0) {
                goto L490;
            } else {
                goto L465;
            }
L465:
            i__6 = _BLNK__1.ncd;
            for (j = 1; j <= i__6; ++j) {
                if ((real) _BLNK__1.kd[j * 5 - 5] > 0.f && (real) 
                    _BLNK__1.kd[j * 5 - 3] > 0.f) {
                if (id1 >= _BLNK__1.kd[j * 5 - 5] && id1 <= 
                    _BLNK__1.kd[j * 5 - 4] && (id2 >= 
                    _BLNK__1.kd[j * 5 - 3] && id2 <= 
                    _BLNK__1.kd[j * 5 - 2])) {
                    goto L475;
                }
                } else if ((real) _BLNK__1.kd[j * 5 - 5] > 0.f) {
                if (id1 >= _BLNK__1.kd[j * 5 - 5] && id1 <= 
                    _BLNK__1.kd[j * 5 - 4]) {
                    goto L475;
                }
                } else if ((real) _BLNK__1.kd[j * 5 - 3] > 0.f) {
                if (id2 >= _BLNK__1.kd[j * 5 - 3] && id2 <= 
                    _BLNK__1.kd[j * 5 - 2]) {
                    goto L475;
                }
                }
/*     IF(ID1-KD(1,J))470,467,467 */
/* 467 IF(KD(2,J)-ID1)470,468,468 */
/* 468 IF(ID2-KD(3,J))470,469,469 */
/* 469 IF(KD(4,J)-ID2)470,475,475 */
/* L470: */
            }
            goto L490;
L475:
            stor_(&td);
L490:
            ;
            }
        }
        }
    }
    }
    goto L410;
L499:
    return 0;
} /* f400_ */

/* Subroutine */ int f500_(void)
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j, k, l, n;
    static real v[12]   /* was [3][4] */, x;
    static integer i1, i2, i3;
    static real t1, t2;
    static doublereal td;
    extern /* Subroutine */ int mm_(real *, real *, real *);
    static real rm[9]   /* was [3][3] */;
    static doublereal d100, d1000;
    static integer ind, kkk;
    static real awt;
    extern doublereal vmv_(real *, real *, real *);
    static doublereal d100k;
    static real pat1[9] /* was [3][3] */;
    extern /* Subroutine */ int atom_(doublereal *, real *), axes_(real *, 
        real *, real *, integer *), lap500_(integer *), eigen_(real *, 
        real *, real *), erpnt_(doublereal *, integer *), unity_(real *, 
        real *, integer *), exitng_(integer *);

    _BLNK__1.ng = 0;
    d100 = 100.f;
    d1000 = 1e3f;
    d100k = 1e5f;
    if ((i__1 = _BLNK__1.nj2 - 11) < 0) {
    goto L500;
    } else if (i__1 == 0) {
    goto L700;
    } else {
    goto L710;
    }
L500:
    if ((i__1 = _BLNK__1.nj2 - 1) < 0) {
    goto L710;
    } else if (i__1 == 0) {
    goto L501;
    } else {
    goto L510;
    }
L501:
    td = _BLNK__1.ain[0];
    atom_(&td, _BLNK__1.orgn);
    if (_BLNK__1.ng != 0) {
    goto L502;
    } else {
    goto L504;
    }
L502:
    erpnt_(&td, &c__501);
    exitng_(&_BLNK__1.ng);
L504:
    for (k = 1; k <= 4; ++k) {
    td = _BLNK__1.ain[k];
    atom_(&td, &v[k * 3 - 3]);
    if (_BLNK__1.ng != 0) {
        goto L502;
    } else {
        goto L506;
    }
L506:
    ;
    }
    for (j = 1; j <= 3; ++j) {
    _BLNK__1.v1[j - 1] = v[j + 2] - v[j - 1];
/* L507: */
    _BLNK__1.v2[j - 1] = v[j + 8] - v[j + 5];
    }
    ind = -1;
    if (_BLNK__1.ain[6] <= 0.) {
    goto L509;
    } else {
    goto L508;
    }
L508:
    ind = -2;
L509:
    axes_(_BLNK__1.v1, _BLNK__1.v2, _BLNK__1.refv, &ind);
    goto L670;
L510:
    if ((i__1 = _BLNK__1.nj2 - 4) < 0) {
    goto L515;
    } else if (i__1 == 0) {
    goto L511;
    } else {
    goto L599;
    }
/*      ***** SHIFT ORIGIN FOR PROJECTION AXIS (IN INCHES) ***** */
L511:
    for (j = 1; j <= 3; ++j) {
    for (k = 1; k <= 3; ++k) {
        t1 = _BLNK__1.ain[k - 1];
/* L512: */
        _BLNK__1.orgn[j - 1] += _BLNK__1.refv[j + k * 3 - 4] * t1 / 
            _BLNK__1.scal1;
    }
    t2 = _BLNK__1.ain[j - 1];
/* L513: */
    _BLNK__1.xo[j - 1] += t2;
    }
    goto L675;
/*      ***** FORM ROTATION MATRIX ***** */
L515:
    for (j = 1; j <= 3; ++j) {
    for (k = 1; k <= 3; ++k) {
/* L514: */
        v[j + k * 3 - 4] = _BLNK__1.refv[j + k * 3 - 4];
    }
    }
    for (l = 1; l <= 139; l += 2) {
    i__ = (integer) _BLNK__1.ain[l - 1];
    if (i__ < 0) {
        goto L532;
    } else if (i__ == 0) {
        goto L519;
    } else {
        goto L516;
    }
L516:
    x = _BLNK__1.ain[l] * .017453293;
    t1 = cos(x);
    t2 = sin(x);
    i3 = (i__ + 2) % 3 + 1;
    i1 = i3 % 3 + 1;
    i2 = i1 % 3 + 1;
    rm[i1 + i1 * 3 - 4] = t1;
    rm[i1 + i2 * 3 - 4] = t2;
    rm[i1 + i3 * 3 - 4] = 0.f;
    rm[i2 + i1 * 3 - 4] = -t2;
    rm[i2 + i2 * 3 - 4] = t1;
    rm[i2 + i3 * 3 - 4] = 0.f;
    rm[i3 + i1 * 3 - 4] = 0.f;
    rm[i3 + i2 * 3 - 4] = 0.f;
    rm[i3 + i3 * 3 - 4] = 1.f;
/* L517: */
    mm_(v, rm, v);
    }
L519:
    if ((i__1 = _BLNK__1.nj2 - 3) < 0) {
    goto L518;
    } else if (i__1 == 0) {
    goto L525;
    } else {
    goto L599;
    }
L518:
    for (j = 1; j <= 3; ++j) {
    for (i__ = 1; i__ <= 3; ++i__) {
/* L522: */
        _BLNK__1.refv[i__ + j * 3 - 4] = v[i__ + j * 3 - 4];
    }
    }
    goto L552;
L525:
    for (j = 1; j <= 3; ++j) {
    for (i__ = 1; i__ <= 3; ++i__) {
/* L528: */
        _BLNK__1.wrkv[i__ + j * 3 - 4] = v[i__ + j * 3 - 4];
    }
    }
    goto L552;
L532:
    if ((i__1 = _BLNK__1.nj2 - 3) < 0) {
    goto L535;
    } else if (i__1 == 0) {
    goto L552;
    } else {
    goto L599;
    }
L535:
    i__ = -i__ % 3;
    i__1 = i__;
    for (j = 1; j <= i__1; ++j) {
    for (k = 1; k <= 3; ++k) {
        t1 = _BLNK__1.refv[k + 5];
        _BLNK__1.refv[k + 5] = _BLNK__1.refv[k + 2];
        _BLNK__1.refv[k + 2] = _BLNK__1.refv[k - 1];
/* L542: */
        _BLNK__1.refv[k - 1] = t1;
    }
    }
L552:
    if ((i__1 = _BLNK__1.nj2 - 3) < 0) {
    goto L670;
    } else if (i__1 == 0) {
    goto L582;
    } else {
    goto L599;
    }
L582:
    mm_(_BLNK__1.aa, _BLNK__1.wrkv, _BLNK__1.aawrk);
    goto L699;
L599:
    if ((i__1 = _BLNK__1.nj2 - 5) < 0) {
    goto L699;
    } else if (i__1 == 0) {
    goto L600;
    } else {
    goto L607;
    }
L600:
    if (_BLNK__1.latm - 1 >= 0) {
    goto L610;
    } else {
    goto L605;
    }
L605:
    _BLNK__1.ng = 12;
L606:
    erpnt_(&c_b113, &c__506);
    exitng_(&_BLNK__1.ng);
L607:
    if ((i__1 = _BLNK__1.nj2 - 6) < 0) {
    goto L699;
    } else if (i__1 == 0) {
    goto L608;
    } else {
    goto L710;
    }
L608:
    if (_BLNK__1.latm - 3 >= 0) {
    goto L610;
    } else {
    goto L605;
    }
L610:
    for (j = 1; j <= 3; ++j) {
    _BLNK__1.v2[j - 1] = 0.f;
    for (i__ = 1; i__ <= 3; ++i__) {
/* L612: */
        rm[i__ + j * 3 - 4] = 0.f;
    }
    }
    awt = 0.f;
    i__1 = _BLNK__1.latm;
    for (k = 1; k <= i__1; ++k) {
    atom_(&_BLNK__1.atomid[k - 1], &_BLNK__1.atoms[k * 3 - 3]);
    t2 = 1.f;
    if (_BLNK__1.ncd <= 0) {
        goto L618;
    } else {
        goto L613;
    }
L613:
    i1 = (integer) (_BLNK__1.atomid[k - 1] / d100k);
    i__2 = _BLNK__1.ncd;
    for (j = 1; j <= i__2; ++j) {
        if (_BLNK__1.kd[j * 5 - 1] == 1) {
        i1 = parms_1.ident[(k << 1) - 2];
        }
        if (_BLNK__1.kd[j * 5 - 1] == 2) {
        i1 = parms_1.ident[(k << 1) - 1];
        }
        t2 = _BLNK__1.cd[(j << 3) - 8];
        if (i1 - _BLNK__1.kd[j * 5 - 5] >= 0) {
        goto L614;
        } else {
        goto L616;
        }
L614:
        if (_BLNK__1.kd[j * 5 - 4] - i1 >= 0) {
        goto L618;
        } else {
        goto L616;
        }
L616:
        ;
    }
    goto L620;
L618:
    awt += t2;
    for (j = 1; j <= 3; ++j) {
/* L619: */
        _BLNK__1.v2[j - 1] += _BLNK__1.atoms[j + k * 3 - 4] * t2;
    }
L620:
    ;
    }
    if (awt <= 0.f) {
    goto L605;
    } else {
    goto L621;
    }
/*     ***** PUT ORIGIN AT CENTER OF GRAVITY ***** */
L621:
    for (j = 1; j <= 3; ++j) {
/* L622: */
    _BLNK__1.orgn[j - 1] = _BLNK__1.v2[j - 1] / awt;
    }
    if ((i__1 = _BLNK__1.nj2 - 6) < 0) {
    goto L699;
    } else if (i__1 == 0) {
    goto L624;
    } else {
    goto L710;
    }
/*     ***** FORM PRODUCT-MOMENT MATRIX FOR ATOMS IN ATOM LIST ***** */
L624:
    i__1 = _BLNK__1.latm;
    for (k = 1; k <= i__1; ++k) {
    t2 = 1.f;
    if (_BLNK__1.ncd <= 0) {
        goto L628;
    } else {
        goto L625;
    }
L625:
    i1 = (integer) (_BLNK__1.atomid[k - 1] / d100k);
    i__2 = _BLNK__1.ncd;
    for (j = 1; j <= i__2; ++j) {
        if (_BLNK__1.kd[j * 5 - 1] == 1) {
        i1 = parms_1.ident[(k << 1) - 2];
        }
        if (_BLNK__1.kd[j * 5 - 1] == 2) {
        i1 = parms_1.ident[(k << 1) - 1];
        }
        t2 = _BLNK__1.cd[(j << 3) - 8];
        if (i1 - _BLNK__1.kd[j * 5 - 5] >= 0) {
        goto L626;
        } else {
        goto L627;
        }
L626:
        if (_BLNK__1.kd[j * 5 - 4] - i1 >= 0) {
        goto L628;
        } else {
        goto L627;
        }
L627:
        ;
    }
    goto L630;
L628:
    for (j = 1; j <= 3; ++j) {
        t1 = (_BLNK__1.atoms[j + k * 3 - 4] - _BLNK__1.orgn[j - 1]) * t2;
        for (i__ = 1; i__ <= 3; ++i__) {
/* L629: */
        rm[i__ + j * 3 - 4] = t1 * (_BLNK__1.atoms[i__ + k * 3 - 4] - 
            _BLNK__1.orgn[i__ - 1]) + rm[i__ + j * 3 - 4];
        }
    }
L630:
    ;
    }
/*     ***** SCALE PRODUCT-MOMENT MATRIX ***** */
    t1 = .03f / (rm[0] + rm[4] + rm[8]);
    for (j = 1; j <= 3; ++j) {
    for (i__ = 1; i__ <= 3; ++i__) {
/* L632: */
        rm[i__ + j * 3 - 4] *= t1;
    }
    }
/*     ***** TRANSFORM TO INERTIAL AXES SYSTEM ***** */
/*     ***** RELATED TO PRINCIPAL AXES CALC IN PRELIM ***** */
    mm_(rm, _BLNK__1.aa, _BLNK__1.da);
    eigen_(_BLNK__1.da, _BLNK__1.rms, _BLNK__1.pat);
    if (_BLNK__1.rms[1] <= 0.f) {
    goto L605;
    } else {
    goto L635;
    }
L635:
    if (_BLNK__1.ng < 0) {
    goto L640;
    } else if (_BLNK__1.ng == 0) {
    goto L633;
    } else {
    goto L606;
    }
/*     ***** 3 UNIQUE EIGENVECTORS --> NEW REFERENCE VECTORS * */
/*     ***** RIGHT-HANDED WITH X LONGEST, Z SHORTEST **** */
L633:
    axes_(&_BLNK__1.pat[6], _BLNK__1.pat, _BLNK__1.refv, &c_n1);
    goto L670;
L640:
    if (_BLNK__1.ng + 6 <= 0) {
    goto L665;
    } else {
    goto L645;
    }
/*     ***** TWO EQUAL EIGENVALUES (2 DIFFERENT CASES) ***** */
L645:
    n = _BLNK__1.ng + 5;
    unity_(&_BLNK__1.pat[n * 3 - 3], _BLNK__1.v1, &c_n1);
    for (k = 1; k <= 3; ++k) {
    kkk = k;
    if ((r__1 = vmv_(_BLNK__1.v1, _BLNK__1.aa, &_BLNK__1.refv[k * 3 - 3]),
         dabs(r__1)) - .58f >= 0.f) {
        goto L650;
    } else {
        goto L655;
    }
L650:
    ;
    }
L655:
    axes_(_BLNK__1.v1, &_BLNK__1.refv[kkk * 3 - 3], _BLNK__1.da, &c_n1);
    for (k = 1; k <= 3; ++k) {
    l = (n + k - 2) % 3 + 1;
    for (j = 1; j <= 3; ++j) {
/* L660: */
        pat1[j + l * 3 - 4] = _BLNK__1.da[j + k * 3 - 4];
    }
    }
L665:
    _BLNK__1.ng = 0;
/*     ***** RIGHT-HANDED WITH X LONGEST, Z SHORTEST **** */
    axes_(&pat1[6], pat1, _BLNK__1.refv, &c_n1);
L670:
    mm_(_BLNK__1.aa, _BLNK__1.refv, _BLNK__1.aarev);
L675:
    for (j = 1; j <= 3; ++j) {
    for (i__ = 1; i__ <= 3; ++i__) {
        _BLNK__1.wrkv[i__ + j * 3 - 4] = _BLNK__1.refv[i__ + j * 3 - 4];
/* L680: */
        _BLNK__1.aawrk[i__ + j * 3 - 4] = _BLNK__1.aarev[i__ + j * 3 - 4];
    }
    }
/*     ***** ELIMINATE ALL PREVIOUSLY STORED OVERLAP INFORMATION **** */
/*     ***** (ALL INSTRUCTIONS FROM 501 THROUGH 510 DO THIS ***** */
L699:
    lap500_(&c__0);
    goto L710;
/*     ***** STORE NEW OVERLAP INFORMATION (INSTRUCTION 511) ***** */
L700:
    lap500_(&c__1);
L710:
    return 0;
} /* f500_ */

/* Subroutine */ int f600_(void)
{
    /* Initialized data */

    static real crtval[99] = { .3389f,.4299f,.4951f,.5479f,.5932f,.6334f,
        .6699f,.7035f,.7349f,.7644f,.7924f,.8192f,.8447f,.8694f,.8932f,
        .9162f,.9386f,.9605f,.9818f,1.0026f,1.023f,1.043f,1.0627f,1.0821f,
        1.1012f,1.12f,1.1386f,1.157f,1.1751f,1.1932f,1.211f,1.2288f,
        1.2464f,1.2638f,1.2812f,1.2985f,1.3158f,1.333f,1.3501f,1.3672f,
        1.3842f,1.4013f,1.4183f,1.4354f,1.4524f,1.4695f,1.4866f,1.5037f,
        1.5209f,1.5382f,1.5555f,1.5729f,1.5904f,1.608f,1.6257f,1.6436f,
        1.6616f,1.6797f,1.698f,1.7164f,1.7351f,1.754f,1.773f,1.7924f,
        1.8119f,1.8318f,1.8519f,1.8724f,1.8932f,1.9144f,1.936f,1.958f,
        1.9804f,2.0034f,2.0269f,2.051f,2.0757f,2.1012f,2.1274f,2.1544f,
        2.1824f,2.2114f,2.2416f,2.273f,2.3059f,2.3404f,2.3767f,2.4153f,
        2.4563f,2.5003f,2.5478f,2.5997f,2.6571f,2.7216f,2.7955f,2.8829f,
        2.9912f,3.1365f,3.3682f };

    /* System generated locals */
    integer i__1;
    real r__1;

    /* Local variables */
    static integer i__, j, m;
    static real x[3], t1, t2, t3, t4;
    static integer km, it4;
    static real del;
    static integer max__[3];
    extern /* Subroutine */ int xyz_(doublereal *, real *, integer *);
    static real scal[4], xmin[3], xmax[3];
    extern /* Subroutine */ int lap500_(integer *);
    static real smul2;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *);
    static real smult;
    extern /* Subroutine */ int exitng_(integer *);

/*     ***** SCALING AND CENTERING FUNCTIONS ***** */
/*     ***** DEL = 1. FOR INCRUMENTING FUNCTIONS ***** */
/*     ***** DEL = 0. FOR REGULAR FUNCTIONS ***** */
    del = (real) (_BLNK__1.nj2 / 10 % 2);
    _BLNK__1.nj2 %= 10;
/*     ***** EXPLICIT ORIGIN AND SCALE ***** */
    t1 = _BLNK__1.ain[0];
    if (t1 != 0.f) {
    goto L602;
    } else {
    goto L604;
    }
L602:
    _BLNK__1.xo[0] = t1 + _BLNK__1.xo[0] * del;
L604:
    t2 = _BLNK__1.ain[1];
    if (t2 != 0.f) {
    goto L606;
    } else {
    goto L608;
    }
L606:
    _BLNK__1.xo[1] = t2 + _BLNK__1.xo[1] * del;
L608:
    t3 = _BLNK__1.ain[2];
    if (t3 <= 0.f) {
    goto L612;
    } else {
    goto L609;
    }
L609:
    if (del <= 0.f) {
    goto L611;
    } else {
    goto L610;
    }
L610:
    _BLNK__1.scal1 *= t3;
    goto L612;
L611:
    _BLNK__1.scal1 = t3;
L612:
    t4 = _BLNK__1.ain[3];
    if (t4 < 0.f) {
    goto L615;
    } else if (t4 == 0) {
    goto L616;
    } else {
    goto L614;
    }
/*     ***** SET ELLIPSOID SCALE FACTOR ***** */
L614:
    _BLNK__1.scal2 = t4;
    goto L616;
L615:
    t4 = -t4;
    if (t4 > 0.f && t4 < 1.f) {
    t4 *= 100.f;
    }
    it4 = t4;
    _BLNK__1.scal2 = crtval[it4 - 1];
/*     ***** AUTOMATIC ORIGIN AND/OR SCALE ***** */
L616:
    if ((i__1 = _BLNK__1.nj2 - 2) < 0) {
    goto L790;
    } else if (i__1 == 0) {
    goto L622;
    } else {
    goto L620;
    }
L620:
    _BLNK__1.xo[0] = _BLNK__1.xlng[0] * .5f;
    _BLNK__1.xo[1] = _BLNK__1.xlng[1] * .5f;
L622:
    if (_BLNK__1.nj2 - 3 != 0) {
    goto L625;
    } else {
    goto L640;
    }
L625:
    _BLNK__1.scal1 = 1.f;
/* L630: */
    if (_BLNK__1.latm - 1 <= 0) {
    goto L635;
    } else {
    goto L640;
    }
L635:
    _BLNK__1.ng = 12;
    erpnt_(&c_b113, &c__602);
    exitng_(&_BLNK__1.ng);
L640:
    for (j = 1; j <= 3; ++j) {
    xmax[j - 1] = -1e5f;
/* L650: */
    xmin[j - 1] = 1e5f;
    }
/*     ***** FIT BOX AROUND SET OF ATOMS ***** */
    i__1 = _BLNK__1.latm;
    for (i__ = 1; i__ <= i__1; ++i__) {
    xyz_(&_BLNK__1.atomid[i__ - 1], &_BLNK__1.atoms[i__ * 3 - 3], &c__3);
    if (_BLNK__1.ng != 0) {
        goto L652;
    } else {
        goto L653;
    }
L652:
    erpnt_(&_BLNK__1.atomid[i__ - 1], &c__600);
    goto L670;
L653:
    for (j = 1; j <= 3; ++j) {
        t1 = _BLNK__1.atoms[j + i__ * 3 - 4];
        if (xmax[j - 1] - t1 >= 0.f) {
        goto L660;
        } else {
        goto L655;
        }
L655:
        xmax[j - 1] = t1;
        max__[j - 1] = i__;
L660:
        if (t1 - xmin[j - 1] >= 0.f) {
        goto L668;
        } else {
        goto L665;
        }
L665:
        xmin[j - 1] = t1;
L668:
        ;
    }
L670:
    ;
    }
/*     ***** KM=TOP ATOM ***** */
    km = max__[2];
    smult = 1.f;
    for (m = 1; m <= 5; ++m) {
    if ((i__1 = m - 2) < 0) {
        goto L740;
    } else if (i__1 == 0) {
        goto L675;
    } else {
        goto L678;
    }
/*     ***** CHECK VIEW DISTANCE ***** */
L675:
    if (_BLNK__1.view <= 0.f) {
        goto L785;
    } else {
        goto L680;
    }
L678:
    if (_BLNK__1.nj2 - 3 != 0) {
        goto L680;
    } else {
        goto L785;
    }
L680:
    t1 = _BLNK__1.atoms[km * 3 - 1] * smult;
    if (_BLNK__1.view * .5f - t1 >= 0.f) {
        goto L690;
    } else {
        goto L685;
    }
/*     ***** INCREASE VIEW DISTANCE ***** */
L685:
    _BLNK__1.view = t1 * 2.f;
/*     ***** FIND PERSPECTIVE PROJECTION LIMITS ***** */
L690:
    for (j = 1; j <= 2; ++j) {
        xmax[j - 1] = -1e5f;
/* L700: */
        xmin[j - 1] = 1e5f;
    }
    i__1 = _BLNK__1.latm;
    for (i__ = 1; i__ <= i__1; ++i__) {
        for (j = 1; j <= 3; ++j) {
/* L705: */
        x[j - 1] = _BLNK__1.atoms[j + i__ * 3 - 4] * smult;
        }
        t2 = _BLNK__1.view / (_BLNK__1.view - x[2]);
        for (j = 1; j <= 2; ++j) {
        t1 = x[j - 1] * t2;
        if (xmax[j - 1] - t1 >= 0.f) {
            goto L715;
        } else {
            goto L710;
        }
L710:
        xmax[j - 1] = t1;
L715:
        if (t1 - xmin[j - 1] >= 0.f) {
            goto L725;
        } else {
            goto L720;
        }
L720:
        xmin[j - 1] = t1;
L725:
        ;
        }
    }
/*     ***** REFINE PARAMETERS ***** */
L740:
    if ((i__1 = _BLNK__1.nj2 - 3) < 0) {
        goto L745;
    } else if (i__1 == 0) {
        goto L742;
    } else {
        goto L755;
    }
L742:
    smul2 = 1.f;
    goto L765;
/*     ***** AUTOMATIC SCALE ONLY ***** */
L745:
    for (j = 1; j <= 2; ++j) {
        t2 = _BLNK__1.xo[j - 1];
        scal[j - 1] = (_BLNK__1.brdr - t2) / xmin[j - 1];
/* L750: */
        scal[j + 1] = (_BLNK__1.xlng[j - 1] - _BLNK__1.brdr - t2) / xmax[
            j - 1];
    }
/* Computing MIN */
    r__1 = min(scal[0],scal[1]), r__1 = min(r__1,scal[2]);
    smul2 = dmin(r__1,scal[3]);
    goto L780;
/*     ***** AUTOMATIC SCALE AND POSITION ***** */
L755:
    for (j = 1; j <= 2; ++j) {
/* L760: */
        scal[j - 1] = (_BLNK__1.xlng[j - 1] - _BLNK__1.brdr * 2.f) / (
            xmax[j - 1] - xmin[j - 1]);
    }
    smul2 = dmin(scal[0],scal[1]);
/*     ***** AUTOMATIC POSITION ***** */
L765:
    for (j = 1; j <= 2; ++j) {
/* L770: */
        _BLNK__1.xo[j - 1] = (_BLNK__1.xlng[j - 1] - smul2 * (xmax[j - 1] 
            + xmin[j - 1])) * .5f;
    }
L780:
    smult *= smul2;
    }
    _BLNK__1.view *= smul2;
L785:
    _BLNK__1.scal1 *= smult;
L790:
    _BLNK__1.scl = _BLNK__1.scal1 * _BLNK__1.scal2;
/*     ***** ELIMINATE ALL PREVIOUSLY STORED OVERLAP INFORMATION ***** */
    lap500_(&c__0);
    return 0;
} /* f600_ */

/* Subroutine */ int f700_(void)
{
    /* Format strings */
    static char fmt_750[] = "(\002 \002,10x,a6,\002  (\002,i3,\002,\002,i3,i"
        "2,\002)   \002,2f8.2,3x,3f8.3,11x,3f8.4)";
    static char fmt_751[] = "(\0021\002,10x,18a4)";
    static char fmt_752[] = "(\0020\002,10x,\002SYMBOL   ATOM CODE\002,7x"
        ",\002PLOTTER X,Y(IN.)\002,3x,\002CARTESIAN X,Y,Z (IN.)\002,15x"
        ",\002CRYSTAL SYSTEM X,Y,Z\002/\002 \002,19x,\002(DIRECTION COSIN"
        "ES(I,J),I=1,3),RMSD(J)),J=1,3\002,12x,\002FOR PRINCIPAL AXES BAS"
        "ED ON WORKING SYSTEM\002/\002 \002)";
    static char fmt_801[] = "(\002 \002,13x,3(3x,3f8.4,f8.5)/\002 \002)";

    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    real r__1, r__2;
    doublereal d__1;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);
    double d_int(doublereal *), d_mod(doublereal *, doublereal *);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, l, m, n;
    static real x[3], z__[3];
    static integer l1;
    static real t1, t2, t6, t7, t4, t3, t5, t8, t9, dh;
    static integer ii;
    static doublereal td;
    static real dv;
    extern /* Subroutine */ int vm_(real *, real *, real *), mv_(real *, real 
        *, real *);
    extern doublereal vv_(real *, real *);
    static doublereal d100;
    static integer ii0, ii1, ii2;
    static real t3a, t3b;
    static doublereal td2;
    static real t3c;
    static doublereal d1000;
    static integer icq;
    static real eye[3];
    static integer lns;
    extern doublereal vmv_(real *, real *, real *);
    static doublereal d100k;
    extern /* Subroutine */ int xyz_(doublereal *, real *, integer *), difv_(
        real *, real *, real *);
    static integer nbis;
    extern /* Subroutine */ int draw_(real *, real *, real *, integer *);
    static integer ndot, itom;
    extern /* Subroutine */ int norm_(real *, real *, real *, integer *), 
        proj_(real *, real *, real *, real *, real *, integer *, integer *
        , integer *), lap500_(integer *), lap700_(integer *, integer *);
    static integer nres1, ndash, lines;
    extern /* Subroutine */ int paxes_(doublereal *, integer *);
    static real chsym;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *);
    static real viewv[3];
    extern /* Subroutine */ int unity_(real *, real *, integer *), pltxy_(
        real *, real *), radial_(integer *);
    static integer nplane, ncycle;
    extern /* Subroutine */ int simbol_(real *, real *, real *, char *, real *
        , integer *, ftnlen);
    static integer nsolid, nresol, nfirst;

    /* Fortran I/O blocks */
    static cilist io___334 = { 0, 0, 0, fmt_750, 0 };
    static cilist io___335 = { 0, 0, 0, fmt_751, 0 };
    static cilist io___337 = { 0, 0, 0, fmt_752, 0 };
    static cilist io___338 = { 0, 0, 0, fmt_750, 0 };
    static cilist io___343 = { 0, 0, 0, fmt_801, 0 };


/*     ***** SUBROUTINE TO DRAW ELLIPSOIDS ***** */
/*     ***** SET ELLIPSOID GRAPHIC DETAILS ***** */
    d100 = 100.f;
    d1000 = 1e3f;
    d100k = 1e5f;
    _BLNK__1.itilt = 0;
    _BLNK__1.ng = 0;
    nfirst = 1;
    nplane = (integer) _BLNK__1.ain[0];
    if (nplane - 1 != 0) {
    goto L720;
    } else {
    goto L715;
    }
L715:
    nfirst = 4;
    nplane = 4;
L720:
    nsolid = (integer) _BLNK__1.ain[1];
    i__1 = abs(nsolid);
    ndot = 64 / pow_ii(&c__2, &i__1);
    lines = (integer) _BLNK__1.ain[2];
    ndash = (integer) _BLNK__1.ain[3];
    chsym = _BLNK__1.ain[4];
    t6 = _BLNK__1.ain[5];
    dh = t6 - chsym * 17.f / 7.f;
    t7 = _BLNK__1.ain[6];
    dv = t7 - chsym * .5f;
/*     ***** ESTABLISH REFERENCE POINT OF VIEW ***** */
    t1 = 1e6f;
    if (_BLNK__1.view <= 0.f) {
    goto L740;
    } else {
    goto L735;
    }
L735:
    t1 = _BLNK__1.view / _BLNK__1.scal1;
L740:
    for (j = 1; j <= 3; ++j) {
/* L741: */
    eye[j - 1] = _BLNK__1.refv[j + 5] * t1 + _BLNK__1.orgn[j - 1];
    }
    lns = -1;
/*     ***** LOOP THROUGH ATOM LIST ***** */
    i__1 = _BLNK__1.latm;
    for (itom = 1; itom <= i__1; ++itom) {
    td = _BLNK__1.atomid[itom - 1];
    k = (integer) (td / d100k);
    if (_BLNK__1.ain[9] <= 0.) {
        goto L744;
    } else {
        goto L7412;
    }
L7412:
    if ((d__1 = _BLNK__1.ain[11] - 1.) < 0.) {
        goto L742;
    } else if (d__1 == 0) {
        goto L7414;
    } else {
        goto L7416;
    }
L7414:
    td2 = (doublereal) parms_1.ident[(k << 1) - 2];
    goto L7422;
L7416:
    td2 = (doublereal) parms_1.ident[(k << 1) - 1];
    goto L7422;
L742:
    d__1 = td / d100k;
    td2 = d_int(&d__1);
L7422:
    if (td2 - _BLNK__1.ain[9] >= 0.) {
        goto L743;
    } else {
        goto L1105;
    }
L743:
    if (_BLNK__1.ain[10] - td2 >= 0.) {
        goto L744;
    } else {
        goto L1105;
    }
L744:
    xyz_(&td, x, &c__2);
    if (_BLNK__1.ng != 0) {
        goto L758;
    } else {
        goto L746;
    }
L746:
    pltxy_(x, z__);
    d__1 = td / d100;
    l = (integer) d_mod(&d__1, &d1000);
    l1 = (integer) d_mod(&td, &d100);
    // :- ortep code for screen, currently not used by cairotep.
    /*
    if (ns_1.ndraw == 1) {
        io___334.ciunit = ns_1.npf;
        s_wsfe(&io___334);
        do_fio(&c__1, parms_1.chem + (k - 1 << 3), (ftnlen)8);
        do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&l, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&l1, (ftnlen)sizeof(integer));
        r__1 = z__[0] + trfac_1.xtrans;
        do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
        r__2 = z__[1] + trfac_1.ytrans;
        do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
        e_wsfe();
    }
    */
    // --
    if (_BLNK__1.nj2 - 10 >= 0) {
        goto L754;
    } else {
        goto L747;
    }
L747:
    lns = (lns + 1) % 18;
    if (lns != 0) {
        goto L749;
    } else {
        goto L748;
    }
L748:
    if (_BLNK__1.nout >= 0) {
        io___335.ciunit = _BLNK__1.nout;
        s_wsfe(&io___335);
        for (i__ = 1; i__ <= 18; ++i__) {
        do_fio(&c__1, _BLNK__1.title + (i__ - 1 << 2), (ftnlen)4);
        }
        e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
        io___337.ciunit = _BLNK__1.nout;
        s_wsfe(&io___337);
        e_wsfe();
    }
L749:
    if (_BLNK__1.nout >= 0) {
        io___338.ciunit = _BLNK__1.nout;
        s_wsfe(&io___338);
        do_fio(&c__1, parms_1.chem + (k - 1 << 3), (ftnlen)8);
        do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&l, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&l1, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&z__[0], (ftnlen)sizeof(real));
        do_fio(&c__1, (char *)&z__[1], (ftnlen)sizeof(real));
        for (i__ = 1; i__ <= 3; ++i__) {
        do_fio(&c__1, (char *)&x[i__ - 1], (ftnlen)sizeof(real));
        }
        for (i__ = 1; i__ <= 3; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.xt[i__ - 1], (ftnlen)sizeof(
            real));
        }
        e_wsfe();
    }
L754:
    if (_BLNK__1.edge - _BLNK__1.brdr * .75f >= 0.f) {
        goto L760;
    } else {
        goto L755;
    }
L755:
    _BLNK__1.ng = 10;
L758:
    erpnt_(&td, &c__700);
    goto L1105;
/*     ***** CALL OVERLAP ROUTINE ***** */
L760:
    icq = 0;
    lap700_(&itom, &icq);
    if (icq >= 0) {
        goto L764;
    } else {
        goto L762;
    }
/*     ***** OMIT HIDDEN ATOM ***** */
L762:
    _BLNK__1.ng = 14;
    goto L758;
L764:
    if (chsym <= 0.f) {
        goto L775;
    } else {
        goto L765;
    }
/*     ***** PLOT CHEMICAL SYMBOLS ***** */
L765:
    t4 = 1.f;
    if (_BLNK__1.view <= 0.f) {
        goto L767;
    } else {
        goto L766;
    }
L766:
    t4 = _BLNK__1.view / (_BLNK__1.view - x[2]);
L767:
    t3 = chsym * t4;
    t4 = _BLNK__1.disp * t4 * .5f;
    _BLNK__1.v1[0] = x[0] + dh * _BLNK__1.symb[0] + dv * _BLNK__1.symb[3];
    _BLNK__1.v1[1] = x[1] + dh * _BLNK__1.symb[1] + dv * _BLNK__1.symb[4];
    _BLNK__1.v1[2] = x[2];
    pltxy_(_BLNK__1.v1, _BLNK__1.v3);
    if (_BLNK__1.edge - chsym >= 0.f) {
        goto L768;
    } else {
        goto L775;
    }
L768:
    _BLNK__1.v2[2] = 0.f;
    for (i__ = 1; i__ <= 3; i__ += 2) {
        _BLNK__1.v2[0] = _BLNK__1.v3[0] + (real) (i__ - 2) * t4;
        for (j = 1; j <= 3; j += 2) {
        _BLNK__1.v2[1] = _BLNK__1.v3[1] + (real) (j - 2) * t4;
        simbol_(_BLNK__1.v2, &_BLNK__1.v2[1], &t3, parms_1.chem + (k 
            - 1 << 3), &_BLNK__1.theta, &c__6, (ftnlen)8);
        if (t4 <= 0.f) {
            goto L775;
        } else {
            goto L770;
        }
L770:
        ;
        }
    }
L775:
    if (nplane <= 0) {
        goto L1105;
    } else {
        goto L780;
    }
/*     ***** ELLIPSOID PRINC VECTORS TOWARD VIEWER ***** */
L780:
    paxes_(&td, &c__2);
    if (_BLNK__1.ng != 0) {
        goto L758;
    } else {
        goto L783;
    }
L783:
    difv_(eye, _BLNK__1.xt, viewv);
    unity_(viewv, viewv, &c_n1);
    vm_(viewv, _BLNK__1.aa, _BLNK__1.v2);
    for (i__ = 1; i__ <= 3; ++i__) {
        if (vv_(_BLNK__1.v2, &_BLNK__1.pat[i__ * 3 - 3]) >= 0.f) {
        goto L795;
        } else {
        goto L785;
        }
L785:
        for (j = 1; j <= 3; ++j) {
        _BLNK__1.pac[j + i__ * 3 - 4] = -_BLNK__1.pac[j + i__ * 3 - 4]
            ;
/* L790: */
        _BLNK__1.pat[j + i__ * 3 - 4] = -_BLNK__1.pat[j + i__ * 3 - 4]
            ;
        }
L795:
        ;
    }
    for (j = 1; j <= 3; ++j) {
        _BLNK__1.pac[j + 8] = _BLNK__1.pac[j - 1];
/* L800: */
        _BLNK__1.pac[j + 11] = _BLNK__1.pac[j + 2];
    }
    if (_BLNK__1.nj2 - 10 >= 0) {
        goto L803;
    } else {
        goto L802;
    }
/* L801: */
L802:
    if (_BLNK__1.nout >= 0) {
        io___343.ciunit = _BLNK__1.nout;
        s_wsfe(&io___343);
        for (k = 1; k <= 3; ++k) {
        for (j = 1; j <= 3; ++j) {
            do_fio(&c__1, (char *)&_BLNK__1.pac[j + k * 3 - 4], (
                ftnlen)sizeof(real));
        }
        do_fio(&c__1, (char *)&_BLNK__1.rms[k - 1], (ftnlen)sizeof(
            real));
        }
        e_wsfe();
    }
/*     ***** V4 = VECTOR NORMAL TO POLAR PLANE ***** */
L803:
    vm_(viewv, _BLNK__1.aawrk, _BLNK__1.v6);
    unity_(_BLNK__1.v6, _BLNK__1.v6, &c__1);
    mv_(_BLNK__1.q, _BLNK__1.v6, _BLNK__1.v4);
    unity_(_BLNK__1.v4, _BLNK__1.v4, &c__1);
/*     ***** SET PLOTTING RESOLUTION FOR ELLIPSOID ***** */
    t3a = sqrt(_BLNK__1.rms[2] * _BLNK__1.rms[1]);
    t3b = sqrt(_BLNK__1.rms[1] * _BLNK__1.rms[0]);
    t3c = sqrt(_BLNK__1.rms[2] * _BLNK__1.rms[0]);
    t3 = (t3a + t3b + t3c) / 3.f * _BLNK__1.scl;
    nresol = 1;
    nbis = 5;
    for (j = 1; j <= 3; ++j) {
        if (t3 - _BLNK__1.res[j - 1] >= 0.f) {
        goto L810;
        } else {
        goto L804;
        }
L804:
        --nbis;
/* L805: */
        nresol <<= 1;
    }
L810:
    nres1 = nresol + 1;
/*     ***** LOOP THROUGH PRINC AND POLAR PLANES ***** */
    i__2 = nplane;
    for (ii = nfirst; ii <= i__2; ++ii) {
        ii0 = (ii + 2) % 3 + 1;
        ii1 = ii % 3 + 1;
        ii2 = (ii + 1) % 3 + 1;
/*     ***** GENERATE CONJUGATE DIAMETERS ***** */
        if (.99938f - (r__1 = vv_(_BLNK__1.v4, &_BLNK__1.pac[ii2 * 3 - 3])
            , dabs(r__1)) <= 0.f) {
        goto L820;
        } else {
        goto L830;
        }
L820:
        t1 = _BLNK__1.rms[ii0 - 1] * _BLNK__1.scl;
        t2 = _BLNK__1.rms[ii1 - 1] * _BLNK__1.scl;
        for (j = 1; j <= 3; ++j) {
        _BLNK__1.da[j - 1] = _BLNK__1.pac[j + ii0 * 3 - 4] * t1;
/* L825: */
        _BLNK__1.da[j + 2] = _BLNK__1.pac[j + ii1 * 3 - 4] * t2;
        }
        goto L850;
L830:
        norm_(&_BLNK__1.pac[ii0 * 3 - 3], &_BLNK__1.pac[ii1 * 3 - 3], 
            _BLNK__1.v1, &c__1);
        norm_(_BLNK__1.v1, _BLNK__1.v4, _BLNK__1.v2, &c__1);
        unity_(_BLNK__1.v2, _BLNK__1.v2, &c__1);
        mv_(_BLNK__1.q, _BLNK__1.v2, _BLNK__1.v3);
        if (ii - 4 >= 0) {
        goto L840;
        } else {
        goto L835;
        }
L835:
        norm_(_BLNK__1.v3, _BLNK__1.v1, _BLNK__1.v5, &c__1);
        goto L843;
L840:
        norm_(_BLNK__1.v3, _BLNK__1.v4, _BLNK__1.v5, &c__1);
L843:
        unity_(_BLNK__1.v5, _BLNK__1.v5, &c__1);
        t1 = _BLNK__1.scl / sqrt(vmv_(_BLNK__1.v2, _BLNK__1.q, 
            _BLNK__1.v2));
        t2 = _BLNK__1.scl / sqrt(vmv_(_BLNK__1.v5, _BLNK__1.q, 
            _BLNK__1.v5));
        for (j = 1; j <= 3; ++j) {
        _BLNK__1.da[j - 1] = _BLNK__1.v2[j - 1] * t1;
/* L845: */
        _BLNK__1.da[j + 2] = _BLNK__1.v5[j - 1] * t2;
        }
/*     ***** GENERATE ELLIPSE ***** */
L850:
        radial_(&nbis);
        if (ii - 4 >= 0) {
        goto L851;
        } else {
        goto L900;
        }
L851:
        if (nsolid <= 0) {
        goto L859;
        } else {
        goto L852;
        }
/*     ***** PLOT DOTTED BOUNDARY ELLIPSE ***** */
L852:
        if (ndot - nresol >= 0) {
        goto L855;
        } else {
        goto L853;
        }
L853:
        i__3 = nsolid - 1;
        radial_(&i__3);
L855:
        proj_(_BLNK__1.d__, _BLNK__1.dp, x, _BLNK__1.xo, &_BLNK__1.view, &
            c__1, &c__129, &ndot);
        i__3 = ndot;
        for (j = 1; i__3 < 0 ? j >= 129 : j <= 129; j += i__3) {
        draw_(&_BLNK__1.dp[(j << 1) - 2], &_BLNK__1.disp, &
            _BLNK__1.disp, &c__3);
        for (i__ = 1; i__ <= 3; i__ += 2) {
            t1 = (real) (i__ - 2) * _BLNK__1.disp;
            for (k = 1; k <= 3; k += 2) {
            t2 = (real) (k - 2) * _BLNK__1.disp;
            draw_(&_BLNK__1.dp[(j << 1) - 2], &t1, &t2, &c__2);
            if (_BLNK__1.disp <= 0.f) {
                goto L857;
            } else {
                goto L856;
            }
L856:
            ;
            }
        }
L857:
        ;
        }
        goto L1100;
/*     ***** PLOT SOLID BOUNDARY ELLIPSE ***** */
L859:
        proj_(_BLNK__1.d__, _BLNK__1.dp, x, _BLNK__1.xo, &_BLNK__1.view, &
            c__1, &c__129, &nresol);
        draw_(_BLNK__1.dp, &c_b317, &c_b317, &c__3);
        i__3 = nresol;
        for (j = nres1; i__3 < 0 ? j >= 129 : j <= 129; j += i__3) {
/* L860: */
        draw_(&_BLNK__1.dp[(j << 1) - 2], &c_b317, &c_b317, &c__2);
        }
        if (_BLNK__1.disp <= 0.f) {
        goto L1100;
        } else {
        goto L865;
        }
/*     ***** BOUNDARY ANNULUS AS A LINEAR FUNCTION OF HEIGHT ***** */
L865:
        difv_(_BLNK__1.xt, _BLNK__1.orgn, _BLNK__1.v1);
        t5 = vv_(_BLNK__1.v1, &_BLNK__1.aarev[6]) * _BLNK__1.scal1;
        t8 = _BLNK__1.ain[7];
        t9 = _BLNK__1.ain[8];
        ncycle = (t8 + t5 * t9) / _BLNK__1.disp + .5f;
        if (ncycle <= 0) {
        goto L1100;
        } else {
        goto L870;
        }
L870:
        t3 = _BLNK__1.disp * 2.f / (t1 + t2);
/*     ***** INCREASE ANNULAR THICKNESS ***** */
        i__3 = ncycle;
        for (i__ = 1; i__ <= i__3; ++i__) {
        t4 = t3 * (real) i__;
        i__4 = nresol;
        for (j = 1; i__4 < 0 ? j >= 129 : j <= 129; j += i__4) {
/* L875: */
            r__1 = _BLNK__1.d__[j * 3 - 3] * t4;
            r__2 = _BLNK__1.d__[j * 3 - 2] * t4;
            draw_(&_BLNK__1.dp[(j << 1) - 2], &r__1, &r__2, &c__2);
        }
        }
        goto L1100;
L900:
        proj_(_BLNK__1.d__, _BLNK__1.dp, x, _BLNK__1.xo, &_BLNK__1.view, &
            c__1, &c__65, &nresol);
/*     ***** PLOT HALF AN ELLIPSE ***** */
        draw_(_BLNK__1.dp, &c_b317, &c_b317, &c__3);
        i__4 = nresol;
        for (j = nres1; i__4 < 0 ? j >= 65 : j <= 65; j += i__4) {
/* L905: */
        draw_(&_BLNK__1.dp[(j << 1) - 2], &c_b317, &c_b317, &c__2);
        }
        if (_BLNK__1.disp <= 0.f) {
        goto L930;
        } else {
        goto L910;
        }
/*     ***** ACCENTUATE FRONT HALF ***** */
L910:
        for (i__ = 1; i__ <= 3; i__ += 2) {
        t2 = (real) (i__ - 2) * _BLNK__1.disp;
        i__4 = nresol;
        for (j = 1; i__4 < 0 ? j >= 65 : j <= 65; j += i__4) {
            k = 66 - j;
/* L915: */
            draw_(&_BLNK__1.dp[(k << 1) - 2], &_BLNK__1.disp, &t2, &
                c__2);
        }
        i__4 = nresol;
        for (k = 1; i__4 < 0 ? k >= 65 : k <= 65; k += i__4) {
/* L925: */
            r__1 = -_BLNK__1.disp;
            r__2 = -t2;
            draw_(&_BLNK__1.dp[(k << 1) - 2], &r__1, &r__2, &c__2);
        }
        }
L930:
        if (nsolid < 0) {
        goto L940;
        } else if (nsolid == 0) {
        goto L967;
        } else {
        goto L935;
        }
L935:
        l = ndot;
        if ((i__4 = ndot - nresol) < 0) {
        goto L938;
        } else if (i__4 == 0) {
        goto L945;
        } else {
        goto L940;
        }
L938:
        i__4 = nsolid - 1;
        radial_(&i__4);
        goto L945;
L940:
        l = nresol;
L945:
        proj_(&_BLNK__1.d__[192], &_BLNK__1.dp[128], x, _BLNK__1.xo, &
            _BLNK__1.view, &c__1, &c__65, &l);
        if (nsolid < 0) {
        goto L960;
        } else if (nsolid == 0) {
        goto L967;
        } else {
        goto L950;
        }
/*     ***** DOTTED LINE ON REVERSE SIDE ***** */
L950:
        i__4 = ndot;
        for (j = 65; i__4 < 0 ? j >= 129 : j <= 129; j += i__4) {
        draw_(&_BLNK__1.dp[(j << 1) - 2], &_BLNK__1.disp, &
            _BLNK__1.disp, &c__3);
        for (i__ = 1; i__ <= 3; i__ += 2) {
            t1 = (real) (i__ - 2) * _BLNK__1.disp;
            for (k = 1; k <= 3; k += 2) {
            t2 = (real) (k - 2) * _BLNK__1.disp;
            draw_(&_BLNK__1.dp[(j << 1) - 2], &t1, &t2, &c__2);
            if (_BLNK__1.disp <= 0.f) {
                goto L958;
            } else {
                goto L955;
            }
L955:
            ;
            }
        }
L958:
        ;
        }
        goto L967;
/*     ***** SINGLE LINE ON REVERSE SIDE ***** */
L960:
        i__4 = nresol;
        for (j = 65; i__4 < 0 ? j >= 129 : j <= 129; j += i__4) {
/* L965: */
        draw_(&_BLNK__1.dp[(j << 1) - 2], &c_b317, &c_b317, &c__2);
        }
/*     ***** DETAIL INTERIOR FEATURES ***** */
L967:
        t2 = (real) (ndash << 1);
        for (j = 1; j <= 3; ++j) {
        t1 = _BLNK__1.pac[j + ii0 * 3 - 4] * _BLNK__1.rms[ii0 - 1] * 
            _BLNK__1.scl;
        _BLNK__1.da[j - 1] = t1;
        _BLNK__1.da[j + 2] = _BLNK__1.pac[j + ii1 * 3 - 4] * 
            _BLNK__1.rms[ii1 - 1] * _BLNK__1.scl;
        _BLNK__1.da[j + 5] = 0.f;
        if (ndash <= 0) {
            goto L975;
        } else {
            goto L970;
        }
L970:
        _BLNK__1.v1[j - 1] = -t1;
        _BLNK__1.v2[j - 1] = t1 / t2;
L975:
        ;
        }
        if (ndash <= 0) {
        goto L987;
        } else {
        goto L980;
        }
/*     ***** DASHED LINE FOR REVERSE AXIS ***** */
L980:
        i__4 = ndash;
        for (j = 1; j <= i__4; ++j) {
        for (k = 1; k <= 2; ++k) {
            l = 4 - k;
            proj_(_BLNK__1.v1, _BLNK__1.dp, x, _BLNK__1.xo, &
                _BLNK__1.view, &c__1, &c__1, &c__1);
            draw_(_BLNK__1.dp, &c_b317, &c_b317, &l);
            for (i__ = 1; i__ <= 3; ++i__) {
/* L985: */
            _BLNK__1.v1[i__ - 1] += _BLNK__1.v2[i__ - 1];
            }
        }
        }
/*     ***** SOLID LINE FOR FORWARD AXIS ***** */
L987:
        if (lines <= 0) {
        goto L1100;
        } else {
        goto L988;
        }
L988:
        proj_(_BLNK__1.da, _BLNK__1.dp, x, _BLNK__1.xo, &_BLNK__1.view, &
            c__1, &c__3, &c__1);
        t1 = _BLNK__1.disp * .5f;
        for (i__ = 1; i__ <= 3; i__ += 2) {
        t2 = (real) (2 - i__) * t1;
        draw_(_BLNK__1.dp, &t1, &t2, &c__3);
        draw_(&_BLNK__1.dp[4], &t1, &t2, &c__2);
        if (_BLNK__1.disp <= 0.f) {
            goto L1000;
        } else {
            goto L989;
        }
L989:
        r__1 = -t1;
        draw_(&_BLNK__1.dp[4], &r__1, &t2, &c__2);
/* L990: */
        r__1 = -t1;
        draw_(_BLNK__1.dp, &r__1, &t2, &c__2);
        }
/*     ***** SHADE QUADRANT BETWEEN TWO PRINCIPAL AXES ***** */
L1000:
        l = lines - 1;
        if (l <= 0) {
        goto L1100;
        } else {
        goto L1005;
        }
L1005:
        t2 = (real) lines;
        i__4 = l;
        for (i__ = 1; i__ <= i__4; ++i__) {
        t1 = (real) i__ / t2;
        t3 = sqrt(1.f - t1 * t1);
        if (i__ % 2 != 0) {
            goto L1010;
        } else {
            goto L1015;
        }
L1010:
        m = i__ << 1;
        n = m - 1;
        goto L1020;
L1015:
        n = i__ << 1;
        m = n - 1;
L1020:
        for (j = 1; j <= 3; ++j) {
            t4 = _BLNK__1.da[j - 1] * t1;
            _BLNK__1.d__[j + m * 3 - 4] = t4;
/* L1025: */
            _BLNK__1.d__[j + n * 3 - 4] = _BLNK__1.da[j + 2] * t3 + 
                t4;
        }
        }
        l <<= 1;
        proj_(_BLNK__1.d__, _BLNK__1.dp, x, _BLNK__1.xo, &_BLNK__1.view, &
            c__1, &l, &c__1);
        i__4 = l;
        for (i__ = 2; i__ <= i__4; i__ += 2) {
        draw_(&_BLNK__1.dp[(i__ - 1 << 1) - 2], &c_b317, &c_b317, &
            c__3);
/* L1030: */
        draw_(&_BLNK__1.dp[(i__ << 1) - 2], &c_b317, &c_b317, &c__2);
        }
L1100:
        ;
    }
L1105:
    ;
    }
/*     ***** ELIMINATE LOCAL OVERLAP INFORMATION BEFORE RETURNING ***** */
    lap500_(&c_n1);
    return 0;
} /* f700_ */

/* Subroutine */ int f800_(void)
{
    /* Format strings */
    static char fmt_835[] = "(\0021\002,10x,18a4)";
    static char fmt_837[] = "(\0020\002,10x,\002SYMBOL   ATOM CODE\002,6x"
        ",\002PLOTTER X,Y(IN.)\002,6x,\002CARTESIAN X,Y,Z (IN.)\002,17x"
        ",\002CRYSTAL SYSTEM X,Y,Z\002/\002 \002)";
    static char fmt_861[] = "(\0020\002,10x,\002BOND SELECTION CODES\002//11"
        "x,\002(SEQUENCE(A))(SEQUENCE(B)) (BOND) (DISTANCES)( BOND )(PERS"
        "P.--LABELS)(NORMAL--LABELS)(DIGITS) \002/11x,\002(  MIN  MAX )( "
        " MIN  MAX ) (TYPE) (MIN   MAX)(RADIUS)(HEIGHT  OFFSET)(HEIGHT  O"
        "FFSET)(NUMBER)\002)";
    static char fmt_871[] = "(\002 \002,10x,i6,i5,i8,i5,i8,2f6.2,5f8.3,f7.0)";
    static char fmt_992[] = "(\0020\002,10x,\002BOND OVERLAP ARRAY CONTAIN"
        "S\002,i9,\002 BONDS (MAXIMUM IS\002,i9,\002)\002/11x,\002ATOM-PA"
        "IR NUMBERS IN ARRAY REFER TO SEQUENCE IN SORTED ATOMS ARRAY\002/"
        "(15x,10f10.0))";

    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    real r__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, l, m, n, j1, k3, n2;
    static real w1[6];
    static integer ia[3], jb, ii, mi, ni, im, na1, na2, na3;
    static doublereal td1, td2, td3;
    static integer nj4, km1, km2, ian;
    static real dsq;
    static integer lns;
    static doublereal d100k;
    extern /* Subroutine */ int xyz_(doublereal *, real *, integer *);
    static real dsq1, dsq2;
    extern /* Subroutine */ int bond_(doublereal *, doublereal *, integer *, 
        integer *, integer *);
    static real dmax__, dist;
    extern /* Subroutine */ int lap500_(integer *);
    static real scal3;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *);

    /* Fortran I/O blocks */
    static cilist io___368 = { 0, 0, 0, fmt_835, 0 };
    static cilist io___370 = { 0, 0, 0, fmt_837, 0 };
    static cilist io___378 = { 0, 0, 0, fmt_861, 0 };
    static cilist io___380 = { 0, 0, 0, fmt_871, 0 };
    static cilist io___399 = { 0, 0, 0, fmt_835, 0 };
    static cilist io___400 = { 0, 0, 0, fmt_837, 0 };
    static cilist io___401 = { 0, 0, 0, fmt_992, 0 };


/*     ***** SUBROUTINE FINDS ATOM PAIRS FOR BONDS ***** */
    d100k = 1e5f;
/* *** old */
/*     NJ4=MOD(NJ2,10)-4 */
/* *** new */
    ian = (integer) _BLNK__1.ain[1];
    nj4 = _BLNK__1.nj2 % 10 - 4 + (ian << 1);
    if (_BLNK__1.nj == 10) {
    nj4 = (ian << 1) - 2;
    }
    lns = -4;
    if (_BLNK__1.nj2 % 10 - 2 >= 0) {
    goto L848;
    } else {
    goto L805;
    }
/*     ***** EXPLICIT DESCRIPTION ***** */
L805:
    ii = 0;
    if (_BLNK__1.ncd <= 0) {
    goto L810;
    } else {
    goto L815;
    }
L810:
    _BLNK__1.ng = 11;
    i__1 = _BLNK__1.nj * 100 + _BLNK__1.nj2;
    erpnt_(&c_b113, &i__1);
    goto L980;
L815:
    ++ii;
    if (140 - ii <= 0) {
    goto L980;
    } else {
    goto L820;
    }
L820:
    td1 = _BLNK__1.ain[ii - 1];
    if (td1 <= 0.) {
    goto L815;
    } else {
    goto L825;
    }
L825:
    ++ii;
    td2 = _BLNK__1.ain[ii - 1];
    if (td2 <= 0.) {
    goto L815;
    } else {
    goto L830;
    }
L830:
    if (_BLNK__1.nj2 - 10 >= 0) {
    goto L838;
    } else {
    goto L832;
    }
L832:
    lns = (lns + 4) % 56;
    if (lns != 0) {
    goto L838;
    } else {
    goto L834;
    }
L834:
    if (_BLNK__1.nout >= 0) {
    io___368.ciunit = _BLNK__1.nout;
    s_wsfe(&io___368);
    for (i__ = 1; i__ <= 18; ++i__) {
        do_fio(&c__1, _BLNK__1.title + (i__ - 1 << 2), (ftnlen)4);
    }
    e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
    io___370.ciunit = _BLNK__1.nout;
    s_wsfe(&io___370);
    e_wsfe();
    }
/*     ***** CHECK IF BOND ATOMS ARE IN ATOMS LIST (FOR OVERLAP CALC) *** */
L838:
    na1 = 0;
    na2 = 0;
    if (_BLNK__1.latm - 2 >= 0) {
    goto L839;
    } else {
    goto L845;
    }
L839:
    n2 = 2;
    i__1 = _BLNK__1.latm;
    for (k = 1; k <= i__1; ++k) {
    td3 = _BLNK__1.atomid[k - 1];
    if (td3 - td1 != 0.) {
        goto L841;
    } else {
        goto L840;
    }
L840:
    na1 = k;
    goto L843;
L841:
    if (td3 - td2 != 0.) {
        goto L844;
    } else {
        goto L842;
    }
L842:
    na2 = k;
L843:
    --n2;
    if (n2 <= 0) {
        goto L845;
    } else {
        goto L844;
    }
L844:
    ;
    }
L845:
    if (na2 - na1 >= 0) {
    goto L847;
    } else {
    goto L846;
    }
L846:
    na3 = na1;
    na1 = na2;
    na2 = na3;
    td3 = td1;
    td1 = td2;
    td2 = td3;
L847:
    bond_(&td1, &td2, &c__1, &na1, &na2);
    goto L815;
/*     ***** IMPLICIT DESCRIPTION ***** */
L848:
    if (_BLNK__1.latm - 2 >= 0) {
    goto L850;
    } else {
    goto L810;
    }
L850:
    scal3 = _BLNK__1.scal1;
    _BLNK__1.scal1 = 1.f;
    i__1 = _BLNK__1.latm;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L855: */
    xyz_(&_BLNK__1.atomid[i__ - 1], &_BLNK__1.atoms[i__ * 3 - 3], &c__2);
    }
    _BLNK__1.scal1 = scal3;
    if (_BLNK__1.ncd <= 0) {
    goto L810;
    } else {
    goto L860;
    }
L860:
    if (_BLNK__1.nout >= 0) {
    io___378.ciunit = _BLNK__1.nout;
    s_wsfe(&io___378);
    e_wsfe();
    }
    dmax__ = 0.f;
    i__1 = _BLNK__1.ncd;
    for (i__ = 1; i__ <= i__1; ++i__) {
    if (dmax__ - _BLNK__1.cd[(i__ << 3) - 7] >= 0.f) {
        goto L866;
    } else {
        goto L865;
    }
L865:
    dmax__ = _BLNK__1.cd[(i__ << 3) - 7];
L866:
    if (_BLNK__1.nout >= 0) {
        io___380.ciunit = _BLNK__1.nout;
        s_wsfe(&io___380);
        for (j = 1; j <= 5; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.kd[j + i__ * 5 - 6], (ftnlen)
            sizeof(integer));
        }
        for (j = 1; j <= 8; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.cd[j + (i__ << 3) - 9], (
            ftnlen)sizeof(real));
        }
        e_wsfe();
    }
/* L870: */
    }
    dmax__ *= dmax__;
/*     ***** LOOP THROUGH ATOMS ARRAY ***** */
    i__1 = _BLNK__1.latm;
    for (m = 1; m <= i__1; ++m) {
    na1 = m;
    td1 = _BLNK__1.atomid[m - 1];
    mi = (integer) (td1 / d100k);
    if (nj4 < 0) {
        goto L8722;
    } else if (nj4 == 0) {
        goto L8724;
    } else {
        goto L8718;
    }
L8718:
    if ((i__2 = nj4 - 2) < 0) {
        goto L8724;
    } else if (i__2 == 0) {
        goto L8726;
    } else {
        goto L8720;
    }
L8720:
    if (nj4 - 4 >= 0) {
        goto L8722;
    } else {
        goto L8726;
    }
L8722:
    ia[0] = (integer) (td1 / d100k);
    goto L8728;
L8724:
    ia[0] = parms_1.ident[(mi << 1) - 2];
    goto L8728;
L8726:
    ia[0] = parms_1.ident[(mi << 1) - 1];
L8728:
    ia[2] = ia[0];
    w1[0] = _BLNK__1.atoms[m * 3 - 3];
    w1[1] = _BLNK__1.atoms[m * 3 - 2];
    w1[2] = _BLNK__1.atoms[m * 3 - 1];
    l = m + 1;
    if (_BLNK__1.latm - l >= 0) {
        goto L872;
    } else {
        goto L977;
    }
L872:
    i__2 = _BLNK__1.latm;
    for (n = l; n <= i__2; ++n) {
        na2 = n;
/* Computing 2nd power */
        r__1 = _BLNK__1.atoms[n * 3 - 3] - w1[0];
        dist = r__1 * r__1;
        if (dmax__ - dist >= 0.f) {
        goto L873;
        } else {
        goto L975;
        }
L873:
/* Computing 2nd power */
        r__1 = _BLNK__1.atoms[n * 3 - 2] - w1[1];
        dist += r__1 * r__1;
        if (dmax__ - dist >= 0.f) {
        goto L874;
        } else {
        goto L975;
        }
L874:
/* Computing 2nd power */
        r__1 = _BLNK__1.atoms[n * 3 - 1] - w1[2];
        dist += r__1 * r__1;
        if (dmax__ - dist >= 0.f) {
        goto L875;
        } else {
        goto L975;
        }
L875:
        dist = sqrt(dist);
        td2 = _BLNK__1.atomid[n - 1];
        ni = (integer) (td2 / d100k);
/*     IF(NJ4)876,877,878 */
/* 876 IA(2)=TD2/D100K */
/*     GO TO 879 */
/* 877 IA(2)=IDENT(1,NI) */
/*     GO TO 879 */
/* 878 IA(2)=IDENT(2,NI) */
        if (nj4 < 0) {
        ia[1] = (integer) (td2 / d100k);
        }
        if (nj4 == 0 || nj4 == 1) {
        ia[1] = parms_1.ident[(ni << 1) - 2];
        }
        if (nj4 > 1) {
        ia[1] = parms_1.ident[(ni << 1) - 1];
        }
/*     ***** SELECT BONDS ACCORDING TO CODES ***** */
/* L879: */
        i__3 = _BLNK__1.ncd;
        for (j = 1; j <= i__3; ++j) {
        jb = j;
        if (dist - _BLNK__1.cd[(j << 3) - 8] >= 0.f) {
            goto L880;
        } else {
            goto L950;
        }
L880:
        if (_BLNK__1.cd[(j << 3) - 7] - dist >= 0.f) {
            goto L881;
        } else {
            goto L950;
        }
L881:
        for (k = 1; k <= 2; ++k) {
            if (ia[k - 1] - _BLNK__1.kd[j * 5 - 5] >= 0) {
            goto L882;
            } else {
            goto L885;
            }
L882:
            if (_BLNK__1.kd[j * 5 - 4] - ia[k - 1] >= 0) {
            goto L883;
            } else {
            goto L885;
            }
L883:
            if (ia[k] - _BLNK__1.kd[j * 5 - 3] >= 0) {
            goto L884;
            } else {
            goto L885;
            }
L884:
            if (_BLNK__1.kd[j * 5 - 2] - ia[k] >= 0) {
            goto L890;
            } else {
            goto L885;
            }
L885:
            ;
        }
        goto L950;
/*     ***** CHECK FOR POLYHEDRA CODE ***** */
L890:
        if (_BLNK__1.cd[(j << 3) - 5] >= 0.f) {
            goto L955;
        } else {
            goto L900;
        }
L900:
        w1[3] = _BLNK__1.atoms[n * 3 - 3];
        w1[4] = _BLNK__1.atoms[n * 3 - 2];
        w1[5] = _BLNK__1.atoms[n * 3 - 1];
        km1 = (r__1 = _BLNK__1.cd[(j << 3) - 5], dabs(r__1));
        km2 = (r__1 = _BLNK__1.cd[(j << 3) - 4], dabs(r__1));
/* Computing 2nd power */
        r__1 = _BLNK__1.cd[(j << 3) - 3];
        dsq1 = r__1 * r__1;
/* Computing 2nd power */
        r__1 = _BLNK__1.cd[(j << 3) - 2];
        dsq2 = r__1 * r__1;
/*     ***** SEARCH FOR POLYHEDRA CENTER ***** */
        i__4 = _BLNK__1.latm;
        for (im = 1; im <= i__4; ++im) {
            k3 = (integer) (_BLNK__1.atomid[im - 1] / d100k);
            if (ian == 1) {
            k3 = parms_1.ident[(im << 1) - 2];
            }
            if (ian == 2) {
            k3 = parms_1.ident[(im << 1) - 1];
            }
            if (k3 - km1 >= 0) {
            goto L905;
            } else {
            goto L935;
            }
L905:
            if (km2 - k3 >= 0) {
            goto L910;
            } else {
            goto L935;
            }
/*     ***** CHECK POLYHEDRA DISTANCE RANGE ***** */
L910:
            for (j1 = 1; j1 <= 4; j1 += 3) {
/* Computing 2nd power */
            r__1 = _BLNK__1.atoms[im * 3 - 3] - w1[j1 - 1];
            dsq = r__1 * r__1;
            if (dsq2 - dsq >= 0.f) {
                goto L915;
            } else {
                goto L935;
            }
L915:
/* Computing 2nd power */
            r__1 = _BLNK__1.atoms[im * 3 - 2] - w1[j1];
            dsq += r__1 * r__1;
            if (dsq2 - dsq >= 0.f) {
                goto L920;
            } else {
                goto L935;
            }
L920:
/* Computing 2nd power */
            r__1 = _BLNK__1.atoms[im * 3 - 1] - w1[j1 + 1];
            dsq += r__1 * r__1;
            if (dsq2 - dsq >= 0.f) {
                goto L925;
            } else {
                goto L935;
            }
L925:
            if (dsq - dsq1 >= 0.f) {
                goto L930;
            } else {
                goto L935;
            }
L930:
            ;
            }
            goto L955;
L935:
            ;
        }
/*     ***** END OF POLYHEDRA CHECK ***** */
L950:
        ;
        }
        goto L975;
/*     ***** PREPARE TO DRAW BOND ***** */
L955:
        if (_BLNK__1.nj2 - 10 >= 0) {
        goto L970;
        } else {
        goto L960;
        }
L960:
        lns = (lns + 4) % 56;
        if (lns <= 0) {
        goto L965;
        } else {
        goto L970;
        }
L965:
        if (_BLNK__1.nout >= 0) {
        io___399.ciunit = _BLNK__1.nout;
        s_wsfe(&io___399);
        for (i__ = 1; i__ <= 18; ++i__) {
            do_fio(&c__1, _BLNK__1.title + (i__ - 1 << 2), (ftnlen)4);
        }
        e_wsfe();
        }
        if (_BLNK__1.nout >= 0) {
        io___400.ciunit = _BLNK__1.nout;
        s_wsfe(&io___400);
        e_wsfe();
        }
L970:
        bond_(&td1, &td2, &jb, &na1, &na2);
L975:
        ;
    }
L977:
    ;
    }
/*     ***** ELIMINATE LOCAL OVERLAP INFORMATION BEFORE RETURNING ***** */
L980:
    if (_BLNK__1.nj2 - 21 >= 0) {
    goto L990;
    } else {
    goto L985;
    }
L985:
    lap500_(&c_n1);
L990:
    if (_BLNK__1.nj2 == 22) {
    if (olap_1.nquad <= 0) {
        goto L993;
    } else {
        goto L991;
    }
/*     ***** PRINT OUT NUMBER OF BOND QUADRANGLES STORED ***** */
/*     ***** PRINT OUT QUADRANGLE IDENTIFICATION ARRAY ***** */
L991:
    if (_BLNK__1.nout >= 0) {
        io___401.ciunit = _BLNK__1.nout;
        s_wsfe(&io___401);
        do_fio(&c__1, (char *)&olap_1.nquad, (ftnlen)sizeof(integer));
        do_fio(&c__1, (char *)&c__2999, (ftnlen)sizeof(integer));
        i__1 = olap_1.nquad;
        for (j = 1; j <= i__1; ++j) {
        do_fio(&c__1, (char *)&olap_1.quad[j * 9 - 1], (ftnlen)sizeof(
            real));
        }
        e_wsfe();
    }
    }
L993:
    return 0;
} /* f800_ */

/* Subroutine */ int f900_(void)
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Builtin functions */
    double sqrt(doublereal);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    double pow_ri(real *, integer *), r_int(real *);

    /* Local variables */
    static integer i__, j, k, l;
    static real x[3], y[3], z__[3];
    static integer i9;
    static real t1, t2, t3, t6, t7, t9, dh;
    static integer ii;
    static real dv, th;
    extern doublereal vv_(real *, real *);
    static real xw[15]  /* was [3][5] */;
    static integer nj3;
    static real tt6, tt7, tt8, hgt, dxw, dyw;
    static doublereal d100k;
    extern /* Subroutine */ int xyz_(doublereal *, real *, integer *);
    static real scal, dist;
    extern /* Subroutine */ int norm_(real *, real *, real *, integer *);
    static integer ilast, klast;
    static real costh, sinth, distr;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *);
    static char tmpti[72];
    extern /* Subroutine */ int unity_(real *, real *, integer *), pltxy_(
        real *, real *);
    static char tmpti2[72];
    extern doublereal arccos_(real *);
    extern /* Subroutine */ int simbol_(real *, real *, real *, char *, real *
        , integer *, ftnlen), numbur_(real *, real *, real *, real *, 
        real *, integer *);
    static integer ioffset;

/*     ***** LABELING FUNCTION SUBROUTINE ***** */
    d100k = 1e5f;
    _BLNK__1.itilt = 0;
    nj3 = _BLNK__1.nj2 % 10;
    th = _BLNK__1.theta;
    sinth = _BLNK__1.symb[1];
    costh = _BLNK__1.symb[0];
    ilast = 1;
    t2 = _BLNK__1.ain[1];
    if (t2 - 11100.f <= 0.f) {
    goto L910;
    } else {
    goto L905;
    }
L905:
    ilast = 2;
L910:
    i__1 = ilast;
    for (ii = 1; ii <= i__1; ++ii) {
/*     ***** OBTAIN WORKING CARTESIAN COORDINATES ***** */
    xyz_(&_BLNK__1.ain[ii - 1], &xw[ii * 3 - 3], &c__2);
    if (_BLNK__1.ng != 0) {
        goto L915;
    } else {
        goto L925;
    }
L925:
    xyz_(&_BLNK__1.ain[ii - 1], &xw[(ii + 3) * 3 - 3], &c__3);
    }
    ii = 1;
/*     ***** FIND MEAN REFERENCE POINT ***** */
    for (j = 1; j <= 3; ++j) {
    t2 = xw[j + ilast * 3 - 4];
    t1 = xw[j - 1];
    xw[j + 5] = t2 - t1;
/* L930: */
    x[j - 1] = (t2 + t1) * .5f;
    }
/*     ***** PERSPECTIVE SCALING FACTOR ***** */
    scal = 1.f;
    if (_BLNK__1.view <= 0.f) {
    goto L940;
    } else {
    goto L935;
    }
L935:
    scal = _BLNK__1.view / (_BLNK__1.view - x[2]);
L940:
    t1 = _BLNK__1.ain[4];
    hgt = scal * t1;
    if ((i__1 = _BLNK__1.nj2 - 3) < 0) {
    goto L960;
    } else if (i__1 == 0) {
    goto L950;
    } else {
    goto L945;
    }
L945:
    if (_BLNK__1.nj2 - 6 <= 0) {
    goto L950;
    } else {
    goto L960;
    }
/*     ***** PROJECTED VECTOR BASELINE ***** */
L950:
    pltxy_(&xw[9], _BLNK__1.v1);
    pltxy_(&xw[12], _BLNK__1.v2);
    t1 = _BLNK__1.v2[0] - _BLNK__1.v1[0];
    t2 = _BLNK__1.v2[1] - _BLNK__1.v1[1];
    t3 = sqrt(t1 * t1 + t2 * t2);
    if (t3 <= 0.f) {
    goto L912;
    } else {
    goto L955;
    }
L955:
    costh = t1 / t3;
    sinth = t2 / t3;
    th = arccos_(&costh);
    if (sinth >= 0.f) {
    goto L960;
    } else {
    goto L958;
    }
L958:
    th = -th;
L960:
    if (_BLNK__1.nj2 - 13 >= 0) {
    goto L985;
    } else {
    goto L965;
    }
/*     ***** FIND CENTER OF PROJECTED LABEL ***** */
L965:
    t6 = _BLNK__1.ain[5];
    t7 = _BLNK__1.ain[6];
    y[0] = scal * (x[0] + t6 * costh - t7 * sinth) + _BLNK__1.xo[0];
    y[1] = scal * (x[1] + t6 * sinth + t7 * costh) + _BLNK__1.xo[1];
    y[2] = 0.f;
/*     ***** CHECK FOR LEGEND RESET ***** */
    for (j = 1; j <= 2; ++j) {
    t1 = _BLNK__1.ain[j + 1];
    if (t1 < 0.f) {
        goto L975;
    } else if (t1 == 0) {
        goto L980;
    } else {
        goto L970;
    }
L970:
    y[j - 1] = t1;
    goto L980;
L975:
    y[j - 1] = _BLNK__1.xlng[j - 1] + t1;
L980:
    ;
    }
/*     ***** SET PARAMETERS FOR INDIVIDUAL FUNCTIONS ***** */
L985:
    switch (_BLNK__1.nj2) {
    case 1:  goto L990;
    case 2:  goto L995;
    case 3:  goto L995;
    case 4:  goto L1000;
    case 5:  goto L1000;
    case 6:  goto L1000;
    case 7:  goto L915;
    case 8:  goto L1105;
    case 9:  goto L1105;
    case 10:  goto L915;
    case 11:  goto L915;
    case 12:  goto L915;
    case 13:  goto L1005;
    case 14:  goto L1005;
    case 15:  goto L1005;
    case 16:  goto L1005;
    case 17:  goto L915;
    }
L990:
    t6 = 17.f;
    l = (integer) (_BLNK__1.ain[0] / d100k);
    ilast = 1;
    dxw = 0.f;
    dyw = 0.f;
    goto L1030;
L995:
    t6 = 215.f;
    ilast = 18;
    t1 = hgt * 24.f / 7.f;
    dxw = costh * t1;
    dyw = sinth * t1;
    goto L1030;
L1000:
    t6 = (real) ((nj3 - 4) * 3 + 10);
    dist = sqrt(vv_(&xw[6], &xw[6])) / _BLNK__1.scal1;
    goto L1030;
/*     ***** TRUE PERSPECTIVE LABELS ***** */
L1005:
    unity_(&xw[6], _BLNK__1.vt, &c__1);
    if (dabs(_BLNK__1.vt[2]) - .9994f >= 0.f) {
    goto L912;
    } else {
    goto L1010;
    }
/*     ***** FORM PERSPECTIVE ROTATION MATRIX ***** */
L1010:
    norm_(&_BLNK__1.aid[6], _BLNK__1.vt, &_BLNK__1.vt[3], &c__1);
    unity_(&_BLNK__1.vt[3], &_BLNK__1.vt[3], &c__1);
    norm_(_BLNK__1.vt, &_BLNK__1.vt[3], &_BLNK__1.vt[6], &c__1);
    for (j = 1; j <= 3; ++j) {
/* L1015: */
    _BLNK__1.vt[j + 8] = x[j - 1];
    }
    _BLNK__1.itilt = 1;
    hgt = _BLNK__1.ain[4];
    th = 0.f;
    y[2] = x[2];
    tt7 = _BLNK__1.ain[6];
    y[1] = x[1] + tt7 - hgt * .5f;
    tt6 = _BLNK__1.ain[5];
    if ((i__1 = _BLNK__1.nj2 - 13) < 0) {
    goto L1030;
    } else if (i__1 == 0) {
    goto L1025;
    } else {
    goto L1020;
    }
/*     ***** PERSPECTIVE BOND LABELS ***** */
L1020:
    y[0] = x[0] + tt6 - hgt * (real) ((6 - nj3) * 3 + 22) / 7.f;
    dist = sqrt(vv_(&xw[6], &xw[6])) / _BLNK__1.scal1;
    goto L1050;
/*     ***** PERSPECTIVE TITLES ***** */
L1025:
    y[0] = x[0] + tt6 - hgt * 215.f / 7.f;
    ilast = 18;
    dxw = hgt * 24.f / 7.f;
    dyw = 0.f;
    goto L1050;
L1030:
    dh = hgt * t6 / 7.f;
    dv = hgt * .5f;
    y[0] = y[0] - dh * costh + dv * sinth;
    y[1] = y[1] - dh * sinth - dv * costh;
    y[2] = 0.f;
/*     ***** PLOT VARIOUS LABELS ***** */
L1050:
    z__[2] = y[2];
    _BLNK__1.xo[2] = y[2];
    switch (nj3) {
    case 1:  goto L1060;
    case 2:  goto L1060;
    case 3:  goto L1060;
    case 4:  goto L1090;
    case 5:  goto L1090;
    case 6:  goto L1090;
    case 7:  goto L915;
    case 8:  goto L1105;
    case 9:  goto L1105;
    }
L1060:
    if (nj3 == 1) {
    goto L1061;
    }
/* *** if title begins in column 1, center it */
    if (*(unsigned char *)&_BLNK__1.title2[0] != ' ') {
    for (i__ = 1; i__ <= 72; ++i__) {
        *(unsigned char *)&tmpti[i__ - 1] = ' ';
        *(unsigned char *)&tmpti2[i__ - 1] = ' ';
/* L101: */
    }
    for (i__ = 1; i__ <= 18; ++i__) {
        i__1 = (i__ << 2) - 4;
        s_copy(tmpti + i__1, _BLNK__1.title2 + (i__ - 1 << 2), (i__ << 2) 
            - i__1, (ftnlen)4);
/* L102: */
    }
    for (i__ = 72; i__ >= 1; --i__) {
        if (*(unsigned char *)&tmpti[i__ - 1] != ' ') {
        klast = i__;
        goto L104;
        }
/* L103: */
    }
L104:
    ioffset = (72 - klast) / 2;
    i__1 = klast;
    for (i__ = 1; i__ <= i__1; ++i__) {
        i__2 = i__ + ioffset - 1;
        s_copy(tmpti2 + i__2, tmpti + (i__ - 1), i__ + ioffset - i__2, (
            ftnlen)1);
/* L105: */
    }
    for (i__ = 1; i__ <= 18; ++i__) {
        i__1 = (i__ << 2) - 4;
        s_copy(_BLNK__1.title2 + (i__ - 1 << 2), tmpti2 + i__1, (ftnlen)4,
             (i__ << 2) - i__1);
/* L106: */
    }
    }
L1061:
    i__1 = ilast;
    for (i__ = 1; i__ <= i__1; ++i__) {
    for (j = 1; j <= 3; j += 2) {
        z__[0] = y[0] + (real) (j - 2) * _BLNK__1.disp * .5f;
        for (k = 1; k <= 3; k += 2) {
        z__[1] = y[1] + (real) (k - 2) * _BLNK__1.disp * .5f;
        if (nj3 - 2 >= 0) {
            goto L1068;
        } else {
            goto L1065;
        }
/*     ***** PLOT CHEMICAL SYMBOL ***** */
L1065:
        simbol_(z__, &z__[1], &hgt, parms_1.chem + (l - 1 << 3), &th, 
            &c__6, (ftnlen)8);
        goto L1070;
/*     ***** PLOT TITLES ***** */
L1068:
        simbol_(z__, &z__[1], &hgt, _BLNK__1.title2 + (i__ - 1 << 2), 
            &th, &c__4, (ftnlen)4);
L1070:
        if (_BLNK__1.disp <= 0.f) {
            goto L1080;
        } else {
            goto L1075;
        }
L1075:
        ;
        }
    }
L1080:
    y[0] += dxw;
/* L1085: */
    y[1] += dyw;
    }
    goto L1199;
/*     ***** PLOT BOND DISTANCE LABELS ***** */
L1090:
    i9 = nj3 - 3;
    t9 = pow_ri(&c_b469, &i9);
    r__1 = dist * t9 + .5f;
    distr = r_int(&r__1) / t9 + 1e-4f;
    numbur_(y, &y[1], &hgt, &distr, &th, &i9);
    goto L1199;
/*     ***** PLOT CENTERED SYMBOLS ***** */
L1105:
    tt8 = _BLNK__1.ain[7];
/* *** ORTEP-II call */
/*     CALL SIMBOL(Y(1),Y(2),HGT,IFIX(TT8),TH,7-NJ3) */
/* *** Only one centered symbol (*) is available in ORTEP-III. */
/* *** It is triggered by the negative value for argument 6. */
/* *** Argument 4 is ignored by SIMBOL. */
    i__1 = 7 - nj3;
    simbol_(y, &y[1], &hgt, " ", &th, &i__1, (ftnlen)1);
    goto L1199;
L912:
    _BLNK__1.ng = 15;
L915:
    i__1 = _BLNK__1.nj * 100 + _BLNK__1.nj2;
    erpnt_(&_BLNK__1.ain[ii - 1], &i__1);
L1199:
    _BLNK__1.itilt = 0;
    return 0;
} /* f900_ */

/* Subroutine */ int f1000_(void)
{
    extern /* Subroutine */ int lap500_(integer *);

/* *** 1001 identical to 511 */
    lap500_(&c__1);
    return 0;
} /* f1000_ */

integer iend_(char *string, ftnlen string_len)
{
    /* System generated locals */
    integer ret_val;

    /* Builtin functions */
    integer i_len(char *, ftnlen);

    /* Local variables */
    static integer i__;

/* *** returns position of last non-space character in string */
    for (i__ = i_len(string, string_len); i__ >= 1; --i__) {
    if (*(unsigned char *)&string[i__ - 1] != ' ') {
        ret_val = i__;
        return ret_val;
    }
/* L800: */
    }
    ret_val = 1;
    return ret_val;
} /* iend_ */

/* Subroutine */ int lap500_(integer *ntype)
{
    /* Format strings */
    static char fmt_2[] = "(\0020\002,10x,\002OVERLAP MARGIN IS\002,f6.3,"
        "\002 INCH\002)";
    static char fmt_4[] = "(\0020\002,10x,\002CONTENTS OF SORTED ATOMS ARRA"
        "Y\002/(15x,10f10.0))";

    /* System generated locals */
    integer i__1;
    real r__1;
    doublereal d__1;

    /* Builtin functions */
    double d_int(doublereal *), sqrt(doublereal);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Local variables */
    static integer i__, j, k, m, j1, j2, k1, k2;
    static real t1;
    static integer ia;
    static doublereal qc[9] /* was [3][3] */, qd[9] /* was [3][3] */, td;
    static integer im;
    extern doublereal vv_(real *, real *);
    extern /* Subroutine */ int f800_(void);
    static doublereal td1, td2, vd1[3], vd2[3], td3;
    static integer ndg;
    extern doublereal vmv_(real *, real *, real *);
    extern /* Subroutine */ int xyz_(doublereal *, real *, integer *), paxes_(
        doublereal *, integer *), erpnt_(doublereal *, integer *);

    /* Fortran I/O blocks */
    static cilist io___440 = { 0, 0, 0, fmt_2, 0 };
    static cilist io___461 = { 0, 0, 0, fmt_4, 0 };


/*     ***** STORE PROJECTED ATOM CONICS AND BOND QUADRANGLES ***** */
/*     ***** ELIMINATE ALL PREVIOUSLY STORED LOCAL OVERLAP INFORMATION ** */
    olap_1.ncover = 0;
    olap_1.nqover = 0;
    if (*ntype >= 0) {
    goto L195;
    } else {
    goto L420;
    }
/*     ***** ELIMINATE ALL PREVIOUSLY STORED GLOBAL OVERLAP INFORMATION * */
L195:
    olap_1.nconic = 0;
    olap_1.nquad = 0;
    if (*ntype <= 0) {
    goto L420;
    } else {
    goto L200;
    }
/*     ***** CONSTANT FOR OVERLAP MARGIN (WHITE MARGIN AT OVERLAP) ***** */
L200:
    if (_BLNK__1.ain[0] < 0.) {
    goto L205;
    } else if (_BLNK__1.ain[0] == 0) {
    goto L215;
    } else {
    goto L210;
    }
/*     ***** NEGATIVE NUMBER OR POSITIVE INTEGER GIVES OVMRGN=0.0 ***** */
L205:
    olap_1.ovmrgn = 0.f;
    goto L220;
/*     ***** SET OVERLAP MARGIN WIDTH DIRECTLY IN INCHES ***** */
L210:
    olap_1.ovmrgn = _BLNK__1.ain[0] - d_int(_BLNK__1.ain);
    goto L220;
/*      ***** DEFAULT OPTION, OVERLAP MARGIN WIDTH AS A FUNCTION OF SCAL1 */
L215:
    if (_BLNK__1.scal1 < .25f) {
/* Computing MAX */
    r__1 = sqrt(_BLNK__1.scal1) * .05f;
    olap_1.ovmrgn = dmax(r__1,.01f);
    } else {
/* Computing MAX */
    r__1 = sqrt(_BLNK__1.scal1) * .03f;
    olap_1.ovmrgn = dmax(r__1,.025f);
    }
L220:
    if (_BLNK__1.nout >= 0) {
    io___440.ciunit = _BLNK__1.nout;
    s_wsfe(&io___440);
    do_fio(&c__1, (char *)&olap_1.ovmrgn, (ftnlen)sizeof(real));
    e_wsfe();
    }
/* L225: */
    if (_BLNK__1.latm <= 0) {
    goto L230;
    } else {
    goto L235;
    }
L230:
    _BLNK__1.ng = 12;
    i__1 = _BLNK__1.nj2 + 510;
    erpnt_(&c_b113, &i__1);
    goto L420;
/*     ***** SORT ATOMS LIST BY -VIEWDISTANCE OR BY Z PARAMETER */
L235:
    if (_BLNK__1.view <= 0.f) {
    goto L250;
    } else {
    goto L240;
    }
/*     ***** CALCULATE VIEWDISTANCES**2 *(-1) IF VIEW.GT.ZERO ***** */
L240:
    i__1 = _BLNK__1.latm;
    for (i__ = 1; i__ <= i__1; ++i__) {
    xyz_(&_BLNK__1.atomid[i__ - 1], _BLNK__1.v3, &c__2);
    _BLNK__1.v3[2] -= _BLNK__1.view;
/* L245: */
    _BLNK__1.atoms[i__ * 3 - 1] = -vv_(_BLNK__1.v3, _BLNK__1.v3);
    }
    goto L260;
/*     ***** STORE CARTESIAN COORDINATES IF VIEW.EQ.ZERO ***** */
L250:
    i__1 = _BLNK__1.latm;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L255: */
    xyz_(&_BLNK__1.atomid[i__ - 1], &_BLNK__1.atoms[i__ * 3 - 3], &c__2);
    }
/*     ***** SORTING PROCEDURE BY SHELL, COMM ACM 2,30 (1959) ***** */
L260:
    m = _BLNK__1.latm;
L265:
    m /= 2;
    if (m <= 0) {
    goto L300;
    } else {
    goto L270;
    }
L270:
    k = _BLNK__1.latm - m;
    j = 1;
L275:
    i__ = j;
L280:
    im = i__ + m;
    if (_BLNK__1.atoms[i__ * 3 - 1] - _BLNK__1.atoms[im * 3 - 1] <= 0.f) {
    goto L295;
    } else {
    goto L285;
    }
L285:
    td = _BLNK__1.atomid[i__ - 1];
    _BLNK__1.atomid[i__ - 1] = _BLNK__1.atomid[im - 1];
    _BLNK__1.atomid[im - 1] = td;
    t1 = _BLNK__1.atoms[i__ * 3 - 1];
    _BLNK__1.atoms[i__ * 3 - 1] = _BLNK__1.atoms[im * 3 - 1];
    _BLNK__1.atoms[im * 3 - 1] = t1;
    i__ -= m;
    if (i__ <= 0) {
    goto L295;
    } else {
    goto L280;
    }
L295:
    ++j;
    if (j - k <= 0) {
    goto L275;
    } else {
    goto L265;
    }
/*     ***** LOOP THROUGH ALL ATOMS IN SORTED ATOMS LIST ***** */
L300:
    i__1 = _BLNK__1.latm;
    for (ia = 1; ia <= i__1; ++ia) {
    xyz_(&_BLNK__1.atomid[ia - 1], &_BLNK__1.atoms[ia * 3 - 3], &c__2);
    paxes_(&_BLNK__1.atomid[ia - 1], &c__2);
    for (j = 1; j <= 3; ++j) {
        _BLNK__1.v1[j - 1] = _BLNK__1.atoms[j + ia * 3 - 4];
        vd1[j - 1] = _BLNK__1.v1[j - 1];
        for (k = 1; k <= 3; ++k) {
/* L305: */
        qd[j + k * 3 - 4] = _BLNK__1.q[j + k * 3 - 4];
        }
    }
    if (_BLNK__1.view <= 0.f) {
        goto L340;
    } else {
        goto L310;
    }
/*     ***** CALCULATE ENVELOPING CONE WITH ORIGIN AT VIEWPOINT ***** */
L310:
    _BLNK__1.v1[2] -= _BLNK__1.view;
    vd1[2] = _BLNK__1.v1[2];
/*     ***** FORM COFACTOR MATRIX ***** */
    for (j = 1; j <= 3; ++j) {
        j1 = j % 3 + 1;
        j2 = (j + 1) % 3 + 1;
        for (k = j; k <= 3; ++k) {
        k1 = k % 3 + 1;
        k2 = (k + 1) % 3 + 1;
        qc[j + k * 3 - 4] = qd[j1 + k1 * 3 - 4] * qd[j2 + k2 * 3 - 4] 
            - qd[j1 + k2 * 3 - 4] * qd[j2 + k1 * 3 - 4];
/* L315: */
        qc[k + j * 3 - 4] = qc[j + k * 3 - 4];
        }
    }
/*     ***** FORM POLARIZED COFACTOR MATRIX AND ADD TO ELLIPSOID MATRIX * */
/* Computing 2nd power */
    r__1 = _BLNK__1.scl;
    td2 = -(r__1 * r__1);
/*     ***** TD1 IS AN ARBITRARY SCALING FACTOR ***** */
    td1 = vmv_(_BLNK__1.v1, _BLNK__1.q, _BLNK__1.v1);
    for (j = 1; j <= 3; ++j) {
        j1 = j % 3 + 1;
        j2 = (j + 1) % 3 + 1;
        for (k = j; k <= 3; ++k) {
        k1 = k % 3 + 1;
        k2 = (k + 1) % 3 + 1;
        qd[j + k * 3 - 4] = (vd1[j2 - 1] * (qc[j1 + k1 * 3 - 4] * vd1[
            k2 - 1] - qc[j1 + k2 * 3 - 4] * vd1[k1 - 1]) + vd1[j1 
            - 1] * (vd1[k1 - 1] * qc[j2 + k2 * 3 - 4] - vd1[k2 - 
            1] * qc[j2 + k1 * 3 - 4]) + td2 * qd[j + k * 3 - 4]) /
             td1;
/* L320: */
        qd[k + j * 3 - 4] = qd[j + k * 3 - 4];
        }
/*     ***** PROJECTED ELLIPSE IN HOMOGENEOUS COORD OF WORKING SYSTEM *** */
        qd[j + 5] = -qd[j + 5] * _BLNK__1.view;
/* L325: */
        qd[j * 3 - 1] = -qd[j * 3 - 1] * _BLNK__1.view;
    }
/*     ***** PROJECT CENTER OF ATOM ONTO PROJECTION PLANE ***** */
    td1 = -_BLNK__1.view / vd1[2];
    vd2[0] = vd1[0] * td1;
    vd2[1] = vd1[1] * td1;
/*     ***** TRANSFORM TO NEW ORIGIN TO IMPROVE CONDITION OF MATRIX Q *** */
    for (j = 1; j <= 3; ++j) {
        for (k = 1; k <= 2; ++k) {
/* L330: */
        qd[j + 5] += qd[j + k * 3 - 4] * vd2[k - 1];
        }
    }
    for (j = 1; j <= 3; ++j) {
        for (k = 1; k <= 2; ++k) {
/* L335: */
        qd[j * 3 - 1] += vd2[k - 1] * qd[k + j * 3 - 4];
        }
    }
    _BLNK__1.v6[0] = _BLNK__1.xo[0] + vd2[0];
    _BLNK__1.v6[1] = _BLNK__1.xo[1] + vd2[1];
    goto L355;
/*     ***** CALCULATE ENVELOPING CYLINDER ALONG Z OF WORKING SYSTEM **** */
L340:
    for (j = 1; j <= 2; ++j) {
        for (k = 1; k <= 2; ++k) {
/* L345: */
        qd[j + k * 3 - 4] -= qd[j + 5] * qd[k + 5] / qd[8];
        }
    }
    for (j = 1; j <= 2; ++j) {
        qd[j + 5] = 0.f;
        qd[j * 3 - 1] = 0.f;
/* L350: */
        _BLNK__1.v6[j - 1] = _BLNK__1.xo[j - 1] + _BLNK__1.atoms[j + ia * 
            3 - 4];
    }
/*     ***** PROJECTED ELLIPSE IN HOMOGENEOUS COORD ABOUT CENTER OF ATOM */
/* Computing 2nd power */
    r__1 = _BLNK__1.scl;
    qd[8] = -(r__1 * r__1);
/*     ***** FIT RECTANGLE AROUND ELLIPSE ALLOWING OVERLAP MARGIN ***** */
/*     ***** FORM MATRIX OF COFACTORS ***** */
L355:
    for (j = 1; j <= 3; ++j) {
        j1 = j % 3 + 1;
        j2 = (j + 1) % 3 + 1;
        for (k = j; k <= 3; ++k) {
        k1 = k % 3 + 1;
        k2 = (k + 1) % 3 + 1;
/* L360: */
        qc[j + k * 3 - 4] = qd[j1 + k1 * 3 - 4] * qd[j2 + k2 * 3 - 4] 
            - qd[j1 + k2 * 3 - 4] * qd[j2 + k1 * 3 - 4];
        }
    }
/*     ***** RESCALE MATRIX OF COFACTORS SO THAT QC(3,3)=1.0 ***** */
    for (j = 1; j <= 3; ++j) {
        for (k = j; k <= 3; ++k) {
        qc[j + k * 3 - 4] /= qc[8];
/* L365: */
        qc[k + j * 3 - 4] = qc[j + k * 3 - 4];
        }
    }
    td2 = qd[8];
    ndg = 0;
    for (j = 1; j <= 2; ++j) {
/*     ***** SOLVE QUADRATIC EQUATION ***** */
/* Computing 2nd power */
        d__1 = qc[j * 3 - 1];
        t1 = d__1 * d__1 - qc[j + j * 3 - 4];
        if (t1 <= 0.f) {
        goto L370;
        } else {
        goto L375;
        }
/*     ***** ROUNDOFF PROBLEMS, RESET LIMITS IN X OR Y ***** */
L370:
        ndg = 1;
        _BLNK__1.v5[j - 1] = olap_1.ovmrgn + .001f;
        goto L380;
L375:
        _BLNK__1.v5[j - 1] = sqrt(t1) + olap_1.ovmrgn;
        _BLNK__1.v6[j - 1] += qc[j * 3 - 1];
        td2 += qd[j * 3 - 1] * qc[j * 3 - 1];
L380:
        olap_1.conic[(j << 1) - 1 + ia * 7 - 8] = _BLNK__1.v6[j - 1] - 
            _BLNK__1.v5[j - 1];
/* L385: */
        olap_1.conic[(j << 1) + ia * 7 - 8] = _BLNK__1.v6[j - 1] + 
            _BLNK__1.v5[j - 1];
    }
    if (ndg <= 0) {
        goto L390;
    } else {
        goto L395;
    }
L390:
    if (td2 >= 0.) {
        goto L395;
    } else {
        goto L400;
    }
/*     ***** ELLIPSE IMAGINARY DUE TO ROUNDOFF, RESET TO REAL VALUE ***** */
L395:
/* Computing 2nd power */
    r__1 = (olap_1.conic[ia * 7 - 6] - olap_1.conic[ia * 7 - 7]) * .5f;
    olap_1.conic[ia * 7 - 3] = 1.f / (r__1 * r__1);
    olap_1.conic[ia * 7 - 2] = 0.f;
/* Computing 2nd power */
    r__1 = (olap_1.conic[ia * 7 - 4] - olap_1.conic[ia * 7 - 5]) * .5f;
    olap_1.conic[ia * 7 - 1] = 1.f / (r__1 * r__1);
    goto L405;
/*     ***** STORE NORMALIZED QUADRATIC COEFFICIENTS FOR ELLIPSE ***** */
/*     ***** SCALED BY OVERLAP MARGIN PARAMETER ***** */
L400:
/* Computing 2nd power */
    r__1 = 1.f - olap_1.ovmrgn * 2.f / (_BLNK__1.v5[0] + _BLNK__1.v5[1]);
    td3 = -(r__1 * r__1) / td2;
    olap_1.conic[ia * 7 - 3] = qd[0] * td3;
    olap_1.conic[ia * 7 - 2] = qd[3] * td3;
    olap_1.conic[ia * 7 - 1] = qd[4] * td3;
L405:
    ;
    }
    olap_1.nconic = _BLNK__1.latm;
/*     ***** PRINT OUT SORTED ATOMS ARRAY ***** */
    if (_BLNK__1.nout >= 0) {
    io___461.ciunit = _BLNK__1.nout;
    s_wsfe(&io___461);
    i__1 = _BLNK__1.latm;
    for (j = 1; j <= i__1; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.atomid[j - 1], (ftnlen)sizeof(
            doublereal));
    }
    e_wsfe();
    }
/*     ***** STORE BOND QUADRANGLES IF SEARCH CODES ARE GIVEN ***** */
    if (_BLNK__1.ncd <= 0) {
    goto L420;
    } else {
    goto L410;
    }
/*     ***** GENERATE PSEUDO-INSTRUCTION 822 TO CALCULATE BONDS ***** */
L410:
    _BLNK__1.nj2 = 22;
    f800_();
/* *** the lines below have been moved to the end of F800 */
/*       IF(NQUAD)420,420,415 */
/* C     ***** PRINT OUT NUMBER OF BOND QUADRANGLES STORED ***** */
/* C     ***** PRINT OUT QUADRANGLE IDENTIFICATION ARRAY ***** */
/*   415 IF (NOUT.GE.0) */
/*      &WRITE (NOUT,6)NQUAD,INT(DM_Q-1),(QUAD(9,J),J=1,NQUAD) */
/*     6 FORMAT(1H0,10X,27HBOND OVERLAP ARRAY CONTAINS,I9,18H BONDS (MAXIMU */
/*      1M IS,I9,1H)/  11X,  66HATOM-PAIR NUMBERS IN ARRAY REFER TO SEQUENC */
/*      2E IN SORTED ATOMS ARRAY/(15X,10F10.0)) */
/* *** the lines above have been moved to the end of F800 */
L420:
    return 0;
} /* lap500_ */

/* Subroutine */ int lap700_(integer *na, integer *icq)
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal), atan(doublereal), cos(doublereal), d_sign(
        doublereal *, doublereal *), r_mod(real *, real *);

    /* Local variables */
    static integer i__, j, k, l, j1, j2, k1, k2;
    static real t1, t2, t6, t7, t8, t9;
    static integer ia, ka, kb;
    static doublereal qa[18]    /* was [3][3][2] */;
    static integer ij;
    static doublereal qc[18]    /* was [3][3][2] */, td;
    static real v12[6]  /* was [3][2] */;
    static doublereal pi;
    static integer iq, na1, na2;
    static doublereal phi;
    static real tid;
    static integer iqq, iqr, ity;
    static doublereal aov3, bov3, pov3, qov2, tidd;
    static real ymin[2], ymax[2], ovmr[2];
    static doublereal root;
    extern /* Subroutine */ int lapab_(integer *, integer *, integer *, 
        integer *);
    static doublereal deter[2];
    extern /* Subroutine */ int erpnt_(doublereal *, integer *);
    static integer itype;
    static doublereal aov3sq, pov3cu, qov2sq;
    extern /* Subroutine */ int lapcon_(real *, real *, real *, real *);

    pi = 3.1415926535897932f;
    *icq = 0;
    olap_1.ncover = 0;
    olap_1.nqover = 0;
    ovmr[0] = olap_1.ovmrgn;
    ovmr[1] = 0.f;
    if (olap_1.nconic - *na <= 0) {
    goto L200;
    } else {
    goto L205;
    }
L200:
    return 0;
/*     ***** ROUGH CHECK FOR OVERLAPPING ATOMS ***** */
L205:
    for (j = 1; j <= 2; ++j) {
    ymin[j - 1] = olap_1.conic[(j << 1) - 1 + *na * 7 - 8];
/* L210: */
    ymax[j - 1] = olap_1.conic[(j << 1) + *na * 7 - 8];
    }
    l = 0;
    i__1 = olap_1.nconic;
    for (ia = *na; ia <= i__1; ++ia) {
    if (ia - *na <= 0) {
        goto L230;
    } else {
        goto L215;
    }
L215:
    for (j = 1; j <= 2; ++j) {
        if (ymax[j - 1] - olap_1.conic[(j << 1) - 1 + ia * 7 - 8] <= 0.f) 
            {
        goto L420;
        } else {
        goto L220;
        }
L220:
        if (ymin[j - 1] - olap_1.conic[(j << 1) + ia * 7 - 8] >= 0.f) {
        goto L420;
        } else {
        goto L225;
        }
L225:
        ;
    }
/*     ***** EXACT CHECK FOR OVERLAPPING ATOMS ***** */
L230:
    if (l - 1 <= 0) {
        goto L235;
    } else {
        goto L240;
    }
L235:
    ++l;
L240:
    lapcon_(&olap_1.conic[ia * 7 - 7], _BLNK__1.da, &v12[l * 3 - 3], &
        ovmr[l - 1]);
    for (j = 1; j <= 3; ++j) {
        for (k = 1; k <= 3; ++k) {
/* L245: */
        qa[j + (k + l * 3) * 3 - 13] = _BLNK__1.da[j + k * 3 - 4];
        }
    }
/*     ***** CALCULATE COFACTORS AND DETERMINANTS ***** */
    deter[l - 1] = 0.f;
    for (j = 1; j <= 3; ++j) {
        j1 = (j + 3) % 3 + 1;
        j2 = (j + 1) % 3 + 1;
        for (k = 1; k <= 3; ++k) {
        k1 = (k + 3) % 3 + 1;
        k2 = (k + 1) % 3 + 1;
        td = qa[j1 + (k1 + l * 3) * 3 - 13] * qa[j2 + (k2 + l * 3) * 
            3 - 13] - qa[j1 + (k2 + l * 3) * 3 - 13] * qa[j2 + (
            k1 + l * 3) * 3 - 13];
        deter[l - 1] += td * qa[j + (k + l * 3) * 3 - 13];
/* L250: */
        qc[j + (k + l * 3) * 3 - 13] = td;
        }
    }
/*     ***** DETER(L) IS THE DETERMINANT TIMES 3 ***** */
    if (l - 1 <= 0) {
        goto L420;
    } else {
        goto L255;
    }
/*     ***** FORM CHARACTERISTIC EQUATION AND EXAMINE ITS ROOTS ***** */
L255:
    aov3 = 0.f;
    bov3 = 0.f;
    for (j = 1; j <= 3; ++j) {
        for (k = 1; k <= 3; ++k) {
        aov3 += qc[j + (k + 6) * 3 - 13] * qa[j + (k + 3) * 3 - 13];
/* L260: */
        bov3 += qc[j + (k + 3) * 3 - 13] * qa[j + (k + 6) * 3 - 13];
        }
    }
    aov3 /= deter[1];
/* Computing 2nd power */
    d__1 = aov3;
    aov3sq = d__1 * d__1;
    bov3 /= deter[1];
    pov3 = bov3 - aov3sq;
    qov2 = aov3 * (aov3sq - bov3 * 1.5) + deter[0] / (deter[1] * 2.);
/*     ***** CHECK DISCRIMINANT OF CHARACTERISTIC CUBIC EQUATION ***** */
    itype = 0;
/* Computing 3rd power */
    d__1 = pov3;
    pov3cu = d__1 * (d__1 * d__1);
/* Computing 2nd power */
    d__1 = qov2;
    qov2sq = d__1 * d__1;
    if ((d__1 = pov3cu + qov2sq) < 0.) {
        goto L270;
    } else if (d__1 == 0) {
        goto L310;
    } else {
        goto L265;
    }
L265:
    if (pov3cu * 1.00001f + qov2sq <= 0.) {
        goto L310;
    } else {
        goto L400;
    }
L270:
    if (pov3cu + qov2sq * 1.00001f >= 0.) {
        goto L310;
    } else {
        goto L275;
    }
/*     ***** THREE REAL ROOTS, ALL DIFFERENT ***** */
L275:
    itype = 1;
/*     ***** NO INTERSECTION IF A/3 AND B/3 INVARIANTS ARE NEGATIVE ***** */
    if (aov3 >= 0.) {
        goto L285;
    } else {
        goto L280;
    }
L280:
    if (bov3 >= 0.) {
        goto L285;
    } else {
        goto L420;
    }
/*     ***** CALCULATE ONE ROOT OF CHARACTERISTIC CUBIC EQUATION ***** */
L285:
    if (qov2 != 0.) {
        goto L295;
    } else {
        goto L290;
    }
L290:
    phi = pi / 2.;
    goto L305;
L295:
    phi = atan(-sqrt(-pov3cu - qov2sq) / qov2);
    if (phi >= 0.) {
        goto L305;
    } else {
        goto L300;
    }
L300:
    phi += pi;
L305:
    root = sqrt(-pov3) * 2. * cos(phi / 3.) - aov3;
    goto L325;
/*     ***** THREE REAL ROOTS, AT LEAST TWO ARE EQUAL ***** */
L310:
    itype = 2;
/*     ***** CHECK SIGNS OF INVARIANTS A/3 AND B/3 ***** */
    if (aov3 >= 0.) {
        goto L320;
    } else {
        goto L315;
    }
L315:
    if (bov3 >= 0.) {
        goto L320;
    } else {
        goto L420;
    }
/*     ***** CALCULATE REPEATED ROOT OF CUBIC EQUATION ***** */
L320:
    d__1 = sqrt(-pov3);
    root = d_sign(&d__1, &qov2) - aov3;
/*     ***** FORM DEGENERATE CONIC (LINE PAIR WHICH MAY BE COINCIDENT) ** */
L325:
    for (j = 1; j <= 3; ++j) {
        for (k = 1; k <= 3; ++k) {
/* L330: */
        _BLNK__1.da[j + k * 3 - 4] = qa[j + (k + 3) * 3 - 13] + root *
             qa[j + (k + 6) * 3 - 13];
        }
    }
/*     ***** EXAMINE INVARIANTS OF THE DEGENERATE CONIC ***** */
    t6 = _BLNK__1.da[0] * _BLNK__1.da[4];
/* Computing 2nd power */
    r__1 = _BLNK__1.da[3];
    t7 = r__1 * r__1;
/*     ***** NEGATIVE DENOTES REAL INTERSECTING LINE PAIR ***** */
/*     ***** POSITIVE DENOTES IMAGINARY LINES INTERSECTING AT REAL POINT */
    if ((r__1 = t6 - t7) < 0.f) {
        goto L335;
    } else if (r__1 == 0) {
        goto L345;
    } else {
        goto L340;
    }
L335:
    if (t6 * 1.0001f - t7 >= 0.f) {
        goto L345;
    } else {
        goto L400;
    }
L340:
    if (t6 - t7 * 1.0001f <= 0.f) {
        goto L345;
    } else {
        goto L365;
    }
L345:
    t8 = _BLNK__1.da[8] * (_BLNK__1.da[0] + _BLNK__1.da[4]);
/* Computing 2nd power */
    r__1 = _BLNK__1.da[6];
/* Computing 2nd power */
    r__2 = _BLNK__1.da[7];
    t9 = r__1 * r__1 + r__2 * r__2;
/*     ***** NEGATIVE DENOTES REAL PARALLEL LINE PAIR ***** */
/*     ***** POSITIVE DENOTES IMAGINARY PARALLELS ***** */
/*     ***** ZERO DENOTES ONE REAL LINE (COINCIDENT PARALLELS) ***** */
    if ((r__1 = t8 - t9) < 0.f) {
        goto L350;
    } else if (r__1 == 0) {
        goto L360;
    } else {
        goto L355;
    }
L350:
    if (t8 * 1.0001f - t9 >= 0.f) {
        goto L360;
    } else {
        goto L400;
    }
L355:
    if (t8 - t9 * 1.0001f <= 0.f) {
        goto L360;
    } else {
        goto L365;
    }
/*     ***** COINCIDENT LINE PAIR FOUND FOR THE REPEATED ROOT ***** */
L360:
    itype = 3;
/*     ***** COMPARE AREAS OF CONICS ***** */
L365:
    ka = 1;
    kb = 2;
    if (qc[(ka * 3 + 3) * 3 - 10] - qc[(kb * 3 + 3) * 3 - 10] >= 0.) {
        goto L375;
    } else {
        goto L370;
    }
L370:
    ka = 2;
    kb = 1;
/*     ***** SEE IF ONE CONIC IS INSIDE THE OTHER CONIC ***** */
L375:
    t1 = 0.f;
    for (j = 1; j <= 3; ++j) {
        t2 = qa[j + (kb * 3 + 3) * 3 - 13];
        for (k = 1; k <= 2; ++k) {
/* L380: */
        t2 += qa[j + (k + kb * 3) * 3 - 13] * v12[k + ka * 3 - 4];
        }
/* L385: */
        t1 += v12[j + ka * 3 - 4] * t2;
    }
/*     ***** DISCARD IF KA IS OUTSIDE KB ***** */
    if (t1 <= 0.f) {
        goto L390;
    } else {
        goto L420;
    }
L390:
    if (ka - 1 <= 0) {
        goto L395;
    } else {
        goto L400;
    }
/*     ***** THE OVERLAPPING ATOM HIDES THE ORIGINAL ATOM ***** */
L395:
    *icq = -1;
    return 0;
/*     ***** STORE OVERLAPPING ATOM ***** */
L400:
    ++(*icq);
    if (olap_1.ncover - 20 >= 0) {
        goto L405;
    } else {
        goto L410;
    }
L405:
    _BLNK__1.ng = 17;
    erpnt_(&_BLNK__1.atomid[ia - 1], &c__700);
    --olap_1.ncover;
L410:
    ++olap_1.ncover;
    ij = 1;
    for (i__ = 1; i__ <= 3; ++i__) {
        for (j = i__; j <= 3; ++j) {
        olap_1.cover[ij + olap_1.ncover * 6 - 7] = qa[i__ + (j + 6) * 
            3 - 13];
/* L415: */
        ++ij;
        }
    }
    olap_1.kc[olap_1.ncover - 1] = ia;
L420:
    ;
    }
/*     ***** SECOND PART OF SUBROUTINE CHECKS FOR BONDS OVER THE ATOM *** */
/* L425: */
    if (olap_1.nquad <= 0) {
    goto L470;
    } else {
    goto L430;
    }
L430:
    ity = 0;
/*     ***** ROUGH CHECK FOR OVERLAPPING BONDS ***** */
    i__1 = olap_1.nquad;
    for (iq = 1; iq <= i__1; ++iq) {
    tid = olap_1.quad[iq * 9 - 1];
    tidd = tid;
    na1 = tid / 1e3f;
    na2 = r_mod(&tid, &c_b1321);
    if (*na - na2 <= 0) {
        goto L435;
    } else {
        goto L465;
    }
L435:
    for (j = 1; j <= 2; ++j) {
/* Computing MIN */
        r__1 = olap_1.quad[j + iq * 9 - 10], r__2 = olap_1.quad[j + 2 + 
            iq * 9 - 10], r__1 = min(r__1,r__2), r__2 = olap_1.quad[j 
            + 4 + iq * 9 - 10], r__1 = min(r__1,r__2), r__2 = 
            olap_1.quad[j + 6 + iq * 9 - 10];
        if (ymax[j - 1] - dmin(r__1,r__2) <= 0.f) {
        goto L465;
        } else {
        goto L440;
        }
L440:
/* Computing MAX */
        r__1 = olap_1.quad[j + iq * 9 - 10], r__2 = olap_1.quad[j + 2 + 
            iq * 9 - 10], r__1 = max(r__1,r__2), r__2 = olap_1.quad[j 
            + 4 + iq * 9 - 10], r__1 = max(r__1,r__2), r__2 = 
            olap_1.quad[j + 6 + iq * 9 - 10];
        if (ymin[j - 1] - dmax(r__1,r__2) >= 0.f) {
        goto L465;
        } else {
        goto L445;
        }
L445:
        ;
    }
/*     ***** EXACT CHECK FOR OVERLAPPING BONDS ***** */
/* L450: */
    --ity;
    iqq = 0;
    iqr = iq;
    lapab_(&iqr, na, &iqq, &ity);
    if (iqq >= 0) {
        goto L460;
    } else {
        goto L455;
    }
L455:
    *icq = -1;
    return 0;
L460:
    ++(*icq);
    if (olap_1.nqover - 30 >= 0) {
        goto L470;
    } else {
        goto L465;
    }
L465:
    ;
    }
L470:
    return 0;
} /* lap700_ */

/* Subroutine */ int lap800_(integer *na1, integer *na2, integer *icq)
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2;

    /* Builtin functions */
    double sqrt(doublereal), r_mod(real *, real *);

    /* Local variables */
    static integer j, k, l, k0, k1, l1;
    static real t1, t2, y1[2], y2[2], t3, t4, t5, t6;
    static integer ia, ib;
    static real fl[16]  /* was [4][4] */;
    static integer iq;
    extern doublereal vv_(real *, real *);
    static integer nb2, nb1, iaq;
    static real qua[12] /* was [3][4] */;
    static integer iqq;
    static real tol, vue[3];
    static integer ity;
    static real tid1, tid2;
    static integer na1p1;
    static doublereal tidd;
    extern /* Subroutine */ int difv_(real *, real *, real *);
    static real ymin[2];
    extern /* Subroutine */ int norm_(real *, real *, real *, integer *);
    static real ymax[2];
    extern /* Subroutine */ int lapab_(integer *, integer *, integer *, 
        integer *), erpnt_(doublereal *, integer *);
    static integer itype;
    extern /* Subroutine */ int unity_(real *, real *, integer *);

/*     ***** SUBROUTINE CHECKS FOR ATOMS AND BONDS OVERLAPPING A BOND *** */
    iq = 0;
    *icq = 0;
    if (*na1 * *na2 <= 0) {
    goto L245;
    } else {
    goto L195;
    }
L195:
    tid1 = (real) (*na1) * 1e3f + (real) (*na2);
    if (olap_1.nconic <= 0) {
    goto L245;
    } else {
    goto L200;
    }
L200:
    if (_BLNK__1.nj2 - 21 >= 0) {
    goto L205;
    } else {
    goto L250;
    }
/*     ***** PART 1, CALLED FROM BOND, STORES BOND OUTLINE INFORMATION ** */
L205:
    if (olap_1.nquad - 3e3f + 1 >= 0.f) {
    goto L210;
    } else {
    goto L215;
    }
L210:
    _BLNK__1.ng = 16;
    erpnt_(&_BLNK__1.atomid[*na1 - 1], &c__822);
    goto L245;
L215:
    ++olap_1.nquad;
/*     ***** CALCULATE OVERLAP MARGIN FOR BOND QUADRANGLE ***** */
    t1 = 0.f;
    t2 = 0.f;
    for (j = 1; j <= 2; ++j) {
    y1[j - 1] = _BLNK__1.dp[j - 1] - _BLNK__1.dp[j + 127];
    y2[j - 1] = _BLNK__1.dp[j + 1] - _BLNK__1.dp[j + 129];
/* Computing 2nd power */
    r__1 = y1[j - 1];
    t1 += r__1 * r__1;
/* L220: */
/* Computing 2nd power */
    r__1 = y2[j - 1];
    t2 += r__1 * r__1;
    }
    if (t1 * t2 <= 0.f) {
    goto L225;
    } else {
    goto L230;
    }
L225:
    t1 = 0.f;
    t2 = 0.f;
    goto L235;
L230:
    t1 = olap_1.ovmrgn / sqrt(t1);
    t2 = olap_1.ovmrgn / sqrt(t2);
/*     ***** STORE BOND QUADRANGLE ***** */
L235:
    for (j = 1; j <= 2; ++j) {
    y1[j - 1] *= t1;
    y2[j - 1] *= t2;
    olap_1.quad[j + olap_1.nquad * 9 - 10] = _BLNK__1.dp[j - 1] + y1[j - 
        1];
    olap_1.quad[j + 2 + olap_1.nquad * 9 - 10] = _BLNK__1.dp[j + 1] + y2[
        j - 1];
    olap_1.quad[j + 4 + olap_1.nquad * 9 - 10] = _BLNK__1.dp[j + 129] - 
        y2[j - 1];
/* L240: */
    olap_1.quad[j + 6 + olap_1.nquad * 9 - 10] = _BLNK__1.dp[j + 127] - 
        y1[j - 1];
    }
    olap_1.quad[olap_1.nquad * 9 - 1] = tid1;
L245:
    return 0;
/*     ***** PART 2, CALLED FROM BOND, OVERLAP CHECK FOR BOND NA1-NA2 *** */
L250:
    olap_1.ncover = 0;
    olap_1.nqover = 0;
    tol = 1e-5f;
    if (olap_1.nconic - *na1 <= 0) {
    goto L245;
    } else {
    goto L255;
    }
/*     ***** SAVE QUADRANGLE TEMPORARILY ***** */
L255:
    iq = olap_1.nquad + 1;
    for (j = 1; j <= 2; ++j) {
    olap_1.quad[j + iq * 9 - 10] = _BLNK__1.dp[j - 1];
    olap_1.quad[j + 2 + iq * 9 - 10] = _BLNK__1.dp[j + 1];
    olap_1.quad[j + 4 + iq * 9 - 10] = _BLNK__1.dp[j + 129];
/* L260: */
    olap_1.quad[j + 6 + iq * 9 - 10] = _BLNK__1.dp[j + 127];
    }
    olap_1.quad[iq * 9 - 1] = tid1;
/*     ***** FIT RECTANGLE AROUND QUADRANGLE ***** */
/* L265: */
    for (j = 1; j <= 2; ++j) {
/* Computing MIN */
    r__1 = _BLNK__1.dp[j - 1], r__2 = _BLNK__1.dp[j + 1], r__1 = min(r__1,
        r__2), r__2 = _BLNK__1.dp[j + 129], r__1 = min(r__1,r__2), 
        r__2 = _BLNK__1.dp[j + 127];
    ymin[j - 1] = dmin(r__1,r__2);
/* L270: */
/* Computing MAX */
    r__1 = _BLNK__1.dp[j - 1], r__2 = _BLNK__1.dp[j + 1], r__1 = max(r__1,
        r__2), r__2 = _BLNK__1.dp[j + 129], r__1 = max(r__1,r__2), 
        r__2 = _BLNK__1.dp[j + 127];
    ymax[j - 1] = dmax(r__1,r__2);
    }
/*     ***** ROUGH CHECK FOR ATOM-OVER-BOND OVERLAP ***** */
    na1p1 = *na1 + 1;
    ity = 0;
    i__1 = olap_1.nconic;
    for (ia = na1p1; ia <= i__1; ++ia) {
    for (j = 1; j <= 2; ++j) {
        if (ia - *na2 != 0) {
        goto L275;
        } else {
        goto L305;
        }
L275:
        if (ymax[j - 1] - olap_1.conic[(j << 1) - 1 + ia * 7 - 8] <= 0.f) 
            {
        goto L305;
        } else {
        goto L280;
        }
L280:
        if (ymin[j - 1] - olap_1.conic[(j << 1) + ia * 7 - 8] >= 0.f) {
        goto L305;
        } else {
        goto L285;
        }
L285:
        ;
    }
/*     ***** CHECK FOR TRUE ATOM-OVER-BOND OVERLAP ***** */
    ++ity;
    iaq = ia;
    lapab_(&iq, &iaq, &iqq, &ity);
    if (iqq < 0) {
        goto L290;
    } else if (iqq == 0) {
        goto L305;
    } else {
        goto L300;
    }
L300:
    ++(*icq);
    if (olap_1.ncover - 20 >= 0) {
        goto L310;
    } else {
        goto L305;
    }
L305:
    ;
    }
L310:
    if (olap_1.nquad <= 0) {
    goto L295;
    } else {
    goto L315;
    }
/*     ***** HIDDEN BOND ***** */
L290:
    *icq = -1;
L295:
    return 0;
/*     ***** ROUGH CHECK FOR BOND-OVER-BOND OVERLAP ***** */
L315:
    difv_(&_BLNK__1.atoms[*na2 * 3 - 3], &_BLNK__1.atoms[*na1 * 3 - 3], 
        _BLNK__1.v1);
    unity_(_BLNK__1.v1, _BLNK__1.v1, &c__1);
    vue[0] = _BLNK__1.atoms[*na1 * 3 - 3];
    vue[1] = _BLNK__1.atoms[*na1 * 3 - 2];
    vue[2] = _BLNK__1.atoms[*na1 * 3 - 1] - _BLNK__1.view;
    i__1 = olap_1.nquad;
    for (ib = 1; ib <= i__1; ++ib) {
    tid2 = olap_1.quad[ib * 9 - 1];
    if (tid1 - tid2 != 0.f) {
        goto L320;
    } else {
        goto L495;
    }
L320:
    nb2 = r_mod(&tid2, &c_b1321);
    nb1 = tid2 / 1e3f;
    if (*na1 - nb2 >= 0) {
        goto L495;
    } else {
        goto L325;
    }
L325:
    for (j = 1; j <= 2; ++j) {
/* Computing MIN */
        r__1 = olap_1.quad[j + ib * 9 - 10], r__2 = olap_1.quad[j + 2 + 
            ib * 9 - 10], r__1 = min(r__1,r__2), r__2 = olap_1.quad[j 
            + 4 + ib * 9 - 10], r__1 = min(r__1,r__2), r__2 = 
            olap_1.quad[j + 6 + ib * 9 - 10];
        if (ymax[j - 1] - dmin(r__1,r__2) <= 0.f) {
        goto L495;
        } else {
        goto L330;
        }
L330:
/* Computing MAX */
        r__1 = olap_1.quad[j + ib * 9 - 10], r__2 = olap_1.quad[j + 2 + 
            ib * 9 - 10], r__1 = max(r__1,r__2), r__2 = olap_1.quad[j 
            + 4 + ib * 9 - 10], r__1 = max(r__1,r__2), r__2 = 
            olap_1.quad[j + 6 + ib * 9 - 10];
        if (ymin[j - 1] - dmax(r__1,r__2) >= 0.f) {
        goto L495;
        } else {
        goto L335;
        }
L335:
        ;
    }
/*     ***** SET UP LINEAR FORMS FOR EDGES OF QUADRANGLE ***** */
    for (l = 1; l <= 4; ++l) {
        k = l << 1;
        k1 = k % 8 + 2;
        qua[l * 3 - 3] = olap_1.quad[k + ib * 9 - 10] - olap_1.quad[k1 + 
            ib * 9 - 10];
        qua[l * 3 - 2] = olap_1.quad[k1 - 1 + ib * 9 - 10] - olap_1.quad[
            k - 1 + ib * 9 - 10];
        qua[l * 3 - 1] = olap_1.quad[k - 1 + ib * 9 - 10] * olap_1.quad[
            k1 + ib * 9 - 10] - olap_1.quad[k + ib * 9 - 10] * 
            olap_1.quad[k1 - 1 + ib * 9 - 10];
/*     ***** NORMALIZE LINE EQUATION COEFFICIENTS ***** */
/* Computing 2nd power */
        r__1 = qua[l * 3 - 3];
/* Computing 2nd power */
        r__2 = qua[l * 3 - 2];
        t1 = sqrt(r__1 * r__1 + r__2 * r__2);
        if (t1 <= 0.f) {
        goto L495;
        } else {
        goto L340;
        }
L340:
        for (j = 1; j <= 3; ++j) {
/* L345: */
        qua[j + l * 3 - 4] /= t1;
        }
    }
/*     ***** EVALUATE LINEAR FORMS AND SIGNATURES FOR QUADRANGLE ***** */
    t3 = 3.f;
    for (k = 1; k <= 4; ++k) {
        t2 = 3.f;
        j = k << 1;
        for (l = 1; l <= 4; ++l) {
        t1 = olap_1.quad[j - 1 + iq * 9 - 10] * qua[l * 3 - 3] + 
            olap_1.quad[j + iq * 9 - 10] * qua[l * 3 - 2] + qua[l 
            * 3 - 1];
        if (t1 >= 0.f) {
            goto L355;
        } else {
            goto L350;
        }
L350:
        t2 += -1.f;
L355:
        fl[l + (k << 2) - 5] = t1;
        }
        if (t2 >= 0.f) {
        goto L365;
        } else {
        goto L360;
        }
L360:
        t3 += -1.f;
L365:
        ;
    }
/*     ***** CHECK FOR 4 POINTS INSIDE QUADRANGLE ***** */
    if (t3 >= 0.f) {
        goto L375;
    } else {
        goto L370;
    }
L370:
    itype = -1;
    goto L415;
/*     ***** CHECK FOR 1 TO 3 POINTS INSIDE QUADRANGLE **** */
L375:
    if (t3 - 3.f >= 0.f) {
        goto L385;
    } else {
        goto L380;
    }
L380:
    itype = 0;
    goto L415;
/*     ***** DETERMINE WHICH EDGES ARE CROSSED BY THE 4 LINE SEGMENTS *** */
L385:
    for (l = 1; l <= 4; ++l) {
        l1 = l % 4 + 1;
/*     ***** LINE SEGMENT L FROM POINT Y1 TO POINT Y2 ***** */
        y1[0] = olap_1.quad[(l << 1) - 1 + iq * 9 - 10];
        y1[1] = olap_1.quad[(l << 1) + iq * 9 - 10];
        y2[0] = olap_1.quad[(l1 << 1) - 1 + iq * 9 - 10];
        y2[1] = olap_1.quad[(l1 << 1) + iq * 9 - 10];
        for (k = 1; k <= 4; ++k) {
        t1 = fl[k + (l << 2) - 5];
        t2 = fl[k + (l1 << 2) - 5];
        t3 = t1 - t2;
/*     ***** T1 AND T2 MUST HAVE OPPOSITE SIGNS FOR INTERSECTION TO OCCUR */
        if (t1 * t2 <= 0.f) {
            goto L390;
        } else {
            goto L405;
        }
/*     ***** COMPONENT OF SEGMENT L PERPENDICULAR TO EDGE K OF IB IS T3 */
L390:
        if (dabs(t3) - 1e-5f <= 0.f) {
            goto L405;
        } else {
            goto L395;
        }
/*     ***** CALCULATE COORDINATES OF INTERSECTION ***** */
L395:
        t4 = (t1 * y2[0] - t2 * y1[0]) / t3;
        t5 = (t1 * y2[1] - t2 * y1[1]) / t3;
        k0 = k << 1;
        k1 = k % 4 + 1 << 1;
/*     ***** IS INTERSECTION WITHIN QUADRANGLE IQ ***** */
        t6 = (t4 - olap_1.quad[k0 - 1 + ib * 9 - 10]) * (olap_1.quad[
            k1 - 1 + ib * 9 - 10] - t4) + (t5 - olap_1.quad[k0 + 
            ib * 9 - 10]) * (olap_1.quad[k1 + ib * 9 - 10] - t5);
        if (dabs(t6) - 1e-4f <= 0.f) {
            goto L410;
        } else {
            goto L400;
        }
L400:
        if (t6 >= 0.f) {
            goto L410;
        } else {
            goto L405;
        }
L405:
        ;
        }
    }
    goto L495;
L410:
    itype = 1;
/*     ***** CHECK OVER/UNDER AMBIGUITY ***** */
L415:
    if ((*na1 - nb1) * (*na2 - nb2) * (*na2 - nb1) != 0) {
        goto L425;
    } else {
        goto L420;
    }
/*     ***** BONDS SHARE AN ATOM ***** */
L420:
    if (*na1 + *na2 - nb1 - nb2 >= 0) {
        goto L495;
    } else {
        goto L465;
    }
L425:
    difv_(&_BLNK__1.atoms[nb2 * 3 - 3], &_BLNK__1.atoms[nb1 * 3 - 3], 
        _BLNK__1.v2);
    difv_(&_BLNK__1.atoms[nb1 * 3 - 3], &_BLNK__1.atoms[*na1 * 3 - 3], 
        _BLNK__1.v4);
    unity_(_BLNK__1.v2, _BLNK__1.v2, &c__1);
    unity_(_BLNK__1.v4, _BLNK__1.v4, &c__1);
    norm_(_BLNK__1.v1, _BLNK__1.v2, _BLNK__1.v3, &c__1);
    if (vv_(_BLNK__1.v3, _BLNK__1.v3) - tol <= 0.f) {
        goto L430;
    } else {
        goto L435;
    }
/*     ***** PARALLEL BONDS, RECALCULATE V3 ***** */
L430:
    norm_(_BLNK__1.v1, _BLNK__1.v4, _BLNK__1.v5, &c__1);
    norm_(_BLNK__1.v5, _BLNK__1.v1, _BLNK__1.v3, &c__1);
/*     ***** CHECK FOR COLLINEAR BONDS ***** */
    if (vv_(_BLNK__1.v3, _BLNK__1.v3) - tol <= 0.f) {
        goto L465;
    } else {
        goto L450;
    }
L435:
    if (vv_(_BLNK__1.v3, _BLNK__1.v4) + tol >= 0.f) {
        goto L450;
    } else {
        goto L440;
    }
L440:
    for (j = 1; j <= 3; ++j) {
/* L445: */
        _BLNK__1.v3[j - 1] = -_BLNK__1.v3[j - 1];
    }
/*     ***** V3 IS NORMAL TO BONDS IQ AND IB GOING FROM IQ TOWARD IB *** */
L450:
    if (_BLNK__1.view <= 0.f) {
        goto L455;
    } else {
        goto L460;
    }
L455:
    if (_BLNK__1.v3[2] <= 0.f) {
        goto L495;
    } else {
        goto L465;
    }
L460:
    if (vv_(vue, _BLNK__1.v3) >= 0.f) {
        goto L495;
    } else {
        goto L465;
    }
/*     ***** OVERLAPPING BOND FOUND ***** */
L465:
    ++(*icq);
    if (itype >= 0) {
        goto L475;
    } else {
        goto L470;
    }
/*     ***** HIDDEN BOND ***** */
L470:
    *icq = -1;
    return 0;
/*     ***** STORE INTERFERING QUADRANGLE ***** */
L475:
    if (olap_1.nqover - 30 >= 0) {
        goto L480;
    } else {
        goto L485;
    }
L480:
    _BLNK__1.ng = 17;
    tidd = tid2;
    erpnt_(&tidd, &c__800);
    return 0;
L485:
    ++olap_1.nqover;
    for (k = 1; k <= 4; ++k) {
        for (j = 1; j <= 3; ++j) {
/* L490: */
        olap_1.qover[j + (k + (olap_1.nqover << 2)) * 3 - 16] = qua[j 
            + k * 3 - 4];
        }
    }
    olap_1.kq[olap_1.nqover - 1] = ib;
L495:
    ;
    }
/* L500: */
    return 0;
} /* lap800_ */

/* Subroutine */ int lapab_(integer *iq, integer *ia, integer *icq, integer *
    ity)
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2;

    /* Builtin functions */
    double r_mod(real *, real *), sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, l, k1, l1;
    static real t1, t2, t3, t4, t5, bf[4];
    static integer ij;
    static real qf[5];
    extern doublereal vv_(real *, real *);
    static integer na1, na2;
    static real con[9]  /* was [3][3] */, tid, qua[12]  /* was [3][4] */;
    static doublereal tidd;
    extern /* Subroutine */ int difv_(real *, real *, real *), norm_(real *, 
        real *, real *, integer *), erpnt_(doublereal *, integer *);
    static integer itype;
    extern /* Subroutine */ int unity_(real *, real *, integer *), lapcon_(
        real *, real *, real *, real *);

/*     ***** SUBROUTINE CHECKS FOR OVERLAP BETWEEN ATOMS AND BONDS ***** */
/*     ***** CALLED BY SUBROUTINES LAP700 AND LAP800 ***** */
    tid = olap_1.quad[*iq * 9 - 1];
    na1 = tid / 1e3f;
    na2 = r_mod(&tid, &c_b1321);
/*     ***** ITY.GT.0, CHECK FOR ATOMS OVER A BOND **** */
/*     ***** ITY.LT.0, CHECK FOR BONDS OVER AN ATOM ***** */
    *icq = 0;
    if (*ity < 0) {
    goto L210;
    } else if (*ity == 0) {
    goto L200;
    } else {
    goto L205;
    }
L200:
    return 0;
L205:
    lapcon_(&olap_1.conic[*ia * 7 - 7], con, _BLNK__1.v1, &c_b317);
    if (*ity - 2 >= 0) {
    goto L240;
    } else {
    goto L220;
    }
L210:
    if (*ity + 2 <= 0) {
    goto L220;
    } else {
    goto L215;
    }
L215:
    lapcon_(&olap_1.conic[*ia * 7 - 7], con, _BLNK__1.v1, &olap_1.ovmrgn);
/*     ***** SET UP LINEAR FORMS FOR EDGES OF QUADRANGLE ***** */
L220:
    for (l = 1; l <= 4; ++l) {
    k = l << 1;
    k1 = k % 8 + 2;
    qua[l * 3 - 3] = olap_1.quad[k + *iq * 9 - 10] - olap_1.quad[k1 + *iq 
        * 9 - 10];
    qua[l * 3 - 2] = olap_1.quad[k1 - 1 + *iq * 9 - 10] - olap_1.quad[k - 
        1 + *iq * 9 - 10];
    qua[l * 3 - 1] = olap_1.quad[k - 1 + *iq * 9 - 10] * olap_1.quad[k1 + 
        *iq * 9 - 10] - olap_1.quad[k + *iq * 9 - 10] * olap_1.quad[
        k1 - 1 + *iq * 9 - 10];
/* Computing 2nd power */
    r__1 = qua[l * 3 - 3];
/* Computing 2nd power */
    r__2 = qua[l * 3 - 2];
    t1 = sqrt(r__1 * r__1 + r__2 * r__2);
    if (t1 <= 0.f) {
        goto L225;
    } else {
        goto L230;
    }
L225:
    *ity = 0;
    *icq = 0;
    goto L430;
/*     ***** TRANSFORM COEFFICIENTS FOR EDGES TO NORMAL FORM ***** */
L230:
    for (j = 1; j <= 3; ++j) {
/* L235: */
        qua[j + l * 3 - 4] /= t1;
    }
    }
/*     ***** EVALUATE 4 QUADRATIC AND 4 BILINEAR FORMS ***** */
L240:
    _BLNK__1.v2[2] = 1.f;
    _BLNK__1.v3[2] = 1.f;
    t2 = 3.f;
    for (l = 1; l <= 4; ++l) {
    l1 = l % 4 + 1 << 1;
    _BLNK__1.v2[0] = olap_1.quad[(l << 1) - 1 + *iq * 9 - 10];
    _BLNK__1.v2[1] = olap_1.quad[(l << 1) + *iq * 9 - 10];
    _BLNK__1.v3[0] = olap_1.quad[l1 - 1 + *iq * 9 - 10];
    _BLNK__1.v3[1] = olap_1.quad[l1 + *iq * 9 - 10];
    qf[l - 1] = 0.f;
    bf[l - 1] = 0.f;
    for (k = 1; k <= 3; ++k) {
        t1 = con[k * 3 - 1];
        for (j = 1; j <= 2; ++j) {
/* L245: */
        t1 += _BLNK__1.v2[j - 1] * con[j + k * 3 - 4];
        }
        qf[l - 1] += t1 * _BLNK__1.v2[k - 1];
/* L250: */
        bf[l - 1] += t1 * _BLNK__1.v3[k - 1];
    }
    if ((r__1 = qf[l - 1]) < 0.f) {
        goto L260;
    } else if (r__1 == 0) {
        goto L255;
    } else {
        goto L265;
    }
L255:
    t2 += -.8f;
    goto L265;
L260:
    t2 += -1.f;
L265:
    ;
    }
    qf[4] = qf[0];
/*     ***** CHECK FOR 4 POINTS OF QUADRANGLE INSIDE OR ON ELLIPSE ***** */
    if (t2 >= 0.f) {
    goto L275;
    } else {
    goto L270;
    }
L270:
    itype = -1;
    goto L330;
/*     ***** CHECK FOR 1 TO 3 POINTS OF QUADRANGLE INSIDE THE ELLIPSE *** */
L275:
    if (t2 - 2.2f >= 0.f) {
    goto L285;
    } else {
    goto L280;
    }
L280:
    itype = 0;
    if ((i__1 = na2 - *ia) < 0) {
    goto L340;
    } else if (i__1 == 0) {
    goto L375;
    } else {
    goto L335;
    }
/*     ***** CHECK FOR QUADRANGLE-ELLIPSE INTERSECTION ***** */
L285:
    for (k = 1; k <= 4; ++k) {
/*     ***** EVALUATE DISCRIMINANT ***** */
/* Computing 2nd power */
    r__1 = bf[k - 1];
    t1 = r__1 * r__1 - qf[k - 1] * qf[k];
    if (t1 <= 0.f) {
        goto L305;
    } else {
        goto L290;
    }
L290:
    t1 = sqrt(t1);
/*     ***** IS INTERSECTION WITHIN BOUNDS OF QUADRANGLE ***** */
    t3 = qf[k - 1] - bf[k - 1];
    t4 = t3 + qf[k] - bf[k - 1];
    if (dabs(t4) - 1e-5f <= 0.f) {
        goto L305;
    } else {
        goto L295;
    }
L295:
    t5 = (t3 - t1) / t4;
    if (t5 < 0.f) {
        goto L305;
    } else if (t5 == 0) {
        goto L280;
    } else {
        goto L300;
    }
L300:
    if (1.f - t5 <= 0.f) {
        goto L305;
    } else {
        goto L280;
    }
L305:
    ;
    }
/*     ***** NO VALID INTERSECTION FOUND ***** */
/*     ***** CHECK FOR CENTER OF ELLIPSE WITHIN THE QUADRANGLE **** */
    t3 = 3.f;
    for (k = 1; k <= 4; ++k) {
    t1 = qua[k * 3 - 1];
    for (j = 1; j <= 2; ++j) {
/* L310: */
        t1 += _BLNK__1.v1[j - 1] * qua[j + k * 3 - 4];
    }
    if (t1 >= 0.f) {
        goto L320;
    } else {
        goto L315;
    }
L315:
    t3 += -1.f;
L320:
    ;
    }
    if (t3 >= 0.f) {
    goto L370;
    } else {
    goto L325;
    }
L325:
    itype = 1;
/*     ***** CHECK OVER/UNDER AMBIGUITY ***** */
L330:
    if (na2 - *ia <= 0) {
    goto L375;
    } else {
    goto L335;
    }
L335:
    if (*ia - na1 <= 0) {
    goto L375;
    } else {
    goto L340;
    }
L340:
    difv_(&_BLNK__1.atoms[na2 * 3 - 3], &_BLNK__1.atoms[na1 * 3 - 3], 
        _BLNK__1.v2);
    difv_(&_BLNK__1.atoms[*ia * 3 - 3], &_BLNK__1.atoms[na1 * 3 - 3], 
        _BLNK__1.v3);
    unity_(_BLNK__1.v2, _BLNK__1.v2, &c__1);
    unity_(_BLNK__1.v3, _BLNK__1.v3, &c__1);
    norm_(_BLNK__1.v2, _BLNK__1.v3, _BLNK__1.v4, &c__1);
    if (vv_(_BLNK__1.v4, _BLNK__1.v4) - 1e-5f <= 0.f) {
    goto L345;
    } else {
    goto L350;
    }
/*     ***** CENTER OF ATOM IQ IS ON THE BOND LINE ***** */
L345:
    if (*ity <= 0) {
    goto L370;
    } else {
    goto L385;
    }
/*     ***** CENTER OF ATOM IQ IS NOT ON THE BOND LINE ***** */
L350:
    norm_(_BLNK__1.v4, _BLNK__1.v2, _BLNK__1.v5, &c__1);
    t1 = -_BLNK__1.v5[2];
    if (_BLNK__1.view <= 0.f) {
    goto L365;
    } else {
    goto L355;
    }
L355:
    t1 = _BLNK__1.v5[2] * (_BLNK__1.atoms[*ia * 3 - 1] - _BLNK__1.view);
    for (j = 1; j <= 2; ++j) {
/* L360: */
    t1 += _BLNK__1.v5[j - 1] * _BLNK__1.atoms[j + *ia * 3 - 4];
    }
L365:
    if (t1 * (real) (*ity) <= 0.f) {
    goto L375;
    } else {
    goto L370;
    }
/*     ***** NO INTERFERENCE FOUND ***** */
L370:
    *icq = 0;
    goto L430;
/*     ***** ITYPE=1 ENCLOSED ELLIPSE / ITYPE=-1 ENCLOSED QUADRANGLE **** */
L375:
    if (itype * *ity >= 0) {
    goto L385;
    } else {
    goto L380;
    }
/*     ***** HIDDEN ATOM OR HIDDEN BOND ***** */
L380:
    *icq = -1;
    goto L430;
L385:
    *icq = 1;
    if (*ity >= 0) {
    goto L390;
    } else {
    goto L410;
    }
/*     ***** STORE INTERFERING ELLIPSE ***** */
L390:
    if (olap_1.ncover - 20 >= 0) {
    goto L395;
    } else {
    goto L400;
    }
L395:
    _BLNK__1.ng = 17;
    erpnt_(&_BLNK__1.atomid[*ia - 1], &c__800);
    --olap_1.ncover;
L400:
    ++olap_1.ncover;
    ij = 1;
    for (i__ = 1; i__ <= 3; ++i__) {
    for (j = i__; j <= 3; ++j) {
        olap_1.cover[ij + olap_1.ncover * 6 - 7] = con[i__ + j * 3 - 4];
/* L405: */
        ++ij;
    }
    }
    olap_1.kc[olap_1.ncover - 1] = *ia;
    goto L430;
/*     ***** STORE INTERFERING QUADRANGLE ***** */
L410:
    if (olap_1.nqover - 30 >= 0) {
    goto L415;
    } else {
    goto L420;
    }
L415:
    _BLNK__1.ng = 18;
    tidd = tid;
    erpnt_(&tidd, &c__700);
    --olap_1.nqover;
L420:
    ++olap_1.nqover;
    for (k = 1; k <= 4; ++k) {
    for (j = 1; j <= 3; ++j) {
/* L425: */
        olap_1.qover[j + (k + (olap_1.nqover << 2)) * 3 - 16] = qua[j + k 
            * 3 - 4];
    }
    }
    olap_1.kq[olap_1.nqover - 1] = *iq;
L430:
    return 0;
} /* lapab_ */

/* Subroutine */ int lapcon_(real *con1, real *con, real *y, real *ovmr)
{
    /* System generated locals */
    real r__1;

    /* Local variables */
    static integer j, k;
    static real t1;

/*     ***** TRANSFORM CONIC TO PLOTTER HOMOGENEOUS COORDINATE SYSTEM *** */
/*     ***** CALLED BY SUBROUTINES LAP700 AND LAPAB ***** */
    /* Parameter adjustments */
    --y;
    con -= 4;
    --con1;

    /* Function Body */
    y[1] = (con1[1] + con1[2]) * .5f;
    y[2] = (con1[3] + con1[4]) * .5f;
    y[3] = 1.f;
    con[4] = con1[5];
    con[7] = con1[6];
    con[5] = con1[6];
    con[8] = con1[7];
    t1 = (con1[2] - con1[1] + con1[4] - con1[3]) * .25f;
/* Computing 2nd power */
    r__1 = (t1 - *ovmr) / t1;
    con[12] = -(r__1 * r__1);
    for (k = 1; k <= 2; ++k) {
    con[k + 9] = 0.f;
    for (j = 1; j <= 2; ++j) {
/* L200: */
        con[k + 9] -= y[j] * con[j + k * 3];
    }
    con[k * 3 + 3] = con[k + 9];
/* L205: */
    con[12] -= con[k * 3 + 3] * y[k];
    }
    return 0;
} /* lapcon_ */

/* Subroutine */ int lapdrw_(real *y, integer *npen, integer *ncq)
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1, r__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, l, m;
    static real z__[3];
    static integer j1;
    static real p0, p1, t1, t2, t3, t4, t5, t6, cb[20];
    static integer i12;
    static real cq[100] /* was [50][2] */;
    static integer iq, im;
    static real ql[240] /* was [4][30][2] */, yn[3], yo[3], seg[2];
    static integer kcq, npn, npo, npm3, nint;
    extern /* Subroutine */ int scribe_(real *, integer *);

/*     ***** SUBROUTINE ELIMINATES HIDDEN LINES AND DRAWS VISIBLE LINES * */
    /* Parameter adjustments */
    --y;

    /* Function Body */
    *ncq = olap_1.ncover + olap_1.nqover;
    if (*ncq <= 0) {
    goto L200;
    } else {
    goto L205;
    }
L200:
    return 0;
/*     ***** CHECK ALL OVERLAPPING ATOMS AND BONDS ***** */
L205:
    npm3 = *npen - 3;
    if (npm3 >= 0) {
    goto L230;
    } else {
    goto L210;
    }
/*     ***** SAVE INFORMATION FROM LAST POINT IF PEN IS DOWN ***** */
L210:
    yo[0] = yn[0];
    yo[1] = yn[1];
    yo[2] = 1.f;
    npo = npn;
    i__1 = *ncq;
    for (k = 1; k <= i__1; ++k) {
/* L215: */
    cq[k - 1] = cq[k + 49];
    }
    if (olap_1.nqover <= 0) {
    goto L230;
    } else {
    goto L220;
    }
L220:
    i__1 = olap_1.nqover;
    for (k = 1; k <= i__1; ++k) {
    for (j = 1; j <= 4; ++j) {
/* L225: */
        ql[j + (k + 30 << 2) - 125] = ql[j + (k + 60 << 2) - 125];
    }
    }
/*     ***** EVALUATE CONIC QUADRATIC FORMS AT NEW POINT YN ***** */
L230:
    yn[0] = y[1];
    yn[1] = y[2];
    yn[2] = 1.f;
    npn = *npen;
    if (olap_1.ncover <= 0) {
    goto L250;
    } else {
    goto L235;
    }
L235:
    i__1 = olap_1.ncover;
    for (k = 1; k <= i__1; ++k) {
    z__[0] = yn[0] * olap_1.cover[k * 6 - 6] + yn[1] * olap_1.cover[k * 6 
        - 5] + olap_1.cover[k * 6 - 4];
    z__[1] = yn[0] * olap_1.cover[k * 6 - 5] + yn[1] * olap_1.cover[k * 6 
        - 3] + olap_1.cover[k * 6 - 2];
    z__[2] = yn[0] * olap_1.cover[k * 6 - 4] + yn[1] * olap_1.cover[k * 6 
        - 2] + olap_1.cover[k * 6 - 1];
    cq[k + 49] = z__[0] * yn[0] + z__[1] * yn[1] + z__[2];
/*     ***** EVALUATE CONIC BILINEAR FORM IF PEN IS DOWN ***** */
    if (npm3 >= 0) {
        goto L245;
    } else {
        goto L240;
    }
L240:
    cb[k - 1] = z__[0] * yo[0] + z__[1] * yo[1] + z__[2];
L245:
    ;
    }
/*     ***** EVALUATE LINEAR FORMS AND SIGNATURE FOR QUADRANGLE ***** */
L250:
    if (olap_1.nqover <= 0) {
    goto L275;
    } else {
    goto L255;
    }
L255:
    kcq = olap_1.ncover;
    i__1 = olap_1.nqover;
    for (k = 1; k <= i__1; ++k) {
    t2 = 3.f;
    for (j = 1; j <= 4; ++j) {
        t1 = yn[0] * olap_1.qover[(j + (k << 2)) * 3 - 15] + yn[1] * 
            olap_1.qover[(j + (k << 2)) * 3 - 14] + olap_1.qover[(j + 
            (k << 2)) * 3 - 13];
        if (t1 >= 0.f) {
        goto L265;
        } else {
        goto L260;
        }
L260:
        t2 += -1.f;
L265:
        ql[j + (k + 60 << 2) - 125] = t1;
    }
    ++kcq;
/*     ***** T2=-1 INSIDE, =0 ACROSS ANY EDGE, =1 ACROSS ANY VERTEX ***** */
/* L270: */
    cq[kcq + 49] = t2;
    }
/*     ***** IF PEN IS UP, OMIT ALL SUBSEQUENT CHECKING ***** */
L275:
    if (npm3 >= 0) {
    goto L280;
    } else {
    goto L285;
    }
L280:
    npn = 3;
    scribe_(yn, &npn);
    return 0;
/*     ***** CHECK FOR HIDDEN SEGMENT ***** */
L285:
    i__1 = *ncq;
    for (k = 1; k <= i__1; ++k) {
    if (cq[k - 1] >= 0.f) {
        goto L295;
    } else {
        goto L290;
    }
L290:
    if (cq[k + 49] >= 0.f) {
        goto L295;
    } else {
        goto L280;
    }
L295:
    ;
    }
/*     ***** FIND ENTRY AND EXIT POINTS ON EACH CONIC ***** */
    nint = 0;
    if (olap_1.ncover <= 0) {
    goto L330;
    } else {
    goto L300;
    }
L300:
    i__1 = olap_1.ncover;
    for (k = 1; k <= i__1; ++k) {
/*     ***** EVALUATE DISCRIMINANT ***** */
/* Computing 2nd power */
    r__1 = cb[k - 1];
    t1 = r__1 * r__1 - cq[k - 1] * cq[k + 49];
    if (t1 <= 0.f) {
        goto L325;
    } else {
        goto L305;
    }
L305:
    t1 = sqrt(t1);
/*     ***** SOLVE QUADRATIC EQATION ***** */
    t2 = cq[k - 1] - cb[k - 1];
    t3 = t2 + cq[k + 49] - cb[k - 1];
    if (dabs(t3) - 1e-5f <= 0.f) {
        goto L325;
    } else {
        goto L310;
    }
L310:
    t4 = (t2 - t1) / t3;
    t5 = (t2 + t1) / t3;
/*     ***** VALID INTERSECTION IF T4.LT.1 AND T5.GT.0 ***** */
    if (t4 - 1.f >= 0.f) {
        goto L325;
    } else {
        goto L315;
    }
L315:
    if (t5 <= 0.f) {
        goto L325;
    } else {
        goto L320;
    }
/*     ***** SAVE VALID CONIC INTERSECTIONS ***** */
L320:
    ++nint;
    olap_1.segm[nint - 1] = t4;
    olap_1.segm[nint + 49] = t5;
L325:
    ;
    }
L330:
    if (olap_1.nqover <= 0) {
    goto L425;
    } else {
    goto L335;
    }
/*     ***** FIND ENTRY AND EXIT POINTS FOR EACH QUADRANGLE ***** */
L335:
    i__1 = olap_1.nqover;
    for (k = 1; k <= i__1; ++k) {
    i12 = 0;
    kcq = olap_1.ncover + k;
/*     ***** CHECK FOR SINGLE INSIDE POINT ***** */
    seg[0] = cq[kcq - 1];
    if (seg[0] >= 0.f) {
        goto L340;
    } else {
        goto L345;
    }
L340:
    seg[0] = 1.f - cq[kcq + 49];
    if (seg[0] - 1.f <= 0.f) {
        goto L350;
    } else {
        goto L345;
    }
/*     ***** INSIDE POINT FOUND, ONLY ONE INTERSECTION POSSIBLE ***** */
L345:
    i12 = 1;
/*     ***** FIND WHICH EDGES ARE CROSSED BY THE SEGMENT ***** */
L350:
    for (j = 1; j <= 4; ++j) {
        t1 = ql[j + (k + 30 << 2) - 125];
        t2 = ql[j + (k + 60 << 2) - 125];
        t3 = t1 - t2;
        if (t1 * t2 <= 0.f) {
        goto L355;
        } else {
        goto L410;
        }
/*     ***** CHECK FOR SEGMENT ON AN EDGE ***** */
L355:
        if (dabs(t3) - 1e-5f <= 0.f) {
        goto L420;
        } else {
        goto L360;
        }
/*     ***** CALCULATE COORDINATES OF INTERSECTION ***** */
L360:
        t4 = (t1 * yn[0] - t2 * yo[0]) / t3;
        t5 = (t1 * yn[1] - t2 * yo[1]) / t3;
        j1 = j % 4 + 1 << 1;
        iq = olap_1.kq[k - 1];
/*     ***** IS INTERSECTION WITHIN LIMITS OF QUADRANGLE ***** */
        t6 = (t4 - olap_1.quad[(j << 1) - 1 + iq * 9 - 10]) * (
            olap_1.quad[j1 - 1 + iq * 9 - 10] - t4) + (t5 - 
            olap_1.quad[(j << 1) + iq * 9 - 10]) * (olap_1.quad[j1 + 
            iq * 9 - 10] - t5);
        if (dabs(t6) - 1e-4f <= 0.f) {
        goto L370;
        } else {
        goto L365;
        }
L365:
        if (t6 >= 0.f) {
        goto L370;
        } else {
        goto L410;
        }
/*     ***** CALCULATE FRACTION PARAMETER AND STORE IT ***** */
L370:
        t1 /= t3;
        if ((i__2 = i12 - 1) < 0) {
        goto L375;
        } else if (i__2 == 0) {
        goto L380;
        } else {
        goto L395;
        }
/*     ***** STORE FIRST INTERSECTION ***** */
L375:
        i12 = 1;
        goto L390;
/*     ***** STORE SECOND INTERSECTION **** */
L380:
        i12 = 2;
        if (t1 - seg[0] >= 0.f) {
        goto L405;
        } else {
        goto L385;
        }
L385:
        seg[1] = seg[0];
L390:
        seg[0] = t1;
        goto L410;
/*     ***** MORE THAN TWO INTERSECTIONS (I.E.,QUADRANGLE DIAGONAL) ***** */
L395:
        if ((r__1 = t1 - seg[0]) < 0.f) {
        goto L390;
        } else if (r__1 == 0) {
        goto L410;
        } else {
        goto L400;
        }
L400:
        if (t1 - seg[1] <= 0.f) {
        goto L410;
        } else {
        goto L405;
        }
L405:
        seg[1] = t1;
L410:
        ;
    }
    if (i12 - 1 <= 0) {
        goto L420;
    } else {
        goto L415;
    }
/*     ***** STORE FRACTION PARAMETERS ***** */
L415:
    ++nint;
    olap_1.segm[nint - 1] = seg[0];
    olap_1.segm[nint + 49] = seg[1];
L420:
    ;
    }
/*     ***** END OF ENTRY-AND-EXIT-POINT CALCULATIONS ***** */
L425:
    if ((i__1 = nint - 1) < 0) {
    goto L430;
    } else if (i__1 == 0) {
    goto L490;
    } else {
    goto L435;
    }
/*     ***** NO INTERFERENCE FOUND, DRAW ENTIRE SEGMENT ***** */
L430:
    scribe_(yn, &c__2);
    return 0;
/*     **** SORT SEGMENT INTERSECTION LIST ***** */
/*     ***** SORTING PROCEDURE BY SHELL,D.L. COMM. ACM 2,30-32 (1959) *** */
L435:
    m = nint;
L440:
    m /= 2;
    if (m <= 0) {
    goto L490;
    } else {
    goto L445;
    }
L445:
    k = nint - m;
    j = 1;
L450:
    i__ = j;
L455:
    im = i__ + m;
    if (olap_1.segm[i__ - 1] >= 0.f) {
    goto L470;
    } else {
    goto L460;
    }
L460:
    if (olap_1.segm[im - 1] <= 0.f) {
    goto L465;
    } else {
    goto L485;
    }
L465:
    if (olap_1.segm[i__ + 49] - olap_1.segm[im + 49] <= 0.f) {
    goto L485;
    } else {
    goto L475;
    }
L470:
    if (olap_1.segm[i__ - 1] - olap_1.segm[im - 1] <= 0.f) {
    goto L485;
    } else {
    goto L475;
    }
L475:
    for (l = 1; l <= 2; ++l) {
    t1 = olap_1.segm[i__ + l * 50 - 51];
    olap_1.segm[i__ + l * 50 - 51] = olap_1.segm[im + l * 50 - 51];
/* L480: */
    olap_1.segm[im + l * 50 - 51] = t1;
    }
    i__ -= m;
    if (i__ <= 0) {
    goto L485;
    } else {
    goto L455;
    }
L485:
    ++j;
    if (j - k <= 0) {
    goto L450;
    } else {
    goto L440;
    }
/*     ***** FIND STARTING POINT P0 AND END POINT P1 ***** */
L490:
    p0 = 0.f;
    k = 0;
L495:
    ++k;
    if (k - nint <= 0) {
    goto L500;
    } else {
    goto L515;
    }
L500:
    p1 = olap_1.segm[k - 1];
    if (p1 >= 0.f) {
    goto L505;
    } else {
    goto L510;
    }
L505:
    if (p1 - p0 <= 0.f) {
    goto L510;
    } else {
    goto L520;
    }
L510:
/* Computing MAX */
    r__1 = p0, r__2 = olap_1.segm[k + 49];
    p0 = dmax(r__1,r__2);
    if ((r__1 = p0 - 1.f) < 0.f) {
    goto L495;
    } else if (r__1 == 0) {
    goto L530;
    } else {
    goto L525;
    }
L515:
    p1 = 1.f;
/*     ***** DRAW SEGMENT FROM P0 TO P1 ***** */
L520:
    if (p0 <= 0.f) {
    goto L535;
    } else {
    goto L530;
    }
L525:
    p0 = 1.f;
L530:
    z__[0] = yo[0] * (1.f - p0) + yn[0] * p0;
    z__[1] = yo[1] * (1.f - p0) + yn[1] * p0;
    npn = 3;
    scribe_(z__, &npn);
    if (p0 - 1.f >= 0.f) {
    goto L540;
    } else {
    goto L535;
    }
L535:
    z__[0] = yo[0] * (1.f - p1) + yn[0] * p1;
    z__[1] = yo[1] * (1.f - p1) + yn[1] * p1;
    npn = 2;
    scribe_(z__, &npn);
    if (p1 - 1.f >= 0.f) {
    goto L540;
    } else {
    goto L510;
    }
L540:
    return 0;
} /* lapdrw_ */

/* Character */ VOID maksym_(char *ret_val, ftnlen ret_val_len, integer *k, 
    real *gp)
{
    /* Initialized data */

    static char fract[5*23] = "1/24 " "1/12 " "1/8  " "1/6  " "5/24 " "1/4  " 
        "7/24 " "1/3  " "3/8  " "5/12 " "11/24" "1/2  " "13/24" "7/12 " 
        "5/8  " "2/3  " "17/24" "3/4  " "19/24" "5/6  " "7/8  " "11/12" 
        "23/24";
    static char xyz[1*3] = "x" "y" "z";

    /* System generated locals */
    address a__1[3], a__2[2];
    integer i__1[3], i__2[2];

    /* Builtin functions */
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen), s_cat(char *,
         char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer i__, j, mm;
    static real tf;
    static integer iw, iff;
    extern integer iend_(char *, ftnlen);
    static char part[12*3];
    static real gpval, tfour;

/* *** returns character string representation of symmetry operator */
    /* Parameter adjustments */
    gp -= 16;

    /* Function Body */
    for (i__ = 1; i__ <= 3; ++i__) {
    s_copy(part + (i__ - 1) * 12, " ", (ftnlen)12, (ftnlen)1);
    iff = 0;
    for (j = 1; j <= 3; ++j) {
        if ((integer) gp[i__ + (j + (*k << 2)) * 3] != 0) {
        if ((integer) gp[i__ + (j + (*k << 2)) * 3] == -1) {
/* Writing concatenation */
            i__1[0] = iend_(part + (i__ - 1) * 12, (ftnlen)12), a__1[
                0] = part + (i__ - 1) * 12;
            i__1[1] = 1, a__1[1] = "-";
            i__1[2] = 1, a__1[2] = xyz + (j - 1);
            s_cat(part + (i__ - 1) * 12, a__1, i__1, &c__3, (ftnlen)
                12);
        }
        if ((integer) gp[i__ + (j + (*k << 2)) * 3] == 1 && iff == 0) 
            {
/* Writing concatenation */
            i__1[0] = iend_(part + (i__ - 1) * 12, (ftnlen)12), a__1[
                0] = part + (i__ - 1) * 12;
            i__1[1] = 1, a__1[1] = " ";
            i__1[2] = 1, a__1[2] = xyz + (j - 1);
            s_cat(part + (i__ - 1) * 12, a__1, i__1, &c__3, (ftnlen)
                12);
        }
        if ((integer) gp[i__ + (j + (*k << 2)) * 3] == 1 && iff == 1) 
            {
/* Writing concatenation */
            i__1[0] = iend_(part + (i__ - 1) * 12, (ftnlen)12), a__1[
                0] = part + (i__ - 1) * 12;
            i__1[1] = 1, a__1[1] = "+";
            i__1[2] = 1, a__1[2] = xyz + (j - 1);
            s_cat(part + (i__ - 1) * 12, a__1, i__1, &c__3, (ftnlen)
                12);
        }
        iff = 1;
        }
/* L300: */
    }
    gpval = gp[i__ + ((*k << 2) + 4) * 3];
    if (gpval > .01f || gpval < -.01f) {
        if (gpval < 0.f) {
/* Writing concatenation */
        i__2[0] = iend_(part + (i__ - 1) * 12, (ftnlen)12), a__2[0] = 
            part + (i__ - 1) * 12;
        i__2[1] = 1, a__2[1] = "-";
        s_cat(part + (i__ - 1) * 12, a__2, i__2, &c__2, (ftnlen)12);
        } else {
/* Writing concatenation */
        i__2[0] = iend_(part + (i__ - 1) * 12, (ftnlen)12), a__2[0] = 
            part + (i__ - 1) * 12;
        i__2[1] = 1, a__2[1] = "+";
        s_cat(part + (i__ - 1) * 12, a__2, i__2, &c__2, (ftnlen)12);
        }
        gpval = dabs(gpval);
        tfour = .041666666666666664f;
        for (mm = 1; mm <= 23; ++mm) {
        tf = (real) mm * tfour;
        if (gpval > tf - .01f && gpval < tf + .01f) {
            iw = mm;
        }
/* L301: */
        }
/* Writing concatenation */
        i__2[0] = iend_(part + (i__ - 1) * 12, (ftnlen)12), a__2[0] = 
            part + (i__ - 1) * 12;
        i__2[1] = 5, a__2[1] = fract + (iw - 1) * 5;
        s_cat(part + (i__ - 1) * 12, a__2, i__2, &c__2, (ftnlen)12);
    }
/* L200: */
    }
/* Writing concatenation */
    i__1[0] = 12, a__1[0] = part;
    i__1[1] = 12, a__1[1] = part + 12;
    i__1[2] = 12, a__1[2] = part + 24;
    s_cat(ret_val, a__1, i__1, &c__3, ret_val_len);
    return ;
} /* maksym_ */

/* Subroutine */ int mm_(real *x, real *y, real *z__)
{
    static real x11, x12, x13, x21, x22, x23, x31, x32, x33, y11, y12, y13, 
        y21, y22, y23, y31, y32, y33;

/*     MULTIPLY TWO MATRICES */
/*     Z(3,3)=X(3,3)*Y(3,3) */
    /* Parameter adjustments */
    z__ -= 4;
    y -= 4;
    x -= 4;

    /* Function Body */
    x11 = x[4];
    x12 = x[7];
    x13 = x[10];
    x21 = x[5];
    x22 = x[8];
    x23 = x[11];
    x31 = x[6];
    x32 = x[9];
    x33 = x[12];
    y11 = y[4];
    y12 = y[7];
    y13 = y[10];
    y21 = y[5];
    y22 = y[8];
    y23 = y[11];
    y31 = y[6];
    y32 = y[9];
    y33 = y[12];
    z__[4] = x11 * y11 + x12 * y21 + x13 * y31;
    z__[5] = x21 * y11 + x22 * y21 + x23 * y31;
    z__[6] = x31 * y11 + x32 * y21 + x33 * y31;
    z__[7] = x11 * y12 + x12 * y22 + x13 * y32;
    z__[8] = x21 * y12 + x22 * y22 + x23 * y32;
    z__[9] = x31 * y12 + x32 * y22 + x33 * y32;
    z__[10] = x11 * y13 + x12 * y23 + x13 * y33;
    z__[11] = x21 * y13 + x22 * y23 + x23 * y33;
    z__[12] = x31 * y13 + x32 * y23 + x33 * y33;
    return 0;
} /* mm_ */

/* Subroutine */ int mv_(real *x, real *y, real *z__)
{
    static real y1, y2, y3;

/*     MATRIX * VECTOR */
/*     Z(3)=X(3,3)*Y(3) */
    /* Parameter adjustments */
    --z__;
    --y;
    x -= 4;

    /* Function Body */
    y1 = y[1];
    y2 = y[2];
    y3 = y[3];
    z__[1] = x[4] * y1 + x[7] * y2 + x[10] * y3;
    z__[2] = x[5] * y1 + x[8] * y2 + x[11] * y3;
    z__[3] = x[6] * y1 + x[9] * y2 + x[12] * y3;
    return 0;
} /* mv_ */

/* Subroutine */ int norm_(real *x, real *y, real *z__, integer *itype)
{
    static integer i__, i1, i2;
    static real t1, z1[3];
    extern /* Subroutine */ int mv_(real *, real *, real *);

/*     ***** VECTOR PRODUCT  Z=X*Y ***** */
/*     ***** ITYPE .GT.0 FOR CARTESIAN,.LE.0 FOR TRICLINIC ***** */
    /* Parameter adjustments */
    --z__;
    --y;
    --x;

    /* Function Body */
    for (i__ = 1; i__ <= 3; ++i__) {
    i1 = (i__ + 3) % 3 + 1;
    i2 = (i__ + 1) % 3 + 1;
    t1 = x[i1] * y[i2] - x[i2] * y[i1];
    if (*itype <= 0) {
        goto L115;
    } else {
        goto L105;
    }
L105:
    z__[i__] = t1;
    goto L125;
L115:
    z1[i__ - 1] = t1;
L125:
    ;
    }
    if (*itype <= 0) {
    goto L135;
    } else {
    goto L300;
    }
L135:
    mv_(_BLNK__1.bb, z1, &z__[1]);
L300:
    return 0;
} /* norm_ */

/* Subroutine */ int numbur_(real *w, real *w2, real *hgt, real *dist, real *
    tht, integer *nd)
{
    /* Format strings */
    static char fmt_25[] = "(\002(F\002,i1,\002.\002,i1,\002)\002)";

    /* System generated locals */
    static char equiv_0[8];

    /* Builtin functions */
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
        ;

    /* Local variables */
    static integer nc;
    static real xd;
    static char ifmt[8];
#define itex (equiv_0)
#define itxt (equiv_0)
    extern /* Subroutine */ int simbol_(real *, real *, real *, char *, real *
        , integer *, ftnlen);

    /* Fortran I/O blocks */
    static icilist io___635 = { 0, ifmt, 0, fmt_25, 8, 1 };
    static icilist io___636 = { 0, itxt, 0, ifmt, 8, 1 };


/* -----CONVERT BOND DISTANCE FOR PLOTTING IN ORTEP */
/* -----COMPUTE NUMBER OF CHARACTERS FOR OUTPUT */
    /* Parameter adjustments */
    --w;

    /* Function Body */
    nc = *nd + 1;
    xd = *dist;
L10:
    if (xd < 1.f) {
    goto L20;
    }
    if (nc >= 9) {
    goto L30;
    }
    ++nc;
    xd /= 10.f;
    goto L10;
/* -----SET UP FORMAT STATEMENT */
L20:
    s_wsfi(&io___635);
    do_fio(&c__1, (char *)&nc, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*nd), (ftnlen)sizeof(integer));
    e_wsfi();
/* -----ENCODE DISTANCE AND PUT IT OUT */
    s_wsfi(&io___636);
    do_fio(&c__1, (char *)&(*dist), (ftnlen)sizeof(real));
    e_wsfi();
    simbol_(&w[1], w2, hgt, itex, tht, &nc, (ftnlen)1);
L30:
    return 0;
} /* numbur_ */

#undef itxt
#undef itex


/* Subroutine */ int paxes_(doublereal *dcode, integer *itype)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double d_mod(doublereal *, doublereal *);

    /* Local variables */
    static integer i__, j, k;
    static real x[3], t1;
    static integer ii;
    extern /* Subroutine */ int mm_(real *, real *, real *);
    static integer it, ks;
    static doublereal d100;
    extern /* Subroutine */ int tmm_(real *, real *, real *);
    static doublereal d100k;

/*     ***** ITYPE .LT.0 FOR COVARIANCE MATRIX IN Q ***** */
/*     ***** ITYPE .GT.0 FOR ELLIPSOID QUADRATIC FORM IN Q ***** */
/*     ***** XABSF(ITYPE)=1 BASED ON TRICLINIC COORDINATE SYSTEM ***** */
/*     ***** =2 OR 3 FOR WORKING OR REFERENCE CARTESIAN SYSTEMS ***** */
/*     ***** CONTRAVARIANT EIGENVECTORS FOR Q IN COLUMNS OF PAC ***** */
/*     ***** CHECK ATOM CODE ***** */
    d100 = 100.f;
    d100k = 1e5f;
    it = abs(*itype) - 1;
    ks = (integer) d_mod(dcode, &d100);
    if (_BLNK__1.nsym - ks >= 0) {
    goto L115;
    } else {
    goto L105;
    }
L105:
    _BLNK__1.ng = 4;
    goto L300;
L115:
    ii = (integer) (*dcode / d100k);
    if (_BLNK__1.natom - ii >= 0) {
    goto L130;
    } else {
    goto L125;
    }
L125:
    _BLNK__1.ng = 5;
    goto L300;
L130:
    if (ii <= 0) {
    goto L125;
    } else {
    goto L135;
    }
/*     ***** CRYSTALLOGRAPHIC SYMMETRY ROTATION ***** */
L135:
    tmm_(&parms_1.pa[(ii * 3 + 1) * 3 - 12], &_BLNK__1.fs[(ks * 3 + 1) * 3 - 
        12], _BLNK__1.pat);
    if ((i__1 = it - 1) < 0) {
    goto L160;
    } else if (i__1 == 0) {
    goto L145;
    } else {
    goto L155;
    }
/*     ***** TRANSFORM TO CARTESIAN SYSTEMS ***** */
L145:
    tmm_(_BLNK__1.pat, _BLNK__1.aawrk, _BLNK__1.pac);
    goto L175;
L155:
    tmm_(_BLNK__1.pat, _BLNK__1.aarev, _BLNK__1.pac);
    goto L175;
L160:
    if (*itype < 0) {
    goto L162;
    } else if (*itype == 0) {
    goto L155;
    } else {
    goto L170;
    }
/*     ***** TRANSFORM TO TRICLINIC SYSTEM ***** */
L162:
    for (j = 1; j <= 9; ++j) {
/* L165: */
    _BLNK__1.pac[j - 1] = _BLNK__1.pat[j - 1];
    }
    goto L175;
L170:
    mm_(_BLNK__1.aa, _BLNK__1.pat, _BLNK__1.pac);
/*     ***** FORM DIAGONAL MATRIX OR ITS INVERSE ***** */
L175:
    for (j = 1; j <= 3; ++j) {
    t1 = parms_1.ev[j + ii * 3 - 4];
    if (*itype <= 0) {
        goto L195;
    } else {
        goto L185;
    }
L185:
    x[j - 1] = 1.f / (t1 * t1);
    goto L205;
L195:
    x[j - 1] = t1 * t1;
L205:
    _BLNK__1.rms[j - 1] = t1;
    }
/*     ***** FORM QUADRATIC FORM ***** */
    for (i__ = 1; i__ <= 3; ++i__) {
    for (j = i__; j <= 3; ++j) {
        t1 = 0.f;
        for (k = 1; k <= 3; ++k) {
/* L225: */
        t1 += _BLNK__1.pac[i__ + k * 3 - 4] * _BLNK__1.pac[j + k * 3 
            - 4] * x[k - 1];
        }
        _BLNK__1.q[j + i__ * 3 - 4] = t1;
/* L245: */
        _BLNK__1.q[i__ + j * 3 - 4] = t1;
    }
    }
L300:
    return 0;
} /* paxes_ */

/* Subroutine */ int pltxy_(real *x, real *y)
{
    /* System generated locals */
    real r__1;

    /* Local variables */
    static real t1, t2, t4;

/*     ***** PLOT COORD. AND CLOSEST EDGE AFTER PROJECTION ***** */
    /* Parameter adjustments */
    --y;
    --x;

    /* Function Body */
    t4 = 1.f;
    t1 = 1.f;
    if (_BLNK__1.view <= 0.f) {
    goto L125;
    } else {
    goto L110;
    }
L110:
    t4 = _BLNK__1.view - x[3];
    if (t4 <= 0.f) {
    goto L115;
    } else {
    goto L120;
    }
L115:
    y[1] = -99.f;
    y[2] = -99.f;
    goto L130;
L120:
    t1 = _BLNK__1.view / t4;
L125:
    y[1] = x[1] * t1 + _BLNK__1.xo[0];
    y[2] = x[2] * t1 + _BLNK__1.xo[1];
    t1 = _BLNK__1.xlng[0] - (r__1 = y[1] * 2.f - _BLNK__1.xlng[0], dabs(r__1))
        ;
    t2 = _BLNK__1.xlng[1] - (r__1 = y[2] * 2.f - _BLNK__1.xlng[1], dabs(r__1))
        ;
    _BLNK__1.edge = dmin(t1,t2) * .5f;
    if (t4 - _BLNK__1.view * .5f >= 0.f) {
    goto L300;
    } else {
    goto L130;
    }
L130:
    _BLNK__1.edge = -99.f;
L300:
    return 0;
} /* pltxy_ */

/* Subroutine */ int plot_(real *x, real *y, integer *ipen)
{
    extern /* Subroutine */ int pensc_(real *, real *, integer *), penhp_(
        real *, real *, integer *), penps_(real *, real *, integer *);

    if (ns_1.ndraw == 0) {
    return 0;
    }
    if (ns_1.ndraw == 1) {
    pensc_(x, y, ipen);
    }
    if (ns_1.ndraw == 2) {
    penps_(x, y, ipen);
    }
    if (ns_1.ndraw == 3) {
    penhp_(x, y, ipen);
    }
    if (ns_1.ndraw == 9) {
    pensc_(x, y, ipen);
    }
    return 0;
} /* plot_ */

/* Subroutine */ int prelim_(void)
{
    /* Format strings */
    static char fmt_106[] = "(i1,f8.6,5f9.6)";
    static char fmt_107[] = "(a)";
    static char fmt_143[] = "(\0020\002,10x,\002DIRECT CELL PARAMETERS\002"
        "/\002 \002,15x,\002A\002,14x,\002B\002,14x,\002C\002,14x,\002ALP"
        "HA\002,10x,\002BETA\002,11x,\002GAMMA\002)";
    static char fmt_145[] = "(\002 \002,10x,f9.5,2f15.6,3f15.3/\002 \002,4"
        "8x,\002COSINE\002,f12.8,2f15.8)";
    static char fmt_147[] = "(\0020\002,10x,\002RECIPROCAL CELL PARAMETER"
        "S\002/\002 \002,15x,\002A*\002,13x,\002B*\002,13x,\002C*\002,13x,"
        "\002ALPHA*\002,9x,\002BETA*\002,10x,\002GAMMA*\002)";
    static char fmt_171[] = "(\0020\002,10x,\002SYMMETRY TRANSFORMATION"
        "S\002/\002 \002,14x,\002NO.\002)";
    static char fmt_173[] = "(i1,f14.10,3f3.0,2(f15.10,3f3.0))";
    static char fmt_176[] = "(\002 \002,13x,i2,5x,a)";
    static char fmt_177[] = "(\0021\002,10x,18a4)";
    static char fmt_1771[] = "(i1)";
    static char fmt_207[] = "(\0020 NO. ATOM \002,8x,\002X\002,10x,\002Y\002"
        ",10x,\002Z\002,13x,\002B11\002,8x,\002B22\002,8x,\002B33\002,8x"
        ",\002B12\002,8x,\002B13\002,8x,\002B23   TYPE\002)";
    static char fmt_209[] = "(\002 \002,i3,1x,a6,3f11.6,5x,6f11.6,f5.0)";
    static char fmt_210[] = "(\002 \002,i3,1x,a6,3f11.6,5x,2f11.6,4f11.0,f5."
        "0)";
    static char fmt_211[] = "(a6,3x,6f9.0)";
    static char fmt_213[] = "(i1,f8.0,5f9.0,7x,f2.0)";
    static char fmt_461[] = "(\0020NO. ATOM \002,8x,\002X\002,10x,\002Y\002,"
        "10x,\002Z\002,13x,\002RMSD 1 \002,4x,\002RMSD 2 \002,4x,\002RMSD"
        " 3 \002)";

    /* System generated locals */
    integer i__1;
    real r__1, r__2;
    doublereal d__1;
    char ch__1[36], ch__2[36];
    cllist cl__1;

    /* Builtin functions */
    integer s_rsfe(cilist *), do_fio(integer *, char *, ftnlen), e_rsfe(void),
         s_wsfe(cilist *), e_wsfe(void), s_rsfi(icilist *), e_rsfi(void);
    double cos(doublereal), sqrt(doublereal);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    double r_mod(real *, real *), sin(doublereal);
    integer f_clos(cllist *);
    double r_sign(real *, real *);

    /* Local variables */
    static real b[9];
    static integer i__, j, k, l, m, n, k9;
    static real t1, t2, t6, t3;
    static integer j22, jk;
    static doublereal td;
    extern /* Subroutine */ int mm_(real *, real *, real *);
    static integer is, kl, lm, iu;
    static real t11, t22;
    extern /* Subroutine */ int vm_(real *, real *, real *);
    static integer ng1, it1, isw;
    extern doublereal vmv_(real *, real *, real *);
    static real d100k;
    static char card[80];
    extern /* Subroutine */ int difv_(real *, real *, real *), axes_(real *, 
        real *, real *, integer *), atom_(doublereal *, real *);
    static real fsym[12]    /* was [3][4] */;
    static integer iflag;
    extern /* Subroutine */ int eigen_(real *, real *, real *), axeqb_(real *,
         real *, real *, integer *);
    static integer lines, matom, ipart;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *);
    static integer isymp;
    extern /* Subroutine */ int unity_(real *, real *, integer *), readin_(
        integer *, char *, shortint *, shortint *, real *, real *, real *,
         integer *, integer *, real *, real *, real *, real *, real *, 
        real *, real *, ftnlen), gtafil_(integer *);
    extern doublereal arccos_(real *);
    extern /* Subroutine */ int exitng_(integer *);
    extern /* Character */ VOID maksym_(char *, ftnlen, integer *, real *);
    extern /* Subroutine */ int tepsym_(char *, integer *, integer *, ftnlen);
    static char sympart[24*3];

    /* Fortran I/O blocks */
    static cilist io___651 = { 0, 0, 0, fmt_107, 0 };
    static cilist io___653 = { 0, 0, 0, fmt_107, 0 };
    static icilist io___654 = { 0, card, 0, fmt_106, 80, 1 };
    static cilist io___660 = { 0, 0, 0, fmt_143, 0 };
    static cilist io___661 = { 0, 0, 0, fmt_145, 0 };
    static cilist io___662 = { 0, 0, 0, fmt_147, 0 };
    static cilist io___663 = { 0, 0, 0, fmt_145, 0 };
    static cilist io___664 = { 0, 0, 0, fmt_171, 0 };
    static cilist io___666 = { 0, 0, 0, fmt_107, 0 };
    static cilist io___667 = { 0, 0, 0, fmt_107, 0 };
    static icilist io___668 = { 0, card, 0, fmt_173, 80, 1 };
    static icilist io___671 = { 0, card, 0, fmt_1771, 80, 1 };
    static cilist io___679 = { 0, 0, 0, fmt_177, 0 };
    static cilist io___680 = { 0, 0, 0, fmt_171, 0 };
    static cilist io___681 = { 0, 0, 0, fmt_176, 0 };
    static cilist io___684 = { 0, 0, 0, fmt_207, 0 };
    static cilist io___688 = { 0, 0, 0, fmt_107, 0 };
    static cilist io___689 = { 0, 0, 0, fmt_107, 0 };
    static icilist io___690 = { 0, card, 0, fmt_211, 80, 1 };
    static cilist io___691 = { 0, 0, 0, fmt_107, 0 };
    static cilist io___692 = { 0, 0, 0, fmt_107, 0 };
    static icilist io___693 = { 0, card, 0, fmt_213, 80, 1 };
    static cilist io___694 = { 0, 0, 0, fmt_177, 0 };
    static cilist io___695 = { 0, 0, 0, fmt_207, 0 };
    static cilist io___696 = { 0, 0, 0, fmt_210, 0 };
    static cilist io___697 = { 0, 0, 0, fmt_209, 0 };
    static cilist io___709 = { 0, 0, 0, fmt_461, 0 };
    static cilist io___710 = { 0, 0, 0, fmt_177, 0 };
    static cilist io___711 = { 0, 0, 0, fmt_461, 0 };
    static cilist io___712 = { 0, 0, 0, fmt_209, 0 };


/*     ***** DATA INPUT ROUTINE ***** */
/*     ***** CELL DIMENSIONS ***** */
    d100k = 1e5f;
/* L106: */
    io___651.ciunit = _BLNK__1.in;
    s_rsfe(&io___651);
    do_fio(&c__1, card, (ftnlen)80);
    e_rsfe();
    io___653.ciunit = queue_1.ned;
    s_wsfe(&io___653);
    do_fio(&c__1, card, (ftnlen)80);
    e_wsfe();
    s_rsfi(&io___654);
    do_fio(&c__1, (char *)&iflag, (ftnlen)sizeof(integer));
    for (i__ = 1; i__ <= 6; ++i__) {
    do_fio(&c__1, (char *)&_BLNK__1.a[i__ - 1], (ftnlen)sizeof(real));
    }
    e_rsfi();
    t1 = dabs(_BLNK__1.a[3]) - 1.f;
    for (j = 1; j <= 3; ++j) {
    if (t1 >= 0.f) {
        goto L110;
    } else {
        goto L115;
    }
/*     ***** CELL ANGLES IN DEGREES ***** */
L110:
    _BLNK__1.a[j + 5] = _BLNK__1.a[j + 2];
    _BLNK__1.a[j + 2] = cos(_BLNK__1.a[j + 5] * .01745329f);
    goto L120;
/*     ***** COSINES OF CELL ANGLES ***** */
L115:
    _BLNK__1.a[j + 5] = arccos_(&_BLNK__1.a[j + 2]);
/*     ***** STORE IDEMFACTOR MATRIX ***** */
L120:
    _BLNK__1.aid[j + j * 3 - 4] = 1.f;
    _BLNK__1.aid[j] = 0.f;
    _BLNK__1.aid[j + 4] = 0.f;
/*     ***** STORE METRIC TENSOR ***** */
/* L125: */
/* Computing 2nd power */
    r__1 = _BLNK__1.a[j - 1];
    _BLNK__1.aa[j + j * 3 - 4] = r__1 * r__1;
    }
    _BLNK__1.aa[3] = _BLNK__1.a[0] * _BLNK__1.a[1] * _BLNK__1.a[5];
    _BLNK__1.aa[6] = _BLNK__1.a[0] * _BLNK__1.a[2] * _BLNK__1.a[4];
    _BLNK__1.aa[7] = _BLNK__1.a[1] * _BLNK__1.a[2] * _BLNK__1.a[3];
    _BLNK__1.aa[1] = _BLNK__1.aa[3];
    _BLNK__1.aa[2] = _BLNK__1.aa[6];
    _BLNK__1.aa[5] = _BLNK__1.aa[7];
/*     ***** INVERT METRIC TENSOR ***** */
    axeqb_(_BLNK__1.aa, _BLNK__1.bb, _BLNK__1.aid, &c__3);
/*     ***** CALCULATE RECIPROCAL CELL PARAMETERS ***** */
    for (j = 1; j <= 3; ++j) {
/* L128: */
    b[j - 1] = sqrt(_BLNK__1.bb[j + j * 3 - 4]);
    }
    b[5] = _BLNK__1.bb[3] / (b[0] * b[1]);
    b[4] = _BLNK__1.bb[6] / (b[0] * b[2]);
    b[3] = _BLNK__1.bb[7] / (b[1] * b[2]);
    for (j = 1; j <= 3; ++j) {
/* L130: */
    b[j + 5] = arccos_(&b[j + 2]);
    }
/*     ***** WAS INPUT FOR REAL OR RECIPROCAL CELL ***** */
    if (_BLNK__1.a[0] - 1.f >= 0.f) {
    goto L150;
    } else {
    goto L135;
    }
L135:
    for (j = 1; j <= 9; ++j) {
    t1 = _BLNK__1.aa[j - 1];
    _BLNK__1.aa[j - 1] = _BLNK__1.bb[j - 1];
    _BLNK__1.bb[j - 1] = t1;
    t1 = _BLNK__1.a[j - 1];
    _BLNK__1.a[j - 1] = b[j - 1];
/* L140: */
    b[j - 1] = t1;
    }
/*     ***** WRITE OUT CELL PARAMETERS ***** */
/* L143: */
/* L145: */
/* L147: */
L150:
    if (_BLNK__1.nout >= 0) {
    io___660.ciunit = _BLNK__1.nout;
    s_wsfe(&io___660);
    e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
    io___661.ciunit = _BLNK__1.nout;
    s_wsfe(&io___661);
    for (i__ = 1; i__ <= 3; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.a[i__ - 1], (ftnlen)sizeof(real));
    }
    for (i__ = 7; i__ <= 9; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.a[i__ - 1], (ftnlen)sizeof(real));
    }
    for (i__ = 4; i__ <= 6; ++i__) {
        do_fio(&c__1, (char *)&_BLNK__1.a[i__ - 1], (ftnlen)sizeof(real));
    }
    e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
    io___662.ciunit = _BLNK__1.nout;
    s_wsfe(&io___662);
    e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
    io___663.ciunit = _BLNK__1.nout;
    s_wsfe(&io___663);
    for (i__ = 1; i__ <= 3; ++i__) {
        do_fio(&c__1, (char *)&b[i__ - 1], (ftnlen)sizeof(real));
    }
    for (i__ = 7; i__ <= 9; ++i__) {
        do_fio(&c__1, (char *)&b[i__ - 1], (ftnlen)sizeof(real));
    }
    for (i__ = 4; i__ <= 6; ++i__) {
        do_fio(&c__1, (char *)&b[i__ - 1], (ftnlen)sizeof(real));
    }
    e_wsfe();
    }
/*     ***** STORE STANDARD VECTORS ***** */
    axes_(_BLNK__1.aid, &_BLNK__1.aid[3], _BLNK__1.refv, &c__0);
    mm_(_BLNK__1.aa, _BLNK__1.refv, _BLNK__1.aarev);
    for (i__ = 1; i__ <= 3; ++i__) {
    for (j = 1; j <= 3; ++j) {
        _BLNK__1.aawrk[j + i__ * 3 - 4] = _BLNK__1.aarev[j + i__ * 3 - 4];
        _BLNK__1.q[j + i__ * 3 - 4] = _BLNK__1.refv[i__ + j * 3 - 4];
/* L160: */
        _BLNK__1.wrkv[j + i__ * 3 - 4] = _BLNK__1.refv[j + i__ * 3 - 4];
    }
    }
/*     ***** READ AND WRITE SYMMETRY TRANSFORMATIONS ***** */
/* L171: */
/* 171 FORMAT(1H010X,24HSYMMETRY TRANSFORMATIONS/1H 14X,3HNO.12X,13HTRANS */
/*    1FORMED X18X,13HTRANSFORMED Y18X,13HTRANSFORMED Z) */
/* L173: */
/* L175: */
/* L176: */
/* L177: */
    if (_BLNK__1.nout >= 0) {
    io___664.ciunit = _BLNK__1.nout;
    s_wsfe(&io___664);
    e_wsfe();
    }
    lines = 14;
    for (i__ = 1; i__ <= 96; ++i__) {
    lines = (lines + 1) % 56;
    io___666.ciunit = _BLNK__1.in;
    s_rsfe(&io___666);
    do_fio(&c__1, card, (ftnlen)80);
    e_rsfe();
    io___667.ciunit = queue_1.ned;
    s_wsfe(&io___667);
    do_fio(&c__1, card, (ftnlen)80);
    e_wsfe();
    if (iflag == 0) {
        s_rsfi(&io___668);
        do_fio(&c__1, (char *)&is, (ftnlen)sizeof(integer));
        for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.ts[j + i__ * 3 - 4], (ftnlen)
            sizeof(real));
        for (k = 1; k <= 3; ++k) {
            do_fio(&c__1, (char *)&_BLNK__1.fs[k + (j + i__ * 3) * 3 
                - 13], (ftnlen)sizeof(real));
        }
        }
        e_rsfi();
    }
    if (iflag == 1) {
        s_rsfi(&io___671);
        do_fio(&c__1, (char *)&is, (ftnlen)sizeof(integer));
        e_rsfi();
        ipart = 1;
        for (jk = 1; jk <= 3; ++jk) {
        for (kl = 1; kl <= 24; ++kl) {
/* L1772: */
            *(unsigned char *)&sympart[(jk - 1) * 24 + (kl - 1)] = 
                ' ';
        }
        }
        jk = 2;
L1773:
        if (*(unsigned char *)&card[jk - 1] == ' ') {
        goto L1776;
        }
        lm = 1;
        for (kl = jk; kl <= 80; ++kl) {
        if (*(unsigned char *)&card[kl - 1] == ' ' || *(unsigned char 
            *)&card[kl - 1] == ',') {
            jk = kl;
            goto L1775;
        }
        *(unsigned char *)&sympart[(ipart - 1) * 24 + (lm - 1)] = *(
            unsigned char *)&card[kl - 1];
        ++lm;
/* L1774: */
        }
L1775:
        ++ipart;
L1776:
        ++jk;
        if (jk < 80) {
        goto L1773;
        }
        for (isymp = 1; isymp <= 3; ++isymp) {
        tepsym_(sympart + (isymp - 1) * 24, &i__, &isymp, (ftnlen)24);
/* L1777: */
        }
    }
    for (j = 1; j <= 3; ++j) {
        fsym[j + 8] = _BLNK__1.ts[j + i__ * 3 - 4];
        for (k = 1; k <= 3; ++k) {
/* L178: */
        fsym[j + k * 3 - 4] = _BLNK__1.fs[k + (j + i__ * 3) * 3 - 13];
        }
    }
    if (lines != 0) {
        goto L185;
    } else {
        goto L180;
    }
L180:
    if (_BLNK__1.nout >= 0) {
        io___679.ciunit = _BLNK__1.nout;
        s_wsfe(&io___679);
        for (j = 1; j <= 18; ++j) {
        do_fio(&c__1, _BLNK__1.title + (j - 1 << 2), (ftnlen)4);
        }
        e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
        io___680.ciunit = _BLNK__1.nout;
        s_wsfe(&io___680);
        e_wsfe();
    }
/* *** ORTEP II symmetry output */
/* 185 IF (NOUT.GE.0) */
/*    &WRITE (NOUT,175)I,(TS(J,I),(FS(K,J,I),K=1,3),J=1,3) */
L185:
    if (_BLNK__1.nout >= 0) {
        io___681.ciunit = _BLNK__1.nout;
        s_wsfe(&io___681);
        do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
        maksym_(ch__2, (ftnlen)36, &c__1, fsym);
        s_copy(ch__1, ch__2, (ftnlen)36, (ftnlen)36);
        do_fio(&c__1, ch__1, (ftnlen)36);
        e_wsfe();
    }
/*     ***** NON-CRYSTALLOGRAPHIC HELIX-SYMMETRY INPUT ***** */
    if (_BLNK__1.fs[(i__ * 3 + 3) * 3 - 10] - 5.f >= 0.f) {
        goto L186;
    } else {
        goto L188;
    }
L186:
    t1 = _BLNK__1.fs[(i__ * 3 + 3) * 3 - 12] / _BLNK__1.fs[(i__ * 3 + 3) *
         3 - 10];
    _BLNK__1.ts[i__ * 3 - 1] += t1;
    r__1 = t1 * _BLNK__1.fs[(i__ * 3 + 3) * 3 - 11];
    t1 = r_mod(&r__1, &c_b235) * 6.28318531f;
    t2 = cos(t1);
    t1 = sin(t1);
    for (j = 1; j <= 9; ++j) {
/* L187: */
        _BLNK__1.vt[j - 1] = _BLNK__1.aid[j - 1];
    }
    _BLNK__1.vt[0] = t2;
    _BLNK__1.vt[4] = t2;
    _BLNK__1.vt[1] = -t1;
    _BLNK__1.vt[3] = t1;
    mm_(_BLNK__1.vt, _BLNK__1.q, _BLNK__1.pac);
    mm_(_BLNK__1.aarev, _BLNK__1.pac, &_BLNK__1.fs[(i__ * 3 + 1) * 3 - 12]
        );
L188:
    if (is != 0) {
        goto L195;
    } else {
        goto L190;
    }
L190:
    ;
    }
    _BLNK__1.ng = 1;
    erpnt_(&c_b113, &c__0);
    i__ = 96;
L195:
    _BLNK__1.nsym = i__;
/* L196: */
    isw = is;
    _BLNK__1.natom = 0;
/*     ***** POSITIONAL AND THERMAL PARAMETERS ***** */
/* L207: */
/* L209: */
/* L210: */
/* L211: */
/* L213: */
    lines += 2;
    if (lines - 56 >= 0) {
    goto L215;
    } else {
    goto L220;
    }
L215:
    lines = -1;
    goto L225;
L220:
    if (_BLNK__1.nout >= 0) {
    io___684.ciunit = _BLNK__1.nout;
    s_wsfe(&io___684);
    e_wsfe();
    }
    if (isw == 2) {
    iu = 18;
// :- gtafil_(&iu);
    if(crtep_nmode == 0) {
        gtafil_(&iu);
    } else {
        printf("External atoms file not supported.\n");
        exit(1);
    }
// --
    }
/*     ***** MAXIMUM NUMBER OF ATOMS EQUALS MAXATM ***** */
L225:
    matom = _BLNK__1.natom + 1;
    i__1 = parms_1.maxatm;
    for (i__ = matom; i__ <= i__1; ++i__) {
    lines = (lines + 1) % 56;
    if (isw == 1) {
        goto L226;
    }
/*     ***** CALL SPECIAL PURPOSE READIN ROUTINE ***** */
    readin_(&iu, parms_1.chem + (i__ - 1 << 3), &parms_1.ident[(i__ << 1) 
        - 2], &parms_1.ident[(i__ << 1) - 1], &parms_1.p[i__ * 3 - 3],
         &parms_1.p[i__ * 3 - 2], &parms_1.p[i__ * 3 - 1], &it1, &is, 
        &parms_1.pa[(i__ * 3 + 1) * 3 - 12], &parms_1.pa[(i__ * 3 + 1)
         * 3 - 11], &parms_1.pa[(i__ * 3 + 1) * 3 - 10], &parms_1.pa[(
        i__ * 3 + 2) * 3 - 12], &parms_1.pa[(i__ * 3 + 2) * 3 - 11], &
        parms_1.pa[(i__ * 3 + 2) * 3 - 10], &parms_1.pa[(i__ * 3 + 3) 
        * 3 - 12], (ftnlen)8);
    t1 = (real) it1;
    k = it1 + 1;
    goto L229;
L226:
    io___688.ciunit = _BLNK__1.in;
    s_rsfe(&io___688);
    do_fio(&c__1, card, (ftnlen)80);
    e_rsfe();
    io___689.ciunit = queue_1.ned;
    s_wsfe(&io___689);
    do_fio(&c__1, card, (ftnlen)80);
    e_wsfe();
    s_rsfi(&io___690);
    do_fio(&c__1, parms_1.chem + (i__ - 1 << 3), (ftnlen)8);
    do_fio(&c__1, (char *)&_BLNK__1.v1[0], (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&_BLNK__1.v1[1], (ftnlen)sizeof(real));
    for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&parms_1.p[j + i__ * 3 - 4], (ftnlen)sizeof(
            real));
    }
    do_fio(&c__1, (char *)&t1, (ftnlen)sizeof(real));
    e_rsfi();
    parms_1.ident[(i__ << 1) - 2] = _BLNK__1.v1[0];
    parms_1.ident[(i__ << 1) - 1] = _BLNK__1.v1[1];
    k = t1 + 1.f;
    if ((real) (k - 1) - t1 != 0.f) {
        goto L227;
    } else {
        goto L228;
    }
L227:
    k = 1;
L228:
    io___691.ciunit = _BLNK__1.in;
    s_rsfe(&io___691);
    do_fio(&c__1, card, (ftnlen)80);
    e_rsfe();
    io___692.ciunit = queue_1.ned;
    s_wsfe(&io___692);
    do_fio(&c__1, card, (ftnlen)80);
    e_wsfe();
    s_rsfi(&io___693);
    do_fio(&c__1, (char *)&is, (ftnlen)sizeof(integer));
    for (j = 1; j <= 7; ++j) {
        do_fio(&c__1, (char *)&parms_1.pa[j + (i__ * 3 + 1) * 3 - 13], (
            ftnlen)sizeof(real));
    }
    e_rsfi();
L229:
    if (lines <= 0) {
        goto L230;
    } else {
        goto L232;
    }
L230:
    if (_BLNK__1.nout >= 0) {
        io___694.ciunit = _BLNK__1.nout;
        s_wsfe(&io___694);
        for (j = 1; j <= 18; ++j) {
        do_fio(&c__1, _BLNK__1.title + (j - 1 << 2), (ftnlen)4);
        }
        e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
        io___695.ciunit = _BLNK__1.nout;
        s_wsfe(&io___695);
        e_wsfe();
    }
L232:
    if (parms_1.pa[(i__ * 3 + 1) * 3 - 10] - 1e4f >= 0.f) {
        goto L234;
    } else {
        goto L235;
    }
L234:
    if (_BLNK__1.nout >= 0) {
        io___696.ciunit = _BLNK__1.nout;
        s_wsfe(&io___696);
        do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
        do_fio(&c__1, parms_1.chem + (i__ - 1 << 3), (ftnlen)8);
        for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&parms_1.p[j + i__ * 3 - 4], (ftnlen)
            sizeof(real));
        }
        for (j = 1; j <= 7; ++j) {
        do_fio(&c__1, (char *)&parms_1.pa[j + (i__ * 3 + 1) * 3 - 13],
             (ftnlen)sizeof(real));
        }
        e_wsfe();
    }
    goto L238;
L235:
    if (_BLNK__1.nout >= 0) {
        io___697.ciunit = _BLNK__1.nout;
        s_wsfe(&io___697);
        do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
        do_fio(&c__1, parms_1.chem + (i__ - 1 << 3), (ftnlen)8);
        for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&parms_1.p[j + i__ * 3 - 4], (ftnlen)
            sizeof(real));
        }
        for (j = 1; j <= 7; ++j) {
        do_fio(&c__1, (char *)&parms_1.pa[j + (i__ * 3 + 1) * 3 - 13],
             (ftnlen)sizeof(real));
        }
        e_wsfe();
    }
L238:
    switch (k) {
        case 1:  goto L244;
        case 2:  goto L239;
        case 3:  goto L241;
        case 4:  goto L242;
        case 5:  goto L244;
    }
/*     ***** TYPE 1 POSITIONAL PARAMETERS (ANGSTROMS) ***** */
L239:
    for (j = 1; j <= 3; ++j) {
/* L240: */
        parms_1.p[j + i__ * 3 - 4] /= _BLNK__1.a[j - 1];
    }
    goto L244;
/*     ***** TYPE 2 POSITIONAL PARAMETERS, STANDARD CARTESIAN ***** */
L241:
    _BLNK__1.v1[0] = parms_1.p[i__ * 3 - 3];
    _BLNK__1.v1[1] = parms_1.p[i__ * 3 - 2];
    goto L243;
/*     ***** TYPE 3 POSITIONAL PARAMETERS ***** */
/*     ***** CYLINDRICAL COORDINATES REFERRED TO STANDARD CARTESIAN ***** */
L242:
    t2 = parms_1.p[i__ * 3 - 2] * .01745329252f;
    _BLNK__1.v1[0] += parms_1.p[i__ * 3 - 3] * cos(t2);
    _BLNK__1.v1[1] += parms_1.p[i__ * 3 - 3] * sin(t2);
L243:
    _BLNK__1.v1[2] = parms_1.p[i__ * 3 - 1];
    vm_(_BLNK__1.v1, _BLNK__1.q, &parms_1.p[i__ * 3 - 3]);
L244:
    if (is != 0) {
        goto L246;
    } else {
        goto L245;
    }
L245:
    ;
    }
    if (isw == 2) {
    cl__1.cerr = 0;
    cl__1.cunit = iu;
    cl__1.csta = 0;
    f_clos(&cl__1);
    }
    _BLNK__1.ng = 2;
    erpnt_(&c_b113, &c__0);
    i__ = parms_1.maxatm;
L246:
    _BLNK__1.natom = i__;
/*     ***** CONVERT TEMP FACTOR COEF TO STANDARD TYPE ZERO ***** */
    ng1 = 0;
    i__1 = _BLNK__1.natom;
    for (i__ = 1; i__ <= i__1; ++i__) {
    t1 = parms_1.pa[(i__ * 3 + 1) * 3 - 12];
/*     interim fix for IBM AIX */
    k9 = 7;
    k = parms_1.pa[k9 + (i__ * 3 + 1) * 3 - 13] + 1.f;
    if (t1 != 0.f) {
        goto L255;
    } else {
        goto L250;
    }
L250:
    t1 = .1f;
    goto L405;
L255:
    t6 = .0506605918f;
    switch (k) {
        case 1:  goto L270;
        case 2:  goto L260;
        case 3:  goto L265;
        case 4:  goto L265;
        case 5:  goto L270;
        case 6:  goto L260;
        case 7:  goto L400;
        case 8:  goto L405;
        case 9:  goto L270;
        case 10:  goto L260;
        case 11:  goto L270;
        case 12:  goto L450;
    }
/*     ***** TYPE 1 ***** */
L260:
    for (j = 4; j <= 6; ++j) {
/* L262: */
        parms_1.pa[j + (i__ * 3 + 1) * 3 - 13] *= .5f;
    }
    goto L270;
/*     ***** TYPES 2 AND 3 (BASE 2 SYSTEMS) ***** */
L265:
    t6 = .351152464f;
    if (k - 4 != 0) {
        goto L270;
    } else {
        goto L260;
    }
/*     ***** TYPES 0 THROUGH 5 ***** */
L270:
    if (parms_1.pa[(i__ * 3 + 1) * 3 - 11] <= 0.f) {
        goto L400;
    } else {
        goto L272;
    }
L272:
    for (j = 1; j <= 3; ++j) {
        for (l = j; l <= 3; ++l) {
        t2 = t6;
        if (k - 5 >= 0) {
            goto L275;
        } else {
            goto L285;
        }
L275:
        if (k - 6 <= 0) {
            goto L280;
        } else {
            goto L281;
        }
/*     ***** TYPES 4 AND 5 ***** */
L280:
        t2 = b[j - 1] * b[l - 1] * t2 * .25f;
        goto L285;
/*     ***** TYPES 8 AND 9 (U(I,J) TENSOR SYSTEMS) ***** */
L281:
        t2 = b[j - 1] * b[l - 1];
        if (k - 11 >= 0) {
            goto L282;
        } else {
            goto L285;
        }
/*     ***** TYPE 10, (CARTESIAN TENSOR SYSTEM) ***** */
L282:
        t2 = 1.f;
L285:
        if (j - l != 0) {
            goto L290;
        } else {
            goto L287;
        }
L287:
        _BLNK__1.vt[j + j * 3 - 4] = t2 * parms_1.pa[j + (i__ * 3 + 1)
             * 3 - 13];
        goto L300;
L290:
        m = j + l + 1;
        _BLNK__1.vt[j + l * 3 - 4] = t2 * parms_1.pa[m + (i__ * 3 + 1)
             * 3 - 13];
        _BLNK__1.vt[l + j * 3 - 4] = _BLNK__1.vt[j + l * 3 - 4];
L300:
        ;
        }
    }
/*     ***** FIND PRINCIPAL AXES ***** */
    if (k - 11 >= 0) {
        goto L305;
    } else {
        goto L310;
    }
L305:
    mm_(_BLNK__1.vt, _BLNK__1.q, _BLNK__1.pac);
    mm_(_BLNK__1.refv, _BLNK__1.pac, _BLNK__1.vt);
L310:
    mm_(_BLNK__1.vt, _BLNK__1.aa, _BLNK__1.da);
    eigen_(_BLNK__1.da, _BLNK__1.rms, _BLNK__1.pat);
/*     ***** ARE EIGENVALUES POSITIVE ***** */
    if (_BLNK__1.rms[0] <= 0.f) {
        goto L325;
    } else {
        goto L320;
    }
L320:
    if (_BLNK__1.ng < 0) {
        goto L350;
    } else if (_BLNK__1.ng == 0) {
        goto L360;
    } else {
        goto L330;
    }
L325:
    _BLNK__1.ng = 3;
L330:
    ng1 = 1;
    d__1 = (doublereal) i__ * d100k + 55501.;
    erpnt_(&d__1, &c__0);
/*     ***** 3 EQUAL EIGENVALUES, USE REFERENCE VECTORS ***** */
L340:
    r__2 = sqrt((r__1 = _BLNK__1.rms[0] + _BLNK__1.rms[1] + _BLNK__1.rms[
        2], dabs(r__1)) / 3.f);
    t3 = r_sign(&r__2, _BLNK__1.rms);
    for (j = 1; j <= 3; ++j) {
        for (k = 1; k <= 3; ++k) {
/* L342: */
        parms_1.pa[j + (k + i__ * 3) * 3 - 13] = _BLNK__1.refv[j + k *
             3 - 4];
        }
/* L345: */
        parms_1.ev[j + i__ * 3 - 4] = t3;
    }
    goto L450;
L350:
    if (_BLNK__1.ng + 6 <= 0) {
        goto L340;
    } else {
        goto L352;
    }
/*     ***** TWO EQUAL EIGENVALUES ***** */
L352:
    n = _BLNK__1.ng + 5;
    unity_(&_BLNK__1.pat[n * 3 - 3], _BLNK__1.v1, &c_n1);
    for (k = 1; k <= 3; ++k) {
        if ((r__1 = vmv_(_BLNK__1.v1, _BLNK__1.aa, &_BLNK__1.refv[k * 3 - 
            3]), dabs(r__1)) - .58f >= 0.f) {
        goto L354;
        } else {
        goto L356;
        }
L354:
        ;
    }
L356:
    mm_(_BLNK__1.aa, _BLNK__1.da, _BLNK__1.vt);
    axes_(_BLNK__1.v1, &_BLNK__1.refv[k * 3 - 3], _BLNK__1.da, &c_n1);
    for (k = 1; k <= 3; ++k) {
        l = (n + k - 2) % 3 + 1;
        for (j = 1; j <= 3; ++j) {
/* L358: */
        parms_1.pa[j + (l + i__ * 3) * 3 - 13] = _BLNK__1.da[j + k * 
            3 - 4];
        }
/* L359: */
        r__2 = sqrt((r__1 = vmv_(&_BLNK__1.da[k * 3 - 3], _BLNK__1.vt, &
            _BLNK__1.da[k * 3 - 3]), dabs(r__1)));
        parms_1.ev[l + i__ * 3 - 4] = r_sign(&r__2, &_BLNK__1.rms[l - 1]);
    }
    goto L450;
/*     ***** MAKE EIGENVECTORS 1 ANGSTROM LONG ***** */
L360:
    axes_(_BLNK__1.pat, &_BLNK__1.pat[6], &parms_1.pa[(i__ * 3 + 1) * 3 - 
        12], &c_n1);
/* L370: */
    _BLNK__1.ng = 0;
/*     ***** SQRT EIGENVALUE = RMS DISPLACEMENT ***** */
    for (j = 1; j <= 3; ++j) {
        t2 = _BLNK__1.rms[j - 1];
/* L375: */
        r__1 = sqrt((dabs(t2)));
        parms_1.ev[j + i__ * 3 - 4] = r_sign(&r__1, &t2);
    }
    goto L450;
/*     ***** TYPE 6 (ISOTROPIC TEMP FACTOR) ***** */
L400:
    t1 = sqrt(t1 * .01266515f);
/*     ***** TYPE 7 (DUMMY SPHERE OR ELLIPSOID OF REVOLUTION) ***** */
L405:
    if (parms_1.pa[(i__ * 3 + 1) * 3 - 11] <= 0.f) {
        goto L409;
    } else {
        goto L406;
    }
/*     ***** ELLIPSOID OF REVOLUTION FOR PASS OR PALE ***** */
L406:
    parms_1.ev[i__ * 3 - 3] = t1;
    parms_1.ev[i__ * 3 - 2] = parms_1.pa[(i__ * 3 + 1) * 3 - 11];
    parms_1.ev[i__ * 3 - 1] = parms_1.pa[(i__ * 3 + 1) * 3 - 11];
    goto L411;
/*     ***** SPHERE FOR PEAK OR PIT, OR A GENERAL SPHERE ATOM ***** */
L409:
    for (j = 1; j <= 3; ++j) {
/* L410: */
        parms_1.ev[j + i__ * 3 - 4] = t1;
    }
L411:
    if (parms_1.pa[(i__ * 3 + 1) * 3 - 10] <= 0.f) {
        goto L430;
    } else {
        goto L415;
    }
/*     ***** FIRST DEFINED VECTOR FOR SPHERE OR CRITICAL POINT ***** */
L415:
    for (j = 1; j <= 2; ++j) {
        td = parms_1.pa[j + 2 + (i__ * 3 + 1) * 3 - 13];
        atom_(&td, &_BLNK__1.vt[j * 3 - 3]);
        if (_BLNK__1.ng != 0) {
        goto L416;
        } else {
        goto L417;
        }
L416:
        erpnt_(&td, &c__0);
        goto L430;
L417:
        ;
    }
    difv_(&_BLNK__1.vt[3], _BLNK__1.vt, _BLNK__1.v1);
    t11 = sqrt(vmv_(_BLNK__1.v1, _BLNK__1.aa, _BLNK__1.v1));
    for (j = 1; j <= 3; ++j) {
/* L418: */
        _BLNK__1.v1[j - 1] /= t11;
    }
/*     ***** SECOND DEFINED VECTOR FOR SPHERE OR CRITICAL POINT ***** */
    for (j = 3; j <= 4; ++j) {
        td = parms_1.pa[j + 2 + (i__ * 3 + 1) * 3 - 13];
        if (td == 0.f) {
        goto L422;
        }
        atom_(&td, &_BLNK__1.vt[j * 3 - 3]);
        if (_BLNK__1.ng != 0) {
        goto L419;
        } else {
        goto L420;
        }
L419:
        erpnt_(&td, &c__0);
        goto L430;
L420:
        ;
    }
    difv_(&_BLNK__1.vt[9], &_BLNK__1.vt[6], _BLNK__1.v2);
    t11 = sqrt(vmv_(_BLNK__1.v2, _BLNK__1.aa, _BLNK__1.v2));
    for (j = 1; j <= 3; ++j) {
/* L421: */
        _BLNK__1.v2[j - 1] /= t11;
    }
/*     ***** CHECK FOR NEARLY PARALLEL UNIT VECTORS ***** */
    if ((r__1 = vmv_(_BLNK__1.v1, _BLNK__1.aa, _BLNK__1.v2), dabs(r__1)) <
         .9f) {
        goto L429;
    }
/*     ***** SUBSTITUTE BEST REFERENCE VECTOR ***** */
L422:
    t22 = 1.f;
    j22 = 0;
    for (j = 1; j <= 3; ++j) {
        t11 = (r__1 = vmv_(_BLNK__1.v1, _BLNK__1.aa, &_BLNK__1.refv[j * 3 
            - 3]), dabs(r__1));
        if (t22 <= t11) {
        goto L424;
        }
        t22 = t11;
        j22 = j;
L424:
        ;
    }
    for (j = 1; j <= 3; ++j) {
/* L425: */
        _BLNK__1.v2[j - 1] = _BLNK__1.refv[j + j22 * 3 - 4];
    }
L429:
    axes_(_BLNK__1.v1, _BLNK__1.v2, &parms_1.pa[(i__ * 3 + 1) * 3 - 12], &
        c_n1);
    goto L450;
/*     ***** REFERENCE VECTORS FOR SPHERE ***** */
L430:
    for (j = 1; j <= 9; ++j) {
/* L435: */
        parms_1.pa[j + (i__ * 3 + 1) * 3 - 13] = _BLNK__1.refv[j - 1];
    }
L450:
    _BLNK__1.ng = 0;
    }
/*     ***** WRITE OUT RMS VALUES ***** */
    lines += 2;
    if (lines - 56 <= 0) {
    goto L458;
    } else {
    goto L455;
    }
L455:
    lines = -1;
    goto L460;
L458:
    if (_BLNK__1.nout >= 0) {
    io___709.ciunit = _BLNK__1.nout;
    s_wsfe(&io___709);
    e_wsfe();
    }
L460:
    i__1 = _BLNK__1.natom;
    for (i__ = 1; i__ <= i__1; ++i__) {
    lines = (lines + 1) % 56;
    if (lines != 0) {
        goto L465;
    } else {
        goto L462;
    }
L462:
    if (_BLNK__1.nout >= 0) {
        io___710.ciunit = _BLNK__1.nout;
        s_wsfe(&io___710);
        for (j = 1; j <= 18; ++j) {
        do_fio(&c__1, _BLNK__1.title + (j - 1 << 2), (ftnlen)4);
        }
        e_wsfe();
    }
    if (_BLNK__1.nout >= 0) {
        io___711.ciunit = _BLNK__1.nout;
        s_wsfe(&io___711);
        e_wsfe();
    }
/* L463: */
L465:
    if (_BLNK__1.nout >= 0) {
        io___712.ciunit = _BLNK__1.nout;
        s_wsfe(&io___712);
        do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
        do_fio(&c__1, parms_1.chem + (i__ - 1 << 3), (ftnlen)8);
        for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&parms_1.p[j + i__ * 3 - 4], (ftnlen)
            sizeof(real));
        }
        for (j = 1; j <= 3; ++j) {
        do_fio(&c__1, (char *)&parms_1.ev[j + i__ * 3 - 4], (ftnlen)
            sizeof(real));
        }
        e_wsfe();
    }
    }
    if (ng1 <= 0) {
    goto L999;
    } else {
    goto L470;
    }
L470:
    exitng_(&_BLNK__1.ng);
L999:
    return 0;
} /* prelim_ */

/* Subroutine */ int prime_(void)
{
    /* Builtin functions */
    double pow_ri(real *, integer *), cos(doublereal), sqrt(doublereal);

    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int lap500_(integer *);

/*     ****GENERAL INITIALIZATION OF PRIME PARAMETERS**** */
    _BLNK__1.brdr = .5f;
/*     ****CALCULATE CONSTANTS**** */
    for (i__ = 1; i__ <= 5; ++i__) {
/* L2950: */
    _BLNK__1.cont[i__ - 1] = sqrt(1.f / ((cos(3.141593f / pow_ri(&c_b1817,
         &i__)) + 1.f) * 2.f));
    }
/*     DISP=.005 */
    _BLNK__1.disp = 0.f;
    _BLNK__1.fore = .866f;
    _BLNK__1.itilt = 0;
    _BLNK__1.latm = 0;
    parms_1.maxatm = 505;
    _BLNK__1.ncd = 0;
    _BLNK__1.ng = 0;
    for (j = 1; j <= 3; ++j) {
/* L3000: */
    _BLNK__1.orgn[j - 1] = 0.f;
    }
    _BLNK__1.res[0] = .75f;
    _BLNK__1.res[1] = _BLNK__1.res[0] * .5f;
    _BLNK__1.res[2] = _BLNK__1.res[1] * .25f;
    _BLNK__1.scal1 = 1.f;
    _BLNK__1.scal2 = 1.54f;
    _BLNK__1.scl = 1.54f;
    for (i__ = 1; i__ <= 3; ++i__) {
    _BLNK__1.symb[i__ + i__ * 3 - 4] = 1.f;
    _BLNK__1.symb[i__] = 0.f;
/* L3005: */
    _BLNK__1.symb[i__ + 4] = 0.f;
    }
    _BLNK__1.taper = .375f;
    _BLNK__1.theta = 0.f;
    _BLNK__1.view = 0.f;
    _BLNK__1.xlng[0] = 10.5f;
    _BLNK__1.xlng[1] = 8.f;
    _BLNK__1.xo[0] = 5.25f;
    _BLNK__1.xo[1] = 4.f;
    _BLNK__1.xo[2] = 0.f;
/*     ***** INITIATE OVERLAP ROUTINES ***** */
    lap500_(&c__0);
    return 0;
} /* prime_ */

/* Subroutine */ int proj_(real *d__, real *dp, real *x, real *xo, real *view,
     integer *i1, integer *i2, integer *i3)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__;
    static real t1, t2, t3, t4;

/*     ***** 3D CARTESIAN TO 2D PLOTTER COORDINATES ***** */
    /* Parameter adjustments */
    --xo;
    --x;
    dp -= 3;
    d__ -= 4;

    /* Function Body */
    t3 = *view - x[3];
    i__1 = *i2;
    i__2 = *i3;
    for (i__ = *i1; i__2 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__2) {
    t1 = d__[i__ * 3 + 1] + x[1];
    t2 = d__[i__ * 3 + 2] + x[2];
    if (*view <= 0.f) {
        goto L135;
    } else {
        goto L120;
    }
L120:
    t4 = *view / (t3 - d__[i__ * 3 + 3]);
    t1 *= t4;
    t2 *= t4;
L135:
    dp[(i__ << 1) + 1] = t1 + xo[1];
/* L145: */
    dp[(i__ << 1) + 2] = t2 + xo[2];
    }
    return 0;
} /* proj_ */

/* Subroutine */ int radial_(integer *nd)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);

    /* Local variables */
    static integer j, k, l, m, n;
    static real t1, t2;
    static integer kdel, kdel1, kdel2;

/*     ***** GENERATE ELLIPSE FROM TWO CONJUGATE VECTORS ***** */
/*     ***** ORTHONORMAL VECTORS PRODUCE 8-128 SPOKED CIRCLE ***** */
/*     ***** ND DENOTES NUMBER OF SUBDIVISIONS (1 TO 5) ***** */
    for (j = 1; j <= 3; ++j) {
    t1 = _BLNK__1.da[j - 1];
    _BLNK__1.d__[j - 1] = t1;
    _BLNK__1.d__[j + 383] = t1;
    _BLNK__1.d__[j + 191] = -t1;
    t1 = _BLNK__1.da[j + 2];
    _BLNK__1.d__[j + 95] = t1;
/* L115: */
    _BLNK__1.d__[j + 287] = -t1;
    }
    i__1 = *nd;
    for (k = 1; k <= i__1; ++k) {
    t1 = _BLNK__1.cont[k - 1];
    i__2 = 6 - k;
    kdel = pow_ii(&c__2, &i__2);
    kdel1 = kdel + 1;
    kdel2 = kdel / 2;
    i__2 = kdel;
    for (l = kdel1; i__2 < 0 ? l >= 65 : l <= 65; l += i__2) {
        j = l - kdel;
        m = l - kdel2;
        for (n = 1; n <= 3; ++n) {
        t2 = (_BLNK__1.d__[n + l * 3 - 4] + _BLNK__1.d__[n + j * 3 - 
            4]) * t1;
        _BLNK__1.d__[n + m * 3 - 4] = t2;
/* L135: */
        _BLNK__1.d__[n + (m + 64) * 3 - 4] = -t2;
        }
    }
    }
    return 0;
} /* radial_ */

/* Subroutine */ int readin_(integer *iu, char *chem, shortint *id1, shortint 
    *id2, real *x1, real *x2, real *x3, integer *it, integer *is, real *
    b1, real *b2, real *b3, real *b4, real *b5, real *b6, real *btype, 
    ftnlen chem_len)
{
    /* Format strings */
    static char fmt_201[] = "(a6,i5,1x,a4,1x,a3,1x,a1,i4,4x,3f8.0,2f6.0)";
    static char fmt_202[] = "(a6)";

    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2;
    alist al__1;

    /* Builtin functions */
    integer s_rsfe(cilist *), do_fio(integer *, char *, ftnlen), e_rsfe(void),
         s_cmp(char *, char *, ftnlen, ftnlen);
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);
    integer f_back(alist *);

    /* Local variables */
    static real tf, occ;
    static char rec[6], res[3], atom[4], chain[1];
    static integer iserno;

    /* Fortran I/O blocks */
    static cilist io___730 = { 0, 0, 0, fmt_201, 0 };
    static cilist io___738 = { 0, 0, 1, fmt_202, 0 };


    *b1 = .1f;
    *b2 = 0.f;
    *b3 = 0.f;
    *b4 = 0.f;
    *b5 = 0.f;
    *b6 = 0.f;
    *btype = 7.f;
    *id1 = 0;
    *id2 = 0;
    *it = 2;
/*     ***** read the pdb file ***** */
    io___730.ciunit = *iu;
    s_rsfe(&io___730);
    do_fio(&c__1, rec, (ftnlen)6);
    do_fio(&c__1, (char *)&iserno, (ftnlen)sizeof(integer));
    do_fio(&c__1, atom, (ftnlen)4);
    do_fio(&c__1, res, (ftnlen)3);
    do_fio(&c__1, chain, (ftnlen)1);
    do_fio(&c__1, (char *)&(*id2), (ftnlen)sizeof(shortint));
    do_fio(&c__1, (char *)&(*x1), (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&(*x2), (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&(*x3), (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&occ, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&tf, (ftnlen)sizeof(real));
    e_rsfe();
    *id1 = 9;
    if (s_cmp(atom, " N  ", (ftnlen)4, (ftnlen)4) == 0) {
    *id1 = 1;
    }
    if (s_cmp(atom, " CA ", (ftnlen)4, (ftnlen)4) == 0) {
    *id1 = 2;
    }
    if (s_cmp(atom, " C  ", (ftnlen)4, (ftnlen)4) == 0) {
    *id1 = 3;
    }
    if (s_cmp(atom, " O  ", (ftnlen)4, (ftnlen)4) == 0) {
    *id1 = 4;
    *b1 = .15f;
    }
/* Writing concatenation */
    i__1[0] = 3, a__1[0] = atom + 1;
    i__1[1] = 3, a__1[1] = res;
    s_cat(chem, a__1, i__1, &c__2, (ftnlen)8);
    *is = 0;
    io___738.ciunit = *iu;
    i__2 = s_rsfe(&io___738);
    if (i__2 != 0) {
    goto L203;
    }
    i__2 = do_fio(&c__1, rec, (ftnlen)6);
    if (i__2 != 0) {
    goto L203;
    }
    i__2 = e_rsfe();
    if (i__2 != 0) {
    goto L203;
    }
    al__1.aerr = 0;
    al__1.aunit = *iu;
    f_back(&al__1);
    return 0;
L203:
    *is = 1;
    return 0;
} /* readin_ */

/* Subroutine */ int recycle_(void)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_cmp(char *, char *, ftnlen, ftnlen);

    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int getpap_(void);

    if (ns_1.ndraw == 2 || ns_1.ndraw == 3) {
    getpap_();
    }
/* *** find 201(203) instruction */
    i__1 = queue_1.nque;
    for (i__ = 1; i__ <= i__1; ++i__) {
    if (s_cmp(queue_1.que + ((i__ - 1) * 73 + 6), "201", (ftnlen)3, (
        ftnlen)3) == 0 || s_cmp(queue_1.que + ((i__ - 1) * 73 + 6), 
        "203", (ftnlen)3, (ftnlen)3) == 0) {
        queue_1.next = i__;
        goto L570;
    }
/* L545: */
    }
/* *** ZERO ATOMS ARRAY AND RETURN TO EXECUTE NEXT INSTRUCTION *** */
L570:
    _BLNK__1.latm = 0;
    for (i__ = 1; i__ <= 2000; ++i__) {
    _BLNK__1.atomid[i__ - 1] = 0.f;
    for (j = 1; j <= 3; ++j) {
/* L580: */
        _BLNK__1.atoms[j + i__ * 3 - 4] = 0.f;
    }
    }
    return 0;
} /* recycle_ */

/* Subroutine */ int scribe_(real *y, integer *npen)
{
    static real yo[2];
    static integer npo;
    extern /* Subroutine */ int plot_(real *, real *, integer *);

/*     ***** SUBROUTINE WHICH LINKS WITH THE PLOTTER-SPECIFIC SUBROUTINES */
    /* Parameter adjustments */
    --y;

    /* Function Body */
    if (*npen - 3 >= 0) {
    goto L205;
    } else {
    goto L210;
    }
/*     ***** KEEP TRACK OF COORDINATES FOR LAST PEN-UP LOCATION ***** */
L205:
    yo[0] = y[1];
    yo[1] = y[2];
    npo = 0;
    return 0;
/*     ***** CALL MECHANICAL PLOTTER PLOTTING SUBROUTINE ***** */
L210:
    if (npo != 0) {
    goto L225;
    } else {
    goto L220;
    }
L220:
    plot_(yo, &yo[1], &c__3);
L225:
    plot_(&y[1], &y[2], &c__2);
    npo = 1;
    return 0;
} /* scribe_ */

/* Subroutine */ int searc_(void)
{
    /* Format strings */
    static char fmt_20[] = "(\0020\002,9x,\002        FROM ATOMS    TO ATOMS"
        "     WITH RADIUS OR, IF A BOX, WITH SEMIDIMENSIONS  \002/11x,"
        "\002CODE   (MIN  MAX)   (MIN  MAX)                \002,7x,\002"
        "A\002,8x,\002B\002,8x,\002C\002)";
    static char fmt_121[] = "(\0020\002,10x,2i3,i5,i4,i5,2i4,18x,3f9.3/\002"
        " \002)";
    static char fmt_124[] = "(\002 \002,15x,2i5,i8,i5,2f9.3)";
    static char fmt_421[] = "(\0020\002,10x,\002VECTORS FROM ATOM  (\002,i3"
        ",\002,\002,i5,\002)\002,6x,\002TO ATOMS\002,i4,\002 THROUGH\002,"
        "i4)";
    static char fmt_427[] = "(\002 \002,13x,2(a6,1x),39x,\002(\002,i3,\002"
        ",\002,i5,\002)\002,3f7.4,7x,\002D =\002,f6.3)";
    static char fmt_429[] = "(\002 \002,13x,2(a6,1x),2(\002  (\002,i3,\002"
        ",\002,i5,\002)\002,3f7.4,3x),4x,\002D =\002,f6.3)";
    static char fmt_441[] = "(\0020\002,10x,\002ANGLES AROUND ATOM\002,i5)";
    static char fmt_460[] = "(\002 \002,13x,3(a6,1x),7x,3(\002 (\002,i3"
        ",\002,\002,i5,\002)\002),12x,\002D =\002,f6.3,7x,\002A =\002,f6."
        "2)";

    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5, i__6, i__7, i__8, i__9, i__10, 
        i__11, i__12;
    real r__1;
    doublereal d__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void);
    double d_mod(doublereal *, doublereal *);
    integer do_fio(integer *, char *, ftnlen);
    double sqrt(doublereal), r_mod(real *, real *);

    /* Local variables */
    static real f;
    static integer i__, j, k, l, m, n;
    static real u[3], v[3], w[8]    /* was [2][4] */, x[4], y[3], z__[3], 
        f1;
    static integer i0, i1, i2, j1, j2, m1, n1, m2, m3, m5, m4, n2;
    static real s2[200], t1, t9;
    static integer n5, n3, n4, l5, l4, l3, l2;
    static real t3;
    static integer i02, ii, ij, ki;
    static doublereal td;
    static real dx[3];
    static integer ll, ml, nl, nn, lu, mu, nu;
    extern /* Subroutine */ int vm_(real *, real *, real *);
    static integer nw[6];
    extern /* Subroutine */ int mv_(real *, real *, real *);
    static real tt;
    extern doublereal vv_(real *, real *);
    static real ww[6]   /* was [2][3] */;
    static doublereal s1d[200], td1, td2, td3, td4, d10k;
    static real tem, dsq, dmx;
    static integer num;
    extern doublereal vmv_(real *, real *, real *);
    static doublereal d100k;
    static integer logc;
    extern /* Subroutine */ int difv_(real *, real *, real *);
    static real dmax__;
    extern /* Subroutine */ int atom_(doublereal *, real *);
    static integer kfun, last, norg, inum, list, itom, ntar;
    extern /* Subroutine */ int stor_(doublereal *);
    static integer itar1, itar2, kfun2, itom1, itom2, ifeat, latom;
    static doublereal dzmin, dzmax;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *);
    static doublereal dzsto;
    static integer listp1;
    static real syito2;
    extern doublereal arccos_(real *);
    static logical featur;
    static real syitom;

    /* Fortran I/O blocks */
    static cilist io___747 = { 0, 0, 0, fmt_20, 0 };
    static cilist io___762 = { 0, 0, 0, fmt_121, 0 };
    static cilist io___764 = { 0, 0, 0, fmt_124, 0 };
    static cilist io___824 = { 0, 0, 0, fmt_421, 0 };
    static cilist io___828 = { 0, 0, 0, fmt_429, 0 };
    static cilist io___829 = { 0, 0, 0, fmt_427, 0 };
    static cilist io___830 = { 0, 0, 0, fmt_441, 0 };
    static cilist io___837 = { 0, 0, 0, fmt_460, 0 };


    featur = FALSE_;
    ifeat = (integer) _BLNK__1.ain[5];
    if ((ifeat == 1 || ifeat == 2) && (_BLNK__1.nj2 % 10 == 5 || _BLNK__1.nj2 
        % 10 == 6)) {
    featur = TRUE_;
    }
/*     ***** OBTAIN PROBLEM PARAMETERS ***** */
    d10k = 1e4f;
    d100k = 1e5f;
    if (_BLNK__1.nout >= 0) {
    io___747.ciunit = _BLNK__1.nout;
    s_wsfe(&io___747);
    e_wsfe();
    }
    if (_BLNK__1.ain[0] - d10k <= 0.) {
    goto L100;
    } else {
    goto L101;
    }
L100:
    itom1 = (integer) _BLNK__1.ain[0];
    syitom = 55501.f;
    goto L103;
L101:
    itom1 = (integer) (_BLNK__1.ain[0] / d100k);
    syitom = d_mod(_BLNK__1.ain, &d100k);
/* L102: */
    if (abs(_BLNK__1.ain[1]) - d10k <= 0.) {
    goto L103;
    } else {
    goto L104;
    }
L103:
    itom2 = (integer) abs(_BLNK__1.ain[1]);
    syito2 = syitom;
    goto L105;
L104:
    itom2 = (integer) (abs(_BLNK__1.ain[1]) / d100k);
    d__1 = abs(_BLNK__1.ain[1]);
    syito2 = d_mod(&d__1, &d100k);
L105:
    itar1 = (integer) _BLNK__1.ain[2];
    if (itar1 <= 0) {
    goto L108;
    } else {
    goto L110;
    }
L108:
    itar1 = 1;
L110:
    itar2 = (integer) _BLNK__1.ain[3];
    dmax__ = _BLNK__1.ain[4];
    if (dmax__ <= 0.f) {
    goto L115;
    } else {
    goto L120;
    }
L115:
    dmax__ = 4.f;
    _BLNK__1.ain[4] = dmax__;
L120:
    dmx = dmax__ * dmax__;
    tem = .01f;
    kfun = _BLNK__1.nj * 100 + _BLNK__1.nj2 % 10;
    k = _BLNK__1.nj * 100 + _BLNK__1.nj2;
    i0 = syitom;
    i02 = syito2;
    latom = _BLNK__1.latm;
/* L121: */
    if (_BLNK__1.nout >= 0) {
    io___762.ciunit = _BLNK__1.nout;
    s_wsfe(&io___762);
    do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&itom1, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&i0, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&itom2, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&i02, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&itar1, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&itar2, (ftnlen)sizeof(integer));
    for (j = 5; j <= 7; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.ain[j - 1], (ftnlen)sizeof(
            doublereal));
    }
    e_wsfe();
    }
/* L124: */
    if (_BLNK__1.ncd <= 0) {
    goto L130;
    } else {
    goto L125;
    }
L125:
    if (_BLNK__1.nout >= 0) {
    io___764.ciunit = _BLNK__1.nout;
    s_wsfe(&io___764);
    i__1 = _BLNK__1.ncd;
    for (i__ = 1; i__ <= i__1; ++i__) {
        for (j = 1; j <= 4; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.kd[j + i__ * 5 - 6], (ftnlen)
            sizeof(integer));
        }
        for (j = 1; j <= 2; ++j) {
        do_fio(&c__1, (char *)&_BLNK__1.cd[j + (i__ << 3) - 9], (
            ftnlen)sizeof(real));
        }
    }
    e_wsfe();
    }
L130:
    for (j = 1; j <= 4; ++j) {
    w[(j << 1) - 2] = 99.f;
/* L135: */
    w[(j << 1) - 1] = -99.f;
    }
    i__1 = itar2;
    for (ki = itar1; ki <= i__1; ++ki) {
    inum = 0;
    i__ = ki;
L136:
    if (featur) {
        ++inum;
        i__ = inum;
        if (parms_1.ident[ifeat + (i__ << 1) - 3] != ki) {
        goto L154;
        }
    }
    td = (real) i__ * d100k;
    atom_(&td, x);
    if (_BLNK__1.ng != 0) {
        goto L140;
    } else {
        goto L145;
    }
L140:
    erpnt_(&td, &kfun);
    goto L600;
L145:
    x[3] = x[0] - x[1];
    for (j = 1; j <= 4; ++j) {
        tem = x[j - 1];
        if (w[(j << 1) - 1] - tem >= 0.f) {
        goto L150;
        } else {
        goto L148;
        }
L148:
        w[(j << 1) - 1] = tem;
L150:
        if (tem - w[(j << 1) - 2] >= 0.f) {
        goto L155;
        } else {
        goto L152;
        }
L152:
        w[(j << 1) - 2] = tem;
L155:
        ;
    }
L154:
    if (featur && inum < _BLNK__1.natom) {
        goto L136;
    }
/* L153: */
    }
    kfun2 = kfun % 10;
    switch (kfun2) {
    case 1:  goto L165;
    case 2:  goto L165;
    case 3:  goto L160;
    case 4:  goto L156;
    case 5:  goto L165;
    case 6:  goto L165;
    }
/*     ***** FIND PARALLELEPIPED WHICH ENCLOSES TRICLINIC BOX ***** */
L156:
    for (j = 1; j <= 3; ++j) {
/* L158: */
    dx[j - 1] = _BLNK__1.ain[j + 3];
    }
    goto L170;
/*     ***** FIND PARALLELEPIPED WHICH ENCLOSES RECTANGULAR BOX ***** */
L160:
    for (j = 1; j <= 3; ++j) {
    dx[j - 1] = 0.f;
    for (i__ = 1; i__ <= 3; ++i__) {
        t9 = _BLNK__1.ain[i__ + 3];
/* L162: */
        dx[j - 1] += (r__1 = _BLNK__1.refv[j + i__ * 3 - 4] * t9, dabs(
            r__1));
    }
    }
    goto L170;
/*     ***** FIND PARALLELEPIPED WHICH ENCLOSES DMAX SPHERE ***** */
L165:
    t1 = 1.f - _BLNK__1.a[3] * _BLNK__1.a[3] - _BLNK__1.a[4] * _BLNK__1.a[4] 
        - _BLNK__1.a[5] * _BLNK__1.a[5] + _BLNK__1.a[3] * 2.f * 
        _BLNK__1.a[4] * _BLNK__1.a[5];
    for (j = 1; j <= 3; ++j) {
/* L168: */
/* Computing 2nd power */
    r__1 = _BLNK__1.a[j + 2];
    dx[j - 1] = sqrt((1.f - r__1 * r__1) / t1) * dmax__ / _BLNK__1.a[j - 
        1];
    }
/*     ***** START SEARCH AROUND REFERENCE ATOMS ***** */
L170:
    list = 0;
    last = 0;
    m1 = itom1;
    n1 = itom2;
    if (kfun2 - 5 >= 0) {
    goto L172;
    } else {
    goto L186;
    }
/*     ***** CONVOLUTE AND REITERATIVE CONVOLUTE INSTRUCTIONS ***** */
L172:
    if (_BLNK__1.latm <= 0) {
    goto L174;
    } else {
    goto L176;
    }
/*     ***** FAULT, NO ENTRIES IN ATOMS LIST ***** */
L174:
    _BLNK__1.ng = 12;
    erpnt_(&c_b113, &kfun);
    goto L600;
/*     ***** CHECK FOR REFERENCE ATOMS IN ATOMS LIST ***** */
L176:
    if (_BLNK__1.latm - last <= 0) {
    goto L600;
    } else {
    goto L177;
    }
L177:
    list = last;
    last = _BLNK__1.latm;
L178:
    ++list;
    if (last - list >= 0) {
    goto L180;
    } else {
    goto L505;
    }
L180:
    td1 = _BLNK__1.atomid[list - 1];
    if (last - list <= 0 || _BLNK__1.ain[7] == 0.) {
    goto L184;
    }
/*     ***** FIND SMALLEST ATOM NUMBER IN REMAINDER OF ATOMS LIST ***** */
    listp1 = list + 1;
    i__1 = last;
    for (j = listp1; j <= i__1; ++j) {
    if (td1 <= _BLNK__1.atomid[j - 1]) {
        goto L182;
    }
    for (i__ = 1; i__ <= 3; ++i__) {
        t1 = _BLNK__1.atoms[i__ + j * 3 - 4];
        _BLNK__1.atoms[i__ + j * 3 - 4] = _BLNK__1.atoms[i__ + list * 3 - 
            4];
/* L181: */
        _BLNK__1.atoms[i__ + list * 3 - 4] = t1;
    }
    td1 = _BLNK__1.atomid[j - 1];
    _BLNK__1.atomid[j - 1] = _BLNK__1.atomid[list - 1];
    _BLNK__1.atomid[list - 1] = td1;
L182:
    ;
    }
L184:
    itom = (integer) (td1 / d100k);
    if (featur) {
    if (parms_1.ident[ifeat + (itom << 1) - 3] < itom1 || parms_1.ident[
        ifeat + (itom << 1) - 3] > itom2) {
        goto L178;
    }
    } else {
    if (itom < itom1 || itom > itom2) {
        goto L178;
    }
    }
    syitom = d_mod(&td1, &d100k);
    syito2 = syitom;
    m1 = itom;
    n1 = itom;
/*     ***** SET INITIAL RUN PARAMETERS ***** */
L186:
    m2 = r_mod(&syitom, &c_b1928);
    r__1 = syitom / 100.f;
    m5 = r_mod(&r__1, &c_b1321);
    m3 = m5 / 100;
    m4 = m5 / 10 % 10;
    m5 %= 10;
/*     ***** SET TERMINAL RUN PARAMETERS ***** */
    n2 = r_mod(&syito2, &c_b1928);
    r__1 = syito2 / 100.f;
    n5 = r_mod(&r__1, &c_b1321);
    n3 = n5 / 100;
    n4 = n5 / 10 % 10;
    n5 %= 10;
/*     ***** START SEARCH AROUND REFERENCE ATOMS ***** */
    i__1 = n5;
    for (l5 = m5; l5 <= i__1; ++l5) {
    i__2 = n4;
    for (l4 = m4; l4 <= i__2; ++l4) {
        i__3 = n3;
        for (l3 = m3; l3 <= i__3; ++l3) {
        i__4 = n2;
        for (l2 = m2; l2 <= i__4; ++l2) {
            i__5 = n1;
            for (itom = m1; itom <= i__5; ++itom) {
            td3 = (doublereal) itom * d100k + (doublereal) (l3 * 
                10000 + l4 * 1000 + l5 * 100 + l2);
            atom_(&td3, y);
            if (_BLNK__1.ng != 0) {
                goto L188;
            } else {
                goto L190;
            }
L188:
            erpnt_(&td3, &kfun);
            goto L500;
/*     ***** K=SYMMETRY EQUIVALENT POSITION ***** */
L190:
            num = 0;
            i__6 = _BLNK__1.nsym;
            for (k = 1; k <= i__6; ++k) {
/*     ***** SUBTRACT SYMMETRY TRANSLATION FROM REFERENCE ATOM ***** */
                for (j = 1; j <= 3; ++j) {
/* L192: */
                u[j - 1] = y[j - 1] - _BLNK__1.ts[j + k * 3 - 
                    4];
                }
/*     ***** DETERMINE LIMITING CELLS TO BE SEARCHED ***** */
/*     ***** FIRST,MOVE THE BOX THROUGH THE SYMMETRY OPERATION ***** */
                for (j = 1; j <= 3; ++j) {
                for (l = 1; l <= 2; ++l) {
                    ww[l + (j << 1) - 3] = 0.f;
                    for (i__ = 1; i__ <= 3; ++i__) {
                    tem = _BLNK__1.fs[i__ + (j + k * 3) * 
                        3 - 13];
                    if (tem < 0.f) {
                        goto L194;
                    } else if (tem == 0) {
                        goto L200;
                    } else {
                        goto L196;
                    }
L194:
                    n = l % 2 + 1;
                    goto L198;
L196:
                    n = l;
L198:
                    ww[l + (j << 1) - 3] += w[n + (i__ << 
                        1) - 3] * tem;
L200:
                    ;
                    }
                }
                }
/*     ***** CHECK FOR MIXED INDEX TRANSFORMATION ***** */
                for (j = 1; j <= 2; ++j) {
                tem = _BLNK__1.fs[(j + k * 3) * 3 - 12];
                if (tem + _BLNK__1.fs[(j + k * 3) * 3 - 11] !=
                     0.f) {
                    goto L215;
                } else {
                    goto L201;
                }
L201:
                if (tem < 0.f) {
                    goto L203;
                } else if (tem == 0) {
                    goto L215;
                } else {
                    goto L207;
                }
L203:
                ww[(j << 1) - 2] = w[7] * tem;
                ww[(j << 1) - 1] = w[6] * tem;
                goto L215;
L207:
                ww[(j << 1) - 2] = w[6] * tem;
                ww[(j << 1) - 1] = w[7] * tem;
L215:
                ;
                }
/*     ***** MOVE 4 CELLS AWAY THEN MOVE BACK UNTIL PARALLELEPIPED AROUND */
/*         REF ATOM AND BOX AROUND TRANSFORMED ASYM UNIT INTERSECT ***** */
                n = 0;
                for (j = 1; j <= 3; ++j) {
                for (i__ = 1; i__ <= 2; ++i__) {
                    ++n;
                    tt = (u[j - 1] - ww[i__ + (j << 1) - 3]) *
                         (real) ((i__ << 1) - 3) - dx[j - 
                        1];
                    tem = 5.f;
L221:
                    tem += -1.f;
                    if (tem + tt <= 0.f) {
                    goto L225;
                    } else {
                    goto L221;
                    }
L225:
                    nw[n - 1] = tem * (real) ((i__ << 1) - 3) 
                        + 5.f;
                }
/*     ***** IF NO POSSIBILITY OF A HIT, GO TO NEXT SYMMETRY OPER ***** */
                if (nw[n - 1] - nw[n - 2] >= 0) {
                    goto L235;
                } else {
                    goto L400;
                }
L235:
                ;
                }
                ll = nw[0];
                lu = nw[1];
                ml = nw[2];
                mu = nw[3];
                nl = nw[4];
                nu = nw[5];
/*     ***** L CELL TRANSLATIONS IN X ***** */
                i__7 = lu;
                for (l = ll; l <= i__7; ++l) {
                v[0] = u[0] + (real) (l - 5);
/*     ***** M CELL TRANSLATIONS IN Y ***** */
                i__8 = mu;
                for (m = ml; m <= i__8; ++m) {
                    v[1] = u[1] + (real) (m - 5);
/*     ***** N CELL TRANSLATIONS IN Z ***** */
                    i__9 = nu;
                    for (nn = nl; nn <= i__9; ++nn) {
                    v[2] = u[2] + (real) (nn - 5);
/*     ***** I = TARGET ATOM ***** */
                    i__10 = itar2;
                    for (ki = itar1; ki <= i__10; ++ki) {
                        inum = 0;
                        i__ = ki;
L244:
                        if (featur) {
              ++inum;
              i__ = inum;
              if (parms_1.ident[ifeat + (i__ << 1) - 3] != ki) {
                  goto L395;
              }
                        }
                        for (j = 1; j <= 3; ++j) {
              tem = 0.f;
              for (ii = 1; ii <= 3; ++ii) {
/* L245: */
                  tem += _BLNK__1.fs[ii + (j + k * 3) * 3 - 13] * 
                      parms_1.p[ii + i__ * 3 - 4];
              }
/*     ***** SEE IF WITHIN PARALLELEPIPED***** */
              tem -= v[j - 1];
              if (dx[j - 1] - dabs(tem) >= 0.f) {
                  goto L250;
              } else {
                  goto L395;
              }
L250:
              x[j - 1] = tem;
                        }
                        switch (kfun2) {
              case 1:  goto L255;
              case 2:  goto L255;
              case 3:  goto L252;
              case 4:  goto L265;
              case 5:  goto L255;
              case 6:  goto L255;
                        }
/*     ***** SEE IF WITHIN MODEL BOX ***** */
L252:
                        vm_(x, _BLNK__1.aarev, &
                            _BLNK__1.v1[1]);
                        for (j = 2; j <= 4; ++j) {
              if (_BLNK__1.ain[j + 2] - (r__1 = _BLNK__1.v1[j - 1]
                  , dabs(r__1)) >= 0.) {
                  goto L253;
              } else {
                  goto L395;
              }
L253:
              ;
                        }
                        goto L265;
/*     ***** SEE IF WITHIN SPHERE ***** */
L255:
                        dsq = vmv_(x, _BLNK__1.aa, x);
                        if (dmx - dsq >= 0.f) {
              goto L256;
                        } else {
              goto L395;
                        }
L256:
                        if (dsq - 1e-4f >= 0.f) {
              goto L260;
                        } else {
              goto L258;
                        }
L258:
                        if (kfun - 402 >= 0) {
              goto L260;
                        } else {
              goto L395;
                        }
L260:
                        tem = sqrt(dsq);
                        if (_BLNK__1.ain[7] <= 0.) {
              goto L265;
                        } else {
              goto L261;
                        }
/*     *****SELECT ONLY FIRST ASYMMETRIC UNIT ENCOUNTERED ***** */
L261:
                        if (_BLNK__1.latm <= 0) {
              goto L265;
                        } else {
              goto L262;
                        }
L262:
                        dzmin = (doublereal) i__ * d100k;
                        dzmax = dzmin + d100k;
                        i__11 = _BLNK__1.latm;
                        for (j = 1; j <= i__11; ++j) {
              dzsto = _BLNK__1.atomid[j - 1];
              if (dzsto - dzmin >= 0.) {
                  goto L263;
              } else {
                  goto L264;
              }
L263:
              if (dzmax - dzsto <= 0.) {
                  goto L264;
              } else {
                  goto L395;
              }
L264:
              ;
                        }
/*     ***** SELECT VECTORS ACCORDING TO CODES IF ANY ***** */
L265:
                        if (_BLNK__1.ncd <= 0) {
              goto L277;
                        }
/*     if logc=0, screening conditions are ORed */
/*     if logc=1, screening conditions are ANDed */
                        logc = (integer) _BLNK__1.ain[8];
/* L268: */
                        i__11 = _BLNK__1.ncd;
                        for (j = 1; j <= i__11; ++j) {
              norg = itom;
              ntar = i__;
              if (_BLNK__1.kd[j * 5 - 1] == 1) {
                  norg = parms_1.ident[(itom << 1) - 2];
                  ntar = parms_1.ident[(i__ << 1) - 2];
              }
              if (_BLNK__1.kd[j * 5 - 1] == 2) {
                  norg = parms_1.ident[(itom << 1) - 1];
                  ntar = parms_1.ident[(i__ << 1) - 1];
              }
              if (logc == 0) {
                  if (_BLNK__1.kd[j * 5 - 4] > 0) {
                  if (norg < _BLNK__1.kd[j * 5 - 5] || norg > 
                      _BLNK__1.kd[j * 5 - 4]) {
                      goto L275;
                  }
                  }
                  if (_BLNK__1.kd[j * 5 - 2] > 0) {
                  if (ntar < _BLNK__1.kd[j * 5 - 3] || ntar > 
                      _BLNK__1.kd[j * 5 - 2]) {
                      goto L275;
                  }
                  }
                  if (_BLNK__1.cd[(j << 3) - 7] > 0.f) {
                  if (tem < _BLNK__1.cd[(j << 3) - 8] || tem 
                      > _BLNK__1.cd[(j << 3) - 7]) {
                      goto L275;
                  }
                  }
                  goto L277;
              }
              if (logc == 1) {
                  if (_BLNK__1.kd[j * 5 - 4] > 0) {
                  if (norg < _BLNK__1.kd[j * 5 - 5] || norg > 
                      _BLNK__1.kd[j * 5 - 4]) {
                      goto L276;
                  }
                  }
                  if (_BLNK__1.kd[j * 5 - 2] > 0) {
                  if (ntar < _BLNK__1.kd[j * 5 - 3] || ntar > 
                      _BLNK__1.kd[j * 5 - 2]) {
                      goto L276;
                  }
                  }
                  if (_BLNK__1.cd[(j << 3) - 7] > 0.f) {
                  if (tem < _BLNK__1.cd[(j << 3) - 8] || tem 
                      > _BLNK__1.cd[(j << 3) - 7]) {
                      goto L276;
                  }
                  }
                  if (j == _BLNK__1.ncd) {
                  goto L277;
                  }
              }
L275:
              ;
                        }
L276:
                        goto L395;
L277:
                        td = d100k * (doublereal) i__ + (
                            doublereal) ((1110 - l * 
                            100 - m * 10 - nn) * 100 
                            + k);
                        if (kfun - 402 >= 0) {
              goto L325;
                        } else {
              goto L278;
                        }
/*     ***** DETERMINE CORRECT POSITION IN SORTED VECTOR TABLE ***** */
L278:
                        if (num <= 0) {
              goto L317;
                        } else {
              goto L279;
                        }
L279:
                        i__11 = num;
                        for (ii = 1; ii <= i__11; ++ii) {
              tt = s2[ii - 1] - tem;
              if (dabs(tt) - 1e-4f <= 0.f) {
                  goto L297;
              } else {
                  goto L281;
              }
L281:
              if (tt < 0.f) {
                  goto L315;
              } else if (tt == 0) {
                  goto L297;
              } else {
                  goto L283;
              }
/*     ***** MOVE LONGER VECTORS TOWARD END OF TABLE ***** */
L283:
              if (200 - num <= 0) {
                  goto L287;
              } else {
                  goto L289;
              }
L287:
              num = 199;
L289:
              ij = num;
              i__12 = num;
              for (j = ii; j <= i__12; ++j) {
                  s1d[ij] = s1d[ij - 1];
                  s2[ij] = s2[ij - 1];
/* L295: */
                  --ij;
              }
              goto L319;
/*     ***** CHECK FOR DUPLICATE VECTORS IF DISTANCES ARE EQUAL ***** */
L297:
              atom_(&s1d[ii - 1], z__);
              for (j = 1; j <= 3; ++j) {
                  if ((r__1 = x[j - 1] + y[j - 1] - z__[j - 1], 
                      dabs(r__1)) - 1e-4f <= 0.f) {
                  goto L305;
                  } else {
                  goto L315;
                  }
L305:
                  ;
              }
              goto L395;
L315:
              ;
                        }
                        if (200 - num <= 0) {
              goto L320;
                        } else {
              goto L317;
                        }
/*     ***** STORE THE RESULT IN VECTOR TABLE ***** */
L317:
                        ii = num + 1;
L319:
                        ++num;
                        s1d[ii - 1] = td;
                        s2[ii - 1] = tem;
L320:
                        if (kfun - 106 >= 0) {
              goto L325;
                        } else {
              goto L395;
                        }
/*     ***** STORE RESULT IN ATOMS TABLE ***** */
L325:
                        for (j = 1; j <= 3; ++j) {
/* L330: */
              _BLNK__1.v1[j - 1] = x[j - 1] + y[j - 1];
                        }
                        stor_(&td);
L395:
                        if (featur && inum < 
                            _BLNK__1.natom) {
              goto L244;
                        }
/* L396: */
                    }
                    }
                }
                }
L400:
                ;
            }
/*     ***** PRINT OUT DISTANCES ***** */
/* L421: */
            i0 = (integer) d_mod(&td3, &d100k);
            if (_BLNK__1.nout >= 0) {
                io___824.ciunit = _BLNK__1.nout;
                s_wsfe(&io___824);
                do_fio(&c__1, (char *)&itom, (ftnlen)sizeof(
                    integer));
                do_fio(&c__1, (char *)&i0, (ftnlen)sizeof(integer)
                    );
                do_fio(&c__1, (char *)&itar1, (ftnlen)sizeof(
                    integer));
                do_fio(&c__1, (char *)&itar2, (ftnlen)sizeof(
                    integer));
                e_wsfe();
            }
            if (num <= 0) {
                goto L500;
            } else {
                goto L423;
            }
L423:
            i__6 = num;
            for (i__ = 1; i__ <= i__6; ++i__) {
                td2 = s1d[i__ - 1];
                i1 = (integer) (td2 / d100k);
                i2 = (integer) (td2 - (doublereal) i1 * d100k);
                atom_(&td2, z__);
                if (i__ - 1 <= 0) {
                goto L432;
                } else {
                goto L434;
                }
/* L427: */
/* L429: */
L432:
                if (_BLNK__1.nout >= 0) {
                io___828.ciunit = _BLNK__1.nout;
                s_wsfe(&io___828);
                do_fio(&c__1, parms_1.chem + (itom - 1 << 3), 
                    (ftnlen)8);
                do_fio(&c__1, parms_1.chem + (i1 - 1 << 3), (
                    ftnlen)8);
                do_fio(&c__1, (char *)&itom, (ftnlen)sizeof(
                    integer));
                do_fio(&c__1, (char *)&i0, (ftnlen)sizeof(
                    integer));
                for (j = 1; j <= 3; ++j) {
                    do_fio(&c__1, (char *)&y[j - 1], (ftnlen)
                        sizeof(real));
                }
                do_fio(&c__1, (char *)&i1, (ftnlen)sizeof(
                    integer));
                do_fio(&c__1, (char *)&i2, (ftnlen)sizeof(
                    integer));
                for (j = 1; j <= 3; ++j) {
                    do_fio(&c__1, (char *)&z__[j - 1], (
                        ftnlen)sizeof(real));
                }
                do_fio(&c__1, (char *)&s2[i__ - 1], (ftnlen)
                    sizeof(real));
                e_wsfe();
                }
                goto L435;
L434:
                if (_BLNK__1.nout >= 0) {
                io___829.ciunit = _BLNK__1.nout;
                s_wsfe(&io___829);
                do_fio(&c__1, parms_1.chem + (itom - 1 << 3), 
                    (ftnlen)8);
                do_fio(&c__1, parms_1.chem + (i1 - 1 << 3), (
                    ftnlen)8);
                do_fio(&c__1, (char *)&i1, (ftnlen)sizeof(
                    integer));
                do_fio(&c__1, (char *)&i2, (ftnlen)sizeof(
                    integer));
                for (j = 1; j <= 3; ++j) {
                    do_fio(&c__1, (char *)&z__[j - 1], (
                        ftnlen)sizeof(real));
                }
                do_fio(&c__1, (char *)&s2[i__ - 1], (ftnlen)
                    sizeof(real));
                e_wsfe();
                }
L435:
                ;
            }
/*     ***** CALCULATE ANGLES ABOUT REF ATOM IF CODE IS 102 ***** */
/* L437: */
            if (kfun - 102 != 0) {
                goto L500;
            } else {
                goto L451;
            }
/* L441: */
L451:
            if (_BLNK__1.nout >= 0) {
                io___830.ciunit = _BLNK__1.nout;
                s_wsfe(&io___830);
                do_fio(&c__1, (char *)&itom, (ftnlen)sizeof(
                    integer));
                e_wsfe();
            }
            l = num - 1;
            if (l <= 0) {
                goto L500;
            } else {
                goto L457;
            }
L457:
            i__6 = l;
            for (i__ = 1; i__ <= i__6; ++i__) {
                td2 = s1d[i__ - 1];
                t3 = s2[i__ - 1];
                i1 = (integer) (td2 / d100k);
                i2 = (integer) (td2 - (doublereal) i1 * d100k);
                atom_(&td2, x);
                difv_(x, y, u);
                mv_(_BLNK__1.aa, u, _BLNK__1.v2);
                m = i__ + 1;
                i__10 = num;
                for (j = m; j <= i__10; ++j) {
                td4 = s1d[j - 1];
                j1 = (integer) (td4 / d100k);
                j2 = (integer) (td4 - (doublereal) j1 * d100k)
                    ;
                atom_(&td4, z__);
                difv_(z__, y, v);
                r__1 = vv_(v, _BLNK__1.v2) / (t3 * s2[j - 1]);
                f = arccos_(&r__1);
                difv_(x, z__, _BLNK__1.v3);
                f1 = sqrt(vmv_(_BLNK__1.v3, _BLNK__1.aa, 
                    _BLNK__1.v3));
/* L460: */
                if (_BLNK__1.nout >= 0) {
                    io___837.ciunit = _BLNK__1.nout;
                    s_wsfe(&io___837);
                    do_fio(&c__1, parms_1.chem + (i1 - 1 << 3)
                        , (ftnlen)8);
                    do_fio(&c__1, parms_1.chem + (itom - 1 << 
                        3), (ftnlen)8);
                    do_fio(&c__1, parms_1.chem + (j1 - 1 << 3)
                        , (ftnlen)8);
                    do_fio(&c__1, (char *)&i1, (ftnlen)sizeof(
                        integer));
                    do_fio(&c__1, (char *)&i2, (ftnlen)sizeof(
                        integer));
                    do_fio(&c__1, (char *)&itom, (ftnlen)
                        sizeof(integer));
                    do_fio(&c__1, (char *)&i0, (ftnlen)sizeof(
                        integer));
                    do_fio(&c__1, (char *)&j1, (ftnlen)sizeof(
                        integer));
                    do_fio(&c__1, (char *)&j2, (ftnlen)sizeof(
                        integer));
                    do_fio(&c__1, (char *)&f1, (ftnlen)sizeof(
                        real));
                    do_fio(&c__1, (char *)&f, (ftnlen)sizeof(
                        real));
                    e_wsfe();
                }
/* L465: */
                }
            }
/* L495: */
L500:
            ;
            }
        }
        }
    }
    }
    if (last - list <= 0) {
    goto L505;
    } else {
    goto L178;
    }
L505:
    if (kfun2 - 6 != 0) {
    goto L600;
    } else {
    goto L176;
    }
L600:
    if (kfun - 106 != 0) {
    goto L610;
    } else {
    goto L605;
    }
L605:
    _BLNK__1.latm = latom;
L610:
    return 0;
} /* searc_ */

/* Subroutine */ int simbol_(real *w, real *w2, real *hgt, char *itxt, real *
    tht, integer *n, ftnlen itxt_len)
{
    /* Initialized data */

    static integer iptr[90] = { 312,32,41,54,47,20,20,54,64,70,103,108,17,115,
        118,128,140,128,140,194,166,102,208,120,27,211,216,0,0,0,0,237,0,
        150,328,301,168,223,92,328,88,188,180,1,82,180,82,30,128,239,244,
        252,269,277,258,284,192,289,76,76,11,6,14,153,350,360,370,378,388,
        398,407,421,428,436,444,450,455,465,472,484,494,504,510,521,526,
        533,536,541,546,550 };
    static integer nknt[90] = { 16,9,12,8,7,7,6,10,6,6,5,7,3,5,4,9,7,12,10,12,
        4,6,3,5,5,5,7,0,0,0,0,2,0,7,9,11,12,14,10,4,4,4,8,5,6,2,5,2,9,5,8,
        13,8,9,11,5,16,12,11,12,3,5,3,13,10,10,8,10,10,9,14,7,8,8,6,5,10,
        7,9,10,10,6,11,5,7,3,5,5,4,7 };
    static integer ixyt[556] = { 44,48,46,26,66,24,64,99,66,26,68,26,64,24,66,
        28,29,22,62,69,29,26,56,26,22,62,29,62,99,22,69,22,25,65,25,28,39,
        59,68,62,63,65,56,26,56,67,68,59,29,22,52,63,68,68,59,39,28,23,32,
        52,63,65,55,22,29,26,66,69,62,32,52,42,49,39,59,35,36,46,45,35,99,
        42,32,33,43,42,31,69,58,53,62,62,37,38,49,58,25,24,33,43,64,29,23,
        32,52,63,69,29,22,25,69,99,47,62,22,29,45,69,62,29,22,46,62,69,47,
        69,99,68,59,39,28,23,32,52,63,68,99,44,62,22,29,59,68,67,56,26,56,
        65,62,49,44,99,32,43,52,32,99,44,46,56,67,68,59,39,28,29,69,49,42,
        99,23,53,64,65,56,36,27,38,68,25,65,45,63,27,45,23,67,29,38,33,22,
        56,67,68,59,39,28,27,36,56,65,63,52,32,23,25,36,29,42,69,29,47,42,
        47,69,29,69,22,62,99,36,56,38,28,29,39,38,99,69,22,99,53,63,62,52,
        53,15,75,38,49,42,32,52,28,39,59,68,66,24,22,62,28,39,59,68,67,56,
        36,56,65,63,52,32,23,28,39,59,68,29,24,64,54,59,52,42,62,23,32,52,
        63,65,56,26,29,69,68,43,42,23,32,52,63,68,59,39,28,26,35,55,66,24,
        64,54,53,57,56,66,26,36,37,33,66,57,47,36,35,44,54,65,67,58,38,27,
        24,33,53,64,57,49,59,57,99,37,29,39,37,22,32,12,22,23,21,22,31,13,
        22,33,11,22,62,67,66,57,37,26,23,32,52,63,22,29,26,37,57,66,63,52,
        32,23,63,52,32,23,26,37,57,66,62,69,66,57,37,26,23,32,52,63,63,52,
        32,23,26,37,57,66,65,35,32,36,26,46,36,38,49,59,68,64,67,66,57,37,
        26,24,33,53,64,62,51,31,22,22,29,26,37,57,66,62,32,52,42,47,37,99,
        48,49,32,41,51,62,67,99,68,69,22,29,24,57,35,62,32,52,42,49,39,22,
        27,26,37,46,42,46,57,66,62,22,27,26,37,57,66,62,63,52,32,23,26,37,
        57,66,63,99,67,45,21,27,26,37,57,66,64,53,33,24,61,67,66,57,37,26,
        24,33,53,64,22,27,26,37,57,66,23,32,52,63,64,55,35,26,37,57,66,42,
        49,47,27,67,67,62,63,52,32,23,27,27,42,67,27,32,46,52,67,22,67,99,
        27,62,27,43,67,31,62,22,67,27,99,35,55 };
    static real rad = .01745329f;

    /* Format strings */
    static char fmt_21[] = "(\002TXT\002,i2,1x,i2,\002 1\002,4(1x,f10.6))";
    static char fmt_22[] = "(80a1)";

    /* System generated locals */
    integer i__1, i__2;
    real r__1, r__2, r__3;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    double sin(doublereal), cos(doublereal);

    /* Local variables */
    static real d__;
    static integer i__, j, k;
    static real dc[10], ds[10], ct;
    static integer ip, nk;
    static real th, dx, dy;
    static integer ix, iy;
    static real st, xo, yo;
    static integer ich;
    static real dct, dst;
    static integer ixy, ipen;
    extern /* Subroutine */ int draw_(real *, real *, real *, integer *);
    static integer itxtj;

    /* Fortran I/O blocks */
    static cilist io___842 = { 0, 0, 0, fmt_21, 0 };
    static cilist io___843 = { 0, 0, 0, fmt_22, 0 };


    /* Parameter adjustments */
    --itxt;
    --w;

    /* Function Body */
/*        @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O */
/*        P   Q   R   S   T   U   V   W   X   Y   Z                   _ */
/*            !   "   #   $   %   &   '   (   )   *   +   ,   -   .   / */
/*        0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ? */
/*        a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p */
/*        q   r   s   t   u   v   w   x   y   z */
    if (ns_1.ndraw == 9) {
    io___842.ciunit = ns_1.npf;
    s_wsfe(&io___842);
    do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&ns_1.nvar, (ftnlen)sizeof(integer));
    r__1 = w[1] + trfac_1.xtrans;
    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
    r__2 = w[2] + trfac_1.ytrans;
    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
    r__3 = *hgt * 8.f;
    do_fio(&c__1, (char *)&r__3, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&(*tht), (ftnlen)sizeof(real));
    e_wsfe();
    io___843.ciunit = ns_1.npf;
    s_wsfe(&io___843);
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
        do_fio(&c__1, itxt + k, (ftnlen)1);
    }
    e_wsfe();
    return 0;
    }
/* -----TEST FOR SPECIAL CASE OF CENTERED SYMBOL */
    if (*n <= 0) {
    goto L400;
    }
/* -----SET UP TABLE OF INCREMENTS BASED ON HGT AND THT */
    if (*tht == 0.f) {
    goto L120;
    }
    th = rad * *tht;
    st = sin(th);
    ct = cos(th);
    goto L130;
L120:
    st = 0.f;
    ct = 1.f;
L130:
    d__ = *hgt / 7.f;
    dst = d__ * st;
    dct = d__ * ct;
    ds[0] = -dst;
    dc[0] = -dct;
    for (i__ = 2; i__ <= 10; ++i__) {
    ds[i__ - 1] = ds[i__ - 2] + dst;
    dc[i__ - 1] = dc[i__ - 2] + dct;
/* L145: */
    }
/* -----START LOOP THROUGH THE N CHARACTERS OF ITXT */
    xo = 0.f;
    yo = 0.f;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
    itxtj = *(unsigned char *)&itxt[j];
    if (itxtj >= 97 && itxtj <= 122) {
        ich = itxtj - 32;
        goto L221;
    }
/* -----MASK IT TO SIX BITS AND ADD ONE. PICK UP POINTER AND COUNTER */
/* L220: */
    ich = itxtj % 64 + 1;
L221:
    ip = iptr[ich - 1];
    nk = nknt[ich - 1];
/* -----TEST FOR SPACE OR UNDEFINED CHARACTER */
    if (nk == 0) {
        goto L360;
    }
/* -----START LOOP THROUGH SEGMENTS OF CHARACTER. LIFT PEN INITIALLY */
    ipen = 3;
    i__2 = nk;
    for (k = 1; k <= i__2; ++k) {
        ixy = ixyt[ip - 1];
/* -----LIFT PEN IF SPECIAL INDICATOR IS FOUND */
        if (ixy != 99) {
        goto L300;
        }
        ipen = 3;
        goto L340;
L300:
        ix = ixy / 10;
        iy = ixy - ix * 10;
        dx = xo + dc[ix - 1] - ds[iy - 1];
        dy = yo + dc[iy - 1] + ds[ix - 1];
        draw_(&w[1], &dx, &dy, &ipen);
/* -----PUT PEN DOWN TO DRAW NEXT SEGMENTS */
        ipen = 2;
L340:
        ++ip;
/* L350: */
    }
/* -----MOVE ORIGIN TO NEXT CHARACTER POSITION */
L360:
    xo += dc[7];
    yo += ds[7];
/* L370: */
    }
    return 0;
/* *** Only one centered symbol (*) is available in ORTEP-III. */
/* -----PLOT ONE SPECIFIC CENTERED SYMBOL. SET UP TABLE OF INCREMENTS */
L400:
    dct = *hgt / 2.f;
    dc[0] = -dct;
    dc[1] = 0.f;
    dc[2] = dct;
/* -----MOVE TO SYMBOL WITH PEN UP OR DOWN, DEPENDING ON N */
    ipen = 3;
    if (*n <= -2) {
    ipen = 2;
    }
/* -----LOOP THROUGH SEGMENTS OF CENTERED SYMBOL */
    for (k = 337; k <= 349; ++k) {
    ixy = ixyt[k - 1];
    ix = ixy / 10;
    iy = ixy - ix * 10;
    draw_(&w[1], &dc[ix - 1], &dc[iy - 1], &ipen);
/* -----PUT PEN DOWN TO DRAW REMAINING SEGMENTS */
    ipen = 2;
/* L440: */
    }
    return 0;
} /* simbol_ */

/* Subroutine */ int spare_(integer *inst)
{
/*     ***** THIS SUBROUTINE MAY BE USED FOR NEW INSTRUCTIONS ***** */
    return 0;
} /* spare_ */

/* Subroutine */ int stor_(doublereal *td1)
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Local variables */
    static integer i__, j, k, l;
    extern /* Subroutine */ int erpnt_(doublereal *, integer *);

/*     ***** STORE IN OR REMOVE FROM ATOMS ARRAY ***** */
    if (_BLNK__1.latm <= 0) {
    goto L481;
    } else {
    goto L450;
    }
L450:
    if (2e3f - _BLNK__1.latm <= 0.f) {
    goto L455;
    } else {
    goto L460;
    }
L455:
    if (_BLNK__1.nj2 - 10 <= 0) {
    goto L490;
    } else {
    goto L460;
    }
L460:
    l = _BLNK__1.latm;
/*     ***** CHECK FOR POSITIONAL DUPLICATION ***** */
    i__1 = l;
    for (k = 1; k <= i__1; ++k) {
    for (j = 1; j <= 3; ++j) {
        if ((r__1 = _BLNK__1.v1[j - 1] - _BLNK__1.atoms[j + k * 3 - 4], 
            dabs(r__1)) - .001f <= 0.f) {
        goto L465;
        } else {
        goto L480;
        }
L465:
        ;
    }
    if (_BLNK__1.nj2 - 10 <= 0) {
        goto L490;
    } else {
        goto L470;
    }
/*     ***** ATOM REMOVAL BY TABLE PUSHDOWN ***** */
L470:
    --_BLNK__1.latm;
    i__2 = _BLNK__1.latm;
    for (i__ = k; i__ <= i__2; ++i__) {
        _BLNK__1.atomid[i__ - 1] = _BLNK__1.atomid[i__];
        for (j = 1; j <= 3; ++j) {
/* L475: */
        _BLNK__1.atoms[j + i__ * 3 - 4] = _BLNK__1.atoms[j + (i__ + 1)
             * 3 - 4];
        }
    }
    goto L490;
L480:
    ;
    }
L481:
    if (_BLNK__1.nj2 - 10 >= 0) {
    goto L490;
    } else {
    goto L482;
    }
/*     ***** STORE ATOM ***** */
L482:
    if ((r__1 = 1999.f - _BLNK__1.latm) < 0.f) {
    goto L490;
    } else if (r__1 == 0) {
    goto L483;
    } else {
    goto L485;
    }
L483:
    _BLNK__1.ng = 16;
    erpnt_(td1, &c__400);
L485:
    ++_BLNK__1.latm;
    for (j = 1; j <= 3; ++j) {
/* L486: */
    _BLNK__1.atoms[j + _BLNK__1.latm * 3 - 4] = _BLNK__1.v1[j - 1];
    }
    _BLNK__1.atomid[_BLNK__1.latm - 1] = *td1;
L490:
    return 0;
} /* stor_ */

/* Subroutine */ int tepsym_(char *txt, integer *num, integer *kk, ftnlen 
    txt_len)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer i_indx(char *, char *, ftnlen, ftnlen), s_cmp(char *, char *, 
        ftnlen, ftnlen);

    /* Local variables */
    static integer i__, k, m, n, iden, inum, ksign, iascii;
    static logical twodig;

/* *** parses character string representation of symmetry operators */
/* *** and stores information */
/* *** convert txt to upper case */
    for (i__ = 1; i__ <= 24; ++i__) {
    iascii = *(unsigned char *)&txt[i__ - 1];
    if (iascii >= 97 && iascii <= 122) {
        *(unsigned char *)&txt[i__ - 1] = (char) (iascii - 32);
    }
/* L101: */
    }
/* *** default value of ts if no fraction specified */
/* L202: */
    _BLNK__1.ts[*kk + *num * 3 - 4] = 0.f;
/* *** look for and interpret a/b style fraction */
    n = i_indx(txt, "/", (ftnlen)24, (ftnlen)1);
    if (n > 0) {
/*        get denominator */
    i__1 = n;
    k = *(unsigned char *)&txt[i__1] - 48;
    i__1 = n + 1;
    m = *(unsigned char *)&txt[i__1] - 48;
    if (m >= 0 && m <= 9) {
        iden = k * 10 + m;
    } else {
        iden = k;
    }
/*        get numerator */
    twodig = FALSE_;
    ksign = 1;
    i__1 = n - 2;
    k = *(unsigned char *)&txt[i__1] - 48;
    if (n - 2 >= 1) {
        i__1 = n - 3;
        m = *(unsigned char *)&txt[i__1] - 48;
        if (m >= 0 && m <= 9) {
        twodig = TRUE_;
        }
        i__1 = n - 3;
        if (s_cmp(txt + i__1, "-", n - 2 - i__1, (ftnlen)1) == 0) {
        ksign = -1;
        }
        if (n - 3 >= 1) {
        i__1 = n - 4;
        if (s_cmp(txt + i__1, "-", n - 3 - i__1, (ftnlen)1) == 0) {
            ksign = -1;
        }
        }
    }
    if (twodig) {
        inum = ksign * (m * 10 + k);
    } else {
        inum = ksign * k;
    }
    _BLNK__1.ts[*kk + *num * 3 - 4] = (real) inum / (real) iden;
    }
/* *** look for and interpret decimal style fraction */
    n = i_indx(txt, ".", (ftnlen)24, (ftnlen)1);
    if (n > 0) {
/*        get post decimal point portion */
    i__1 = n;
    k = *(unsigned char *)&txt[i__1] - 48;
    i__1 = n + 1;
    m = *(unsigned char *)&txt[i__1] - 48;
    if (m >= 0 && m <= 9) {
        _BLNK__1.ts[*kk + *num * 3 - 4] = (real) k * .1f + (real) m * 
            .01f;
    } else {
        _BLNK__1.ts[*kk + *num * 3 - 4] = (real) k * .1f;
    }
/*        get sign */
    ksign = 1;
    if (n - 1 >= 1) {
        i__1 = n - 2;
        if (s_cmp(txt + i__1, "-", n - 1 - i__1, (ftnlen)1) == 0) {
        ksign = -1;
        }
    }
    if (n - 2 >= 1) {
        i__1 = n - 3;
        if (s_cmp(txt + i__1, "-", n - 2 - i__1, (ftnlen)1) == 0) {
        ksign = -1;
        }
    }
    _BLNK__1.ts[*kk + *num * 3 - 4] = (real) ksign * _BLNK__1.ts[*kk + *
        num * 3 - 4];
    }
/* *** interpret xyz portion of symmetry operation */
    for (i__ = 1; i__ <= 24; ++i__) {
    if (*(unsigned char *)&txt[i__ - 1] == 'X') {
        _BLNK__1.fs[(*kk + *num * 3) * 3 - 12] = 1.f;
        if (i__ >= 2) {
        i__1 = i__ - 2;
        if (s_cmp(txt + i__1, "-", i__ - 1 - i__1, (ftnlen)1) == 0) {
            _BLNK__1.fs[(*kk + *num * 3) * 3 - 12] = -1.f;
        }
        }
    }
    if (*(unsigned char *)&txt[i__ - 1] == 'Y') {
        _BLNK__1.fs[(*kk + *num * 3) * 3 - 11] = 1.f;
        if (i__ >= 2) {
        i__1 = i__ - 2;
        if (s_cmp(txt + i__1, "-", i__ - 1 - i__1, (ftnlen)1) == 0) {
            _BLNK__1.fs[(*kk + *num * 3) * 3 - 11] = -1.f;
        }
        }
    }
    if (*(unsigned char *)&txt[i__ - 1] == 'Z') {
        _BLNK__1.fs[(*kk + *num * 3) * 3 - 10] = 1.f;
        if (i__ >= 2) {
        i__1 = i__ - 2;
        if (s_cmp(txt + i__1, "-", i__ - 1 - i__1, (ftnlen)1) == 0) {
            _BLNK__1.fs[(*kk + *num * 3) * 3 - 10] = -1.f;
        }
        }
    }
/* L303: */
    }
    return 0;
} /* tepsym_ */

/* Subroutine */ int tmm_(real *x, real *y, real *z__)
{
    static real x11, x12, x13, x21, x22, x23, x31, x32, x33, y11, y12, y13, 
        y21, y22, y23, y31, y32, y33;

/*     Z = TRANSPOSED (TRANSPOSE(X) * (Y) ) */
/*     Z(3,3)=X(3,3)*Y(3,3) */
    /* Parameter adjustments */
    z__ -= 4;
    y -= 4;
    x -= 4;

    /* Function Body */
    x11 = x[4];
    x12 = x[7];
    x13 = x[10];
    x21 = x[5];
    x22 = x[8];
    x23 = x[11];
    x31 = x[6];
    x32 = x[9];
    x33 = x[12];
    y11 = y[4];
    y12 = y[7];
    y13 = y[10];
    y21 = y[5];
    y22 = y[8];
    y23 = y[11];
    y31 = y[6];
    y32 = y[9];
    y33 = y[12];
    z__[4] = x11 * y11 + x21 * y21 + x31 * y31;
    z__[7] = x12 * y11 + x22 * y21 + x32 * y31;
    z__[10] = x13 * y11 + x23 * y21 + x33 * y31;
    z__[5] = x11 * y12 + x21 * y22 + x31 * y32;
    z__[8] = x12 * y12 + x22 * y22 + x32 * y32;
    z__[11] = x13 * y12 + x23 * y22 + x33 * y32;
    z__[6] = x11 * y13 + x21 * y23 + x31 * y33;
    z__[9] = x12 * y13 + x22 * y23 + x32 * y33;
    z__[12] = x13 * y13 + x23 * y23 + x33 * y33;
    return 0;
} /* tmm_ */

/* Subroutine */ int uinput_0_(int n__, integer *in, integer *nout, integer *
    iu)
{
    /* Format strings */
    static char fmt_115[] = "(\002 Enter instruction set file name or \"exi"
        "t\" [\002,a,\002]:  \002,$)";
    static char fmt_120[] = "(a)";
    static char fmt_130[] = "(/\002 \"\002,a,\002\" does not exist\002/)";
    // :- static char fmt_140[] = "(\002 Drawing to (1) Screen, (2) Postscript fil"
    //    "e, (3) HPGL file, or (0) Omit [\002,i1,\002]:  \002,$)";
    static char fmt_140[] = "(\002 Output Target: (1) Cairo/Png, (2) Postscript, (3) HPGL, or (0) Omit [\002,i1,\002]:  \002,$)";
    // --
    static char fmt_145[] = "(a)";
    static char fmt_155[] = "(i1)";
    static char fmt_1452[] = "(\002 (1) Portrait or (2) Landscape orientatio"
        "n [\002,i1,\002]:  \002,$)";
    static char fmt_1453[] = "(\002 How tall is printer page in inches? ["
        "\002,f5.2,\002]:  \002,$)";
    static char fmt_1454[] = "(f10.0)";
    static char fmt_150[] = "(\002 Text output to (1) File, (2) Screen, or ("
        "0) Omit [\002,i1,\002]:  \002,$)";
    static char fmt_165[] = "(\002 Enter output file name [\002,a,\002]: "
        " \002,$)";
    static char fmt_215[] = "(\002 Enter atom parameter file name or \"exi"
        "t\" [\002,a,\002]:  \002,$)";
    // :-
    /*
    static char fmt_303[] = "(/,\002 (1) Save drawing as Postscript file\002"
        ",/,\002 (2) Save drawing as HPGL file\002,/,\002 (3) Redraw stru"
        "cture on screen\002,/,\002 (4) Edit instruction set\002,/,\002 ["
        "Quit]:  \002,$)";
    */
    static char fmt_303[] = "(/,\002 (1) Save drawing as Postscript file\002"
        ",/,\002 (2) Save drawing as HPGL file\002,/,\002 (3) Save drawing as PNG file"
        "\002,/,\002 (4) Edit instruction set\002,/,\002 ["
        "Quit]:  \002,$)";
    // --
    static char fmt_304[] = "(a)";

    /* System generated locals */
    address a__1[2];
    integer i__1, i__2[2];
    olist o__1;

    /* Builtin functions */
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    integer i_indx(char *, char *, ftnlen, ftnlen), s_wsfe(cilist *), do_fio(
        integer *, char *, ftnlen), e_wsfe(void), s_rsfe(cilist *), 
        e_rsfe(void), s_cmp(char *, char *, ftnlen, ftnlen), f_open(olist 
        *), s_rsfi(icilist *), e_rsfi(void), s_wsle(cilist *), do_lio(
        integer *, integer *, char *, ftnlen), e_wsle(void);
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer nou, ipos, iflag;
    static char fname[60];
    extern /* Subroutine */ int editr_(void), dflts_(void), exitng_(integer *)
        ;
    static char answer[1], txtans[60];
    extern /* Subroutine */ int recycle_(void);

    /* Fortran I/O blocks */
    static cilist io___901 = { 0, 6, 0, fmt_115, 0 };
    static cilist io___902 = { 0, 5, 0, fmt_120, 0 };
    static cilist io___904 = { 0, 6, 0, fmt_130, 0 };
    static cilist io___905 = { 0, 6, 0, fmt_140, 0 };
    static cilist io___906 = { 0, 5, 0, fmt_145, 0 };
    static icilist io___908 = { 0, answer, 0, fmt_155, 1, 1 };
    static cilist io___909 = { 0, 6, 0, 0, 0 };
    static cilist io___910 = { 0, 6, 0, fmt_1452, 0 };
    static cilist io___911 = { 0, 5, 0, fmt_145, 0 };
    static icilist io___912 = { 0, answer, 0, fmt_155, 1, 1 };
    static cilist io___913 = { 0, 6, 0, 0, 0 };
    static cilist io___914 = { 0, 6, 0, fmt_1453, 0 };
    static cilist io___915 = { 0, 5, 0, fmt_120, 0 };
    static icilist io___916 = { 0, txtans, 0, fmt_1454, 60, 1 };
    static cilist io___917 = { 0, 6, 0, fmt_150, 0 };
    static cilist io___918 = { 0, 5, 0, fmt_145, 0 };
    static icilist io___920 = { 0, answer, 0, fmt_155, 1, 1 };
    static cilist io___921 = { 0, 6, 0, fmt_165, 0 };
    static cilist io___922 = { 0, 5, 0, fmt_120, 0 };
    static cilist io___923 = { 0, 6, 0, fmt_215, 0 };
    static cilist io___924 = { 0, 5, 0, fmt_120, 0 };
    static cilist io___925 = { 0, 6, 0, fmt_130, 0 };
    static cilist io___926 = { 0, 6, 0, fmt_303, 0 };
    static cilist io___927 = { 0, 5, 0, fmt_304, 0 };


/* *** user input routine */
    switch(n__) {
    case 1: goto L_getpap;
    case 2: goto L_gtafil;
    case 3: goto L_go2edtr;
    }

    dflts_();
    iflag = 0;
/* *** get the input file name and open the file or "exit" *** */
L110:
    s_copy(fname, dfl_1.infile__, (ftnlen)60, (ftnlen)60);
    ipos = i_indx(fname, " ", (ftnlen)60, (ftnlen)1);
    s_wsfe(&io___901);
    do_fio(&c__1, fname, ipos - 1);
    e_wsfe();
    s_rsfe(&io___902);
    do_fio(&c__1, txtans, (ftnlen)60);
    e_rsfe();
    if (s_cmp(txtans, "exit", (ftnlen)4, (ftnlen)4) == 0 || s_cmp(txtans, 
        "EXIT", (ftnlen)4, (ftnlen)4) == 0) {
    exitng_(&c__0);
    }
    if (*(unsigned char *)txtans != ' ') {
    s_copy(fname, txtans, (ftnlen)60, (ftnlen)60);
    }
    o__1.oerr = 1;
    o__1.ounit = *in;
    o__1.ofnmlen = 60;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "old";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
    goto L125;
    }
    goto L135;
L125:
    ipos = i_indx(fname, " ", (ftnlen)60, (ftnlen)1);
    s_copy(fname, fname, (ftnlen)60, ipos - 1);
    s_wsfe(&io___904);
    do_fio(&c__1, fname, ipos - 1);
    e_wsfe();
    goto L110;
/* *** determine where ortep drawing should go *** */
L135:
    s_wsfe(&io___905);
    do_fio(&c__1, (char *)&dfl_1.idraw, (ftnlen)sizeof(integer));
    e_wsfe();
    s_rsfe(&io___906);
    do_fio(&c__1, answer, (ftnlen)1);
    e_rsfe();
    if (*(unsigned char *)answer == ' ') {
    ns_1.ndraw = dfl_1.idraw;
    } else {
    s_rsfi(&io___908);
    do_fio(&c__1, (char *)&ns_1.ndraw, (ftnlen)sizeof(integer));
    e_rsfi();
    }
    if (ns_1.ndraw < 0 || ns_1.ndraw > 3 && ns_1.ndraw != 9) {
    s_wsle(&io___909);
    do_lio(&c__9, &c__1, "invalid selection", (ftnlen)17);
    e_wsle();
    goto L135;
    }
    if (ns_1.ndraw == 0 || ns_1.ndraw == 1 || ns_1.ndraw == 9) {
    goto L149;
    }
    goto L1451;
/* *** need to get this information if printing from editor */

L_getpap:
    iflag = 1;
/* *** determine orientation of drawing *** */
L1451:
    s_wsfe(&io___910);
    do_fio(&c__1, (char *)&dfl_1.iorient, (ftnlen)sizeof(integer));
    e_wsfe();
    s_rsfe(&io___911);
    do_fio(&c__1, answer, (ftnlen)1);
    e_rsfe();
    if (*(unsigned char *)answer == ' ') {
    ns_1.norient = dfl_1.iorient;
    } else {
    s_rsfi(&io___912);
    do_fio(&c__1, (char *)&ns_1.norient, (ftnlen)sizeof(integer));
    e_rsfi();
    }
    if (ns_1.norient < 1 || ns_1.norient > 2) {
    s_wsle(&io___913);
    do_lio(&c__9, &c__1, "invalid selection", (ftnlen)17);
    e_wsle();
    goto L1451;
    }
/* *** determine paper length for postscript landscape */
    if (ns_1.ndraw == 2 && ns_1.norient == 2) {
    s_wsfe(&io___914);
    do_fio(&c__1, (char *)&dfl_1.fpaplen, (ftnlen)sizeof(real));
    e_wsfe();
    s_rsfe(&io___915);
    do_fio(&c__1, txtans, (ftnlen)60);
    e_rsfe();
    if (*(unsigned char *)txtans != ' ') {
        s_rsfi(&io___916);
        do_fio(&c__1, (char *)&dfl_1.fpaplen, (ftnlen)sizeof(real));
        e_rsfi();
    }
    ns_1.nvar = dfl_1.fpaplen * 1e3f;
    }
/* *** if called from recycle, return there */
    if (iflag == 1) {
    return 0;
    }
/* *** determine where ortep output should go *** */
L149:
    s_wsfe(&io___917);
    do_fio(&c__1, (char *)&dfl_1.iout, (ftnlen)sizeof(integer));
    e_wsfe();
    s_rsfe(&io___918);
    do_fio(&c__1, answer, (ftnlen)1);
    e_rsfe();
    if (*(unsigned char *)answer == ' ') {
    nou = dfl_1.iout;
    } else {
    s_rsfi(&io___920);
    do_fio(&c__1, (char *)&nou, (ftnlen)sizeof(integer));
    e_rsfi();
    }
/* *** set output unit number *** */
    *nout = -4;
    if (nou == 1) {
    *nout = 4;
    }
    if (nou == 2) {
    *nout = 6;
    }
/* *** if output goes to a file; get its name and open the file *** */
    if (*nout == 4) {
    ipos = i_indx(fname, ".", (ftnlen)60, (ftnlen)1);
    if (ipos != 0) {
/* Writing concatenation */
        i__2[0] = ipos - 1, a__1[0] = fname;
        i__2[1] = 4, a__1[1] = dfl_1.ext;
        s_cat(fname, a__1, i__2, &c__2, (ftnlen)60);
        goto L160;
    }
    ipos = i_indx(fname, " ", (ftnlen)60, (ftnlen)1);
/* Writing concatenation */
    i__2[0] = ipos - 1, a__1[0] = fname;
    i__2[1] = 4, a__1[1] = dfl_1.ext;
    s_cat(fname, a__1, i__2, &c__2, (ftnlen)60);
L160:
    ipos = i_indx(fname, " ", (ftnlen)60, (ftnlen)1);
    s_wsfe(&io___921);
    do_fio(&c__1, fname, ipos - 1);
    e_wsfe();
    s_rsfe(&io___922);
    do_fio(&c__1, txtans, (ftnlen)60);
    e_rsfe();
    if (*(unsigned char *)txtans != ' ') {
        s_copy(fname, txtans, (ftnlen)60, (ftnlen)60);
    }
    o__1.oerr = 1;
    o__1.ounit = *nout;
    o__1.ofnmlen = 60;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "old";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
        goto L170;
    }
    goto L175;
L170:
    o__1.oerr = 0;
    o__1.ounit = *nout;
    o__1.ofnmlen = 60;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    }
L175:
    return 0;
/* *** get file name of an external file containing atomic parameters */

L_gtafil:
L210:
    s_copy(fname, dfl_1.atomfi, (ftnlen)60, (ftnlen)60);
    ipos = i_indx(fname, " ", (ftnlen)60, (ftnlen)1);
    s_wsfe(&io___923);
    do_fio(&c__1, fname, ipos - 1);
    e_wsfe();
    s_rsfe(&io___924);
    do_fio(&c__1, txtans, (ftnlen)60);
    e_rsfe();
    if (s_cmp(txtans, "exit", (ftnlen)4, (ftnlen)4) == 0 || s_cmp(txtans, 
        "EXIT", (ftnlen)4, (ftnlen)4) == 0) {
    exitng_(&c__0);
    }
    if (*(unsigned char *)txtans != ' ') {
    s_copy(fname, txtans, (ftnlen)60, (ftnlen)60);
    }
    o__1.oerr = 1;
    o__1.ounit = *iu;
    o__1.ofnmlen = 60;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "old";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
    goto L225;
    }
    return 0;
L225:
    ipos = i_indx(fname, " ", (ftnlen)60, (ftnlen)1);
    s_wsfe(&io___925);
    do_fio(&c__1, fname, ipos - 1);
    e_wsfe();
    goto L210;
/* *** ask user about using editor */

L_go2edtr:
    s_wsfe(&io___926);
    e_wsfe();
/* 303 format(/,' Edit instruction set? (Y)es or (N)o [N]:  ',$) */
    s_rsfe(&io___927);
    do_fio(&c__1, answer, (ftnlen)1);
    e_rsfe();
    if (*(unsigned char *)answer == '1') {
    ns_1.ndraw = 2;
    recycle_();
    } else {
    if (*(unsigned char *)answer == '2') {
        ns_1.ndraw = 3;
        recycle_();
    } else {
        if (*(unsigned char *)answer == '3') {
        ns_1.ndraw = 1;
        recycle_();
        } else {
        if (*(unsigned char *)answer == '4') {
            editr_();
        }
        }
    }
    }
    return 0;
} /* uinput_ */

/* Subroutine */ int uinput_(integer *in, integer *nout)
{
    return uinput_0_(0, in, nout, (integer *)0);
    }

/* Subroutine */ int getpap_(void)
{
    return uinput_0_(1, (integer *)0, (integer *)0, (integer *)0);
    }

/* Subroutine */ int gtafil_(integer *iu)
{
    return uinput_0_(2, (integer *)0, (integer *)0, iu);
    }

/* Subroutine */ int go2edtr_(void)
{
    return uinput_0_(3, (integer *)0, (integer *)0, (integer *)0);
    }

/* Subroutine */ int unity_(real *x, real *z__, integer *itype)
{
    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static real y[3], t1;

    /* Parameter adjustments */
    --z__;
    --x;

    /* Function Body */
    y[0] = x[1];
    y[1] = x[2];
    y[2] = x[3];
    if (*itype <= 0) {
    goto L125;
    } else {
    goto L105;
    }
L105:
    t1 = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    goto L145;
L125:
    t1 = sqrt(y[0] * (y[0] * _BLNK__1.aa[0] + y[1] * (_BLNK__1.aa[3] + 
        _BLNK__1.aa[1]) + y[2] * (_BLNK__1.aa[6] + _BLNK__1.aa[2])) + y[1]
         * (y[1] * _BLNK__1.aa[4] + y[2] * (_BLNK__1.aa[7] + _BLNK__1.aa[
        5])) + y[2] * y[2] * _BLNK__1.aa[8]);
L145:
    if (t1 <= 0.f) {
    goto L155;
    } else {
    goto L175;
    }
L155:
    _BLNK__1.ng = 5;
    goto L300;
L175:
    z__[1] = y[0] / t1;
    z__[2] = y[1] / t1;
    z__[3] = y[2] / t1;
L300:
    return 0;
} /* unity_ */

/* Subroutine */ int vm_(real *y, real *x, real *z__)
{
    static real y1, y2, y3;

/*     TRANSPOSED VECTOR * MATRIX */
/*     Z(3)=Y(3)*X(3,3) */
    /* Parameter adjustments */
    --z__;
    x -= 4;
    --y;

    /* Function Body */
    y1 = y[1];
    y2 = y[2];
    y3 = y[3];
    z__[1] = x[4] * y1 + x[5] * y2 + x[6] * y3;
    z__[2] = x[7] * y1 + x[8] * y2 + x[9] * y3;
    z__[3] = x[10] * y1 + x[11] * y2 + x[12] * y3;
    return 0;
} /* vm_ */

doublereal vmv_(real *x1, real *q, real *x2)
{
    /* System generated locals */
    real ret_val;

/*     TRANSPOSED VECTOR * MATRIX * VECTOR */
/*     VMV=X1(3)*Q(3,3)*X2(3)    TO  EVALUATE QUADRATIC OR BILINEAR FORM */
    /* Parameter adjustments */
    --x2;
    q -= 4;
    --x1;

    /* Function Body */
    ret_val = x1[1] * (x2[1] * q[4] + x2[2] * q[7] + x2[3] * q[10]) + x1[2] * 
        (x2[1] * q[5] + x2[2] * q[8] + x2[3] * q[11]) + x1[3] * (x2[1] * 
        q[6] + x2[2] * q[9] + x2[3] * q[12]);
    return ret_val;
} /* vmv_ */

doublereal vv_(real *x, real *y)
{
    /* System generated locals */
    real ret_val;

/*     TRANSPOSED VECTOR * VECTOR */
/*     VV=X(3)*Y(3) */
    /* Parameter adjustments */
    --y;
    --x;

    /* Function Body */
    ret_val = x[1] * y[1] + x[2] * y[2] + x[3] * y[3];
    return ret_val;
} /* vv_ */

/* Subroutine */ int xyz_(doublereal *dqa, real *x, integer *itype)
{
    static integer i__, j;
    static real t1, t2;
    static integer it, ng1;
    extern /* Subroutine */ int atom_(doublereal *, real *);

/*     ***** ITYPE .GT.0 CART. COORD. FROM ATOM CODE WORD ***** */
/*     ***** XABSF(ITYPE) .LE.2 FOR WORKING SYSTEM ***** */
/*     ***** XABSF(ITYPE) .GT.2 FOR REFERENCE SYSTEM ***** */
/*     ***** ITYPE .LE.0 USES TRICLINIC COORD. XT ***** */
    /* Parameter adjustments */
    --x;

    /* Function Body */
    it = abs(*itype) - 2;
    ng1 = _BLNK__1.ng;
    _BLNK__1.ng = 0;
    if (*itype <= 0) {
    goto L10;
    } else {
    goto L5;
    }
L5:
    atom_(dqa, _BLNK__1.xt);
    if (_BLNK__1.ng != 0) {
    goto L30;
    } else {
    goto L10;
    }
L10:
    t1 = 0.f;
    for (j = 1; j <= 3; ++j) {
    t2 = _BLNK__1.xt[j - 1] - _BLNK__1.orgn[j - 1];
    _BLNK__1.v1[j - 1] = t2;
/* L15: */
    t1 += dabs(t2);
    }
    if (t1 - 1e-4f <= 0.f) {
    goto L20;
    } else {
    goto L40;
    }
L20:
    _BLNK__1.ng = ng1;
L30:
    for (j = 1; j <= 3; ++j) {
/* L35: */
    x[j] = 0.f;
    }
    goto L300;
L40:
    if (it <= 0) {
    goto L45;
    } else {
    goto L60;
    }
/*     ***** RELATIVE TO WORKING SYSTEM ***** */
L45:
    for (i__ = 1; i__ <= 3; ++i__) {
    t1 = 0.f;
    for (j = 1; j <= 3; ++j) {
/* L50: */
        t1 += _BLNK__1.v1[j - 1] * _BLNK__1.aawrk[j + i__ * 3 - 4];
    }
/* L55: */
    x[i__] = t1 * _BLNK__1.scal1;
    }
    goto L300;
/*     ***** RELATIVE TO REFERENCE SYSTEM ***** */
L60:
    for (i__ = 1; i__ <= 3; ++i__) {
    t1 = 0.f;
    for (j = 1; j <= 3; ++j) {
/* L65: */
        t1 += _BLNK__1.v1[j - 1] * _BLNK__1.aarev[j + i__ * 3 - 4];
    }
/* L70: */
    x[i__] = t1 * _BLNK__1.scal1;
    }
L300:
    return 0;
} /* xyz_ */

/* ***************************************************************** */
/* *** DUMMY SCREEN OUTPUT (MAY BE REPLACED WITH SCREEN DRIVER CODE) */
/* ***************************************************************** */

/* ----------------------------------------------------------------- */
// :- cairotep adapts these functions to utilize cairo as the plotting backend:
/* ----------------------------------------------------------------- */

/* Subroutine */ int initsc_(void)
{
    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: CT Init.\n");
    // default to 10.5, 8.0, 0.5:
    printf("Debug: (W: %f, H: %f, B: %f).\n", _BLNK__1.xlng[0], _BLNK__1.xlng[1], _BLNK__1.brdr);
    #endif

    /*
    // if both cw ch are set, change stage size to:
    // note that the stage size could be changed by further 301 instructions.
    if(crtep_cw > 0 && crtep_ch > 0) {
        _BLNK__1.brdr = (crtep_cw > crtep_ch ? crtep_ch : crtep_cw)*.1;
        _BLNK__1.xlng[0] = crtep_cw - _BLNK__1.brdr;
        _BLNK__1.xlng[1] = crtep_ch - _BLNK__1.brdr;
    }
    */

    // initialize canvas size:
    // use 301 before 201, or canvas is the original stage size (mismatch).
    integer w = (crtep_cw > 0 ? crtep_cw : (crtep_cw = _BLNK__1.xlng[0] + _BLNK__1.brdr)) * crtep_dpi;
    integer h = (crtep_ch > 0 ? crtep_ch : (crtep_ch = _BLNK__1.xlng[1] + _BLNK__1.brdr)) * crtep_dpi;

    if(crtep_scale > 0) {
        w = w*crtep_scale/100.;
        h = h*crtep_scale/100.;
    }

    if(crtep_nmode == 0) { // find a output file name.
        crtep_outfname = "TEP000.PNG";
        static char buf[11];
        FILE* f = NULL;
        int i;
        for(i=1; i<=999; i++) {
            sprintf(buf, "TEP%03d.PNG", i);
            f = fopen(buf, "r");
            if(f == NULL) {
                crtep_outfname = buf;
                break;
            }
        }
        printf(" Save PNG file to:  %s\n", crtep_outfname);
    }

    #ifdef CRTEP_DEBUG_TRACE
    crtep_ct = clock();
    #endif

    switch(crtep_ntype) {
        case 0: // PNG, image buffer
            //if(!crtep_outfname || strlen(crtep_outfname) == 0) crtep_outfname = strcat(strcpy(malloc(strlen(crtep_infname)+5), crtep_infname), ".png");
            if(crtep_bg < 0) { // with transparent support.
                crtep_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
            } else {
                crtep_surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h);
            }
            break;
        case 1: // SVG
            //if(!crtep_outfname || strlen(crtep_outfname) == 0) crtep_outfname = strcat(strcpy(malloc(strlen(crtep_infname)+5), crtep_infname), ".svg");
            crtep_surface = cairo_svg_surface_create(crtep_outfname, w, h);
            break;
        case 2: // PDF
            //if(!crtep_outfname || strlen(crtep_outfname) == 0) crtep_outfname = strcat(strcpy(malloc(strlen(crtep_infname)+5), crtep_infname), ".pdf");
            crtep_surface = cairo_pdf_surface_create(crtep_outfname, w, h);
            break;
        case 3: // PS
            //if(!crtep_outfname || strlen(crtep_outfname) == 0) crtep_outfname = strcat(strcpy(malloc(strlen(crtep_infname)+5), crtep_infname), ".ps");
            crtep_surface = cairo_ps_surface_create(crtep_outfname, w, h);
			//cairo_ps_surface_restrict_to_level(crtep_surface, CAIRO_PS_LEVEL_2);
            break;
        case 4: // EPS
            //if(!crtep_outfname || strlen(crtep_outfname) == 0) crtep_outfname = strcat(strcpy(malloc(strlen(crtep_infname)+5), crtep_infname), ".ps");
            crtep_surface = cairo_ps_surface_create(crtep_outfname, w, h);
			//cairo_ps_surface_restrict_to_level(crtep_surface, CAIRO_PS_LEVEL_2);
			cairo_ps_surface_set_eps(crtep_surface, 1);
            break;
        default:
            printf("Cairo X.\n");
            exit(1);
    }
    if(cairo_surface_status(crtep_surface) != CAIRO_STATUS_SUCCESS) {
        printf("Can't Create Cairo Surface.\n");
        exit(1);
    }
    crtep_cr = cairo_create(crtep_surface);

    if(crtep_scale > 0) { // set canvas scale.
        cairo_scale(crtep_cr, crtep_scale/100., crtep_scale/100.);
    }

    if(crtep_pw > 0) { // set initial pen width.
        cairo_set_line_width(crtep_cr, crtep_pw);
    } else { // default to 1.
        cairo_set_line_width(crtep_cr, 1);
    }

    if(0 <= crtep_bg) { // paint background.
        if(crtep_bg <= 0xFFFFFF) {
            integer r, g, b;
            r = crtep_bg >> 16;
            g = crtep_bg >> 8 & 0xFF;
            b = crtep_bg & 0xFF;
            cairo_set_source_rgb(crtep_cr, r/255., g/255., b/255.);
        } else {
            cairo_set_source_rgb(crtep_cr, 1, 1, 1); // white.
        }
        cairo_paint(crtep_cr);
    } // else transparent.
    if(0 <= crtep_fg && crtep_fg <= 0xFFFFFF) { // set initial foreground.
        integer r, g, b;
        r = crtep_fg >> 16;
        g = crtep_fg >> 8 & 0xFF;
        b = crtep_fg & 0xFF;
        cairo_set_source_rgb(crtep_cr, r/255., g/255., b/255.);
    } else { // default to blacK.
        cairo_set_source_rgb(crtep_cr, 0, 0, 0);
    }

    return 0;
} /* initsc_ */

/* Subroutine */ int penwsc_(real *penw)
{
    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: CT Pen Width (%f).\n", *penw);
    #endif
    cairo_stroke(crtep_cr);
    cairo_set_line_width(crtep_cr, *penw);
    return 0;
} /* penwsc_ */

/* Subroutine */ int colrsc_(integer *icolor)
{
    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: CT Pen Color (%d).\n", *icolor);
    #endif
    cairo_stroke(crtep_cr);
    integer c = *icolor; // C (long int).
    if(c < 0) {
        integer r, g, b;
        c = -c;
        if(c <= 0xFFFFFF) {
            r = c >> 16;
            g = c >> 8 & 0xFF;
            b = c & 0xFF;
            cairo_set_source_rgb(crtep_cr, r/255., g/255., b/255.);
        } else {
            printf("Bad 24-Bit Color Value, Use Black.\n");
            cairo_set_source_rgb(crtep_cr, 0, 0, 0);
        }
    } else if(c < 8) {
        switch(c) {
            case 0:
            case 1: // blacK
                cairo_set_source_rgb(crtep_cr, 0, 0, 0);
                break;
            case 2: // Red
                cairo_set_source_rgb(crtep_cr, 1, 0, 0);
                //cairo_set_source_rgb(crtep_cr, 0x99/255., 0, 0);
                break;
            case 3: // Green
                cairo_set_source_rgb(crtep_cr, 0, 1, 0);
                //cairo_set_source_rgb(crtep_cr, 0, 0x99/255., 0);
                break;
            case 4: // Blue
                cairo_set_source_rgb(crtep_cr, 0, 0, 1);
                //cairo_set_source_rgb(crtep_cr, 0, 0, 0x99/255.);
                break;
            case 5: // Cyan
                cairo_set_source_rgb(crtep_cr, 0, 1, 1);
                //cairo_set_source_rgb(crtep_cr, 0x33/255., 0x66/255., 0x99/255.);
                break;
            case 6: // Magenta
                cairo_set_source_rgb(crtep_cr, 1, 0, 1);
                //cairo_set_source_rgb(crtep_cr, 1, 0x33/255., 0x66/255.);
                break;
            case 7: // Yellow
                cairo_set_source_rgb(crtep_cr, 1, 1, 0);
                //cairo_set_source_rgb(crtep_cr, 1, 0x99/255., 0);
                break;
            default:
                cairo_set_source_rgb(crtep_cr, 0, 0, 0); // dead code.
        }
    } else {
        printf("Bad Color Index, Use Black.\n");
        cairo_set_source_rgb(crtep_cr, 0, 0, 0);
    }
    return 0;
} /* colrsc_ */

/* Subroutine */ int pensc_(real *x, real *y, integer *ipen)
{
    #ifdef CRTEP_DEBUG_TRACE
    //printf("Debug: CT Pen (%f, %f, %d).\n", *x, *y, *ipen);
    #endif
    if(*ipen == 2) { // pen down
        cairo_line_to(crtep_cr, crtep_dpi*(*x+trfac_1.xtrans), crtep_dpi*(crtep_ch-*y-trfac_1.ytrans));
    } else if(*ipen == 3) { // pen up
        if(crtep_frag) cairo_stroke(crtep_cr);
        cairo_move_to(crtep_cr, crtep_dpi*(*x+trfac_1.xtrans), crtep_dpi*(crtep_ch-*y-trfac_1.ytrans));
    }
    return 0;
} /* pensc_ */

/* Subroutine */ int endsc_(void)
{
    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: CT End.\n");
    #endif

    cairo_stroke(crtep_cr);

    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: Time 1: %d ms.\n", clock()-crtep_ct);
    crtep_ct = clock();
    #endif

    // Write output and clean up:
    if(crtep_ntype == 0) cairo_surface_write_to_png(crtep_surface, crtep_outfname);
    cairo_destroy(crtep_cr);
    cairo_surface_destroy(crtep_surface);

    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: Time 2: %d ms.\n", clock()-crtep_ct);
    #endif

    return 0;
} /* endsc_ */

// --

/* *** end of dummy screen output */
/* **************************************************************** */
/* **************************************************************** */
/* *** PGPLOT CODE FOR SCREEN OUTPUT */
/* *** if PGPLOT is implemented, use the subroutines here */
/* *** instead of the ones in the DUMMY SCREEN OUTPUT section */
/*     PGPLOT is a free graphics library developed by T. J. Pearson at */
/*     the California Institute of Technology. Information about PGPLOT */
/*     can be found on the World Wide Web at */
/*     http://astro.caltech.edu/~tjp/pgplot */
/*     or via e-mail to tjp@astro.caltech.edu. */
/* **************************************************************** */
/*     subroutine initsc */
/*     character*10 outdev */
/*     common /ns/ npf,ndraw,norient,nvar */
/*     integer pgbeg */

/*     xwid=11. */
/*     yhgt=8.5 */

/* *** The following is for PGPLOT on an X-windows system. */
/*     outdev = '/XWINDOW' */
/* *** The following is for PGPLOT on an MS-DOS system. */
/*     outdev = '/MS' */
/* *** The following is for PGPLOT on a Macintosh system. */
/*     outdev = '/MAC' */
/*     open(npf,status='scratch') */
/*     if (pgbeg(0,' ',1,1) .ne. 1) call exitng(8) */
/*     switch black and white */
/*     call pgscr(0,1.,1.,1.) */
/*     call pgscr(1,0.,0.,0.) */
/*     set up drawing window */
/*     call pgpage */
/*     call pgqch(osize) */
/*     call pgsch(0.) */
/*     call pgvstd */
/*     call pgwnad(0.,xwid,0.,yhgt) */
/*     call pgsch(osize) */
/*     call pgbox('BCT',1.,0,'BCT',1.,0) */
/*     call pgsci(1) */
/*     call pgsfs(2) */
/*     call pgrect(10.4,11.,8.2,8.5) */
/*     call pgtext(10.5,8.3,'EXIT') */
/*     return */
/*     end */
/*     subroutine colrsc(icolor) */
/* *** set plot color */
/* *** in ORTEP icolor=0 => black */
/* *** PGPLOT is set up for 1=black */
/*     common /ns/ npf,ndraw,norient,nvar */
/*     icol=icolor */
/*     if (icol.eq.0) icol=1 */
/*     nvar=icol */
/*     if (ndraw.eq.1) call pgsci(icol) */
/*     if (ndraw.eq.9) write (npf,111) icol */
/* 111 format('COL',1x,i2) */
/*     return */
/*     end */
/*     subroutine penwsc(penw) */
/* *** change pen width */
/* *** PGPLOT measures pen width in 200ths of an inch */
/*     common /ns/ npf,ndraw,norient,nvar */
/*     ipenw=nint(.001*penw*200.) */
/*     if (ipenw.le.0) ipenw=1 */
/*     if (ipenw.gt.200) ipenw=200 */
/*     if (ndraw.eq.1) call pgslw(ipenw) */
/*     if (ndraw.eq.9) write (npf,111) ipenw */
/* 111 format('WID',1x,i3) */
/*     return */
/*     end */
/*     subroutine pensc(x,y,ipen) */
/* *** move the pen */
/*     common /trfac/ xtrans,ytrans */
/*     common /ns/ npf,ndraw,norient,nvar */
/*     if (ipen.eq.2) then */
/*        if (ndraw.eq.1) call pgdraw(x+xtrans,y+ytrans) */
/*        if (ndraw.eq.9) write (npf,111) x+xtrans,y+ytrans */
/* 111    format('LIN',2(1x,f10.6)) */
/*     end if */
/*     if (ipen.eq.3) then */
/*        if (ndraw.eq.1) call pgmove(x+xtrans,y+ytrans) */
/*        if (ndraw.eq.9) write (npf,112) x+xtrans,y+ytrans */
/* 112    format('MOV',2(1x,f10.6)) */
/*     end if */
/*     return */
/*     end */
/*     subroutine endsc */
/*     common /ns/ npf,ndraw,norient,nvar */
/*     call curssc */
/*     close(npf) */
/* *** tell user to hit <enter> key */
/*     call pgsci(0) */
/*     call pgsfs(1) */
/*     call pgrect(7.5,11.,8.2,8.5) */
/*     call pgsci(1) */
/*     call pgsfs(2) */
/*     call pgrect(7.5,11.,8.2,8.5) */
/*     call pgsci(1) */
/*     call pgtext(7.6,8.3,'Hit <RETURN> or <ENTER> key') */
/*     call pgend */
/*     return */
/*     end */
/*     subroutine curssc */
/* *** correlate screen cursor position with atom positions and display results */
/*     character ch */
/*     character*21 str */
/*     integer pgcurs */
/*     character*6 label,alabel */
/*     character*9 tomid,atomid */
/*     common /trfac/ xtrans,ytrans */
/*     common /ns/ npf,ndraw,norient,nvar */
/*     call pgsfs(1) */
/*     call pgscf(1) */
/*     call pgsch(1.) */
/* *** get cursor position */
/* 1   junk = pgcurs(x,y,ch) */
/*     if (ch.eq.'x' .or. ch.eq.'X') return */
/*     if (ch.eq.'d' .or. ch.eq.'D') return */
/*     if (x.ge.10.4 .and. x.le.11. .and. y.ge.8.2 .and. y.le.8.5) return */
/*     if (ichar(ch).eq.13) return */
/* *** initial values for variables */
/*     xpt = x */
/*     ypt = y */
/*     adiffx = .0625 */
/*     adiffy = .0625 */
/*     odiffx = adiffx */
/*     odiffy = adiffy */
/*     atomid = '         ' */
/*     alabel = '      ' */
/*     iflag = 0 */
/*     nflag = 0 */
/*     rewind(npf) */
/* 2   read(npf,3,end=4) label,tomid,xx,yy */
/* 3   format(11x,a6,3x,a9,4x,2f8.0) */
/*     diffx = abs(xx-xpt) */
/*     diffy = abs(yy-ypt) */
/*     if (diffx.le.adiffx .and. diffy.le.adiffy) nflag=nflag+1 */
/*     if (diffx.le.odiffx .and. diffy.le.odiffy) then */
/*        atomid = tomid */
/*        alabel = label */
/*        odiffx = diffx */
/*        odiffy = diffy */
/*     end if */
/*     go to 2 */
/* 4   if (nflag.eq.0) write(str,5) */
/*     if (nflag.eq.1) write(str,6) alabel,atomid */
/*     if (nflag.gt.1) write(str,7) alabel,atomid */
/* 5   format('Not near atom center') */
/* 6   format(a6,1x,a9) */
/* 7   format(a6,1x,a9,' + ??') */
/* *** erase rectangle */
/*     call pgsci(0) */
/*     call pgsfs(1) */
/*     call pgrect(0.,2.8,8.2,8.5) */
/* *** redraw empty rectangle */
/*     call pgsci(1) */
/*     call pgsfs(2) */
/*     call pgrect(0.,2.8,8.2,8.5) */
/* *** print atom information in rectangle */
/*     call pgtext(0.1,8.3,str) */
/*     go to 1 */
/*     end */
/* *** end of PGPLOT specific routines */
/* **************************************************************** */
/* **************************************************************** */
/* *** HPGL FILE OUTPUT */
/* **************************************************************** */
/* Subroutine */ int inithp_(void)
{
    /* Format strings */
    static char fmt_10[] = "(\002TEP\002,i3.3,\002.PRN\002)";
    static char fmt_13[] = "(/,\002 HPGL file name:  \002,a)";
    static char fmt_21[] = "(a1,\002E\002)";
    static char fmt_22[] = "(a1,\002%0B\002)";
    static char fmt_23[] = "(\002IN;\002/\002SP1;\002/\002PW.15;\002)";
    static char fmt_24[] = "(\002RO90.;\002)";

    /* System generated locals */
    integer i__1;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
        , f_open(olist *), f_clos(cllist *), s_wsfe(cilist *), e_wsfe(
        void);

    /* Local variables */
    static integer i__;
    static char esc[1], fname[10];

    /* Fortran I/O blocks */
    static icilist io___941 = { 0, fname, 0, fmt_10, 10, 1 };
    static cilist io___942 = { 0, 6, 0, fmt_13, 0 };
    static cilist io___944 = { 0, 0, 0, fmt_21, 0 };
    static cilist io___945 = { 0, 0, 0, fmt_22, 0 };
    static cilist io___946 = { 0, 0, 0, fmt_23, 0 };
    static cilist io___947 = { 0, 0, 0, fmt_24, 0 };


// :-
    if(crtep_nmode == 0) {
// --
    for (i__ = 1; i__ <= 999; ++i__) {
    s_wsfi(&io___941);
    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
    e_wsfi();
    o__1.oerr = 1;
    o__1.ounit = ns_1.npf;
    o__1.ofnmlen = 10;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "old";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
        goto L12;
    }
    cl__1.cerr = 0;
    cl__1.cunit = ns_1.npf;
    cl__1.csta = 0;
    f_clos(&cl__1);
/* L11: */
    }
L12:
    o__1.oerr = 0;
    o__1.ounit = ns_1.npf;
    o__1.ofnmlen = 10;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    s_wsfe(&io___942);
    do_fio(&c__1, fname, (ftnlen)10);
    e_wsfe();
// :-
    } else {
    o__1.oerr = 0;
    o__1.ounit = ns_1.npf;
    o__1.ofnmlen = strlen(crtep_outfname);
    o__1.ofnm = crtep_outfname;
    o__1.orl = 0;
    o__1.osta = "replace";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    }
// --
    *(unsigned char *)esc = '\33';
    io___944.ciunit = ns_1.npf;
    s_wsfe(&io___944);
    do_fio(&c__1, esc, (ftnlen)1);
    e_wsfe();
    io___945.ciunit = ns_1.npf;
    s_wsfe(&io___945);
    do_fio(&c__1, esc, (ftnlen)1);
    e_wsfe();
    io___946.ciunit = ns_1.npf;
    s_wsfe(&io___946);
    e_wsfe();
    if (ns_1.norient == 2) {
    io___947.ciunit = ns_1.npf;
    s_wsfe(&io___947);
    e_wsfe();
    }

    // :-
    #ifdef CRTEP_DEBUG_TRACE
    crtep_ct = clock();
    #endif
    // --

    return 0;
} /* inithp_ */

/* Subroutine */ int colrhp_(integer *icolor)
{
    /* Format strings */
    static char fmt_21[] = "(\002SP\002,i1,\002;\002)";

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Local variables */
    static integer icol;

    /* Fortran I/O blocks */
    static cilist io___949 = { 0, 0, 0, fmt_21, 0 };


/* *** set plot color */
/* *** in ORTEP icolor=0 => black */
/* *** plotter pen 1=black */
    icol = *icolor;
    if (icol == 0) {
    icol = 1;
    }
    io___949.ciunit = ns_1.npf;
    s_wsfe(&io___949);
    do_fio(&c__1, (char *)&icol, (ftnlen)sizeof(integer));
    e_wsfe();
    return 0;
} /* colrhp_ */

/* Subroutine */ int penwhp_(real *penw)
{
    /* Format strings */
    static char fmt_21[] = "(\002PW\002,f5.2,\002;\002)";

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Fortran I/O blocks */
    static cilist io___950 = { 0, 0, 0, fmt_21, 0 };


    if (*penw == 0.f) {
    *penw = .15f;
    } else {
    *penw *= .0252f;
    }
    io___950.ciunit = ns_1.npf;
    s_wsfe(&io___950);
    do_fio(&c__1, (char *)&(*penw), (ftnlen)sizeof(real));
    e_wsfe();
    return 0;
} /* penwhp_ */

/* Subroutine */ int penhp_(real *x, real *y, integer *ipen)
{
    /* Format strings */
    static char fmt_101[] = "(\002PD\002,i4,\002,\002,i4,\002;\002)";
    static char fmt_102[] = "(\002PU\002,i4,\002,\002,i4,\002;\002)";

    /* System generated locals */
    real r__1;

    /* Builtin functions */
    integer i_nint(real *), s_wsfe(cilist *), do_fio(integer *, char *, 
        ftnlen), e_wsfe(void);

    /* Local variables */
    static integer ix, iy;

    /* Fortran I/O blocks */
    static cilist io___953 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___954 = { 0, 0, 0, fmt_102, 0 };


    r__1 = (*x + trfac_1.xtrans) * 1e3f;
    ix = i_nint(&r__1);
    r__1 = (*y + trfac_1.ytrans) * 1e3f;
    iy = i_nint(&r__1);
    if (*ipen == 2) {
    io___953.ciunit = ns_1.npf;
    s_wsfe(&io___953);
    do_fio(&c__1, (char *)&ix, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iy, (ftnlen)sizeof(integer));
    e_wsfe();
    }
    if (*ipen == 3) {
    io___954.ciunit = ns_1.npf;
    s_wsfe(&io___954);
    do_fio(&c__1, (char *)&ix, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iy, (ftnlen)sizeof(integer));
    e_wsfe();
    }
    return 0;
} /* penhp_ */

/* Subroutine */ int endhp_(void)
{
    /* Format strings */
    static char fmt_31[] = "(\002PU;\002,/,\002SP0;\002,/,\002PG;\002,/,\002"
        "IN;\002)";
    static char fmt_34[] = "(a1,\002%0A\002)";
    static char fmt_35[] = "(a1,\002E\002)";

    /* System generated locals */
    cllist cl__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen),
         f_clos(cllist *);

    /* Local variables */
    static char esc[1];

    /* Fortran I/O blocks */
    static cilist io___956 = { 0, 0, 0, fmt_31, 0 };
    static cilist io___957 = { 0, 0, 0, fmt_34, 0 };
    static cilist io___958 = { 0, 0, 0, fmt_35, 0 };


    *(unsigned char *)esc = '\33';
    io___956.ciunit = ns_1.npf;
    s_wsfe(&io___956);
    e_wsfe();
    io___957.ciunit = ns_1.npf;
    s_wsfe(&io___957);
    do_fio(&c__1, esc, (ftnlen)1);
    e_wsfe();
    io___958.ciunit = ns_1.npf;
    s_wsfe(&io___958);
    do_fio(&c__1, esc, (ftnlen)1);
    e_wsfe();
    cl__1.cerr = 0;
    cl__1.cunit = ns_1.npf;
    cl__1.csta = 0;
    f_clos(&cl__1);

    // :-
    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: Time 1: %d ms.\n", clock()-crtep_ct);
    #endif
    // --

    return 0;
} /* endhp_ */

/* *** end of HPGL specific routines */
/* **************************************************************** */
/* **************************************************************** */
/* *** POSTSCRIPT FILE OUTPUT */
/* **************************************************************** */
/* Subroutine */ int initps_(void)
{
    /* Format strings */
    static char fmt_10[] = "(\002TEP\002,i3.3,\002.PRN\002)";
    static char fmt_13[] = "(/,\002 Postscript file name:  \002,a)";
    static char fmt_21[] = "(\002%!PS-Adobe-3.0 EPSF-3.0\002)";
    static char fmt_22[] = "(\002%%Creator: ORTEP-III Version 1.0.3 Jan. 31,"
        " 2000\002)";
    static char fmt_23[] = "(\002%%BoundingBox: (atend)\002,/,\002%%Pages: "
        "1\002)";
    static char fmt_24[] = "(\002%%Orientation: Landscape\002)";
    static char fmt_25[] = "(\002%%Orientation: Portrait\002)";
    static char fmt_26[] = "(\002%%BeginProlog\002)";
    static char fmt_27[] = "(\002/m {moveto} def\002)";
    static char fmt_28[] = "(\002/l {lineto} def\002)";
    static char fmt_29[] = "(\002%%EndProlog\002,/,\002%%Page: 1 1\002)";
    static char fmt_30[] = "(\002%%BeginPageSetup\002)";
    static char fmt_31[] = "(\0020.072 0.072 scale\002)";
    static char fmt_32[] = "(i6,1x,i6,\002 translate\002)";
    static char fmt_33[] = "(\002-90 rotate\002)";
    static char fmt_34[] = "(\0020 setgray 1 setlinecap 5 setlinewidth\002)";
    static char fmt_35[] = "(\002%%EndPageSetup\002)";

    /* System generated locals */
    integer i__1;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    integer s_wsfi(icilist *), do_fio(integer *, char *, ftnlen), e_wsfi(void)
        , f_open(olist *), f_clos(cllist *), s_wsfe(cilist *), e_wsfe(
        void);

    /* Local variables */
    static integer i__;
    static char fname[10];

    /* Fortran I/O blocks */
    static icilist io___961 = { 0, fname, 0, fmt_10, 10, 1 };
    static cilist io___962 = { 0, 6, 0, fmt_13, 0 };
    static cilist io___963 = { 0, 0, 0, fmt_21, 0 };
    static cilist io___964 = { 0, 0, 0, fmt_22, 0 };
    static cilist io___965 = { 0, 0, 0, fmt_23, 0 };
    static cilist io___966 = { 0, 0, 0, fmt_24, 0 };
    static cilist io___967 = { 0, 0, 0, fmt_25, 0 };
    static cilist io___968 = { 0, 0, 0, fmt_26, 0 };
    static cilist io___969 = { 0, 0, 0, fmt_27, 0 };
    static cilist io___970 = { 0, 0, 0, fmt_28, 0 };
    static cilist io___971 = { 0, 0, 0, fmt_29, 0 };
    static cilist io___972 = { 0, 0, 0, fmt_30, 0 };
    static cilist io___973 = { 0, 0, 0, fmt_31, 0 };
    static cilist io___974 = { 0, 0, 0, fmt_32, 0 };
    static cilist io___975 = { 0, 0, 0, fmt_33, 0 };
    static cilist io___976 = { 0, 0, 0, fmt_34, 0 };
    static cilist io___977 = { 0, 0, 0, fmt_35, 0 };


/* *** initialize variables to calculate bounding box */
    ps_1.ixmin = 20000;
    ps_1.ixmax = 0;
    ps_1.iymin = 20000;
    ps_1.iymax = 0;
// :-
    if(crtep_nmode == 0) {
// --
    for (i__ = 1; i__ <= 999; ++i__) {
    s_wsfi(&io___961);
    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
    e_wsfi();
    o__1.oerr = 1;
    o__1.ounit = ns_1.npf;
    o__1.ofnmlen = 10;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "old";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
        goto L12;
    }
    cl__1.cerr = 0;
    cl__1.cunit = ns_1.npf;
    cl__1.csta = 0;
    f_clos(&cl__1);
/* L11: */
    }
L12:
    o__1.oerr = 0;
    o__1.ounit = ns_1.npf;
    o__1.ofnmlen = 10;
    o__1.ofnm = fname;
    o__1.orl = 0;
    o__1.osta = "new";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    s_wsfe(&io___962);
    do_fio(&c__1, fname, (ftnlen)10);
    e_wsfe();
// :-
    } else {
    o__1.oerr = 0;
    o__1.ounit = ns_1.npf;
    o__1.ofnmlen = strlen(crtep_outfname);
    o__1.ofnm = crtep_outfname;
    o__1.orl = 0;
    o__1.osta = "replace";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    }
// --
    ps_1.ixt = 0;
    ps_1.iyt = 0;
    io___963.ciunit = ns_1.npf;
    s_wsfe(&io___963);
    e_wsfe();
    io___964.ciunit = ns_1.npf;
    s_wsfe(&io___964);
    e_wsfe();
    io___965.ciunit = ns_1.npf;
    s_wsfe(&io___965);
    e_wsfe();
    if (ns_1.norient == 2) {
    io___966.ciunit = ns_1.npf;
    s_wsfe(&io___966);
    e_wsfe();
    ps_1.iyt = ns_1.nvar;
    } else {
    io___967.ciunit = ns_1.npf;
    s_wsfe(&io___967);
    e_wsfe();
    }
    io___968.ciunit = ns_1.npf;
    s_wsfe(&io___968);
    e_wsfe();
    io___969.ciunit = ns_1.npf;
    s_wsfe(&io___969);
    e_wsfe();
    io___970.ciunit = ns_1.npf;
    s_wsfe(&io___970);
    e_wsfe();
    io___971.ciunit = ns_1.npf;
    s_wsfe(&io___971);
    e_wsfe();
    io___972.ciunit = ns_1.npf;
    s_wsfe(&io___972);
    e_wsfe();
    io___973.ciunit = ns_1.npf;
    s_wsfe(&io___973);
    e_wsfe();
    io___974.ciunit = ns_1.npf;
    s_wsfe(&io___974);
    do_fio(&c__1, (char *)&ps_1.ixt, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&ps_1.iyt, (ftnlen)sizeof(integer));
    e_wsfe();
    if (ns_1.norient == 2) {
    io___975.ciunit = ns_1.npf;
    s_wsfe(&io___975);
    e_wsfe();
    }
    io___976.ciunit = ns_1.npf;
    s_wsfe(&io___976);
    e_wsfe();
    io___977.ciunit = ns_1.npf;
    s_wsfe(&io___977);
    e_wsfe();

    // :-
    #ifdef CRTEP_DEBUG_TRACE
    crtep_ct = clock();
    #endif
    // --

    return 0;
} /* initps_ */

/* Subroutine */ int colrps_(integer *icolor)
{
    /* Format strings */
    static char fmt_101[] = "(\002stroke\002)";
    static char fmt_1[] = "(\0020 setgray\002)";
    static char fmt_2[] = "(\0021 0 0 setrgbcolor\002)";
    static char fmt_3[] = "(\0020 1 0 setrgbcolor\002)";
    static char fmt_4[] = "(\0020 0 1 setrgbcolor\002)";
    static char fmt_5[] = "(\0020 1 1 setrgbcolor\002)";
    static char fmt_6[] = "(\0021 0 1 setrgbcolor\002)";
    static char fmt_7[] = "(\0021 1 0 setrgbcolor\002)";

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void);

    /* Fortran I/O blocks */
    static cilist io___978 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___979 = { 0, 0, 0, fmt_1, 0 };
    static cilist io___980 = { 0, 0, 0, fmt_1, 0 };
    static cilist io___981 = { 0, 0, 0, fmt_2, 0 };
    static cilist io___982 = { 0, 0, 0, fmt_3, 0 };
    static cilist io___983 = { 0, 0, 0, fmt_4, 0 };
    static cilist io___984 = { 0, 0, 0, fmt_5, 0 };
    static cilist io___985 = { 0, 0, 0, fmt_6, 0 };
    static cilist io___986 = { 0, 0, 0, fmt_7, 0 };


    io___978.ciunit = ns_1.npf;
    s_wsfe(&io___978);
    e_wsfe();
    if (*icolor == 0) {
    io___979.ciunit = ns_1.npf;
    s_wsfe(&io___979);
    e_wsfe();
    }
    if (*icolor == 1) {
    io___980.ciunit = ns_1.npf;
    s_wsfe(&io___980);
    e_wsfe();
    }
    if (*icolor == 2) {
    io___981.ciunit = ns_1.npf;
    s_wsfe(&io___981);
    e_wsfe();
    }
    if (*icolor == 3) {
    io___982.ciunit = ns_1.npf;
    s_wsfe(&io___982);
    e_wsfe();
    }
    if (*icolor == 4) {
    io___983.ciunit = ns_1.npf;
    s_wsfe(&io___983);
    e_wsfe();
    }
    if (*icolor == 5) {
    io___984.ciunit = ns_1.npf;
    s_wsfe(&io___984);
    e_wsfe();
    }
    if (*icolor == 6) {
    io___985.ciunit = ns_1.npf;
    s_wsfe(&io___985);
    e_wsfe();
    }
    if (*icolor == 7) {
    io___986.ciunit = ns_1.npf;
    s_wsfe(&io___986);
    e_wsfe();
    }
    return 0;
} /* colrps_ */

/* Subroutine */ int penwps_(real *penw)
{
    /* Format strings */
    static char fmt_101[] = "(\002stroke\002)";
    static char fmt_102[] = "(f10.2,1x,\002setlinewidth\002)";

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);

    /* Fortran I/O blocks */
    static cilist io___987 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___988 = { 0, 0, 0, fmt_102, 0 };


    io___987.ciunit = ns_1.npf;
    s_wsfe(&io___987);
    e_wsfe();
    if (*penw == 0.f) {
    *penw = 5.f;
    }
    io___988.ciunit = ns_1.npf;
    s_wsfe(&io___988);
    do_fio(&c__1, (char *)&(*penw), (ftnlen)sizeof(real));
    e_wsfe();
    return 0;
} /* penwps_ */

/* Subroutine */ int penps_(real *x, real *y, integer *ipen)
{
    /* Format strings */
    static char fmt_101[] = "(i6,1x,i6,1x,\002l\002)";
    static char fmt_102[] = "(\002stroke\002/i6,1x,i6,1x,\002m\002)";

    /* System generated locals */
    real r__1;

    /* Builtin functions */
    integer i_nint(real *), s_wsfe(cilist *), do_fio(integer *, char *, 
        ftnlen), e_wsfe(void);

    /* Local variables */
    static integer ix, iy;

    /* Fortran I/O blocks */
    static cilist io___991 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___992 = { 0, 0, 0, fmt_102, 0 };


    r__1 = (*x + trfac_1.xtrans) * 1e3f;
    ix = i_nint(&r__1);
    r__1 = (*y + trfac_1.ytrans) * 1e3f;
    iy = i_nint(&r__1);
    if (*ipen == 2) {
    io___991.ciunit = ns_1.npf;
    s_wsfe(&io___991);
    do_fio(&c__1, (char *)&ix, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iy, (ftnlen)sizeof(integer));
    e_wsfe();
    }
    if (*ipen == 3) {
    io___992.ciunit = ns_1.npf;
    s_wsfe(&io___992);
    do_fio(&c__1, (char *)&ix, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iy, (ftnlen)sizeof(integer));
    e_wsfe();
    }
/* *** variables to calculate bounding box */
    if (ix < ps_1.ixmin) {
    ps_1.ixmin = ix;
    }
    if (ix > ps_1.ixmax) {
    ps_1.ixmax = ix;
    }
    if (iy < ps_1.iymin) {
    ps_1.iymin = iy;
    }
    if (iy > ps_1.iymax) {
    ps_1.iymax = iy;
    }
    return 0;
} /* penps_ */

/* Subroutine */ int endps_(void)
{
    /* Format strings */
    static char fmt_25[] = "(\002stroke\002/\002showpage\002)";
    static char fmt_26[] = "(\002%%BoundingBox: \002,4(i6,1x))";
    static char fmt_27[] = "(\002%%Trailer\002/\002%%EOF\002)";

    /* System generated locals */
    cllist cl__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen),
         f_clos(cllist *);

    /* Local variables */
    static integer ixmn, iymn, ixmx, iymx;

    /* Fortran I/O blocks */
    static cilist io___993 = { 0, 0, 0, fmt_25, 0 };
    static cilist io___998 = { 0, 0, 0, fmt_26, 0 };
    static cilist io___999 = { 0, 0, 0, fmt_27, 0 };


    io___993.ciunit = ns_1.npf;
    s_wsfe(&io___993);
    e_wsfe();
/* *** calculate bounding box */
    if (ns_1.norient == 1) {
    ixmn = (real) (ps_1.ixmin + ps_1.ixt) * .072f - 2;
    iymn = (real) (ps_1.iymin + ps_1.iyt) * .072f - 2;
    ixmx = (real) (ps_1.ixmax + ps_1.ixt) * .072f + 2;
    iymx = (real) (ps_1.iymax + ps_1.iyt) * .072f + 2;
    } else {
    ixmn = (real) (ps_1.ixt + ps_1.iymin) * .072f - 2;
    iymn = (real) (ps_1.iyt - ps_1.ixmax) * .072f - 2;
    ixmx = (real) (ps_1.ixt + ps_1.iymax) * .072f + 2;
    iymx = (real) (ps_1.iyt - ps_1.ixmin) * .072f + 2;
    }
    if (ixmn < 0) {
    ixmn = 0;
    }
    if (iymn < 0) {
    iymn = 0;
    }
/* *** put bounding box at end of postscript file */
    io___998.ciunit = ns_1.npf;
    s_wsfe(&io___998);
    do_fio(&c__1, (char *)&ixmn, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iymn, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&ixmx, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iymx, (ftnlen)sizeof(integer));
    e_wsfe();
    io___999.ciunit = ns_1.npf;
    s_wsfe(&io___999);
    e_wsfe();
    cl__1.cerr = 0;
    cl__1.cunit = ns_1.npf;
    cl__1.csta = 0;
    f_clos(&cl__1);

    // :-
    #ifdef CRTEP_DEBUG_TRACE
    printf("Debug: Time 1: %d ms.\n", clock()-crtep_ct);
    #endif
    // --

    return 0;
} /* endps_ */

/* *** end of Postscript specific routines */
/* **************************************************************** */
/* ****************************************************************** */
/* *** DEFAULT VALUES FOR ORTEP INPUT AND OUTPUT OPTIONS ARE SET HERE */
/* *** USED IN SUBROUTINE UINPUT */
/* ****************************************************************** */
/* Subroutine */ int dflts_(void)
{
    /* Builtin functions */
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);

/* *** name of default input file */
    s_copy(dfl_1.infile__, "TEP.IN", (ftnlen)60, (ftnlen)6);
/* *** where ortep drawing output should go */
/* *** 1: Screen, 2: Postscript file, 3: HPGL file */
    dfl_1.idraw = 1;
/* *** orientation of drawing */
/* *** 1: portrait, 2: landscape */
    dfl_1.iorient = 1;
/* *** height of page */
    dfl_1.fpaplen = 11.f;
/* *** where ortep text output should go */
/* *** 1: file, 2: screen, 0: omit */
    dfl_1.iout = 0;
/* *** text output filename extension */
    s_copy(dfl_1.ext, ".out", (ftnlen)4, (ftnlen)4);
/* *** default name of external atom parameter file */
    s_copy(dfl_1.atomfi, "ATOMS.DAT", (ftnlen)60, (ftnlen)9);
    return 0;
} /* dflts_ */

/* Main program alias */ int ortep_ () { MAIN__ (); return 0; }
