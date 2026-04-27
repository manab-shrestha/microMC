
% Increase counter:

if (exist('idx', 'var'));
  idx = idx + 1;
else;
  idx = 1;
end;

% Version, title and date:

VERSION                   (idx, [1: 14])  = 'Serpent 2.1.32' ;
COMPILE_DATE              (idx, [1: 20])  = 'Apr 11 2026 20:32:17' ;
DEBUG                     (idx, 1)        = 0 ;
TITLE                     (idx, [1: 31])  = 'MICROMC VALIDATION K-EIGENVALUE' ;
CONFIDENTIAL_DATA         (idx, 1)        = 0 ;
INPUT_FILE_NAME           (idx, [1:  7])  = 'input_k' ;
WORKING_DIRECTORY         (idx, [1: 31])  = '/Users/shrestha/micromc/serpent' ;
HOSTNAME                  (idx, [1: 18])  = 'manabs-macbook-pro' ;
CPU_TYPE                  (idx, [1:  7])  = 'Unknown' ;
START_DATE                (idx, [1: 24])  = 'Fri Apr 24 00:51:24 2026' ;
COMPLETE_DATE             (idx, [1: 24])  = 'Fri Apr 24 03:13:12 2026' ;

% Run parameters:

POP                       (idx, 1)        = 1000000 ;
CYCLES                    (idx, 1)        = 250 ;
SKIP                      (idx, 1)        = 50 ;
BATCH_INTERVAL            (idx, 1)        = 1 ;
SRC_NORM_MODE             (idx, 1)        = 2 ;
SEED                      (idx, 1)        = 1776988284221 ;
UFS_MODE                  (idx, 1)        = 0 ;
UFS_ORDER                 (idx, 1)        = 1.00000;
NEUTRON_TRANSPORT_MODE    (idx, 1)        = 1 ;
PHOTON_TRANSPORT_MODE     (idx, 1)        = 0 ;
GROUP_CONSTANT_GENERATION (idx, 1)        = 0 ;
B1_CALCULATION            (idx, [1:  3])  = [ 0 0 0 ];
B1_BURNUP_CORRECTION      (idx, 1)        = 0 ;

CRIT_SPEC_MODE            (idx, 1)        = 0 ;
IMPLICIT_REACTION_RATES   (idx, 1)        = 1 ;

% Optimization:

OPTIMIZATION_MODE         (idx, 1)        = 4 ;
RECONSTRUCT_MICROXS       (idx, 1)        = 1 ;
RECONSTRUCT_MACROXS       (idx, 1)        = 1 ;
DOUBLE_INDEXING           (idx, 1)        = 0 ;
MG_MAJORANT_MODE          (idx, 1)        = 0 ;

% Parallelization:

MPI_TASKS                 (idx, 1)        = 1 ;
OMP_THREADS               (idx, 1)        = 1 ;
MPI_REPRODUCIBILITY       (idx, 1)        = 0 ;
OMP_REPRODUCIBILITY       (idx, 1)        = 1 ;
SHARE_BUF_ARRAY           (idx, 1)        = 1 ;
SHARE_RES2_ARRAY          (idx, 1)        = 1 ;
OMP_SHARED_QUEUE_LIM      (idx, 1)        = 0 ;

% File paths:

XS_DATA_FILE_PATH         (idx, [1: 43])  = '/Users/shrestha/endfb71/s2v0_endfb71.xsdata' ;
DECAY_DATA_FILE_PATH      (idx, [1:  3])  = 'N/A' ;
SFY_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;
NFY_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;
BRA_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;

% Collision and reaction sampling (neutrons/photons):

MIN_MACROXS               (idx, [1:   4]) = [  5.00000E-02 2.5E-09  0.00000E+00 0.0E+00 ];
DT_THRESH                 (idx, [1:  2])  = [  9.00000E-01  9.00000E-01 ];
ST_FRAC                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_FRAC                   (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_EFF                    (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_EFF          (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_FAIL         (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_COL_EFF               (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKING_LOOPS        (idx, [1:   8]) = [  1.97818E+00 4.9E-06  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKS                (idx, [1:   4]) = [  2.32190E+01 3.1E-05  0.00000E+00 0.0E+00 ];
AVG_REAL_COL              (idx, [1:   4]) = [  2.32190E+01 3.1E-05  0.00000E+00 0.0E+00 ];
AVG_VIRT_COL              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_SURF_CROSS            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
LOST_PARTICLES            (idx, 1)        = 0 ;

% Run statistics:

CYCLE_IDX                 (idx, 1)        = 250 ;
SIMULATED_HISTORIES       (idx, 1)        = 249997354 ;
MEAN_POP_SIZE             (idx, [1:  2])  = [  9.99989E+05 0.00009 ];
MEAN_POP_WGT              (idx, [1:  2])  = [  9.99989E+05 0.00009 ];
SIMULATION_COMPLETED      (idx, 1)        = 1 ;

% Running times:

TOT_CPU_TIME              (idx, 1)        =  1.41768E+02 ;
RUNNING_TIME              (idx, 1)        =  1.41810E+02 ;
INIT_TIME                 (idx, [1:  2])  = [  4.63500E-02  4.63500E-02 ];
PROCESS_TIME              (idx, [1:  2])  = [  1.99997E-04  1.99997E-04 ];
TRANSPORT_CYCLE_TIME      (idx, [1:  3])  = [  1.41764E+02  1.41764E+02  0.00000E+00 ];
MPI_OVERHEAD_TIME         (idx, [1:  2])  = [  0.00000E+00  0.00000E+00 ];
ESTIMATED_RUNNING_TIME    (idx, [1:  2])  = [  1.41808E+02  0.00000E+00 ];
CPU_USAGE                 (idx, 1)        = 0.99970 ;
TRANSPORT_CPU_USAGE       (idx, [1:   2]) = [  9.99814E-01 4.7E-05 ];
OMP_PARALLEL_FRAC         (idx, 1)        =  9.86490E-01 ;

% Memory usage:

ALLOC_MEMSIZE             (idx, 1)        = 6816.95;
MEMSIZE                   (idx, 1)        = 6766.58;
XS_MEMSIZE                (idx, 1)        = 56.44;
MAT_MEMSIZE               (idx, 1)        = 34.08;
RES_MEMSIZE               (idx, 1)        = 0.25;
IFC_MEMSIZE               (idx, 1)        = 0.00;
MISC_MEMSIZE              (idx, 1)        = 6675.81;
UNKNOWN_MEMSIZE           (idx, 1)        = 0.00;
UNUSED_MEMSIZE            (idx, 1)        = 50.37;

% Geometry parameters:

TOT_CELLS                 (idx, 1)        = 2 ;
UNION_CELLS               (idx, 1)        = 0 ;

% Neutron energy grid:

NEUTRON_ERG_TOL           (idx, 1)        =  0.00000E+00 ;
NEUTRON_ERG_NE            (idx, 1)        = 178505 ;
NEUTRON_EMIN              (idx, 1)        =  1.00000E-11 ;
NEUTRON_EMAX              (idx, 1)        =  2.00000E+01 ;

% Unresolved resonance probability table sampling:

URES_DILU_CUT             (idx, 1)        =  1.00000E-09 ;
URES_EMIN                 (idx, 1)        =  1.00000E+37 ;
URES_EMAX                 (idx, 1)        = -1.00000E+37 ;
URES_AVAIL                (idx, 1)        = 2 ;
URES_USED                 (idx, 1)        = 0 ;

% Nuclides and reaction channels:

TOT_NUCLIDES              (idx, 1)        = 4 ;
TOT_TRANSPORT_NUCLIDES    (idx, 1)        = 4 ;
TOT_DOSIMETRY_NUCLIDES    (idx, 1)        = 0 ;
TOT_DECAY_NUCLIDES        (idx, 1)        = 0 ;
TOT_PHOTON_NUCLIDES       (idx, 1)        = 0 ;
TOT_REA_CHANNELS          (idx, 1)        = 115 ;
TOT_TRANSMU_REA           (idx, 1)        = 0 ;

% Neutron physics options:

USE_DELNU                 (idx, 1)        = 1 ;
USE_URES                  (idx, 1)        = 0 ;
USE_DBRC                  (idx, 1)        = 0 ;
IMPL_CAPT                 (idx, 1)        = 0 ;
IMPL_NXN                  (idx, 1)        = 1 ;
IMPL_FISS                 (idx, 1)        = 0 ;
DOPPLER_PREPROCESSOR      (idx, 1)        = 1 ;
TMS_MODE                  (idx, 1)        = 0 ;
SAMPLE_FISS               (idx, 1)        = 1 ;
SAMPLE_CAPT               (idx, 1)        = 1 ;
SAMPLE_SCATT              (idx, 1)        = 1 ;

% Energy deposition:

EDEP_MODE                 (idx, 1)        = 0 ;
EDEP_DELAYED              (idx, 1)        = 1 ;
EDEP_KEFF_CORR            (idx, 1)        = 1 ;
EDEP_LOCAL_EGD            (idx, 1)        = 0 ;
EDEP_COMP                 (idx, [1:  9])  = [ 0 0 0 0 0 0 0 0 0 ];
EDEP_CAPT_E               (idx, 1)        =  0.00000E+00 ;

% Radioactivity data:

TOT_ACTIVITY              (idx, 1)        =  0.00000E+00 ;
TOT_DECAY_HEAT            (idx, 1)        =  0.00000E+00 ;
TOT_SF_RATE               (idx, 1)        =  0.00000E+00 ;
ACTINIDE_ACTIVITY         (idx, 1)        =  0.00000E+00 ;
ACTINIDE_DECAY_HEAT       (idx, 1)        =  0.00000E+00 ;
FISSION_PRODUCT_ACTIVITY  (idx, 1)        =  0.00000E+00 ;
FISSION_PRODUCT_DECAY_HEAT(idx, 1)        =  0.00000E+00 ;
INHALATION_TOXICITY       (idx, 1)        =  0.00000E+00 ;
INGESTION_TOXICITY        (idx, 1)        =  0.00000E+00 ;
ACTINIDE_INH_TOX          (idx, 1)        =  0.00000E+00 ;
ACTINIDE_ING_TOX          (idx, 1)        =  0.00000E+00 ;
FISSION_PRODUCT_INH_TOX   (idx, 1)        =  0.00000E+00 ;
FISSION_PRODUCT_ING_TOX   (idx, 1)        =  0.00000E+00 ;
SR90_ACTIVITY             (idx, 1)        =  0.00000E+00 ;
TE132_ACTIVITY            (idx, 1)        =  0.00000E+00 ;
I131_ACTIVITY             (idx, 1)        =  0.00000E+00 ;
I132_ACTIVITY             (idx, 1)        =  0.00000E+00 ;
CS134_ACTIVITY            (idx, 1)        =  0.00000E+00 ;
CS137_ACTIVITY            (idx, 1)        =  0.00000E+00 ;
PHOTON_DECAY_SOURCE       (idx, 1)        =  0.00000E+00 ;
NEUTRON_DECAY_SOURCE      (idx, 1)        =  0.00000E+00 ;
ALPHA_DECAY_SOURCE        (idx, 1)        =  0.00000E+00 ;
ELECTRON_DECAY_SOURCE     (idx, 1)        =  0.00000E+00 ;

% Normalization coefficient:

NORM_COEF                 (idx, [1:   4]) = [  9.97931E-07 5.4E-05  0.00000E+00 0.0E+00 ];

% Analog reaction rate estimators:

CONVERSION_RATIO          (idx, [1:   2]) = [  6.10701E-01 0.00013 ];
U235_FISS                 (idx, [1:   4]) = [  4.68131E-01 7.4E-05  9.26360E-01 2.5E-05 ];
U238_FISS                 (idx, [1:   4]) = [  3.72138E-02 0.00033  7.36404E-02 0.00031 ];
U235_CAPT                 (idx, [1:   4]) = [  1.13078E-01 0.00018  2.28591E-01 0.00016 ];
U238_CAPT                 (idx, [1:   4]) = [  3.54980E-01 0.00011  7.17602E-01 5.4E-05 ];

% Neutron balance (particles/weight):

BALA_SRC_NEUTRON_SRC     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_FISS    (idx, [1:  2])  = [ 249997354 2.50000E+08 ];
BALA_SRC_NEUTRON_NXN     (idx, [1:  2])  = [ 0 5.23428E+05 ];
BALA_SRC_NEUTRON_VR      (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_TOT     (idx, [1:  2])  = [ 249997354 2.50523E+08 ];

BALA_LOSS_NEUTRON_CAPT    (idx, [1:  2])  = [ 123657190 1.23925E+08 ];
BALA_LOSS_NEUTRON_FISS    (idx, [1:  2])  = [ 126340164 1.26598E+08 ];
BALA_LOSS_NEUTRON_LEAK    (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_CUT     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_ERR     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_TOT     (idx, [1:  2])  = [ 249997354 2.50523E+08 ];

BALA_NEUTRON_DIFF         (idx, [1:  2])  = [ 0 -2.98917E-04 ];

% Normalized total reaction rates (neutrons):

TOT_POWER                 (idx, [1:   2]) = [  1.64054E-11 3.4E-05 ];
TOT_POWDENS               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_GENRATE               (idx, [1:   2]) = [  1.24634E+00 3.4E-05 ];
TOT_FISSRATE              (idx, [1:   2]) = [  5.05383E-01 3.4E-05 ];
TOT_CAPTRATE              (idx, [1:   2]) = [  4.94617E-01 3.5E-05 ];
TOT_ABSRATE               (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_SRCRATE               (idx, [1:   2]) = [  9.97931E-01 5.4E-05 ];
TOT_FLUX                  (idx, [1:   2]) = [  1.64081E+01 4.5E-05 ];
TOT_PHOTON_PRODRATE       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_LEAKRATE              (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
ALBEDO_LEAKRATE           (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_LOSSRATE              (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_CUTRATE               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_RR                    (idx, [1:   2]) = [  2.32179E+01 3.3E-05 ];
INI_FMASS                 (idx, 1)        =  0.00000E+00 ;
TOT_FMASS                 (idx, 1)        =  0.00000E+00 ;

% Six-factor formula:

SIX_FF_ETA                (idx, [1:   2]) = [  1.76824E+00 5.3E-05 ];
SIX_FF_F                  (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_P                  (idx, [1:   2]) = [  5.13278E-01 6.4E-05 ];
SIX_FF_EPSILON            (idx, [1:   2]) = [  1.37597E+00 5.6E-05 ];
SIX_FF_LF                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_LT                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_KINF               (idx, [1:   2]) = [  1.24883E+00 6.4E-05 ];
SIX_FF_KEFF               (idx, [1:   2]) = [  1.24883E+00 6.4E-05 ];

% Fission neutron and energy production:

NUBAR                     (idx, [1:   2]) = [  2.46612E+00 2.0E-06 ];
FISSE                     (idx, [1:   2]) = [  2.02607E+02 1.9E-07 ];

% Criticality eigenvalues:

ANA_KEFF                  (idx, [1:   6]) = [  1.24882E+00 6.5E-05  1.24022E+00 6.5E-05  8.60814E-03 0.00110 ];
IMP_KEFF                  (idx, [1:   2]) = [  1.24895E+00 3.4E-05 ];
COL_KEFF                  (idx, [1:   2]) = [  1.24892E+00 7.7E-05 ];
ABS_KEFF                  (idx, [1:   2]) = [  1.24895E+00 3.4E-05 ];
ABS_KINF                  (idx, [1:   2]) = [  1.24895E+00 3.4E-05 ];
GEOM_ALBEDO               (idx, [1:   6]) = [  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00 ];

% ALF (Average lethargy of neutrons causing fission):
% Based on E0 = 2.000000E+01 MeV

ANA_ALF                   (idx, [1:   2]) = [  1.66281E+01 2.8E-05 ];
IMP_ALF                   (idx, [1:   2]) = [  1.66285E+01 1.4E-05 ];

% EALF (Energy corresponding to average lethargy of neutrons causing fission):

ANA_EALF                  (idx, [1:   2]) = [  1.20099E-06 0.00046 ];
IMP_EALF                  (idx, [1:   2]) = [  1.20051E-06 0.00023 ];

% AFGE (Average energy of neutrons causing fission):

ANA_AFGE                  (idx, [1:   2]) = [  2.60336E-01 0.00033 ];
IMP_AFGE                  (idx, [1:   2]) = [  2.60254E-01 0.00015 ];

% Forward-weighted delayed neutron parameters:

PRECURSOR_GROUPS          (idx, 1)        = 6 ;
FWD_ANA_BETA_ZERO         (idx, [1:  14]) = [  5.80350E-03 0.00074  1.81849E-04 0.00419  9.77435E-04 0.00166  9.62322E-04 0.00179  2.24123E-03 0.00111  1.01742E-03 0.00179  4.23246E-04 0.00272 ];
FWD_ANA_LAMBDA            (idx, [1:  14]) = [  5.07399E-01 0.00108  1.33592E-02 2.6E-05  3.25743E-02 2.5E-05  1.21137E-01 1.3E-05  3.06456E-01 3.1E-05  8.63790E-01 4.8E-05  2.90169E+00 8.1E-05 ];

% Beta-eff using Meulekamp's method:

ADJ_MEULEKAMP_BETA_EFF    (idx, [1:  14]) = [  6.96952E-03 0.00117  2.18854E-04 0.00664  1.17464E-03 0.00288  1.15191E-03 0.00285  2.69147E-03 0.00187  1.22554E-03 0.00282  5.07106E-04 0.00442 ];
ADJ_MEULEKAMP_LAMBDA      (idx, [1:  14]) = [  5.07287E-01 0.00162  1.33590E-02 4.2E-05  3.25712E-02 4.2E-05  1.21136E-01 2.3E-05  3.06461E-01 5.1E-05  8.63768E-01 8.5E-05  2.90192E+00 0.00013 ];

% Adjoint weighted time constants using Nauchi's method:

IFP_CHAIN_LENGTH          (idx, 1)        = 15 ;
ADJ_NAUCHI_GEN_TIME       (idx, [1:   6]) = [  5.26481E-06 0.00015  5.26121E-06 0.00015  5.78241E-06 0.00139 ];
ADJ_NAUCHI_LIFETIME       (idx, [1:   6]) = [  6.57480E-06 0.00013  6.57032E-06 0.00013  7.22120E-06 0.00139 ];
ADJ_NAUCHI_BETA_EFF       (idx, [1:  14]) = [  6.89530E-03 0.00111  2.15792E-04 0.00588  1.15766E-03 0.00253  1.14031E-03 0.00277  2.66641E-03 0.00171  1.21167E-03 0.00252  5.03459E-04 0.00369 ];
ADJ_NAUCHI_LAMBDA         (idx, [1:  14]) = [  5.08147E-01 0.00150  1.33595E-02 3.7E-05  3.25739E-02 3.5E-05  1.21138E-01 1.9E-05  3.06480E-01 4.5E-05  8.63884E-01 8.0E-05  2.90186E+00 0.00012 ];

% Adjoint weighted time constants using IFP:

ADJ_IFP_GEN_TIME          (idx, [1:   6]) = [  5.26578E-06 0.00031  5.26208E-06 0.00032  5.79825E-06 0.00334 ];
ADJ_IFP_LIFETIME          (idx, [1:   6]) = [  6.57601E-06 0.00031  6.57140E-06 0.00031  7.24097E-06 0.00333 ];
ADJ_IFP_IMP_BETA_EFF      (idx, [1:  14]) = [  6.88667E-03 0.00319  2.19343E-04 0.01670  1.13989E-03 0.00822  1.13549E-03 0.00786  2.67521E-03 0.00541  1.20636E-03 0.00769  5.10380E-04 0.01166 ];
ADJ_IFP_IMP_LAMBDA        (idx, [1:  14]) = [  5.11510E-01 0.00479  1.33596E-02 0.00011  3.25773E-02 0.00010  1.21139E-01 6.2E-05  3.06501E-01 0.00014  8.64179E-01 0.00020  2.90256E+00 0.00034 ];
ADJ_IFP_ANA_BETA_EFF      (idx, [1:  14]) = [  6.88823E-03 0.00305  2.19232E-04 0.01618  1.14180E-03 0.00775  1.13692E-03 0.00750  2.67191E-03 0.00507  1.20962E-03 0.00751  5.08753E-04 0.01119 ];
ADJ_IFP_ANA_LAMBDA        (idx, [1:  14]) = [  5.10879E-01 0.00451  1.33595E-02 0.00011  3.25754E-02 9.8E-05  1.21137E-01 5.9E-05  3.06493E-01 0.00013  8.64122E-01 0.00020  2.90249E+00 0.00032 ];
ADJ_IFP_ROSSI_ALPHA       (idx, [1:   2]) = [ -1.30875E+03 0.00319 ];

% Adjoint weighted time constants using perturbation technique:

ADJ_PERT_GEN_TIME         (idx, [1:   2]) = [  5.27120E-06 0.00010 ];
ADJ_PERT_LIFETIME         (idx, [1:   2]) = [  6.58278E-06 6.6E-05 ];
ADJ_PERT_BETA_EFF         (idx, [1:   2]) = [  6.90133E-03 0.00067 ];
ADJ_PERT_ROSSI_ALPHA      (idx, [1:   2]) = [ -1.30925E+03 0.00065 ];

% Inverse neutron speed :

ANA_INV_SPD               (idx, [1:   2]) = [  2.85889E-07 7.9E-05 ];

% Analog slowing-down and thermal neutron lifetime (total/prompt/delayed):

ANA_SLOW_TIME             (idx, [1:   6]) = [  1.16308E-06 6.3E-05  1.16310E-06 6.3E-05  1.16058E-06 0.00075 ];
ANA_THERM_TIME            (idx, [1:   6]) = [  7.73314E-06 8.5E-05  7.73316E-06 8.5E-05  7.73012E-06 0.00100 ];
ANA_THERM_FRAC            (idx, [1:   6]) = [  5.14388E-01 6.4E-05  5.13473E-01 6.5E-05  6.71114E-01 0.00126 ];
ANA_DELAYED_EMTIME        (idx, [1:   2]) = [  1.03847E+01 0.00171 ];
ANA_MEAN_NCOL             (idx, [1:   4]) = [  2.32190E+01 3.1E-05  2.60832E+01 4.4E-05 ];

