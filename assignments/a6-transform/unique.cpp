#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;

class Unique : public atkui::Framework
{
public:
    Unique() : atkui::Framework(atkui::Perspective) {}
    virtual ~Unique() {}

    virtual void setup()
    {
        lookAt(vec3(200), vec3(0));

        Joint *root = new Joint("root");
        root->setLocalTranslation(vec3(300, 50, 0));
        _octopus.addJoint(root);

        Joint *joint11 = new Joint("joint11");
        joint11->setLocalTranslation(vec3(50, 50, 0));
        _octopus.addJoint(joint11, root);

        Joint *joint12 = new Joint("joint12");
        joint12->setLocalTranslation(vec3(60, 0, 0));
        _octopus.addJoint(joint12, joint11);

        Joint *joint13 = new Joint("joint13");
        joint13->setLocalTranslation(vec3(70, 0, 0));
        _octopus.addJoint(joint13, joint12);

        Joint *joint21 = new Joint("joint21");
        joint21->setLocalTranslation(vec3(50, sqrt(500), sqrt(500)));
        _octopus.addJoint(joint21, root);

        Joint *joint22 = new Joint("joint22");
        joint22->setLocalTranslation(vec3(60, 0, 0));
        _octopus.addJoint(joint22, joint21);

        Joint *joint23 = new Joint("joint23");
        joint23->setLocalTranslation(vec3(70, 0, 0));
        _octopus.addJoint(joint23, joint22);

        Joint *joint31 = new Joint("joint31");
        joint31->setLocalTranslation(vec3(50, 0, 50));
        _octopus.addJoint(joint31, root);

        Joint *joint32 = new Joint("joint32");
        joint32->setLocalTranslation(vec3(60, 0, 0));
        _octopus.addJoint(joint32, joint31);

        Joint *joint33 = new Joint("joint33");
        joint33->setLocalTranslation(vec3(70, 0, 0));
        _octopus.addJoint(joint33, joint32);

        Joint *joint41 = new Joint("joint41");
        joint41->setLocalTranslation(vec3(50, -sqrt(500), sqrt(500)));
        _octopus.addJoint(joint41, root);

        Joint *joint42 = new Joint("joint42");
        joint42->setLocalTranslation(vec3(60, 0, 0));
        _octopus.addJoint(joint42, joint41);

        Joint *joint43 = new Joint("joint43");
        joint43->setLocalTranslation(vec3(70, 0, 0));
        _octopus.addJoint(joint43, joint42);

        Joint *joint51 = new Joint("joint51");
        joint51->setLocalTranslation(vec3(50, -50, 0));
        _octopus.addJoint(joint51, root);

        Joint *joint52 = new Joint("joint52");
        joint52->setLocalTranslation(vec3(60, 0, 0));
        _octopus.addJoint(joint52, joint51);

        Joint *joint53 = new Joint("joint53");
        joint53->setLocalTranslation(vec3(70, 0, 0));
        _octopus.addJoint(joint53, joint52);

        Joint *joint61 = new Joint("joint61");
        joint61->setLocalTranslation(vec3(50, -sqrt(500), -sqrt(500)));
        _octopus.addJoint(joint61, root);

        Joint *joint62 = new Joint("joint62");
        joint62->setLocalTranslation(vec3(60, 0, 0));
        _octopus.addJoint(joint62, joint61);

        Joint *joint63 = new Joint("joint63");
        joint63->setLocalTranslation(vec3(70, 0, 0));
        _octopus.addJoint(joint63, joint62);

        Joint *joint71 = new Joint("joint71");
        joint71->setLocalTranslation(vec3(50, 0, -50));
        _octopus.addJoint(joint71, root);

        Joint *joint72 = new Joint("joint72");
        joint72->setLocalTranslation(vec3(60, 0, 0));
        _octopus.addJoint(joint72, joint71);

        Joint *joint73 = new Joint("joint73");
        joint73->setLocalTranslation(vec3(70, 0, 0));
        _octopus.addJoint(joint73, joint72);

        Joint *joint81 = new Joint("joint81");
        joint81->setLocalTranslation(vec3(50, sqrt(500), -sqrt(500)));
        _octopus.addJoint(joint81, root);

        Joint *joint82 = new Joint("joint82");
        joint82->setLocalTranslation(vec3(60, 0, 0));
        _octopus.addJoint(joint82, joint81);

        Joint *joint83 = new Joint("joint83");
        joint83->setLocalTranslation(vec3(70, 0, 0));
        _octopus.addJoint(joint83, joint82);

        // todo: add more joints
        _octopus.fk(); // compute local2global transforms
    }

    virtual void scene()
    {
        Joint *root = _octopus.getRoot();
        vec3 relpos = root->getLocalTranslation();
        relpos += vec3(elapsedTime() / -100.0f, 0, 0);
        root->setLocalTranslation(relpos);

        setColor(vec3(0, 0, 1));
        drawCube(vec3(0, 0, 1000), vec3(width() * 100, height() * 100, 1));
        drawCube(vec3(1000, 0, 0), vec3(1, height() * 100, width() * 100));
        drawCube(vec3(0, 0, -1000), vec3(width() * 100, height() * 100, 1));
        drawCube(vec3(-1000, 0, 0), vec3(1, height() * 100, width() * 100));
        drawCube(vec3(0, -1000, 0), vec3(width() * 100, 1, height() * 100));
        drawCube(vec3(0, 1000, 0), vec3(height() * 100, 1, width() * 100));
        // todo animate joints
        // hint: use glm::angleAxis(...) to compute quaternions for each joint
        int numJoints = _octopus.getNumJoints();
        float ftime = sin(elapsedTime());
        if (ftime > 0)
        {
            ftime *= -1.0f;
        }

        for (int i = 1; i < numJoints; i++)
        {
            Joint *joint = _octopus.getByID(i);
            int partOfLimb = 1;
            if (joint->getNumChildren() != 0)
            {
                Joint *child = joint->getChildAt(0);
                if (child->getNumChildren() == 0)
                {
                    partOfLimb = -1;
                }
            }
            float Y = 0, Z = 0;
            switch (i / 3)
            {
            case 0:
                Z = 50;
                partOfLimb *= -1;
                break;
            case 1:
                Y = sqrt(500);
                Z = -sqrt(500);
                break;
            case 2:
                Y = 50;
                break;
            case 3:
                Y = sqrt(500);
                Z = sqrt(500);
                break;
            case 4:
                Z = -50;
                partOfLimb *= -1;
                break;
            case 5:
                Y = sqrt(500);
                Z = -sqrt(500);
                partOfLimb *= -1;
                break;
            case 6:
                Y = -50;
                break;
            case 7:
                Y = -sqrt(500);
                Z = -sqrt(500);
                break;
            }
            joint->setLocalRotation(glm::angleAxis(ftime * partOfLimb / 100.0f, vec3(0, Y, Z)));
        }

        _octopus.fk(); // computes local2global transforms
        setColor(vec3(1, 0, 1));

        vec3 pos = root->getGlobalTranslation();
        drawSphere(pos, 100);

        // todo: loop over all joints and draw
        for (int i = 1; i < numJoints; i++)
        {
            Joint *child = _octopus.getByID(i);
            Joint *parent = child->getParent();
            vec3 globalParentPos = parent->getGlobalTranslation();
            vec3 globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 10);
        }
    }

protected:
    Skeleton _octopus;
};

int main(int argc, char **argv)
{
    Unique viewer;
    viewer.run();
}
