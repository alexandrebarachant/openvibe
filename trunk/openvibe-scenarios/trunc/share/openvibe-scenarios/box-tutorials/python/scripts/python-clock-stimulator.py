class MyOVBox(OVBox):
    # the constructor creates the box and initializes object variables
	def __init__(self):
		OVBox.__init__(self)
		self.stim_label = None
		self.stim_code = None
		
	# the initialize method reads settings and outputs the first header
	def initialize(self):
		# all settings
		self.stim_label = self.setting['Stimulation']
		self.stim_code = OpenViBE_stimulation[self.stim_label]
		self.output[0].append(OVStimulationHeader(0., 0.))
	
	def process(self):
		stim_set = OVStimulationSet(self.get_current_time(), self.get_current_time()+self._clock)
		stim_set.append(OVStimulation(self.stim_code, self.get_current_time(), 0.))
		self.output[0].append(stim_set)

	def uninitialize(self):
		end = self.get_current_time()
		self.output[0].append(OVStimulationEnd(end, end))				

if __name__ == '__main__':
	box = MyOVBox()
