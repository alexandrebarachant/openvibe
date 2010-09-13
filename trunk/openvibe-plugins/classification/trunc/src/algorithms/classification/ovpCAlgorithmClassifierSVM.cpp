#include "ovpCAlgorithmClassifierSVM.h"

//#include <map>
#include <sstream>
#include <iostream>



using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

using namespace OpenViBEPlugins;
using namespace OpenViBEPlugins::Classification;

using namespace OpenViBEToolkit;

boolean CAlgorithmClassifierSVM::train(const IFeatureVectorSet& rFeatureVectorSet)
{
	// default Param values
	//std::cout<<"param config"<<std::endl;
	struct svm_parameter l_oParam;
	l_oParam.svm_type = C_SVC;
	l_oParam.kernel_type = LINEAR;
	l_oParam.degree = 3;
	l_oParam.gamma = 0;	// 1/num_features
	l_oParam.coef0 = 0;
	l_oParam.nu = 0.5;
	l_oParam.cache_size = 100;
	l_oParam.C = 1;
	l_oParam.eps = 1e-3;
	l_oParam.p = 0.1;
	l_oParam.shrinking = 1;
	l_oParam.probability = 1;
	l_oParam.nr_weight = 0;
	l_oParam.weight_label = NULL;
	l_oParam.weight = NULL;

	this->getLogManager() << LogLevel_Trace << paramToString(&l_oParam);

	//configure m_oProb
	//std::cout<<"prob config"<<std::endl;
	struct svm_problem l_oProb;
	l_oProb.l=rFeatureVectorSet.getFeatureVectorCount();
	m_ui32NumberOfFeatures=rFeatureVectorSet[0].getSize();

	l_oProb.y = new double[l_oProb.l];
	l_oProb.x = new svm_node*[l_oProb.l];

	//std::cout<< "number vector:"<<l_oProb.l<<" size of vector:"<<m_ui32NumberOfFeatures<<std::endl;

	for(int i=0;i<l_oProb.l;i++)
	{
		l_oProb.x[i] = new svm_node[m_ui32NumberOfFeatures+1];
		l_oProb.y[i] = rFeatureVectorSet[i].getLabel();
		for(uint32 j=0;j<m_ui32NumberOfFeatures;j++)
		{
			l_oProb.x[i][j].index=j;
			l_oProb.x[i][j].value=rFeatureVectorSet[i].getBuffer()[j];
		}
		l_oProb.x[i][m_ui32NumberOfFeatures].index=-1;
	}
	if(l_oParam.gamma == 0 && m_ui32NumberOfFeatures > 0)
	{
		l_oParam.gamma = 1.0/m_ui32NumberOfFeatures;
	}

	if(l_oParam.kernel_type == PRECOMPUTED)
	{
		for(int i=0;i<l_oProb.l;i++)
		{
			if(l_oProb.x[i][0].index!=0)
			{
				this->getLogManager() << LogLevel_Error << "Wrong input format: first column must be 0:sample_serial_number\n";
				return false;
			}
			if(l_oProb.x[i][0].value <= 0 || l_oProb.x[i][0].value > m_ui32NumberOfFeatures)
			{
				this->getLogManager() << LogLevel_Error << "Wrong input format: sample_serial_number out of range\n";
				return false;
			}
		}
	}

	this->getLogManager() << LogLevel_Trace << problemToString(&l_oProb);

	//make a model
	//std::cout<<"svm_train"<<std::endl;
	if(m_pModel)
	{
		//std::cout<<"delete model"<<std::endl;
		delete m_pModel;
		m_pModel=NULL;
	}
	m_pModel=svm_train(&l_oProb,&l_oParam);


	if(m_pModel == NULL)
	{
		this->getLogManager() << LogLevel_Error << "the training with SVM had failed\n";
		return false;
	}

	//std::cout<<"log model"<<std::endl;
	this->getLogManager() << LogLevel_Trace << modelToString();

	//xml file
	//std::cout<<"model save"<<std::endl;

	std::vector <OpenViBE::CString> l_vSVCoef;
	std::vector <OpenViBE::CString> l_vSVValue;

	//std::cout<<"model save: rho"<<std::endl;
	std::stringstream l_sRho;
	l_sRho << std::scientific << m_pModel->rho[0];

	for(int i=1;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
	{
		l_sRho << " " << m_pModel->rho[i];
	}

	//std::cout<<"model save: sv_coef and SV"<<std::endl;
	for(int i=0;i<m_pModel->l;i++)
	{
		std::stringstream l_sSVCoef;
		std::stringstream l_sSVValue;

		l_sSVCoef << m_pModel->sv_coef[0][i];
		for(int j=1;j<m_pModel->nr_class-1;j++)
		{
			l_sSVCoef << " " << m_pModel->sv_coef[j][i];
		}

		const svm_node *p = m_pModel->SV[i];

		if(m_pModel->param.kernel_type == PRECOMPUTED)
		{
			l_sSVValue << "0:" << (double)(p->value);
		}
		else
		{
			if(p->index != -1)
			{
				l_sSVValue << p->index << ":" <<p->value;
				p++;
			}
			while(p->index != -1)
			{
				l_sSVValue << " " << p->index << ":" << p->value;
				p++;
			}
		}
		l_vSVCoef.push_back(CString(l_sSVCoef.str().c_str()));
		l_vSVValue.push_back(CString(l_sSVValue.str().c_str()));
	}

	//std::cout<<"xml save"<<std::endl;
	m_oConfiguration.setSize(0, true);
	XML::IWriter* l_pWriter=XML::createWriter(*this);
	l_pWriter->openChild("OpenViBE-Classifier");
	 l_pWriter->openChild("SVM");
	  l_pWriter->openChild("Param");
	   l_pWriter->openChild("svm_type");
	    l_pWriter->setChildData(svm_type_table[m_pModel->param.svm_type]);
	   l_pWriter->closeChild();
	   l_pWriter->openChild("kernel_type");
		l_pWriter->setChildData(kernel_type_table[m_pModel->param.kernel_type]);
	   l_pWriter->closeChild();
	   if(m_pModel->param.kernel_type == POLY)
	   {
		   l_pWriter->openChild("degree");
		   std::stringstream l_sParamDegree;
		   l_sParamDegree << m_pModel->param.degree;
		   l_pWriter->setChildData(l_sParamDegree.str().c_str());
		   l_pWriter->closeChild();
	   }
	   if(m_pModel->param.kernel_type == POLY || m_pModel->param.kernel_type == RBF || m_pModel->param.kernel_type == SIGMOID)
	   {
		   l_pWriter->openChild("gamma");
		   std::stringstream l_sParamGamma;
		   l_sParamGamma << m_pModel->param.gamma;
		   l_pWriter->setChildData(l_sParamGamma.str().c_str());
		   l_pWriter->closeChild();
	   }
	   if(m_pModel->param.kernel_type == POLY || m_pModel->param.kernel_type == SIGMOID)
	   {
		   l_pWriter->openChild("coef0");
		   std::stringstream l_sParamCoef0;
		   l_sParamCoef0 << m_pModel->param.coef0;
		   l_pWriter->setChildData(l_sParamCoef0.str().c_str());
		   l_pWriter->closeChild();
	   }
	  l_pWriter->closeChild();
	  l_pWriter->openChild("Model");
	   l_pWriter->openChild("nr_class");
	    std::stringstream l_sModelNrClass;
	    l_sModelNrClass << m_pModel->nr_class;
	    l_pWriter->setChildData(l_sModelNrClass.str().c_str());
	   l_pWriter->closeChild();
	   l_pWriter->openChild("total_sv");
	    std::stringstream l_sModelTotalSV;
	   	l_sModelTotalSV << m_pModel->l;
	    l_pWriter->setChildData(l_sModelTotalSV.str().c_str());
	   l_pWriter->closeChild();
	   l_pWriter->openChild("rho");
	    l_pWriter->setChildData(l_sRho.str().c_str());
	   l_pWriter->closeChild();
	   if(m_pModel->label)
	   {
		   l_pWriter->openChild("label");
		   //std::cout<<"model save: label"<<std::endl;
		   std::stringstream l_sLabel;
		   l_sLabel << m_pModel->label[0];
		   for(int i=1;i<m_pModel->nr_class;i++)
		   {
			   l_sLabel << " " << m_pModel->label[i];
		   }
		    l_pWriter->setChildData(l_sLabel.str().c_str());
		   l_pWriter->closeChild();
	   }
	   if(m_pModel->probA)
	   {
		   l_pWriter->openChild("probA");
		   //std::cout<<"model save: probA"<<std::endl;
		   std::stringstream l_sProbA;
		   for(int i=1;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
		   {
			   l_sProbA << " " << m_pModel->probA[i];
		   }
		   l_sProbA << std::scientific << m_pModel->probA[0];
		   l_pWriter->setChildData(l_sProbA.str().c_str());
		   l_pWriter->closeChild();
	   }
	   if(m_pModel->probB)
	   {
		   l_pWriter->openChild("probB");
		   //std::cout<<"model save: probB"<<std::endl;
		   std::stringstream l_sProbB;
		   for(int i=1;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
		   {
		   		l_sProbB << " " << m_pModel->probB[i];
		   }
		   l_sProbB << std::scientific << m_pModel->probB[0];
		   l_pWriter->setChildData(l_sProbB.str().c_str());
		   l_pWriter->closeChild();
	   }

	   if(m_pModel->nSV)
	   {
		   l_pWriter->openChild("nr_sv");
		   //std::cout<<"model save: nSV"<<std::endl;
		   std::stringstream l_sNrSV;
		   l_sNrSV << m_pModel->nSV[0];
		   for(int i=1;i<m_pModel->nr_class;i++)
		   {
			   l_sNrSV << " " << m_pModel->nSV[i];
		   }
		    l_pWriter->setChildData(l_sNrSV.str().c_str());
		   l_pWriter->closeChild();
	   }
	   l_pWriter->openChild("SVs");
	   for(int i=0;i<m_pModel->l;i++)
	   {
		   l_pWriter->openChild("SV");
		    l_pWriter->openChild("coef");
		     l_pWriter->setChildData(l_vSVCoef[i]);
		    l_pWriter->closeChild();
		    l_pWriter->openChild("value");
		     l_pWriter->setChildData(l_vSVValue[i]);
		    l_pWriter->closeChild();
		   l_pWriter->closeChild();
	   }
	   l_pWriter->closeChild();
	  l_pWriter->closeChild();
	 l_pWriter->closeChild();
	l_pWriter->closeChild();
	l_pWriter->release();
	l_pWriter=NULL;

	for(int i=0;i<l_oProb.l;i++)
	{
		delete[] l_oProb.x[i];
	}
	delete[] l_oProb.y;
	delete[] l_oProb.x;
	l_oProb.y=NULL;
	l_oProb.x=NULL;
	delete m_pModel;
	m_pModel=NULL;
	return true;
}

boolean CAlgorithmClassifierSVM::classify(const IFeatureVector& rFeatureVector, float64& rf64Class, IVector& rClassificationValues)
{
	//std::cout<<"classify"<<std::endl;
	if(m_pModel==NULL)
	{
		this->getLogManager() << LogLevel_Error << "classify impossible with a model equal NULL\n";
		return false;
	}
	//std::cout<<"create l_pX"<<std::endl;
	svm_node* l_pX=new svm_node[rFeatureVector.getSize()+1];
	//std::cout<<"rFeatureVector.getSize():"<<rFeatureVector.getSize()<<"m_ui32NumberOfFeatures"<<m_ui32NumberOfFeatures<<std::endl;
	for(int i=0;i<rFeatureVector.getSize();i++)
	{
		l_pX[i].index=i;
		l_pX[i].value=rFeatureVector.getBuffer()[i];
		//std::cout<< l_pX[i].index << ";"<<l_pX[i].value<<" ";
	}
	l_pX[rFeatureVector.getSize()].index=-1;

	//std::cout<<"create l_pProbEstimates"<<std::endl;
	double *l_pProbEstimates=NULL;
	l_pProbEstimates = new double[m_pModel->nr_class];
	for(int i=0;i<m_pModel->nr_class;i++)
	{
		l_pProbEstimates[i]=0;
	}
	//std::cout<<"call svm_predict_probability "<<m_pModel<<" "<<m_pModel->SV<<std::endl;
	/*for(int i=0;i<m_pModel->l;i++)
			{
				svm_node* j=m_pModel->SV[i];
				while(j->index!=-1)
				{
					std::cout<<j<<"/";
					std::cout<<j->index << ":";
					std::cout<< j->value<<" ";
					j++;
				}
			}*/
	//std::cout<<std::endl;
	rf64Class=svm_predict_probability(m_pModel,l_pX,l_pProbEstimates);
	//std::cout<<rf64Class<<std::endl;
	//std::cout<<"probability"<<std::endl;
	this->getLogManager() << LogLevel_Trace <<"Label predict: "<<rf64Class<<"\n";

	for(int i=0;i<m_pModel->nr_class;i++)
	{
		this->getLogManager() << LogLevel_Trace << "index:"<<i<<" label:"<< m_pModel->label[i]<<" probability:"<<l_pProbEstimates[i]<<"\n";
		if( m_pModel->label[i] == 1 )
		{
			rClassificationValues.setSize(1);
			rClassificationValues[0]=1-l_pProbEstimates[i];

		}
	}
	//std::cout<<";"<<rf64Class<<";"<<rClassificationValues[0] <<";"<<l_pProbEstimates[0]<<";"<<l_pProbEstimates[1]<<std::endl;
	//std::cout<<"Label predict "<<rf64Class<< " proba:"<<rClassificationValues[0]<<std::endl;
	//std::cout<<"end classify"<<std::endl;
	delete[] l_pX;
	delete[] l_pProbEstimates;
	/*if(rFeatureVector.getSize()+1!=(unsigned int)m_oCoefficients.size())
	{
		this->getLogManager() << LogLevel_Warning << "Feature vector size " << rFeatureVector.getSize() << " and hyperplane parameter size " << (uint32) m_oCoefficients.size() << " does not match\n";
		return false;
	}

	itpp::vec l_oFeatures(rFeatureVector.getBuffer(), rFeatureVector.getSize());

	l_oFeatures.ins(0, 1);

	float64 l_f64Result=-l_oFeatures*m_oCoefficients;

	rClassificationValues.setSize(1);
	rClassificationValues[0]=l_f64Result;

	if(l_f64Result < 0)
	{
		rf64Class=m_f64Class1;
	}
	else
	{
		rf64Class=m_f64Class2;
	}*/

	return true;
}

boolean CAlgorithmClassifierSVM::saveConfiguration(IMemoryBuffer& rMemoryBuffer)
{
	//std::cout<<"save config"<<std::endl;
	rMemoryBuffer.setSize(0, true);
	rMemoryBuffer.append(m_oConfiguration);
	return true;
}

boolean CAlgorithmClassifierSVM::loadConfiguration(const IMemoryBuffer& rMemoryBuffer)
{

	if(m_pModel)
	{
		//std::cout<<"delete m_pModel load config"<<std::endl;
		delete m_pModel;
		m_pModel=NULL;
	}
	//std::cout<<"load config"<<std::endl;
	m_pModel=new svm_model();
	m_pModel->rho = NULL;
	m_pModel->probA = NULL;
	m_pModel->probB = NULL;
	m_pModel->label = NULL;
	m_pModel->nSV = NULL;
	m_i32IndexSV=-1;
	XML::IReader* l_pReader=XML::createReader(*this);
	l_pReader->processData(rMemoryBuffer.getDirectPointer(), rMemoryBuffer.getSize());
	l_pReader->release();
	l_pReader=NULL;
	//std::cout<<"load config fin"<<std::endl;
	this->getLogManager() << LogLevel_Trace << modelToString();
	return true;
}

void CAlgorithmClassifierSVM::write(const char* sString)
{
	m_oConfiguration.append((const uint8*)sString, ::strlen(sString));
}

void CAlgorithmClassifierSVM::openChild(const char* sName, const char** sAttributeName, const char** sAttributeValue, XML::uint64 ui64AttributeCount)
{
	m_vNode.push(sName);
	if( CString(sName) == CString("SVs"))
	{
		m_pModel->sv_coef = new double*[m_pModel->nr_class-1];
		for(int i=0;i<m_pModel->nr_class-1;i++)
		{
			m_pModel->sv_coef[i]=new double[m_pModel->l];
		}
		m_pModel->SV = new svm_node*[m_pModel->l];
	}
	if(CString(sName) == CString("SV"))
	{
		m_i32IndexSV++;
	}
}

void CAlgorithmClassifierSVM::processChildData(const char* sData)
{
	std::stringstream l_sData(sData);
	// param
	if(m_vNode.top()==CString("svm_type"))
	{
		for(int i =0; svm_type_table[i];i++)
		{
			if ( strcmp(svm_type_table[i],l_sData.str().c_str())==0)
			{
				m_pModel->param.svm_type=i;
			}
		}
		if(svm_type_table[m_pModel->param.svm_type] == NULL)
		{
			this->getLogManager() << LogLevel_Error << "load configuration error: bad value for the parameter svm_type\n";
		}
	}
	if(m_vNode.top()==CString("kernel_type"))
	{
		for(int i =0; kernel_type_table[i];i++)
		{
			if ( strcmp(kernel_type_table[i],l_sData.str().c_str())==0)
			{
				m_pModel->param.kernel_type=i;
			}
		}
		if(kernel_type_table[m_pModel->param.kernel_type] == NULL)
		{
			this->getLogManager() << LogLevel_Error << "load configuration error: bad value for the parameter kernel_type\n";
		}
	}
	if(m_vNode.top()==CString("degree"))
	{
		l_sData >> m_pModel->param.degree;
	}
	if(m_vNode.top()==CString("gamma"))
	{
		l_sData >> m_pModel->param.gamma;
	}
	if(m_vNode.top()==CString("coef0"))
	{
		l_sData >> m_pModel->param.coef0;
	}

	//model
	if(m_vNode.top()==CString("nr_class"))
	{
		l_sData >> m_pModel->nr_class;
	}
	if(m_vNode.top()==CString("total_sv"))
	{
		l_sData >> m_pModel->l;
	}
	if(m_vNode.top()==CString("rho"))
	{
		m_pModel->rho = new double[m_pModel->nr_class*(m_pModel->nr_class-1)/2];
		for(int i=0;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
		{
			l_sData >> m_pModel->rho[i];
		}
	}
	if(m_vNode.top()==CString("label"))
	{
		m_pModel->label = new int[m_pModel->nr_class];
		for(int i=0;i<m_pModel->nr_class;i++)
		{
			l_sData >> m_pModel->label[i];
		}
	}
	if(m_vNode.top()==CString("probA"))
	{
		m_pModel->probA = new double[m_pModel->nr_class*(m_pModel->nr_class-1)/2];
		for(int i=0;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
		{
			l_sData >> m_pModel->probA[i];
		}
	}
	if(m_vNode.top()==CString("probB"))
	{
		m_pModel->probB = new double[m_pModel->nr_class*(m_pModel->nr_class-1)/2];
		for(int i=0;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
		{
			l_sData >> m_pModel->probB[i];
		}
	}
	if(m_vNode.top()==CString("nr_sv"))
	{
		m_pModel->nSV = new int[m_pModel->nr_class];
		for(int i=0;i<m_pModel->nr_class;i++)
		{
			l_sData >> m_pModel->nSV[i];
		}
	}
	if(m_vNode.top()==CString("coef"))
	{
		for(int j=0;j<m_pModel->nr_class-1;j++)
		{
			l_sData >> m_pModel->sv_coef[j][m_i32IndexSV];
		}
	}
	if(m_vNode.top()==CString("value"))
	{
		std::vector <int> l_vSVMIndex;
		std::vector <double> l_vSVMValue;
		char l_cSeparateChar;
		while(!l_sData.eof())
		{
			int l_iIndex;
			double l_dValue;
			l_sData >> l_iIndex;
			l_sData >> l_cSeparateChar;
			l_sData >> l_dValue;
			l_vSVMIndex.push_back(l_iIndex);
			l_vSVMValue.push_back(l_dValue);
		}
		m_ui32NumberOfFeatures=l_vSVMIndex.size();
		m_pModel->SV[m_i32IndexSV] = new svm_node[l_vSVMIndex.size()+1];
		//std::stringstream l_sdata;
		for(size_t i=0;i<l_vSVMIndex.size();i++)
		{
			m_pModel->SV[m_i32IndexSV][i].index=l_vSVMIndex[i];
			m_pModel->SV[m_i32IndexSV][i].value=l_vSVMValue[i];
			//l_sdata << m_pModel->SV[m_i32IndexSV][i].value <<" ";
		}
		//this->getLogManager() << LogLevel_Info << l_sdata.str().c_str()<<"\n";
		m_pModel->SV[m_i32IndexSV][l_vSVMIndex.size()].index=-1;
	}
}

void CAlgorithmClassifierSVM::closeChild(void)
{
	m_vNode.pop();
}

CString CAlgorithmClassifierSVM::paramToString(svm_parameter *pParam)
{
	std::stringstream l_sParam;
	if(pParam == NULL)
	{
		l_sParam << "Param: NULL\n";
		return CString(l_sParam.str().c_str());
	}
	l_sParam << "Param:\n";
	l_sParam << "\tsvm_type: "<<svm_type_table[pParam->svm_type]<<"\n";
	l_sParam << "\tkernel_type: "<<kernel_type_table[pParam->kernel_type]<<"\n";
	l_sParam << "\tdegree: "<<pParam->degree<<"\n";
	l_sParam << "\tgamma: "<<pParam->gamma<<"\n";
	l_sParam << "\tcoef0: "<<pParam->coef0<<"\n";
	l_sParam << "\tnu: "<<pParam->nu << "\n";
	l_sParam << "\tcache_size: "<<pParam->cache_size << "\n";
	l_sParam << "\tC: "<<pParam->C << "\n";
	l_sParam << "\teps: "<<pParam->eps << "\n";
	l_sParam << "\tp: "<<pParam->p << "\n";
	l_sParam << "\tshrinking: "<<pParam->shrinking << "\n";
	l_sParam << "\tprobability: "<<pParam->probability << "\n";
	l_sParam << "\tnr weight: "<<pParam->nr_weight << "\n";
	l_sParam << "\tweight label: "<<pParam->weight_label << "\n";
	l_sParam << "\tweight: "<<pParam->weight << "\n";
	return CString(l_sParam.str().c_str());
}
CString CAlgorithmClassifierSVM::modelToString()
{
	std::stringstream l_sModel;
	if(m_pModel==NULL)
	{
		l_sModel << "Model: NULL\n";
		return CString(l_sModel.str().c_str());
	}
	l_sModel << paramToString(&m_pModel->param);
	l_sModel << "Model:" << "\n";
	l_sModel << "\tnr_class: "<< m_pModel->nr_class <<"\n";
	l_sModel << "\ttotal_sv: "<<m_pModel->l<<"\n";
	l_sModel << "\trho: ";
	if(m_pModel->rho)
	{
		l_sModel << m_pModel->rho[0];
		for(int i=1;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
		{
			l_sModel << " "<<m_pModel->rho[i];
		}
	}
	l_sModel << "\n";
	l_sModel << "\tlabel: ";
	if(m_pModel->label)
	{
		l_sModel << m_pModel->label[0];
		for(int i=1;i<m_pModel->nr_class;i++)
		{
			l_sModel << " "<<m_pModel->label[i];
		}
	}
	l_sModel << "\n";
	l_sModel << "\tprobA: ";
	if(m_pModel->probA)
	{
		l_sModel << m_pModel->probA[0];
		for(int i=1;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
		{
			l_sModel << " "<<m_pModel->probA[i];
		}
	}
	l_sModel << "\n";
	l_sModel << "\tprobB: ";
	if(m_pModel->probB)
	{
		l_sModel << m_pModel->probB[0];
		for(int i=1;i<m_pModel->nr_class*(m_pModel->nr_class-1)/2;i++)
		{
			l_sModel << " "<<m_pModel->probB[i];
		}
	}
	l_sModel << "\n";
	l_sModel << "\tnr_sv: ";
	if(m_pModel->nSV)
	{
		l_sModel << m_pModel->nSV[0];
		for(int i=1;i<m_pModel->nr_class;i++)
		{
			l_sModel << " "<<m_pModel->nSV[i];
		}
	}
	l_sModel << "\n";
	/*l_sModel << "\tSV: "<<"\n";
	if(m_pModel->sv_coef !=NULL && m_pModel->SV != NULL)
	{
		for(int i=0;i<m_pModel->l;i++)
		{
			l_sModel << "\t("<<i<<")\tcoef: "<<m_pModel->sv_coef[0][i];
			for(int j=1;j<m_pModel->nr_class-1;j++)
			{
				l_sModel << " "<<m_pModel->sv_coef[j][i];
			}
			l_sModel << "\n";
			l_sModel << "\t("<<i<<")\tvalue: "<<m_pModel->SV[i][0].index<<":"<<m_pModel->SV[i][0].value;
			for(int j=1;j<m_ui32NumberOfFeatures+1;j++)
			{
				l_sModel << " "<<m_pModel->SV[i][j].index<<":"<<m_pModel->SV[i][0].value;
			}
			l_sModel << "\n";
		}
	}*/
	return CString(l_sModel.str().c_str());
}
CString CAlgorithmClassifierSVM::problemToString(svm_problem *pProb)
{
	std::stringstream l_sProb;
	if(pProb == NULL)
	{
		l_sProb << "Problem: NULL\n";
		return CString(l_sProb.str().c_str());
	}
	l_sProb << "Problem";
	l_sProb << "\ttotal sv: "<<pProb->l<<"\n";
	l_sProb << "\tnb features: " << m_ui32NumberOfFeatures << "\n";
	/*for(int i=0;i<pProb->l;i++)
	{
		l_sProb << "\t("<<i<<")\ty = " << pProb->y[i]<<": ";
		for(uint32 j=0;j<m_ui32NumberOfFeatures+1;j++)
		{
			l_sProb << " " <<pProb->x[i][j].index <<";"<<pProb->x[i][j].value;
		}
		l_sProb<<"\n";
	}*/
	return CString(l_sProb.str().c_str());
}
