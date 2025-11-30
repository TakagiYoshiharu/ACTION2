/*!
 * @brief	カプセルコライダー。
 */

#include "k2EngineLowPreCompile.h"
#include "Physics.h"
#include "CapsuleCollider.h"

#include "CapsuleCollider.h"
    namespace nsK2EngineLow {

        bool CCapsuleCollider::IsIntersect(const CCapsuleCollider& other) const {
            float dist = (m_position - other.m_position).Length();
            float combinedRadius = m_radius + other.m_radius;
            return dist <= combinedRadius;
        }

    }


