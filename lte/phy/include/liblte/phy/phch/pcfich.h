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

#ifndef PCFICH_
#define PCFICH_

#include "liblte/config.h"
#include "liblte/phy/common/phy_common.h"
#include "liblte/phy/mimo/precoding.h"
#include "liblte/phy/mimo/layermap.h"
#include "liblte/phy/modem/mod.h"
#include "liblte/phy/modem/demod_soft.h"
#include "liblte/phy/scrambling/scrambling.h"
#include "liblte/phy/phch/regs.h"

#define PCFICH_CFI_LEN  32
#define PCFICH_RE       PCFICH_CFI_LEN/2

typedef _Complex float cf_t;

/* PCFICH object */
typedef struct LIBLTE_API {
  lte_cell_t cell;
  int nof_symbols;

  /* handler to REGs resource mapper */
  regs_t *regs;

  /* buffers */
  cf_t ce[MAX_PORTS][PCFICH_RE];
  cf_t pcfich_symbols[MAX_PORTS][PCFICH_RE];
  cf_t pcfich_x[MAX_PORTS][PCFICH_RE];
  cf_t pcfich_d[PCFICH_RE];
  
  // cfi table in floats 
  float cfi_table_float[3][PCFICH_CFI_LEN];

  /* bit message */
  uint8_t data[PCFICH_CFI_LEN];

  /* received soft bits */
  float data_f[PCFICH_CFI_LEN]; 

  /* tx & rx objects */
  modem_table_t mod;
  demod_soft_t demod;
  sequence_t seq_pcfich[NSUBFRAMES_X_FRAME];
  precoding_t precoding; 

} pcfich_t;

LIBLTE_API int pcfich_init(pcfich_t *q, 
                           regs_t *regs, 
                           lte_cell_t cell);

LIBLTE_API void pcfich_free(pcfich_t *q);

LIBLTE_API int pcfich_decode(pcfich_t *q, 
                             cf_t *sf_symbols, 
                             cf_t *ce[MAX_PORTS],
                             float noise_estimate, 
                             uint32_t subframe, 
                             uint32_t *cfi, 
                             float *corr_result);

LIBLTE_API int pcfich_encode(pcfich_t *q, 
                             uint32_t cfi, 
                             cf_t *sf_symbols[MAX_PORTS],
                             uint32_t subframe);

#endif
