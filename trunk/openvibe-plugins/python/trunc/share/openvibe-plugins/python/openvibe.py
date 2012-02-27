# -*- coding: utf-8 -*-
#File Name : openvibe.py
#Created By : Aurélien Van Langhenhove

from StimulationsCodes import *
import traceback, collections
from StringIO import StringIO

class NewStd(StringIO):
    def __init__(self):
        StringIO.__init__(self)
    def flush(self):
        self.buf = str()
    def isempty(self):
        if len(self.buf) == 0:
            return True
        else:
            return False

sys.stdout = NewStd()
sys.stderr = NewStd()


def execfile_handling_exception(filename, maindictionary):
    print "executing script file"

    try:
        maindictionary['box'] = OVBox(default=True) # on en cree une au cas ou
        #print globals()
        execfile(filename, maindictionary) 
        return 0;
    except:
        print "error"
        print traceback.format_exc()
        return -1;

def decorator_function(target):
    """ add a try except block to protect openvibe box in case of exception """
    def wrapper(self):
        try :
            print "using decorator"
            print 'Calling function "%s"' % target.__name__
            return target(self)
        except:
            print traceback.format_exc()

    return wrapper

class OVChunk(object):
    def __init__(self, start_time, end_time):
        self.start_time = start_time
        self.end_time = end_time


class OVStreamedMatrixHeader(OVChunk):
    def __init__(self, start_time, end_time, dimension_size, dimension_label):
        OVChunk.__init__(self, start_time, end_time)
        self.dimension_size = list(dimension_size)
        self.dimension_label = list(dimension_label)
        
    def dimension_count(self):
        return len(self.dimension_size)
        
    def buffer_element_count(self):
        element_count = 0
        for dimension, size in enumerate(self.dimension_size):
            if dimension == 0:
                element_count = int(size)
            else:
                element_count *= int(size)
        return element_count

class OVStreamedMatrixBuffer(OVChunk, list):
    def __init__(self, start_time, end_time, buffer_elements):
        OVChunk.__init__(self, start_time, end_time)
        list.__init__(self, buffer_elements)

class OVStreamedMatrixEnd(OVChunk):
    pass 


    
class OVSignalHeader(OVStreamedMatrixHeader):
    def __init__(self, start_time, end_time, dimension_size, dimension_label, sampling_rate):
        OVStreamedMatrixHeader.__init__(self, start_time, end_time, dimension_size, dimension_label)
        self.sampling_rate = sampling_rate
        
class OVSignalBuffer(OVStreamedMatrixBuffer):
    pass 
    
class OVSignalEnd(OVChunk):
    pass 
    


class OVStimulation(object):
    def __init__(self, identifier, date, duration):
        self.identifier = identifier
        self.date = date
        self.duration = duration

class OVStimulationHeader(OVChunk):
    pass

class OVStimulationSet(OVChunk, list):
    def __init__(self, start_time, end_time):
        list.__init__(self)
        OVChunk.__init__(self, start_time, end_time)
        
    def append(self, item):
        if isinstance(item, OVStimulation):
            list.append(self, item)
        else:
            raise TypeError("The item must be an OVStimulation")

class OVStimulationEnd(OVChunk):
    pass 
    


class OVBuffer(object):
    def __init__(self, input_type):
        self.__deque = collections.deque()
        self.__type = input_type
    def __len__(self):
        return len(self.__deque)
    def __getitem__(self, key):
        return self.__deque[key]
    def __setitem__(self, key, item):
        self.__deque[key] = item
    def __delitem__(self, key):
        del self.__deque[key]
    def append(self, to_append):
        self.__deque.appendleft(to_append)
    def pop(self):
        return self.__deque.pop()
    def type(self):
        return self.__type


class OVBox(object):
	def __init__(self, default=False):
		self.input = list()
		self.output = list()
		self.setting = dict()
		self.var = dict()
		self._clock = 0
		self._current_time = 0.
		self.default= default
	def addInput(self, input_type):
		self.input.append(OVBuffer(input_type))
	def addOutput(self, output_type):
		self.output.append(OVBuffer(output_type))
	def get_clock(self):
		return self._clock
	def get_current_time(self):
		return self._current_time
	def initialize(self):
		if self.default == True:
			print "box has not been created by user script, using default one from openvibe.py"
		pass
	def process(self):
		pass
	def uninitialize(self):
		pass
	def real_initialize(self):
		try :
			return self.initialize()
		except:
			print traceback.format_exc()
	def real_process(self):
		try :
			return self.process()
		except:
			print traceback.format_exc()
	def real_uninitialize(self):
		try :
			return self.uninitialize()
		except:
			print traceback.format_exc()
