#ifndef __SamplePlugin_CTopographicMap3DDisplay_H__
#define __SamplePlugin_CTopographicMap3DDisplay_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>

#include "ovpCTopographicMapDatabase.h"
#include "ovpCTopographicMap3DDisplay/ovpCTopographicMap3DView.h"

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CTopographicMap3DDisplay : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >,
		virtual public OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback,
		virtual public CTopographicMapDrawable
		{
		public:
			CTopographicMap3DDisplay(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(
				OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean processClock(
				OpenViBE::Kernel::IMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_TopographicMap3DDisplay)

			//IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback implementation
			virtual void setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
			virtual void setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
			virtual void setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
			virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer);
			/*
			virtual void setChannelCount(const OpenViBE::uint32 ui32ChannelCount);
			virtual void setChannelName(const OpenViBE::uint32 ui32ChannelIndex, const char* sChannelName);
			virtual void setSampleCountPerBuffer(const OpenViBE::uint32 ui32SampleCountPerBuffer);
			virtual void setSamplingRate(const OpenViBE::uint32 ui32SamplingFrequency);
			virtual void setSampleBuffer(const OpenViBE::float64* pBuffer);
			*/

			//CSignalDisplayDrawable implementation
			//-------------------------------------
			/**
			 * Initializes the window.
			 */
			virtual void init();
			/**
			 * Invalidates the window's content and tells it to redraw itself.
			 */
			virtual void redraw();

			//CTopographicMapDrawable implementation
			//--------------------------------------
			/**
			* Returns pointer to sample matrix (places where to interpolate values)
			*/
			virtual OpenViBE::CMatrix* getSampleCoordinatesMatrix();
			/**
			* Sets pointer to value matrix (values interpolated at places specified in sample matrix)
			*/
			virtual OpenViBE::boolean setSampleValuesMatrix(OpenViBE::IMatrix* pSampleValuesMatrix);

		protected:
			void process3D();

			/**
			* Returns world coordinates of an electrode
			*/
			OpenViBE::boolean getChannelWorldCoordinates(
				OpenViBE::uint32 ui32ChannelIndex,
				OpenViBE::float64& rElectrodeWorldX,
				OpenViBE::float64& rElectrodeWorldY,
				OpenViBE::float64& rElectrodeWorldZ);

			/**
			* Returns object (local) coordinates of an electrode
			*/
			OpenViBE::boolean getElectrodeObjectCoordinates(
				OpenViBE::uint32 ui32ChannelIndex,
				OpenViBE::float64& rElectrodeObjectX,
				OpenViBE::float64& rElectrodeObjectY,
				OpenViBE::float64& rElectrodeObjectZ);

			//ebml
			EBML::IReader* m_pStreamedMatrixReader;

			OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback* m_pStreamedMatrixReaderCallBack;

			//Start and end time of the last buffer
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;

			OpenViBE::Kernel::IAlgorithmProxy* m_pProxy;
			CTopographicMapDatabase* m_pTopographicMapDatabase;
			CTopographicMap3DView* m_pTopographicMap3DView; //main object used for the display (contains all the GUI code)

			OpenViBE::CIdentifier m_o3DWidgetIdentifier;

			OpenViBE::boolean m_bCameraPositioned;

			OpenViBE::uint32 m_ui32NbColors; //number of predefined colors
			OpenViBE::float32* m_pColorScale; //scale of predefined colors potentials are converted to

			std::vector<OpenViBE::CIdentifier> m_oElectrodeIds; //ids of electrode objects
			OpenViBE::CIdentifier m_oScalpId; //ID of scalp object
			OpenViBE::uint32 m_ui32NbScalpVertices;	//number of scalp vertices
			OpenViBE::CMatrix m_oSampleCoordinatesMatrix; //normalized vertices where to interpolate potentials
			OpenViBE::float32* m_pScalpColors; //scalp vertex colors
		};

		class CTopographicMap3DDisplayDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("3D topographic map"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Vincent Delannoy"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("3D potentials mapping using spherical spline interpolation"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("3D visualisation/Basic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString(GTK_STOCK_EXECUTE); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_TopographicMap3DDisplay; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SimpleVisualisation::CTopographicMap3DDisplay(); }

			virtual OpenViBE::boolean hasFunctionality(OpenViBE::Kernel::EPluginFunctionality ePF) const
			{
				return ePF == OpenViBE::Kernel::PluginFunctionality_Visualization;
			}

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				//rPrototype.addSetting("Log level to use", OV_TypeId_LogLevel, "Information");
				rPrototype.addInput("Signal", OV_TypeId_StreamedMatrix);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_TopographicMap3DDisplayDesc);
		};
	};
};

#endif // __SamplePlugin_CTopographicMap3DDisplay_H__
