% openvibe toolbox for Matlab
% OV_getSignalOutputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a signal output.
% MEANT TO BE USED IN USER SCRIPT

function [errno, nb_channels, channel_names, dynamic] = OV_getChannelLocalisationOutputHeader(box_in, output_index)
    
    if(numel(box_in.outputs{output_index}.header) == 0)
        nb_channels = 0;
        channel_names = 0;
        dynamic = 0;
        errno = 1;
    else
        nb_channels = box_in.outputs{output_index}.header.nb_channels;
        channel_names = box_in.outputs{output_index}.header.channel_names;
        dynamic = box_in.outputs{output_index}.header.dynamic;
        errno = 0;
    end
    
end