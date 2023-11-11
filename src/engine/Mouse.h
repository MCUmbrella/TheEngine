//
// Created by mcumbrella on 2023/11/11.
//

#ifndef THEENGINE_MOUSE_H
#define THEENGINE_MOUSE_H

class Mouse
{
public:
    int x = 0, y = 0;
    bool buttonsP[5]{};
    bool buttonsH[5]{};
    bool buttonsH_p[5]{};
    bool wheel[4]{};
    bool hidden = false;

    int getX();

    int getY();

    bool lHolding();

    bool mHolding();

    bool rHolding();

    bool x1Holding();

    bool x2Holding();

    bool lPressed();

    bool mPressed();

    bool rPressed();

    bool x1Pressed();

    bool x2Pressed();

    bool wheelUp();

    bool wheelDown();

    bool wheelLeft();

    bool wheelRight();

    bool isHidden();

    bool setHidden(const bool& state);
};

#endif //THEENGINE_MOUSE_H
