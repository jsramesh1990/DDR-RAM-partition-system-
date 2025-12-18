#ifndef GAMING_PARTITION_H
#define GAMING_PARTITION_H

#include "ddr_memory.h"

// Game object structure
typedef struct {
    uint32_t id;
    float position_x;
    float position_y;
    float velocity_x;
    float velocity_y;
    uint32_t health;
    uint32_t score;
    uint32_t texture_id;
} game_object_t;

// Game state structure
typedef struct {
    uint32_t frame_count;
    uint32_t player_count;
    uint32_t active_objects;
    float game_time;
    uint32_t score;
    bool paused;
} game_state_t;

// Gaming functions
void gaming_init(memory_partition_t* partition);
void gaming_load_textures(void);
void gaming_update_physics(void);
void gaming_render_frame(void);
void gaming_process_input(void);
void gaming_calculate_score(void);

// Memory management for gaming
game_object_t* create_game_object(memory_partition_t* partition);
void destroy_game_object(game_object_t* obj);
game_state_t* get_game_state(void);

// Performance monitoring
void gaming_start_frame(void);
void gaming_end_frame(void);
float gaming_get_fps(void);

#endif // GAMING_PARTITION_H
