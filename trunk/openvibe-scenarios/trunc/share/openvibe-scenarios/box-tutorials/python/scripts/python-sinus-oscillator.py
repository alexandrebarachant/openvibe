import numpy

class MyOVBox(OVBox):
	def __init__(self):
		OVBox.__init__(self)
		self.channel_count = 0
		self.sampling_frequency = 0
		self.epoch_sample_count = 0
		self.start_time = 0.
		self.end_time = 0.
		self.dimension_size = list()
		self.dimension_label = list()
		self.time_buffer = list()
		self.signal_buffer = None
		self.signal_header = None
		
	def initialize(self):
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
		self.end_time = 1.*self.epoch_sample_count/self.sampling_frequency
		self.signal_buffer = numpy.zeros((self.channel_count, self.epoch_sample_count))
		self.update_time_buffer()
		self.update_signal_buffer()
		#self.send_signal_buffer_to_openvibe()
		
	def update_start_time(self):
		self.start_time += 1.*self.epoch_sample_count/self.sampling_frequency
		
	def update_end_time(self):
		self.end_time = float(self.start_time + 1.*self.epoch_sample_count/self.sampling_frequency)
	
	def update_time_buffer(self):
		self.time_buffer = numpy.arange(self.start_time, self.end_time, 1./self.sampling_frequency)
		
	def update_signal_buffer(self):
		for row_index, row in enumerate(self.signal_buffer):
			self.signal_buffer[row_index,:] = 100.*numpy.sin( 2.*numpy.pi*(row_index+1.)*self.time_buffer )
			
	def send_signal_buffer_to_openvibe(self):
		start = self.time_buffer[0]
		end = self.time_buffer[-1] + 1./self.sampling_frequency
		buffer_elements = self.signal_buffer.reshape(self.channel_count*self.epoch_sample_count).tolist()
		self.output[0].append( OVSignalBuffer(start, end, buffer_elements) )
	
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
