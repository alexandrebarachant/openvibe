#ifndef __OpenViBEApplication_Defines_H__
#define __OpenViBEApplication_Defines_H__

#include <OMKSimulatedObject.h>
#include <OMKSimulatedObject.inl>
#include <OMKExtensibleSimulatedObject.h>
#include <OMKExtensibleSimulatedObject.inl>
#include <OMKBasicTypes.h>
#include <OMKParametersAccessor.h>
#include <OMKParametersAccessor.inl>
#include <OMKTransform.h>
#include <OMKTransformType.h>

#define OVA_DECLARE_TYPE_EVENT(TheType, TheName) \
	typedef OMK::Type::SimpleTypeT < TheType > TheName ## Type; \
	typedef OMK::ValuedEvent < TheName ## Type > TheName ## Event; \
	typedef TheType TheName;

typedef std::pair < int, int > std_pair_int_int;
typedef std::list < double > std_list_double;
OVA_DECLARE_TYPE_EVENT(std_pair_int_int, VrpnButtonState)
OVA_DECLARE_TYPE_EVENT(std_list_double, VrpnAnalogState)
OVA_DECLARE_TYPE_EVENT(Wm4::Vector3f, Position)
OVA_DECLARE_TYPE_EVENT(Wm4::Vector3f, Orientation)
OVA_DECLARE_TYPE_EVENT(Wm4::Vector3f, Scale)

namespace OMK
{
	namespace Type
	{
		inline OMK::IncomingSynchronisationMessage& operator >> (OMK::IncomingSynchronisationMessage& rIncomingSynchronisationMessage, Wm4::Vector3f& rValue)
		{
			rIncomingSynchronisationMessage.get(rValue[0]);
			rIncomingSynchronisationMessage.get(rValue[1]);
			rIncomingSynchronisationMessage.get(rValue[2]);
			return rIncomingSynchronisationMessage;
		}

		inline OMK::OutgoingSynchronisationMessage& operator << (OMK::OutgoingSynchronisationMessage& rOutgoingSynchronisationMessage, const Wm4::Vector3f& rValue)
		{
			rOutgoingSynchronisationMessage.pack(rValue[0]);
			rOutgoingSynchronisationMessage.pack(rValue[1]);
			rOutgoingSynchronisationMessage.pack(rValue[2]);
			return rOutgoingSynchronisationMessage;
		}

		inline std::istream& operator >> (std::istream& rIStream, Wm4::Vector3f& rValue)
		{
			rIStream >> rValue[0];
			rIStream >> rValue[1];
			rIStream >> rValue[2];
			return rIStream;
		}

		inline std::ostream& operator << (std::ostream& rOStream, const Wm4::Vector3f& rValue)
		{
			rOStream << rValue[0] << " ";
			rOStream << rValue[1] << " ";
			rOStream << rValue[2] << " ";
			return rOStream;
		}
	}
}

extern OMK::Name g_sVrpnButtonStateUpdate;
extern OMK::Name g_sVrpnAnalogStateUpdate;

/*
extern OMK::Name g_s3DEntityManipulator_Reset;

extern OMK::Name g_sManipulate3DEntity_Goal;

extern OMK::Name g_sManipulate3DEntity_Translate;
extern OMK::Name g_sManipulate3DEntity_SetPosition;
extern OMK::Name g_sManipulate3DEntity_Rotate;
extern OMK::Name g_sManipulate3DEntity_SetOrientation;
extern OMK::Name g_sManipulate3DEntity_Scale;
extern OMK::Name g_sManipulate3DEntity_SetScale;
extern OMK::Name g_sManipulate3DEntity_Reset;
*/

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVA_OS_Linux
// #define OVA_OS_Windows
// #define OVA_OS_MacOS
// #define OVA_OS_

#if defined TARGET_OS_Windows
 #define OVA_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVA_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVA_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVA_ARCHITECTURE_i386
// #define OVA_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVA_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compilator software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVA_COMPILATOR_GCC
// #define OVA_COMPILATOR_VisualStudio
// #define OVA_COMPILATOR_

#if defined TARGET_COMPILATOR_GCC
 #define OVA_COMPILATOR_GCC
#elif defined TARGET_COMPILATOR_VisualStudio
 #define OVA_COMPILATOR_VisualStudio
#else
 #warning "No target compilator defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// API Definition                                                    //
//___________________________________________________________________//
//                                                                   //

// Taken from
// - http://people.redhat.com/drepper/dsohowto.pdf
// - http://www.nedprod.com/programs/gccvisibility.html
#if defined OVA_Shared
 #if defined OVA_OS_Windows
  #define OVA_API_Export __declspec(dllexport)
  #define OVA_API_Import __declspec(dllimport)
 #elif defined OVA_OS_Linux
  #define OVA_API_Export __attribute__((visibility("default")))
  #define OVA_API_Import __attribute__((visibility("default")))
 #else
  #define OVA_API_Export
  #define OVA_API_Import
 #endif
#else
 #define OVA_API_Export
 #define OVA_API_Import
#endif

#if defined OVA_Exports
 #define OVA_API OVA_API_Export
#else
 #define OVA_API OVA_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif

#endif // __OpenViBEApplication_Defines_H__
