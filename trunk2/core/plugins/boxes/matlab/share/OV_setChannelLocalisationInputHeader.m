% OV_MATLAB toolkit
% OV_setChannelLocalizationInputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a Channel Localization input.

function box_out = OV_setChannelLocalisationInputHeader(box_in, input_index, nb_channels, channel_names, dynamic)
    
    box_in.inputs{input_index}.header.type = 'Channel Localisation Stream';
    box_in.inputs{input_index}.header.nb_channels = nb_channels;
    box_in.inputs{input_index}.header.channel_names = channel_names;
    box_in.inputs{input_index}.header.dynamic = dynamic;
    
    box_in.inputs{input_index}.buffer = {};
    
    box_out = box_in;
end