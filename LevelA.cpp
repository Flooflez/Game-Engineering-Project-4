/**
* Author: Maximilian Ta
* Assignment: Rise of the AI
* Date due: 2023-11-18, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
*
* Original File created by Sebastián Romero Cruz
**/

#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

unsigned int LEVEL_DATA[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
};

LevelA::~LevelA()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/images/platforms.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA, map_texture_id, 1.0f, 4, 1);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(5.0f, -2.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/player.png");
    
    // Walking
    m_state.player->m_walking[m_state.player->LEFT]  = new int[4] { 1, 5, 9,  13 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.player->m_walking[m_state.player->UP]    = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN]  = new int[4] { 0, 4, 8,  12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start George looking left
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index  = 0;
    m_state.player->m_animation_time   = 0.0f;
    m_state.player->m_animation_cols   = 4;
    m_state.player->m_animation_rows   = 4;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.6f);
    
    // Jumping
    m_state.player->m_jumping_power = 5.0f;
    
    /**
     Enemies' stuff */

    m_state.enemies = new Entity[ENEMY_COUNT];

    //enemy 1
    m_state.enemies[0].m_walking[m_state.enemies[0].LEFT] = new int[4] { 1, 5, 9, 13 };
    m_state.enemies[0].m_walking[m_state.enemies[0].RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.enemies[0].m_walking[m_state.enemies[0].UP] = new int[4] { 2, 6, 10, 14 };
    m_state.enemies[0].m_walking[m_state.enemies[0].DOWN] = new int[4] { 0, 4, 8, 12 };
    m_state.enemies[0].m_animation_indices = m_state.enemies[0].m_walking[m_state.enemies[0].RIGHT];  // start enemy looking left
    m_state.enemies[0].m_animation_frames = 4;
    m_state.enemies[0].m_animation_index = 0;
    m_state.enemies[0].m_animation_time = 0.0f;
    m_state.enemies[0].m_animation_cols = 4;
    m_state.enemies[0].m_animation_rows = 4;
    
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(FLOATER);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = Utility::load_texture("assets/images/floater.png");
    m_state.enemies[0].set_position(glm::vec3(1.0f, -3.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(0.5f);
    m_state.enemies[0].set_width(0.6f);
    m_state.enemies[0].set_height(0.8f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));

    m_state.enemies[0].m_projectile_texture_id = Utility::load_texture("assets/images/projectile.png");


    //enemy 2
    m_state.enemies[1].m_walking[m_state.enemies[1].LEFT] = new int[4] { 1, 5, 9, 13 };
    m_state.enemies[1].m_walking[m_state.enemies[1].RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.enemies[1].m_walking[m_state.enemies[1].UP] = new int[4] { 2, 6, 10, 14 };
    m_state.enemies[1].m_walking[m_state.enemies[1].DOWN] = new int[4] { 0, 4, 8, 12 };
    m_state.enemies[1].m_animation_indices = m_state.enemies[1].m_walking[m_state.enemies[1].RIGHT];  // start enemy looking left
    m_state.enemies[1].m_animation_frames = 4;
    m_state.enemies[1].m_animation_index = 0;
    m_state.enemies[1].m_animation_time = 0.0f;
    m_state.enemies[1].m_animation_cols = 4;
    m_state.enemies[1].m_animation_rows = 4;

    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_ai_type(JUMPER);
    m_state.enemies[1].set_ai_state(ATTACKING);
    m_state.enemies[1].m_texture_id = Utility::load_texture("assets/images/jumper.png");
    m_state.enemies[1].set_position(glm::vec3(2.0f, -4.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_state.enemies[1].set_speed(2.0f);
    m_state.enemies[1].set_width(0.6f);
    m_state.enemies[1].set_height(0.7f);
    m_state.enemies[1].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.enemies[1].m_jumping_power = 5.0f;
    
    //enemy 3
    m_state.enemies[2].m_walking[m_state.enemies[2].LEFT] = new int[4] { 1, 5, 9, 13 };
    m_state.enemies[2].m_walking[m_state.enemies[2].RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.enemies[2].m_walking[m_state.enemies[2].UP] = new int[4] { 2, 6, 10, 14 };
    m_state.enemies[2].m_walking[m_state.enemies[2].DOWN] = new int[4] { 0, 4, 8, 12 };
    m_state.enemies[2].m_animation_indices = m_state.enemies[2].m_walking[m_state.enemies[2].RIGHT];  // start enemy looking left
    m_state.enemies[2].m_animation_frames = 4;
    m_state.enemies[2].m_animation_index = 0;
    m_state.enemies[2].m_animation_time = 0.0f;
    m_state.enemies[2].m_animation_cols = 4;
    m_state.enemies[2].m_animation_rows = 4;

    m_state.enemies[2].set_entity_type(ENEMY);
    m_state.enemies[2].set_ai_type(SHOOTER);
    m_state.enemies[2].set_ai_state(IDLE);
    m_state.enemies[2].m_texture_id = Utility::load_texture("assets/images/shooter.png");
    m_state.enemies[2].set_position(glm::vec3(7.0f, -1.1f, 0.0f));
    m_state.enemies[2].set_movement(glm::vec3(0.0f));
    m_state.enemies[2].set_speed(0.0f);
    m_state.enemies[2].set_width(0.6f);
    m_state.enemies[2].set_height(0.8f);
    m_state.enemies[2].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    m_state.enemies[2].m_projectile_texture_id = Utility::load_texture("assets/images/projectile.png");
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_state.bgm = Mix_LoadMUS("assets/audio/badsong.wav");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(2.3f);
    
    m_state.jump_sfx = Mix_LoadWAV("assets/audio/woop.wav");
}

void LevelA::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
    }
}


void LevelA::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
        m_state.enemies[i].render_projectile(program);
    }
}
