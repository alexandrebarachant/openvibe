#ifndef __OpenViBE_Kernel_Player_IPlayerManager_H__
#define __OpenViBE_Kernel_Player_IPlayerManager_H__

#include "../ovIKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IPlayer;

		class OV_API IPlayerManager : virtual public OpenViBE::Kernel::IKernelObject
		{
		public:

			virtual OpenViBE::boolean createPlayer(
				OpenViBE::CIdentifier& rPlayerIdentifier)=0;
			virtual OpenViBE::boolean releasePlayer(
				const OpenViBE::CIdentifier& rPlayerIdentifier)=0;
			virtual OpenViBE::Kernel::IPlayer& getPlayer(
				const OpenViBE::CIdentifier& rPlayerIdentifier)=0;
			virtual OpenViBE::CIdentifier getNextPlayerIdentifier(
				const OpenViBE::CIdentifier& rPreviousIdentifier) const=0;

			_IsDerivedFromClass_(OpenViBE::Kernel::IKernelObject, OV_ClassId_Kernel_Player_PlayerManager);
		};
	};
};

#endif // __OpenViBE_Kernel_Player_IPlayerManager_H__
