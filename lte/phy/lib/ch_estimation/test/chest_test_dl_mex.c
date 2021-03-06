/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2014 The libLTE Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution.
 *
 * \section LICENSE
 *
 * This file is part of the libLTE library.
 *
 * libLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include <string.h>
#include "liblte/phy/phy.h"
#ifdef UNDEF_BOOL
#undef bool
#endif
#include "mex.h"


/** MEX function to be called from MATLAB to test the channel estimator 
 */

#define CELLID  prhs[0]
#define PORTS   prhs[1]
#define INPUT   prhs[2]
#define FREQ_FILTER   prhs[3]
#define TIME_FILTER   prhs[4]
#define NOF_INPUTS 5
#define SFIDX   prhs[5]

void help()
{
  mexErrMsgTxt
    ("[estChannel, avg_refs, output] = liblte_chest(cell_id, nof_ports, inputSignal,[sf_idx|freq_filter],"
     "[time_filter])\n\n"
     " Returns a matrix of size equal to the inputSignal matrix with the channel estimates\n "
     "for each resource element in inputSignal. The inputSignal matrix is the received Grid\n"
     "of size nof_resource_elements x nof_ofdm_symbols.\n"
     "The sf_idx is the subframe index only used if inputSignal is 1 subframe length.\n"
     "Returns the averaged references and output signal after ZF/MMSE equalization\n"
    );
}

/* the gateway function */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

  int i;
  lte_cell_t cell; 
  chest_dl_t chest;
  precoding_t cheq; 
  cf_t *input_signal = NULL, *output_signal[MAX_LAYERS]; 
  cf_t *output_signal2 = NULL;
  cf_t *ce[MAX_PORTS]; 
  double *outr0=NULL, *outi0=NULL;
  double *outr1=NULL, *outi1=NULL;
  double *outr2=NULL, *outi2=NULL;
  
  if (nrhs < NOF_INPUTS) {
    help();
    return;
  }

  if (!mxIsDouble(CELLID) && mxGetN(CELLID) != 1 && 
      !mxIsDouble(PORTS) && mxGetN(PORTS) != 1 && 
      mxGetM(CELLID) != 1) {
    help();
    return;
  }

  cell.id = (uint32_t) *((double*) mxGetPr(CELLID));
  cell.nof_prb = mxGetM(INPUT)/RE_X_RB;
  cell.nof_ports = (uint32_t) *((double*) mxGetPr(PORTS)); 
  if ((mxGetN(INPUT)%14) == 0) {
    cell.cp = CPNORM;    
  } else if ((mxGetN(INPUT)%12)!=0) {
    cell.cp = CPEXT;
  } else {
    mexErrMsgTxt("Invalid number of symbols\n");
    help();
    return;
  }
  
  if (chest_dl_init(&chest, cell)) {
    mexErrMsgTxt("Error initiating channel estimator\n");
    return;
  }
  
  int nsubframes;
  if (cell.cp == CPNORM) {
    nsubframes = mxGetN(INPUT)/14;
  } else {
    nsubframes = mxGetN(INPUT)/12;
  }
  
  uint32_t sf_idx=0; 
  if (nsubframes == 1) {
    if (nrhs != NOF_INPUTS+1) {
      mexErrMsgTxt("Received 1 subframe. Need to provide subframe index.\n");
      help();
      return;
    }
    sf_idx = (uint32_t) *((double*) mxGetPr(SFIDX));
  } else {
    if (nrhs != NOF_INPUTS) {
      help();
      return;
    }
  }
    
  uint32_t filter_len = 0;
  float *filter; 
  double *f; 
  
  filter_len = mxGetNumberOfElements(FREQ_FILTER);
  filter = malloc(sizeof(float) * filter_len);
  f = (double*) mxGetPr(FREQ_FILTER);
  for (i=0;i<filter_len;i++) {
    filter[i] = (float) f[i];
  }

  chest_dl_set_filter_freq(&chest, filter, filter_len);

  filter_len = mxGetNumberOfElements(TIME_FILTER);
  filter = malloc(sizeof(float) * filter_len);
  f = (double*) mxGetPr(TIME_FILTER);
  for (i=0;i<filter_len;i++) {
    filter[i] = (float) f[i];
  }
  chest_dl_set_filter_time(&chest, filter, filter_len);
  


  double *inr=(double *)mxGetPr(INPUT);
  double *ini=(double *)mxGetPi(INPUT);
  
  /** Allocate input buffers */
  int nof_re = 2*CP_NSYMB(cell.cp)*cell.nof_prb*RE_X_RB;
  for (i=0;i<MAX_PORTS;i++) {
    ce[i] = vec_malloc(nof_re * sizeof(cf_t));
  }
  input_signal = vec_malloc(nof_re * sizeof(cf_t));
  for (i=0;i<MAX_PORTS;i++) {
    output_signal[i] = vec_malloc(nof_re * sizeof(cf_t));
  }
  output_signal2 = vec_malloc(nof_re * sizeof(cf_t));
  
  precoding_init(&cheq, nof_re);
  
  /* Create output values */
  if (nlhs >= 1) {
    plhs[0] = mxCreateDoubleMatrix(nof_re * nsubframes, cell.nof_ports, mxCOMPLEX);
    outr0 = mxGetPr(plhs[0]);
    outi0 = mxGetPi(plhs[0]);
  }  
  if (nlhs >= 2) {
    plhs[1] = mxCreateDoubleMatrix(REFSIGNAL_MAX_NUM_SF(cell.nof_prb)*nsubframes, cell.nof_ports, mxCOMPLEX);
    outr1 = mxGetPr(plhs[1]);
    outi1 = mxGetPi(plhs[1]);
  }
  if (nlhs >= 3) {
    plhs[2] = mxCreateDoubleMatrix(nof_re * nsubframes, 1,  mxCOMPLEX);
    outr2 = mxGetPr(plhs[2]);
    outi2 = mxGetPi(plhs[2]);
  }
    
  for (int sf=0;sf<nsubframes;sf++) {
    /* Convert input to C complex type */
    for (i=0;i<nof_re;i++) {
      __real__ input_signal[i] = (float) *inr;
      if (ini) {
        __imag__ input_signal[i] = (float) *ini;
      }
      inr++;
      ini++;
    }
    
    if (nsubframes != 1) {
      sf_idx = sf%10;
    }
    
    if (chest_dl_estimate(&chest, input_signal, ce, sf_idx)) {
      mexErrMsgTxt("Error running channel estimator\n");
      return;
    }    
       
    if (cell.nof_ports == 1) {
      predecoding_single(&cheq, input_signal, ce[0], output_signal2, nof_re, chest_dl_get_noise_estimate(&chest));            
    } else {
      predecoding_diversity(&cheq, input_signal, ce, output_signal, cell.nof_ports, nof_re, chest_dl_get_noise_estimate(&chest));
      layerdemap_diversity(output_signal, output_signal2, cell.nof_ports, nof_re/cell.nof_ports);
    }
    
    if (nlhs >= 1) { 
      for (int j=0;j<cell.nof_ports;j++) {
        for (i=0;i<nof_re;i++) {      
          *outr0 = (double) crealf(ce[j][i]);
          if (outi0) {
            *outi0 = (double) cimagf(ce[j][i]);
          }
          outr0++;
          outi0++;
        } 
      }
    }
    if (nlhs >= 2) {    
      for (int j=0;j<cell.nof_ports;j++) {
        for (i=0;i<REFSIGNAL_NUM_SF(cell.nof_prb,j);i++) {
          *outr1 = (double) crealf(chest.pilot_estimates_average[j][i]);
          if (outi1) {
            *outi1 = (double) cimagf(chest.pilot_estimates_average[j][i]);
          }
          outr1++;
          outi1++;
        }
      }    
    }
    if (nlhs >= 3) {
      for (i=0;i<nof_re;i++) {      
        *outr2 = (double) crealf(output_signal2[i]);
        if (outi2) {
          *outi2 = (double) cimagf(output_signal2[i]);
        }
        outr2++;
        outi2++;
      }
    }
  }

  if (nlhs >= 4) {
    plhs[3] = mxCreateDoubleScalar(chest_dl_get_snr(&chest));
  }
  if (nlhs >= 5) {
    plhs[4] = mxCreateDoubleScalar(chest_dl_get_noise_estimate(&chest));
  }
  if (nlhs >= 6) {
    plhs[5] = mxCreateDoubleScalar(chest_dl_get_rsrp(&chest));
  }
  
  chest_dl_free(&chest);
  precoding_free(&cheq);

  return;
}

