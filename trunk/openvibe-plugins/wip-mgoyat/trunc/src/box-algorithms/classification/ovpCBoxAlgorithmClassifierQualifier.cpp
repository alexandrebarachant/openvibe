#include "ovpCBoxAlgorithmClassifierQualifier.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>

namespace
{
	char* itoa(int i, char* buffer, unsigned int len)
	{
		::sprintf(buffer, "%i", i);
		return buffer;
	}
};

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Classification;
using namespace std;

boolean CBoxAlgorithmClassifierQualifier::initialize(void)
{
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	/*CString l_sConfigurationFilename;
	l_rStaticBoxContext.getSettingValue(1, l_sConfigurationFilename);

	for(uint32 i=2; i<l_rStaticBoxContext.getSettingCount(); i++)
	{
		CString l_sStimulationName;
		l_rStaticBoxContext.getSettingValue(i, l_sStimulationName);
		m_vStimulation[i-2]=this->getTypeManager().getEnumerationEntryValueFromName(OV_TypeId_Stimulation, l_sStimulationName);
	}*/

	m_pRowDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));
	m_pColumnDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));;
	m_pTargetDecoder=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(OVP_GD_ClassId_Algorithm_StimulationStreamDecoder));

	m_pRowDecoder->initialize();
	m_pColumnDecoder->initialize();
	m_pTargetDecoder->initialize();

	ip_pRowDecoderMemoryBuffer.initialize(m_pRowDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	ip_pColumnDecoderMemoryBuffer.initialize(m_pColumnDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	ip_pTargetDecoderMemoryBuffer.initialize(m_pTargetDecoder->getInputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_InputParameterId_MemoryBufferToDecode));
	//op_pStimulationDecoded.initialize();

	m_ui64LastTargetRow=0;
	m_ui64LastTargetColumn=0;

	m_ui64VraiPositif=0;
	m_ui64FauxPositif=0;
	m_ui64VraiNegatif=0;
	m_ui64FauxNegatif=0;

	return true;
}

boolean CBoxAlgorithmClassifierQualifier::uninitialize(void)
{
	saveClassifierQualification();

	ip_pRowDecoderMemoryBuffer.uninitialize();
	ip_pColumnDecoderMemoryBuffer.uninitialize();
	ip_pTargetDecoderMemoryBuffer.uninitialize();

	m_pRowDecoder->uninitialize();
	m_pColumnDecoder->uninitialize();
	m_pTargetDecoder->uninitialize();

	this->getAlgorithmManager().releaseAlgorithm(*m_pRowDecoder);
	this->getAlgorithmManager().releaseAlgorithm(*m_pColumnDecoder);
	this->getAlgorithmManager().releaseAlgorithm(*m_pTargetDecoder);

	return true;
}

boolean CBoxAlgorithmClassifierQualifier::processInput(uint32 ui32InputIndex)
{
	getBoxAlgorithmContext()->markAlgorithmAsReadyToProcess();

	return true;
}

boolean CBoxAlgorithmClassifierQualifier::process(void)
{
	IBoxIO& l_rDynamicBoxContext=this->getDynamicBoxContext();
	IBox& l_rStaticBoxContext=this->getStaticBoxContext();

	uint32 targetIdx=6+6;
	for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(targetIdx); i++)
	  {
		uint64 l_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(targetIdx, i);
		uint64 l_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(targetIdx, i);

		ip_pTargetDecoderMemoryBuffer=l_rDynamicBoxContext.getInputChunk(targetIdx, i);
		TParameterHandler < IStimulationSet* > op_pStimulationSet(m_pTargetDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
		m_pTargetDecoder->process();

		if(m_pTargetDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
			  {
				bool Row=true;
				for(uint32 j=0; j<op_pStimulationSet->getStimulationCount(); j++)
				  {
					if(op_pStimulationSet->getStimulationIdentifier(j)!=0 )
					  {
						//std::cout<<"Target : "<<j<<"ieme = "<<op_pStimulationSet->getStimulationIdentifier(j)<<std::endl;
						//make something
						Row ? 	m_ui64LastTargetRow=op_pStimulationSet->getStimulationIdentifier(j)-OVTK_StimulationId_Label_00-1	:
								m_ui64LastTargetColumn=op_pStimulationSet->getStimulationIdentifier(j)-OVTK_StimulationId_Label_00-1;
						Row=false;
						std::cout<<"Target is "<<m_ui64LastTargetRow<<","<<m_ui64LastTargetColumn<<std::endl;
					  }
				  }
			  }
		l_rDynamicBoxContext.markInputAsDeprecated(targetIdx, i);
	  }

	m_bStimulationNotEmpty=false;
	for(uint32 k=0; k<6; k++)
	  {
		for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(k); i++)
		  {
			uint64 l_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(k, i);
			uint64 l_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(k, i);

			ip_pRowDecoderMemoryBuffer=l_rDynamicBoxContext.getInputChunk(k, i);
			TParameterHandler < IStimulationSet* > op_pStimulationSetR(m_pRowDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
			m_pRowDecoder->process();

			if(m_pRowDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
			  {
				for(uint32 j=0; j<op_pStimulationSetR->getStimulationCount(); j++)
				  {
					if(op_pStimulationSetR->getStimulationIdentifier(j)!=0 )
					  {
						m_bStimulationNotEmpty=true;

						//std::cout<<k<<"ieme Row get a Stimulation : "<<j<<"ieme = "<<op_pStimulationSetR->getStimulationIdentifier(j)<<std::endl;
						//make something
						if(op_pStimulationSetR->getStimulationIdentifier(j)==OVTK_StimulationId_Label_00 )
						  {
						    std::cout<<"it's a undefined label"<<std::endl;
						  }
						if(op_pStimulationSetR->getStimulationIdentifier(j)==OVTK_StimulationId_Target )
						  {
							//std::cout<<"it's a target label"<<std::endl;
							if(k==m_ui64LastTargetRow)//Vrai positif (VP)
							  {
								//std::cout<<"VP : "<<k<<std::endl;
								m_ui64VraiPositif++;
							  }
							else//Faux positif (FP)
							  {
								m_ui64FauxPositif++;
							  }
						  }
						if(op_pStimulationSetR->getStimulationIdentifier(j)==OVTK_StimulationId_NonTarget )
						  {
							//std::cout<<"it's a nonTarget label"<<std::endl;
							if(k==m_ui64LastTargetRow)//Faux négatif (FN)
							  {
								m_ui64FauxNegatif++;
							  }
							else//Vrai négatif (VN)
							  {
								m_ui64VraiNegatif++;
							  }
						  }
					  }
				  }
			  }

			l_rDynamicBoxContext.markInputAsDeprecated(k, i);
		}
	  }

	/*if(m_bStimulationNotEmpty)
	  {
		std::cout<<"Classifier evolution Row : VP = "<<m_ui64VraiPositif<<". FP = "<<m_ui64FauxPositif<<
										". FN = "<<m_ui64FauxNegatif<<". VN = "<<m_ui64VraiNegatif<<std::endl;
	  }*/
  	/*for(uint32 k=0; k<l_rStaticBoxContext.getInputCount()-1; k++)
	  {
		if(l_rDynamicBoxContext.getInputChunkCount(k)!=0) {std::cout<<k<<"- nb input after row= "<<l_rDynamicBoxContext.getInputChunkCount(k)<<std::endl;}
	  }*/

	m_bStimulationNotEmpty=false;
	for(uint32 k=6; k<12; k++)
	  {
		for(uint32 i=0; i<l_rDynamicBoxContext.getInputChunkCount(k); i++)
		  {
			uint64 l_ui64StartTime=l_rDynamicBoxContext.getInputChunkStartTime(k, i);
			uint64 l_ui64EndTime=l_rDynamicBoxContext.getInputChunkEndTime(k, i);

			ip_pColumnDecoderMemoryBuffer=l_rDynamicBoxContext.getInputChunk(k, i);
			TParameterHandler < IStimulationSet* > op_pStimulationSetC(m_pColumnDecoder->getOutputParameter(OVP_GD_Algorithm_StimulationStreamDecoder_OutputParameterId_StimulationSet));
			m_pColumnDecoder->process();

			if(m_pColumnDecoder->isOutputTriggerActive(OVP_GD_Algorithm_StimulationStreamDecoder_OutputTriggerId_ReceivedBuffer))
			  {
				for(uint32 j=0; j<op_pStimulationSetC->getStimulationCount(); j++)
				  {
					if(op_pStimulationSetC->getStimulationIdentifier(j)!=0 )
					  {
						//std::cout<<"stim = "<<op_pStimulationSetC->getStimulationIdentifier(j)<<std::endl;
						m_bStimulationNotEmpty=true;

						//std::cout<<k<<"ieme Row get a Stimulation : "<<j<<"ieme = "<<op_pStimulationSetC->getStimulationIdentifier(j)<<std::endl;
						//make something
						if(op_pStimulationSetC->getStimulationIdentifier(j)==OVTK_StimulationId_Label_00 )
						  {
						    std::cout<<"it's a undefined label"<<std::endl;
						  }
						if(op_pStimulationSetC->getStimulationIdentifier(j)==OVTK_StimulationId_Target )
						  {
							//std::cout<<"it's a target label"<<std::endl;
							if(k==m_ui64LastTargetColumn)//Vrai positif (VP)
							  {
								//std::cout<<"VP : "<<k<<std::endl;
								m_ui64VraiPositif++;
							  }
							else//Faux positif (FP)
							  {
								m_ui64FauxPositif++;
							  }
						  }
						if(op_pStimulationSetC->getStimulationIdentifier(j)==OVTK_StimulationId_NonTarget )
						  {
							//std::cout<<"it's a nonTarget label"<<std::endl;
							if(k==m_ui64LastTargetColumn)//Faux négatif (FN)
							  {
								m_ui64FauxNegatif++;
							  }
							else//Vrai négatif (VN)
							  {
								m_ui64VraiNegatif++;
							  }
						  }
					  }
				  }
			  }

			l_rDynamicBoxContext.markInputAsDeprecated(k, i);
		}
	  }

	/*if(m_bStimulationNotEmpty)
	  {
		std::cout<<"Classifier evolution Column : VP = "<<m_ui64VraiPositif<<". FP = "<<m_ui64FauxPositif<<
										". FN = "<<m_ui64FauxNegatif<<". VN = "<<m_ui64VraiNegatif<<std::endl<<std::endl;
	  }*/

	return true;
}

void CBoxAlgorithmClassifierQualifier::saveClassifierQualification()
{
	FILE * pFile;
	pFile = fopen ("ClassifierQualification.txt" , "w");
	if (pFile == NULL) perror ("Error opening file");
	else
	  {
		char buf[30];
		fwrite ("VP : \n" , 1 , 6 , pFile ); //nombre de stimuli cible correctement identifiés (Vrai positifs)
		std::string strVP=itoa(m_ui64VraiPositif,buf,10);
		fwrite (strVP.c_str() , 1 , strVP.size() , pFile );
		fwrite ("\nVN : \n" , 1 , 7 , pFile ); //nombre de stimuli non-cibles justement ignorés (Vrais négatifs)
		std::string strVN=itoa(m_ui64VraiNegatif,buf,10);
		fwrite (strVN.c_str() , 1 , strVN.size() , pFile );
		fwrite ("\nFP : \n" , 1 , 7 , pFile ); //nombre de stimuli cible non identifiés (Faux négatifs)
		std::string strFP=itoa(m_ui64FauxPositif,buf,10);
		fwrite (strFP.c_str() , 1 , strFP.size() , pFile );
		fwrite ("\nFN : \n" , 1 , 7 , pFile ); //nombre de stimuli non-cible identifiés (Faux positifs)
		std::string strFN=itoa(m_ui64FauxNegatif,buf,10);
		fwrite (strFN.c_str() , 1 , strFN.size() , pFile );

		fwrite ("\nAccuracy : \n" , 1 , 13 , pFile ); //1-VP/(VP+FN)
		float accuracy=1-m_ui64VraiPositif/float(m_ui64VraiPositif+m_ui64FauxNegatif);
		std::string strAcc=float2string(accuracy);
		fwrite (strAcc.c_str() , 1 , strAcc.size() , pFile );
		fwrite ("\nSpecificity : \n" , 1 , 16 , pFile ); //VN/(VN+FP)
		float specificity=m_ui64VraiNegatif/float(m_ui64VraiNegatif+m_ui64FauxPositif);
		std::string strSpec=float2string(specificity);
		fwrite (strSpec.c_str() , 1 , strSpec.size() , pFile );

		fwrite ("\nITI : \n" , 1 , 8 , pFile ); //InterStimuli
		std::string strITI="UNKNOWN";//itoa(-1,buf,10);
		fwrite (strITI.c_str() , 1 , strITI.size() , pFile );
		fwrite ("\nNStim : \n" , 1 , 10 , pFile ); //nombre total de stimulation
		std::string strNStim=itoa(m_ui64VraiPositif+m_ui64FauxPositif+m_ui64VraiNegatif+m_ui64FauxNegatif,buf,10);
		fwrite (strNStim.c_str() , 1 , strNStim.size() , pFile );

		fwrite ("\nBitrate : \n" , 1 , 12 , pFile ); //bit rate
		std::string strBrate="UNKNOWN";//itoa(-1,buf,10);
		fwrite (strBrate.c_str() , 1 , strBrate.size() , pFile );
		fwrite ("\nBitrate1N : \n" , 1 , 14 , pFile ); //bit rate pour 1 commande indépendamment du temps
		float TauxReco=m_ui64VraiPositif/float(m_ui64VraiPositif+m_ui64FauxPositif+m_ui64VraiNegatif+m_ui64FauxNegatif);
		float bt1N=(TauxReco*log(TauxReco)+(1-TauxReco)*log((1-TauxReco)/float(36-1))+log(float(36)) )/log(float(2));
		std::string strBT1N=float2string(bt1N);
		fwrite (strBT1N.c_str() , 1 , strBT1N.size() , pFile );

		fwrite ("\nEND\n" , 1 , 5 , pFile );
		fclose (pFile);
	  }
}

std::string CBoxAlgorithmClassifierQualifier::float2string(float f)
{
  std::ostringstream os;
  os << f;
  return os.str();
}
