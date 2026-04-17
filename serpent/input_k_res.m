
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
TITLE                     (idx, [1: 31])  = 'micromc validation k-eigenvalue' ;
CONFIDENTIAL_DATA         (idx, 1)        = 0 ;
INPUT_FILE_NAME           (idx, [1:  7])  = 'input_k' ;
WORKING_DIRECTORY         (idx, [1: 31])  = '/Users/shrestha/micromc/serpent' ;
HOSTNAME                  (idx, [1: 18])  = 'manabs-macbook-pro' ;
CPU_TYPE                  (idx, [1:  7])  = 'Unknown' ;
START_DATE                (idx, [1: 24])  = 'Sat Apr 11 21:36:42 2026' ;
COMPLETE_DATE             (idx, [1: 24])  = 'Sat Apr 11 21:39:25 2026' ;

% Run parameters:

POP                       (idx, 1)        = 10000 ;
CYCLES                    (idx, 1)        = 1000 ;
SKIP                      (idx, 1)        = 100 ;
BATCH_INTERVAL            (idx, 1)        = 1 ;
SRC_NORM_MODE             (idx, 1)        = 2 ;
SEED                      (idx, 1)        = 1775939802269 ;
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
OMP_THREADS               (idx, 1)        = 2 ;
MPI_REPRODUCIBILITY       (idx, 1)        = 0 ;
OMP_REPRODUCIBILITY       (idx, 1)        = 1 ;
OMP_HISTORY_PROFILE       (idx, [1:   2]) = [  1.00026E+00  9.99744E-01  ];
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

MIN_MACROXS               (idx, [1:   4]) = [  5.00000E-02 3.4E-09  0.00000E+00 0.0E+00 ];
DT_THRESH                 (idx, [1:  2])  = [  9.00000E-01  9.00000E-01 ];
ST_FRAC                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_FRAC                   (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_EFF                    (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_EFF          (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_FAIL         (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_COL_EFF               (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKING_LOOPS        (idx, [1:   8]) = [  1.97828E+00 2.3E-05  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKS                (idx, [1:   4]) = [  2.32258E+01 0.00017  0.00000E+00 0.0E+00 ];
AVG_REAL_COL              (idx, [1:   4]) = [  2.32258E+01 0.00017  0.00000E+00 0.0E+00 ];
AVG_VIRT_COL              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_SURF_CROSS            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
LOST_PARTICLES            (idx, 1)        = 0 ;

% Run statistics:

CYCLE_IDX                 (idx, 1)        = 1000 ;
SIMULATED_HISTORIES       (idx, 1)        = 10000951 ;
MEAN_POP_SIZE             (idx, [1:  2])  = [  1.00010E+04 0.00045 ];
MEAN_POP_WGT              (idx, [1:  2])  = [  1.00010E+04 0.00045 ];
SIMULATION_COMPLETED      (idx, 1)        = 1 ;

% Running times:

TOT_CPU_TIME              (idx, 1)        =  5.39947E+00 ;
RUNNING_TIME              (idx, 1)        =  2.72387E+00 ;
INIT_TIME                 (idx, [1:  2])  = [  1.05000E-02  1.05000E-02 ];
PROCESS_TIME              (idx, [1:  2])  = [  6.66698E-05  6.66698E-05 ];
TRANSPORT_CYCLE_TIME      (idx, [1:  3])  = [  2.71330E+00  2.71330E+00  0.00000E+00 ];
MPI_OVERHEAD_TIME         (idx, [1:  2])  = [  0.00000E+00  0.00000E+00 ];
ESTIMATED_RUNNING_TIME    (idx, [1:  2])  = [  2.72382E+00  0.00000E+00 ];
CPU_USAGE                 (idx, 1)        = 1.98228 ;
TRANSPORT_CPU_USAGE       (idx, [1:   2]) = [  1.98685E+00 9.8E-05 ];
OMP_PARALLEL_FRAC         (idx, 1)        =  9.84489E-01 ;

% Memory usage:

ALLOC_MEMSIZE             (idx, 1)        = 193.51;
MEMSIZE                   (idx, 1)        = 138.44;
XS_MEMSIZE                (idx, 1)        = 60.83;
MAT_MEMSIZE               (idx, 1)        = 9.55;
RES_MEMSIZE               (idx, 1)        = 1.18;
IFC_MEMSIZE               (idx, 1)        = 0.00;
MISC_MEMSIZE              (idx, 1)        = 66.88;
UNKNOWN_MEMSIZE           (idx, 1)        = 0.00;
UNUSED_MEMSIZE            (idx, 1)        = 55.08;

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

NORM_COEF                 (idx, [1:   4]) = [  6.07975E-06 0.00015  0.00000E+00 0.0E+00 ];

% Analog reaction rate estimators:

CONVERSION_RATIO          (idx, [1:   2]) = [  6.10324E-01 0.00069 ];
U235_FISS                 (idx, [1:   4]) = [  2.85407E-02 0.00033  9.26635E-01 0.00012 ];
U238_FISS                 (idx, [1:   4]) = [  2.25986E-03 0.00165  7.33653E-02 0.00157 ];
U235_CAPT                 (idx, [1:   4]) = [  6.88236E-03 0.00089  2.28463E-01 0.00083 ];
U238_CAPT                 (idx, [1:   4]) = [  2.16186E-02 0.00050  7.17620E-01 0.00029 ];

% Neutron balance (particles/weight):

BALA_SRC_NEUTRON_SRC     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_FISS    (idx, [1:  2])  = [ 10000951 1.00000E+07 ];
BALA_SRC_NEUTRON_NXN     (idx, [1:  2])  = [ 0 2.10777E+04 ];
BALA_SRC_NEUTRON_VR      (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_TOT     (idx, [1:  2])  = [ 10000951 1.00211E+07 ];

BALA_LOSS_NEUTRON_CAPT    (idx, [1:  2])  = [ 4944702 4.95494E+06 ];
BALA_LOSS_NEUTRON_FISS    (idx, [1:  2])  = [ 5056249 5.06613E+06 ];
BALA_LOSS_NEUTRON_LEAK    (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_CUT     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_ERR     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_TOT     (idx, [1:  2])  = [ 10000951 1.00211E+07 ];

BALA_NEUTRON_DIFF         (idx, [1:  2])  = [ 0 -9.31323E-08 ];

% Normalized total reaction rates (neutrons):

TOT_POWER                 (idx, [1:   2]) = [  9.99900E-13 0.00034 ];
TOT_POWDENS               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_GENRATE               (idx, [1:   2]) = [  7.59636E-02 0.00034 ];
TOT_FISSRATE              (idx, [1:   2]) = [  3.08028E-02 0.00034 ];
TOT_CAPTRATE              (idx, [1:   2]) = [  3.01296E-02 0.00024 ];
TOT_ABSRATE               (idx, [1:   2]) = [  6.09324E-02 0.00023 ];
TOT_SRCRATE               (idx, [1:   2]) = [  6.07975E-02 0.00015 ];
TOT_FLUX                  (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_PHOTON_PRODRATE       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_LEAKRATE              (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
ALBEDO_LEAKRATE           (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_LOSSRATE              (idx, [1:   2]) = [  6.09324E-02 0.00023 ];
TOT_CUTRATE               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_RR                    (idx, [1:   2]) = [  1.41493E+00 8.6E-05 ];
INI_FMASS                 (idx, 1)        =  0.00000E+00 ;
TOT_FMASS                 (idx, 1)        =  0.00000E+00 ;

% Six-factor formula:

SIX_FF_ETA                (idx, [1:   2]) = [  1.76849E+00 0.00026 ];
SIX_FF_F                  (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_P                  (idx, [1:   2]) = [  5.13681E-01 0.00032 ];
SIX_FF_EPSILON            (idx, [1:   2]) = [  1.37533E+00 0.00027 ];
SIX_FF_LF                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_LT                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_KINF               (idx, [1:   2]) = [  1.24931E+00 0.00032 ];
SIX_FF_KEFF               (idx, [1:   2]) = [  1.24931E+00 0.00032 ];

% Fission neutron and energy production:

NUBAR                     (idx, [1:   2]) = [  2.46613E+00 9.6E-06 ];
FISSE                     (idx, [1:   2]) = [  2.02607E+02 9.5E-07 ];

% Criticality eigenvalues:

ANA_KEFF                  (idx, [1:   6]) = [  1.24936E+00 0.00033  1.24071E+00 0.00033  8.60674E-03 0.00518 ];
IMP_KEFF                  (idx, [1:   2]) = [  1.24928E+00 0.00018 ];
COL_KEFF                  (idx, [1:   2]) = [  1.24950E+00 0.00041 ];
ABS_KEFF                  (idx, [1:   2]) = [  1.24928E+00 0.00018 ];
ABS_KINF                  (idx, [1:   2]) = [  1.24928E+00 0.00018 ];
GEOM_ALBEDO               (idx, [1:   6]) = [  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00 ];

% ALF (Average lethargy of neutrons causing fission):
% Based on E0 = 2.000000E+01 MeV

ANA_ALF                   (idx, [1:   2]) = [  1.66340E+01 0.00014 ];
IMP_ALF                   (idx, [1:   2]) = [  1.66281E+01 7.0E-05 ];

% EALF (Energy corresponding to average lethargy of neutrons causing fission):

ANA_EALF                  (idx, [1:   2]) = [  1.19697E-06 0.00227 ];
IMP_EALF                  (idx, [1:   2]) = [  1.20185E-06 0.00117 ];

% AFGE (Average energy of neutrons causing fission):

ANA_AFGE                  (idx, [1:   2]) = [  2.59371E-01 0.00169 ];
IMP_AFGE                  (idx, [1:   2]) = [  2.60326E-01 0.00072 ];

% Forward-weighted delayed neutron parameters:

PRECURSOR_GROUPS          (idx, 1)        = 6 ;
FWD_ANA_BETA_ZERO         (idx, [1:  14]) = [  5.78972E-03 0.00386  1.82140E-04 0.02127  9.79520E-04 0.00923  9.63185E-04 0.00910  2.21965E-03 0.00612  1.02547E-03 0.00887  4.19756E-04 0.01398 ];
FWD_ANA_LAMBDA            (idx, [1:  14]) = [  5.07013E-01 0.00540  1.20135E-02 0.01061  3.25757E-02 0.00014  1.21137E-01 7.0E-05  3.06422E-01 0.00016  8.64009E-01 0.00027  2.88361E+00 0.00250 ];

% Beta-eff using Meulekamp's method:

ADJ_MEULEKAMP_BETA_EFF    (idx, [1:  14]) = [  6.97205E-03 0.00602  2.09497E-04 0.03538  1.16858E-03 0.01485  1.15103E-03 0.01545  2.68849E-03 0.01006  1.24226E-03 0.01446  5.12190E-04 0.02309 ];
ADJ_MEULEKAMP_LAMBDA      (idx, [1:  14]) = [  5.10786E-01 0.00886  1.33642E-02 0.00018  3.25684E-02 0.00021  1.21128E-01 0.00011  3.06387E-01 0.00026  8.63918E-01 0.00041  2.90014E+00 0.00060 ];

% Adjoint weighted time constants using Nauchi's method:

IFP_CHAIN_LENGTH          (idx, 1)        = 15 ;
ADJ_NAUCHI_GEN_TIME       (idx, [1:   6]) = [  5.26485E-06 0.00071  5.26164E-06 0.00072  5.73003E-06 0.00703 ];
ADJ_NAUCHI_LIFETIME       (idx, [1:   6]) = [  6.57703E-06 0.00064  6.57301E-06 0.00064  7.15837E-06 0.00702 ];
ADJ_NAUCHI_BETA_EFF       (idx, [1:  14]) = [  6.88673E-03 0.00519  2.15050E-04 0.03041  1.17441E-03 0.01294  1.13882E-03 0.01297  2.64899E-03 0.00839  1.21275E-03 0.01257  4.96708E-04 0.02025 ];
ADJ_NAUCHI_LAMBDA         (idx, [1:  14]) = [  5.05330E-01 0.00791  1.33641E-02 0.00022  3.25748E-02 0.00020  1.21135E-01 0.00011  3.06439E-01 0.00024  8.63946E-01 0.00040  2.89993E+00 0.00066 ];

% Adjoint weighted time constants using IFP:

ADJ_IFP_GEN_TIME          (idx, [1:   6]) = [  5.27090E-06 0.00154  5.26694E-06 0.00155  5.83336E-06 0.01687 ];
ADJ_IFP_LIFETIME          (idx, [1:   6]) = [  6.58452E-06 0.00150  6.57957E-06 0.00150  7.28802E-06 0.01688 ];
ADJ_IFP_IMP_BETA_EFF      (idx, [1:  14]) = [  7.00762E-03 0.01534  2.03927E-04 0.09457  1.20842E-03 0.03815  1.12741E-03 0.04039  2.72674E-03 0.02489  1.25329E-03 0.03878  4.87819E-04 0.06034 ];
ADJ_IFP_IMP_LAMBDA        (idx, [1:  14]) = [  4.99255E-01 0.02308  1.33632E-02 0.00046  3.25947E-02 0.00043  1.21097E-01 0.00024  3.06389E-01 0.00063  8.64107E-01 0.00096  2.89616E+00 0.00135 ];
ADJ_IFP_ANA_BETA_EFF      (idx, [1:  14]) = [  7.03440E-03 0.01459  2.07401E-04 0.08988  1.19456E-03 0.03667  1.13533E-03 0.03912  2.74946E-03 0.02374  1.25213E-03 0.03684  4.95522E-04 0.05885 ];
ADJ_IFP_ANA_LAMBDA        (idx, [1:  14]) = [  5.02097E-01 0.02266  1.33632E-02 0.00046  3.25964E-02 0.00043  1.21092E-01 0.00024  3.06443E-01 0.00062  8.64035E-01 0.00095  2.89640E+00 0.00134 ];
ADJ_IFP_ROSSI_ALPHA       (idx, [1:   2]) = [ -1.33386E+03 0.01543 ];

% Adjoint weighted time constants using perturbation technique:

ADJ_PERT_GEN_TIME         (idx, [1:   2]) = [  5.26885E-06 0.00044 ];
ADJ_PERT_LIFETIME         (idx, [1:   2]) = [  6.58201E-06 0.00029 ];
ADJ_PERT_BETA_EFF         (idx, [1:   2]) = [  6.97074E-03 0.00283 ];
ADJ_PERT_ROSSI_ALPHA      (idx, [1:   2]) = [ -1.32330E+03 0.00287 ];

% Inverse neutron speed :

ANA_INV_SPD               (idx, [1:   2]) = [  2.85914E-07 0.00040 ];

% Analog slowing-down and thermal neutron lifetime (total/prompt/delayed):

ANA_SLOW_TIME             (idx, [1:   6]) = [  1.16297E-06 0.00032  1.16298E-06 0.00032  1.16148E-06 0.00352 ];
ANA_THERM_TIME            (idx, [1:   6]) = [  7.73013E-06 0.00044  7.73006E-06 0.00044  7.74179E-06 0.00514 ];
ANA_THERM_FRAC            (idx, [1:   6]) = [  5.14790E-01 0.00032  5.13878E-01 0.00032  6.82084E-01 0.00651 ];
ANA_DELAYED_EMTIME        (idx, [1:   2]) = [  1.05425E+01 0.00903 ];
ANA_MEAN_NCOL             (idx, [1:   4]) = [  2.32258E+01 0.00017  2.60968E+01 0.00023 ];

% Group constant generation:

GC_UNIVERSE_NAME          (idx, [1:  1])  = '0' ;

% Micro- and macro-group structures:

MICRO_NG                  (idx, 1)        = 70 ;
MICRO_E                   (idx, [1:  71]) = [  2.00000E+01  6.06550E+00  3.67900E+00  2.23100E+00  1.35300E+00  8.21000E-01  5.00000E-01  3.02500E-01  1.83000E-01  1.11000E-01  6.74300E-02  4.08500E-02  2.47800E-02  1.50300E-02  9.11800E-03  5.50000E-03  3.51910E-03  2.23945E-03  1.42510E-03  9.06898E-04  3.67262E-04  1.48728E-04  7.55014E-05  4.80520E-05  2.77000E-05  1.59680E-05  9.87700E-06  4.00000E-06  3.30000E-06  2.60000E-06  2.10000E-06  1.85500E-06  1.50000E-06  1.30000E-06  1.15000E-06  1.12300E-06  1.09700E-06  1.07100E-06  1.04500E-06  1.02000E-06  9.96000E-07  9.72000E-07  9.50000E-07  9.10000E-07  8.50000E-07  7.80000E-07  6.25000E-07  5.00000E-07  4.00000E-07  3.50000E-07  3.20000E-07  3.00000E-07  2.80000E-07  2.50000E-07  2.20000E-07  1.80000E-07  1.40000E-07  1.00000E-07  8.00000E-08  6.70000E-08  5.80000E-08  5.00000E-08  4.20000E-08  3.50000E-08  3.00000E-08  2.50000E-08  2.00000E-08  1.50000E-08  1.00000E-08  5.00000E-09  1.00000E-11 ];

MACRO_NG                  (idx, 1)        = 2 ;
MACRO_E                   (idx, [1:   3]) = [  1.00000E+37  6.25000E-07  0.00000E+00 ];

% Micro-group spectrum:

INF_MICRO_FLX             (idx, [1: 140]) = [  3.10083E+04 0.00244  1.21414E+05 0.00099  2.36231E+05 0.00059  2.51206E+05 0.00056  2.37957E+05 0.00047  2.48525E+05 0.00053  1.78381E+05 0.00047  1.52944E+05 0.00048  1.18562E+05 0.00049  9.73465E+04 0.00053  8.43664E+04 0.00053  7.54640E+04 0.00049  7.01087E+04 0.00040  6.64899E+04 0.00058  6.47552E+04 0.00057  5.57531E+04 0.00051  5.51061E+04 0.00055  5.42598E+04 0.00060  5.30652E+04 0.00064  1.02636E+05 0.00039  9.76604E+04 0.00041  6.87660E+04 0.00053  4.35071E+04 0.00059  4.84874E+04 0.00052  4.38608E+04 0.00052  4.03733E+04 0.00063  6.13052E+04 0.00059  1.41994E+04 0.00107  1.77229E+04 0.00103  1.60718E+04 0.00093  9.26292E+03 0.00118  1.61177E+04 0.00097  1.09028E+04 0.00118  9.18979E+03 0.00108  1.73348E+03 0.00287  1.71395E+03 0.00240  1.76767E+03 0.00262  1.83238E+03 0.00223  1.81371E+03 0.00234  1.79199E+03 0.00224  1.85300E+03 0.00274  1.74359E+03 0.00221  3.30059E+03 0.00207  5.29497E+03 0.00139  6.78196E+03 0.00143  1.83303E+04 0.00082  2.08952E+04 0.00086  2.51293E+04 0.00076  1.76250E+04 0.00109  1.29597E+04 0.00111  9.87034E+03 0.00135  1.10707E+04 0.00126  1.93864E+04 0.00090  2.34045E+04 0.00101  3.85196E+04 0.00077  4.74355E+04 0.00066  5.50285E+04 0.00069  2.88040E+04 0.00075  1.83510E+04 0.00094  1.22022E+04 0.00091  1.02995E+04 0.00104  9.57902E+03 0.00104  7.62378E+03 0.00102  4.91426E+03 0.00123  4.38709E+03 0.00105  3.77776E+03 0.00129  3.10793E+03 0.00114  2.34854E+03 0.00138  1.47960E+03 0.00143  5.18240E+02 0.00202 ];

% Integral parameters:

INF_KINF                  (idx, [1:   2]) = [  1.24950E+00 0.00042 ];

% Flux spectra in infinite geometry:

INF_FLX                   (idx, [1:   4]) = [  8.81856E-01 6.1E-05  1.18165E-01 0.00046 ];
INF_FISS_FLX              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Reaction cross sections:

INF_TOT                   (idx, [1:   4]) = [  1.25402E+00 7.6E-05  2.61577E+00 4.3E-05 ];
INF_CAPT                  (idx, [1:   4]) = [  2.44560E-02 0.00039  7.24698E-02 8.6E-05 ];
INF_ABS                   (idx, [1:   4]) = [  3.36923E-02 0.00030  2.64226E-01 9.7E-05 ];
INF_FISS                  (idx, [1:   4]) = [  9.23630E-03 0.00015  1.91757E-01 0.00010 ];
INF_NSF                   (idx, [1:   4]) = [  2.35338E-02 0.00016  4.67253E-01 0.00010 ];
INF_NUBAR                 (idx, [1:   4]) = [  2.54797E+00 2.9E-05  2.43670E+00 0.0E+00 ];
INF_KAPPA                 (idx, [1:   4]) = [  2.03546E+02 2.2E-06  2.02270E+02 1.9E-09 ];
INF_INVV                  (idx, [1:   4]) = [  5.07854E-08 0.00026  2.04071E-06 9.4E-05 ];

% Total scattering cross sections:

INF_SCATT0                (idx, [1:   4]) = [  1.22035E+00 7.9E-05  2.35147E+00 5.9E-05 ];
INF_SCATT1                (idx, [1:   4]) = [  5.82387E-01 9.5E-05  8.00929E-01 0.00023 ];
INF_SCATT2                (idx, [1:   4]) = [  2.29421E-01 0.00015  3.00899E-01 0.00050 ];
INF_SCATT3                (idx, [1:   4]) = [  1.89588E-02 0.00194  1.20237E-01 0.00098 ];
INF_SCATT4                (idx, [1:   4]) = [ -2.40393E-02 0.00127  5.49358E-02 0.00214 ];
INF_SCATT5                (idx, [1:   4]) = [  7.97678E-04 0.03819  2.96803E-02 0.00400 ];
INF_SCATT6                (idx, [1:   4]) = [  1.28714E-02 0.00199  1.85236E-02 0.00596 ];
INF_SCATT7                (idx, [1:   4]) = [  2.38029E-03 0.01114  1.28344E-02 0.00811 ];

% Total scattering production cross sections:

INF_SCATTP0               (idx, [1:   4]) = [  1.22049E+00 7.8E-05  2.35147E+00 5.9E-05 ];
INF_SCATTP1               (idx, [1:   4]) = [  5.82388E-01 9.5E-05  8.00929E-01 0.00023 ];
INF_SCATTP2               (idx, [1:   4]) = [  2.29421E-01 0.00015  3.00899E-01 0.00050 ];
INF_SCATTP3               (idx, [1:   4]) = [  1.89584E-02 0.00194  1.20237E-01 0.00098 ];
INF_SCATTP4               (idx, [1:   4]) = [ -2.40396E-02 0.00127  5.49358E-02 0.00214 ];
INF_SCATTP5               (idx, [1:   4]) = [  7.98171E-04 0.03809  2.96803E-02 0.00400 ];
INF_SCATTP6               (idx, [1:   4]) = [  1.28712E-02 0.00199  1.85236E-02 0.00596 ];
INF_SCATTP7               (idx, [1:   4]) = [  2.38051E-03 0.01113  1.28344E-02 0.00811 ];

% Diffusion parameters:

INF_TRANSPXS              (idx, [1:   4]) = [  4.68912E-01 0.00018  1.66289E+00 0.00014 ];
INF_DIFFCOEF              (idx, [1:   4]) = [  7.10866E-01 0.00018  2.00455E-01 0.00014 ];

% Reduced absoption and removal:

INF_RABSXS                (idx, [1:   4]) = [  3.35470E-02 0.00031  2.64226E-01 9.7E-05 ];
INF_REMXS                 (idx, [1:   4]) = [  7.05892E-02 0.00012  2.75501E-01 0.00037 ];

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

INF_S0                    (idx, [1:   8]) = [  1.18343E+00 7.8E-05  3.69157E-02 0.00030  1.12036E-02 0.00194  2.34027E+00 6.0E-05 ];
INF_S1                    (idx, [1:   8]) = [  5.69597E-01 9.4E-05  1.27897E-02 0.00062  5.86314E-03 0.00279  7.95066E-01 0.00023 ];
INF_S2                    (idx, [1:   8]) = [  2.31163E-01 0.00015 -1.74193E-03 0.00356  2.87002E-03 0.00424  2.98029E-01 0.00050 ];
INF_S3                    (idx, [1:   8]) = [  2.29698E-02 0.00158 -4.01100E-03 0.00123  9.52156E-04 0.01223  1.19285E-01 0.00098 ];
INF_S4                    (idx, [1:   8]) = [ -2.21336E-02 0.00141 -1.90566E-03 0.00272  4.13012E-05 0.21122  5.48945E-02 0.00213 ];
INF_S5                    (idx, [1:   8]) = [  1.27732E-03 0.02349 -4.79640E-04 0.00918 -2.61747E-04 0.02783  2.99420E-02 0.00396 ];
INF_S6                    (idx, [1:   8]) = [  1.30039E-02 0.00189 -1.32519E-04 0.03303 -3.09806E-04 0.02661  1.88334E-02 0.00585 ];
INF_S7                    (idx, [1:   8]) = [  2.46985E-03 0.01070 -8.95589E-05 0.03846 -2.70514E-04 0.02461  1.31049E-02 0.00789 ];

% Scattering production matrixes:

INF_SP0                   (idx, [1:   8]) = [  1.18358E+00 7.8E-05  3.69157E-02 0.00030  1.12036E-02 0.00194  2.34027E+00 6.0E-05 ];
INF_SP1                   (idx, [1:   8]) = [  5.69599E-01 9.4E-05  1.27897E-02 0.00062  5.86314E-03 0.00279  7.95066E-01 0.00023 ];
INF_SP2                   (idx, [1:   8]) = [  2.31163E-01 0.00015 -1.74193E-03 0.00356  2.87002E-03 0.00424  2.98029E-01 0.00050 ];
INF_SP3                   (idx, [1:   8]) = [  2.29694E-02 0.00158 -4.01100E-03 0.00123  9.52156E-04 0.01223  1.19285E-01 0.00098 ];
INF_SP4                   (idx, [1:   8]) = [ -2.21339E-02 0.00140 -1.90566E-03 0.00272  4.13012E-05 0.21122  5.48945E-02 0.00213 ];
INF_SP5                   (idx, [1:   8]) = [  1.27781E-03 0.02343 -4.79640E-04 0.00918 -2.61747E-04 0.02783  2.99420E-02 0.00396 ];
INF_SP6                   (idx, [1:   8]) = [  1.30038E-02 0.00189 -1.32519E-04 0.03303 -3.09806E-04 0.02661  1.88334E-02 0.00585 ];
INF_SP7                   (idx, [1:   8]) = [  2.47007E-03 0.01070 -8.95589E-05 0.03846 -2.70514E-04 0.02461  1.31049E-02 0.00789 ];

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

CMM_TRANSPXS              (idx, [1:   4]) = [  5.07509E-01 0.00034  1.52426E+00 0.00176 ];
CMM_TRANSPXS_X            (idx, [1:   4]) = [  5.07467E-01 0.00060  1.52884E+00 0.00355 ];
CMM_TRANSPXS_Y            (idx, [1:   4]) = [  5.07960E-01 0.00064  1.52738E+00 0.00307 ];
CMM_TRANSPXS_Z            (idx, [1:   4]) = [  5.07122E-01 0.00072  1.51811E+00 0.00271 ];
CMM_DIFFCOEF              (idx, [1:   4]) = [  6.56807E-01 0.00034  2.18718E-01 0.00174 ];
CMM_DIFFCOEF_X            (idx, [1:   4]) = [  6.56868E-01 0.00060  2.18164E-01 0.00354 ];
CMM_DIFFCOEF_Y            (idx, [1:   4]) = [  6.56233E-01 0.00064  2.18338E-01 0.00304 ];
CMM_DIFFCOEF_Z            (idx, [1:   4]) = [  6.57320E-01 0.00072  2.19651E-01 0.00272 ];

% Delayed neutron parameters (Meulekamp method):

BETA_EFF                  (idx, [1:  14]) = [  6.97205E-03 0.00602  2.09497E-04 0.03538  1.16858E-03 0.01485  1.15103E-03 0.01545  2.68849E-03 0.01006  1.24226E-03 0.01446  5.12190E-04 0.02309 ];
LAMBDA                    (idx, [1:  14]) = [  5.10786E-01 0.00886  1.33642E-02 0.00018  3.25684E-02 0.00021  1.21128E-01 0.00011  3.06387E-01 0.00026  8.63918E-01 0.00041  2.90014E+00 0.00060 ];

