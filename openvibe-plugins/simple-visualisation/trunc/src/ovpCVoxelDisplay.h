#ifndef __SamplePlugin_CVoxelDisplay_H__
#define __SamplePlugin_CVoxelDisplay_H__

#include "ovp_defines.h"

#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

#include <glade/glade.h>
#include <gtk/gtk.h>

#include "ovpCStreamedMatrixDatabase.h"

namespace OpenViBEPlugins
{
	namespace SimpleVisualisation
	{
		class CVoxelView;

		class CVoxel
		{
		public:
			CVoxel();

			OpenViBE::boolean setObjectIdentifiers(
				OpenViBE::CIdentifier oCubeIdentifier,
				OpenViBE::CIdentifier oSphereIdentifier);

			OpenViBE::boolean setPosition(
				OpenViBE::float32 f32X,
				OpenViBE::float32 f32Y,
				OpenViBE::float32 f32Z);

			//object identifiers
			OpenViBE::CIdentifier m_oCubeIdentifier;
			OpenViBE::CIdentifier m_oSphereIdentifier;
			//current visibility state of active object
			OpenViBE::boolean m_bVisible;
			//coordinates of active object
			OpenViBE::float32 m_f32X, m_f32Y, m_f32Z;
		};

		class CVoxelDisplay : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:
			CVoxelDisplay(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);

			virtual OpenViBE::boolean initialize(void);

			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processInput(
				OpenViBE::uint32 ui32InputIndex);

			virtual OpenViBE::boolean processClock(
				OpenViBE::Kernel::IMessageClock& rMessageClock);

			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_VoxelDisplay)

			/** \name CVoxelView callbacks */
			//@{

			OpenViBE::boolean setVoxelObject(
				OpenViBE::Kernel::EStandard3DObject eStandard3DObject);

			OpenViBE::boolean toggleColorModification(
				OpenViBE::boolean bModifyColor);

			OpenViBE::boolean toggleTransparencyModification(
				OpenViBE::boolean bModifyTransparency);

			OpenViBE::boolean toggleSizeModification(
				OpenViBE::boolean bModifySize);

			OpenViBE::boolean setMinScaleFactor(
				OpenViBE::float64 f64MinScaleFactor);

			OpenViBE::boolean setMaxScaleFactor(
				OpenViBE::float64 f64MaxScaleFactor);

			OpenViBE::boolean setVoxelDisplayThreshold(
				OpenViBE::float64 f64Threshold);

			OpenViBE::boolean setSkullOpacity(
				OpenViBE::float64 f64Opacity);

			OpenViBE::boolean setPaused(
				OpenViBE::boolean bPaused);

			OpenViBE::boolean repositionCamera();

			//@}

		private:
			OpenViBE::CIdentifier getActiveShapeIdentifier(CVoxel& rVoxel);

			OpenViBE::boolean process3D();

				OpenViBE::boolean createVoxels();

				//REMOVE ME
				OpenViBE::boolean computePotentials();
				OpenViBE::boolean getMinMaxPotentials(OpenViBE::float64& rMinPotential, OpenViBE::float64& rMaxPotential);
				//

				OpenViBE::boolean updateVoxels();

		private:
			//Streamed matrix database
			CStreamedMatrixDatabase* m_pStreamedMatrixDatabase;
			//GUI management
			CVoxelView* m_pVoxelView;

			OpenViBE::CIdentifier m_o3DWidgetIdentifier;
			OpenViBE::CIdentifier m_oResourceGroupIdentifier;

			OpenViBE::boolean m_bCameraPositioned;
			OpenViBE::boolean m_bPaused;
			OpenViBE::float64 m_f64Time;

			//REMOVE ME : get matrix from database!
			OpenViBE::CMatrix m_oPotentialMatrix;

			OpenViBE::uint32 m_ui32NbColors; //number of predefined colors
			OpenViBE::float32* m_pColorScale; //scale of predefined colors potentials are converted to

			std::vector<OpenViBE::CIdentifier> m_oElectrodeIds; //ids of electrode objects
			std::vector<OpenViBEPlugins::SimpleVisualisation::CVoxel> m_oVoxels; //voxels vector
			OpenViBE::CIdentifier m_oScalpId;
			OpenViBE::CIdentifier m_oFaceId;

			/** \name Members modified by CVoxelView requests */
			//@{
			//set voxel object
			OpenViBE::boolean m_bSetVoxelObject;
			OpenViBE::Kernel::EStandard3DObject m_eVoxelObject;

			//toggle color modification
			OpenViBE::boolean m_bToggleColorModification;
			OpenViBE::boolean m_bColorModificationToggled;

			//toggle transparency modification
			OpenViBE::boolean m_bToggleTransparencyModification;
			OpenViBE::boolean m_bTransparencyModificationToggled;

			//toggle size modification
			OpenViBE::boolean m_bToggleSizeModification;
			OpenViBE::boolean m_bSizeModificationToggled;

			//scale factors
			OpenViBE::float64 m_f64MinScaleFactor;
			OpenViBE::float64 m_f64MaxScaleFactor;

			//voxel display threshold
			OpenViBE::float64 m_f64VoxelDisplayThreshold;

			//set skull opacity
			OpenViBE::boolean m_bSetSkullOpacity;
			OpenViBE::float64 m_f64SkullOpacity;

			//reposition camera
			OpenViBE::boolean m_bRepositionCamera;
			//@}
		};

		class CVoxelDisplayDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Voxel visualizer"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Vincent Delannoy"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Displays brain activity as voxels"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Visualisation/Volume"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString(GTK_STOCK_EXECUTE); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_VoxelDisplay; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SimpleVisualisation::CVoxelDisplay(); }

			virtual OpenViBE::boolean hasFunctionality(OpenViBE::Kernel::EPluginFunctionality ePF) const
			{
				return ePF == OpenViBE::Kernel::PluginFunctionality_Visualization;
			}

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				//rPrototype.addSetting("Log level to use", OV_TypeId_LogLevel, "Information");
				rPrototype.addInput("Streamed matrix", OV_TypeId_StreamedMatrix);
				rPrototype.addFlag(OpenViBE::Kernel::BoxFlag_IsUnstable);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_VoxelDisplayDesc);
		};
	};
};

#endif // __SamplePlugin_CTopographicMap3DDisplay_H__
