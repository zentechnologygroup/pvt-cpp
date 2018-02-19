#!/usr/bin/env ruby

add_class 'Blackoil'
add_members 'yghc', 'ppchc', 'tpchc', 'ppcm', 'tpcm', 'adjustedppcm'
add_members 'adjustedtpcm', 'pb_q'
add_members 't_q', 'p_q', 'temperature', 'pressure', 'bobp', 'uobp', 'pobp'
add_members 'bwbp', 'tpr', 'pb_par', 't_par', 'p_par', 'p_pb', 'rs_pb', 'tpr_par'
add_members('pb_pars', 'rs_pars', 'uod_pars', 'bo_pars', 'co_pars', 'uo_pars',
            'po_pars', 'ug_pars', 'bw_pars', 'uw_pars', 'pw_pars', 'rsw_pars',
            'cw_pars', 'cwa_pars', 'cg_pars', 'sgo_pars', 'sgw_pars')

add_class 'Drygas'
