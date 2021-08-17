#pragma once

enum Event {
    GAME_START,
    GAME_END,
    EXIT_PROGRAM,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    CREATE_SHAPE_AND_WALL,
    DESTROY_SHAPE_AND_WALL,
    LEVEL_FAILED,
    LEVEL_SUCCESS,
    DISPLAY_SHAPE
};

struct ScheduledEvent {
    Event type;
    double timeRemaining;
};
