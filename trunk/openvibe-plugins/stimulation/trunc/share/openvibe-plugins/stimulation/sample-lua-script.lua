#!/bin/lua

-- Stimulation codes can be found at this url :
-- http://openvibe.inria.fr/documentation/unstable/Doc_Stimulations.html

OVTK_StimulationId_ExperimentStart                   = 0x00008001
OVTK_StimulationId_ExperimentStop                    = 0x00008002
OVTK_StimulationId_SegmentStart                      = 0x00008003
OVTK_StimulationId_SegmentStop                       = 0x00008004
OVTK_StimulationId_TrialStart                        = 0x00008005
OVTK_StimulationId_TrialStop                         = 0x00008006
OVTK_StimulationId_BaselineStart                     = 0x00008007
OVTK_StimulationId_BaselineStop                      = 0x00008008
OVTK_StimulationId_RestStart                         = 0x00008009
OVTK_StimulationId_RestStop                          = 0x0000800a
OVTK_StimulationId_VisualStimulationStart            = 0x0000800b
OVTK_StimulationId_VisualStimulationStop             = 0x0000800c

OVTK_StimulationId_LabelStart                        = 0x00008100
OVTK_StimulationId_Label_00                          = 0x00008100
OVTK_StimulationId_Label_01                          = 0x00008101
OVTK_StimulationId_Label_02                          = 0x00008102
OVTK_StimulationId_Label_03                          = 0x00008103
OVTK_StimulationId_Label_04                          = 0x00008104
OVTK_StimulationId_Label_05                          = 0x00008105
OVTK_StimulationId_Label_06                          = 0x00008106
OVTK_StimulationId_Label_07                          = 0x00008107
OVTK_StimulationId_Label_08                          = 0x00008108
OVTK_StimulationId_Label_09                          = 0x00008109
OVTK_StimulationId_Label_0A                          = 0x0000810a
OVTK_StimulationId_Label_0B                          = 0x0000810b
OVTK_StimulationId_Label_0C                          = 0x0000810c
OVTK_StimulationId_Label_0D                          = 0x0000810d
OVTK_StimulationId_Label_0E                          = 0x0000810e
OVTK_StimulationId_Label_0F                          = 0x0000810f
OVTK_StimulationId_LabelEnd                          = 0x000081ff

OVTK_StimulationId_Train                             = 0x00008201
OVTK_StimulationId_Beep                              = 0x00008202
OVTK_StimulationId_DoubleBeep                        = 0x00008203
OVTK_StimulationId_EndOfFile                         = 0x00008204
OVTK_StimulationId_Target                            = 0x00008205
OVTK_StimulationId_NonTarget                         = 0x00008206

-- this function is called when the box is initialized
function initialize()
	io.write("initialize has been called\n");
end

-- this function is called when the box is uninitialized
function uninitialize()
	io.write("uninitialize had been called\n");
end

-- this function is called once by the box
function process()
	io.write("process has been called\n");

	-- enters infinite loop
	-- cpu will be released with a call to sleep
	-- at the end of the loop
	while true do

		-- gets current simulated time
		t = get_current_time()

		-- loops on all inputs of the box
		for input = 1, 2 do

			-- loops on every received stimulation for a given input
			for stimulation = 1, get_stimulation_count(input) do

				-- logs the received stimulation
				io.write(string.format("At time %f on input %i got stimulation id:%s date:%s duration:%s\n", t, input, get_stimulation(input, 1)))

				-- discards it
				remove_stimulation(input, 1)

				-- triggers a new OVTK_StimulationId_Label_00 stimulation five seconds after
				send_stimulation(1, OVTK_StimulationId_Label_00, t+5, 0)
			end
		end

		-- releases cpu
		sleep()
	end
end
