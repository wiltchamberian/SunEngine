/*****************************************************************************
* @brief : Character
* ½ÇÉ«
* @author : acedtang
* @date : 2021/8/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __CHARACTER_H
#define __CHARACTER_H

#include "Pawn.h"

namespace Sun {
	class SWorld;
	class Character :public Pawn {
	public:
		//·¢Éä×Óµ¯
		virtual GameObject* spawn();
		void setWorld(SWorld* world) {
			world_ = world;
		}
		bool supportCollideQuery() const override;
		bool needAddToBVH() const override;
	protected:
		SWorld* world_ = nullptr;
	};
}


#endif