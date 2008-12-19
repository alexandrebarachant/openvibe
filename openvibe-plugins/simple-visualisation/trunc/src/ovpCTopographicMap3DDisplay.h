#ifndef __SamplePlugin_CTopographicMap3DDisplay_H__
#define __SamplePlugin_CTopographicMap3DDisplay_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <ebml/IReader.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include "ovpCTopographicMapDatabase.h"

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CTopographicMap3DView;

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

			/** \name IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback implementation */
			//@{
			virtual void setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
			virtual void setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
			virtual void setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
			virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer);
			//@}

			/** \name CSignalDisplayDrawable implementation */
			//@{

			/**
			 * \brief Initialize window.
			 */
			virtual void init();
			/**
			 * \brief Invalidate window contents and tell it to redraw itself.
			 */
			virtual void redraw();

			//@}

			/** \name CTopographicMapDrawable implementation */
			//@{

			/**
			 * \brief Return pointer to sample matrix (locations where to interpolate values)
			 */
			virtual OpenViBE::CMatrix* getSampleCoordinatesMatrix();
			/**
			 * \brief Set pointer to values matrix (interpolation results)
			 */
			virtual OpenViBE::boolean setSampleValuesMatrix(OpenViBE::IMatrix* pSampleValuesMatrix);
			/**
			 * \brief Toggle electrodes on/off
			 */
			void toggleElectrodes(OpenViBE::boolean);

			//void toggleSamplingPoints(OpenViBE::boolean);

			//@}

		protected:
			/**
			 * \brief Decode a channel localisation memory buffer
			 * This function fills a matrix with names and coordinates of electrodes in
			 * the frame of the head model used by this plugin. Coordinates are expected
			 * to be expressed in a cartesian frame.
			 * \param pMemoryBuffer Memory buffer to decode
			 * \return True if memory buffer could be properly decoded, false otherwise
			 */
			OpenViBE::boolean decodeChannelLocalisationMemoryBuffer(
				const OpenViBE::IMemoryBuffer* pMemoryBuffer);

			/**
			 * \brief Process 3D requests
			 */
			void process3D();

			/**
			 * \brief Create 3D skull
			 */
			OpenViBE::boolean createSkull();

			/**
			 * \brief Create 3D objects at channel locations
			 */
			OpenViBE::boolean createElectrodes();

			/**
			 * \brief Create 3D objects at sampling points locations
			 */
			OpenViBE::boolean createSamplingPoints();

			/**
			 * \brief Return world coordinates of an electrode
			 */
			OpenViBE::boolean getChannelWorldCoordinates(
				OpenViBE::uint32 ui32ChannelIndex,
				OpenViBE::float64& rElectrodeWorldX,
				OpenViBE::float64& rElectrodeWorldY,
				OpenViBE::float64& rElectrodeWorldZ);

			/**
			 * \brief Return object (local) coordinates of an electrode
			 */
			OpenViBE::boolean getElectrodeObjectCoordinates(
				OpenViBE::uint32 ui32ChannelIndex,
				OpenViBE::float64& rElectrodeObjectX,
				OpenViBE::float64& rElectrodeObjectY,
				OpenViBE::float64& rElectrodeObjectZ);

		private:
			//channel localisation decoder
			OpenViBE::Kernel::IAlgorithmProxy* m_pChannelLocalisationStreamDecoder;

			//ebml
			EBML::IReader* m_pStreamedMatrixReader;
			OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback* m_pStreamedMatrixReaderCallBack;

			//Name of file containing face mesh
			OpenViBE::CString m_oFaceMeshFilename;
			//Name of file containing scalp mesh
			OpenViBE::CString m_oScalpMeshFilename;
			//Name of file containing sphere on which scalp vertices are mapped
			OpenViBE::CString m_oProjectionSphereMeshFilename;

			//Start and end time of the last buffer
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSphericalSplineInterpolation;
			CTopographicMapDatabase* m_pTopographicMapDatabase;
			CTopographicMap3DView* m_pTopographicMap3DView; //main object used for the display (contains all the GUI code)

			OpenViBE::CIdentifier m_o3DWidgetIdentifier;
			OpenViBE::CIdentifier m_oResourceGroupIdentifier;

			OpenViBE::boolean m_bSkullCreated;
			OpenViBE::boolean m_bCameraPositioned;
			OpenViBE::boolean m_bElectrodesCreated;

			//flag set to true once channel localisation buffer is received
			OpenViBE::boolean m_bModelElectrodeCoordinatesInitialized;
			//matrix of electrode labels and coordinates expressed in head model frame
			OpenViBE::CMatrix m_oModelElectrodeCoordinates;

			OpenViBE::uint32 m_ui32NbColors; //number of predefined colors
			OpenViBE::float32* m_pColorScale; //scale of predefined colors potentials are converted to

			std::vector<OpenViBE::CIdentifier> m_oElectrodeIds; //ids of electrode objects
			OpenViBE::boolean m_bNeedToggleElectrodes;
			OpenViBE::boolean m_bElectrodesToggleState;

			std::vector<OpenViBE::CIdentifier> m_oSamplingPointIds; //ids of sampling point objects
			OpenViBE::boolean m_bNeedToggleSamplingPoints;
			OpenViBE::boolean m_bSamplingPointsToggleState;
			//ID of scalp object
			OpenViBE::CIdentifier m_oScalpId;
			//number of scalp vertices
			OpenViBE::uint32 m_ui32NbScalpVertices;
			//working copy of scalp vertices coordinates
			OpenViBE::float32* m_pScalpVertices;
			//center of unit sphere on which scalp vertices are projected, in normalized space (X right Y front Z up)
			OpenViBE::float32 m_f32ProjectionCenter[3];
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
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Visualisation/Topography"); }
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
				rPrototype.addSetting("Interpolation type", OVP_TypeId_SphericalLinearInterpolationType, "1");
				rPrototype.addSetting("Delay (in s)", OV_TypeId_Float, "0");
				rPrototype.addSetting("Face mesh filename", OV_TypeId_String, "face");
				rPrototype.addSetting("Scalp mesh filename", OV_TypeId_String, "scalp");
				rPrototype.addSetting("Sphere projection mesh filename", OV_TypeId_String, "projection_center");
				rPrototype.addInput("Signal", OV_TypeId_StreamedMatrix);
				rPrototype.addInput("Channel localisation", OV_TypeId_ChannelLocalisation);
				rPrototype.addInput("Channel localisation in model frame", OV_TypeId_ChannelLocalisation);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_TopographicMap3DDisplayDesc);
		};
	};
};

#endif // __SamplePlugin_CTopographicMap3DDisplay_H__
