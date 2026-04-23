
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
START_DATE                (idx, [1: 24])  = 'Thu Apr 23 11:10:09 2026' ;
COMPLETE_DATE             (idx, [1: 24])  = 'Thu Apr 23 11:44:57 2026' ;

% Run parameters:

POP                       (idx, 1)        = 100000 ;
CYCLES                    (idx, 1)        = 500 ;
SKIP                      (idx, 1)        = 50 ;
BATCH_INTERVAL            (idx, 1)        = 1 ;
SRC_NORM_MODE             (idx, 1)        = 2 ;
SEED                      (idx, 1)        = 1776939009866 ;
UFS_MODE                  (idx, 1)        = 0 ;
UFS_ORDER                 (idx, 1)        = 1.00000;
NEUTRON_TRANSPORT_MODE    (idx, 1)        = 1 ;
PHOTON_TRANSPORT_MODE     (idx, 1)        = 0 ;
GROUP_CONSTANT_GENERATION (idx, 1)        = 1 ;
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

MIN_MACROXS               (idx, [1:   4]) = [  5.00000E-02 2.0E-09  0.00000E+00 0.0E+00 ];
DT_THRESH                 (idx, [1:  2])  = [  9.00000E-01  9.00000E-01 ];
ST_FRAC                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_FRAC                   (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_EFF                    (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_EFF          (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_FAIL         (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_COL_EFF               (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKING_LOOPS        (idx, [1:   8]) = [  1.97817E+00 1.0E-05  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKS                (idx, [1:   4]) = [  2.32194E+01 6.9E-05  0.00000E+00 0.0E+00 ];
AVG_REAL_COL              (idx, [1:   4]) = [  2.32194E+01 6.9E-05  0.00000E+00 0.0E+00 ];
AVG_VIRT_COL              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_SURF_CROSS            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
LOST_PARTICLES            (idx, 1)        = 0 ;

% Run statistics:

CYCLE_IDX                 (idx, 1)        = 500 ;
SIMULATED_HISTORIES       (idx, 1)        = 50000826 ;
MEAN_POP_SIZE             (idx, [1:  2])  = [  1.00002E+05 0.00019 ];
MEAN_POP_WGT              (idx, [1:  2])  = [  1.00002E+05 0.00019 ];
SIMULATION_COMPLETED      (idx, 1)        = 1 ;

% Running times:

TOT_CPU_TIME              (idx, 1)        =  3.47676E+01 ;
RUNNING_TIME              (idx, 1)        =  3.47958E+01 ;
INIT_TIME                 (idx, [1:  2])  = [  1.32000E-02  1.32000E-02 ];
PROCESS_TIME              (idx, [1:  2])  = [  1.50001E-04  1.50001E-04 ];
TRANSPORT_CYCLE_TIME      (idx, [1:  3])  = [  3.47824E+01  3.47824E+01  0.00000E+00 ];
MPI_OVERHEAD_TIME         (idx, [1:  2])  = [  0.00000E+00  0.00000E+00 ];
ESTIMATED_RUNNING_TIME    (idx, [1:  2])  = [  3.47955E+01  0.00000E+00 ];
CPU_USAGE                 (idx, 1)        = 0.99919 ;
TRANSPORT_CPU_USAGE       (idx, [1:   2]) = [  9.99242E-01 7.9E-05 ];
OMP_PARALLEL_FRAC         (idx, 1)        =  9.91738E-01 ;

% Memory usage:

ALLOC_MEMSIZE             (idx, 1)        = 808.86;
MEMSIZE                   (idx, 1)        = 759.26;
XS_MEMSIZE                (idx, 1)        = 56.46;
MAT_MEMSIZE               (idx, 1)        = 34.09;
RES_MEMSIZE               (idx, 1)        = 1.05;
IFC_MEMSIZE               (idx, 1)        = 0.00;
MISC_MEMSIZE              (idx, 1)        = 667.66;
UNKNOWN_MEMSIZE           (idx, 1)        = 0.00;
UNUSED_MEMSIZE            (idx, 1)        = 49.60;

% Geometry parameters:

TOT_CELLS                 (idx, 1)        = 2 ;
UNION_CELLS               (idx, 1)        = 0 ;

% Neutron energy grid:

NEUTRON_ERG_TOL           (idx, 1)        =  0.00000E+00 ;
NEUTRON_ERG_NE            (idx, 1)        = 178562 ;
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

NORM_COEF                 (idx, [1:   4]) = [  9.97914E-06 0.00013  0.00000E+00 0.0E+00 ];

% Analog reaction rate estimators:

CONVERSION_RATIO          (idx, [1:   2]) = [  6.10630E-01 0.00030 ];
U235_FISS                 (idx, [1:   4]) = [  4.68068E-01 0.00017  9.26243E-01 5.6E-05 ];
U238_FISS                 (idx, [1:   4]) = [  3.72726E-02 0.00075  7.37567E-02 0.00070 ];
U235_CAPT                 (idx, [1:   4]) = [  1.13101E-01 0.00043  2.28639E-01 0.00039 ];
U238_CAPT                 (idx, [1:   4]) = [  3.54911E-01 0.00025  7.17467E-01 0.00014 ];

% Neutron balance (particles/weight):

BALA_SRC_NEUTRON_SRC     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_FISS    (idx, [1:  2])  = [ 50000826 5.00000E+07 ];
BALA_SRC_NEUTRON_NXN     (idx, [1:  2])  = [ 0 1.05094E+05 ];
BALA_SRC_NEUTRON_VR      (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_TOT     (idx, [1:  2])  = [ 50000826 5.01051E+07 ];

BALA_LOSS_NEUTRON_CAPT    (idx, [1:  2])  = [ 24732120 2.47852E+07 ];
BALA_LOSS_NEUTRON_FISS    (idx, [1:  2])  = [ 25268706 2.53199E+07 ];
BALA_LOSS_NEUTRON_LEAK    (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_CUT     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_ERR     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_TOT     (idx, [1:  2])  = [ 50000826 5.01051E+07 ];

BALA_NEUTRON_DIFF         (idx, [1:  2])  = [ 0 -1.04308E-06 ];

% Normalized total reaction rates (neutrons):

TOT_POWER                 (idx, [1:   2]) = [  1.64056E-11 7.6E-05 ];
TOT_POWDENS               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_GENRATE               (idx, [1:   2]) = [  1.24636E+00 7.5E-05 ];
TOT_FISSRATE              (idx, [1:   2]) = [  5.05390E-01 7.6E-05 ];
TOT_CAPTRATE              (idx, [1:   2]) = [  4.94610E-01 7.8E-05 ];
TOT_ABSRATE               (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_SRCRATE               (idx, [1:   2]) = [  9.97914E-01 0.00013 ];
TOT_FLUX                  (idx, [1:   2]) = [  1.64089E+01 0.00010 ];
TOT_PHOTON_PRODRATE       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_LEAKRATE              (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
ALBEDO_LEAKRATE           (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_LOSSRATE              (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_CUTRATE               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_RR                    (idx, [1:   2]) = [  2.32180E+01 7.8E-05 ];
INI_FMASS                 (idx, 1)        =  0.00000E+00 ;
TOT_FMASS                 (idx, 1)        =  0.00000E+00 ;

% Six-factor formula:

SIX_FF_ETA                (idx, [1:   2]) = [  1.76812E+00 0.00012 ];
SIX_FF_F                  (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_P                  (idx, [1:   2]) = [  5.13343E-01 0.00014 ];
SIX_FF_EPSILON            (idx, [1:   2]) = [  1.37592E+00 0.00012 ];
SIX_FF_LF                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_LT                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_KINF               (idx, [1:   2]) = [  1.24885E+00 0.00013 ];
SIX_FF_KEFF               (idx, [1:   2]) = [  1.24885E+00 0.00013 ];

% Fission neutron and energy production:

NUBAR                     (idx, [1:   2]) = [  2.46613E+00 4.2E-06 ];
FISSE                     (idx, [1:   2]) = [  2.02607E+02 4.0E-07 ];

% Criticality eigenvalues:

ANA_KEFF                  (idx, [1:   6]) = [  1.24882E+00 0.00014  1.24025E+00 0.00014  8.59846E-03 0.00255 ];
IMP_KEFF                  (idx, [1:   2]) = [  1.24898E+00 7.6E-05 ];
COL_KEFF                  (idx, [1:   2]) = [  1.24898E+00 0.00017 ];
ABS_KEFF                  (idx, [1:   2]) = [  1.24898E+00 7.6E-05 ];
ABS_KINF                  (idx, [1:   2]) = [  1.24898E+00 7.6E-05 ];
GEOM_ALBEDO               (idx, [1:   6]) = [  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00 ];

% ALF (Average lethargy of neutrons causing fission):
% Based on E0 = 2.000000E+01 MeV

ANA_ALF                   (idx, [1:   2]) = [  1.66274E+01 5.9E-05 ];
IMP_ALF                   (idx, [1:   2]) = [  1.66282E+01 2.9E-05 ];

% EALF (Energy corresponding to average lethargy of neutrons causing fission):

ANA_EALF                  (idx, [1:   2]) = [  1.20216E-06 0.00099 ];
IMP_EALF                  (idx, [1:   2]) = [  1.20092E-06 0.00049 ];

% AFGE (Average energy of neutrons causing fission):

ANA_AFGE                  (idx, [1:   2]) = [  2.60570E-01 0.00074 ];
IMP_AFGE                  (idx, [1:   2]) = [  2.60356E-01 0.00031 ];

% Forward-weighted delayed neutron parameters:

PRECURSOR_GROUPS          (idx, 1)        = 6 ;
FWD_ANA_BETA_ZERO         (idx, [1:  14]) = [  5.80550E-03 0.00182  1.81954E-04 0.01001  9.78745E-04 0.00415  9.59953E-04 0.00413  2.24158E-03 0.00291  1.01530E-03 0.00406  4.27957E-04 0.00628 ];
FWD_ANA_LAMBDA            (idx, [1:  14]) = [  5.09241E-01 0.00248  1.33589E-02 5.8E-05  3.25759E-02 5.6E-05  1.21138E-01 3.0E-05  3.06459E-01 7.0E-05  8.63803E-01 0.00011  2.90192E+00 0.00017 ];

% Beta-eff using Meulekamp's method:

ADJ_MEULEKAMP_BETA_EFF    (idx, [1:  14]) = [  6.98395E-03 0.00285  2.23683E-04 0.01595  1.17482E-03 0.00671  1.15421E-03 0.00709  2.68797E-03 0.00457  1.22273E-03 0.00628  5.20538E-04 0.01026 ];
ADJ_MEULEKAMP_LAMBDA      (idx, [1:  14]) = [  5.11475E-01 0.00399  1.33595E-02 9.8E-05  3.25743E-02 8.9E-05  1.21130E-01 5.0E-05  3.06481E-01 0.00011  8.63941E-01 0.00018  2.90261E+00 0.00028 ];

% Adjoint weighted time constants using Nauchi's method:

IFP_CHAIN_LENGTH          (idx, 1)        = 15 ;
ADJ_NAUCHI_GEN_TIME       (idx, [1:   6]) = [  5.26642E-06 0.00031  5.26283E-06 0.00032  5.78316E-06 0.00314 ];
ADJ_NAUCHI_LIFETIME       (idx, [1:   6]) = [  6.57672E-06 0.00028  6.57224E-06 0.00028  7.22211E-06 0.00315 ];
ADJ_NAUCHI_BETA_EFF       (idx, [1:  14]) = [  6.88776E-03 0.00256  2.16694E-04 0.01320  1.15664E-03 0.00562  1.13922E-03 0.00591  2.65690E-03 0.00414  1.20638E-03 0.00538  5.11932E-04 0.00899 ];
ADJ_NAUCHI_LAMBDA         (idx, [1:  14]) = [  5.11041E-01 0.00347  1.33587E-02 8.7E-05  3.25774E-02 7.9E-05  1.21135E-01 4.3E-05  3.06475E-01 9.7E-05  8.63914E-01 0.00017  2.90161E+00 0.00025 ];

% Adjoint weighted time constants using IFP:

ADJ_IFP_GEN_TIME          (idx, [1:   6]) = [  5.26863E-06 0.00071  5.26562E-06 0.00071  5.70758E-06 0.00759 ];
ADJ_IFP_LIFETIME          (idx, [1:   6]) = [  6.57947E-06 0.00069  6.57572E-06 0.00069  7.12743E-06 0.00758 ];
ADJ_IFP_IMP_BETA_EFF      (idx, [1:  14]) = [  6.78785E-03 0.00705  2.09139E-04 0.04327  1.12419E-03 0.01735  1.10828E-03 0.01779  2.63306E-03 0.01180  1.18767E-03 0.01782  5.25502E-04 0.02578 ];
ADJ_IFP_IMP_LAMBDA        (idx, [1:  14]) = [  5.20745E-01 0.01053  1.33535E-02 0.00020  3.25853E-02 0.00024  1.21128E-01 0.00013  3.06410E-01 0.00031  8.63551E-01 0.00050  2.89798E+00 0.00074 ];
ADJ_IFP_ANA_BETA_EFF      (idx, [1:  14]) = [  6.77525E-03 0.00679  2.11559E-04 0.04235  1.11851E-03 0.01653  1.10806E-03 0.01684  2.62348E-03 0.01102  1.18290E-03 0.01708  5.30745E-04 0.02488 ];
ADJ_IFP_ANA_LAMBDA        (idx, [1:  14]) = [  5.22180E-01 0.01011  1.33539E-02 0.00020  3.25847E-02 0.00023  1.21135E-01 0.00013  3.06412E-01 0.00030  8.63499E-01 0.00047  2.89858E+00 0.00071 ];
ADJ_IFP_ROSSI_ALPHA       (idx, [1:   2]) = [ -1.28956E+03 0.00713 ];

% Adjoint weighted time constants using perturbation technique:

ADJ_PERT_GEN_TIME         (idx, [1:   2]) = [  5.27365E-06 0.00018 ];
ADJ_PERT_LIFETIME         (idx, [1:   2]) = [  6.58575E-06 0.00013 ];
ADJ_PERT_BETA_EFF         (idx, [1:   2]) = [  6.85643E-03 0.00130 ];
ADJ_PERT_ROSSI_ALPHA      (idx, [1:   2]) = [ -1.30015E+03 0.00131 ];

% Inverse neutron speed :

ANA_INV_SPD               (idx, [1:   2]) = [  2.85870E-07 0.00017 ];

% Analog slowing-down and thermal neutron lifetime (total/prompt/delayed):

ANA_SLOW_TIME             (idx, [1:   6]) = [  1.16292E-06 0.00015  1.16292E-06 0.00015  1.16294E-06 0.00167 ];
ANA_THERM_TIME            (idx, [1:   6]) = [  7.73148E-06 0.00019  7.73140E-06 0.00019  7.74165E-06 0.00218 ];
ANA_THERM_FRAC            (idx, [1:   6]) = [  5.14454E-01 0.00014  5.13544E-01 0.00014  6.71480E-01 0.00302 ];
ANA_DELAYED_EMTIME        (idx, [1:   2]) = [  1.03934E+01 0.00417 ];
ANA_MEAN_NCOL             (idx, [1:   4]) = [  2.32194E+01 6.9E-05  2.60820E+01 9.5E-05 ];

% Group constant generation:

GC_UNIVERSE_NAME          (idx, [1:  1])  = '0' ;

% Micro- and macro-group structures:

MICRO_NG                  (idx, 1)        = 70 ;
MICRO_E                   (idx, [1:  71]) = [  2.00000E+01  6.06550E+00  3.67900E+00  2.23100E+00  1.35300E+00  8.21000E-01  5.00000E-01  3.02500E-01  1.83000E-01  1.11000E-01  6.74300E-02  4.08500E-02  2.47800E-02  1.50300E-02  9.11800E-03  5.50000E-03  3.51910E-03  2.23945E-03  1.42510E-03  9.06898E-04  3.67262E-04  1.48728E-04  7.55014E-05  4.80520E-05  2.77000E-05  1.59680E-05  9.87700E-06  4.00000E-06  3.30000E-06  2.60000E-06  2.10000E-06  1.85500E-06  1.50000E-06  1.30000E-06  1.15000E-06  1.12300E-06  1.09700E-06  1.07100E-06  1.04500E-06  1.02000E-06  9.96000E-07  9.72000E-07  9.50000E-07  9.10000E-07  8.50000E-07  7.80000E-07  6.25000E-07  5.00000E-07  4.00000E-07  3.50000E-07  3.20000E-07  3.00000E-07  2.80000E-07  2.50000E-07  2.20000E-07  1.80000E-07  1.40000E-07  1.00000E-07  8.00000E-08  6.70000E-08  5.80000E-08  5.00000E-08  4.20000E-08  3.50000E-08  3.00000E-08  2.50000E-08  2.00000E-08  1.50000E-08  1.00000E-08  5.00000E-09  1.00000E-11 ];

MACRO_NG                  (idx, 1)        = 2 ;
MACRO_E                   (idx, [1:   3]) = [  1.00000E+37  6.25000E-07  0.00000E+00 ];

% Micro-group spectrum:

INF_MICRO_FLX             (idx, [1: 140]) = [  3.10094E+05 0.00124  1.21458E+06 0.00046  2.35809E+06 0.00028  2.51292E+06 0.00023  2.37810E+06 0.00024  2.48537E+06 0.00023  1.78322E+06 0.00020  1.53031E+06 0.00022  1.18487E+06 0.00023  9.72860E+05 0.00025  8.43142E+05 0.00015  7.54645E+05 0.00025  7.00588E+05 0.00022  6.65059E+05 0.00022  6.48202E+05 0.00024  5.57732E+05 0.00025  5.51170E+05 0.00024  5.42628E+05 0.00023  5.30694E+05 0.00023  1.02576E+06 0.00020  9.76206E+05 0.00016  6.87087E+05 0.00015  4.34926E+05 0.00031  4.85107E+05 0.00028  4.38767E+05 0.00021  4.03245E+05 0.00025  6.12478E+05 0.00021  1.41772E+05 0.00053  1.77063E+05 0.00048  1.60669E+05 0.00038  9.25670E+04 0.00058  1.60997E+05 0.00053  1.09008E+05 0.00055  9.17948E+04 0.00052  1.73872E+04 0.00096  1.71835E+04 0.00096  1.76759E+04 0.00088  1.82751E+04 0.00116  1.81162E+04 0.00101  1.79522E+04 0.00115  1.84763E+04 0.00093  1.74431E+04 0.00112  3.29303E+04 0.00087  5.28595E+04 0.00066  6.76823E+04 0.00047  1.83430E+05 0.00041  2.08850E+05 0.00042  2.51182E+05 0.00035  1.76290E+05 0.00040  1.29352E+05 0.00053  9.87573E+04 0.00045  1.10888E+05 0.00044  1.93720E+05 0.00036  2.33813E+05 0.00036  3.84920E+05 0.00030  4.74366E+05 0.00031  5.49887E+05 0.00029  2.87856E+05 0.00037  1.83443E+05 0.00044  1.21974E+05 0.00040  1.02898E+05 0.00049  9.57307E+04 0.00058  7.61437E+04 0.00049  4.90567E+04 0.00041  4.38277E+04 0.00052  3.79034E+04 0.00048  3.10655E+04 0.00057  2.34357E+04 0.00071  1.48282E+04 0.00061  5.17090E+03 0.00062 ];

% Integral parameters:

INF_KINF                  (idx, [1:   2]) = [  1.24898E+00 0.00018 ];

% Flux spectra in infinite geometry:

INF_FLX                   (idx, [1:   4]) = [  1.44703E+01 0.00013  1.93863E+00 0.00011 ];
INF_FISS_FLX              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Reaction cross sections:

INF_TOT                   (idx, [1:   4]) = [  1.25409E+00 2.9E-05  2.61576E+00 1.7E-05 ];
INF_CAPT                  (idx, [1:   4]) = [  2.44722E-02 0.00016  7.24698E-02 3.8E-05 ];
INF_ABS                   (idx, [1:   4]) = [  3.37084E-02 0.00012  2.64226E-01 4.3E-05 ];
INF_FISS                  (idx, [1:   4]) = [  9.23619E-03 6.0E-05  1.91756E-01 4.4E-05 ];
INF_NSF                   (idx, [1:   4]) = [  2.35338E-02 6.3E-05  4.67253E-01 4.4E-05 ];
INF_NUBAR                 (idx, [1:   4]) = [  2.54800E+00 1.3E-05  2.43670E+00 0.0E+00 ];
INF_KAPPA                 (idx, [1:   4]) = [  2.03546E+02 7.6E-07  2.02270E+02 4.7E-09 ];
INF_INVV                  (idx, [1:   4]) = [  5.07722E-08 9.3E-05  2.04069E-06 3.8E-05 ];

% Total scattering cross sections:

INF_SCATT0                (idx, [1:   4]) = [  1.22038E+00 2.9E-05  2.35152E+00 2.6E-05 ];
INF_SCATT1                (idx, [1:   4]) = [  5.82385E-01 4.9E-05  8.00904E-01 8.8E-05 ];
INF_SCATT2                (idx, [1:   4]) = [  2.29431E-01 8.3E-05  3.01045E-01 0.00027 ];
INF_SCATT3                (idx, [1:   4]) = [  1.89424E-02 0.00086  1.20245E-01 0.00060 ];
INF_SCATT4                (idx, [1:   4]) = [ -2.41062E-02 0.00071  5.49728E-02 0.00111 ];
INF_SCATT5                (idx, [1:   4]) = [  7.69019E-04 0.01760  2.96947E-02 0.00116 ];
INF_SCATT6                (idx, [1:   4]) = [  1.28567E-02 0.00100  1.85514E-02 0.00218 ];
INF_SCATT7                (idx, [1:   4]) = [  2.39081E-03 0.00516  1.28498E-02 0.00281 ];

% Total scattering production cross sections:

INF_SCATTP0               (idx, [1:   4]) = [  1.22053E+00 2.9E-05  2.35152E+00 2.6E-05 ];
INF_SCATTP1               (idx, [1:   4]) = [  5.82388E-01 4.9E-05  8.00904E-01 8.8E-05 ];
INF_SCATTP2               (idx, [1:   4]) = [  2.29432E-01 8.3E-05  3.01045E-01 0.00027 ];
INF_SCATTP3               (idx, [1:   4]) = [  1.89423E-02 0.00086  1.20245E-01 0.00060 ];
INF_SCATTP4               (idx, [1:   4]) = [ -2.41062E-02 0.00071  5.49728E-02 0.00111 ];
INF_SCATTP5               (idx, [1:   4]) = [  7.69132E-04 0.01763  2.96947E-02 0.00116 ];
INF_SCATTP6               (idx, [1:   4]) = [  1.28567E-02 0.00100  1.85514E-02 0.00218 ];
INF_SCATTP7               (idx, [1:   4]) = [  2.39088E-03 0.00514  1.28498E-02 0.00281 ];

% Diffusion parameters:

INF_TRANSPXS              (idx, [1:   4]) = [  4.69015E-01 7.0E-05  1.66271E+00 4.5E-05 ];
INF_DIFFCOEF              (idx, [1:   4]) = [  7.10709E-01 7.0E-05  2.00476E-01 4.5E-05 ];

% Reduced absoption and removal:

INF_RABSXS                (idx, [1:   4]) = [  3.35635E-02 0.00011  2.64226E-01 4.3E-05 ];
INF_REMXS                 (idx, [1:   4]) = [  7.06074E-02 6.9E-05  2.75439E-01 0.00017 ];

% Poison cross sections:

INF_I135_YIELD            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_XE135_YIELD           (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_PM147_YIELD           (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_PM148_YIELD           (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_PM148M_YIELD          (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_PM149_YIELD           (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_SM149_YIELD           (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_I135_MICRO_ABS        (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_XE135_MICRO_ABS       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_PM147_MICRO_ABS       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_PM148_MICRO_ABS       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_PM148M_MICRO_ABS      (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_PM149_MICRO_ABS       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_SM149_MICRO_ABS       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_XE135_MACRO_ABS       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_SM149_MACRO_ABS       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Poison decay constants:

PM147_LAMBDA              (idx, 1)        =  0.00000E+00 ;
PM148_LAMBDA              (idx, 1)        =  0.00000E+00 ;
PM148M_LAMBDA             (idx, 1)        =  0.00000E+00 ;
PM149_LAMBDA              (idx, 1)        =  0.00000E+00 ;
I135_LAMBDA               (idx, 1)        =  0.00000E+00 ;
XE135_LAMBDA              (idx, 1)        =  0.00000E+00 ;
XE135M_LAMBDA             (idx, 1)        =  0.00000E+00 ;
I135_BR                   (idx, 1)        =  0.00000E+00 ;

% Fission spectra:

INF_CHIT                  (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_CHIP                  (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
INF_CHID                  (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Scattering matrixes:

INF_S0                    (idx, [1:   8]) = [  1.18348E+00 2.9E-05  3.69012E-02 0.00014  1.11934E-02 0.00096  2.34033E+00 2.8E-05 ];
INF_S1                    (idx, [1:   8]) = [  5.69594E-01 5.0E-05  1.27918E-02 0.00021  5.84370E-03 0.00110  7.95060E-01 8.8E-05 ];
INF_S2                    (idx, [1:   8]) = [  2.31167E-01 7.9E-05 -1.73597E-03 0.00186  2.85020E-03 0.00178  2.98194E-01 0.00027 ];
INF_S3                    (idx, [1:   8]) = [  2.29530E-02 0.00067 -4.01059E-03 0.00071  9.40241E-04 0.00487  1.19305E-01 0.00061 ];
INF_S4                    (idx, [1:   8]) = [ -2.21986E-02 0.00076 -1.90761E-03 0.00168  3.61245E-05 0.10521  5.49367E-02 0.00111 ];
INF_S5                    (idx, [1:   8]) = [  1.25392E-03 0.01082 -4.84899E-04 0.00406 -2.66575E-04 0.01178  2.99613E-02 0.00119 ];
INF_S6                    (idx, [1:   8]) = [  1.29944E-02 0.00101 -1.37638E-04 0.01594 -3.02994E-04 0.00877  1.88544E-02 0.00218 ];
INF_S7                    (idx, [1:   8]) = [  2.48101E-03 0.00496 -9.01949E-05 0.01982 -2.57581E-04 0.00917  1.31074E-02 0.00279 ];

% Scattering production matrixes:

INF_SP0                   (idx, [1:   8]) = [  1.18363E+00 2.9E-05  3.69012E-02 0.00014  1.11934E-02 0.00096  2.34033E+00 2.8E-05 ];
INF_SP1                   (idx, [1:   8]) = [  5.69596E-01 5.0E-05  1.27918E-02 0.00021  5.84370E-03 0.00110  7.95060E-01 8.8E-05 ];
INF_SP2                   (idx, [1:   8]) = [  2.31168E-01 7.9E-05 -1.73597E-03 0.00186  2.85020E-03 0.00178  2.98194E-01 0.00027 ];
INF_SP3                   (idx, [1:   8]) = [  2.29529E-02 0.00067 -4.01059E-03 0.00071  9.40241E-04 0.00487  1.19305E-01 0.00061 ];
INF_SP4                   (idx, [1:   8]) = [ -2.21986E-02 0.00076 -1.90761E-03 0.00168  3.61245E-05 0.10521  5.49367E-02 0.00111 ];
INF_SP5                   (idx, [1:   8]) = [  1.25403E-03 0.01083 -4.84899E-04 0.00406 -2.66575E-04 0.01178  2.99613E-02 0.00119 ];
INF_SP6                   (idx, [1:   8]) = [  1.29943E-02 0.00102 -1.37638E-04 0.01594 -3.02994E-04 0.00877  1.88544E-02 0.00218 ];
INF_SP7                   (idx, [1:   8]) = [  2.48107E-03 0.00494 -9.01949E-05 0.01982 -2.57581E-04 0.00917  1.31074E-02 0.00279 ];

% Micro-group spectrum:

B1_MICRO_FLX              (idx, [1: 140]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Integral parameters:

B1_KINF                   (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
B1_KEFF                   (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
B1_B2                     (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
B1_ERR                    (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];

% Critical spectra in infinite geometry:

B1_FLX                    (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_FISS_FLX               (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Reaction cross sections:

B1_TOT                    (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_CAPT                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_ABS                    (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_FISS                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_NSF                    (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_NUBAR                  (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_KAPPA                  (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_INVV                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Total scattering cross sections:

B1_SCATT0                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATT1                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATT2                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATT3                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATT4                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATT5                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATT6                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATT7                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Total scattering production cross sections:

B1_SCATTP0                (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATTP1                (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATTP2                (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATTP3                (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATTP4                (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATTP5                (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATTP6                (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SCATTP7                (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Diffusion parameters:

B1_TRANSPXS               (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_DIFFCOEF               (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Reduced absoption and removal:

B1_RABSXS                 (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_REMXS                  (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Poison cross sections:

B1_I135_YIELD             (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_XE135_YIELD            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_PM147_YIELD            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_PM148_YIELD            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_PM148M_YIELD           (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_PM149_YIELD            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SM149_YIELD            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_I135_MICRO_ABS         (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_XE135_MICRO_ABS        (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_PM147_MICRO_ABS        (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_PM148_MICRO_ABS        (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_PM148M_MICRO_ABS       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_PM149_MICRO_ABS        (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SM149_MICRO_ABS        (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_XE135_MACRO_ABS        (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SM149_MACRO_ABS        (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Fission spectra:

B1_CHIT                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_CHIP                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_CHID                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Scattering matrixes:

B1_S0                     (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_S1                     (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_S2                     (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_S3                     (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_S4                     (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_S5                     (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_S6                     (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_S7                     (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Scattering production matrixes:

B1_SP0                    (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SP1                    (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SP2                    (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SP3                    (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SP4                    (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SP5                    (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SP6                    (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
B1_SP7                    (idx, [1:   8]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Additional diffusion parameters:

CMM_TRANSPXS              (idx, [1:   4]) = [  5.07313E-01 0.00015  1.52083E+00 0.00076 ];
CMM_TRANSPXS_X            (idx, [1:   4]) = [  5.07334E-01 0.00021  1.52170E+00 0.00137 ];
CMM_TRANSPXS_Y            (idx, [1:   4]) = [  5.07140E-01 0.00032  1.52152E+00 0.00115 ];
CMM_TRANSPXS_Z            (idx, [1:   4]) = [  5.07469E-01 0.00025  1.51940E+00 0.00148 ];
CMM_DIFFCOEF              (idx, [1:   4]) = [  6.57056E-01 0.00015  2.19181E-01 0.00076 ];
CMM_DIFFCOEF_X            (idx, [1:   4]) = [  6.57031E-01 0.00021  2.19062E-01 0.00137 ];
CMM_DIFFCOEF_Y            (idx, [1:   4]) = [  6.57283E-01 0.00032  2.19085E-01 0.00114 ];
CMM_DIFFCOEF_Z            (idx, [1:   4]) = [  6.56856E-01 0.00025  2.19397E-01 0.00148 ];

% Delayed neutron parameters (Meulekamp method):

BETA_EFF                  (idx, [1:  14]) = [  6.98395E-03 0.00285  2.23683E-04 0.01595  1.17482E-03 0.00671  1.15421E-03 0.00709  2.68797E-03 0.00457  1.22273E-03 0.00628  5.20538E-04 0.01026 ];
LAMBDA                    (idx, [1:  14]) = [  5.11475E-01 0.00399  1.33595E-02 9.8E-05  3.25743E-02 8.9E-05  1.21130E-01 5.0E-05  3.06481E-01 0.00011  8.63941E-01 0.00018  2.90261E+00 0.00028 ];

