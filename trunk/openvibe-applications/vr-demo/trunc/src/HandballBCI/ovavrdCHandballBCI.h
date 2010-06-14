#ifndef __OpenViBEApplication_CHandballBCI_H__
#define __OpenViBEApplication_CHandballBCI_H__

#include "../ovavrdCOgreVRApplication.h"

namespace OpenViBEVRDemos {

	/**
	 * \class CHandballBCI
	 * \author Laurent Bonnet (INRIA/IRISA)
	 * \date 2010-02-16
	 * \brief Ogre application for the Handball application.
	 *
	 * \details The HandballBCI application is based on motor imagery of the hands. 
	 * The 3d scene is a gymnasium, with 2 handball goals.
	 * The subject has to move his right/left hand to move a ball in the right/left goal.
	 * TODO: full documentation.
	 */
	class CHandballBCI : public COgreVRApplication
	{
		public:

			CHandballBCI();

		private:

			virtual bool initialise(void);
	
			virtual bool process(double timeSinceLastProcess);	
			
			int m_iGoalScore;
			int m_iSideScore;
			int m_iClassificationScore;
			int m_iFeedbackLoopCount;
			int m_iPhase;
			int m_iLastPhase;
			int m_iMark;
			int m_iLastMark;

			double m_dFeedback;
			double m_dLastFeedback;

			bool m_bShowMark;
			bool m_bGoalMarkedAtThisPhase;
			bool m_bScoreChanged;
			float m_fBallSpeed;
			float m_fBallPosition;
			float m_fBallOrientation;
			float m_fGoalScoreScale;
			float m_fSideScoreScale;
			float m_fClassificationScoreScale;

			double m_dMinAnalog;
			double m_dMaxAnalog;
			std::list < double > m_vAnalogHistory;

			enum
			{
				Mark_None,
				Mark_Left,
				Mark_Right,
			} EMark;

			enum
			{
				Phase_Passive,
				Phase_GetReady,
				Phase_Active,
			} EPhase;
	};
};
#endif //__OpenViBEApplication_CHandballBCI_H__