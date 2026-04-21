
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
START_DATE                (idx, [1: 24])  = 'Tue Apr 21 21:34:09 2026' ;
COMPLETE_DATE             (idx, [1: 24])  = 'Tue Apr 21 21:41:47 2026' ;

% Run parameters:

POP                       (idx, 1)        = 100000 ;
CYCLES                    (idx, 1)        = 500 ;
SKIP                      (idx, 1)        = 50 ;
BATCH_INTERVAL            (idx, 1)        = 1 ;
SRC_NORM_MODE             (idx, 1)        = 2 ;
SEED                      (idx, 1)        = 1776803649203 ;
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
OMP_THREADS               (idx, 1)        = 16 ;
MPI_REPRODUCIBILITY       (idx, 1)        = 0 ;
OMP_REPRODUCIBILITY       (idx, 1)        = 1 ;
OMP_HISTORY_PROFILE       (idx, [1:  16]) = [  8.98493E-01  1.00687E+00  1.00873E+00  1.00543E+00  1.00420E+00  1.00928E+00  1.00571E+00  1.00535E+00  1.00429E+00  1.01410E+00  1.00641E+00  1.00240E+00  1.00944E+00  1.00085E+00  1.00640E+00  1.01206E+00  ];
SHARE_BUF_ARRAY           (idx, 1)        = 0 ;
SHARE_RES2_ARRAY          (idx, 1)        = 1 ;
OMP_SHARED_QUEUE_LIM      (idx, 1)        = 0 ;

% File paths:

XS_DATA_FILE_PATH         (idx, [1: 43])  = '/Users/shrestha/endfb71/s2v0_endfb71.xsdata' ;
DECAY_DATA_FILE_PATH      (idx, [1:  3])  = 'N/A' ;
SFY_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;
NFY_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;
BRA_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;

% Collision and reaction sampling (neutrons/photons):

MIN_MACROXS               (idx, [1:   4]) = [  5.00000E-02 1.5E-09  0.00000E+00 0.0E+00 ];
DT_THRESH                 (idx, [1:  2])  = [  9.00000E-01  9.00000E-01 ];
ST_FRAC                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_FRAC                   (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_EFF                    (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_EFF          (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_FAIL         (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_COL_EFF               (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKING_LOOPS        (idx, [1:   8]) = [  1.97820E+00 1.0E-05  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKS                (idx, [1:   4]) = [  2.32204E+01 7.4E-05  0.00000E+00 0.0E+00 ];
AVG_REAL_COL              (idx, [1:   4]) = [  2.32204E+01 7.4E-05  0.00000E+00 0.0E+00 ];
AVG_VIRT_COL              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_SURF_CROSS            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
LOST_PARTICLES            (idx, 1)        = 0 ;

% Run statistics:

CYCLE_IDX                 (idx, 1)        = 500 ;
SIMULATED_HISTORIES       (idx, 1)        = 50000476 ;
MEAN_POP_SIZE             (idx, [1:  2])  = [  1.00001E+05 0.00020 ];
MEAN_POP_WGT              (idx, [1:  2])  = [  1.00001E+05 0.00020 ];
SIMULATION_COMPLETED      (idx, 1)        = 1 ;

% Running times:

TOT_CPU_TIME              (idx, 1)        =  7.41644E+01 ;
RUNNING_TIME              (idx, 1)        =  7.64052E+00 ;
INIT_TIME                 (idx, [1:  2])  = [  1.36500E-02  1.36500E-02 ];
PROCESS_TIME              (idx, [1:  2])  = [  1.66667E-04  1.66667E-04 ];
TRANSPORT_CYCLE_TIME      (idx, [1:  3])  = [  7.62668E+00  7.62668E+00  0.00000E+00 ];
MPI_OVERHEAD_TIME         (idx, [1:  2])  = [  0.00000E+00  0.00000E+00 ];
ESTIMATED_RUNNING_TIME    (idx, [1:  2])  = [  7.64022E+00  0.00000E+00 ];
CPU_USAGE                 (idx, 1)        = 9.70673 ;
TRANSPORT_CPU_USAGE       (idx, [1:   2]) = [  9.76427E+00 0.00109 ];
OMP_PARALLEL_FRAC         (idx, 1)        =  9.20396E-01 ;

% Memory usage:

ALLOC_MEMSIZE             (idx, 1)        = 958.90;
MEMSIZE                   (idx, 1)        = 826.53;
XS_MEMSIZE                (idx, 1)        = 122.01;
MAT_MEMSIZE               (idx, 1)        = 34.13;
RES_MEMSIZE               (idx, 1)        = 2.44;
IFC_MEMSIZE               (idx, 1)        = 0.00;
MISC_MEMSIZE              (idx, 1)        = 667.95;
UNKNOWN_MEMSIZE           (idx, 1)        = 0.00;
UNUSED_MEMSIZE            (idx, 1)        = 132.37;

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

NORM_COEF                 (idx, [1:   4]) = [  9.97788E-06 0.00013  0.00000E+00 0.0E+00 ];

% Analog reaction rate estimators:

CONVERSION_RATIO          (idx, [1:   2]) = [  6.10271E-01 0.00030 ];
U235_FISS                 (idx, [1:   4]) = [  4.68251E-01 0.00017  9.26314E-01 5.8E-05 ];
U238_FISS                 (idx, [1:   4]) = [  3.72488E-02 0.00077  7.36863E-02 0.00073 ];
U235_CAPT                 (idx, [1:   4]) = [  1.12991E-01 0.00044  2.28553E-01 0.00038 ];
U238_CAPT                 (idx, [1:   4]) = [  3.54746E-01 0.00025  7.17562E-01 0.00013 ];

% Neutron balance (particles/weight):

BALA_SRC_NEUTRON_SRC     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_FISS    (idx, [1:  2])  = [ 50000476 5.00000E+07 ];
BALA_SRC_NEUTRON_NXN     (idx, [1:  2])  = [ 0 1.04690E+05 ];
BALA_SRC_NEUTRON_VR      (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_TOT     (idx, [1:  2])  = [ 50000476 5.01047E+07 ];

BALA_LOSS_NEUTRON_CAPT    (idx, [1:  2])  = [ 24720370 2.47736E+07 ];
BALA_LOSS_NEUTRON_FISS    (idx, [1:  2])  = [ 25280106 2.53311E+07 ];
BALA_LOSS_NEUTRON_LEAK    (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_CUT     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_ERR     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_TOT     (idx, [1:  2])  = [ 50000476 5.01047E+07 ];

BALA_NEUTRON_DIFF         (idx, [1:  2])  = [ 0 1.38581E-06 ];

% Normalized total reaction rates (neutrons):

TOT_POWER                 (idx, [1:   2]) = [  1.64079E-11 7.6E-05 ];
TOT_POWDENS               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_GENRATE               (idx, [1:   2]) = [  1.24653E+00 7.5E-05 ];
TOT_FISSRATE              (idx, [1:   2]) = [  5.05462E-01 7.6E-05 ];
TOT_CAPTRATE              (idx, [1:   2]) = [  4.94538E-01 7.8E-05 ];
TOT_ABSRATE               (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_SRCRATE               (idx, [1:   2]) = [  9.97788E-01 0.00013 ];
TOT_FLUX                  (idx, [1:   2]) = [  1.64064E+01 0.00011 ];
TOT_PHOTON_PRODRATE       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_LEAKRATE              (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
ALBEDO_LEAKRATE           (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_LOSSRATE              (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_CUTRATE               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_RR                    (idx, [1:   2]) = [  2.32159E+01 7.9E-05 ];
INI_FMASS                 (idx, 1)        =  0.00000E+00 ;
TOT_FMASS                 (idx, 1)        =  0.00000E+00 ;

% Six-factor formula:

SIX_FF_ETA                (idx, [1:   2]) = [  1.76868E+00 0.00012 ];
SIX_FF_F                  (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_P                  (idx, [1:   2]) = [  5.13405E-01 0.00014 ];
SIX_FF_EPSILON            (idx, [1:   2]) = [  1.37591E+00 0.00013 ];
SIX_FF_LF                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_LT                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_KINF               (idx, [1:   2]) = [  1.24938E+00 0.00014 ];
SIX_FF_KEFF               (idx, [1:   2]) = [  1.24938E+00 0.00014 ];

% Fission neutron and energy production:

NUBAR                     (idx, [1:   2]) = [  2.46611E+00 4.5E-06 ];
FISSE                     (idx, [1:   2]) = [  2.02607E+02 4.5E-07 ];

% Criticality eigenvalues:

ANA_KEFF                  (idx, [1:   6]) = [  1.24934E+00 0.00014  1.24077E+00 0.00014  8.61581E-03 0.00239 ];
IMP_KEFF                  (idx, [1:   2]) = [  1.24914E+00 7.5E-05 ];
COL_KEFF                  (idx, [1:   2]) = [  1.24930E+00 0.00018 ];
ABS_KEFF                  (idx, [1:   2]) = [  1.24914E+00 7.5E-05 ];
ABS_KINF                  (idx, [1:   2]) = [  1.24914E+00 7.5E-05 ];
GEOM_ALBEDO               (idx, [1:   6]) = [  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00 ];

% ALF (Average lethargy of neutrons causing fission):
% Based on E0 = 2.000000E+01 MeV

ANA_ALF                   (idx, [1:   2]) = [  1.66282E+01 6.5E-05 ];
IMP_ALF                   (idx, [1:   2]) = [  1.66293E+01 3.2E-05 ];

% EALF (Energy corresponding to average lethargy of neutrons causing fission):

ANA_EALF                  (idx, [1:   2]) = [  1.20123E-06 0.00107 ];
IMP_EALF                  (idx, [1:   2]) = [  1.19962E-06 0.00054 ];

% AFGE (Average energy of neutrons causing fission):

ANA_AFGE                  (idx, [1:   2]) = [  2.60213E-01 0.00079 ];
IMP_AFGE                  (idx, [1:   2]) = [  2.60203E-01 0.00034 ];

% Forward-weighted delayed neutron parameters:

PRECURSOR_GROUPS          (idx, 1)        = 6 ;
FWD_ANA_BETA_ZERO         (idx, [1:  14]) = [  5.81163E-03 0.00174  1.82935E-04 0.00906  9.81788E-04 0.00415  9.55892E-04 0.00420  2.24036E-03 0.00278  1.02994E-03 0.00403  4.20716E-04 0.00620 ];
FWD_ANA_LAMBDA            (idx, [1:  14]) = [  5.07064E-01 0.00239  1.33602E-02 5.8E-05  3.25760E-02 5.8E-05  1.21132E-01 3.1E-05  3.06439E-01 7.1E-05  8.63864E-01 0.00012  2.90053E+00 0.00019 ];

% Beta-eff using Meulekamp's method:

ADJ_MEULEKAMP_BETA_EFF    (idx, [1:  14]) = [  6.96608E-03 0.00265  2.21430E-04 0.01546  1.18703E-03 0.00653  1.14142E-03 0.00664  2.68431E-03 0.00439  1.23421E-03 0.00617  4.97692E-04 0.00996 ];
ADJ_MEULEKAMP_LAMBDA      (idx, [1:  14]) = [  5.04277E-01 0.00395  1.33605E-02 9.9E-05  3.25782E-02 8.9E-05  1.21126E-01 4.9E-05  3.06432E-01 0.00011  8.63952E-01 0.00019  2.90092E+00 0.00031 ];

% Adjoint weighted time constants using Nauchi's method:

IFP_CHAIN_LENGTH          (idx, 1)        = 15 ;
ADJ_NAUCHI_GEN_TIME       (idx, [1:   6]) = [  5.26287E-06 0.00032  5.25939E-06 0.00032  5.76511E-06 0.00305 ];
ADJ_NAUCHI_LIFETIME       (idx, [1:   6]) = [  6.57505E-06 0.00029  6.57071E-06 0.00029  7.20276E-06 0.00307 ];
ADJ_NAUCHI_BETA_EFF       (idx, [1:  14]) = [  6.89487E-03 0.00240  2.19665E-04 0.01287  1.17006E-03 0.00567  1.13122E-03 0.00592  2.64614E-03 0.00392  1.22852E-03 0.00605  4.99264E-04 0.00850 ];
ADJ_NAUCHI_LAMBDA         (idx, [1:  14]) = [  5.07439E-01 0.00336  1.33603E-02 8.8E-05  3.25774E-02 8.1E-05  1.21129E-01 4.4E-05  3.06476E-01 0.00010  8.63877E-01 0.00017  2.90126E+00 0.00026 ];

% Adjoint weighted time constants using IFP:

ADJ_IFP_GEN_TIME          (idx, [1:   6]) = [  5.26613E-06 0.00068  5.26254E-06 0.00068  5.77658E-06 0.00741 ];
ADJ_IFP_LIFETIME          (idx, [1:   6]) = [  6.57915E-06 0.00067  6.57467E-06 0.00068  7.21718E-06 0.00742 ];
ADJ_IFP_IMP_BETA_EFF      (idx, [1:  14]) = [  6.89630E-03 0.00722  2.16197E-04 0.04040  1.17402E-03 0.01721  1.13227E-03 0.01769  2.67243E-03 0.01178  1.20237E-03 0.01781  4.99019E-04 0.02674 ];
ADJ_IFP_IMP_LAMBDA        (idx, [1:  14]) = [  5.04386E-01 0.01009  1.33630E-02 0.00024  3.25706E-02 0.00025  1.21120E-01 0.00013  3.06368E-01 0.00031  8.63710E-01 0.00049  2.89692E+00 0.00073 ];
ADJ_IFP_ANA_BETA_EFF      (idx, [1:  14]) = [  6.89199E-03 0.00692  2.20523E-04 0.03813  1.16653E-03 0.01593  1.12927E-03 0.01707  2.67042E-03 0.01109  1.20506E-03 0.01693  5.00181E-04 0.02532 ];
ADJ_IFP_ANA_LAMBDA        (idx, [1:  14]) = [  5.05253E-01 0.00960  1.33630E-02 0.00024  3.25720E-02 0.00024  1.21122E-01 0.00012  3.06387E-01 0.00030  8.63911E-01 0.00047  2.89800E+00 0.00071 ];
ADJ_IFP_ROSSI_ALPHA       (idx, [1:   2]) = [ -1.31071E+03 0.00724 ];

% Adjoint weighted time constants using perturbation technique:

ADJ_PERT_GEN_TIME         (idx, [1:   2]) = [  5.26937E-06 0.00019 ];
ADJ_PERT_LIFETIME         (idx, [1:   2]) = [  6.58318E-06 0.00013 ];
ADJ_PERT_BETA_EFF         (idx, [1:   2]) = [  6.88940E-03 0.00143 ];
ADJ_PERT_ROSSI_ALPHA      (idx, [1:   2]) = [ -1.30745E+03 0.00143 ];

% Inverse neutron speed :

ANA_INV_SPD               (idx, [1:   2]) = [  2.85977E-07 0.00018 ];

% Analog slowing-down and thermal neutron lifetime (total/prompt/delayed):

ANA_SLOW_TIME             (idx, [1:   6]) = [  1.16273E-06 0.00014  1.16273E-06 0.00014  1.16258E-06 0.00169 ];
ANA_THERM_TIME            (idx, [1:   6]) = [  7.73414E-06 0.00020  7.73427E-06 0.00020  7.71780E-06 0.00234 ];
ANA_THERM_FRAC            (idx, [1:   6]) = [  5.14521E-01 0.00014  5.13608E-01 0.00014  6.71777E-01 0.00287 ];
ANA_DELAYED_EMTIME        (idx, [1:   2]) = [  1.04212E+01 0.00408 ];
ANA_MEAN_NCOL             (idx, [1:   4]) = [  2.32204E+01 7.4E-05  2.60840E+01 1.0E-04 ];

% Group constant generation:

GC_UNIVERSE_NAME          (idx, [1:  1])  = '0' ;

% Micro- and macro-group structures:

MICRO_NG                  (idx, 1)        = 70 ;
MICRO_E                   (idx, [1:  71]) = [  2.00000E+01  6.06550E+00  3.67900E+00  2.23100E+00  1.35300E+00  8.21000E-01  5.00000E-01  3.02500E-01  1.83000E-01  1.11000E-01  6.74300E-02  4.08500E-02  2.47800E-02  1.50300E-02  9.11800E-03  5.50000E-03  3.51910E-03  2.23945E-03  1.42510E-03  9.06898E-04  3.67262E-04  1.48728E-04  7.55014E-05  4.80520E-05  2.77000E-05  1.59680E-05  9.87700E-06  4.00000E-06  3.30000E-06  2.60000E-06  2.10000E-06  1.85500E-06  1.50000E-06  1.30000E-06  1.15000E-06  1.12300E-06  1.09700E-06  1.07100E-06  1.04500E-06  1.02000E-06  9.96000E-07  9.72000E-07  9.50000E-07  9.10000E-07  8.50000E-07  7.80000E-07  6.25000E-07  5.00000E-07  4.00000E-07  3.50000E-07  3.20000E-07  3.00000E-07  2.80000E-07  2.50000E-07  2.20000E-07  1.80000E-07  1.40000E-07  1.00000E-07  8.00000E-08  6.70000E-08  5.80000E-08  5.00000E-08  4.20000E-08  3.50000E-08  3.00000E-08  2.50000E-08  2.00000E-08  1.50000E-08  1.00000E-08  5.00000E-09  1.00000E-11 ];

MACRO_NG                  (idx, 1)        = 2 ;
MACRO_E                   (idx, [1:   3]) = [  1.00000E+37  6.25000E-07  0.00000E+00 ];

% Micro-group spectrum:

INF_MICRO_FLX             (idx, [1: 140]) = [  3.09806E+05 0.00084  1.21404E+06 0.00053  2.36050E+06 0.00026  2.51074E+06 0.00028  2.37819E+06 0.00017  2.48538E+06 0.00026  1.78273E+06 0.00023  1.52943E+06 0.00022  1.18487E+06 0.00019  9.72317E+05 0.00022  8.43052E+05 0.00022  7.54770E+05 0.00027  7.00183E+05 0.00025  6.64806E+05 0.00023  6.48057E+05 0.00022  5.57814E+05 0.00028  5.51165E+05 0.00030  5.42579E+05 0.00020  5.30662E+05 0.00024  1.02583E+06 0.00018  9.76420E+05 0.00016  6.87309E+05 0.00024  4.35078E+05 0.00030  4.85109E+05 0.00033  4.38926E+05 0.00025  4.03584E+05 0.00028  6.12656E+05 0.00017  1.41789E+05 0.00042  1.77108E+05 0.00043  1.60555E+05 0.00045  9.25591E+04 0.00058  1.61126E+05 0.00049  1.09031E+05 0.00062  9.17399E+04 0.00045  1.73802E+04 0.00117  1.71419E+04 0.00117  1.76775E+04 0.00122  1.82424E+04 0.00106  1.80953E+04 0.00095  1.79698E+04 0.00120  1.84405E+04 0.00088  1.74192E+04 0.00102  3.29634E+04 0.00082  5.28207E+04 0.00090  6.76527E+04 0.00066  1.83424E+05 0.00050  2.08876E+05 0.00033  2.51198E+05 0.00042  1.76172E+05 0.00040  1.29430E+05 0.00054  9.87098E+04 0.00040  1.10786E+05 0.00056  1.93633E+05 0.00033  2.33747E+05 0.00036  3.85030E+05 0.00031  4.74384E+05 0.00032  5.50504E+05 0.00021  2.88086E+05 0.00028  1.83577E+05 0.00040  1.22042E+05 0.00042  1.02983E+05 0.00044  9.57658E+04 0.00036  7.61557E+04 0.00051  4.90596E+04 0.00063  4.38299E+04 0.00057  3.79065E+04 0.00059  3.11201E+04 0.00061  2.34547E+04 0.00055  1.48210E+04 0.00077  5.18225E+03 0.00093 ];

% Integral parameters:

INF_KINF                  (idx, [1:   2]) = [  1.24930E+00 0.00017 ];

% Flux spectra in infinite geometry:

INF_FLX                   (idx, [1:   4]) = [  1.44675E+01 9.7E-05  1.93893E+00 1.0E-04 ];
INF_FISS_FLX              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Reaction cross sections:

INF_TOT                   (idx, [1:   4]) = [  1.25412E+00 2.7E-05  2.61590E+00 1.9E-05 ];
INF_CAPT                  (idx, [1:   4]) = [  2.44696E-02 0.00013  7.24769E-02 4.0E-05 ];
INF_ABS                   (idx, [1:   4]) = [  3.37057E-02 9.6E-05  2.64255E-01 4.4E-05 ];
INF_FISS                  (idx, [1:   4]) = [  9.23611E-03 5.6E-05  1.91778E-01 4.6E-05 ];
INF_NSF                   (idx, [1:   4]) = [  2.35332E-02 6.1E-05  4.67306E-01 4.6E-05 ];
INF_NUBAR                 (idx, [1:   4]) = [  2.54796E+00 1.4E-05  2.43670E+00 3.8E-09 ];
INF_KAPPA                 (idx, [1:   4]) = [  2.03546E+02 1.2E-06  2.02270E+02 2.7E-09 ];
INF_INVV                  (idx, [1:   4]) = [  5.07778E-08 9.4E-05  2.04094E-06 4.3E-05 ];

% Total scattering cross sections:

INF_SCATT0                (idx, [1:   4]) = [  1.22042E+00 2.9E-05  2.35170E+00 2.8E-05 ];
INF_SCATT1                (idx, [1:   4]) = [  5.82400E-01 5.2E-05  8.00906E-01 0.00011 ];
INF_SCATT2                (idx, [1:   4]) = [  2.29432E-01 8.0E-05  3.00883E-01 0.00028 ];
INF_SCATT3                (idx, [1:   4]) = [  1.89510E-02 0.00111  1.20061E-01 0.00047 ];
INF_SCATT4                (idx, [1:   4]) = [ -2.40916E-02 0.00076  5.48276E-02 0.00084 ];
INF_SCATT5                (idx, [1:   4]) = [  7.80022E-04 0.01921  2.96227E-02 0.00142 ];
INF_SCATT6                (idx, [1:   4]) = [  1.28731E-02 0.00086  1.84918E-02 0.00219 ];
INF_SCATT7                (idx, [1:   4]) = [  2.40245E-03 0.00546  1.29350E-02 0.00321 ];

% Total scattering production cross sections:

INF_SCATTP0               (idx, [1:   4]) = [  1.22056E+00 2.9E-05  2.35170E+00 2.8E-05 ];
INF_SCATTP1               (idx, [1:   4]) = [  5.82402E-01 5.2E-05  8.00906E-01 0.00011 ];
INF_SCATTP2               (idx, [1:   4]) = [  2.29432E-01 8.0E-05  3.00883E-01 0.00028 ];
INF_SCATTP3               (idx, [1:   4]) = [  1.89510E-02 0.00111  1.20061E-01 0.00047 ];
INF_SCATTP4               (idx, [1:   4]) = [ -2.40917E-02 0.00076  5.48276E-02 0.00084 ];
INF_SCATTP5               (idx, [1:   4]) = [  7.79935E-04 0.01928  2.96227E-02 0.00142 ];
INF_SCATTP6               (idx, [1:   4]) = [  1.28730E-02 0.00086  1.84918E-02 0.00219 ];
INF_SCATTP7               (idx, [1:   4]) = [  2.40233E-03 0.00546  1.29350E-02 0.00321 ];

% Diffusion parameters:

INF_TRANSPXS              (idx, [1:   4]) = [  4.69011E-01 8.0E-05  1.66297E+00 5.7E-05 ];
INF_DIFFCOEF              (idx, [1:   4]) = [  7.10716E-01 8.0E-05  2.00445E-01 5.7E-05 ];

% Reduced absoption and removal:

INF_RABSXS                (idx, [1:   4]) = [  3.35613E-02 9.7E-05  2.64255E-01 4.4E-05 ];
INF_REMXS                 (idx, [1:   4]) = [  7.06156E-02 6.7E-05  2.75422E-01 0.00011 ];

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

INF_S0                    (idx, [1:   8]) = [  1.18350E+00 2.7E-05  3.69119E-02 0.00013  1.12194E-02 0.00087  2.34048E+00 2.8E-05 ];
INF_S1                    (idx, [1:   8]) = [  5.69609E-01 4.9E-05  1.27906E-02 0.00037  5.86028E-03 0.00119  7.95046E-01 0.00011 ];
INF_S2                    (idx, [1:   8]) = [  2.31167E-01 7.7E-05 -1.73540E-03 0.00184  2.86636E-03 0.00185  2.98016E-01 0.00028 ];
INF_S3                    (idx, [1:   8]) = [  2.29583E-02 0.00091 -4.00732E-03 0.00062  9.46203E-04 0.00343  1.19115E-01 0.00047 ];
INF_S4                    (idx, [1:   8]) = [ -2.21810E-02 0.00081 -1.91062E-03 0.00121  4.03231E-05 0.10204  5.47873E-02 0.00083 ];
INF_S5                    (idx, [1:   8]) = [  1.26540E-03 0.01147 -4.85377E-04 0.00443 -2.67403E-04 0.01179  2.98901E-02 0.00139 ];
INF_S6                    (idx, [1:   8]) = [  1.30075E-02 0.00081 -1.34411E-04 0.01428 -3.10232E-04 0.00817  1.88020E-02 0.00211 ];
INF_S7                    (idx, [1:   8]) = [  2.49023E-03 0.00497 -8.77749E-05 0.01398 -2.63266E-04 0.01315  1.31982E-02 0.00304 ];

% Scattering production matrixes:

INF_SP0                   (idx, [1:   8]) = [  1.18365E+00 2.7E-05  3.69119E-02 0.00013  1.12194E-02 0.00087  2.34048E+00 2.8E-05 ];
INF_SP1                   (idx, [1:   8]) = [  5.69611E-01 4.9E-05  1.27906E-02 0.00037  5.86028E-03 0.00119  7.95046E-01 0.00011 ];
INF_SP2                   (idx, [1:   8]) = [  2.31168E-01 7.8E-05 -1.73540E-03 0.00184  2.86636E-03 0.00185  2.98016E-01 0.00028 ];
INF_SP3                   (idx, [1:   8]) = [  2.29583E-02 0.00091 -4.00732E-03 0.00062  9.46203E-04 0.00343  1.19115E-01 0.00047 ];
INF_SP4                   (idx, [1:   8]) = [ -2.21810E-02 0.00081 -1.91062E-03 0.00121  4.03231E-05 0.10204  5.47873E-02 0.00083 ];
INF_SP5                   (idx, [1:   8]) = [  1.26531E-03 0.01151 -4.85377E-04 0.00443 -2.67403E-04 0.01179  2.98901E-02 0.00139 ];
INF_SP6                   (idx, [1:   8]) = [  1.30075E-02 0.00081 -1.34411E-04 0.01428 -3.10232E-04 0.00817  1.88020E-02 0.00211 ];
INF_SP7                   (idx, [1:   8]) = [  2.49010E-03 0.00497 -8.77749E-05 0.01398 -2.63266E-04 0.01315  1.31982E-02 0.00304 ];

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

CMM_TRANSPXS              (idx, [1:   4]) = [  5.07368E-01 0.00016  1.52020E+00 0.00099 ];
CMM_TRANSPXS_X            (idx, [1:   4]) = [  5.07074E-01 0.00027  1.52246E+00 0.00126 ];
CMM_TRANSPXS_Y            (idx, [1:   4]) = [  5.07546E-01 0.00022  1.51960E+00 0.00132 ];
CMM_TRANSPXS_Z            (idx, [1:   4]) = [  5.07486E-01 0.00036  1.51870E+00 0.00182 ];
CMM_DIFFCOEF              (idx, [1:   4]) = [  6.56986E-01 0.00016  2.19274E-01 0.00099 ];
CMM_DIFFCOEF_X            (idx, [1:   4]) = [  6.57368E-01 0.00027  2.18952E-01 0.00126 ];
CMM_DIFFCOEF_Y            (idx, [1:   4]) = [  6.56756E-01 0.00022  2.19366E-01 0.00132 ];
CMM_DIFFCOEF_Z            (idx, [1:   4]) = [  6.56834E-01 0.00036  2.19503E-01 0.00183 ];

% Delayed neutron parameters (Meulekamp method):

BETA_EFF                  (idx, [1:  14]) = [  6.96608E-03 0.00265  2.21430E-04 0.01546  1.18703E-03 0.00653  1.14142E-03 0.00664  2.68431E-03 0.00439  1.23421E-03 0.00617  4.97692E-04 0.00996 ];
LAMBDA                    (idx, [1:  14]) = [  5.04277E-01 0.00395  1.33605E-02 9.9E-05  3.25782E-02 8.9E-05  1.21126E-01 4.9E-05  3.06432E-01 0.00011  8.63952E-01 0.00019  2.90092E+00 0.00031 ];

