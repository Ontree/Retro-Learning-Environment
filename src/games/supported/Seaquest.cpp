/* *****************************************************************************
 * The lines 57, 110, 119 and 127 are based on Xitari's code, from Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "Seaquest.hpp"

#include "../RomUtils.hpp"
#include <iomanip>

using namespace rle;

SeaquestSettings::SeaquestSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* SeaquestSettings::clone() const { 
    
    RomSettings* rval = new SeaquestSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void SeaquestSettings::step(const RleSystem& system) {

    // update the reward
    reward_t score = getDecimalScore(0xBA, 0xB9, 0xB8, &system);
    m_reward = score - m_score;
    m_score = score;

    // update terminal status
    m_terminal = readRam(&system, 0xA3) != 0;
    m_lives = readRam(&system, 0xBB) + 1;
}


/* is end of game */
bool SeaquestSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t SeaquestSettings::getReward() const { 

    return m_reward; 
}


/* is an action part of the minimal set? */
bool SeaquestSettings::isMinimal(const Action &a) const {

    switch (a) {
    case PLAYER_A | JOYPAD_NOOP:
    case PLAYER_A | JOYPAD_FIRE:
    case PLAYER_A | JOYPAD_UP:
    case PLAYER_A | JOYPAD_RIGHT:
    case PLAYER_A | JOYPAD_LEFT:
    case PLAYER_A | JOYPAD_DOWN:
    case PLAYER_A | JOYPAD_UP | JOYPAD_RIGHT:
    case PLAYER_A | JOYPAD_UP | JOYPAD_LEFT:
    case PLAYER_A | JOYPAD_DOWN | JOYPAD_RIGHT:
    case PLAYER_A | JOYPAD_DOWN | JOYPAD_LEFT:
    case PLAYER_A | JOYPAD_UP | JOYPAD_FIRE:
    case PLAYER_A | JOYPAD_RIGHT | JOYPAD_FIRE:
    case PLAYER_A | JOYPAD_LEFT | JOYPAD_FIRE:
    case PLAYER_A | JOYPAD_DOWN | JOYPAD_FIRE:
    case PLAYER_A | JOYPAD_UP | JOYPAD_RIGHT | JOYPAD_FIRE:
    case PLAYER_A | JOYPAD_UP | JOYPAD_LEFT | JOYPAD_FIRE:
    case PLAYER_A | JOYPAD_DOWN | JOYPAD_RIGHT | JOYPAD_FIRE:
    case PLAYER_A | JOYPAD_DOWN | JOYPAD_LEFT | JOYPAD_FIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void SeaquestSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_lives    = 4;
}


/* saves the state of the rom settings */
void SeaquestSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putInt(m_lives);
}

// loads the state of the rom settings
void SeaquestSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_lives = ser.getInt();
}

