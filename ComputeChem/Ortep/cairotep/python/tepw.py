# Module TEPW:

# ######## ######## ######## ######## ######## ######## #
# Module for TEP INS Generation                         #
# Y. Lin (transi@fzu.edu.cn)                            #
# Distributed under:                                    #
# GNU Lesser General Public License                     #
# http://www.gnu.org/licenses/lgpl.html                 #
# ######## ######## ######## ######## ######## ######## #

import sys

# Table of Element Symbol:
_table_es = ['',
             'H' , 'He', 'Li', 'Be', 'B' , 'C' , 'N' , 'O' , 'F' , 'Ne',
             'Na', 'Mg', 'Al', 'Si', 'P' , 'S' , 'Cl', 'Ar', 'K' , 'Ca',
             'Sc', 'Ti', 'V' , 'Cr', 'Mn', 'Fe', 'Co', 'Ni', 'Cu', 'Zn',
             'Ga', 'Ge', 'As', 'Se', 'Br', 'Kr', 'Rb', 'Sr', 'Y' , 'Zr',
             'Nb', 'Mo', 'Tc', 'Ru', 'Rh', 'Pd', 'Ag', 'Cd', 'In', 'Sn',
             'Sb', 'Te', 'I' , 'Xe', 'Cs', 'Ba', 'La', 'Ce', 'Pr', 'Nd',
             'Pm', 'Sm', 'Eu', 'Gd', 'Tb', 'Dy', 'Ho', 'Er', 'Tm', 'Yb',
             'Lu', 'Hf', 'Ta', 'W' , 'Re', 'Os', 'Ir', 'Pt', 'Au', 'Hg',
             'Tl', 'Pb', 'Bi', 'Po', 'At', 'Rn', 'Fr', 'Ra', 'Ac', 'Th',
             'Pa', 'U' , 'Np', 'Pu', 'Am', 'Cm', 'Bk', 'Cf', 'Es', 'Fm',
             'Md', 'No', 'Lr', 'Rf', 'Db', 'Sg', 'Bh', 'Hs', 'Mt', 'Ds'] # Lr=Lw

# Table of Element Number (dict in Symbol:Number pairs, build from _table_es):
_table_en = {}
for _i in range(0, len(_table_es)):
    _table_en[_table_es[_i]] = _i
_table_en['Lw'] = _table_en['Lr']

# Table of Covalent Radius order by Element Number:
# RADII data from CCDC:-
_table_cr = [0, # Dummy.
             0.23, 1.5 , 0.68, 0.35, 0.83, 0.68, 0.68, 0.68, 0.64, 1.5 , # 001-010
             0.97, 1.1 , 1.35, 1.2 , 1.05, 1.02, 0.99, 1.51, 1.33, 0.99, # 011-020
             1.44, 1.47, 1.33, 1.35, 1.35, 1.34, 1.33, 1.5 , 1.52, 1.45, # 021-030
             1.22, 1.17, 1.21, 1.22, 1.21, 1.5 , 1.47, 1.12, 1.78, 1.56, # 031-040
             1.48, 1.47, 1.35, 1.4 , 1.45, 1.5 , 1.59, 1.69, 1.63, 1.46, # 041-050
             1.46, 1.47, 1.4 , 1.5 , 1.67, 1.34, 1.87, 1.83, 1.82, 1.81, # 051-060
             1.8 , 1.8 , 1.99, 1.79, 1.76, 1.75, 1.74, 1.73, 1.72, 1.94, # 061-070
             1.72, 1.57, 1.43, 1.37, 1.35, 1.37, 1.32, 1.5 , 1.5 , 1.7 , # 071-080
             1.55, 1.54, 1.54, 1.68, 1.21, 1.5 , 1.5 , 1.9 , 1.88, 1.79, # 081-090
             1.61, 1.58, 1.55, 1.53, 1.51, 0.99, 1.54, 1.83, 1.5 , 1.5 , # 091-100
             1.5 , 1.5 , 1.5 , 1.5 , 1.5 , 1.5 , 1.5 , 1.5 , 1.5 , 1.5 ] # 101-110

table_es = _table_es
table_en = _table_en
table_cr = _table_cr

# default bond max distance calculation function and tolerance:
_bond_max_t = 0.25
bond_max_t = _bond_max_t
def _func_bond_max(en1, en2):
    return table_cr[en1]+_table_cr[en2]+bond_max_t

# default color mapping function:
def _func_color_map(en):
    if en == -1: return -0x333333
    if en == -2: return -0x999999
    #
    if en in (1, 3, 11, 19, 37, 55, 87): # IA
        return -0x21698c
    elif en in (4, 12, 20, 38, 56, 88): # IIA
        return -0xe77931
    elif en in (5, 13, 31, 49, 81): # IIIA
        return -0xde718c
    elif en in (6, 14, 32, 50, 82): # IVA
        return -0x6b6d6b
    elif en in (7, 15, 33, 51, 83): # VA
        return -0x5a59ad
    elif en in (8, 16, 34, 52, 84): # VIA
        return -0xd69273
    elif en in (9, 17, 35, 53, 85): # VIIA
        return -0x218e6b
    elif en in (2, 10, 18, 36, 54, 86): # O
        return -0xa52829
    else: # B
        return -0x84497b # -0x8c8ebd

# Group of atoms in the same Element-Number:
class AtomGroup:
    '''Group of atoms in the same Element-Number'''

    # Element-Number:
    en = 0
    # List of atoms:
    # ORTEP Type 8: (Label, x, y, z, U11, U22, U33, U12, U13, U23)
    # ORTEP Type 7: (Label, x, y, z, R)
    al = None #[]

    def __init__(self, en, al=None):
        self.en = en
        if al is None:
            self.al = []
        else:
            self.al = al
        return
    def append(self, ta):
        self.al.append(ta)
        return

# Tep-Instruction Writer:
class TepInWriter:
    '''Tep-Instruction Writer'''

    # CONST FIELDS:
    CONTENTS_AUNIT = 0
    CONTENTS_GROW = 1
    CONTENTS_CELL = 2
    VIEW_STD = 0
    VIEW_010EST = 1
    VIEW_100 = 2
    VIEW_010 = 3
    VIEW_001 = 4

    # list of atom-number-runs in tuple (N1, N2):
    _ganrs = None #[]
    # list of Element-Number:
    _gens = None #[]
    # atom-number of first, last:
    _an_first = 0
    _an_last = 0
    # atom-number of ORIG, CELLMDPT, CENTROID:
    _an_orig = 0
    _an_cellmdpt = 0
    _an_centroid = 0

    # output file:
    output = sys.stdout #None

    # title, cell, symmetries, atom-groups:
    title = ''
    cell = (10, 10, 10, 90, 90, 90)
    symmetries = ['+x, +y, +z']
    atomGroups = None #[]

    # contents scheme:
    contents = CONTENTS_AUNIT
    # initial view scheme:
    view = VIEW_010EST

    # width, height, margin and viewing-distance control:
    width = 10.7 # 11
    height = 8.2 # 8.5
    margin = 0.3 # 0.1
    distance = 30.0

    # rotation (DEG) in X, Y, Z axis:
    rx = 0
    ry = 0
    rz = 0

    # ellipsoid probability:
    probability = 50

    # default pen-color, pen-width:
    penColor = 0
    penWidth = 1.0
    # pen-color, pen-width for bond:
    #penColorBond = 0
    penWidthBond = 0.5
    # pen-color, pen-width for cell:
    #penColorCell = 0
    penWidthCell = 0.5
    # const pen-color, pen-width:
    constPenColor = False
    constPenWidth = False
    # monochrome = False

    # default atom-style, atom-label parameters:
    atomStyle = (4, 0, 4, 0)
    atomLabel = (0.18, 0.36, 0.36)
    # default bond-type, bond-radius:
    bondType = 5
    bondRadius = 0.05

    def __init__(self):
        self.funcBondMax = _func_bond_max
        self.funcColorMap = _func_color_map

    # title
    def writeTitle(self, s):
        if len(s)>72:
            s = s[0:72]
        self.output.write('%s\n'%s)
        return
    # cell tuple (a, b, c, alpha, beta, gamma)
    def writeCell(self, cell):
        # Type A: 1
        self.output.write('1%8.3f%9.3f%9.3f%9.3f%9.3f%9.3f\n' % cell)
        return
    # syms list of string
    def writeSymmetries(self, syms):
        last = len(syms)-1
        for i in range(0, last+1):
            if i==last:
                self.output.write('1   %s\n' % syms[i])
            else:
                self.output.write('    %s\n' % syms[i])
        return
    # atoms list of AtomGroup
    def writeAtoms(self, ags):
        self._ganrs = []
        self._gens = []
        sumf = [0, 0, 0]
        n = 0
        self._an_first = n+1
        for ag in ags:
            self._gens.append(ag.en)
            n0 = n+1
            for a in ag.al:
                n += 1
                sumf[0] += a[1]
                sumf[1] += a[2]
                sumf[2] += a[3]
                self.output.write('%-6s                     %9.5f%9.5f%9.5f        0\n'%a[0:4])
                if len(a) == 10:
                    self.output.write('%9.5f%9.5f%9.5f%9.5f%9.5f%9.5f        8\n'%a[4:10])
                else:
                    self.output.write('%9.5f                                                     7\n'%a[4])
            self._ganrs.append((n0, n))
        self._an_last = n
        # dummy atom ORIG:
        self.output.write('ORIG                         0.00000  0.00000  0.00000        7\n')
        self.output.write('  0.01000  0.00000  0.00000  0.00000  0.00000  0.00000        0\n')
        n += 1
        self._an_orig = n
        # dummy atom CELLMDPT:
        self.output.write('CELLMDPT                     0.50000  0.50000  0.50000        0\n')
        self.output.write('  0.00000  0.00000  0.00000  0.00000  0.00000  0.00000        0\n')
        n += 1
        self._an_cellmdpt = n
        # dummy atom CENTROID and terminate the atoms set:
        self.output.write('CENTROID                   %9.5f%9.5f%9.5f        0\n' % (sumf[0]/self._an_last, sumf[1]/self._an_last, sumf[2]/self._an_last))
        self.output.write('1 0.00000  0.00000  0.00000  0.00000  0.00000  0.00000        0\n')
        n += 1
        self._an_centroid = n
        return
    def writeComment(self, s=''):
        if len(s): # s should not contain line-breaks.
            self.output.write('# %s\n'%s)
        else:
            self.output.write('#\n')
        return
    # la 0, 1, 2, 3
    # ins I6
    # params list of F9, max length 7
    def writeCard0(self, la=0, ins=201, params=(), fmt='%9.3f'):
        self.output.write('%3d%6d'%(la, ins))
        if len(params)>7:
            raise Exception('Max length of params list exceeded')
        for p in params:
            self.output.write(fmt % p)
        self.output.write('\n')
        return
    def writeCard0_(self, la=0, ins=201, params=(0, 0, 0, 0, 0, 0, 0), fmt='%9.3f%9.3f%9.3f%9.3f%9.3f%9.3f%9.3f'):
        self.output.write('%3d%6d'%(la, ins))
        self.output.write(fmt % params)
        self.output.write('\n')
        return
    # la 0, 1, 2, 3
    # params list of F9, max length 7
    def writeCard1(self, la=0, params=(), fmt='%9.3f'):
        self.output.write('%3d      '%la)
        if len(params)>7:
            raise Exception('Max length of params list exceeded')
        for p in params:
            self.output.write(fmt % p)
        self.output.write('\n')
        return
    def writeCard1_(self, la=0, params=(0, 0, 0, 0, 0, 0, 0), fmt='%9.3f%9.3f%9.3f%9.3f%9.3f%9.3f%9.3f'):
        self.output.write('%3d      '%la)
        self.output.write(fmt % params)
        self.output.write('\n')
        return
    # la 0, 1, 2, 3
    # params1 list of I3, max length 5
    # params2 list of F6, max length 7
    def writeCard2(self, la=0, params1=(), params2=(), fmt1='%3d', fmt2='%6.2f'):
        self.output.write('%3d      '%la)
        if len(params1)>5:
            raise Exception('Max length of params1 list exceeded')
        for p in params1:
            self.output.write(fmt1 % p)
        npad = 5-len(params1)
        self.output.write('   '*npad)
        if len(params2)>7:
            raise Exception('Max length of params2 list exceeded')
        for p in params2:
            self.output.write(fmt2 % p)
        self.output.write('\n')
        return
    def writeCard2_(self, la=0, params1=(0, 0, 0, 0, 0), params2=(0, 0, 0, 0, 0, 0, 0),
                    fmt1='%3d%3d%3d%3d%3d', fmt2='%6.2f%6.2f%6.2f%6.2f%6.2f%6.2f%6.2f'):
        self.output.write('%3d      '%la)
        self.output.write(fmt1 % params1)
        self.output.write(fmt2 % params2)
        self.output.write('\n')
        return
    def writeCard3(self, s):
        if len(s)>72:
            s = s[0:72]
        self.output.write('%s\n'%s)
        return
    # write instructions to file:
    def write(self):
        self.writeTitle(self.title)
        self.writeCell(self.cell)
        self.writeSymmetries(self.symmetries)

        self.writeAtoms(self.atomGroups)
        ng = len(self._gens)

        self.writeComment('Set plot boundary, margin and view distance')
        self.writeComment('[F9:W, F9:H, F9:VD, F9:MRGN]')
        self.writeCard0(0, 301, (self.width, self.height, self.distance, self.margin))
        self.writeComment('Initialise plotter')
        self.writeCard0(0, 201)
        if not self.constPenColor:
            self.writeComment('Set pen color [I9:COLOR]')
            self.writeCard0_(0, 204, (self.penColor,), '%9i')
        if not self.constPenWidth:
            self.writeComment('Set pen width [F9:WIDTH]')
            self.writeCard0_(0, 205, (self.penWidth,), '%9.2f')
        if self.contents == TepInWriter.CONTENTS_CELL:
            self.writeComment('Store origin atoms for cell outline')
            self.writeCard0_(0, 401, (self._an_orig*100000+55501, -(self._an_orig*100000+66601)), '%9i%9i')
            self.writeComment('Add all atoms in box of enclosure')
            self.writeComment('[2F9:ADRO, 2F9:ANRT, F9:A/2, F9:B/2, F9:C/2]')
            self.writeCard0_(0, 404, (self._an_cellmdpt*100000+55501, self._an_cellmdpt*100000+55501,
                                      self._an_first, self._an_last,
                                      0.5, 0.5, 0.5),
                              '%9i%9i%9i%9i%9.3f%9.3f%9.3f')
            self.writeComment('Complete cell [2I9:ANRO, 2I9:ANRT, F9:DMAX]')
            self.writeComment('[2I3:ANRO, 2I3:ANRT, I3:TYPE, F6:DMIN, F6:DMAX]')
            self.writeCard0_(2, 406, (self._an_first, self._an_last, self._an_first, self._an_last, 4.0), '%9i%9i%9i%9i%9.3f')
            la = 2
            for i in range(0, ng):
                for j in range(0, ng):
                    if i== ng-1 and j == ng-1:
                        la = 0
                    self.writeCard2(la,
                                    (self._ganrs[i][0], self._ganrs[i][1], self._ganrs[j][0], self._ganrs[j][1], 0),
                                    (0.65, self.funcBondMax(self._gens[i], self._gens[j])))
        elif self.contents == TepInWriter.CONTENTS_GROW:
            self.writeComment('Insert the asymmetric unit [2I9:ADR]')
            self.writeCard0_(0, 401, (self._an_first*100000+55501, -(self._an_last*100000+55501)), '%9i%9i')
            #
            self.writeComment('Complete fragment [2I9:ANRO, 2I9:ANRT, F9:DMAX]')
            self.writeComment('[2I3:ANRO, 2I3:ANRT, I3:TYPE, F6:DMIN, F6:DMAX]')
            self.writeCard0_(2, 406, (self._an_first, self._an_last, self._an_first, self._an_last, 4.0), '%9i%9i%9i%9i%9.3f')
            la = 2
            for i in range(0, ng):
                for j in range(0, ng):
                    if i == ng-1 and j == ng-1: la = 0
                    self.writeCard2(la,
                                    (self._ganrs[i][0], self._ganrs[i][1], self._ganrs[j][0], self._ganrs[j][1], 0),
                                    (0.65, self.funcBondMax(self._gens[i], self._gens[j])))
        else: # TepInWriter.CONTENTS_AUNIT
            self.writeComment('Insert the asymmetric unit [2I9:ADR]')
            self.writeCard0_(0, 401, (self._an_first*100000+55501, -(self._an_last*100000+55501)), '%9i%9i')
        #
        if self.view == TepInWriter.VIEW_010EST:
            self.writeComment('Best plane view')
            self.writeCard0(2, 506)
            for i in range(0, ng):
                la = 2
                if i == ng-1: # last item
                    la = 0
                if self._gens[i] == 1: # H
                    self.writeCard2(la, (self._ganrs[i][0], self._ganrs[i][1], 0, 0, 0), (0,))
                else:
                    self.writeCard2(la, (self._ganrs[i][0], self._ganrs[i][1], 0, 0, 0), (1,))
        elif self.view == TepInWriter.VIEW_100:
            self.writeComment('View (100)')
            self.writeComment('[I9:ORIG, 2I9:ADRU, 2I9:ADRV, X9, I9:TYPE(1)]')
            ancx = self._an_centroid*100000
            self.writeCard0(0, 501, (ancx+55501, ancx+55501, ancx+56501, ancx+55501, ancx+55601, 0, 1), '%9i')
        elif self.view == TepInWriter.VIEW_010:
            self.writeComment('View (010)')
            self.writeComment('[I9:ORIG, 2I9:ADRU, 2I9:ADRV, X9, I9:TYPE(1)]')
            ancx = self._an_centroid*100000
            self.writeCard0(0, 501, (ancx+55501, ancx+55501, ancx+65501, ancx+55501, ancx+55601, 0, 1), '%9i')
        elif self.view == TepInWriter.VIEW_001:
            self.writeComment('View (001)')
            self.writeComment('[I9:ORIG, 2I9:ADRU, 2I9:ADRV, X9, I9:TYPE(1)]')
            ancx = self._an_centroid*100000
            self.writeCard0(0, 501, (ancx+55501, ancx+55501, ancx+65501, ancx+55501, ancx+56501, 0, 1), '%9i')
        else: # TepInWriter.VIEW_STD
            self.writeComment('Standard view')
            self.writeCard0(2, 505)
            for i in range(0, ng):
                la = 2
                if i == ng-1: # last item
                    la = 0
                if self._gens[i] == 1: # H
                    self.writeCard2(la, (self._ganrs[i][0], self._ganrs[i][1], 0, 0, 0), (0,))
                else:
                    self.writeCard2(la, (self._ganrs[i][0], self._ganrs[i][1], 0, 0, 0), (1,))
        #
        if self.rx != 0:
            self.writeComment('Rotation X [I9:AXIS(1), F9:ANGLE]')
            self.writeCard0_(0, 502, (1, self.rx), '%9i%9.3f')
        if self.ry != 0:
            self.writeComment('Rotation Y [I9:AXIS(2), F9:ANGLE]')
            self.writeCard0_(0, 502, (2, self.ry), '%9i%9.3f')
        if self.rz != 0:
            self.writeComment('Rotation Z [I9:AXIS(3), F9:ANGLE]')
            self.writeCard0_(0, 502, (3, self.rz), '%9i%9.3f')
        #
        self.writeComment('Apply ellipsoid scaling [3X9, I9:-PROB]')
        self.writeCard0_(0, 604, (0, 0, 0, - self.probability), '%9i%9i%9i%9i')
        self.writeComment('Apply incremental positioning and scaling')
        self.writeCard0(0, 611, (0, 0, 0.85)) # (0, 0, 0.9)
        self.writeCard0(0, 1001, (0.04,))
        #
        self.writeComment('All possible bonds (STORE)')
        for i in range(0, ng):
            self.writeComment('--%s--' % table_es[self._gens[i]])
            for j in range(i, ng):
                self.writeComment('%s-%s:' % (table_es[self._gens[i]], table_es[self._gens[j]]))
                self.writeCard0(2, 822)
                nbt = self.bondType
                br = self.bondRadius
                if self._gens[i] == 1: # H
                    nbt = 1
                    br = 0.01
                self.writeCard2(0,
                                (self._ganrs[i][0], self._ganrs[i][1], self._ganrs[j][0], self._ganrs[j][1], nbt),
                                (0.65, self.funcBondMax(self._gens[i], self._gens[j]), br))
        #
        if self.contents == TepInWriter.CONTENTS_CELL:
            self.writeComment('Explicit bond ORIGs for cell outline (STORE)')
            anox = self._an_orig*100000
            self.writeCard0_(2, 821, (anox+55501, anox+65501, anox+55501, anox+56501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 821, (anox+55501, anox+55601, anox+66601, anox+66501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 821, (anox+66601, anox+65601, anox+66601, anox+56601), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 821, (anox+56601, anox+55601, anox+56601, anox+56501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 821, (anox+66501, anox+65501, anox+66501, anox+56501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 821, (anox+65601, anox+55601, anox+65601, anox+65501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
        #
        self.writeComment('Draw atoms [4I9:STYLE, 3F9:LABEL]')
        self.writeComment('[2X9, 2I9:ANR]')
        for i in range(0, ng):
            if not self.constPenColor:
                self.writeComment('Change pen color [I9:COLOR]')
                self.writeCard0_(0, 204, (self.funcColorMap(self._gens[i]),), '%9i')
            self.writeComment('--%s--' % _table_es[self._gens[i]])
            ins = 705
            style = self.atomStyle
            esl = self.atomLabel
            if self.contents != TepInWriter.CONTENTS_AUNIT: # no atom label
                esl = (0, 0, 0)
            if self._gens[i] == 1: # H
                style = (0, 0, 0, 0)
                esl = (0, 0, 0)
                ins = 704
            self.writeCard0_(1, ins, style+esl, '%9i%9i%9i%9i%9.3f%9.3f%9.3f')
            self.writeCard1_(0, (0, 0, self._ganrs[i][0], self._ganrs[i][1]), '%9i%9i%9i%9i')
        #
        if not self.constPenColor:
            self.writeComment('Change pen color [I9:COLOR]')
            self.writeCard0_(0, 204, (self.funcColorMap(-1),), '%9i') # self.penColorBond
        if not self.constPenWidth:
            self.writeComment('Change pen width [F9:WIDTH]')
            self.writeCard0_(0, 205, (self.penWidthBond,), '%9.2f')
        self.writeComment('All possible bonds (DRAW)')
        for i in range(0, ng):
            self.writeComment('--%s--' % table_es[self._gens[i]])
            for j in range(i, ng):
                self.writeComment('%s-%s:' % (table_es[self._gens[i]], table_es[self._gens[j]]))
                self.writeCard0(2, 812)
                nbt = self.bondType
                br = self.bondRadius
                if self._gens[i] == 1: # H
                    nbt = 1
                    br = 0.01
                self.writeCard2(0,
                                (self._ganrs[i][0], self._ganrs[i][1], self._ganrs[j][0], self._ganrs[j][1], nbt),
                                (0.65, self.funcBondMax(self._gens[i], self._gens[j]), br))
        #
        if self.contents == TepInWriter.CONTENTS_CELL:
            if not self.constPenColor:
                self.writeComment('Change pen color [I9:COLOR]')
                self.writeCard0_(0, 204, (self.funcColorMap(-2),), '%9i') # self.penColorCell
            if not self.constPenWidth:
                self.writeComment('Change pen width [F9:WIDTH]')
                self.writeCard0_(0, 205, (self.penWidthCell,), '%9.2f')
            self.writeComment('Explicit bond ORIGs for cell outline (DRAW)')
            self.writeComment('[2I9:ADR1, 2I9:ADR2]')
            self.writeComment('[4X3, I3:TYPE, 2X6, F6:RADIUS]')
            anox = self._an_orig*100000
            self.writeCard0_(2, 811, (anox+55501, anox+65501, anox+55501, anox+56501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 811, (anox+55501, anox+55601, anox+66601, anox+66501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 811, (anox+66601, anox+65601, anox+66601, anox+56601), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 811, (anox+56601, anox+55601, anox+56601, anox+56501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 811, (anox+66501, anox+65501, anox+66501, anox+56501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
            self.writeCard0_(2, 811, (anox+65601, anox+55601, anox+65601, anox+65501), '%9i%9i%9i%9i')
            self.writeCard2(0, (0, 0, 0, 0, 5), (0, 0, 0.01))
        #
        self.writeComment('Close plot')
        self.writeCard0(0, 202)
        self.writeComment('End instructions')
        self.writeCard0(0, -1)
        return
