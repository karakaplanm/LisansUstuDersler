# Script CIFTEP:

# ######## ######## ######## ######## ######## ######## #
# CIFTEP                                                #
# ORTEP3 instructions generation from CIF files         #
# Y. Lin (transi@fzu.edu.cn)                            #
# Distributed under:                                    #
# GNU Lesser General Public License                     #
# http://www.gnu.org/licenses/lgpl.html                 #
# ######## ######## ######## ######## ######## ######## #

import sys, os, os.path
from optparse import OptionParser, OptionGroup

import tepw
from tepw import AtomGroup, TepInWriter

from PyCifRW.CifFile import CifFile

# parse float string, ignore esd:
def parseFloatWithEsd(s):
    t = s.find('(')
    if t > 0:
        s = s[0:t]
    return float(s)
pfwe = parseFloatWithEsd

# cmp func for AtomGroup:
def agcmp(ag1, ag2):
    return ag1.en - ag2.en

# global flag for verbose message printing:
verbose = True

# build option-parser:
op = OptionParser(version='0.1.2')
op.prog = 'ciftep'
##op.version = '0.1.2'
op.description = 'CIFTEP: Generate ORTEP3 Instructions From CIF Files'
op.usage = '%prog [options] cif-files'
#
op.add_option('-o', '--output', type='string', dest='tfname', metavar='OUTPUT',
              help='Specify output file name')
#
op.add_option('-c', '--contents', type='choice', choices=['aunit', 'grow', 'cell'],
              help='Contents scheme, [aunit|grow|cell], default: aunit')
op.set_default('contents', 'aunit')
op.add_option('-i', '--view', type='choice', choices=['std', 'best', '100', '010', '001'],
              help='Initial view scheme, [std|best|100|010|001], default: best')
op.set_default('view', 'best')
#
op.add_option('-n', '--normalize-uh', type='float', dest='uH',
              help='Normalize the u values of all H atoms to, default: 0.1, negative values mean no normalization')
op.set_default('uH', 0.1)
op.add_option('-b', '--no-label', action='store_true', dest='noLabel',
              help='No atom labels, (0, 0, 0)')
op.set_default('noLabel', False)
op.add_option('-e', '--probability', type='int', dest='prob',
              help='Ellipsoid probability, 1-99, default: 50')
op.set_default('prob', 50)
#
op.add_option('-m', '--color-map', type='string', dest='cm',
              help='Color map, [mono|color|...]')
op.set_default('cm', 'color')
#
og = OptionGroup(op, 'Projection control parameters')
og.add_option('-W', '--width', type='float', dest='width',
              help='Canvas width')
op.set_default('width', 10.7)
og.add_option('-H', '--height', type='float', dest='height',
              help='Canvas height')
op.set_default('height', 8.2)
og.add_option('-M', '--margin', type='float', dest='margin',
              help='Canvas margin')
op.set_default('margin', 0.3)
og.add_option('-D', '--distance', type='float', dest='distance',
              help='Viewing distance')
op.set_default('distance', 30.0)
op.add_option_group(og)
#
og = OptionGroup(op, 'Rotations after initial view')
og.add_option('-X', '--rotation-x', type='int', dest='rx',
              help='Specify the X rotation')
op.set_default('rx', 0)
og.add_option('-Y', '--rotation-y', type='int', dest='ry',
              help='Specify the Y rotation')
op.set_default('ry', 0)
og.add_option('-Z', '--rotation-z', type='int', dest='rz',
              help='Specify the Z rotation')
op.set_default('rz', 0)
op.add_option_group(og)
#
og = OptionGroup(op, 'CRTEP related')
og.add_option('-t', '--tep', action='store_true', dest='tep',
              help='Call crtep after tep instructions generation')
op.set_default('tep', False)
og.add_option('-p', '--tep-params', type='string', dest='tep_params', metavar='PARAMS',
              help='String passed to CRTEP as parameters')
og.add_option('-r', '--tep-clear', action='store_true', dest='tep_clear',
              help='Clear tep instructions after calling CRTEP')
op.set_default('tep_clear', False)
og.add_option('--tep-exec', type='string', dest='tep_exec', metavar='EXEC',
              help='CRTEP executable name')
op.set_default('tep_exec', 'crtep')
og.add_option('--tep-output', type='string', dest='tep_gfname', metavar='OUTPUT',
              help='CRTEP output name')
op.add_option_group(og)
#
op.add_option('-q', '--quiet', action='store_false', dest='verbose',
              help='Suppress verbose message printing')
op.set_default('verbose', True)
#
def print_description():
    print op.description
    print 'version: %s' % op.version
    print 'author: Y. Lin (transi@fzu.edu.cn)'
    print 'Type ciftep -h for more information.'
    return

# cif to tep generation:
# return a list of tep instruction file names/paths.
def ciftep(cfname, tfname, contents='aunit', view='best', proj=(10.7, 8.2, 0.3, 30.0), rot=(0, 0, 0), prob=50, cm='color', uH=0.1, noLabel=False):
    global verbose
    if verbose: print 'Read From CIF File: %s' % cfname
    cf = None
    r = [] # store tep file names.
    try:
        cf = CifFile(cfname)
        cfks = cf.keys()
        # prepare tfname for multi-block cif.
        tfc = None
        if len(cfks) > 1: # multi-block cif.
            tfc = os.path.splitext(tfname)
            tfname = ''
        #
        for cfk in cfks:
            cfb = cf[cfk] # select the named data block.
            title = cfk
            cell = (pfwe(cfb['_cell_length_a']),
                    pfwe(cfb['_cell_length_b']),
                    pfwe(cfb['_cell_length_c']),
                    pfwe(cfb['_cell_angle_alpha']),
                    pfwe(cfb['_cell_angle_beta']),
                    pfwe(cfb['_cell_angle_gamma']))
            #
            syms = cfb['_symmetry_equiv_pos_as_xyz']
            #
            atoms = {}
            loop = cfb.GetLoop('_atom_site_label')
            for d in loop:
                atoms[d['_atom_site_label']] = {'en': tepw.table_en[d['_atom_site_type_symbol'].capitalize()],
                                                'x': pfwe(d['_atom_site_fract_x']),
                                                'y': pfwe(d['_atom_site_fract_y']),
                                                'z': pfwe(d['_atom_site_fract_z']),
                                                'u': pfwe(d['_atom_site_U_iso_or_equiv']),
                                                'uani': (d['_atom_site_adp_type'].capitalize() == 'Uani')}
            aniatoms = {}
            loop = cfb.GetLoop('_atom_site_aniso_label')
            for d in loop:
                aniatoms[d['_atom_site_aniso_label']] = {'u11': pfwe(d['_atom_site_aniso_U_11']),
                                                         'u22': pfwe(d['_atom_site_aniso_U_22']),
                                                         'u33': pfwe(d['_atom_site_aniso_U_33']),
                                                         'u23': pfwe(d['_atom_site_aniso_U_23']),
                                                         'u13': pfwe(d['_atom_site_aniso_U_13']),
                                                         'u12': pfwe(d['_atom_site_aniso_U_12'])}
            ags = {}
            for k, d in atoms.items():
                en = d['en']
                if not ags.has_key(en):
                    ags[en] = AtomGroup(en)
                # use const uH for H:
                if en == 1 and uH > 0:
                    ags[en].append((k,
                                    d['x'], d['y'], d['z'],
                                    uH))
                    continue
                if d['uani']:
                    du = aniatoms[k]
                    ags[en].append((k,
                                    d['x'], d['y'], d['z'],
                                    du['u11'], du['u22'], du['u33'], du['u12'], du['u13'], du['u23']))
                else:
                    ags[en].append((k,
                                    d['x'], d['y'], d['z'],
                                    d['u']))
            #
            if tfc:
                tfname = tfc[0]+'_'+cfk+tfc[1]
            #
            if verbose: print 'Write To TEP Instruction File: %s' % tfname
            tiw = None
            try:
                tiw = TepInWriter()
                tiw.title = title
                tiw.cell = cell
                tiw.symmetries = syms
                ags = ags.values()
                ags.sort(agcmp)
                tiw.atomGroups = ags
                tiw.output = file(tfname, 'w')
                tiw.contents = ['aunit', 'grow', 'cell'].index(contents)
                tiw.view = ['std', 'best', '100', '010', '001'].index(view)
                tiw.width = proj[0]
                tiw.height = proj[1]
                tiw.margin = proj[2]
                tiw.distance = proj[3]
                tiw.rx = rot[0]
                tiw.ry = rot[1]
                tiw.rz = rot[2]
                tiw.probability = prob
                if noLabel:
                    tiw.atomLabel = (0, 0, 0)
                if cm == 'mono':
                    tiw.constPenColor = True
                elif cm == 'color':
                    pass
                else: # not implemented yet.
                    if verbose: print 'Custom Color Map Not Implemented Yet!'
                tiw.write()
                tiw.output.close()
                r.append(tfname)
            except Exception, e:
                if verbose: print 'Error Writing TEP Instructions!'
                if verbose: print e
    except Exception, e:
        if verbose: print 'Error Reading CIF!'
        if verbose: print e
    #
    return r

# crtep execution:
def crtep(tfname, crtep_exec='crtep', params='', gfname='', clear=False):
    global verbose
    r = 0
    try:
        if params:
            if gfname:
                if verbose: print 'Execute CRTEP [%s %s %s %s]' % (crtep_exec, tfname, params, gfname)
                r = os.spawnl(os.P_WAIT, crtep_exec, 'crtep', tfname, params, gfname)
            else:
                if verbose: print 'Execute CRTEP [%s %s %s]' % (crtep_exec, tfname, params)
                r = os.spawnl(os.P_WAIT, crtep_exec, 'crtep', tfname, params)
        elif gfname:
            if verbose: print 'Execute CRTEP [%s : %s]' % (crtep_exec, tfname)
            r = os.spawnl(os.P_WAIT, crtep_exec, 'crtep', tfname, ':', gfname)
        else:
            if verbose: print 'Execute CRTEP [%s %s]' % (crtep_exec, tfname)
            r = os.spawnl(os.P_WAIT, crtep_exec, 'crtep', tfname)
        if r == 0:
            if verbose: print 'CRTEP Execution Complete.'
        else:
            if verbose: print 'CRTEP Return Error Code: %i.' % r
    except Exception, e:
        if verbose: print 'Error CRTEP Execution!'
        if verbose: print e
        return 1
    try:
        if clear:
            if verbose: print 'Remove TEP Instruction File: %s' % tfname
            os.remove(tfname)
    except Exception, e:
        if verbose: print 'Error Removing TEP Instruction File!'
        if verbose: print e
        return 1
    return 0

# program main entry:
def main():
    global verbose
    vs, largs = op.parse_args()
    if len(largs) > 0:
        verbose = vs.verbose
        for cfname in largs:
            tfname = ''
            if vs.tfname and len(largs) == 1:
                tfname = vs.tfname
            else:
                tfname = os.path.splitext(cfname)[0]+'.dat'
            r = ciftep(cfname, tfname, vs.contents, vs.view, (vs.width, vs.height, vs.margin, vs.distance), (vs.rx, vs.ry, vs.rz), vs.prob, vs.cm, vs.uH, vs.noLabel)
            if len(r) == 0: continue
            if vs.tep:
                for tfname in r:
                    crtep(tfname, vs.tep_exec, vs.tep_params, vs.tep_gfname, vs.tep_clear)
    else:
        print_description()
        sys.exit(0)
        ##op.print_usage()
        ##op.print_help()
    if verbose: print 'O.K.'
    return

if __name__ == '__main__':
    main()
