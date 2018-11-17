
//#include "j2DynamicEntity.h"
//#include "j2Collision.h"
//
//j2DynamicEntity::j2DynamicEntity() : j2Entity(type) {
//
//
//
//
//}
//

//void j2DynamicEntity::CheckEntityMovement() {
//
//	if (Speed.x > 0.0f) {
//		MovingRight = true;
//		MovingLeft = false;
//	}
//	else if (Speed.x < 0.0f) {
//		MovingLeft = true;
//		MovingRight = false;
//	}
//	else if (Speed.x == 0.0f) {
//		MovingLeft = false;
//		MovingRight = false;
//	}
//
//	if (landed == true)
//	{
//		Speed.y = 0.0f;
//	}
//	else if (Speed.y < 0.0f) {
//		MovingUp = true;
//		MovingDown = false;
//	}
//	else if (Speed.y > 0.0f) {
//		MovingDown = true;
//		MovingUp = false;
//	}
//	else if (Speed.y == 0.0f) {
//		MovingUp = false;
//		MovingDown = false;
//	}
//
//}
//
//void j2DynamicEntity::SetColliderRespectPivot(bool lookingTo, Collider*col, iPoint CharacterPos, int Displacementx, int Displacementy) {
//
//	if (lookingTo)
//		col->SetPos(CharacterPos.x + Displacementx, CharacterPos.y + Displacementy);
//	else
//		col->SetPos(CharacterPos.x, CharacterPos.y + Displacementy);
//
//
//
//}
