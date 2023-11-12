//
// Created by mcumbrella on 2023/11/11.
//

#ifndef THEENGINE_MOUSE_H
#define THEENGINE_MOUSE_H

class Mouse
{
public:
    int x = 0, y = 0; // the cursor location in the window
    bool buttonsP[5]{}; // pressed buttons at a single tick (calculated at the end of pollSDLEvents)
    bool buttonsH[5]{}; // currently holding buttons. [0]=left, [1]=middle, [2]=right, [3]=X1, [4]=X2
    bool buttonsH_p[5]{}; // buttonsH at previous tick
    bool wheel[4]{}; // the state of the wheel at a single tick. [0]=up, [1]=down, [2]=left, [3]=right
    bool hidden = false; // the visibility of the system cursor

    /**
     * Get the X location of the cursor in the window.
     */
    int getX();

    /**
     * Get the Y location of the cursor in the window.
     */
    int getY();

    /**
     * Check if the user is holding the left button.
     */
    bool lHolding();

    /**
     * Check if the user is holding the middle button.
     */
    bool mHolding();

    /**
     * Check if the user is holding the right button.
     */
    bool rHolding();

    /**
     * Check if the user is holding the X1 button.
     */
    bool x1Holding();

    /**
     * Check if the user is holding the X2 button.
     */
    bool x2Holding();

    /**
     * Check if the user pressed the left button.
     */
    bool lPressed();

    /**
     * Check if the user pressed the middle button.
     */
    bool mPressed();

    /**
     * Check if the user pressed the right button.
     */
    bool rPressed();

    /**
     * Check if the user pressed the X1 button.
     */
    bool x1Pressed();

    /**
     * Check if the user pressed the X2 button.
     */
    bool x2Pressed();

    /**
     * Check if the wheel has been scrolled up.
     */
    bool wheelUp();

    /**
     * Check if the wheel has been scrolled down.
     */
    bool wheelDown();

    /**
     * Check if the wheel has been scrolled left.
     */
    bool wheelLeft();

    /**
     * Check if the wheel has been scrolled right.
     */
    bool wheelRight();

    /**
     * Check if the system cursor has been hidden.
     */
    bool isHidden();

    /**
     * Show or hide the system cursor.
     * @param state true to hide, false to show.
     */
    bool setHidden(const bool& state);
};

#endif //THEENGINE_MOUSE_H
