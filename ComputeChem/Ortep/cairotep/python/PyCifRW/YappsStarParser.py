from StarFile import *
from types import *
import copy
# An alternative specification for the Cif Parser, based on Yapps2
# by Amit Patel (http://theory.stanford.edu/~amitp/Yapps)
#
# helper code: we define our match tokens
lastval = ''
def monitor(location,value):
    global lastval
    # print 'At %s: %s' % (location,`value`)
    lastval = `value`
    return value

# Strip extras gets rid of leading and trailing whitespace, and
# semicolons.
def stripextras(value):
    # we get rid of semicolons and leading/trailing terminators etc.
     import re
     jj = re.compile("[\n\r\f \t\v]*")
     semis = re.compile("[\n\r\f \t\v]*[\n\r\f]\n*;")
     cut = semis.match(value)
     if cut:        #we have a semicolon-delimited string
          nv = value[cut.end():len(value)-2]
          try:
             if nv[-1]=='\r': nv = nv[:-1]
          except IndexError:    #empty data value
             pass
          return nv 
     else: 
          cut = jj.match(value)
          if cut:
               return stripstring(value[cut.end():])
          return value

# helper function to get rid of inverted commas etc.

def stripstring(value):
     if value:
         if value[0]== '\'' and value[-1]=='\'':
           return value[1:-1]
         if value[0]=='"' and value[-1]=='"':
           return value[1:-1]
     return value

# helper function to populate a nested LoopBlock structure given an
# empty structure together with listed values.   The values are 
# organised into a list of lists, where each time 'stop' was
# encountered one list terminates and a new one starts. 
# For a correctly constructed loop, the final 'popout' will pop out
# of the iteration completely and raise a StopIteration error.
#
# Note that there may be an empty list at the very end of our itemlists,
# so we remove that if necessary.
#
# We optimise for CIF files by loading differently if we have a flat loop

def makeloop(loopstructure,itemlists):
    if itemlists[-1] == []: itemlists.pop(-1)
    # print 'Making loop with %s' % `itemlists`
    if loopstructure.dimension == 1 and loopstructure.loops == []:
        storage_iter = loopstructure.fast_load_iter()
    else:
        storage_iter = loopstructure.load_iter()
    nowloop = loopstructure
    for datalist in itemlists:
       for datavalue in datalist:
           try:
               nowloop,target = storage_iter.next()
           except StopIteration:
               print "StopIter at %s/%s" % (datavalue,datalist)
               raise StopIteration
           # print 'Got %s %s ->' % (`nowloop`,`target`),
           target.append(datavalue)
           # print '%s' % `target`
       # the end of each list is the same as a stop_ token
       # print 'Saw end of list'
       nowloop.popout = True
       nowloop,blank = storage_iter.next()  #execute the pop
       # print 'discarding %s/%s' % (`nowloop`,`blank`)
    # print 'Makeloop returning %s' % `loopstructure`
    return loopstructure

# return an object with the appropriate amount of nesting
def make_empty(nestlevel):
    gd = []
    for i in range(1,nestlevel):
        gd = [gd]
    return gd

# this function updates a dictionary first checking for name collisions,
# which imply that the CIF is invalid.  We need case insensitivity for
# names. 

# Unfortunately we cannot check loop item contents against non-loop contents
# in a non-messy way during parsing, as we may not have easy access to previous
# key value pairs in the context of our call (unlike our built-in access to all 
# previous loops).
# For this reason, we don't waste time checking looped items against non-looped
# names during parsing of a data block.  This would only match a subset of the
# final items.   We do check against ordinary items, however.
#
# Note the following situations:
# (1) new_dict is empty -> we have just added a loop; do no checking
# (2) new_dict is not empty -> we have some new key-value pairs
#
def cif_update(old_dict,new_dict,loops):
    old_keys = map(lambda a:a.lower(),old_dict.keys())
    if new_dict != {}:    # otherwise we have a new loop
        #print 'Comparing %s to %s' % (`old_keys`,`new_dict.keys()`)
        for new_key in new_dict.keys():
            if new_key.lower() in old_keys:
                raise CifError, "Duplicate dataname or blockname %s in input file" % new_key
            old_dict[new_key] = new_dict[new_key]
#
# this takes two lines, so we couldn't fit it into a one line execution statement...
def order_update(order_array,new_name):
    order_array.append(new_name) 
    return new_name

from string import *
import re
from yapps_compiled_rt import *

class StarParserScanner(Scanner):
    patterns = [
        ('([ \t\n\r](?!;))|[ \t]', re.compile('([ \t\n\r](?!;))|[ \t]')),
        ('#.*[\n\r](?!;)', re.compile('#.*[\n\r](?!;)')),
        ('#.*', re.compile('#.*')),
        ('LBLOCK', re.compile('(L|l)(O|o)(O|o)(P|p)_')),
        ('GLOBAL', re.compile('(G|g)(L|l)(O|o)(B|b)(A|a)(L|l)_')),
        ('STOP', re.compile('(S|s)(T|t)(O|o)(P|p)_')),
        ('save_heading', re.compile('(S|s)(A|a)(V|v)(E|e)_[][!%&\\(\\)*+,./:<=>?@0-9A-Za-z\\\\^`{}\\|~"#$\';_-]+')),
        ('save_end', re.compile('(S|s)(A|a)(V|v)(E|e)_')),
        ('data_name', re.compile('_[][!%&\\(\\)*+,./:<=>?@0-9A-Za-z\\\\^`{}\\|~"#$\';_-]+')),
        ('data_heading', re.compile('(D|d)(A|a)(T|t)(A|a)_[][!%&\\(\\)*+,./:<=>?@0-9A-Za-z\\\\^`{}\\|~"#$\';_-]+')),
        ('start_sc_line', re.compile('(\n|\r\n);([^\n\r])*(\r\n|\r|\n)+')),
        ('sc_line_of_text', re.compile('[^;\r\n]([^\r\n])*(\r\n|\r|\n)+')),
        ('end_sc_line', re.compile(';')),
        ('data_value_1', re.compile('((?!(((S|s)(A|a)(V|v)(E|e)_[^\\s]*)|((G|g)(L|l)(O|o)(B|b)(A|a)(L|l)_[^\\s]*)|((S|s)(T|t)(O|o)(P|p)_[^\\s]*)|((D|d)(A|a)(T|t)(A|a)_[^\\s]*)))[^\\s"#$\'_\\[\\]][^\\s]*)|\'((\'(?=\\S))|([^\n\r\x0c\']))*\'+|"(("(?=\\S))|([^\n\r"]))*"+')),
        ('END', re.compile('$')),
    ]
    def __init__(self, str):
        Scanner.__init__(self,None,['([ \t\n\r](?!;))|[ \t]', '#.*[\n\r](?!;)', '#.*'],str)

class StarParser(Parser):
    def input(self):
        _token_ = self._peek('END', 'data_heading')
        if _token_ == 'data_heading':
            dblock = self.dblock()
            allblocks = StarFile(); allblocks.NewBlock(dblock[0],blockcontents=dblock[1],fix=False,replace=False)
            while self._peek('END', 'data_heading') == 'data_heading':
                dblock = self.dblock()
                allblocks.NewBlock(dblock[0],blockcontents=monitor('input',dblock[1]),fix=False,replace=False)
            END = self._scan('END')
        else: # == 'END'
            END = self._scan('END')
            allblocks = StarFile()
        return allblocks

    def dblock(self):
        data_heading = self._scan('data_heading')
        heading = data_heading[5:];thisblock=StarBlock(overwrite=False)
        while self._peek('save_heading', 'LBLOCK', 'data_name', 'save_end', 'END', 'data_heading') in ['save_heading', 'LBLOCK', 'data_name']:
            _token_ = self._peek('save_heading', 'LBLOCK', 'data_name')
            if _token_ != 'save_heading':
                dataseq = self.dataseq(thisblock)
            else: # == 'save_heading'
                save_frame = self.save_frame()
                thisblock["saves"].NewBlock(save_frame[0],save_frame[1],fix=False,replace=True)
        return (heading,monitor('dblock',thisblock))

    def dataseq(self, starblock):
        data = self.data(starblock)
        while self._peek('LBLOCK', 'data_name', 'save_end', 'save_heading', 'END', 'data_heading') in ['LBLOCK', 'data_name']:
            data = self.data(starblock)

    def data(self, currentblock):
        _token_ = self._peek('LBLOCK', 'data_name')
        if _token_ == 'LBLOCK':
            top_loop = self.top_loop()
            currentblock.insert_loop(top_loop,audit=False)
        else: # == 'data_name'
            datakvpair = self.datakvpair()
            currentblock.AddLoopItem(datakvpair,precheck=True)

    def datakvpair(self):
        data_name = self._scan('data_name')
        data_value = self.data_value()
        return (data_name,data_value)

    def data_value(self):
        _token_ = self._peek('data_value_1', 'start_sc_line')
        if _token_ == 'data_value_1':
            data_value_1 = self._scan('data_value_1')
            thisval = stripstring(data_value_1)
        else: # == 'start_sc_line'
            sc_lines_of_text = self.sc_lines_of_text()
            thisval = stripextras(sc_lines_of_text)
        return monitor('data_value',thisval)

    def sc_lines_of_text(self):
        start_sc_line = self._scan('start_sc_line')
        lines = start_sc_line
        while self._peek('sc_line_of_text', 'end_sc_line') == 'sc_line_of_text':
            sc_line_of_text = self._scan('sc_line_of_text')
            lines = lines+sc_line_of_text
        end_sc_line = self._scan('end_sc_line')
        return monitor('sc_line_of_text',lines+end_sc_line)

    def top_loop(self):
        LBLOCK = self._scan('LBLOCK')
        loopfield = self.loopfield()
        loopvalues = self.loopvalues()
        return makeloop(loopfield,loopvalues)

    def loopfield(self):
        toploop=LoopBlock(dimension=1,overwrite=False);curloop=toploop;poploop=None;dim=1
        while self._peek('data_name', 'LBLOCK', 'STOP', 'data_value_1', 'start_sc_line') not in ['data_value_1', 'start_sc_line']:
            _token_ = self._peek('data_name', 'LBLOCK', 'STOP')
            if _token_ == 'data_name':
                data_name = self._scan('data_name')
                curloop[data_name]=[]
            elif _token_ == 'LBLOCK':
                LBLOCK = self._scan('LBLOCK')
                dim=dim+1;newloop=LoopBlock(dimension=dim,overwrite=False);poploop=curloop;curloop.insert_loop(newloop,audit=False);curloop=newloop
            else: # == 'STOP'
                STOP = self._scan('STOP')
                curloop=poploop;dim=dim-1
        return toploop

    def loopvalues(self):
        data_value = self.data_value()
        dataloop=[[data_value]]
        while self._peek('data_value_1', 'STOP', 'start_sc_line', 'LBLOCK', 'data_name', 'save_end', 'save_heading', 'END', 'data_heading') in ['data_value_1', 'STOP', 'start_sc_line']:
            _token_ = self._peek('data_value_1', 'STOP', 'start_sc_line')
            if _token_ != 'STOP':
                data_value = self.data_value()
                dataloop[-1].append(monitor('loopval',data_value))
            else: # == 'STOP'
                STOP = self._scan('STOP')
                dataloop.append([])
        return dataloop

    def save_frame(self):
        save_heading = self._scan('save_heading')
        savehead = save_heading[5:];savebody = StarBlock(overwrite=False)
        while self._peek('save_end', 'LBLOCK', 'data_name', 'save_heading', 'END', 'data_heading') in ['LBLOCK', 'data_name']:
            dataseq = self.dataseq(savebody)
        save_end = self._scan('save_end')
        return (savehead,monitor('save_frame',savebody))


def parse(rule, text):
    P = StarParser(StarParserScanner(text))
    return wrap_error_reporter(P, rule)



