//  Copyright (C) 2020 Daniel Ward <weluvgoatz@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "object/bumper.hpp"

#include "audio/sound_manager.hpp"
#include "object/player.hpp"
#include "sprite/sprite.hpp"
#include "sprite/sprite_manager.hpp"
#include "supertux/sector.hpp"
#include "util/reader_mapping.hpp"

namespace {
const std::string TRAMPOLINE_SOUND = "sounds/trampoline.wav";
const float BOUNCE_X = -450.0f;
const float BOUNCE_Y = 700.0f;
}

Bumper::Bumper(const ReaderMapping& reader) :
  MovingSprite(reader, "images/objects/trampoline/right_trampoline.sprite", LAYER_OBJECTS, COLGROUP_MOVING),
  physic(),
  left()
{
	reader.get("left", left);
	if (left)
  {
    m_sprite_name = "images/objects/trampoline/left_trampoline.sprite";
    m_default_sprite_name = m_sprite_name;
    m_sprite = SpriteManager::current()->create(m_sprite_name);
  }
	m_sprite->set_action("normal");
	physic.enable_gravity(false);
}
  
ObjectSettings
Bumper::get_settings()
{
  ObjectSettings result = MovingSprite::get_settings();

  result.add_bool(_("Facing Left"), &left, "left", false);

  result.reorder({"left", "sprite", "x", "y"});

  return result;
}
  
void
Bumper::update(float dt_sec)
{
  if (m_sprite->animation_done())
  {
    m_sprite->set_action("normal");
  }
  m_col.m_movement = physic.get_movement (dt_sec);
}

HitResponse
Bumper::collision(GameObject& other, const CollisionHit& hit)
{
  auto player = dynamic_cast<Player*> (&other);
  if (player)
  {
    if (!left)
    {
      player->get_physic().set_velocity(BOUNCE_Y, BOUNCE_X);
      SoundManager::current()->play(TRAMPOLINE_SOUND);
      m_sprite->set_action("swinging", 1);
    }
	  else if (left)
    {
      player->get_physic().set_velocity(-BOUNCE_Y, BOUNCE_X);
	    SoundManager::current()->play(TRAMPOLINE_SOUND);
      m_sprite->set_action("swinging", 1);
    }
  }
	
	auto bumper = dynamic_cast<Bumper*> (&other);
	if (bumper)
  {
    physic.set_velocity_y(0);
	  return FORCE_MOVE;
	}
  return ABORT_MOVE;
}

/* EOF */