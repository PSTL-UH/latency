/* This file creates a derived datatype
   consisting of 92 elements, which can
   however handled by just three operations
   (by a smart MPI library). It is not invented,
   but extracted from a real, industrial application.
*/

#include "latency.h"
#include "struct_def.h"

MPI_Datatype LAT_92elements ()
{
  MPI_Datatype newtype;
  
  struct_Input_Param Input_Param;
  EVDW_int32   i ;
  int          block_lengths[92] ;
  MPI_Datatype typelist[92];
  MPI_Aint     displacements[92], base_address ;

  /* First construct the typelist. */

  for(i=0; i<11; i++)
   typelist[i] = MPI_CHAR ;
  for(i=11; i<54; i++)
   typelist[i] = MPI_EVDW_INT32 ;
  for(i=54; i<92; i++)
   typelist[i] = MPI_DOUBLE ;

  /* Construct the array block_lengths. */

  for(i=0; i<11; i++) block_lengths[i] = EVDW_FNMAX ;
  for(i=11; i<92; i++) block_lengths[i] = 1 ;

  block_lengths[52] = block_lengths[53] = EVDW_Nlevel_max ;
  block_lengths[68] = EVDW_Neqn_max ;
  block_lengths[69] = 3 ;
  block_lengths[70] = block_lengths[71] = EVDW_Nstage_max ;

  /* Construct the array displacements. */

  MPI_Address(&(Input_Param), &base_address) ;

  MPI_Address(Input_Param.grid_file,         &displacements[0]) ;
  MPI_Address(Input_Param.sol_file,          &displacements[1]) ;
  MPI_Address(Input_Param.bc_file,           &displacements[2]) ;
  MPI_Address(Input_Param.conv_file,         &displacements[3]) ;
  MPI_Address(Input_Param.conv_file_digress, &displacements[4]) ;
  MPI_Address(Input_Param.tec_file,          &displacements[5]) ;

  MPI_Address(Input_Param.par_grid_file, &displacements[6]) ;
  MPI_Address(Input_Param.par_sol_file,  &displacements[7]) ;
  MPI_Address(Input_Param.par_comm_file, &displacements[8]) ;

  MPI_Address(Input_Param.sep_lines_file,    &displacements[9]) ;
  MPI_Address(Input_Param.rigid_body_motion, &displacements[10]) ;

  MPI_Address(&(Input_Param.Ntime_steps),     &displacements[11]) ;
  MPI_Address(&(Input_Param.Niter),           &displacements[12]) ;
  MPI_Address(&(Input_Param.Niter_coarse),    &displacements[13]) ;
  MPI_Address(&(Input_Param.Nsave),           &displacements[14]) ;
  MPI_Address(&(Input_Param.Nlevel),          &displacements[15]) ;
  MPI_Address(&(Input_Param.Nstage),          &displacements[16]) ;
  MPI_Address(&(Input_Param.Nsmooth),         &displacements[17]) ;
  MPI_Address(&(Input_Param.Nsmooth_corr),    &displacements[18]) ;
  MPI_Address(&(Input_Param.Nagglo),          &displacements[19]) ;
  MPI_Address(&(Input_Param.NJacobi_GS_iter), &displacements[20]) ;

  MPI_Address(&(Input_Param.agglom_neigh_of_neigh),    &displacements[21]) ;
  MPI_Address(&(Input_Param.agglo_algorithm),          &displacements[22]) ;
  MPI_Address(&(Input_Param.smoother),                 &displacements[23]) ;
  MPI_Address(&(Input_Param.divide_res),               &displacements[24]) ;
  MPI_Address(&(Input_Param.equation),                 &displacements[25]) ;
  MPI_Address(&(Input_Param.mode),                     &displacements[26]) ;
  MPI_Address(&(Input_Param.restart),                  &displacements[27]) ;
  MPI_Address(&(Input_Param.MG_cycle),                 &displacements[28]) ;
  MPI_Address(&(Input_Param.Full_MG),                  &displacements[29]) ;
  MPI_Address(&(Input_Param.freeze),                   &displacements[30]) ;
  MPI_Address(&(Input_Param.check_density_pressure),   &displacements[31]) ;
  MPI_Address(&(Input_Param.discretization),           &displacements[32]) ;
  MPI_Address(&(Input_Param.discretization_coarse),    &displacements[33]) ;
  MPI_Address(&(Input_Param.discretization_DC_fine),   &displacements[34]) ;
  MPI_Address(&(Input_Param.partitioning),             &displacements[35]) ;
  MPI_Address(&(Input_Param.riemann),                  &displacements[36]) ;
  MPI_Address(&(Input_Param.tessellation),             &displacements[37]) ;
  MPI_Address(&(Input_Param.boundary_condition),       &displacements[38]) ;
  MPI_Address(&(Input_Param.Nkrylov_spaces),           &displacements[39]) ;
  MPI_Address(&(Input_Param.Nnonlinear_krylov_spaces), &displacements[40]) ;
  MPI_Address(&(Input_Param.storage_format),           &displacements[41]) ;
  MPI_Address(&(Input_Param.par_visualization),        &displacements[42]) ;
  MPI_Address(&(Input_Param.defect_correction),        &displacements[43]) ;
  MPI_Address(&(Input_Param.NDC_iter),                 &displacements[44]) ;
  MPI_Address(&(Input_Param.use_mass_matrix),          &displacements[45]) ;
  MPI_Address(&(Input_Param.inviscid_separation),      &displacements[46]) ;
  MPI_Address(&(Input_Param.all_force_coefficients),   &displacements[47]) ;
  MPI_Address(&(Input_Param.tecplot_format),           &displacements[48]) ;
  MPI_Address(&(Input_Param.all_processors_IO),        &displacements[49]) ;
  MPI_Address(&(Input_Param.system_advection),         &displacements[50]) ;
  MPI_Address(&(Input_Param.scalar_advection),         &displacements[51]) ;

  MPI_Address(Input_Param.Nsmooth_down,                &displacements[52]) ;
  MPI_Address(Input_Param.Nsmooth_up,                  &displacements[53]) ;

  MPI_Address(&(Input_Param.log_stop),                 &displacements[54]) ;
  MPI_Address(&(Input_Param.log_stop_coarse),          &displacements[55]) ;
  MPI_Address(&(Input_Param.eps_smooth),               &displacements[56]) ;
  MPI_Address(&(Input_Param.CFL),                      &displacements[57]) ;
  MPI_Address(&(Input_Param.CFL_coarse),               &displacements[58]) ;
  MPI_Address(&(Input_Param.Mach),                     &displacements[59]) ;
  MPI_Address(&(Input_Param.Mach_coefficients),        &displacements[60]) ;
  MPI_Address(&(Input_Param.alpha),                    &displacements[61]) ;
  MPI_Address(&(Input_Param.beta),                     &displacements[62]) ;
  MPI_Address(&(Input_Param.Reynolds),                 &displacements[63]) ;
  MPI_Address(&(Input_Param.one_over_Reynolds),        &displacements[64]) ;
  MPI_Address(&(Input_Param.Prandt_lam),               &displacements[65]) ;
  MPI_Address(&(Input_Param.Prandt_turb),              &displacements[66]) ;
  MPI_Address(&(Input_Param.gamma),                    &displacements[67]) ;
  MPI_Address(Input_Param.U_inf,                       &displacements[68]) ;
  MPI_Address(Input_Param.x_ref,                       &displacements[69]) ;
  MPI_Address(Input_Param.eta_RK,                      &displacements[70]) ;
  MPI_Address(Input_Param.cdis,                        &displacements[71]) ;
  MPI_Address(&(Input_Param.threshold_cell_volume),    &displacements[72]) ;
  MPI_Address(&(Input_Param.omega_prolongation),       &displacements[73]) ;
  MPI_Address(&(Input_Param.omega_restriction),        &displacements[74]) ;
  MPI_Address(&(Input_Param.omega_DC),                 &displacements[75]) ;
  MPI_Address(&(Input_Param.Sref),                     &displacements[76]) ;
  MPI_Address(&(Input_Param.Lref),                     &displacements[77]) ;
  MPI_Address(&(Input_Param.Delta_t),                  &displacements[78]) ;

  MPI_Address(&(Input_Param.qfil_JST_MATD),            &displacements[79]) ;
  MPI_Address(&(Input_Param.kappa2_JST_MATD),          &displacements[80]) ;
  MPI_Address(&(Input_Param.kappa4_JST_MATD),          &displacements[81]) ;
  MPI_Address(&(Input_Param.omega_pressure_switch),    &displacements[82]) ;
  MPI_Address(&(Input_Param.kappa2_coarse_JST_MATD),   &displacements[83]) ;
  MPI_Address(&(Input_Param.eps_CUSP_SLIP_Loehner),    &displacements[84]) ;
  MPI_Address(&(Input_Param.M_limit_CUSP_SLIP),        &displacements[85]) ;
  MPI_Address(&(Input_Param.nu_limit_CUSP_SLIP),       &displacements[86]) ;
  MPI_Address(&(Input_Param.q_CUSP_SLIP),              &displacements[87]) ;
  MPI_Address(&(Input_Param.kappa4_CUSP_SLIP),         &displacements[88]) ;
  MPI_Address(&(Input_Param.kappa2_SLIP),              &displacements[89]) ;
  MPI_Address(&(Input_Param.kappa_Loehner),            &displacements[90]) ;
  MPI_Address(&(Input_Param.K_Venkat),                 &displacements[91]) ;

  /* Substract the base address from displacements. */

  for(i=0; i<92; i++)
   displacements[i] -= base_address ;

  /* Create the derived data type. */

  MPI_Type_struct(92, block_lengths, displacements, typelist, &newtype) ;

  /* Commit it, such that it can be used. */

  MPI_Type_commit(&newtype) ;


  return ( newtype );
}

  
