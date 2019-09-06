#ifndef HMI_INTERFACE_H
#define HMI_INTERFACE_H

extern void HMI_dispaly();

enum WarningState
{
    NORMAL,
    DISTRACT,
    SMOKING,
    PHONING,
    LDW,
    HMW,
    FATIGUE,
    NO_DRIVER,
    NO_CAMERA_COVERED,
    PCW,
    FCW
};

#define FCW_LEVEL_1   4
#define FCW_LEVEL_2   5
#define PCW_LEVEL_1   4
#define PCW_LEVEL_2   5
#define HMW_WARN      3
#define LDW_WARN_L    3
#define LDW_WARN_R    4
#define TIRED_THRESHOLD    1

#endif
