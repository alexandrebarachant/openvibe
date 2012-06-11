from math import *
import random, time

class MyOVBox(OVBox):
	def __init__(self):
		OVBox.__init__(self)
		self.channel_count = 0
		self.sampling_frequency = 0
		self.epoch_sample_count = 0
		self.start_time = 0
		self.end_time = 0
		self.dimension_size = list()
		self.dimension_label = list()
		self.time_buffer = list()
		self.signal_buffer = list()
		self.signal_header = None
		
	def initialize(self):
		random.seed(time.time())
		self.channel_count = int(self.setting['Channel count'])
		self.sampling_frequency = int(self.setting['Sampling frequency'])
		self.epoch_sample_count = int(self.setting['Generated epoch sample count'])
		
		#creation of the signal header
		for i in range(self.channel_count):
			self.dimension_label.append( 'Sinus'+str(i) )
		self.dimension_label += self.epoch_sample_count*['']
		self.dimension_size = [self.channel_count, self.epoch_sample_count]
		self.signal_header = OVSignalHeader(0., 0., self.dimension_size, self.dimension_label, self.sampling_frequency)
		self.output[0].append(self.signal_header)
		
		#creation of the first signal chunk
		self.end_time = self.epoch_sample_count
		self.signal_buffer = [0.]*self.channel_count*self.epoch_sample_count
		self.update_time_buffer()
		self.update_signal_buffer()
		
		
	def update_start_time(self):
		self.start_time += self.epoch_sample_count
		
	def update_end_time(self):
		self.end_time += self.epoch_sample_count
		#self.end_time = self.start_time + self.epoch_sample_count
	
	def update_time_buffer(self):
		self.time_buffer = range(self.start_time, self.end_time)
		for instant_index, instant in enumerate(self.time_buffer):
			self.time_buffer[instant_index] = float(instant)/self.sampling_frequency
		
		
	def update_signal_buffer(self):
		for channel in range(self.channel_count):
			for instant_index, instant in enumerate(self.time_buffer):
				new_sample = 100.*sin(2.*pi*float(channel+1)*instant) + (1000.*random.random()-500.)
				self.signal_buffer[channel*self.epoch_sample_count+instant_index] = new_sample
			
	def send_signal_buffer_to_openvibe(self):
		start = self.time_buffer[0]
		end = self.time_buffer[-1] + 1.0/self.sampling_frequency
		self.output[0].append( OVSignalBuffer(start, end, self.signal_buffer) )
	
	def process(self):
		start = self.time_buffer[0]
		end = self.time_buffer[-1]
		if self.get_current_time() >= end:
			self.send_signal_buffer_to_openvibe()
			self.update_start_time()
			self.update_end_time()
			self.update_time_buffer()
			self.update_signal_buffer()

	def uninitialize(self):
		end = self.time_buffer[-1]
		self.output[0].append(OVSignalEnd(end, end))

box = MyOVBox()
