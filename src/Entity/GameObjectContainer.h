#ifndef __GAME_OBJECT_CONTAINER_H
#define __GAME_OBJECT_CONTAINER_H

#include <map>
#include "Util/List.h"
#include "Entity/GameObject.h"

namespace Sun {

	//using GameObjectContainer = List<GameObject*>;
	//using GameObjectContainer = std::map<Guid, GameObject*>;
	struct GameObjectContainer :public std::map<UUID, GameObject*> {
		void push_back(GameObject* obj) {
			if(obj)
				insert(std::make_pair(obj->getGuid(), obj));
		}
	};

	//容器选择依据:
	/*
	gameobject的容器选择是一个值得考虑的问题，有以下几种选择:
	1,数组
	因为gameobject可能在游戏过程中动态删除，因此对于频繁删除的场景效率较低
	2,链表
	因为很少需要索引，删除和添加都是O(1)，但有一个致命问题，GameObject的删除
	通常是消息触发GameObject成员函数，由于链表中一般存放GameObject的指针，因此，
	GameObject本身并不知道其在链表中的位置(除非另外存储);导致无法索引到该GameObject
	3,map
	虽然插入删除效率不如链表，但是由于绝大多数场景删除和添加是少数操作。因此可以
	选择
	4，为何容器中一定要存储GameObject指针，因为如果不然的话不能支持GameObject的多态
	实现
	*/
}


#endif