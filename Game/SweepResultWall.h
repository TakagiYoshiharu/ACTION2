#pragma once
struct SweepResultWall : public btCollisionWorld::ConvexResultCallback {
    bool isHit = false;
    Vector3 hitPosition;
    btRigidBody* me = nullptr;
    Vector3 startPos;

    SweepResultWall() {}

    btScalar addSingleResult(btCollisionWorld::LocalConvexResult& result, bool normalInWorldSpace) override {
        if (result.m_hitCollisionObject == me) {
            return 1.0f; // é©ï™é©êgÇ∆ÇÃè’ìÀÇÕñ≥éã
        }

        isHit = true;

        btVector3 hitPointWorld = result.m_hitPointLocal;
        hitPosition.Set(hitPointWorld.getX(), hitPointWorld.getY(), hitPointWorld.getZ());

        return result.m_hitFraction;
    }
};

