#ifndef __DIRECTION__
#define __DIRECTION__

enum JoyStick_Position {
    Middle,
    Top,
    TopRight,
    Right, 
    BottomRight,
    Bottom, 
    BottomLeft,
    Left,
    TopLeft
};

char* direction_text[] = {"Middle", "Top", "Top Right", "Right", "Bottom Right", "Bottom", "Bottom Left", "Left", "Top Left"};

// enum Game_Direction {
//     North, 
//     East,
//     South,
//     West
// };

#endif /* __DIRECTION__ */