#ifdef _CRAYT3E

typedef short           EVDW_int32 ;
#define MPI_EVDW_INT32  MPI_SHORT
#define EVDW_MAXINT32   MAXSHORT

#else

typedef int             EVDW_int32 ;
#define MPI_EVDW_INT32  MPI_INT
#define EVDW_MAXINT32   MAXINT

#endif


#define EVDW_FNMAX    1024  /* Maximum number of characters in file names. */
#define EVDW_Nlevel_max 10  /* Maximum number of multigrid levels.         */
#define EVDW_Neqn_max    7  /* Maximum number of governing equations.      */
#define EVDW_Nstage_max  6  /* Maximum number of Runge-Kutta stages.       */


typedef struct
{
  char   grid_file[EVDW_FNMAX] ;         /* File which contains the primary grid.     */
  char   sol_file[EVDW_FNMAX] ;          /* File for the final solution.              */
  char   bc_file[EVDW_FNMAX] ;           /* File with the BC's of the patches.        */
  char   conv_file[EVDW_FNMAX] ;         /* File to which the convergence history     */
                                         /* must be written.                          */
  char   conv_file_digress[EVDW_FNMAX] ; /* Name of the digress convergence file.     */
  char   tec_file[EVDW_FNMAX] ;          /* File, which contains the solution on      */
                                         /* the boundaries, readable by TECPLOT.      */
  char   par_grid_file[EVDW_FNMAX] ;     /* Base name grid file for visualization     */
  char   par_sol_file[EVDW_FNMAX] ;      /* Base name sol file for visualization.     */
  char   par_comm_file[EVDW_FNMAX] ;     /* Base name comm file for visualization     */

  char   sep_lines_file[EVDW_FNMAX] ;    /* File which contains the information       */
                                         /* concerning the separation lines.          */
  char   rigid_body_motion[EVDW_FNMAX] ; /* File which describes a rigid body motion. */

  EVDW_int32  Ntime_steps ;              /* Number of unsteady time steps. */
  EVDW_int32  Niter ;                    /* Number of multigrid cycles.    */
  EVDW_int32  Niter_coarse ;             /* Same, but on the coarse grids. */
  EVDW_int32  Nsave ;                    /* Save after every # iteration.  */
  EVDW_int32  Nlevel ;                   /* Number of multi-grid levels.   */
  EVDW_int32  Nstage ;                   /* Number of Runge-Kutta stages.  */
  EVDW_int32  Nsmooth ;                  /* Number of residual smoothing   */
                                         /* iterations.                    */
  EVDW_int32  Nsmooth_corr ;             /* Number of smoothing iterations */
                                         /* for the multigrid corrections. */
  EVDW_int32  Nagglo ;                   /* Maximum number of cells, which */
                                         /* are agglomerated.              */
  EVDW_int32  NJacobi_GS_iter ;          /* Number of iterations for Jacobi*/
                                         /* and Gauss-Seidel smoothers.    */
  EVDW_int32  agglom_neigh_of_neigh ;    /* Indicator whether neighbors of */
                                         /* neihgbors must be agglomerated */
  EVDW_int32  agglo_algorithm ;          /* Which agglomeration algorithm. */
                                         /* 1: Isentropic per subdomain.   */
                                         /* 2: As 1, but with smoothing.   */
                                         /* 3: Greedy type, with control   */
                                         /*    over the aspect ratios.     */
                                         /* 4: As 1, but with smoothing.   */
                                         /* 5: Using ParMetis.             */
  EVDW_int32  smoother ;                 /* Which MG smoother.             */
                                         /* 1: Explicit Runge-Kutta.       */
                                         /* 2: Jacobi.                     */
                                         /* 3: Gauss-Seidel.               */
                                         /* 4: LU-SGS.                     */
                                         /* 5: LU-SGS prec. GMRES.         */
  EVDW_int32  divide_res ;               /* Indicator whether the residuals*/
                                         /* must be divided by the volume  */
                                         /* for the convergence criterion. */
  EVDW_int32  equation ;                 /* Which set of equations are     */
                                         /* Euler, NS or RANS.             */
  EVDW_int32  mode ;                     /* Which mode, steady, unsteady,  */
                                         /* unsteady moving grid.          */
  EVDW_int32  restart ;                  /* restart == 0: No restart.      */
                                         /* restart != 0: Restart.         */
  EVDW_int32  MG_cycle ;                 /* MG_cycle == 0: V-cycle.        */
                                         /* MG_cycle == 1: W-cycle.        */
  EVDW_int32  Full_MG ;                  /* Full_MG == 0: No full multigrid*/
                                         /* Full_MG == 1: Full multigrid.  */
  EVDW_int32  freeze ;                   /* freeze == TRUE : Freeze the    */
                                         /*         coarse grid far field. */
                                         /* freeze == FALSE : Don't do it. */
  EVDW_int32  check_density_pressure ;   /* TRUE: Check for negative values*/
                                         /* FALSE: Don't check it.         */
  EVDW_int32  discretization ;           /* Which discretization scheme.   */
  EVDW_int32  discretization_coarse ;    /* Idem, but for coarse grids.    */
  EVDW_int32  discretization_DC_fine ;   /* Idem, but for DC on fine grid. */
  EVDW_int32  partitioning ;             /* Which partitioning technique.  */
  EVDW_int32  riemann ;                  /* Which riemann solver.          */
  EVDW_int32  tessellation ;             /* Which tesselation to use.      */
  EVDW_int32  boundary_condition ;       /* Which boundary conditions.     */
  EVDW_int32  Nkrylov_spaces ;           /* # krylov spaces in krylov solv.*/
  EVDW_int32  Nnonlinear_krylov_spaces ; /* # nonlinear krylov spaces.     */
  EVDW_int32  storage_format ;           /* storage_format = 0: point.     */
                                         /* storage_format = 1: vector.    */
  EVDW_int32  par_visualization ;        /* 0: No parallel visualization.  */
                                         /* 1: parallel visualization.     */
  EVDW_int32  defect_correction ;        /* 0: No defect correction.       */
                                         /* 1: Use defect correction.      */
  EVDW_int32  NDC_iter ;                 /* # DC iterations per DC step.   */
  EVDW_int32  use_mass_matrix ;          /* 0: No mass matrix used.        */
                                         /* 1: Use mass matrix (unsteady). */
  EVDW_int32  inviscid_separation ;      /* 0: No inviscid separation.     */
                                         /* 1: Inviscid separation present */
  EVDW_int32  all_force_coefficients ;   /* 0: Not all coefficients.       */
                                         /* 1: Compute all coefficients.   */
  EVDW_int32  tecplot_format ;           /* 0: Ascii.                      */
                                         /* 1: Binary, default value.      */
  EVDW_int32  all_processors_IO ;        /* Whether or not all processors  */
                                         /* are do IO. Default yes.        */
  EVDW_int32  system_advection ;         /* MDHR system advection scheme.  */
  EVDW_int32  scalar_advection ;         /* MDHR scalar advection scheme.  */

  EVDW_int32  Nsmooth_down[EVDW_Nlevel_max] ;  /* Number of smoothing iterations */
                                               /* in coarsening or down phase.   */
  EVDW_int32  Nsmooth_up[EVDW_Nlevel_max] ;    /* Number of smoothing iterations */
                                               /* in refinement or up phase.     */

  double log_stop ;                 /* Log of L2 norm for which the   */
                                    /* computation can be stopped.    */
  double log_stop_coarse ;          /* Idem, but on the coarse grids. */
  double eps_smooth ;               /* Residual smoothing parameter.  */
  double CFL ;                      /* CFL-number.                    */
  double CFL_coarse ;               /* CFL-number on coarse grids.    */
  double Mach ;                     /* Free-stream Mach number.       */
  double Mach_coefficients ;        /* Mach number to be used for the */
                                    /* computation of the coefficients*/
  double alpha ;                    /* Angle of attack.               */
  double beta ;                     /* Side slip angle.               */
  double Reynolds ;                 /* Reynolds number per meter.     */
  double one_over_Reynolds ;        /* 1.0/Reynolds.                  */
  double Prandt_lam ;               /* Laminar Prandtl-number         */
  double Prandt_turb ;              /* Turbulent Prandtl-number       */
  double gamma ;                    /* Specific heat ratio.           */
  double U_inf[EVDW_Neqn_max] ;     /* Free-stream state vector.      */
  double x_ref[3] ;                 /* Moment reference point.        */
  double eta_RK[EVDW_Nstage_max] ;  /* Runge-Kutta coefficients.      */
  double cdis[EVDW_Nstage_max] ;    /* Dissipative coefficients.      */
  double threshold_cell_volume ;    /* Threshold value for multigrid. */
  double omega_prolongation ;       /* Underrelaxation parameter in   */
                                    /* the prolongation.              */
  double omega_restriction ;        /* Underrelaxation parameter in   */
                                    /* the restriction.               */
  double omega_DC ;                 /* Underrelaxation parameter in   */
                                    /* the Defect Correction.         */
  double Sref ;                     /* Reference surface for force    */
                                    /* and momentum coefficients.     */
  double Lref ;                     /* Reference length for momentum  */
                                    /* coefficient.                   */
  double Delta_t ;                  /* Unsteady time step.            */

  /* Control parameters for the different schemes. */

  double qfil_JST_MATD ;            /* qfil == 0.0. Standard 4th order.     */
                                    /* qfil != 0.0. Modified 4th order.     */
                                    /* JST and MATD schemes.                */
  double kappa2_JST_MATD ;          /* Parameter for 2nd order dissipation. */
                                    /* JST and MATD schemes.                */
  double kappa4_JST_MATD ;          /* Parameter for 4th order dissipation. */
                                    /* JST and MATD schemes. Depends on qfil*/
  double omega_pressure_switch ;    /* Parameter in the pressure switch.    */
                                    /* JST, MATD, CUSP and SLIP schemes.    */
  double kappa2_coarse_JST_MATD ;   /* Parameter for 2nd order dissipation  */
                                    /* for 1st order JST and MATD schemes.  */
  double eps_CUSP_SLIP_Loehner ;    /* Small number in the denominator when */
                                    /* limiting. CUSP, SLIP and             */
                                    /* upwind Loehner schemes.              */
  double M_limit_CUSP_SLIP ;        /* Threshold Mach number in the limiter.*/
                                    /* CUSP and SLIP schemes.               */
  double nu_limit_CUSP_SLIP ;       /* Threshold pressure switch value in   */
                                    /* the limiter. CUSP and SLIP schemes.  */
  double q_CUSP_SLIP ;              /* Exponent in the limiter; CUSP, SLIP. */
  double kappa4_CUSP_SLIP ;         /* Parameter for 4th order dissipation. */
                                    /* CUSP and SLIP schemes.               */
  double kappa2_SLIP ;              /* Parameter for 2nd order dissipation. */
                                    /* SLIP scheme.                         */
  double kappa_Loehner ;            /* Parameter in kappa-interpolation.    */
                                    /* Upwind Loehner scheme.               */
  double K_Venkat ;                 /* Parameter in Venkat's limiter.       */
                                    /* Upwind Venkat scheme.                */
} struct_Input_Param ;

