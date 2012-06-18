% OV_MATLAB toolkit
% OV_setSettings.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% This file is meant to be executed prior to any user defined script.
% It construct the objects used by the other functions of the toolkit
% MEANT TO BE USED BY OPENVIBE

function box_out = OV_setSettings(box_in, names, types, values)
	
    box_in.settings = struct('name',names,'type',types,'value',values);
	box_out = box_in;
end