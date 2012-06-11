import numpy

class MyOVBox(OVBox):
	def __init__(self):
		OVBox.__init__(self)
		self.signal_header = None
	
	def process(self):
		for chunk_index in range( len(self.input[0]) ):
			if(type(self.input[0][chunk_index]) == OVSignalHeader):
				self.signal_header = self.input[0].pop()
				
				output_header = OVSignalHeader(
				self.signal_header.start_time, 
				self.signal_header.end_time, 
				[1, self.signal_header.dimension_size[1]], 
				['Mean']+self.signal_header.dimension_size[1]*[''],
				self.signal_header.sampling_rate)
				
				self.output[0].append(output_header)
				
			elif(type(self.input[0][chunk_index]) == OVSignalBuffer):
				chunk = self.input[0].pop()
				numpy_buffer = numpy.array(chunk).reshape(tuple(self.signal_header.dimension_size))
				numpy_buffer = numpy_buffer.mean(axis=0)
				chunk = OVSignalBuffer(chunk.start_time, chunk.end_time, numpy_buffer.tolist())
				self.output[0].append(chunk)
				
			elif(type(self.input[0][chunk_index]) == OVSignalEnd):
				self.output[0].append(self.input[0].pop())	 			

box = MyOVBox()
