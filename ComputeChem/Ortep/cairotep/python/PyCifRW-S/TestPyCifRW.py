# We attempt to implement testing of the PyCif module using
# the PyUnit framework
#
# 
# note that some tests rely on the presence of the StarScan
# dynamic library under Linux, so will be redundant on other
# platforms
#
import unittest, CifFile
import StarFile
import re

# Test basic setting and reading of the CifBlock

class BlockRWTestCase(unittest.TestCase):
    def setUp(self):
    	# we want to get a datablock ready so that the test
	# case will be able to write a single item
	self.cf = CifFile.CifBlock()

    def tearDown(self):
        # get rid of our test object
	del self.cf
	
    def testTupleNumberSet(self):
        """Test tuple setting with numbers"""
        self.cf['_test_tuple'] = (11,13.5,-5.6)
        self.failUnless(map(float,
	     self.cf['_test_tuple']))== [11,13.5,-5.6]

    def testTupleComplexSet(self):
        """Test setting multiple names in loop"""
	names = (('_item_name_1','_item_name#2','_item_%$#3'),)
	values = (((1,2,3,4),('hello','good_bye','a space','# 4'),
	          (15.462, -99.34,10804,0.0001)),)
        self.cf.AddCifItem((names,values))
	self.failUnless(tuple(map(float, self.cf[names[0][0]])) == values[0][0])
	self.failUnless(tuple(self.cf[names[0][1]]) == values[0][1])
	self.failUnless(tuple(map(float, self.cf[names[0][2]])) == values[0][2])

    def testStringSet(self):
        """test string setting"""
        self.cf['_test_string_'] = 'A short string'
	self.failUnless(self.cf['_test_string_'] == 'A short string')

    def testTooLongSet(self):
        """test setting overlong data names"""
        dataname = '_a_long_long_'*7
        try:
            self.cf[dataname] = 1.0
        except (StarFile.StarError,CifFile.CifError): pass
        else: self.fail()

    def testTooLongLoopSet(self):
        """test setting overlong data names in a loop"""
        dataname = '_a_long_long_'*7
        try:
            self.cf[dataname] = (1.0,2.0,3.0)
        except (StarFile.StarError,CifFile.CifError): pass
        else: self.fail()

    def testBadStringSet(self):
        """test setting values with bad characters"""
        dataname = '_name_is_ok'
        try:
            self.cf[dataname] = "eca234\f\vaqkadlf"
        except StarFile.StarError: pass
        else: self.fail()

    def testBadNameSet(self):
        """test setting names with bad characters"""
        dataname = "_this_is_not ok"
        try:
            self.cf[dataname] = "nnn"
        except StarFile.StarError: pass
        else: self.Fail()

    def testMoreBadStrings(self):
        dataname = "_name_is_ok"
        val = "so far, ok, but now we have a " + chr(128)
        try:
            self.cf[dataname] = val
        except StarFile.StarError: pass
        else: self.Fail()

    def testEmptyString(self):
        """An empty string is, in fact, legal"""
        self.cf['_an_empty_string'] = ''
        
# Now test operations which require a preexisting block
#

class BlockChangeTestCase(unittest.TestCase):
   def setUp(self):
        self.cf = CifFile.CifBlock()
	self.names = (('_item_name_1','_item_name#2','_item_%$#3'),)
	self.values = (((1,2,3,4),('hello','good_bye','a space','# 4'),
	          (15.462, -99.34,10804,0.0001)),)
        self.cf.AddCifItem((self.names,self.values))
	self.cf['_non_loop_item'] = 'Non loop string item'
	self.cf['_number_item'] = 15.65
       
   def tearDown(self):
       del self.cf

   def testFromBlockSet(self):
        """Test that we can use a CifBlock to set a CifBlock"""
        df = CifFile.CifFile()
        df.NewBlock('testname',self.cf)

   def testLoop(self):
        """Check GetLoop returns values and names in matching order"""
   	results = self.cf.GetLoop(self.names[0][2])
	for key in results.keys():
	    self.failUnless(key in self.names[0])
	    self.failUnless(tuple(results[key]) == self.values[0][list(self.names[0]).index(key)])
	
   def testSimpleRemove(self):
       """Check item deletion outside loop"""
       self.cf.RemoveCifItem('_non_loop_item')
       try:
           a = self.cf['_non_loop_item']
       except KeyError: pass
       else: self.Fail()

   def testLoopRemove(self):
       """Check item deletion inside loop"""
       self.cf.RemoveCifItem(self.names[0][1])
       try:
           a = self.cf[self.names[0][1]]
       except KeyError: pass
       else: self.Fail()

   def testFullLoopRemove(self):
       """Check removal of all loop items"""
       for name in self.names[0]: self.cf.RemoveCifItem(name)
       self.failUnless(len(self.cf.loops)==0, `self.cf.loops`)

# test adding data to a loop.  We test straight addition, then make sure the errors
# happen at the right time
#
   def testAddToLoop(self):
       """Test adding to a loop"""
       adddict = {'_address':['1 high street','2 high street','3 high street','4 high st'],
                  '_address2':['Ecuador','Bolivia','Colombia','Mehico']}
       self.cf.AddToLoop('_item_name#2',adddict)
       newkeys = self.cf.GetLoop('_item_name#2').keys()
       self.failUnless(adddict.keys()[0] in newkeys)
       self.failUnless(len(self.cf.GetLoop('_item_name#2'))==len(self.values[0])+2)
       
   def testBadAddToLoop(self):
       """Test incorrect loop addition"""
       adddict = {'_address':['1 high street','2 high street','3 high street'],
                  '_address2':['Ecuador','Bolivia','Colombia']}
       try:
           self.cf.AddToLoop('_no_item',adddict)
       except KeyError: pass
       else: self.Fail()
       try:
           self.cf.AddToLoop('_item_name#2',adddict)
       except StarFile.StarLengthError:
           pass 
       else: self.Fail()

   def testChangeLoop(self):
       """Test changing pre-existing item in loop"""
       # Items should be silently replaced, but if an
       # item exists in a loop already, it should be
       # deleted from that loop first
       self.cf["_item_name_1"] = (5,6,7,8)

#
#  Test the mapping type implementation
#
   def testGetOperation(self):
       """Test the get mapping call"""
       self.cf.get("_item_name_1")
       self.cf.get("_item_name_nonexist")

#
#  Test case insensitivity
#
   def testDataNameCase(self):
       """Test same name, different case causes error"""
       self.assertEqual(self.cf["_Item_Name_1"],self.cf["_item_name_1"])
       self.cf["_Item_NaMe_1"] = "the quick pewse fox"
       self.assertEqual(self.cf["_Item_NaMe_1"],self.cf["_item_name_1"])

#  Test iteration
#
   def testIteration(self):
       """We create an iterator and iterate"""
       testloop = self.cf.GetLoop("_item_name_1")
       i = 0
       for test_pack in testloop:
           self.assertEqual(test_pack["_item_name_1"],self.values[0][0][i]) 
           self.assertEqual(test_pack["_item_name#2"],self.values[0][1][i]) 
           i += 1

#
#  Test changing item order
#
   def testChangeOrder(self):
       """We move some stuff around"""
       testloop = self.cf.GetLoop("_item_name_1")
       self.cf.ChangeItemOrder("_Number_Item",0)
       testloop.ChangeItemOrder("_Item_Name_1",2)
       self.assertEqual(testloop.GetItemOrder()[2],"_Item_Name_1")
       self.assertEqual(self.cf.GetItemOrder()[0],"_Number_Item")
       
#
#  Test setting of block names
#

class BlockNameTestCase(unittest.TestCase):
   def testBlockName(self):
       """Make sure long block names cause errors"""
       df = CifFile.CifBlock()
       cf = CifFile.CifFile()
       try:
           cf['a_very_long_block_name_which_should_be_rejected_out_of_hand123456789012345678']=df
       except CifFile.CifError: pass
       else: self.fail()

   def testBlockOverwrite(self):
       """Upper/lower case should be seen as identical"""
       df = CifFile.CifBlock()
       ef = CifFile.CifBlock()
       cf = CifFile.CifFile()
       df['_random_1'] = 'oldval'
       ef['_random_1'] = 'newval'
       cf['_lowercaseblock'] = df
       cf['_LowerCaseBlock'] = ef
       assert(cf['_Lowercaseblock']['_random_1'] == 'newval')
       assert(len(cf) == 1)
       

class FileWriteTestCase(unittest.TestCase):
   def setUp(self):
       """Write out a file, then read it in again. Non alphabetic ordering to
          check order preservation."""
       # fill up the block with stuff
       items = (('_item_1','Some data'),
             ('_item_3','34.2332'),
             ('_item_4','Some very long data which we hope will overflow the single line and force printing of another line aaaaa bbbbbb cccccc dddddddd eeeeeeeee fffffffff hhhhhhhhh iiiiiiii jjjjjj'),
             ('_item_2','Some_underline_data'),
             ('_item_empty',''),
             ('_item_quote',"'ABC"),
             ('_item_apost','"def'),
             ('_item_sws'," \n "),
             (('_item_5','_item_7','_item_6'),
             ([1,2,3,4],
              ['a','b','c','d'],
              [5,6,7,8])),
             (('_string_1','_string_2'),
              ([';this string begins with a semicolon',
               'this string is way way too long and should overflow onto the next line eventually if I keep typing for long enough',
               ';just_any_old_semicolon-starting-string'],
               ['a string with a final quote"',
               'a string with a " and a safe\';',
               'a string with a final \''])))
       # save block items as well
       s_items = (('_sitem_1','Some save data'),
             ('_sitem_2','Some_underline_data'),
             ('_sitem_3','34.2332'),
             ('_sitem_4','Some very long data which we hope will overflow the single line and force printing of another line aaaaa bbbbbb cccccc dddddddd eeeeeeeee fffffffff hhhhhhhhh iiiiiiii jjjjjj'),
             (('_sitem_5','_sitem_6','_sitem_7'),
             ([1,2,3,4],
              [5,6,7,8],
              ['a','b','c','d'])),
             (('_string_1','_string_2'),
              ([';this string begins with a semicolon',
               'this string is way way too long and should overflow onto the next line eventually if I keep typing for long enough',
               ';just_any_old_semicolon-starting-string'],
               ['a string with a final quote"',
               'a string with a " and a safe\';',
               'a string with a final \''])))
       self.cf = CifFile.CifBlock(items)
       self.save_block = CifFile.CifBlock(s_items)
       self.cf["saves"]["test_save_frame"] = self.save_block
       self.cfs = self.cf["saves"]["test_save_frame"]
       cif = CifFile.CifFile()
       cif['testblock'] = self.cf
       outfile = open('test.cif','w')
       outfile.write(str(cif))
       outfile.close()
       self.ef = CifFile.CifFile('test.cif')
       self.df = self.ef['testblock']
       self.dfs = self.df["saves"]["test_save_frame"]
       flfile = CifFile.ReadCif('test.cif',scantype="flex")
       self.flf = flfile['testblock']
       self.flfs = self.flf["saves"]["test_save_frame"]

   def tearDown(self):
       import os
       #os.remove('test.cif')
       del self.dfs
       del self.df
       del self.cf
       del self.ef
       del self.flf
       del self.flfs

   def testStringInOut(self):
       """Test writing short strings in and out"""
       self.failUnless(self.cf['_item_1']==self.df['_item_1'])
       self.failUnless(self.cf['_item_2']==self.df['_item_2'])
       self.failUnless(self.cfs['_sitem_1']==self.dfs['_sitem_1'])
       self.failUnless(self.cfs['_sitem_2']==self.dfs['_sitem_2'])
       self.failUnless(self.cfs['_sitem_1']==self.flfs['_sitem_1'])
       self.failUnless(self.cfs['_sitem_2']==self.flfs['_sitem_2'])

   def testApostropheInOut(self):
       """Test correct behaviour for values starting with apostrophes
       or quotation marks"""
       self.failUnless(self.cf['_item_quote']==self.df['_item_quote'])
       self.failUnless(self.cf['_item_apost']==self.df['_item_apost'])
       self.failUnless(self.cf['_item_quote']==self.flf['_item_quote'])
       self.failUnless(self.cf['_item_apost']==self.flf['_item_apost'])
       
   def testNumberInOut(self):
       """Test writing number in and out"""
       self.failUnless(self.cf['_item_3']==(self.df['_item_3']))
       self.failUnless(self.cfs['_sitem_3']==(self.dfs['_sitem_3']))
       self.failUnless(self.cf['_item_3']==(self.flf['_item_3']))
       self.failUnless(self.cfs['_sitem_3']==(self.flfs['_sitem_3']))

   def testLongStringInOut(self):
       """Test writing long string in and out
          Note that whitespace may vary due to carriage returns,
	  so we remove all returns before comparing"""
       import re
       compstring = re.sub('\n','',self.df['_item_4'])
       self.failUnless(compstring == self.cf['_item_4'])
       compstring = re.sub('\n','',self.dfs['_sitem_4'])
       self.failUnless(compstring == self.cfs['_sitem_4'])
       compstring = re.sub('\n','',self.flf['_item_4'])
       self.failUnless(compstring == self.cf['_item_4'])
       compstring = re.sub('\n','',self.flfs['_sitem_4'])
       self.failUnless(compstring == self.cfs['_sitem_4'])

   def testEmptyStringInOut(self):
       """An empty string is in fact kosher""" 
       self.failUnless(self.cf['_item_empty']=='')
       self.failUnless(self.flf['_item_empty']=='')

   def testSemiWhiteSpace(self):
       """Test that white space in a semicolon string is preserved"""
       self.failUnless(self.cf['_item_sws']==self.df['_item_sws'])
       self.failUnless(self.cf['_item_sws']==self.flf['_item_sws'])

   def testLoopDataInOut(self):
       """Test writing in and out loop data"""
       olditems = self.cf.GetLoop('_item_5')
       for key,value in olditems.items():
           self.failUnless(tuple(map(str,value))==tuple(self.df[key]))
           self.failUnless(tuple(map(str,value))==tuple(self.flf[key]))
       # save frame test
       olditems = self.cfs.GetLoop('_sitem_5').items()
       for key,value in olditems:
           self.failUnless(tuple(map(str,value))==tuple(self.dfs[key]))
           self.failUnless(tuple(map(str,value))==tuple(self.flfs[key]))

   def testLoopStringInOut(self):
       """Test writing in and out string loop data"""
       olditems = self.cf.GetLoop('_string_1')
       newitems = self.df.GetLoop('_string_1')
       flexnewitems = self.flf.GetLoop('_string_1')
       for key,value in olditems.items():
           compstringa = map(lambda a:re.sub('\n','',a),value)
           compstringb = map(lambda a:re.sub('\n','',a),self.df[key])
           compstringc = map(lambda a:re.sub('\n','',a),self.flf[key])
           self.failUnless(compstringa==compstringb and compstringa==compstringc)

   def testGetLoopData(self):
       """Test the get method for looped data"""
       newvals = self.df.get('_string_1')
       self.failUnless(len(newvals)==3)

   def testAddSaveFrame(self):
       """Test adding a save frame"""
       s_items = (('_sitem_1','Some save data'),
             ('_sitem_2','Some_underline_data'),
             ('_sitem_3','34.2332'),
             ('_sitem_4','Some very long data which we hope will overflow the single line and force printing of another line aaaaa bbbbbb cccccc dddddddd eeeeeeeee fffffffff hhhhhhhhh iiiiiiii jjjjjj'),
             (('_sitem_5','_sitem_6','_sitem_7'),
             ([1,2,3,4],
              [5,6,7,8],
              ['a','b','c','d'])))
       bb = CifFile.CifBlock(s_items)
       self.cf["saves"]["some_name"]=bb

   def testCopySaveFrame(self):
       """Early implementations didn't copy the save frame properly"""
       jj = CifFile.CifFile(self.ef)  #this will trigger a copy
       self.failUnless(len(jj["testblock"]["saves"])>0)

   def testProperCifFile(self):
       """We test that all objects have been converted from Star objects"""
       print "Classnames:\n  CifFile is %s" % `self.ef.__class__`
       self.failUnless(self.ef.__class__==CifFile.CifFile)
       print "  CifBlock is %s" % `self.df.__class__`
       self.failUnless(self.df.__class__==CifFile.CifBlock)
       jj = self.df.GetLoop('_item_5')
       print "  CifLoop is %s" % `jj.__class__`
       self.failUnless(jj.__class__==CifFile.CifLoopBlock)
       jj = self.dfs.GetLoop('_sitem_5')
       print "  Save frame loop is %s" % `jj.__class__` 
       self.failUnless(jj.__class__==CifFile.CifLoopBlock)

   def testUpdateItemOrder(self):
       """Tests that when the StarBlock is remade into a CifBlock
          the loops are properly adjusted in the item order"""
       jj = self.df.GetLoop('_item_5')
       self.failUnless(jj in self.df.item_order, "Wanted %s, got %s" % (`jj`,`self.df.item_order`))

##############################################################
#
# Test dictionary type
#
##############################################################
ddl1dic = CifFile.CifDic("dictionaries/cif_core.dic")
class DictTestCase(unittest.TestCase):
    def setUp(self):
	pass
    
    def tearDown(self):
	pass

    def testnum_and_esd(self):
        """Test conversion of numbers with esds"""
        testnums = ["5.65","-76.24(3)","8(2)","6.24(3)e3","55.2(2)d4"]
        res = map(CifFile.get_number_with_esd,testnums)
        print `res`
        self.failUnless(res[0]==(5.65,None))
        self.failUnless(res[1]==(-76.24,0.03))
        self.failUnless(res[2]==(8,2))
        self.failUnless(res[3]==(6240,30))
        self.failUnless(res[4]==(552000,2000))
         
    def testdot(self):
        """Make sure a single dot is skipped"""
        res1,res2 = CifFile.get_number_with_esd(".")
        self.failUnless(res1==None)

##############################################################
#
#  Validation testing
#
##############################################################

# We first test single item checking
class DDL1TestCase(unittest.TestCase):

    def setUp(self):
	#self.ddl1dic = CifFile.CifFile("dictionaries/cif_core.dic")
	#items = (("_atom_site_label","S1"),
	#	 ("_atom_site_fract_x","0.74799(9)"),
        #         ("_atom_site_adp_type","Umpe"),
	#	 ("_this_is_not_in_dict","not here"))
	bl = CifFile.CifBlock()
	self.cf = CifFile.ValidCifFile(dic=ddl1dic)
	self.cf["test_block"] = bl
	self.cf["test_block"].AddCifItem(("_atom_site_label",
	      ["C1","Cr2","H3","U4"]))	

    def tearDown(self):
        del self.cf

    def testItemType(self):
        """Test that types are correctly checked and reported"""
        #numbers
        self.cf["test_block"]["_diffrn_radiation_wavelength"] = "0.75"
        try:
            self.cf["test_block"]["_diffrn_radiation_wavelength"] = "moly"
        except CifFile.ValidCifError: pass

    def testItemEsd(self):
        """Test that non-esd items are not allowed with esds"""
        #numbers
        try:
            self.cf["test_block"]["_chemical_melting_point_gt"] = "1325(6)"
        except CifFile.ValidCifError: pass

    def testItemEnum(self):
        """Test that enumerations are understood"""
        self.cf["test_block"]["_diffrn_source_target"]="Cr"
        try:
            self.cf["test_block"]["_diffrn_source_target"]="2.5"
        except CifFile.ValidCifError: pass 
        else: self.Fail()

    def testItemRange(self):
        """Test that ranges are correctly handled"""
        self.cf["test_block"]["_diffrn_source_power"] = "0.0"
        self.cf["test_block"]["_diffrn_standards_decay_%"] = "98"

    def testItemLooping(self):
        """test that list yes/no/both works"""
        pass

    def testListReference(self):
        """Test that _list_reference is handled correctly"""
        #can be both looped and unlooped; if unlooped, no need for ref.
        self.cf["test_block"]["_diffrn_radiation_wavelength"] = "0.75"
        try:
            self.cf["test_block"].AddCifItem(((
                "_diffrn_radiation_wavelength",
                "_diffrn_radiation_wavelength_wt"),(("0.75","0.71"),("0.5","0.1"))))
        except CifFile.ValidCifError: pass
        else: self.Fail()
        
    def testUniqueness(self):
        """Test that non-unique values are found"""
        # in cif_core.dic only one set is available
        try:
            self.cf["test_block"].AddCifItem(((
                "_publ_body_label",
                "_publ_body_element"),
                  (
                   ("1.1","1.2","1.3","1.2"),
                   ("section","section","section","section") 
                     )))
        except CifFile.ValidCifError: pass
        else: self.Fail()

    def testParentChild(self):
	"""Test that non-matching values are reported"""
        self.assertRaises(CifFile.ValidCifError,
	    self.cf["test_block"].AddCifItem,
	    (("_geom_bond_atom_site_label_1","_geom_bond_atom_site_label_2"),
	      [["C1","C2","H3","U4"],
	      ["C1","Cr2","H3","U4"]]))	
	# now we test that a missing parent is flagged
        # self.assertRaises(CifFile.ValidCifError,
	#     self.cf["test_block"].AddCifItem,
	#     (("_atom_site_type_symbol","_atom_site_label"),
	#       [["C","C","N"],["C1","C2","N1"]]))

    def testReport(self):
        CifFile.validate_report(CifFile.validate("tests/C13H2203_with_errors.cif",dic=ddl1dic))

class FakeDicTestCase(unittest.TestCase):
# we test stuff that hasn't been used in official dictionaries to date.
    def setUp(self):
        self.testcif = CifFile.CifFile("dictionaries/novel_test.cif")

    def testTypeConstruct(self):
        self.assertRaises(CifFile.ValidCifError,CifFile.ValidCifFile,
                           diclist=["dictionaries/novel.dic"],datasource=self.testcif)
          
class DicMergeTestCase(unittest.TestCase):
    def setUp(self):
        self.offdic = CifFile.CifFile("dictionaries/dict_official")
        self.adic = CifFile.CifFile("dictionaries/dict_A")
        self.bdic = CifFile.CifFile("dictionaries/dict_B")
        self.cdic = CifFile.CifFile("dictionaries/dict_C")
        self.cvdica = CifFile.CifFile("dictionaries/cvdica.dic")
        self.cvdicb = CifFile.CifFile("dictionaries/cvdicb.dic")
        self.cvdicc = CifFile.CifFile("dictionaries/cvdicc.dic")
        self.cvdicd = CifFile.CifFile("dictionaries/cvdicd.dic")
        self.testcif = CifFile.CifFile("dictionaries/merge_test.cif")
       
    def testAStrict(self):
        self.assertRaises(StarFile.StarError,CifFile.merge_dic,[self.offdic,self.adic],mergemode="strict")
        
    def testAOverlay(self):
        newdic = CifFile.merge_dic([self.offdic,self.adic],mergemode='overlay')
        # print newdic.__str__()
        self.assertRaises(CifFile.ValidCifError,CifFile.ValidCifFile,
                                  datasource="dictionaries/merge_test.cif",
                                  dic=newdic)
        
    def testAReverseO(self):
        # the reverse should be OK!
        newdic = CifFile.merge_dic([self.adic,self.offdic],mergemode='overlay')
        jj = CifFile.ValidCifFile(datasource="dictionaries/merge_test.cif",
                                  dic = newdic)

#    def testCOverlay(self):
#        self.offdic = CifFile.merge_dic([self.offdic,self.cdic],mergemode='replace') 
#        print "New dic..."
#        print self.offdic.__str__()
#        self.assertRaises(CifFile.ValidCifError,CifFile.ValidCifFile,
#                          datasource="dictionaries/merge_test.cif",
#                          dic = self.offdic)

    # now for the final example in "maintenance.html"
    def testCVOverlay(self):
        jj = open("merge_debug","w")
        newdic = CifFile.merge_dic([self.cvdica,self.cvdicb,self.cvdicc,self.cvdicd],mergemode='overlay')
        jj.write(newdic.__str__())

    def tearDown(self):
        pass

if __name__=='__main__':
    unittest.main()

