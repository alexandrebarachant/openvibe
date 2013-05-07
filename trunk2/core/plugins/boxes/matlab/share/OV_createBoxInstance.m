% OV_MATLAB toolkit
% OV_createBoxInstance.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011
%
% This file is meant to be executed prior to any user defined script.
% It construct the objects used by the other functions of the toolkit
% MEANT TO BE USED BY OPENVIBE
%
%out 


function box = OV_createBoxInstance(input_count, output_count)
    
	box = [];
    box.settings = [];
    box.inputs = {};
    for i = 1 : input_count
		box.inputs{i}.header = {};
		box.inputs{i}.buffer = {};
    end
    
    box.outputs = {};
	for i = 1 : output_count
		box.outputs{i}.header = {};
		box.outputs{i}.buffer = {};
    end
    box.user_data = {};
    
    box.clock = 0;
    
end

	