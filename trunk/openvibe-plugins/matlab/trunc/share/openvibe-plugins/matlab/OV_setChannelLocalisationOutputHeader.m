% OV_MATLAB toolkit
% OV_setChannelLocalizationInputHeader.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% Set the header information on a Channel Localization input.

function box_out = OV_setChannelLocalisationOutputHeader(box_in, output_index, nb_channels, channel_names, dynamic)
    
    box_in.output{output_index}.header.type = 'Channel Localisation Stream';
    box_in.output{output_index}.header.nb_channels = nb_channels;
    box_in.output{output_index}.header.channel_names = channel_names;
    box_in.output{output_index}.header.dynamic = dynamic;
    
    box_in.output{output_index}.buffer = {};
    
    box_out = box_in;
end