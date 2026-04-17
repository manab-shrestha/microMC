
% Increase counter:

if (exist('idx', 'var'));
  idx = idx + 1;
else;
  idx = 1;
end;

% Version, title and date:

VERSION                   (idx, [1: 14])  = 'Serpent 2.1.32' ;
COMPILE_DATE              (idx, [1: 20])  = 'Apr 17 2026 10:29:26' ;
DEBUG                     (idx, 1)        = 0 ;
TITLE                     (idx, [1: 31])  = 'MICROMC VALIDATION K-EIGENVALUE' ;
CONFIDENTIAL_DATA         (idx, 1)        = 0 ;
INPUT_FILE_NAME           (idx, [1:  7])  = 'input_k' ;
WORKING_DIRECTORY         (idx, [1: 28])  = '/home/ms3281/micromc/serpent' ;
HOSTNAME                  (idx, [1:  5])  = 'forge' ;
CPU_TYPE                  (idx, [1: 29])  = 'Intel(R) Core(TM) Ultra 5 235' ;
CPU_MHZ                   (idx, 1)        = 283.0 ;
START_DATE                (idx, [1: 24])  = 'Fri Apr 17 10:39:40 2026' ;
COMPLETE_DATE             (idx, [1: 24])  = 'Fri Apr 17 10:40:55 2026' ;

% Run parameters:

POP                       (idx, 1)        = 10000 ;
CYCLES                    (idx, 1)        = 200 ;
SKIP                      (idx, 1)        = 10 ;
BATCH_INTERVAL            (idx, 1)        = 1 ;
SRC_NORM_MODE             (idx, 1)        = 2 ;
SEED                      (idx, 1)        = 1776418780000 ;
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

XS_DATA_FILE_PATH         (idx, [1: 40])  = '/home/ms3281/endfb71/s2v0_endfb71.xsdata' ;
DECAY_DATA_FILE_PATH      (idx, [1:  3])  = 'N/A' ;
SFY_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;
NFY_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;
BRA_DATA_FILE_PATH        (idx, [1:  3])  = 'N/A' ;

% Collision and reaction sampling (neutrons/photons):

MIN_MACROXS               (idx, [1:   4]) = [  5.00000E-02 0.0E+00  0.00000E+00 0.0E+00 ];
DT_THRESH                 (idx, [1:  2])  = [  9.00000E-01  9.00000E-01 ];
ST_FRAC                   (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_FRAC                   (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
DT_EFF                    (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_EFF          (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
REA_SAMPLING_FAIL         (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_COL_EFF               (idx, [1:   4]) = [  1.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKING_LOOPS        (idx, [1:   8]) = [  1.97817E+00 5.2E-05  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_TRACKS                (idx, [1:   4]) = [  2.32228E+01 0.00039  0.00000E+00 0.0E+00 ];
AVG_REAL_COL              (idx, [1:   4]) = [  2.32228E+01 0.00039  0.00000E+00 0.0E+00 ];
AVG_VIRT_COL              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
AVG_SURF_CROSS            (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
LOST_PARTICLES            (idx, 1)        = 0 ;

% Run statistics:

CYCLE_IDX                 (idx, 1)        = 200 ;
SIMULATED_HISTORIES       (idx, 1)        = 2000359 ;
MEAN_POP_SIZE             (idx, [1:  2])  = [  1.00018E+04 0.00106 ];
MEAN_POP_WGT              (idx, [1:  2])  = [  1.00018E+04 0.00106 ];
SIMULATION_COMPLETED      (idx, 1)        = 1 ;

% Running times:

TOT_CPU_TIME              (idx, 1)        =  1.25687E+00 ;
RUNNING_TIME              (idx, 1)        =  1.25708E+00 ;
INIT_TIME                 (idx, [1:  2])  = [  8.30000E-03  8.30000E-03 ];
PROCESS_TIME              (idx, [1:  2])  = [  1.83332E-04  1.83332E-04 ];
TRANSPORT_CYCLE_TIME      (idx, [1:  3])  = [  1.24860E+00  1.24860E+00  0.00000E+00 ];
MPI_OVERHEAD_TIME         (idx, [1:  2])  = [  0.00000E+00  0.00000E+00 ];
ESTIMATED_RUNNING_TIME    (idx, [1:  2])  = [  1.25673E+00  0.00000E+00 ];
CPU_USAGE                 (idx, 1)        = 0.99983 ;
TRANSPORT_CPU_USAGE       (idx, [1:   2]) = [  9.99833E-01 8.4E-05 ];
OMP_PARALLEL_FRAC         (idx, 1)        =  9.84435E-01 ;

% Memory usage:

AVAIL_MEM                 (idx, 1)        = 31542.35 ;
ALLOC_MEMSIZE             (idx, 1)        = 212.23;
MEMSIZE                   (idx, 1)        = 163.58;
XS_MEMSIZE                (idx, 1)        = 56.44;
MAT_MEMSIZE               (idx, 1)        = 38.19;
RES_MEMSIZE               (idx, 1)        = 1.99;
IFC_MEMSIZE               (idx, 1)        = 0.00;
MISC_MEMSIZE              (idx, 1)        = 66.97;
UNKNOWN_MEMSIZE           (idx, 1)        = 0.00;
UNUSED_MEMSIZE            (idx, 1)        = 48.64;

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

NORM_COEF                 (idx, [1:   4]) = [  9.97577E-05 0.00065  0.00000E+00 0.0E+00 ];

% Analog reaction rate estimators:

CONVERSION_RATIO          (idx, [1:   2]) = [  6.09462E-01 0.00153 ];
U235_FISS                 (idx, [1:   4]) = [  4.68223E-01 0.00082  9.26306E-01 0.00025 ];
U238_FISS                 (idx, [1:   4]) = [  3.72530E-02 0.00336  7.36940E-02 0.00315 ];
U235_CAPT                 (idx, [1:   4]) = [  1.13248E-01 0.00206  2.29174E-01 0.00182 ];
U238_CAPT                 (idx, [1:   4]) = [  3.54384E-01 0.00131  7.17123E-01 0.00063 ];

% Neutron balance (particles/weight):

BALA_SRC_NEUTRON_SRC     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_FISS    (idx, [1:  2])  = [ 2000359 2.00000E+06 ];
BALA_SRC_NEUTRON_NXN     (idx, [1:  2])  = [ 0 4.15497E+03 ];
BALA_SRC_NEUTRON_VR      (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_SRC_NEUTRON_TOT     (idx, [1:  2])  = [ 2000359 2.00415E+06 ];

BALA_LOSS_NEUTRON_CAPT    (idx, [1:  2])  = [ 988815 9.90717E+05 ];
BALA_LOSS_NEUTRON_FISS    (idx, [1:  2])  = [ 1011544 1.01344E+06 ];
BALA_LOSS_NEUTRON_LEAK    (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_CUT     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_ERR     (idx, [1:  2])  = [ 0 0.00000E+00 ];
BALA_LOSS_NEUTRON_TOT     (idx, [1:  2])  = [ 2000359 2.00415E+06 ];

BALA_NEUTRON_DIFF         (idx, [1:  2])  = [ 0 -1.37370E-08 ];

% Normalized total reaction rates (neutrons):

TOT_POWER                 (idx, [1:   2]) = [  1.64071E-11 0.00035 ];
TOT_POWDENS               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_GENRATE               (idx, [1:   2]) = [  1.24649E+00 0.00034 ];
TOT_FISSRATE              (idx, [1:   2]) = [  5.05436E-01 0.00035 ];
TOT_CAPTRATE              (idx, [1:   2]) = [  4.94564E-01 0.00036 ];
TOT_ABSRATE               (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_SRCRATE               (idx, [1:   2]) = [  9.97577E-01 0.00065 ];
TOT_FLUX                  (idx, [1:   2]) = [  1.64062E+01 0.00050 ];
TOT_PHOTON_PRODRATE       (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];
TOT_LEAKRATE              (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
ALBEDO_LEAKRATE           (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_LOSSRATE              (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
TOT_CUTRATE               (idx, [1:   2]) = [  0.00000E+00 0.0E+00 ];
TOT_RR                    (idx, [1:   2]) = [  2.32122E+01 0.00039 ];
INI_FMASS                 (idx, 1)        =  0.00000E+00 ;
TOT_FMASS                 (idx, 1)        =  0.00000E+00 ;

% Six-factor formula:

SIX_FF_ETA                (idx, [1:   2]) = [  1.76819E+00 0.00063 ];
SIX_FF_F                  (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_P                  (idx, [1:   2]) = [  5.13052E-01 0.00072 ];
SIX_FF_EPSILON            (idx, [1:   2]) = [  1.37766E+00 0.00062 ];
SIX_FF_LF                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_LT                 (idx, [1:   2]) = [  1.00000E+00 0.0E+00 ];
SIX_FF_KINF               (idx, [1:   2]) = [  1.24968E+00 0.00073 ];
SIX_FF_KEFF               (idx, [1:   2]) = [  1.24968E+00 0.00073 ];

% Fission neutron and energy production:

NUBAR                     (idx, [1:   2]) = [  2.46616E+00 2.1E-05 ];
FISSE                     (idx, [1:   2]) = [  2.02607E+02 2.1E-06 ];

% Criticality eigenvalues:

ANA_KEFF                  (idx, [1:   6]) = [  1.24957E+00 0.00072  1.24108E+00 0.00072  8.60550E-03 0.01251 ];
IMP_KEFF                  (idx, [1:   2]) = [  1.24912E+00 0.00034 ];
COL_KEFF                  (idx, [1:   2]) = [  1.24965E+00 0.00087 ];
ABS_KEFF                  (idx, [1:   2]) = [  1.24912E+00 0.00034 ];
ABS_KINF                  (idx, [1:   2]) = [  1.24912E+00 0.00034 ];
GEOM_ALBEDO               (idx, [1:   6]) = [  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00  1.00000E+00 0.0E+00 ];

% ALF (Average lethargy of neutrons causing fission):
% Based on E0 = 2.000000E+01 MeV

ANA_ALF                   (idx, [1:   2]) = [  1.66222E+01 0.00031 ];
IMP_ALF                   (idx, [1:   2]) = [  1.66290E+01 0.00015 ];

% EALF (Energy corresponding to average lethargy of neutrons causing fission):

ANA_EALF                  (idx, [1:   2]) = [  1.21124E-06 0.00516 ];
IMP_EALF                  (idx, [1:   2]) = [  1.20062E-06 0.00244 ];

% AFGE (Average energy of neutrons causing fission):

ANA_AFGE                  (idx, [1:   2]) = [  2.60950E-01 0.00367 ];
IMP_AFGE                  (idx, [1:   2]) = [  2.60528E-01 0.00158 ];

% Forward-weighted delayed neutron parameters:

PRECURSOR_GROUPS          (idx, 1)        = 6 ;
FWD_ANA_BETA_ZERO         (idx, [1:  14]) = [  5.77897E-03 0.00850  1.88086E-04 0.04683  9.59975E-04 0.02117  9.70902E-04 0.02119  2.25640E-03 0.01236  9.89921E-04 0.01948  4.13686E-04 0.03395 ];
FWD_ANA_LAMBDA            (idx, [1:  14]) = [  5.01123E-01 0.01198  1.18890E-02 0.02492  3.25679E-02 0.00032  1.21133E-01 0.00015  3.06518E-01 0.00035  8.64950E-01 0.00065  2.88458E+00 0.00512 ];

% Beta-eff using Meulekamp's method:

ADJ_MEULEKAMP_BETA_EFF    (idx, [1:  14]) = [  7.03646E-03 0.01388  2.44242E-04 0.07079  1.22085E-03 0.03683  1.16880E-03 0.03390  2.75142E-03 0.02156  1.17099E-03 0.03299  4.80166E-04 0.04988 ];
ADJ_MEULEKAMP_LAMBDA      (idx, [1:  14]) = [  4.88037E-01 0.01851  1.33567E-02 0.00032  3.25763E-02 0.00046  1.21119E-01 0.00024  3.06369E-01 0.00053  8.63917E-01 0.00090  2.89743E+00 0.00134 ];

% Adjoint weighted time constants using Nauchi's method:

IFP_CHAIN_LENGTH          (idx, 1)        = 15 ;
ADJ_NAUCHI_GEN_TIME       (idx, [1:   6]) = [  5.26287E-06 0.00155  5.25793E-06 0.00158  5.99027E-06 0.01589 ];
ADJ_NAUCHI_LIFETIME       (idx, [1:   6]) = [  6.57574E-06 0.00143  6.56957E-06 0.00146  7.48404E-06 0.01583 ];
ADJ_NAUCHI_BETA_EFF       (idx, [1:  14]) = [  6.89422E-03 0.01233  2.17387E-04 0.06676  1.17243E-03 0.02904  1.15790E-03 0.03115  2.69212E-03 0.01935  1.17752E-03 0.02799  4.76854E-04 0.04730 ];
ADJ_NAUCHI_LAMBDA         (idx, [1:  14]) = [  4.94010E-01 0.01720  1.33544E-02 0.00035  3.25757E-02 0.00043  1.21107E-01 0.00021  3.06415E-01 0.00054  8.64353E-01 0.00087  2.90515E+00 0.00157 ];

% Adjoint weighted time constants using IFP:

ADJ_IFP_GEN_TIME          (idx, [1:   6]) = [  5.15391E-06 0.01188  5.14875E-06 0.01188  5.79641E-06 0.04094 ];
ADJ_IFP_LIFETIME          (idx, [1:   6]) = [  6.43918E-06 0.01186  6.43275E-06 0.01186  7.24161E-06 0.04095 ];
ADJ_IFP_IMP_BETA_EFF      (idx, [1:  14]) = [  6.74668E-03 0.03861  1.45734E-04 0.19040  1.14330E-03 0.09120  1.03330E-03 0.09200  2.62842E-03 0.05393  1.23824E-03 0.09097  5.57683E-04 0.12575 ];
ADJ_IFP_IMP_LAMBDA        (idx, [1:  14]) = [  5.27624E-01 0.04547  1.33528E-02 0.00088  3.25839E-02 0.00111  1.21163E-01 0.00062  3.06389E-01 0.00140  8.66964E-01 0.00225  2.91396E+00 0.00336 ];
ADJ_IFP_ANA_BETA_EFF      (idx, [1:  14]) = [  6.71321E-03 0.03630  1.46438E-04 0.18367  1.09570E-03 0.09310  1.05040E-03 0.08383  2.63595E-03 0.05350  1.23917E-03 0.08508  5.45557E-04 0.12155 ];
ADJ_IFP_ANA_LAMBDA        (idx, [1:  14]) = [  5.30171E-01 0.04536  1.33528E-02 0.00088  3.25806E-02 0.00112  1.21149E-01 0.00060  3.06442E-01 0.00137  8.66770E-01 0.00221  2.91515E+00 0.00337 ];
ADJ_IFP_ROSSI_ALPHA       (idx, [1:   2]) = [ -1.31021E+03 0.03662 ];

% Adjoint weighted time constants using perturbation technique:

ADJ_PERT_GEN_TIME         (idx, [1:   2]) = [  5.26703E-06 0.00093 ];
ADJ_PERT_LIFETIME         (idx, [1:   2]) = [  6.58083E-06 0.00059 ];
ADJ_PERT_BETA_EFF         (idx, [1:   2]) = [  7.03232E-03 0.00572 ];
ADJ_PERT_ROSSI_ALPHA      (idx, [1:   2]) = [ -1.33546E+03 0.00584 ];

% Inverse neutron speed :

ANA_INV_SPD               (idx, [1:   2]) = [  2.85938E-07 0.00084 ];

% Analog slowing-down and thermal neutron lifetime (total/prompt/delayed):

ANA_SLOW_TIME             (idx, [1:   6]) = [  1.16307E-06 0.00075  1.16308E-06 0.00075  1.16307E-06 0.00757 ];
ANA_THERM_TIME            (idx, [1:   6]) = [  7.74291E-06 0.00096  7.74187E-06 0.00098  7.87940E-06 0.01073 ];
ANA_THERM_FRAC            (idx, [1:   6]) = [  5.14162E-01 0.00073  5.13246E-01 0.00073  6.82435E-01 0.01465 ];
ANA_DELAYED_EMTIME        (idx, [1:   2]) = [  1.02933E+01 0.01944 ];
ANA_MEAN_NCOL             (idx, [1:   4]) = [  2.32228E+01 0.00039  2.60766E+01 0.00051 ];

% Group constant generation:

GC_UNIVERSE_NAME          (idx, [1:  1])  = '0' ;

% Micro- and macro-group structures:

MICRO_NG                  (idx, 1)        = 70 ;
MICRO_E                   (idx, [1:  71]) = [  2.00000E+01  6.06550E+00  3.67900E+00  2.23100E+00  1.35300E+00  8.21000E-01  5.00000E-01  3.02500E-01  1.83000E-01  1.11000E-01  6.74300E-02  4.08500E-02  2.47800E-02  1.50300E-02  9.11800E-03  5.50000E-03  3.51910E-03  2.23945E-03  1.42510E-03  9.06898E-04  3.67262E-04  1.48728E-04  7.55014E-05  4.80520E-05  2.77000E-05  1.59680E-05  9.87700E-06  4.00000E-06  3.30000E-06  2.60000E-06  2.10000E-06  1.85500E-06  1.50000E-06  1.30000E-06  1.15000E-06  1.12300E-06  1.09700E-06  1.07100E-06  1.04500E-06  1.02000E-06  9.96000E-07  9.72000E-07  9.50000E-07  9.10000E-07  8.50000E-07  7.80000E-07  6.25000E-07  5.00000E-07  4.00000E-07  3.50000E-07  3.20000E-07  3.00000E-07  2.80000E-07  2.50000E-07  2.20000E-07  1.80000E-07  1.40000E-07  1.00000E-07  8.00000E-08  6.70000E-08  5.80000E-08  5.00000E-08  4.20000E-08  3.50000E-08  3.00000E-08  2.50000E-08  2.00000E-08  1.50000E-08  1.00000E-08  5.00000E-09  1.00000E-11 ];

MACRO_NG                  (idx, 1)        = 2 ;
MACRO_E                   (idx, [1:   3]) = [  1.00000E+37  6.25000E-07  0.00000E+00 ];

% Micro-group spectrum:

INF_MICRO_FLX             (idx, [1: 140]) = [  3.10573E+04 0.00600  1.21593E+05 0.00283  2.35617E+05 0.00151  2.51769E+05 0.00108  2.37872E+05 0.00155  2.48668E+05 0.00064  1.78352E+05 0.00114  1.52959E+05 0.00067  1.18574E+05 0.00128  9.74167E+04 0.00103  8.41864E+04 0.00153  7.54153E+04 0.00121  6.99859E+04 0.00084  6.65425E+04 0.00127  6.48045E+04 0.00125  5.57016E+04 0.00096  5.51110E+04 0.00066  5.42439E+04 0.00158  5.30361E+04 0.00068  1.02716E+05 0.00106  9.75892E+04 0.00057  6.86971E+04 0.00108  4.35327E+04 0.00141  4.85898E+04 0.00144  4.38197E+04 0.00140  4.04783E+04 0.00160  6.11651E+04 0.00111  1.41848E+04 0.00244  1.76609E+04 0.00147  1.60922E+04 0.00275  9.21371E+03 0.00206  1.60886E+04 0.00199  1.08961E+04 0.00346  9.18990E+03 0.00210  1.72962E+03 0.00514  1.71057E+03 0.00652  1.76529E+03 0.00497  1.82802E+03 0.00532  1.82552E+03 0.00638  1.78317E+03 0.00542  1.85220E+03 0.00415  1.73652E+03 0.00662  3.27740E+03 0.00718  5.31014E+03 0.00388  6.79333E+03 0.00378  1.83039E+04 0.00203  2.08637E+04 0.00224  2.51166E+04 0.00152  1.76133E+04 0.00260  1.29323E+04 0.00185  9.86096E+03 0.00372  1.11017E+04 0.00266  1.93537E+04 0.00184  2.33422E+04 0.00196  3.84643E+04 0.00117  4.73815E+04 0.00141  5.49434E+04 0.00108  2.87689E+04 0.00136  1.84447E+04 0.00149  1.22723E+04 0.00153  1.03212E+04 0.00387  9.59859E+03 0.00167  7.63369E+03 0.00137  4.91020E+03 0.00347  4.38808E+03 0.00262  3.80199E+03 0.00291  3.10112E+03 0.00223  2.35356E+03 0.00220  1.48274E+03 0.00578  5.17291E+02 0.00426 ];

% Integral parameters:

INF_KINF                  (idx, [1:   2]) = [  1.24965E+00 0.00072 ];

% Flux spectra in infinite geometry:

INF_FLX                   (idx, [1:   4]) = [  1.44685E+01 0.00053  1.93812E+00 0.00048 ];
INF_FISS_FLX              (idx, [1:   4]) = [  0.00000E+00 0.0E+00  0.00000E+00 0.0E+00 ];

% Reaction cross sections:

INF_TOT                   (idx, [1:   4]) = [  1.25391E+00 0.00014  2.61636E+00 0.00012 ];
INF_CAPT                  (idx, [1:   4]) = [  2.44721E-02 0.00050  7.25021E-02 0.00026 ];
INF_ABS                   (idx, [1:   4]) = [  3.37082E-02 0.00036  2.64366E-01 0.00029 ];
INF_FISS                  (idx, [1:   4]) = [  9.23607E-03 0.00031  1.91864E-01 0.00030 ];
INF_NSF                   (idx, [1:   4]) = [  2.35347E-02 0.00028  4.67515E-01 0.00030 ];
INF_NUBAR                 (idx, [1:   4]) = [  2.54813E+00 6.8E-05  2.43670E+00 0.0E+00 ];
INF_KAPPA                 (idx, [1:   4]) = [  2.03546E+02 3.6E-06  2.02270E+02 5.6E-09 ];
INF_INVV                  (idx, [1:   4]) = [  5.07495E-08 0.00055  2.04171E-06 0.00026 ];

% Total scattering cross sections:

INF_SCATT0                (idx, [1:   4]) = [  1.22020E+00 0.00014  2.35228E+00 0.00018 ];
INF_SCATT1                (idx, [1:   4]) = [  5.82355E-01 0.00030  8.00822E-01 0.00071 ];
INF_SCATT2                (idx, [1:   4]) = [  2.29446E-01 0.00043  3.00683E-01 0.00141 ];
INF_SCATT3                (idx, [1:   4]) = [  1.90141E-02 0.00463  1.20308E-01 0.00236 ];
INF_SCATT4                (idx, [1:   4]) = [ -2.41612E-02 0.00283  5.50285E-02 0.00619 ];
INF_SCATT5                (idx, [1:   4]) = [  6.43593E-04 0.09803  2.97069E-02 0.01271 ];
INF_SCATT6                (idx, [1:   4]) = [  1.27435E-02 0.00458  1.86526E-02 0.01461 ];
INF_SCATT7                (idx, [1:   4]) = [  2.43553E-03 0.02365  1.31021E-02 0.01172 ];

% Total scattering production cross sections:

INF_SCATTP0               (idx, [1:   4]) = [  1.22034E+00 0.00014  2.35228E+00 0.00018 ];
INF_SCATTP1               (idx, [1:   4]) = [  5.82356E-01 0.00030  8.00822E-01 0.00071 ];
INF_SCATTP2               (idx, [1:   4]) = [  2.29447E-01 0.00043  3.00683E-01 0.00141 ];
INF_SCATTP3               (idx, [1:   4]) = [  1.90135E-02 0.00463  1.20308E-01 0.00236 ];
INF_SCATTP4               (idx, [1:   4]) = [ -2.41614E-02 0.00282  5.50285E-02 0.00619 ];
INF_SCATTP5               (idx, [1:   4]) = [  6.44242E-04 0.09847  2.97069E-02 0.01271 ];
INF_SCATTP6               (idx, [1:   4]) = [  1.27438E-02 0.00458  1.86526E-02 0.01461 ];
INF_SCATTP7               (idx, [1:   4]) = [  2.43610E-03 0.02366  1.31021E-02 0.01172 ];

% Diffusion parameters:

INF_TRANSPXS              (idx, [1:   4]) = [  4.68903E-01 0.00030  1.66334E+00 0.00042 ];
INF_DIFFCOEF              (idx, [1:   4]) = [  7.10880E-01 0.00030  2.00400E-01 0.00042 ];

% Reduced absoption and removal:

INF_RABSXS                (idx, [1:   4]) = [  3.35650E-02 0.00037  2.64366E-01 0.00029 ];
INF_REMXS                 (idx, [1:   4]) = [  7.05835E-02 0.00028  2.75214E-01 0.00085 ];

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

INF_S0                    (idx, [1:   8]) = [  1.18333E+00 0.00014  3.68661E-02 0.00060  1.11396E-02 0.00552  2.34114E+00 0.00018 ];
INF_S1                    (idx, [1:   8]) = [  5.69571E-01 0.00030  1.27835E-02 0.00102  5.78164E-03 0.00814  7.95040E-01 0.00074 ];
INF_S2                    (idx, [1:   8]) = [  2.31187E-01 0.00042 -1.74110E-03 0.00857  2.81011E-03 0.01109  2.97873E-01 0.00139 ];
INF_S3                    (idx, [1:   8]) = [  2.30213E-02 0.00371 -4.00713E-03 0.00241  9.25378E-04 0.03105  1.19382E-01 0.00244 ];
INF_S4                    (idx, [1:   8]) = [ -2.22466E-02 0.00282 -1.91467E-03 0.00583  4.78283E-05 0.44329  5.49807E-02 0.00620 ];
INF_S5                    (idx, [1:   8]) = [  1.14882E-03 0.05654 -5.05226E-04 0.02164 -2.54829E-04 0.08411  2.99617E-02 0.01269 ];
INF_S6                    (idx, [1:   8]) = [  1.29027E-02 0.00430 -1.59254E-04 0.07198 -2.70130E-04 0.06411  1.89227E-02 0.01450 ];
INF_S7                    (idx, [1:   8]) = [  2.52504E-03 0.02234 -8.95077E-05 0.05507 -2.49186E-04 0.05226  1.33513E-02 0.01218 ];

% Scattering production matrixes:

INF_SP0                   (idx, [1:   8]) = [  1.18347E+00 0.00014  3.68661E-02 0.00060  1.11396E-02 0.00552  2.34114E+00 0.00018 ];
INF_SP1                   (idx, [1:   8]) = [  5.69572E-01 0.00030  1.27835E-02 0.00102  5.78164E-03 0.00814  7.95040E-01 0.00074 ];
INF_SP2                   (idx, [1:   8]) = [  2.31188E-01 0.00042 -1.74110E-03 0.00857  2.81011E-03 0.01109  2.97873E-01 0.00139 ];
INF_SP3                   (idx, [1:   8]) = [  2.30207E-02 0.00371 -4.00713E-03 0.00241  9.25378E-04 0.03105  1.19382E-01 0.00244 ];
INF_SP4                   (idx, [1:   8]) = [ -2.22467E-02 0.00281 -1.91467E-03 0.00583  4.78283E-05 0.44329  5.49807E-02 0.00620 ];
INF_SP5                   (idx, [1:   8]) = [  1.14947E-03 0.05681 -5.05226E-04 0.02164 -2.54829E-04 0.08411  2.99617E-02 0.01269 ];
INF_SP6                   (idx, [1:   8]) = [  1.29031E-02 0.00429 -1.59254E-04 0.07198 -2.70130E-04 0.06411  1.89227E-02 0.01450 ];
INF_SP7                   (idx, [1:   8]) = [  2.52561E-03 0.02234 -8.95077E-05 0.05507 -2.49186E-04 0.05226  1.33513E-02 0.01218 ];

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

CMM_TRANSPXS              (idx, [1:   4]) = [  5.07374E-01 0.00092  1.52488E+00 0.00405 ];
CMM_TRANSPXS_X            (idx, [1:   4]) = [  5.07131E-01 0.00110  1.53010E+00 0.00862 ];
CMM_TRANSPXS_Y            (idx, [1:   4]) = [  5.07111E-01 0.00170  1.51327E+00 0.00601 ];
CMM_TRANSPXS_Z            (idx, [1:   4]) = [  5.07901E-01 0.00167  1.53247E+00 0.00416 ];
CMM_DIFFCOEF              (idx, [1:   4]) = [  6.56982E-01 0.00092  2.18629E-01 0.00407 ];
CMM_DIFFCOEF_X            (idx, [1:   4]) = [  6.57300E-01 0.00110  2.17996E-01 0.00861 ];
CMM_DIFFCOEF_Y            (idx, [1:   4]) = [  6.57335E-01 0.00170  2.20344E-01 0.00596 ];
CMM_DIFFCOEF_Z            (idx, [1:   4]) = [  6.56312E-01 0.00167  2.17548E-01 0.00420 ];

% Delayed neutron parameters (Meulekamp method):

BETA_EFF                  (idx, [1:  14]) = [  7.03646E-03 0.01388  2.44242E-04 0.07079  1.22085E-03 0.03683  1.16880E-03 0.03390  2.75142E-03 0.02156  1.17099E-03 0.03299  4.80166E-04 0.04988 ];
LAMBDA                    (idx, [1:  14]) = [  4.88037E-01 0.01851  1.33567E-02 0.00032  3.25763E-02 0.00046  1.21119E-01 0.00024  3.06369E-01 0.00053  8.63917E-01 0.00090  2.89743E+00 0.00134 ];

