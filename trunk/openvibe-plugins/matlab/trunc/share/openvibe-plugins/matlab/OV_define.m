% OV_MATLAB toolkit
% OV_define.m
% -------------------------------
% Author : Laurent Bonnet (INRIA)
% Date   : 28/07/2011

% This file is meant to be executed prior to any user defined script.
% It declare all stim IDs as  variable for use  by the user.
% MEANT TO BE CALLED BY OPENVIBE



function OV_define()

% STREAM TYPES:
    global OV_TypeId_EBMLStream;
    OV_TypeId_EBMLStream = uint64(hex2dec('434F65872EFD2B7E'));
    global OV_TypeId_ChannelLocalisation;
    OV_TypeId_ChannelLocalisation = uint64(hex2dec('013DF452A3A8879A'));
    global OV_TypeId_ExperimentationInformation;
    OV_TypeId_ExperimentationInformation = uint64(hex2dec('403488E7565D70B6'));
    global OV_TypeId_Stimulations;
    OV_TypeId_Stimulations = uint64(hex2dec('6F752DD0082A321E'));
    global OV_TypeId_StreamedMatrix;
    OV_TypeId_StreamedMatrix = uint64(hex2dec('544A003E6DCBA5F6'));
    global OV_TypeId_FeatureVector;
    OV_TypeId_FeatureVector = uint64(hex2dec('17341935152FF448'));
    global OV_TypeId_Signal;
    OV_TypeId_Signal = uint64(hex2dec('5BA36127195FEAE1'));
    global OV_TypeId_Spectrum;
    OV_TypeId_Spectrum = uint64(hex2dec('1F261C0A593BF6BD'));

% SETTING TYPES:
	global OV_TypeId_Boolean;
    OV_TypeId_Boolean = uint64(hex2dec('2CDB2F0B12F231EA'));
	global OV_TypeId_Integer;
    OV_TypeId_Integer = uint64(hex2dec('007DEEF92F3E95C6'));
	global OV_TypeId_Float;
    OV_TypeId_Float = uint64(hex2dec('512A166F5C3EF83F'));
	global OV_TypeId_String;
    OV_TypeId_String = uint64(hex2dec('79A9EDEB245D83FC'));
	global OV_TypeId_Filename;
    OV_TypeId_Filename = uint64(hex2dec('330306DD74A95F98'));
	global OV_TypeId_Script;
    OV_TypeId_Script = uint64(hex2dec('B0D0DB4549CBC34A'));
	global OV_TypeId_Stimulation;
    OV_TypeId_Stimulation = uint64(hex2dec('2C132D6E44AB0D97'));
	global OV_TypeId_LogLevel;
    OV_TypeId_LogLevel = uint64(hex2dec('A88B36670871638C'));
	global OV_TypeId_Color;
    OV_TypeId_Color = uint64(hex2dec('7F45A2A97DB12219'));
	global OV_TypeId_ColorGradient;
    OV_TypeId_ColorGradient = uint64(hex2dec('3D3C7C7FEF0E7129'));
	
	
% STIM CODES:
    global OVTK_StimulationId_ExperimentStart;
	global OVTK_StimulationId_ExperimentStop;
	global OVTK_StimulationId_SegmentStart;
	global OVTK_StimulationId_SegmentStop;
	global OVTK_StimulationId_TrialStart;
	global OVTK_StimulationId_TrialStop;
	global OVTK_StimulationId_BaselineStart;
	global OVTK_StimulationId_BaselineStop;
	global OVTK_StimulationId_RestStart;
	global OVTK_StimulationId_RestStop;
	global OVTK_StimulationId_VisualStimulationStart;
	global OVTK_StimulationId_VisualStimulationStop;
	global OVTK_StimulationId_VisualSteadyStateStimulationStart;
	global OVTK_StimulationId_VisualSteadyStateStimulationStop;

	global OVTK_StimulationId_LabelStart;
	global OVTK_StimulationId_Label_00;
	global OVTK_StimulationId_Label_01;
	global OVTK_StimulationId_Label_02;
	global OVTK_StimulationId_Label_03;
	global OVTK_StimulationId_Label_04;
	global OVTK_StimulationId_Label_05;
	global OVTK_StimulationId_Label_06;
	global OVTK_StimulationId_Label_07;
	global OVTK_StimulationId_Label_08;
	global OVTK_StimulationId_Label_09;
	global OVTK_StimulationId_Label_0A;
	global OVTK_StimulationId_Label_0B;
	global OVTK_StimulationId_Label_0C;
	global OVTK_StimulationId_Label_0D;
	global OVTK_StimulationId_Label_0E;
	global OVTK_StimulationId_Label_0F;
	global OVTK_StimulationId_Label_10;
	global OVTK_StimulationId_Label_11;
	global OVTK_StimulationId_Label_12;
	global OVTK_StimulationId_Label_13;
	global OVTK_StimulationId_Label_14;
	global OVTK_StimulationId_Label_15;
	global OVTK_StimulationId_Label_16;
	global OVTK_StimulationId_Label_17;
	global OVTK_StimulationId_Label_18;
	global OVTK_StimulationId_Label_19;
	global OVTK_StimulationId_Label_1A;
	global OVTK_StimulationId_Label_1B;
	global OVTK_StimulationId_Label_1C;
	global OVTK_StimulationId_Label_1D;
	global OVTK_StimulationId_Label_1E;
	global OVTK_StimulationId_Label_1F;
	global OVTK_StimulationId_LabelEnd;

	global OVTK_StimulationId_Train;
	global OVTK_StimulationId_Beep;
	global OVTK_StimulationId_DoubleBeep;
	global OVTK_StimulationId_EndOfFile;
	global OVTK_StimulationId_Target;
	global OVTK_StimulationId_NonTarget;
	global OVTK_StimulationId_TrainCompleted;
	global OVTK_StimulationId_Reset;

	 % -- ___________________________________________________________________ --
	 % --                                                                     --
	 % --  GDF file format stimulation identifiers                            --
	 % -- ___________________________________________________________________ --
	 % --                                                                     --

	global OVTK_GDF_Artifact_EOG_Large;
	global OVTK_GDF_Artifact_ECG;
	global OVTK_GDF_Artifact_EMG;
	global OVTK_GDF_Artifact_Movement;
	global OVTK_GDF_Artifact_Failing_Electrode;
	global OVTK_GDF_Artifact_Sweat;
	global OVTK_GDF_Artifact_50_60_Hz_Interference;
	global OVTK_GDF_Artifact_Breathing;
	global OVTK_GDF_Artifact_Pulse;
	global OVTK_GDF_Artifact_EOG_Small;

	global OVTK_GDF_Calibration;

	global OVTK_GDF_EEG_Sleep_Splindles;
	global OVTK_GDF_EEG_K_Complexes;
	global OVTK_GDF_EEG_Saw_Tooth_Waves;
	global OVTK_GDF_EEG_Idling_EEG_Eyes_Open;
	global OVTK_GDF_EEG_Idling_EEG_Eyes_Closed;
	global OVTK_GDF_EEG_Spike;
	global OVTK_GDF_EEG_Seizure;

	global OVTK_GDF_VEP;
	global OVTK_GDF_AEP;
	global OVTK_GDF_SEP;
	global OVTK_GDF_TMS;

	global OVTK_GDF_SSVEP;
	global OVTK_GDF_SSAEP;
	global OVTK_GDF_SSSEP;

	global OVTK_GDF_Start_Of_Trial;
	global OVTK_GDF_Left;
	global OVTK_GDF_Right;
	global OVTK_GDF_Foot;
	global OVTK_GDF_Tongue;
	global OVTK_GDF_class5;
	global OVTK_GDF_Down;
	global OVTK_GDF_class7;
	global OVTK_GDF_class8;
	global OVTK_GDF_class9;
	global OVTK_GDF_class10;
	global OVTK_GDF_class11;
	global OVTK_GDF_Up;
	global OVTK_GDF_Feedback_Continuous;
	global OVTK_GDF_Feedback_Discrete;
	global OVTK_GDF_Cue_Unknown_Undefined;
	global OVTK_GDF_Beep;
	global OVTK_GDF_Cross_On_Screen;
	global OVTK_GDF_Flashing_Light;
	 % --  SPECIALY ADDED BY YR
	global OVTK_GDF_End_Of_Trial;

	global OVTK_GDF_Correct;
	global OVTK_GDF_Incorrect;
	 % --  SPECIALY ADDED BY YR
	global OVTK_GDF_End_Of_Session;
	global OVTK_GDF_Rejection;

	global OVTK_GDF_OAHE;
	global OVTK_GDF_RERA;
	global OVTK_GDF_CAHE;
	global OVTK_GDF_CSB;
	global OVTK_GDF_Sleep_Hypoventilation;
	global OVTK_GDF_Maximum_Inspiration;
	global OVTK_GDF_Start_Of_Inspiration;

	global OVTK_GDF_Wake;
	global OVTK_GDF_Stage_1;
	global OVTK_GDF_Stage_2;
	global OVTK_GDF_Stage_3;
	global OVTK_GDF_Stage_4;
	global OVTK_GDF_REM;

	global OVTK_GDF_Lights_On;
	global OVTK_GDF_Lights_Off;

	global OVTK_GDF_Eyes_Left;
	global OVTK_GDF_Eyes_Right;
	global OVTK_GDF_Eyes_Up;
	global OVTK_GDF_Eyes_Down;
	global OVTK_GDF_Horizontal_Eye_Movement;
	global OVTK_GDF_Vertical_Eye_Movement;
	global OVTK_GDF_Rotation_Clockwise;
	global OVTK_GDF_Rotation_Counterclockwise;
	global OVTK_GDF_Eye_Blink;

	global OVTK_GDF_Left_Hand_Movement;
	global OVTK_GDF_Right_Hand_Movement;
	global OVTK_GDF_Head_Movement;
	global OVTK_GDF_Tongue_Movement;
	global OVTK_GDF_Swallowing;
	global OVTK_GDF_Biting;
	global OVTK_GDF_Foot_Movement;
	global OVTK_GDF_Foot_Right_Movement;
	global OVTK_GDF_Arm_Movement;
	global OVTK_GDF_Arm_Right_Movement;

	global OVTK_GDF_ECG_Fiducial_Point_QRS_Complex;
	global OVTK_GDF_ECG_P_Wave;
	global OVTK_GDF_ECG_QRS_Complex;
	global OVTK_GDF_ECG_R_Point;
	global OVTK_GDF_ECG_T_Wave;
	global OVTK_GDF_ECG_U_Wave;

	global OVTK_GDF_Start;
	global OVTK_GDF_25_Watt;
	global OVTK_GDF_50_Watt;
	global OVTK_GDF_75_Watt;
	global OVTK_GDF_100_Watt;
	global OVTK_GDF_125_Watt;
	global OVTK_GDF_150_Watt;
	global OVTK_GDF_175_Watt;
	global OVTK_GDF_200_Watt;
	global OVTK_GDF_225_Watt;
	global OVTK_GDF_250_Watt;
	global OVTK_GDF_275_Watt;
	global OVTK_GDF_300_Watt;
	global OVTK_GDF_325_Watt;
	global OVTK_GDF_350_Watt;

	global OVTK_GDF_Start_Of_New_Segment;
	global OVTK_GDF_Non_Equidistant_Sampling_Value;
	
	 % -- ___________________________________________________________________ --
	 % --                                                                     --
	 % --  OpenViBE toolkit stimulation identifiers                           --
	 % -- ___________________________________________________________________ --
	 % --                                                                     --

	 OVTK_StimulationId_ExperimentStart                    = uint64(hex2dec('00008001'));
	 OVTK_StimulationId_ExperimentStop                     = uint64(hex2dec('00008002'));
	 OVTK_StimulationId_SegmentStart                       = uint64(hex2dec('00008003'));
	 OVTK_StimulationId_SegmentStop                        = uint64(hex2dec('00008004'));
	 OVTK_StimulationId_TrialStart                         = uint64(hex2dec('00008005'));
	 OVTK_StimulationId_TrialStop                          = uint64(hex2dec('00008006'));
	 OVTK_StimulationId_BaselineStart                      = uint64(hex2dec('00008007'));
	 OVTK_StimulationId_BaselineStop                       = uint64(hex2dec('00008008'));
	 OVTK_StimulationId_RestStart                          = uint64(hex2dec('00008009'));
	 OVTK_StimulationId_RestStop                           = uint64(hex2dec('0000800a'));
	 OVTK_StimulationId_VisualStimulationStart             = uint64(hex2dec('0000800b'));
	 OVTK_StimulationId_VisualStimulationStop              = uint64(hex2dec('0000800c'));
	 OVTK_StimulationId_VisualSteadyStateStimulationStart  = uint64(hex2dec('00008010'));
	 OVTK_StimulationId_VisualSteadyStateStimulationStop   = uint64(hex2dec('00008011'));

	 OVTK_StimulationId_LabelStart                         = uint64(hex2dec('00008100'));
	 OVTK_StimulationId_Label_00                           = uint64(hex2dec('00008100'));
	 OVTK_StimulationId_Label_01                           = uint64(hex2dec('00008101'));
	 OVTK_StimulationId_Label_02                           = uint64(hex2dec('00008102'));
	 OVTK_StimulationId_Label_03                           = uint64(hex2dec('00008103'));
	 OVTK_StimulationId_Label_04                           = uint64(hex2dec('00008104'));
	 OVTK_StimulationId_Label_05                           = uint64(hex2dec('00008105'));
	 OVTK_StimulationId_Label_06                           = uint64(hex2dec('00008106'));
	 OVTK_StimulationId_Label_07                           = uint64(hex2dec('00008107'));
	 OVTK_StimulationId_Label_08                           = uint64(hex2dec('00008108'));
	 OVTK_StimulationId_Label_09                           = uint64(hex2dec('00008109'));
	 OVTK_StimulationId_Label_0A                           = uint64(hex2dec('0000810a'));
	 OVTK_StimulationId_Label_0B                           = uint64(hex2dec('0000810b'));
	 OVTK_StimulationId_Label_0C                           = uint64(hex2dec('0000810c'));
	 OVTK_StimulationId_Label_0D                           = uint64(hex2dec('0000810d'));
	 OVTK_StimulationId_Label_0E                           = uint64(hex2dec('0000810e'));
	 OVTK_StimulationId_Label_0F                           = uint64(hex2dec('0000810f'));
	 OVTK_StimulationId_Label_10                           = uint64(hex2dec('00008110'));
	 OVTK_StimulationId_Label_11                           = uint64(hex2dec('00008111'));
	 OVTK_StimulationId_Label_12                           = uint64(hex2dec('00008112'));
	 OVTK_StimulationId_Label_13                           = uint64(hex2dec('00008113'));
	 OVTK_StimulationId_Label_14                           = uint64(hex2dec('00008114'));
	 OVTK_StimulationId_Label_15                           = uint64(hex2dec('00008115'));
	 OVTK_StimulationId_Label_16                           = uint64(hex2dec('00008116'));
	 OVTK_StimulationId_Label_17                           = uint64(hex2dec('00008117'));
	 OVTK_StimulationId_Label_18                           = uint64(hex2dec('00008118'));
	 OVTK_StimulationId_Label_19                           = uint64(hex2dec('00008119'));
	 OVTK_StimulationId_Label_1A                           = uint64(hex2dec('0000811a'));
	 OVTK_StimulationId_Label_1B                           = uint64(hex2dec('0000811b'));
	 OVTK_StimulationId_Label_1C                           = uint64(hex2dec('0000811c'));
	 OVTK_StimulationId_Label_1D                           = uint64(hex2dec('0000811d'));
	 OVTK_StimulationId_Label_1E                           = uint64(hex2dec('0000811e'));
	 OVTK_StimulationId_Label_1F                           = uint64(hex2dec('0000811f'));
	 OVTK_StimulationId_LabelEnd                           = uint64(hex2dec('000081ff'));

	 OVTK_StimulationId_Train                              = uint64(hex2dec('00008201'));
	 OVTK_StimulationId_Beep                               = uint64(hex2dec('00008202'));
	 OVTK_StimulationId_DoubleBeep                         = uint64(hex2dec('00008203'));
	 OVTK_StimulationId_EndOfFile                          = uint64(hex2dec('00008204'));
	 OVTK_StimulationId_Target                             = uint64(hex2dec('00008205'));
	 OVTK_StimulationId_NonTarget                          = uint64(hex2dec('00008206'));
	 OVTK_StimulationId_TrainCompleted                     = uint64(hex2dec('00008207'));
	 OVTK_StimulationId_Reset                              = uint64(hex2dec('00008208'));

	 % -- ___________________________________________________________________ --
	 % --                                                                     --
	 % --  GDF file format stimulation identifiers                            --
	 % -- ___________________________________________________________________ --
	 % --                                                                     --

	 OVTK_GDF_Artifact_EOG_Large                                = uint64(hex2dec('101'));
	 OVTK_GDF_Artifact_ECG                                      = uint64(hex2dec('102'));
	 OVTK_GDF_Artifact_EMG                                      = uint64(hex2dec('103'));
	 OVTK_GDF_Artifact_Movement                                 = uint64(hex2dec('104'));
	 OVTK_GDF_Artifact_Failing_Electrode                        = uint64(hex2dec('105'));
	 OVTK_GDF_Artifact_Sweat                                    = uint64(hex2dec('106'));
	 OVTK_GDF_Artifact_50_60_Hz_Interference                    = uint64(hex2dec('107'));
	 OVTK_GDF_Artifact_Breathing                                = uint64(hex2dec('108'));
	 OVTK_GDF_Artifact_Pulse                                    = uint64(hex2dec('109'));
	 OVTK_GDF_Artifact_EOG_Small                                = uint64(hex2dec('10A'));

	 OVTK_GDF_Calibration                                       = uint64(hex2dec('10F'));

	 OVTK_GDF_EEG_Sleep_Splindles                               = uint64(hex2dec('111'));
	 OVTK_GDF_EEG_K_Complexes                                   = uint64(hex2dec('112'));
	 OVTK_GDF_EEG_Saw_Tooth_Waves                               = uint64(hex2dec('113'));
	 OVTK_GDF_EEG_Idling_EEG_Eyes_Open                          = uint64(hex2dec('114'));
	 OVTK_GDF_EEG_Idling_EEG_Eyes_Closed                        = uint64(hex2dec('115'));
	 OVTK_GDF_EEG_Spike                                         = uint64(hex2dec('116'));
	 OVTK_GDF_EEG_Seizure                                       = uint64(hex2dec('117'));

	 OVTK_GDF_VEP                                               = uint64(hex2dec('121'));
	 OVTK_GDF_AEP                                               = uint64(hex2dec('122'));
	 OVTK_GDF_SEP                                               = uint64(hex2dec('123'));
	 OVTK_GDF_TMS                                               = uint64(hex2dec('12F'));

	 OVTK_GDF_SSVEP                                             = uint64(hex2dec('131'));
	 OVTK_GDF_SSAEP                                             = uint64(hex2dec('132'));
	 OVTK_GDF_SSSEP                                             = uint64(hex2dec('133'));

	 OVTK_GDF_Start_Of_Trial                                    = uint64(hex2dec('300'));
	 OVTK_GDF_Left                                              = uint64(hex2dec('301'));
	 OVTK_GDF_Right                                             = uint64(hex2dec('302'));
	 OVTK_GDF_Foot                                              = uint64(hex2dec('303'));
	 OVTK_GDF_Tongue                                            = uint64(hex2dec('304'));
	 OVTK_GDF_class5                                            = uint64(hex2dec('305'));
	 OVTK_GDF_Down                                              = uint64(hex2dec('306'));
	 OVTK_GDF_class7                                            = uint64(hex2dec('307'));
	 OVTK_GDF_class8                                            = uint64(hex2dec('308'));
	 OVTK_GDF_class9                                            = uint64(hex2dec('309'));
	 OVTK_GDF_class10                                           = uint64(hex2dec('30A'));
	 OVTK_GDF_class11                                           = uint64(hex2dec('30B'));
	 OVTK_GDF_Up                                                = uint64(hex2dec('30C'));
	 OVTK_GDF_Feedback_Continuous                               = uint64(hex2dec('30D'));
	 OVTK_GDF_Feedback_Discrete                                 = uint64(hex2dec('30E'));
	 OVTK_GDF_Cue_Unknown_Undefined                             = uint64(hex2dec('30F'));
	 OVTK_GDF_Beep                                              = uint64(hex2dec('311'));
	 OVTK_GDF_Cross_On_Screen                                   = uint64(hex2dec('312'));
	 OVTK_GDF_Flashing_Light                                    = uint64(hex2dec('313'));
	 % --  SPECIALY ADDED BY YR
	 OVTK_GDF_End_Of_Trial                                      = uint64(hex2dec('320'));

	 OVTK_GDF_Correct                                           = uint64(hex2dec('381'));
	 OVTK_GDF_Incorrect                                         = uint64(hex2dec('382'));
	 % --  SPECIALY ADDED BY YR
	 OVTK_GDF_End_Of_Session                                    = uint64(hex2dec('3F2'));
	 OVTK_GDF_Rejection                                         = uint64(hex2dec('3FF'));

	 OVTK_GDF_OAHE                                              = uint64(hex2dec('401'));
	 OVTK_GDF_RERA                                              = uint64(hex2dec('402'));
	 OVTK_GDF_CAHE                                              = uint64(hex2dec('403'));
	 OVTK_GDF_CSB                                               = uint64(hex2dec('404'));
	 OVTK_GDF_Sleep_Hypoventilation                             = uint64(hex2dec('405'));
	 OVTK_GDF_Maximum_Inspiration                               = uint64(hex2dec('40E'));
	 OVTK_GDF_Start_Of_Inspiration                              = uint64(hex2dec('40F'));

	 OVTK_GDF_Wake                                              = uint64(hex2dec('410'));
	 OVTK_GDF_Stage_1                                           = uint64(hex2dec('411'));
	 OVTK_GDF_Stage_2                                           = uint64(hex2dec('412'));
	 OVTK_GDF_Stage_3                                           = uint64(hex2dec('413'));
	 OVTK_GDF_Stage_4                                           = uint64(hex2dec('414'));
	 OVTK_GDF_REM                                               = uint64(hex2dec('415'));

	 OVTK_GDF_Lights_On                                         = uint64(hex2dec('420'));
	 OVTK_GDF_Lights_Off                                        = uint64(hex2dec('8420'));

	 OVTK_GDF_Eyes_Left                                         = uint64(hex2dec('431'));
	 OVTK_GDF_Eyes_Right                                        = uint64(hex2dec('432'));
	 OVTK_GDF_Eyes_Up                                           = uint64(hex2dec('433'));
	 OVTK_GDF_Eyes_Down                                         = uint64(hex2dec('434'));
	 OVTK_GDF_Horizontal_Eye_Movement                           = uint64(hex2dec('435'));
	 OVTK_GDF_Vertical_Eye_Movement                             = uint64(hex2dec('436'));
	 OVTK_GDF_Rotation_Clockwise                                = uint64(hex2dec('437'));
	 OVTK_GDF_Rotation_Counterclockwise                         = uint64(hex2dec('438'));
	 OVTK_GDF_Eye_Blink                                         = uint64(hex2dec('439'));

	 OVTK_GDF_Left_Hand_Movement                                = uint64(hex2dec('441'));
	 OVTK_GDF_Right_Hand_Movement                               = uint64(hex2dec('442'));
	 OVTK_GDF_Head_Movement                                     = uint64(hex2dec('443'));
	 OVTK_GDF_Tongue_Movement                                   = uint64(hex2dec('444'));
	 OVTK_GDF_Swallowing                                        = uint64(hex2dec('445'));
	 OVTK_GDF_Biting                                            = uint64(hex2dec('446'));
	 OVTK_GDF_Foot_Movement                                     = uint64(hex2dec('447'));
	 OVTK_GDF_Foot_Right_Movement                               = uint64(hex2dec('448'));
	 OVTK_GDF_Arm_Movement                                      = uint64(hex2dec('449'));
	 OVTK_GDF_Arm_Right_Movement                                = uint64(hex2dec('44A'));

	 OVTK_GDF_ECG_Fiducial_Point_QRS_Complex                    = uint64(hex2dec('501'));
	 OVTK_GDF_ECG_P_Wave                                        = uint64(hex2dec('502'));
	 OVTK_GDF_ECG_QRS_Complex                                   = uint64(hex2dec('503'));
	 OVTK_GDF_ECG_R_Point                                       = uint64(hex2dec('504'));
	 OVTK_GDF_ECG_T_Wave                                        = uint64(hex2dec('506'));
	 OVTK_GDF_ECG_U_Wave                                        = uint64(hex2dec('507'));

	 OVTK_GDF_Start                                             = uint64(hex2dec('580'));
	 OVTK_GDF_25_Watt                                           = uint64(hex2dec('581'));
	 OVTK_GDF_50_Watt                                           = uint64(hex2dec('582'));
	 OVTK_GDF_75_Watt                                           = uint64(hex2dec('583'));
	 OVTK_GDF_100_Watt                                          = uint64(hex2dec('584'));
	 OVTK_GDF_125_Watt                                          = uint64(hex2dec('585'));
	 OVTK_GDF_150_Watt                                          = uint64(hex2dec('586'));
	 OVTK_GDF_175_Watt                                          = uint64(hex2dec('587'));
	 OVTK_GDF_200_Watt                                          = uint64(hex2dec('588'));
	 OVTK_GDF_225_Watt                                          = uint64(hex2dec('589'));
	 OVTK_GDF_250_Watt                                          = uint64(hex2dec('58A'));
	 OVTK_GDF_275_Watt                                          = uint64(hex2dec('58B'));
	 OVTK_GDF_300_Watt                                          = uint64(hex2dec('58C'));
	 OVTK_GDF_325_Watt                                          = uint64(hex2dec('58D'));
	 OVTK_GDF_350_Watt                                          = uint64(hex2dec('58E'));

	 OVTK_GDF_Start_Of_New_Segment                              = uint64(hex2dec('7FFE'));
	 OVTK_GDF_Non_Equidistant_Sampling_Value                    = uint64(hex2dec('7FFF'));
 
end
